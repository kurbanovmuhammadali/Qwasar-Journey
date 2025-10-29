#define _POSIX_C_SOURCE 200809L
#include "tar_ops.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <utime.h>
#include <errno.h>

#define BLOCK_SIZE 512



int create_archive(const char *archive_name, int file_count, char **files) {//in short this is the part where i create arhive files 
    int fd = open(archive_name, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd == -1) {
        perror("my_tar: open");
        return 1;
    }

    for (int i = 0; i < file_count; i++) {
        const char *filename = files[i];
        struct stat st;
        if (stat(filename, &st) == -1) {
            fprintf(stderr, "my_tar: %s: Cannot stat: No such file or directory\n", filename);
            close(fd);
            return 1;
        }
        if (!S_ISREG(st.st_mode)) {
            fprintf(stderr, "my_tar: %s: Not a regular file\n", filename);
            close(fd);
            return 1;
        }

        if (write_header(fd, filename, &st) != 0) {
            fprintf(stderr, "my_tar: Error writing header for %s\n", filename);
            close(fd);
            return 1;
        }

        int infd = open(filename, O_RDONLY);
        if (infd == -1) {
            fprintf(stderr, "my_tar: %s: Cannot open for reading\n", filename);
            close(fd);
            return 1;
        }

        char buf[4096];
        ssize_t r;
        size_t total = 0;
        while ((r = read(infd, buf, sizeof(buf))) > 0) {
            ssize_t w = write(fd, buf, r);
            if (w != r) {
                fprintf(stderr, "my_tar: Error writing data for %s\n", filename);
                close(infd);
                close(fd);
                return 1;
            }
            total += r;
        }
        close(infd);

        if (write_padding(fd, total) != 0) {
            fprintf(stderr, "my_tar: Error writing padding for %s\n", filename);
            close(fd);
            return 1;
        }
    }

    // Write two 512-byte blocks of zeroes to mark end of archive
    char end[BLOCK_SIZE * 2] = {0};
    if (write(fd, end, sizeof(end)) != sizeof(end)) {
        fprintf(stderr, "my_tar: Error writing end of archive\n");
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}


int list_archive(const char *archive_name) {
    int fd = open(archive_name, O_RDONLY);
    if (fd == -1) {
        perror("my_tar: open");
        return 1;
    }

    struct posix_header header;
    while (1) {
        ssize_t r = read(fd, &header, BLOCK_SIZE);
        if (r == 0) break;  // EOF
        if (r != BLOCK_SIZE) {
            fprintf(stderr, "my_tar: Corrupted archive\n");
            close(fd);
            return 1;
        }

        // Check if all zero block -> end
        int all_zero = 1;
        for (int i = 0; i < BLOCK_SIZE; i++) {
            if (((char*)&header)[i] != 0) {
                all_zero = 0;
                break;
            }
        }
        if (all_zero) break;

        // Get size
        unsigned int size = oct_to_uint(header.size, sizeof(header.size));

        char filename[256];
        if (header.prefix[0]) {
            snprintf(filename, sizeof(filename), "%s/%s", header.prefix, header.name);
        } else {
            strncpy(filename, header.name, sizeof(filename));
            filename[sizeof(filename)-1] = '\0';
        }

        printf("%s\n", filename);

        // skip file contents + padding
        off_t offset = size + (BLOCK_SIZE - (size % BLOCK_SIZE)) % BLOCK_SIZE;
        if (lseek(fd, offset, SEEK_CUR) == (off_t)-1) {
            perror("my_tar: lseek");
            close(fd);
            return 1;
        }
    }

    close(fd);
    return 0;
}//is is for the listing the arhive 

int extract_archive(const char *archive_name) {
    int fd = open(archive_name, O_RDONLY);
    if (fd == -1) {
        perror("my_tar: open");
        return 1;
    }

    struct posix_header header;
    while (1) {
        ssize_t r = read(fd, &header, BLOCK_SIZE);
        if (r == 0) break;  // EOF
        if (r != BLOCK_SIZE) {
            fprintf(stderr, "my_tar: Corrupted archive\n");
            close(fd);
            return 1;
        }

        int all_zero = 1;
        for (int i = 0; i < BLOCK_SIZE; i++) {
            if (((char*)&header)[i] != 0) {
                all_zero = 0;
                break;
            }
        }
        if (all_zero) break;

        unsigned int size = oct_to_uint(header.size, sizeof(header.size));

        char filename[256];
        if (header.prefix[0]) {
            snprintf(filename, sizeof(filename), "%s/%s", header.prefix, header.name);
        } else {
            strncpy(filename, header.name, sizeof(filename));
            filename[sizeof(filename)-1] = '\0';
        }

        // Create parent directories
        if (create_parent_dirs(filename) != 0) {
            close(fd);
            return 1;
        }

        int outfd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (outfd == -1) {
            fprintf(stderr, "my_tar: %s: Cannot open for writing\n", filename);
            close(fd);
            return 1;
        }

        size_t remaining = size;
        char buf[4096];
        while (remaining > 0) {
            ssize_t to_read = remaining > sizeof(buf) ? sizeof(buf) : remaining;
            ssize_t rr = read(fd, buf, to_read);
            if (rr <= 0) {
                fprintf(stderr, "my_tar: Unexpected EOF\n");
                close(outfd);
                close(fd);
                return 1;
            }
            ssize_t ww = write(outfd, buf, rr);
            if (ww != rr) {
                fprintf(stderr, "my_tar: Write error\n");
                close(outfd);
                close(fd);
                return 1;
            }
            remaining -= rr;
        }
        close(outfd);

        // Restore mtime
        struct utimbuf times;
        times.actime = times.modtime = oct_to_uint(header.mtime, sizeof(header.mtime));
        utime(filename, &times);

        // skip padding
        off_t pad = (BLOCK_SIZE - (size % BLOCK_SIZE)) % BLOCK_SIZE;
        if (pad > 0) {
            if (lseek(fd, pad, SEEK_CUR) == (off_t)-1) {
                perror("my_tar: lseek");
                close(fd);
                return 1;
            }
        }
    }

    close(fd);
    return 0;
}//for extracting files from or in the archive 

int append_archive(const char *archive_name, int file_count, char **files) {
    int fd = open(archive_name, O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("my_tar: open");
        return 1;
    }

    // Get size of file
    off_t size = lseek(fd, 0, SEEK_END);
    if (size == (off_t)-1) {
        perror("my_tar: lseek");
        close(fd);
        return 1;
    }

    off_t pos = size - BLOCK_SIZE * 2;
    if (pos < 0) pos = 0;

    char zero_block[BLOCK_SIZE] = {0};
    int found_end_marker = 0;

    while (pos >= 0) {
        if (lseek(fd, pos, SEEK_SET) == (off_t)-1) {
            perror("my_tar: lseek");
            close(fd);
            return 1;
        }

        char block1[BLOCK_SIZE], block2[BLOCK_SIZE];
        if (read(fd, block1, BLOCK_SIZE) != BLOCK_SIZE ||
            read(fd, block2, BLOCK_SIZE) != BLOCK_SIZE) {
            break;  // Not enough data — treat as no end marker
        }

        if (memcmp(block1, zero_block, BLOCK_SIZE) == 0 &&
            memcmp(block2, zero_block, BLOCK_SIZE) == 0) {
            found_end_marker = 1;
            break;
        }

        pos -= BLOCK_SIZE;
    }

    if (found_end_marker) {
        // Truncate at the position of first zero block
        if (ftruncate(fd, pos) == -1) {
            perror("my_tar: ftruncate");
            close(fd);
            return 1;
        }
    }

    // Move to end after truncate (or original end if no marker found)
    if (lseek(fd, 0, SEEK_END) == (off_t)-1) {
        perror("my_tar: lseek");
        close(fd);
        return 1;
    }

    // Now write new entries just like in create
    for (int i = 0; i < file_count; i++) {
        const char *filename = files[i];
        struct stat st;
        if (stat(filename, &st) == -1) {
            fprintf(stderr, "my_tar: %s: Cannot stat: No such file or directory\n", filename);
            close(fd);
            return 1;
        }
        if (!S_ISREG(st.st_mode)) {
            fprintf(stderr, "my_tar: %s: Not a regular file\n", filename);
            close(fd);
            return 1;
        }

        if (write_header(fd, filename, &st) != 0) {
            fprintf(stderr, "my_tar: Error writing header for %s\n", filename);
            close(fd);
            return 1;
        }

        int infd = open(filename, O_RDONLY);
        if (infd == -1) {
            fprintf(stderr, "my_tar: %s: Cannot open for reading\n", filename);
            close(fd);
            return 1;
        }

        char buf[4096];
        ssize_t r;
        size_t total = 0;
        while ((r = read(infd, buf, sizeof(buf))) > 0) {
            ssize_t w = write(fd, buf, r);
            if (w != r) {
                fprintf(stderr, "my_tar: Error writing data for %s\n", filename);
                close(infd);
                close(fd);
                return 1;
            }
            total += r;
        }
        close(infd);

        if (write_padding(fd, total) != 0) {
            fprintf(stderr, "my_tar: Error writing padding for %s\n", filename);
            close(fd);
            return 1;
        }
    }

    // Write the two zero blocks at end
    char end[BLOCK_SIZE * 2] = {0};
    if (write(fd, end, sizeof(end)) != sizeof(end)) {
        fprintf(stderr, "my_tar: Error writing end of archive\n");
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}//adding files or directories in arhive 
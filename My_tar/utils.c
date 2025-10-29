#define _POSIX_C_SOURCE 200809L
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
//this is thefile for the functions that are used in the tar_ops.c or the in the main code part 

unsigned int oct_to_uint(char *str, int size) {
    unsigned int val = 0;
    for (int i = 0; i < size && str[i] && str[i] != ' '; i++) {
        if (str[i] < '0' || str[i] > '7') break;
        val = val * 8 + (str[i] - '0');
    }
    return val;
}

void uint_to_oct(unsigned int val, char *str, int size) {
    snprintf(str, size, "%0*o", size - 1, val);
    str[size - 1] = ' ';
}

unsigned int checksum(struct posix_header *header) {
    unsigned char *bytes = (unsigned char *)header;
    unsigned int sum = 0;
    for (int i = 0; i < BLOCK_SIZE; i++) {
        if (i >= 148 && i < 156) sum += ' ';
        else sum += bytes[i];
    }
    return sum;
}

int write_header(int fd, const char *filename, struct stat *st) {
    struct posix_header header;
    memset(&header, 0, sizeof(header));

    if (strlen(filename) > 100) {
        const char *slash = strrchr(filename, '/');
        if (!slash || slash - filename > 155) {
            fprintf(stderr, "my_tar: Filename too long: %s\n", filename);
            return -1;
        }
        size_t prefix_len = slash - filename;
        if (prefix_len > 155) prefix_len = 155;
        memcpy(header.prefix, filename, prefix_len);
        header.prefix[prefix_len] = '\0';
        strncpy(header.name, slash + 1, 100);
    } else {
        strncpy(header.name, filename, 100);
    }

    snprintf(header.mode, sizeof(header.mode), "%07o", st->st_mode & 0777);
    snprintf(header.uid, sizeof(header.uid), "%07o", st->st_uid);
    snprintf(header.gid, sizeof(header.gid), "%07o", st->st_gid);
    snprintf(header.size, sizeof(header.size), "%011o", (unsigned int)st->st_size);
    snprintf(header.mtime, sizeof(header.mtime), "%011o", (unsigned int)st->st_mtime);

    memset(header.chksum, ' ', sizeof(header.chksum));
    header.typeflag = '0';
    memcpy(header.magic, "ustar", 5);
    header.magic[5] = '\0';
    memcpy(header.version, "00", 2);

    unsigned int chksum = checksum(&header);
    snprintf(header.chksum, sizeof(header.chksum), "%06o", chksum);
    header.chksum[6] = '\0';
    header.chksum[7] = ' ';

    ssize_t written = write(fd, &header, BLOCK_SIZE);
    return written == BLOCK_SIZE ? 0 : -1;
}

int write_padding(int fd, size_t size) {
    size_t pad = (BLOCK_SIZE - (size % BLOCK_SIZE)) % BLOCK_SIZE;
    if (pad > 0) {
        char zero[BLOCK_SIZE] = {0};
        ssize_t written = write(fd, zero, pad);
        if (written != (ssize_t)pad) return -1;
    }
    return 0;
}

int ensure_dir_exists(const char *dirpath) {
    struct stat st;
    if (stat(dirpath, &st) == -1) {
        if (mkdir(dirpath, 0755) == -1 && errno != EEXIST) {
            fprintf(stderr, "my_tar: Cannot create directory %s\n", dirpath);
            return -1;
        }
    } else if (!S_ISDIR(st.st_mode)) {
        fprintf(stderr, "my_tar: %s exists and is not a directory\n", dirpath);
        return -1;
    }
    return 0;
}

int create_parent_dirs(const char *path) {
    char *tmp = strdup(path);
    if (!tmp) {
        fprintf(stderr, "my_tar: Memory allocation error\n");
        return -1;
    }
    for (char *p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = 0;
            if (ensure_dir_exists(tmp) != 0) {
                free(tmp);
                return -1;
            }
            *p = '/';
        }
    }
    free(tmp);
    return 0;
}

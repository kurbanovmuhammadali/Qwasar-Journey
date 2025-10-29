#ifndef UTILS_H
#define UTILS_H

#include <sys/stat.h>
#include <stddef.h>   

#define BLOCK_SIZE 512
//this is extinsion for them or in other words to use those functions in other files
struct posix_header {
    char name[100];       //   0
    char mode[8];         // 100
    char uid[8];          // 108
    char gid[8];          // 116
    char size[12];        // 124
    char mtime[12];       // 136
    char chksum[8];       // 148
    char typeflag;        // 156
    char linkname[100];   // 157
    char magic[6];        // 257
    char version[2];      // 263
    char uname[32];       // 265
    char gname[32];       // 297
    char devmajor[8];     // 329
    char devminor[8];     // 337
    char prefix[155];     // 345
    char padding[12];     // 500
};

unsigned int oct_to_uint(char *str, int size);
void uint_to_oct(unsigned int val, char *str, int size);
unsigned int checksum(struct posix_header *header);
int write_header(int fd, const char *filename, struct stat *st);
int write_padding(int fd, size_t size);
int ensure_dir_exists(const char *dirpath);
int create_parent_dirs(const char *path);

#endif

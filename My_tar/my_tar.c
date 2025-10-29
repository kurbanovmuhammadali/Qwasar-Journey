
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tar_ops.h"

void usage() {
    fprintf(stderr, "Usage: my_tar [-cfxtfr] archive.tar [files...]\n");
    fprintf(stderr, "  -c : create\n");
    fprintf(stderr, "  -x : extract\n");
    fprintf(stderr, "  -t : list\n");
    fprintf(stderr, "  -r : append\n");
    fprintf(stderr, "  -f : archive file\n");
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        usage();
        return 1;
    }

    int c_flag = 0, x_flag = 0, t_flag = 0, r_flag = 0;
    char *archive_name = NULL;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            for (size_t j = 1; j < strlen(argv[i]); j++) {
                switch (argv[i][j]) {
                    case 'c': c_flag = 1; break;
                    case 'x': x_flag = 1; break;
                    case 't': t_flag = 1; break;
                    case 'r': r_flag = 1; break;
                    case 'f':
                        if (i + 1 >= argc) {
                            fprintf(stderr, "my_tar: -f requires an archive filename\n");
                            return 1;
                        }
                        archive_name = argv[++i];
                        goto flags_done;
                    default:
                        fprintf(stderr, "my_tar: Unknown option -%c\n", argv[i][j]);
                        return 1;
                }
            }
        } else {
            break;
        }
    }

flags_done:

    if (!archive_name) {
        fprintf(stderr, "my_tar: Must specify archive with -f\n");
        return 1;
    }

    int action_count = c_flag + x_flag + t_flag + r_flag;
    if (action_count != 1) {
        fprintf(stderr, "my_tar: Exactly one of -c, -x, -t, or -r must be specified\n");
        return 1;
    }

    int files_start = 0;
    for (int i = 1; i < argc; i++) {
        if (argv[i] == archive_name) {
            files_start = i + 1;
            break;
        }
    }

    int file_count = argc - files_start;
    char **files = &argv[files_start];

    if (c_flag) {
        if (file_count < 1) {
            fprintf(stderr, "my_tar: No files specified for creation\n");
            return 1;
        }
        return create_archive(archive_name, file_count, files);
    } else if (x_flag) {
        if (file_count != 0) {
            fprintf(stderr, "my_tar: No files supported for extraction in this implementation\n");
            return 1;
        }
        return extract_archive(archive_name);
    } else if (t_flag) {
        if (file_count != 0) {
            fprintf(stderr, "my_tar: No files supported for listing in this implementation\n");
            return 1;
        }
        return list_archive(archive_name);
    } else if (r_flag) {
        if (file_count < 1) {
            fprintf(stderr, "my_tar: No files specified for append\n");
            return 1;
        }
        return append_archive(archive_name, file_count, files);
    }

    return 0;
}

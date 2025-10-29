#ifndef TAR_OPS_H
#define TAR_OPS_H

int create_archive(const char *archive_name, int file_count, char **files);
int list_archive(const char *archive_name);
int extract_archive(const char *archive_name);
int append_archive(const char *archive_name, int file_count, char **files);

#endif
//IT IS UTILITIS FOR tar_ops.c WHERE THE MAIN FUNCTION OF my_tar.c PRESENT IF THERE wouldn’t BE SO MANY FUNCTION RESTRICTION I THINK I WOULDNT NEED utils.c and *h
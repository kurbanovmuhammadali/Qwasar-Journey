
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
// making a struct for using a little bit later in our code )
typedef struct s_entry {
    char *name;
    char *fullpath;
    struct timespec mtime;
    int is_dir;
} t_entry;
int is_hidden(const char *name) {
    return name[0] == '.';//in this function i find the hidden files or directories such as '.' and etc.
}
void free_entries(t_entry **entries, int count) {//entries is some kind of file names and count is how many files or directories is there
    for (int i = 0; i < count; i++) {
        free(entries[i]->name);
        free(entries[i]->fullpath);//here i mae some spaces for our fututre files or directories, i use it because in the future with malloc it cant give more or less space 
        free(entries[i]);
    }
    free(entries);
}
t_entry *new_entry(const char *path) {//in this function i create a new entry from a path
    t_entry *e = malloc(sizeof(t_entry));
    if (!e) return NULL;

    e->fullpath = strdup(path);//here i dont want to explain every part of the code but shortly in this function i creat or allocate memory with malloc for t_entry files
    if (!e->fullpath) {
        free(e);
        return NULL;
    }

    const char *slash = strrchr(path, '/');
    if (slash && *(slash + 1) != '\0')
        e->name = strdup(slash + 1);
    else if (slash && slash == path)
        e->name = strdup("/");
    else
        e->name = strdup(path);

    if (!e->name) {
        free(e->fullpath);
        free(e);
        return NULL;
    }

    struct stat sb;
    if (lstat(e->fullpath, &sb) == -1) {
        free(e->name);
        free(e->fullpath);
        free(e);
        return NULL;
    }

    e->mtime = sb.st_mtim;
    e->is_dir = S_ISDIR(sb.st_mode);
    return e;
}
// //name of the function says it all but ...
void sort_entries(t_entry **entries, int count, int sort_by_time) {
    for (int i = 1; i < count; i++) {
        t_entry *key = entries[i];//here i sort the t_entry files 
        int j = i - 1;
        while (j >= 0) {//using simple algoritm called insertion sort
            int cmp = 0;
            if (sort_by_time) {
                if (entries[j]->mtime.tv_sec < key->mtime.tv_sec)// but if the sort by time is true then i sort it with mtime() ;)
                    cmp = 1;
                else if (entries[j]->mtime.tv_sec == key->mtime.tv_sec &&
                         entries[j]->mtime.tv_nsec < key->mtime.tv_nsec)
                    cmp = 1;
            } else {
                if (strcmp(entries[j]->name, key->name) > 0)
                    cmp = 1;
            }

            if (!cmp) break;
            entries[j + 1] = entries[j];
            j--;
        }
        entries[j + 1] = key;
    }
}
void print_entries(t_entry **entries, int count) {//this function prints every file in its own line 
    for (int i = 0; i < count; i++)
        printf("%s\n", entries[i]->name);
}
// other things are boring to rxplain just be chill and check the command with gandalf and with the readme instructions
int collect_dir(const char *path, t_entry ***entries, int *count, int show_hidden) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("opendir");
        return -1;
    }
    struct dirent *de;
    int cap = 64;
    *entries = malloc(sizeof(t_entry *) * cap);
    if (!*entries) {
        closedir(dir);
        return -1;
    }
    *count = 0;
    while ((de = readdir(dir))) {
        if (!show_hidden && is_hidden(de->d_name))
            continue;

        size_t len = strlen(path) + strlen(de->d_name) + 2;
        char *full = malloc(len);
        if (!full) continue;

        snprintf(full, len, "%s/%s", path, de->d_name);
        t_entry *e = new_entry(full);
        free(full);
        if (!e) continue;

        if (*count >= cap) {
            cap *= 2;
            t_entry **tmp = realloc(*entries, sizeof(t_entry *) * cap);
            if (!tmp) {
                free_entries(*entries, *count);
                closedir(dir);
                return -1;
            }
            *entries = tmp;
        }
        (*entries)[(*count)++] = e;
    }
    closedir(dir);
    return 0;
}
int main(int argc, char **argv) {
    int opt_a = 0, opt_t = 0;
    int i = 1;
    while (i < argc && argv[i][0] == '-') {
        if (!strcmp(argv[i], "-a")) opt_a = 1;
        else if (!strcmp(argv[i], "-t")) opt_t = 1;
        else if (!strcmp(argv[i], "-at") || !strcmp(argv[i], "-ta")) {
            opt_a = 1;
            opt_t = 1;
        } else break;
        i++;
    }
    if (i == argc) {
        t_entry **entries = NULL;
        int count = 0;
        if (collect_dir(".", &entries, &count, opt_a) == 0) {
            sort_entries(entries, count, opt_t);
            print_entries(entries, count);
            free_entries(entries, count);
        }
    } else {
        int f_count = 0, d_count = 0;
        t_entry **files = malloc(sizeof(t_entry *) * (argc - i));
        t_entry **dirs = malloc(sizeof(t_entry *) * (argc - i));
        while (i < argc) {
            t_entry *e = new_entry(argv[i]);
            if (!e) {
                i++;
                continue;
            }
            if (e->is_dir)
                dirs[d_count++] = e;
            else
                files[f_count++] = e;
            i++;
        }

        sort_entries(files, f_count, opt_t);
        sort_entries(dirs, d_count, opt_t);
        if (f_count > 0)
            print_entries(files, f_count);

        if (f_count > 0 && d_count > 0)
            printf("\n");

        for (int j = 0; j < d_count; j++) {
            if (f_count + d_count > 1)
                printf("%s:\n", dirs[j]->fullpath);

            t_entry **entries = NULL;
            int count = 0;
            if (collect_dir(dirs[j]->fullpath, &entries, &count, opt_a) == 0) {
                sort_entries(entries, count, opt_t);
                print_entries(entries, count);
                free_entries(entries, count);
            }

            if (j < d_count - 1)
                printf("\n");
        }

        free_entries(files, f_count);
        free_entries(dirs, d_count);
    }
    return 0;
}

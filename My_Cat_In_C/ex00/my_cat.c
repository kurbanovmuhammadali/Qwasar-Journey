#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h> 
#define BUFFER_SIZE 1000
int main(int argument, char *param_1[]) {
    char buffer[BUFFER_SIZE];
    int fd, bytes_read;
    if (argument == 1) {
        while ((bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0) {
            write(STDOUT_FILENO, buffer, bytes_read);
        }

    } else {
        for (int i = 1; i < argument; i++) {
            fd = open(param_1[i], O_RDONLY);
            if (fd == -1) {
                write(STDERR_FILENO, "Error opening file: ", 19);
                write(STDERR_FILENO, param_1[i], strlen(param_1[i]));
                write(STDERR_FILENO, "\n", 1);
                continue;
            }
            while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
                write(STDOUT_FILENO, buffer, bytes_read);
            }
            close(fd);
        }
    }

    return 0;
}
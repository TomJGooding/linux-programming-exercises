#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    int append = 0;
    int invalid_opt = 0;
    int opt = 0;
    while ((opt = getopt(argc, argv, "a")) != -1) {
        switch (opt) {
            case 'a':
                append = 1;
                break;
            default:
                invalid_opt = 1;
                break;
        }
    }

    if (invalid_opt || argc != optind + 1) {
        fprintf(stderr, "Usage: %s [-a] FILE\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *file_path = argv[optind];

    int flags = O_CREAT | O_WRONLY | (append ? O_APPEND : O_TRUNC);
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP;
    int fd = open(file_path, flags, mode);
    if (fd == -1) {
        perror(file_path);
        exit(EXIT_FAILURE);
    }

    char buf[BUF_SIZE];
    ssize_t n_read = 0;
    while ((n_read = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
        if (write(fd, buf, n_read) != n_read) {
            perror(file_path);
            exit(EXIT_FAILURE);
        }
        if (write(STDOUT_FILENO, buf, n_read) != n_read) {
            perror("stdout");
            exit(EXIT_FAILURE);
        }
    }
    if (n_read == -1) {
        perror("stdin");
        exit(EXIT_FAILURE);
    }

    if (close(fd) == -1) {
        perror(file_path);
        exit(EXIT_FAILURE);
    }
}

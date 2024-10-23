#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s SOURCE DEST\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *src_path = argv[1];
    char *dest_path = argv[2];

    int src_fd = open(src_path, O_RDONLY);
    if (src_fd == -1) {
        perror(src_path);
        exit(EXIT_FAILURE);
    }

    int dest_flags = O_CREAT | O_WRONLY | O_TRUNC;
    mode_t dest_mode = S_IRUSR | S_IWUSR | S_IRGRP;
    int dest_fd = open(dest_path, dest_flags, dest_mode);
    if (dest_fd == -1) {
        perror(dest_path);
        exit(EXIT_FAILURE);
    }

    char buf[BUF_SIZE];
    ssize_t n_read = 0;
    while ((n_read = read(src_fd, buf, BUF_SIZE)) > 0) {
        if (write(dest_fd, buf, n_read) != n_read) {
            perror(dest_path);
            exit(EXIT_FAILURE);
        }
    }
    if (n_read == -1) {
        perror(src_path);
        exit(EXIT_FAILURE);
    }

    if (close(src_fd) == -1) {
        perror(src_path);
        exit(EXIT_FAILURE);
    }
    if (close(dest_fd) == -1) {
        perror(dest_path);
        exit(EXIT_FAILURE);
    }
}

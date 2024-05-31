#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // 添加这一行
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int pipefd[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // Child process
        close(pipefd[1]); // Close unused write end of the pipe

        int destination_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (destination_fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        while ((bytes_read = read(pipefd[0], buffer, BUFFER_SIZE)) > 0) {
            if (write(destination_fd, buffer, bytes_read) != bytes_read) {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }

        close(pipefd[0]);
        close(destination_fd);
        exit(EXIT_SUCCESS);
    } else { // Parent process
        close(pipefd[0]); // Close unused read end of the pipe

        int source_fd = open(argv[1], O_RDONLY);
        if (source_fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        while ((bytes_read = read(source_fd, buffer, BUFFER_SIZE)) > 0) {
            if (write(pipefd[1], buffer, bytes_read) != bytes_read) {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }

        close(pipefd[1]);
        close(source_fd);
        wait(NULL); // Wait for child process to finish
        exit(EXIT_SUCCESS);
    }

    return 0;
}

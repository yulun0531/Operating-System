#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

#define SHARED_MEM_SIZE 64

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct timeval start, end;
    pid_t pid;
    void *shared_memory;
    int *start_time;

    // 创建共享内存
    int shared_mem_fd = shm_open("/time_shared_mem", O_CREAT | O_RDWR, 0666);
    if (shared_mem_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    if (ftruncate(shared_mem_fd, SHARED_MEM_SIZE) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }
    shared_memory = mmap(NULL, SHARED_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shared_mem_fd, 0);
    if (shared_memory == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    close(shared_mem_fd);

    start_time = (int *)shared_memory;

    gettimeofday(&start, NULL);

    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // Child process
        *start_time = start.tv_sec;
        execvp(argv[1], &argv[1]);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else { // Parent process
        wait(NULL);
        gettimeofday(&end, NULL);

        int start_seconds = *start_time;
        int end_seconds = end.tv_sec;
        double elapsed_time = (end_seconds - start_seconds) + (end.tv_usec - start.tv_usec) / 1000000.0;

        printf("Elapsed time: %.6f seconds\n", elapsed_time);

        // 删除共享内存对象
        if (shm_unlink("/time_shared_mem") == -1) {
            perror("shm_unlink");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

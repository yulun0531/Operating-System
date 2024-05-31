#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define MAX 100

int fib[MAX];

void *calculateFibonacci(void *arg) {
    int n = *(int*)arg;
    fib[0] = 0;
    fib[1] = 1;
    for (int i = 2; i < n; i++) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./fibonacci <number_of_fibonacci_numbers>\n");
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0 || n > MAX) {
        printf("Please enter a positive integer less than or equal to %d.\n", MAX);
        return 1;
    }

    pthread_t tid;
    pthread_create(&tid, NULL, calculateFibonacci, &n);
    pthread_join(tid, NULL);

    printf("Fibonacci Sequence:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", fib[i]);
    }
    printf("\n");

    return 0;
}

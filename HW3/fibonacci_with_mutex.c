#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_NUMBERS 1000

int fib_numbers[MAX_NUMBERS]; // 共享资源
int count = 0; // 当前计算的斐波那契数个数
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // 互斥锁

void *calculate_fibonacci(void *arg) {
    int n = *((int*)arg);
    int a = 0, b = 1;
    
    for (int i = 0; i < n; i++) {
        pthread_mutex_lock(&mutex); // 获取锁
        fib_numbers[count++] = a;
        pthread_mutex_unlock(&mutex); // 释放锁
        int temp = a;
        a = b;
        b = temp + b;
    }
    pthread_exit(NULL);
}

int main() {
    int n;
    printf("Enter the number of Fibonacci numbers to generate: ");
    scanf("%d", &n);
    
    pthread_t tid;
    pthread_create(&tid, NULL, calculate_fibonacci, &n);
    
    pthread_join(tid, NULL); // 等待子线程结束
    
    // 父线程读取斐波那契数列
    pthread_mutex_lock(&mutex); // 获取锁
    printf("Fibonacci numbers:\n");
    for (int i = 0; i < count; i++) {
        printf("%d ", fib_numbers[i]);
    }
    printf("\n");
    pthread_mutex_unlock(&mutex); // 释放锁
    
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define TOTAL_POINTS 1000000  // 總點數
#define NUM_THREADS 4  // 子線程數量

int points_in_circle = 0;  // 圓內點的計數
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;  // 用於多線程中的鎖

// 生成隨機點並計算點在圓內的數量
void *generate_points(void *arg) {
    int points_per_thread = TOTAL_POINTS / NUM_THREADS;
    for (int i = 0; i < points_per_thread; i++) {
        double x = (double)rand() / RAND_MAX * 2 - 1;  // 在[-1, 1]內生成x坐標
        double y = (double)rand() / RAND_MAX * 2 - 1;  // 在[-1, 1]內生成y坐標
        if (x*x + y*y <= 1) {  // 如果點在圓內
            pthread_mutex_lock(&lock);
            points_in_circle++;
            pthread_mutex_unlock(&lock);
        }
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    srand(time(NULL));

    // 創建子線程來生成點並計算點在圓內的數量
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, generate_points, NULL);
    }

    // 等待所有子線程完成
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // 計算π的估計值
    double pi_estimate = 4 * (double)points_in_circle / TOTAL_POINTS;
    printf("Estimated value of pi: %f\n", pi_estimate);

    return 0;
}

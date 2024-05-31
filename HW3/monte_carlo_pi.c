#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <stdbool.h>

#define TOTAL_POINTS 1000000
#define NUM_THREADS 4

int points_in_circle = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* generate_points(void* arg) {
    int points_per_thread = TOTAL_POINTS / NUM_THREADS;
    int points_in_circle_thread = 0;

    for (int i = 0; i < points_per_thread; ++i) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        double distance = sqrt(x * x + y * y);

        if (distance <= 1.0) {
            ++points_in_circle_thread;
        }
    }

    pthread_mutex_lock(&mutex);
    points_in_circle += points_in_circle_thread;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];

    srand((unsigned int)time(NULL));

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_create(&threads[i], NULL, generate_points, NULL);
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    double pi_estimate = 4.0 * ((double)points_in_circle / TOTAL_POINTS);
    printf("Estimate of Pi: %lf\n", pi_estimate);

    return 0;
}

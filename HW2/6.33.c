#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define MAX_RESOURCES 5
int available_resources = MAX_RESOURCES;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int decrease_count(int count) {
    pthread_mutex_lock(&mutex);
    if (available_resources < count) {
        pthread_mutex_unlock(&mutex);
        return -1;
    }
    else {
        available_resources -= count;
        pthread_mutex_unlock(&mutex);
        return 0;
    }
}

int increase_count(int count) {
    pthread_mutex_lock(&mutex);
    available_resources += count;
    pthread_mutex_unlock(&mutex);
    return 0;
}
int take_resource(int count) {
    if (pthread_mutex_lock(&mutex) != 0) {
        return -1; // Failed to acquire lock
    }
    if (available_resources < count || count <= 0) {
        pthread_mutex_unlock(&mutex);
        return -1;
    }
    else {
        if (available_resources - count < 0) {
            count = available_resources; // Take maximum available resources
        }
        available_resources -= count;
        pthread_mutex_unlock(&mutex);
        return count;
    }
}

int release_resource(int count) {
    pthread_mutex_lock(&mutex);
    available_resources += count;
    pthread_mutex_unlock(&mutex);
    return 0;
}
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <take_count> <release_count>\n", argv[0]);
        return 1;
    }

    int take_count = atoi(argv[1]);
    int release_count = atoi(argv[2]);

    // Test decrease_count
    if (take_resource(take_count) >= 0) {
        printf("Decreasing count by %d\n", take_count);
    } else {
        printf("Failed to decrease count by %d: Not enough resources\n", take_count);
    }
    printf("Available resources after decrease: %d\n", available_resources);

    // Test increase_count
    printf("Increasing count by %d\n", release_count);
    release_resource(release_count);
    printf("Available resources after increase: %d\n", available_resources);

    return 0;
}


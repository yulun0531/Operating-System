#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define MAX_FARMERS 5 // 最多允許在橋上的農民數量

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t northbound_queue = PTHREAD_COND_INITIALIZER;
pthread_cond_t southbound_queue = PTHREAD_COND_INITIALIZER;

int num_northbound_waiting = 0;
int num_southbound_waiting = 0;
int num_farmers_on_bridge = 0;

void cross_bridge(char direction) {
    printf("前往%s的農民正在穿過橋樑。\n", direction == 'N' ? "北行" : "南行");
    // 模擬穿越時間
    sleep(rand() % 3 + 1);
    printf("前往%s的農民已經穿過橋樑。\n", direction == 'N' ? "北行" : "南行");
}

void* northbound_farmer(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        while (num_farmers_on_bridge >= MAX_FARMERS || num_southbound_waiting > 0) {
            num_northbound_waiting++;
            pthread_cond_wait(&northbound_queue, &mutex);
            num_northbound_waiting--;
        }
        num_farmers_on_bridge++;
        pthread_mutex_unlock(&mutex);
        
        cross_bridge('N');
        
        pthread_mutex_lock(&mutex);
        num_farmers_on_bridge--;
        if (num_southbound_waiting > 0)
            pthread_cond_signal(&southbound_queue);
        else if (num_northbound_waiting > 0)
            pthread_cond_broadcast(&northbound_queue);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* southbound_farmer(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        while (num_farmers_on_bridge >= MAX_FARMERS || num_northbound_waiting > 0) {
            num_southbound_waiting++;
            pthread_cond_wait(&southbound_queue, &mutex);
            num_southbound_waiting--;
        }
        num_farmers_on_bridge++;
        pthread_mutex_unlock(&mutex);
        
        cross_bridge('S');
        
        pthread_mutex_lock(&mutex);
        num_farmers_on_bridge--;
        if (num_northbound_waiting > 0)
            pthread_cond_signal(&northbound_queue);
        else if (num_southbound_waiting > 0)
            pthread_cond_broadcast(&southbound_queue);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t northbound_threads[3], southbound_threads[3]; // 創建多個線程

    // 創建北行和南行農民的線程
    for (int i = 0; i < 3; ++i) {
        pthread_create(&northbound_threads[i], NULL, northbound_farmer, NULL);
        pthread_create(&southbound_threads[i], NULL, southbound_farmer, NULL);
    }

    // 等待線程結束
    for (int i = 0; i < 3; ++i) {
        pthread_join(northbound_threads[i], NULL);
        pthread_join(southbound_threads[i], NULL);
    }

    return 0;
}

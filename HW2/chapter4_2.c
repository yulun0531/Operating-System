#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ARRAY_SIZE 8

int array[ARRAY_SIZE] = {4, 2, 7, 1, 3, 5, 6, 8};
int sorted_array[ARRAY_SIZE];

// Compare function used for sorting
int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// Thread function to sort a sublist
void *sort_sublist(void *args) {
    int *sublist = (int *)args;
    qsort(sublist, ARRAY_SIZE / 2, sizeof(int), compare);
    pthread_exit(NULL);
}

// Thread function to merge two sorted sublists
void *merge_sublists(void *args) {
    int *sublist1 = array;
    int *sublist2 = array + ARRAY_SIZE / 2;
    int *merged_array = (int *)args;
    int i = 0, j = 0, k = 0;

    while (i < ARRAY_SIZE / 2 && j < ARRAY_SIZE / 2) {
        if (sublist1[i] < sublist2[j]) {
            merged_array[k++] = sublist1[i++];
        } else {
            merged_array[k++] = sublist2[j++];
        }
    }

    while (i < ARRAY_SIZE / 2) {
        merged_array[k++] = sublist1[i++];
    }

    while (j < ARRAY_SIZE / 2) {
        merged_array[k++] = sublist2[j++];
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t sorting_thread1, sorting_thread2, merging_thread;

    // Create threads to sort two sublists
    pthread_create(&sorting_thread1, NULL, sort_sublist, array);
    pthread_create(&sorting_thread2, NULL, sort_sublist, array + ARRAY_SIZE / 2);

    // Wait for sorting threads to finish
    pthread_join(sorting_thread1, NULL);
    pthread_join(sorting_thread2, NULL);

    // Create thread to merge sorted sublists
    pthread_create(&merging_thread, NULL, merge_sublists, sorted_array);
    pthread_join(merging_thread, NULL);

    // Print sorted merged sublist
    printf("Sorted Merged Sublist: ");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", sorted_array[i]);
    }
    printf("\n");

    return 0;
}

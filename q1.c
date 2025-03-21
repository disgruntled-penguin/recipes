#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define max_number 1000000 // total numbers to sum

int thread_count; // number of threads
long long global_sum = 0; // shared sum across threads
pthread_mutex_t mutex; // mutex for synchronization

// function to measure time in microseconds
long long get_time_in_microseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000LL + tv.tv_usec;
}

// sequential summation
long long sequential_sum() {
    long long sum = 0;
    for (int i = 1; i <= max_number; i++) {
        sum += i;
    }
    return sum;
}

// function for each thread to sum a portion of numbers
void *threaded_sum(void *arg) {
    int thread_id = *(int *)arg;
    long long local_sum = 0;
    
    int start = (max_number / thread_count) * thread_id + 1;
    int end = (max_number / thread_count) * (thread_id + 1);

    for (int i = start; i <= end; i++) {
        local_sum += i;
    }

    pthread_mutex_lock(&mutex);
    global_sum += local_sum;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main() {
    printf("enter number of threads (2, 4, 6, 8, etc.): ");
    scanf("%d", &thread_count);

    pthread_t threads[thread_count];
    int thread_ids[thread_count];
    pthread_mutex_init(&mutex, NULL);

    // measure time for sequential sum
    long long start_time = get_time_in_microseconds();
    long long seq_result = sequential_sum();
    long long end_time = get_time_in_microseconds();
    printf("sequential sum: %lld, time taken: %lld microseconds\n", seq_result, end_time - start_time);

    // measure time for multithreaded sum
    global_sum = 0;
    start_time = get_time_in_microseconds();

    for (int i = 0; i < thread_count; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, threaded_sum, &thread_ids[i]);
    }

    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    end_time = get_time_in_microseconds();
    printf("multithreaded sum: %lld, time taken: %lld microseconds\n", global_sum, end_time - start_time);

    pthread_mutex_destroy(&mutex);
    return 0;
}

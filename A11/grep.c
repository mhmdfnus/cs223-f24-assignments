#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct {
    int thread_id;
    int start_idx;
    int end_idx;
    char **files;
    char *keyword;
    int *line_counts;
    pthread_mutex_t *mutex;
} ThreadData;

void *search_files(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int count = 0;

    // Print the thread's file processing summary
    printf("Thread %d searching %d files (%d to %d)\n",
           data->thread_id,
           data->end_idx - data->start_idx + 1,
           data->start_idx + 3, // Adjust to match example output indices
           data->end_idx + 3); // Adjust to match example output indices

    for (int i = data->start_idx; i <= data->end_idx; i++) {
        FILE *file = fopen(data->files[i], "r");
        if (!file) {
            pthread_mutex_lock(data->mutex);
            printf("Thread %d: Unable to open file %s\n", data->thread_id, data->files[i]);
            pthread_mutex_unlock(data->mutex);
            continue;
        }

        int file_count = 0;
        char line[1024];
        while (fgets(line, sizeof(line), file)) {
            if (strstr(line, data->keyword)) {
                pthread_mutex_lock(data->mutex);
                printf("%d) %s:%s", data->thread_id, data->files[i], line);
                pthread_mutex_unlock(data->mutex);
                file_count++;
            }
        }
        fclose(file);

        pthread_mutex_lock(data->mutex);
        printf("Thread %d found %d lines containing keyword in %s\n",
               data->thread_id, file_count, data->files[i]);
        pthread_mutex_unlock(data->mutex);

        count += file_count;
    }

    data->line_counts[data->thread_id] = count;
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "usage: ./grep <NumThreads> <Keyword> <Files>\n");
        return EXIT_FAILURE;
    }

    int num_threads = atoi(argv[1]);
    char *keyword = argv[2];
    int num_files = argc - 3;
    char **files = &argv[3];

    if (num_threads <= 0) {
        fprintf(stderr, "Number of threads must be positive.\n");
        return EXIT_FAILURE;
    }

    // Print the very first output line
    printf("Searching %d files for keyword: %s\n", num_files, keyword);

    // Dynamically allocate memory for arrays
    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
    ThreadData *thread_data = malloc(num_threads * sizeof(ThreadData));
    int *line_counts = calloc(num_threads, sizeof(int));
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    int files_per_thread = num_files / num_threads;
    int extra_files = num_files % num_threads;

    int file_idx = 0;
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].thread_id = i;
        thread_data[i].start_idx = file_idx;
        thread_data[i].end_idx = file_idx + files_per_thread - 1;
        if (extra_files > 0) {
            thread_data[i].end_idx++;
            extra_files--;
        }
        file_idx = thread_data[i].end_idx + 1;

        thread_data[i].files = files;
        thread_data[i].keyword = keyword;
        thread_data[i].line_counts = line_counts;
        thread_data[i].mutex = &mutex;

        pthread_create(&threads[i], NULL, search_files, &thread_data[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    int total_count = 0;
    for (int i = 0; i < num_threads; i++) {
        total_count += line_counts[i];
        printf("Thread %d found %d lines containing keyword: %s\n",
               i, line_counts[i], keyword);
    }

    printf("Total lines containing keyword '%s': %d\n", keyword, total_count);

    // Free dynamically allocated memory
    free(threads);
    free(thread_data);
    free(line_counts);

    pthread_mutex_destroy(&mutex);
    return EXIT_SUCCESS;
}

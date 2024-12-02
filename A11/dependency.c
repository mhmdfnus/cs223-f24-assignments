/*
Author: Mohammad Fanous
Date: 12/1/2024
this program lists the file dependencies of files in a binary search tree
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h> 
#include "tree.h"

#define MAX_LINE 256

typedef struct {
    int start;
    int end;
    char **files;
} ThreadArgs;

pthread_mutex_t tree_mutex;
struct tree_node *root = NULL;

void parse_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("File open error");
        return;
    }

    char line[MAX_LINE];
    while (fgets(line, MAX_LINE, file)) {
        if (strstr(line, "#include")) {
            char *start = strchr(line, '"');
            if (start) {
                char *end = strchr(start + 1, '"');
                if (end) {
                    *end = '\0';
                    pthread_mutex_lock(&tree_mutex);
                    root = insert(start + 1, root);
                    pthread_mutex_unlock(&tree_mutex);
                }
            }
        }
    }
    fclose(file);
}

void *thread_worker(void *args) {
    ThreadArgs *thread_args = (ThreadArgs *)args;
    for (int i = thread_args->start; i < thread_args->end; i++) {
        parse_file(thread_args->files[i]);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <NumThreads> <Files...>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int num_threads = atoi(argv[1]);
    int num_files = argc - 2;
    char **files = &argv[2];

    printf("Processing %d files\n", num_files);

    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
    ThreadArgs *thread_args = malloc(num_threads * sizeof(ThreadArgs));
    pthread_mutex_init(&tree_mutex, NULL);

    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    int files_per_thread = num_files / num_threads;
    int remaining_files = num_files % num_threads;

    int start = 0;
    for (int i = 0; i < num_threads; i++) {
        int end = start + files_per_thread + (remaining_files > 0 ? 1 : 0);
        remaining_files--;
        thread_args[i] = (ThreadArgs){start, end, files};
        printf("Thread %d processing %d files (arguments %d to %d)\n", i, end - start, start + 2, end + 1);
        pthread_create(&threads[i], NULL, thread_worker, &thread_args[i]);
        start = end;
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end_time, NULL); 
    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                          (end_time.tv_usec - start_time.tv_usec) / 1e6;
    printf("Elapsed time is %.6f seconds\n", elapsed_time);

    char command[MAX_LINE];
    while (1) {
        printf("> ");
        if (!fgets(command, MAX_LINE, stdin)) break;
        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "quit") == 0) {
            break;
        } else if (strcmp(command, "list") == 0) {
            pthread_mutex_lock(&tree_mutex);
            printSorted(root);
            pthread_mutex_unlock(&tree_mutex);
        } else {
            pthread_mutex_lock(&tree_mutex);
            struct tree_node *node = find(command, root);
            pthread_mutex_unlock(&tree_mutex);
            if (node) {
                printf("%s found in the tree.\n", command);
            } else {
                printf("%s not found.\n", command);
            }
        }
    }

    pthread_mutex_destroy(&tree_mutex);
    clear(root); 
    free(threads);
    free(thread_args);
    return EXIT_SUCCESS;
}

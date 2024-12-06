/*
Author: Mohammad Fanous
Date: 12/6/2024
this program is a simplified linked list-based implementation of
malloc and free
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

// Define the chunk structure with the added `used` field
struct chunk {
    int size; // Total size of the block
    int used; // Amount of space currently in use
    struct chunk *next; // Pointer to the next free block
};

struct chunk *flist = NULL; // Free list head

// Memory allocation function
void *malloc(size_t size) {
    if (size == 0) {
        return NULL; // Return NULL if size is 0
    }

    struct chunk *prev = NULL;
    struct chunk *current = flist;

    // First-fit search for a free chunk
    while (current != NULL) {
        if (current->size >= size) {
            // Found a suitable chunk
            if (prev != NULL) {
                prev->next = current->next; // Remove from free list
            } else {
                flist = current->next; // Update free list head
            }

            current->used = size; // Mark the chunk as used
            return (void *)(current + 1); // Return the memory after the header
        }
        prev = current;
        current = current->next;
    }

    // No suitable chunk found, allocate new memory with mmap
    size_t total_size = size + sizeof(struct chunk);
    void *memory = mmap(NULL, total_size, PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (memory == MAP_FAILED) {
        // mmap failed, return NULL
        return NULL;
    }

    struct chunk *new_chunk = (struct chunk *)memory;
    new_chunk->size = size; // Set the size
    new_chunk->used = size; // Set the used space
    new_chunk->next = NULL;

    return (void *)(new_chunk + 1); // Return memory after the header
}

// Memory deallocation function
void free(void *memory) {
    if (memory == NULL) {
        return; // Do nothing for NULL pointers
    }

    // Calculate the header address
    struct chunk *cnk = (struct chunk *)memory - 1;

    cnk->used = 0; // Mark the chunk as free
    cnk->next = flist; // Add it to the free list
    flist = cnk; // Update free list head
}


#include <minishell.h>

size_t get_block_size(void *ptr) {
    if (ptr == NULL)
        return 0;
    return *((size_t *)ptr - 1);
}

void *my_malloc(size_t size) {
    size_t *ptr = (size_t *)malloc(size + sizeof(size_t));
    if (ptr == NULL)
        return NULL;
    *ptr = size;
    return (void *)(ptr + 1);
}

void my_free(void *ptr) {
    if (ptr != NULL)
        free((size_t *)ptr - 1);
}

void *ft_realloc(void *ptr, size_t new_size) {
    void *new_ptr;
    size_t old_size;
    size_t copy_size;

    if (new_size == 0) {
        my_free(ptr);
        return NULL;
    }
    new_ptr = my_malloc(new_size);
    if (new_ptr == NULL)
        return NULL;
    if (ptr != NULL) {
        old_size = get_block_size(ptr);
        copy_size = old_size < new_size ? old_size : new_size;
        ft_memcpy(new_ptr, ptr, copy_size);
        my_free(ptr);
    }
    return new_ptr;
}

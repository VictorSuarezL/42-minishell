#include <minishell.h>

size_t	get_block_size(void *ptr)
{
	if (ptr == NULL)
		return (0);
	return (*((size_t *)ptr - 1));
}

void	*my_malloc(size_t size)
{
	size_t	*ptr;

	ptr = (size_t *)malloc(size + sizeof(size_t));
	if (ptr == NULL)
		return (NULL);
	*ptr = size;
	return ((void *)(ptr + 1));
}

void	my_free(void *ptr)
{
	if (ptr != NULL)
		free((size_t *)ptr - 1);
}

void *ft_realloc(void *ptr, size_t new_size) {
    if (new_size == 0) {
        free(ptr);
        return NULL;
    }

    void *new_ptr = malloc(new_size);
    if (new_ptr == NULL) {
        return NULL;
    }

    if (ptr != NULL) {
        size_t old_size = get_block_size(ptr);
        ft_memcpy(new_ptr, ptr, old_size); // Copy memory using memcpy
        free(ptr);
    }

    return new_ptr;
}


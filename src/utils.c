#include "utils.h"
#include <stdlib.h>

int ft_strlen(char* str)
{
    if (!str)
        return 0;
    int len = 0;

    while (str[len])
        len++;
    return len;
}

void ft_memcpy(char* dest, char* src)
{
    if (!dest || !src)
        return;
    
    
    int i = 0;
    
    while (src[i])
    {
        dest[i] = src[i];
        i++;
    }
}

void free_all(void* ptr, int height)
{
    if (!ptr)
        return;
    void** arr = (void**)ptr;

    for (int i = 0; i < height; i++)
        if (arr[i])
            free(arr[i]);
    
    if (arr)
        free(arr);
}

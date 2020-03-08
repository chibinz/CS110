#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "vector.h"

static void vector_print(Vector* v)
{
    size_t i;

    printf("%ld %ld %ld\n", v->size, v->capacity, v->element_size);
    
    for(i = 0; i < v->size; i++)
    {
        switch(v->element_size)
        {
            case 8: printf("%ld ", ((int64_t *)v->data)[i]); break;
            case 4: printf("%d ", ((int *)v->data)[i]); break;
            case 2: printf("%d ", ((int16_t *)v->data)[i]); break;
            case 1: printf("%d ", ((int8_t *)v->data)[i]); break;
            default: printf("Error, unsupported type");
        }
    }
    printf("\n");
}

static bool less(const void* a, const void* b)
{
    return *((int *) a) < *((int *) b);
}

int main(void)
{
    int a = 1;
    int b = 10;
    int c = 100;

    Vector* v = malloc(sizeof(Vector));
    Vector* w = malloc(sizeof(Vector));

    Vector* x = NULL;

    vector_init(x, 2, 4);

    vector_init(v, 2, sizeof(a));
    vector_copy(w, v);

    vector_push_back(v, &c);
    vector_print(v);
    vector_push_back(v, &b);
    vector_print(v);
    vector_push_front(v, &a);
    vector_print(v);
    vector_insert(v, 1, &b);
    vector_print(v);
    vector_insert(v, 1, &b);
    vector_print(v);
    vector_assign(v, 1, &c);
    vector_print(v);
    vector_sort(v, less);
    vector_print(v);
    vector_pop_back(v);
    vector_print(v);
    vector_pop_front(v);
    vector_print(v);
    vector_erase(v, 2);
    vector_print(v);

    vector_resize(v, 17);
    vector_print(v);

    vector_clear(v);
    vector_print(v);

    vector_destroy(v);
    vector_destroy(w);

    return 0;
}
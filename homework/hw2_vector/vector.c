#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vector.h"

#define check_null(ptr) if(ptr==NULL){return VECTOR_ERROR;}

/***** METHODS *****/
/* For following methods which has return type INT, return VECTOR_ERROR 
   if any error(s) occured, return VECTOR_SUCCESS for operation is done 
   successfully. */
static bool vector_is_full(Vector* vector)
{
    return vector->size == vector->capacity;
}

static void vector_grow(Vector* vector)
{
    void* temp;
    void* new_data;

    /* Grow vector */
    vector->capacity *= VECTOR_GROWTH_FACTOR;
    new_data = malloc(vector->capacity);
    memcpy(new_data, vector->data, vector->size * vector->element_size);

    /* Free old buffer */
    temp = vector->data;
    vector->data = new_data;
    free(temp);
}

static void vector_attempt_grow(Vector* vector)
{
    if(vector_is_full(vector)) vector_grow(vector);
}

/* Return size of vector data in bytes */
static size_t vector_data_size(Vector* vector)
{
    return vector->size * vector->element_size;
}

/* Constructor, set CAPACITY and ELEMENT_SIZE to VECTOR, alloc memory for DATA */
int vector_init(Vector* vector, size_t capacity, size_t element_size)
{
    check_null(vector);

    if(capacity < VECTOR_MINIMUM_CAPACITY) return VECTOR_ERROR;

    vector->size = 0;
    vector->capacity = capacity;
    vector->element_size = element_size;
    vector->data = malloc(capacity * element_size);

    return VECTOR_SUCCESS;
}

/* Copy Constructor, destination should be an **UNINITIALIZED** vector.
   You should copy the memory, instead of changing the pointers only. */
int vector_copy(Vector* destination, Vector* source)
{
    check_null(destination);
    check_null(source);

    if(destination->data != VECTOR_UNINITIALIZED) return VECTOR_ERROR;

    memcpy(destination, source, sizeof(Vector));

    destination->data = malloc(source->capacity * source->element_size);
    memcpy(destination->data, source->data, vector_data_size(source));

    return VECTOR_SUCCESS;
}

/* Destructor, please free all memory allocated */
int vector_destroy(Vector* vector)
{
    check_null(vector);

    free(vector->data);

    return VECTOR_SUCCESS;
}

/* Insertion */
int vector_push_back(Vector* vector, void* element)
{
    check_null(vector);
    check_null(element);

    vector_attempt_grow(vector);

    memcpy((char*)vector->data + vector_data_size(vector), element, vector->element_size);

    vector->size += 1;

    return VECTOR_SUCCESS;
}

int vector_push_front(Vector* vector, void* element)
{
    check_null(vector);
    check_null(element);

    vector_attempt_grow(vector);

    memmove((char*)vector->data + vector->element_size, vector->data, vector_data_size(vector));
    vector->size += 1;

    /* Copy element into buffer */
    memcpy(vector->data, element, vector->element_size);

    return VECTOR_SUCCESS;
}

/* Insert ELEMENT to INDEX, move all elements which index > INDEX right by one time */
int vector_insert(Vector* vector, size_t index, void* element)
{
    char* addr;         /* Address of data corresponding to index */
    size_t move_size;   /* Size of data to be moved right in bytes */

    check_null(vector);
    check_null(element);
    if(index >= vector->size) return VECTOR_ERROR;

    vector_attempt_grow(vector);

    /* Move data to the left by one element */
    addr = vector_get(vector, index);
    move_size = (vector->size - index) * vector->element_size;
    memmove(addr + vector->element_size, addr, move_size);
    vector->size += 1;

    /* Copy element into buffer */
    memcpy(addr, element, vector->element_size); 

    return VECTOR_SUCCESS;
}

/* Just assign VECTOR[INDEX] = ELEMENT, you should not move any elements */
int vector_assign(Vector* vector, size_t index, void* element)
{
    char* addr;

    check_null(vector);
    check_null(element);
    if(index >= vector->size) return VECTOR_ERROR;

    addr = (char*)vector->data + vector->element_size * index;

    /* Copy element into buffer */
    memcpy(addr, element, vector->element_size); 

    return VECTOR_SUCCESS; 
}

/* Deletion */
/* Delete the right-most element */
int vector_pop_back(Vector* vector)
{
    check_null(vector);
    if(vector_is_empty(vector)) return VECTOR_ERROR;

    vector->size -= 1;

    return VECTOR_SUCCESS;
}

/* Delete element at INDEX = 0, move all other elements left by one */
int vector_pop_front(Vector* vector)
{
    check_null(vector);
    if(vector_is_empty(vector)) return VECTOR_ERROR;

    vector->size -= 1;

    memmove(vector->data, (char*)vector->data + vector->element_size, vector_data_size(vector));

    return VECTOR_SUCCESS;
}

/* Delete element at INDEX, move all rhs elements left by one */
int vector_erase(Vector* vector, size_t index)
{
    char* addr;         /* Address of data corresponding to index */
    size_t move_size;   /* Size of data to be moved left in bytes */

    check_null(vector);
    if(index >= vector->size) return VECTOR_ERROR;

    /* Move data to the left by one element */
    addr = vector_get(vector, index);
    move_size = (vector->size - index - 1) * vector->element_size;
    memmove(addr, addr + vector->element_size, move_size);

    vector->size -= 1;

    return VECTOR_SUCCESS;
}

/* Delete all elements in the vector */
int vector_clear(Vector* vector)
{
    check_null(vector);

    vector->size = 0;

    return VECTOR_SUCCESS;
}

/* Lookup */
/* Returns VECTOR[INDEX] */
void* vector_get(Vector* vector, size_t index)
{
    return (char*)vector->data + vector->element_size * index;
}

/* Returns VECTOR[0] */
void* vector_front(Vector* vector)
{
    return vector->data;
}

/* Returns VECTOR[-1] */
void* vector_back(Vector* vector)
{
    return (char*)vector->data + vector_data_size(vector);
}

/* Information */
/* Returns size of VECTOR */
size_t vector_size(const Vector* vector)
{    
    return vector->size;
}

/* Returns TRUE if VECTOR is empty(i.e. size == 0) */
bool vector_is_empty(const Vector* vector)
{
    check_null(vector);

    return vector->size == 0;
}

/* Memory management */
int vector_resize(Vector* vector, size_t new_size)
{
    check_null(vector);
    
    while(vector->capacity < new_size) vector_grow(vector);

    vector->size = new_size;

    return VECTOR_SUCCESS;
}

Iterator vector_begin(Vector* vector)
{
    Iterator i;
    i.element_size = vector->element_size;
    i.pointer = vector_front(vector);

    return i;
}

Iterator vector_end(Vector* vector)
{
    Iterator i;
    i.element_size = vector->element_size;
    i.pointer = vector_back(vector);

    return i;
}

/* Returns an iterator points to vector[index] */
Iterator vector_iterator(Vector* vector, size_t index)
{
    Iterator i;
    i.element_size = vector->element_size;
    i.pointer = vector_get(vector, index);

    return i;
}

/* Returns the element, similar to `operator *()' in C++ */
void* iterator_get(Iterator* iterator)
{
    return iterator->pointer;
}

/* Similar to `operator++()' in C++ */
void iterator_increment(Iterator* iterator)
{
    iterator->pointer = (char*)iterator->pointer + iterator->element_size;
}

/* Similar to `operator--()' in C++ */
void iterator_decrement(Iterator* iterator)
{
    iterator->pointer = (char*)iterator->pointer - iterator->element_size;
}

/* Similar to `operator==(Iterator second)' in C++ */
bool iterator_equals(Iterator* first, Iterator* second)
{
    return first->pointer == second->pointer;
}

static void vector_swap(Vector* vector, size_t a, size_t b)
{
    void* a_addr = vector_get(vector, a);
    void* b_addr = vector_get(vector, b);
    
    void* temp = malloc(vector->element_size);
    memcpy(temp, a_addr, vector->element_size);
    memcpy(a_addr, b_addr, vector->element_size);
    memcpy(b_addr, temp, vector->element_size);
    free(temp);
}

/* Sort the vector in nondecreasing according to comparision function less.
     You can implement any sorting algorithm you want such as bubble sort 
	 or quick sort.	 There is no strict time or space limit. */
void vector_sort(Vector* vector, vector_less_func *less)
{
    size_t i, j;

    /* Insert sort */
    for(i = 1; i < vector->size; i++)
        for(j = i; j > 0; j--)
            if(less(vector_get(vector, j), vector_get(vector, j - 1)))
                vector_swap(vector, j, j - 1);
            else
                break;
}
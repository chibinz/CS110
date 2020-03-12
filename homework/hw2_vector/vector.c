#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vector.h"

/***** METHODS *****/
/* For following methods which has return type INT, return VECTOR_ERROR 
   if any error(s) occured, return VECTOR_SUCCESS for operation is done 
   successfully. */
static bool vector_is_full(Vector* vector)
{
    return vector->size == vector->capacity;
}

/* Return size of vector data in bytes */
static size_t vector_data_size(Vector* vector)
{
    return vector->size * vector->element_size;
}

/* Vector capacity *= 2 */
static void vector_grow(Vector* vector)
{
    /* Declare local variable */
    void* new_data;

    /* Grow vector */
    vector->capacity *= VECTOR_GROWTH_FACTOR;
    new_data = malloc(vector->capacity * vector->element_size);
    memcpy(new_data, vector->data, vector_data_size(vector));

    /* Free old buffer */
    free(vector->data);
    vector->data = new_data;
}

/* Grow if full*/
static void vector_attempt_grow(Vector* vector)
{
    if(vector_is_full(vector)) vector_grow(vector);
}

/* Constructor, set CAPACITY and ELEMENT_SIZE to VECTOR, alloc memory for DATA */
int vector_init(Vector* vector, size_t capacity, size_t element_size)
{
    /* Return early if vector == NULL */
    if(vector == NULL) return VECTOR_ERROR;

    /* Set attribute */
    vector->size = 0;
    vector->capacity = MAX(capacity, VECTOR_MINIMUM_CAPACITY);
    vector->element_size = element_size;

    /* Allocate data */
    vector->data = malloc(vector->capacity * element_size);

    return VECTOR_SUCCESS;
}

/* Copy Constructor, destination should be an **UNINITIALIZED** vector.
   You should copy the memory, instead of changing the pointers only. */
int vector_copy(Vector* destination, Vector* source)
{
    /* Error handling */
    if(destination == NULL) return VECTOR_ERROR;
    if(source == NULL) return VECTOR_ERROR;
    if(source->data == VECTOR_UNINITIALIZED) return VECTOR_ERROR;

    /* Init fields */
    vector_init(destination, source->capacity, source->element_size);
    destination->size = source->size;

    /* Allocate and copy data */
    memcpy(destination->data, source->data, vector_data_size(source));

    return VECTOR_SUCCESS;
}

/* Destructor, please free all memory allocated */
int vector_destroy(Vector* vector)
{
    /* Error handling */
    if(vector == NULL) return VECTOR_ERROR;
    if(vector->data == VECTOR_UNINITIALIZED) return VECTOR_ERROR;

    /* Free and set NULL */
    free(vector->data);
    vector->data = VECTOR_UNINITIALIZED;

    return VECTOR_SUCCESS;
}

/* Insertion */
int vector_push_back(Vector* vector, void* element)
{
    /* Error handling */
    if(vector == NULL) return VECTOR_ERROR;
    if(element == NULL) return VECTOR_ERROR;

    vector_attempt_grow(vector);

    /* Copy data */
    memcpy((char*)vector->data + vector_data_size(vector), element, vector->element_size);

    vector->size += 1;

    return VECTOR_SUCCESS;
}

/* Insert in front */
int vector_push_front(Vector* vector, void* element)
{
    return vector_insert(vector, 0, element);
}

/* Insert ELEMENT to INDEX, move all elements which index > INDEX right by one time */
int vector_insert(Vector* vector, size_t index, void* element)
{
    char* addr;         /* Address of data corresponding to index */
    size_t move_size;   /* Size of data to be moved right in bytes */

    /* Error handling */
    if(vector == NULL) return VECTOR_ERROR;
    if(element == NULL) return VECTOR_ERROR;
    if(index > vector->size) return VECTOR_ERROR;

    /* Grow if needed */
    vector_attempt_grow(vector);

    /* Calculate offset */
    addr = (char*)vector->data + vector->element_size * index; 
    move_size = (vector->size - index) * vector->element_size;
    
    /* Move data to the left by one element */
    memmove(addr + vector->element_size, addr, move_size);
    memcpy(addr, element, vector->element_size); 

    vector->size += 1;

    return VECTOR_SUCCESS;
}

/* Just assign VECTOR[INDEX] = ELEMENT, you should not move any elements */
int vector_assign(Vector* vector, size_t index, void* element)
{
    /* Error handling */
    if(vector == NULL) return VECTOR_ERROR;
    if(element == NULL) return VECTOR_ERROR;
    if(vector->data == VECTOR_UNINITIALIZED) return VECTOR_ERROR;
    if(index >= vector->size) return VECTOR_ERROR;

    /* Copy element into buffer */
    memcpy(vector_get(vector, index), element, vector->element_size); 

    return VECTOR_SUCCESS; 
}

/* Delete the right-most element */
int vector_pop_back(Vector* vector)
{
    /* Error handling */
    if(vector == NULL) return VECTOR_ERROR;
    if(vector->data == VECTOR_UNINITIALIZED) return VECTOR_ERROR;
    if(vector_is_empty(vector)) return VECTOR_ERROR;

    /* Decrement size */
    vector->size -= 1;

    return VECTOR_SUCCESS;
}

/* Delete element at INDEX = 0, move all other elements left by one */
int vector_pop_front(Vector* vector)
{
    return vector_erase(vector, 0);
}

/* Delete element at INDEX, move all rhs elements left by one */
int vector_erase(Vector* vector, size_t index)
{
    char* addr;         /* Address of data corresponding to index */
    size_t move_size;   /* Size of data to be moved left in bytes */

    /* Error handling */
    if(vector == NULL) return VECTOR_ERROR;
    if(vector->data == VECTOR_UNINITIALIZED) return VECTOR_ERROR;
    if(vector_is_empty(vector)) return VECTOR_ERROR;
    if(index >= vector->size) return VECTOR_ERROR;

    /* Move data to the left by one element */
    addr = vector_get(vector, index);
    move_size = (vector->size - index - 1) * vector->element_size;
    memmove(addr, addr + vector->element_size, move_size);

    /* Decrement size */
    vector->size -= 1;

    return VECTOR_SUCCESS;
}

/* Delete all elements in the vector */
int vector_clear(Vector* vector)
{
    /* Error handling */
    if(vector == NULL) return VECTOR_ERROR;
    if(vector->data == VECTOR_UNINITIALIZED) return VECTOR_ERROR;

    vector->size = 0;

    return VECTOR_SUCCESS;
}

/* Lookup */
/* Returns VECTOR[INDEX] */
void* vector_get(Vector* vector, size_t index)
{
    /* Error handling */
    if(vector == NULL) return NULL;
    if(vector->data == VECTOR_UNINITIALIZED) return NULL;
    if(index >= vector->size) return NULL;

    return (char*)vector->data + vector->element_size * index;
}

/* Returns VECTOR[0] */
void* vector_front(Vector* vector)
{
    return vector_get(vector, 0);
}

/* Returns VECTOR[-1] */
void* vector_back(Vector* vector)
{
    return vector_get(vector, vector->size - 1);
}

/* Information */
/* Returns size of VECTOR */
size_t vector_size(const Vector* vector)
{    
    if(vector == NULL || vector->data == VECTOR_UNINITIALIZED) return 0;
    return vector->size;
}

/* Returns TRUE if VECTOR is empty(i.e. size == 0) */
bool vector_is_empty(const Vector* vector)
{
    if(vector == NULL || vector->data == VECTOR_UNINITIALIZED) return false;
    return vector->size == 0;
}

/* Memory management */
int vector_resize(Vector* vector, size_t new_size)
{
    if(vector == NULL) return VECTOR_ERROR;
    if(vector->data == VECTOR_UNINITIALIZED) return VECTOR_ERROR;
    
    while(vector->capacity < new_size) vector_grow(vector);

    /* update size */
    vector->size = new_size;

    return VECTOR_SUCCESS;
}

/* Return vector[0] */
Iterator vector_begin(Vector* vector)
{
    Iterator i;

    /* Error handling */
    if(vector == NULL || vector->data == VECTOR_UNINITIALIZED)
    {
        i.element_size = 0;
        i.pointer = NULL;

        return i;
    }
    /* Set attribute */
    i.element_size = vector->element_size;
    i.pointer = vector_front(vector);

    return i;
}

/* Return vector[-1] */
Iterator vector_end(Vector* vector)
{
    Iterator i;

    /* Error handling */
    if(vector == NULL || vector->data == VECTOR_UNINITIALIZED)
    {
        i.element_size = 0;
        i.pointer = NULL;

        return i;
    }

    /* Set attribute */
    i.element_size = vector->element_size;
    i.pointer = (char*)vector_back(vector) + vector->element_size;

    return i;
}

/* Returns an iterator points to vector[index] */
Iterator vector_iterator(Vector* vector, size_t index)
{
    Iterator i;

    /* Error handling */
    if(vector == NULL || vector->data == VECTOR_UNINITIALIZED)
    {
        i.element_size = 0;
        i.pointer = NULL;

        return i;
    }

    /* Set attribute */
    i.element_size = vector->element_size;
    i.pointer = vector_get(vector, index);

    return i;
}

/* Returns the element, similar to `operator *()' in C++ */
void* iterator_get(Iterator* iterator)
{
    if(iterator == NULL) return NULL;
    return iterator->pointer;
}

/* Similar to `operator++()' in C++ */
void iterator_increment(Iterator* iterator)
{
    if(iterator == NULL) return;
    iterator->pointer = (char*)iterator->pointer + iterator->element_size;
}

/* Similar to `operator--()' in C++ */
void iterator_decrement(Iterator* iterator)
{
    if(iterator == NULL) return;
    iterator->pointer = (char*)iterator->pointer - iterator->element_size;
}

/* Similar to `operator==(Iterator second)' in C++ */
bool iterator_equals(Iterator* first, Iterator* second)
{
    if(first == NULL) return false;
    if(second == NULL) return false;

    /* Comments */
    return first->pointer == second->pointer;
}

static void vector_swap(Vector* vector, size_t a, size_t b)
{
    /* Get element address */
    void* a_addr = vector_get(vector, a);
    void* b_addr = vector_get(vector, b);
    
    /* Allocate temp */
    void* temp = malloc(vector->element_size);

    /* Swap a, b */
    memcpy(temp, a_addr, vector->element_size);
    memcpy(a_addr, b_addr, vector->element_size);
    memcpy(b_addr, temp, vector->element_size);

    /* Free temp */
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
            /* Compare using less */
            if(less(vector_get(vector, j), vector_get(vector, j - 1)))
                vector_swap(vector, j, j - 1);
            else
                break;
}
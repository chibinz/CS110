/* ShanghaiTech University
 *
 * Computer Architecture I
 * 
 * HW 2
 */

#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stddef.h>

/***** DEFINITIONS *****/

/* Minimun capacity for a vector */
#define VECTOR_MINIMUM_CAPACITY 	2

/* When your vector is full, gain its capacity to VECTOR_GROWTH_FACTOR*original_capacity */
#define VECTOR_GROWTH_FACTOR 		2

/* If operations failed, return VECTOR_ERROR, otherwise, return VECTOR_SUCCESS */
#define VECTOR_ERROR 				-1
#define VECTOR_SUCCESS 				0

/* Please use `vector->data != VECTOR_UNINITIALIZED' to check if the vector is initialized */
#define VECTOR_UNINITIALIZED 		NULL

#define MAX(a, b) ((a) > (b) ? (a) : (b))

/***** STRUCTURES *****/
/* Definitions for vector and iterator structure */
typedef struct Vector 
{
	size_t size;
	size_t capacity;
	size_t element_size;
	void* data;
} Vector;

typedef struct Iterator 
{
	void* pointer;
	size_t element_size;
} Iterator;

/***** METHODS *****/
/* For following methods which has return type INT, return VECTOR_ERROR 
   if any error(s) occured, return VECTOR_SUCCESS for operation is done 
   successfully. */

/* Constructor, set CAPACITY and ELEMENT_SIZE to VECTOR, alloc memory for DATA */
int vector_init(Vector* vector, size_t capacity, size_t element_size);

/* Copy Constructor, destination should be an **UNINITIALIZED** vector.
   You should copy the memory, instead of changing the pointers only. */
int vector_copy(Vector* destination, Vector* source);

/* Destructor, please free all memory allocated */
int vector_destroy(Vector* vector);

/* Insertion */
int vector_push_back(Vector* vector, void* element);
int vector_push_front(Vector* vector, void* element);
/* Insert ELEMENT to INDEX, move all elements which index > INDEX right by one time */
int vector_insert(Vector* vector, size_t index, void* element);
/* Just assign VECTOR[INDEX] = ELEMENT, you should not move any elements */
int vector_assign(Vector* vector, size_t index, void* element);

/* Deletion */
/* Delete the right-most element */
int vector_pop_back(Vector* vector);
/* Delete element at INDEX = 0, move all other elements left by one */
int vector_pop_front(Vector* vector);
/* Delete element at INDEX, move all rhs elements left by one */
int vector_erase(Vector* vector, size_t index);
/* Delete all elements in the vector */
int vector_clear(Vector* vector);

/* Lookup */
/* Returns VECTOR[INDEX] */
void* vector_get(Vector* vector, size_t index);
/* Returns VECTOR[0] */
void* vector_front(Vector* vector);
/* Returns VECTOR[-1] */
void* vector_back(Vector* vector);

/* You should not modify this */
#define VECTOR_GET_AS(type, vector_pointer, index) \
	*((type*)vector_get((vector_pointer), (index)))

/* Information */
/* Returns size of VECTOR */
size_t vector_size(const Vector* vector);
/* Returns TRUE if VECTOR is empty(i.e. size == 0) */
bool vector_is_empty(const Vector* vector);

/* Memory management */
int vector_resize(Vector* vector, size_t new_size);

/* Iterators */
Iterator vector_begin(Vector* vector);
Iterator vector_end(Vector* vector);
/* Returns an iterator points to vector[index] */
Iterator vector_iterator(Vector* vector, size_t index);

/* Returns the element, similar to `operator *()' in C++ */
void* iterator_get(Iterator* iterator);
/* You should not modify this */
#define ITERATOR_GET_AS(type, iterator) *((type*)iterator_get((iterator)))

/* Similar to `operator++()' in C++ */
void iterator_increment(Iterator* iterator);
/* Similar to `operator--()' in C++ */
void iterator_decrement(Iterator* iterator);
/* Similar to `operator==(Iterator second)' in C++ */
bool iterator_equals(Iterator* first, Iterator* second);

typedef bool vector_less_func(const void *a, const void *b);

/* Sort the vector in nondecreasing according to comparision function less.
     You can implement any sorting algorithm you want such as bubble sort 
	 or quick sort.	 There is no strict time or space limit. */
void vector_sort(Vector *vector, vector_less_func *less);

#endif /* VECTOR_H */

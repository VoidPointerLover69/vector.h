#ifndef VECTOR_LIB
#define VECTOR_LIB

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//creates a new vector for the given type and a cleanup function
#define defVector(t) \
	typedef struct {\
		t* arr;\
		size_t size;\
		size_t memsize;\
	} vector__##t;\
	void clean_vec_##t(vector__##t** vec) {\
		free((*vec)->arr);\
		free(*vec);\
	}\
	typedef vector__##t* vector_##t;

//using attribute cleanup to automatically free memory 
#define newVector(vect, t) \
	vector__##t* __attribute__((__cleanup__(clean_vec_##t))) vect = malloc(sizeof(vector_##t));\
	vect->arr = malloc(16);\
	vect->size = 0;\
	vect->memsize = 16;

//a variadic macro, using typeof() and __VA_ARGS__ to create an array of arguments given and pushes infrost of vector
#define push_front(vec, ...) \
{\
	typeof(*(vec->arr)) lol[] = {__VA_ARGS__};\
	size_t lols = sizeof(lol)/sizeof(lol[0]);\
	if (vec->memsize <= vec->size) {\
		size_t new_memsize = vec->memsize+lols+sizeof(lol[0]);\
		void* new_arr = realloc(vec->arr, new_memsize);\
		vec->arr = new_arr;\
		vec->memsize = new_memsize;\
	}\
	for (int i = 0; i < lols; i++) {\
		for (int j = vec->size+i; j > 0; j--) {\
			vec->arr[j] = vec->arr[j-1];\
		}\
	}\
	for (int i = 0; i < lols; i++) {\
		vec->arr[i] = lol[i];\
	}\
	vec->size += lols;\
}

//same as push_front but it pushes back
#define push_back(vec, ...) \
{\
	typeof(*(vec->arr)) lol[] = {__VA_ARGS__};\
	size_t lols = sizeof(lol)/sizeof(lol[0]);\
	if (vec->memsize <= vec->size+1) {\
		size_t new_memsize = vec->memsize+lols+sizeof(lol[0]);\
		void* new_arr = realloc(vec->arr, new_memsize);\
		vec->arr = new_arr;\
		vec->memsize = new_memsize;\
	}\
	for (int i = vec->size; i < lols+vec->size; i ++) {\
		vec->arr[i] = lol[i-vec->size];\
	}\
	vec->size += lols;\
}

//concatenates two vectors 
#define vecCat(dest, source) \
	if (dest->memsize < dest->size + source->size) {\
		dest->arr = realloc(dest->arr, dest->size + source->size + sizeof(*source->arr));\
		dest->memsize = source->size + sizeof(*source->arr);\
	}\
	for (int i = dest->size; i <  dest->size + source->size; i++) {\
		dest->arr[i] = source->arr[i - dest->size]; \
	}\
	dest->size += source->size;\

//pushes source infront of dest 
#define vecCatfr(dest, source) \
	if ((dest->size + source->size) > dest->memsize) { \
		dest->arr = realloc(dest->arr, dest->size + source->size + sizeof(*dest->arr));\
		dest->memsize = dest->size + source->size + sizeof(*dest->arr); \
	}\
	for (int i = 0; i < dest->size; i++) {\
		dest->arr[i+source->size] = dest->arr[i]; \
	} \
	for (int i = 0; i < source->size; i++) { \
		dest->arr[i] = source->arr[i]; \
	} \
	dest->size += source->size; \

//swaps two vectors
#define vecSwap(vec1, vec2) \
	if (vec1->memsize < vec2->memsize)\
		vec1->arr = realloc(vec1->arr, vec2->memsize+sizeof(*vec1->arr));\
	else if (vec1->memsize > vec2->memsize)\
		vec2->arr = realloc(vec2->arr, vec1->memsize+sizeof(*vec1->arr));\
	{\
	void* idk = malloc(vec1->memsize);\
	memcpy(idk, vec1->arr, vec1->size*sizeof(vec1->arr[0]));\
	memcpy(vec1->arr, vec2->arr, vec2->size*sizeof(vec1->arr[0])); \
	memcpy(vec2->arr, idk, vec1->size*sizeof(vec1->arr[0]));\
	size_t bug = vec1->size;\
	vec1->size = vec2->size;\
	vec2->size = bug;\
	free(idk);\
	}

#define vecIndex(vec, ind) vec->arr[ind]

#define vecSize(vec) vec->size

#define vecMemsize(vec) vec->memsize

#define vecArr(vec) vec->arr

#define vecPop(vec, ind) \
	if (vec->size != 0) {\
		for(int i = ind; i < vec->size; i++) {\
			vec->arr[i] = vec->arr[i+1];\
		}\
		vec->size--;\
	}
//as far as I've tested it it works though it might not be such a good idea but memcpy was a bitch so it will do until i find a bug

#define vecCopy(vec1, vec2)\
	if (vec2->memsize > vec1->memsize) {\
		vec1->memsize = vec2->memsize;\
		vec1->arr = realloc(vec1->arr, vec1->memsize);\
	}\
	memcpy(vec1->arr, vec2->arr, vec2->size*sizeof(vec2->arr[0]));\
	vec1->size = vec2->size;   
	
#endif

#ifndef VECTOR_LIB
#define VECTOR_LIB

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

#define newVector(vect, t) \
vector_##t* __attribute__((__cleanup__(clean_vec_##t))) vect = malloc(sizeof(vector_##t));\
vect->arr = malloc(16);\
vect->size = 0;\
vect->memsize = 16;\

#define push_back(vec, T) \
if (vec->memsize >= vec->size) {\
    size_t new_memsize = vec->memsize+sizeof(T)+8;\
    void* new_arr = realloc(vec->arr, new_memsize);\
    vec->arr = new_arr;\
    vec->memsize = new_memsize;\
}\
vec->arr[vec->size] = T;\
vec->size++;

#define push_front(vec, T) \
if (vec->memsize >= vec->size) {\
    size_t new_memsize = vec->memsize+sizeof(T)+8;\
    void* new_arr = realloc(vec->arr, new_memsize);\
    vec->arr = new_arr;\
    vec->memsize = new_memsize;\
}\
for (int i = vec->size; i > 0; i--) {\
    vec->arr[i] = vec->arr[i-1];\
}\
vec->arr[0] = (T);\
vec->size++;

#define vecIndex(vec, ind) vec->arr[ind]

#define vecSize(vec) vec->size

#define vecMemsize(vec) vec->memsize

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

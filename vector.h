#ifndef VECTOR_LIB
#define VECTOR_LIB

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define newVector(vec, t) \
typedef struct {\
    t* arr;\
    size_t size;\
    size_t memsize;\
    char* type;\
} vector_##t;\
void clean_vec_##t(vector_##t** vec) {\
    free((*vec)->arr);\
    free((*vec)->type);\
    free(*vec);\
}\
vector_##t* __attribute__((__cleanup__(clean_vec_##t))) vec = malloc(sizeof(vector_##t));\
vec->arr = malloc(16);\
vec->size = 0;\
vec->memsize = 16;\
vec->type = malloc(strlen(#t)+1);\
strcpy(vec->type, #t);

#define push_back(vec, t) \
if (vec->memsize >= vec->size) {\
    size_t new_memsize = vec->memsize+sizeof(t)+8;\
    void* new_arr = realloc(vec->arr, new_memsize);\
    vec->arr = new_arr;\
    vec->memsize = new_memsize;\
}\
vec->arr[vec->size] = t;\
vec->size++;




#endif
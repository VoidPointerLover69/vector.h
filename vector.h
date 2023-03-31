#ifndef VECTOR_LIB
#define VECTOR_LIB

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define newVector(vect, t) \
typedef struct {\
    t* arr;\
    size_t size;\
    size_t memsize;\
    char* type;\
} vector_##vect;\
void clean_vec_##vect(vector_##vect** vec) {\
    free((*vec)->arr);\
    free((*vec)->type);\
    free(*vec);\
}\
vector_##vect* __attribute__((__cleanup__(clean_vec_##vect))) vect = malloc(sizeof(vector_##vect));\
vect->arr = malloc(16);\
vect->size = 0;\
vect->memsize = 16;\
vect->type = malloc(strlen(#t)+1);\
strcpy(vect->type, #t);

#define push_back(vec, T) \
if (vec->memsize >= vec->size) {\
    size_t new_memsize = vec->memsize+sizeof(T)+8;\
    void* new_arr = realloc(vec->arr, new_memsize);\
    vec->arr = new_arr;\
    vec->memsize = new_memsize;\
}\
vec->arr[vec->size] = T;\
vec->size++;




#endif

#ifndef VECTOR_LIB
#define VECTOR_LIB

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

//creates a new vector for the given type and a cleanup function
#define defVector(t) \
	typedef struct {\
		t* arr;\
		size_t size;\
		size_t capacity;\
	} vector__##t;\
	void clean_vec_##t(vector__##t** vec) {\
		free((*vec)->arr);\
		free(*vec);\
	}\
	typedef vector__##t* vector_##t;

//using attribute cleanup to automatically free memory 
#define newVector(vect, t) \
	vector__##t* __attribute__((__cleanup__(clean_vec_##t))) vect = malloc(sizeof(vector_##t));\
	vect->arr = malloc(sizeof(t)*8);\
	vect->size = 0;\
	vect->capacity = 8;

//a variadic macro, using typeof() and __VA_ARGS__ to create an array of arguments given and pushes infrost of vector
#define push_front(vec, ...) {\
		vecType(vec) lol[] = {__VA_ARGS__};\
		size_t lols = sizeof(lol)/sizeof(lol[0]);\
		if (vec->capacity <= vec->size) {\
			size_t new_capacity = vec->capacity+lols*2;\
			vec->arr = realloc(vec->arr, new_capacity*sizeof(lol[0]));\
			vec->capacity = new_capacity;\
		}\
		if (vec->size > 0) {\
			memmove(vec->arr+vec->size, vec->arr, vec->size*sizeof(vecType(vec)));\
			memmove(vec->arr, lol, sizeof(lol));\
		}\
		else {\
			memmove(vec->arr, lol, sizeof(lol));\
		}\
		vec->size += lols;\
}

//same as push_front but it pushes back
#define push_back(vec, ...) {\
		vecType(vec) lol[] = {__VA_ARGS__};\
		size_t lols = sizeof(lol)/sizeof(lol[0]);\
		if (vec->capacity <= vec->size+1) {\
			size_t new_capacity = vec->capacity+lols*2;\
			vec->arr = realloc(vec->arr, new_capacity*sizeof(lol[0]));\
			vec->capacity = new_capacity;\
		}\
		memmove(vec->arr+vec->size, lol, sizeof(lol));\
		vec->size += lols;\
}


#define vecAssign(vec, Start_, End_, T) {\
	if (Start_ == End_)\
		vec->arr[Start_] = T;\
	else {\
		typeof(T) hahaha_t[] = {[0 ... End_-Start_] = T};\
\
		if (vecCapacity(vec)*typeSize(T) < sizeof(hahaha_t)) {\
			vec->arr = realloc(vec->arr, vec->capacity+sizeof(hahaha_t)*2);\
			vec->capacity += sizeof(hahaha_t)/typeSize(T);\
		}\
\
		memmove(vec->arr+Start_, hahaha_t, sizeof(hahaha_t));\
	}\
	if(vecSize(vec) < End_+1)\
		vecSize(vec) = End_+1;\
}

#define vecEmplace(vec, Pos_, ...) {\
	vecType(vec) args_list[] = {__VA_ARGS__};\
	size_t args_size = sizeof(args_list)/sizeof(vecType(vec));\
\
	if (vecCapacity(vec) < args_size+vecSize(vec))\
		vecArr(vec) = realloc(vecArr(vec), (args_size*2+vecSize(vec))*sizeof(vecType(vec)));\
\
	if (Pos_ <= vecSize(vec)) {\
		memmove(vec->arr+Pos_+args_size, vec->arr+Pos_, sizeof(args_list));\
		memmove(vec->arr+Pos_, args_list, sizeof(args_list));\
	}\
\
	vecSize(vec) += args_size;\
}

#define vecReplace(vec, Pos_, ...) {\
	vecType(vec) args_list[] = {__VA_ARGS__};\
	size_t args_size = sizeof(args_list)/sizeof(vecType(vec));\
\
	if (vecCapacity(vec) < args_size+vecSize(vec))\
		vecArr(vec) = realloc(vecArr(vec), (args_size*2+vecSize(vec))*sizeof(vecType(vec)));\
\
	if (Pos_ <= vecSize(vec)) \
		memmove(vec->arr+Pos_, args_list, sizeof(args_list));\
}

//concatenates two vectors 
#define vecCat(dest, source) \
	if (dest->capacity < dest->size + source->size) {\
		dest->arr = realloc(dest->arr, (dest->size + source->size) * sizeof(vecType(source)));\
		dest->capacity = source->size + dest->size;\
	}\
	memmove(dest->arr+dest->size, source->arr, sizeof(vecType(dest))*source->size);\
	dest->size += source->size;\

//pushes source infront of dest 
#define vecCatfr(dest, source) \
	if ((dest->size + source->size) > dest->capacity) { \
		dest->arr = realloc(dest->arr, dest->size + source->size + sizeof(*dest->arr));\
		dest->capacity = dest->size + source->size; \
	}\
	memmove(dest->arr+source->size, dest->arr, dest->size*sizeof(vecType(dest)));\
	memmove(dest->arr, source->arr, source->size*sizeof(vecType(dest)));\
	dest->size += source->size; \

//swaps two vectors
#define vecSwap(vec1, vec2) \
	if (vec1->capacity < vec2->capacity)\
		vec1->arr = realloc(vec1->arr, vec2->capacity+sizeof(*vec1->arr));\
	else if (vec1->capacity > vec2->capacity)\
		vec2->arr = realloc(vec2->arr, vec1->capacity+sizeof(*vec1->arr));\
	\
	{\
		vecType(vec1)* idk = malloc(vec1->capacity);\
\
		memcpy(idk, vec1->arr, vec1->size*sizeof(vec1->arr[0]));\
\
		memcpy(vec1->arr, vec2->arr, vec2->size*sizeof(vec1->arr[0])); \
\
		memcpy(vec2->arr, idk, vec1->size*sizeof(vec1->arr[0]));\
\
		size_t bug = vec1->size;\
		vec1->size = vec2->size;\
		vec2->size = bug;\
		free(idk);\
	}\
			
#define typeSize(_T_) sizeof(typeof(_T_))

#define vecIndex(vec, ind) vec->arr[ind]

#define vecSize(vec) vec->size

#define vecCapacity(vec) vec->capacity

#define vecArr(vec) vec->arr

#define vecType(vec) typeof(*(vec->arr))

#define vecClear(vec) vec->size = 0;

#define typeofVec(vec) typeof(vec)

#define vecPop(vec, ind) {\
	if(ind != 0 && ind <= vec->size && vec->size != 0) {\
		memmove(vec->arr+ind, vec->arr+ind+1, (vec->size-ind)*sizeof(vecType(vec)));\
		vec->size--;\
	}\
}	
//as far as I've tested it it works though it might not be such a good idea but memcpy was a bitch so it will do until i find a bug

#define vecCopy(vec1, vec2)\
	if (vec2->capacity > vec1->capacity) {\
		vec1->capacity = vec2->capacity;\
		vec1->arr = realloc(vec1->arr, vec2->capacity*sizeof(vec2->arr[0]));\
	}\
	memcpy(vec1->arr, vec2->arr, vec2->size*sizeof(vec2->arr[0]));\
	vec1->size = vec2->size;   
	
#endif

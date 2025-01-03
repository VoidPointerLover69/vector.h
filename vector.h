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
		if (vec->capacity < vec->size+sizeof(lols)) {\
			size_t new_capacity = vec->capacity+lols*2;\
			vec->arr = realloc(vec->arr, new_capacity*sizeof(lol[0]));\
			vec->capacity = new_capacity;\
		}\
		if (vec->size > 0)\
			memmove(vec->arr+lols, vec->arr, vec->size*sizeof(vecType(vec)));\
\
		memmove(vec->arr, lol, sizeof(lol));\
		vec->size += lols;\
}

//same as push_front but it pushes back
#define push_back(vec, ...) {\
	vecType(vec) lol[] = {__VA_ARGS__};\
	size_t lols = sizeof(lol)/sizeof(lol[0]);\
\
	if (vec->capacity <= vec->size+lols) {\
		size_t new_capacity = vec->capacity+lols*2;\
\
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
\
		if (vecCapacity(vec) < End_-Start_) {\
			vec->arr = realloc(vec->arr, vec->capacity+(End_-Start_)*2);\
			vec->capacity += End_-Start_+(End_-Start_)*2;\
		}\
\
		for (int i = Start_; i < End_; i++) \
			vec->arr[i] = T;\
\
	}\
\
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
#define vecCatfr(dest, source) {\
	if ((dest->size + source->size) > dest->capacity) { \
		dest->arr = realloc(dest->arr, (dest->size + source->size*2)*sizeof(vecType(dest)));\
		dest->capacity = dest->size + source->size; \
	}\
	if (dest->size > 0)\
		memmove(dest->arr+source->size, dest->arr, dest->size*sizeof(vecType(dest)));\
\
	memmove(dest->arr, source->arr, source->size*sizeof(vecType(dest)));\
	dest->size += source->size; \
}

//swaps two vectors
#define vecSwap(vec1, vec2) {\
	if (vec1->capacity != vec2->capacity) {\
		size_t pp_ = vec1->capacity;\
		vec1->capacity = vec2->capacity;\
		vec2->capacity = pp_;\
	}\
\
		vecType(vec1)* idk = malloc(vec1->capacity*sizeof(vecType(vec1)));\
\
		memcpy(idk, vec1->arr, vec1->size*sizeof(vecType(vec1)));\
\
		memcpy(vec1->arr, vec2->arr, vec2->size*sizeof(vecType(vec1))); \
\
		memcpy(vec2->arr, idk, vec1->size*sizeof(vecType(vec1)));\
\
		size_t bug = vec1->size;\
		vec1->size = vec2->size;\
		vec2->size = bug;\
		free(idk);\
}

#define vecReverse(vec, ...) {\
	size_t lol[] = {0, ##__VA_ARGS__};\
	if (sizeof(lol)/sizeof(size_t) == 3){\
\
		vecType(vec) swap;\
		vecType(vec)* lo = vec->arr;\
		vecType(vec)* hi = vec->arr + (lol[2]-lol[1]);\
\
		while (lo < hi) {\
		swap = *lo;\
		*lo++ = *hi;\
		*hi-- = swap;\
		}\
	}\
	else {\
		vecType(vec) swap;\
		vecType(vec)* lo = vec->arr;\
		vecType(vec)* hi = vec->arr + vecSize(vec)-1;\
\
		while (lo < hi) {\
		swap = *lo;\
		*lo++ = *hi;\
		*hi-- = swap;\
		}\
	}\
}

#define vecIndex(vec, ind) vec->arr[ind]

#define vecSize(vec) vec->size

#define vecCapacity(vec) vec->capacity

#define vecArr(vec) vec->arr

#define vecType(vec) typeof(*(vec->arr))

#define vecClear(vec) vec->size = 0

#define vecPop(vec, ...) {\
	int lol[] = {vecSize(vec), ##__VA_ARGS__};\
	if (sizeof(lol)/sizeof(int) != 1) {\
		if(lol[1] <= vec->size && vec->size != 0) {\
			memmove(vec->arr+lol[1], vec->arr+lol[1]+1, (vec->size-lol[1])*sizeof(vecType(vec)));\
			vec->size--;\
		}\
	}\
	else\
		vecSize(vec)--;\
}	

#define vecCopy(vec1, vec2)\
	if (vec2->capacity > vec1->capacity) {\
		vec1->capacity = vec2->capacity;\
		vec1->arr = realloc(vec1->arr, vec2->capacity*sizeof(vec2->arr[0]));\
	}\
	memcpy(vec1->arr, vec2->arr, vec2->size*sizeof(vec2->arr[0]));\
	vec1->size = vec2->size;   

#define vecLinSearch(vec, T, ret, ...) {\
	ret = -1;\
	int start = 0;\
	int end = vec->size;\
	int lol[] = {0, ##__VA_ARGS__};\
\
	if (sizeof(lol)/sizeof(int) != 1) {\
		start = lol[1];\
		end = lol[2];\
        }\
\
	for (int i = start; i <= end; i++) {\
		if (vec->arr[i] == T)\
			ret = i;\
	}\
}

#define vecBinSearch(vec, T, ret, ...) {\
	ret = -1;\
	int start = 0;\
	int end = vec->size;\
	int lol[] = {0, ##__VA_ARGS__};\
\
	if (sizeof(lol)/sizeof(int) != 1) {\
		start = lol[1];\
		end = lol[2];\
	}\
\
	int mid = 0;\
	while (start <= end) {\
		mid = start + (end-start)/2;\
\
		if (vec->arr[mid] == T) {\
			ret = mid;\
			break;\
		}\
\
		if (vec->arr[mid] < T)\
			start = mid+1;\
		else\
			end = mid-1;\
	}\
}\

#define vecResize(vec, n, ...) {\
	vecType(vec) lol[] = {__VA_ARGS__};\
\
	if(n <= vec->size) {\
		vec->size = n;\
	}\
\
	if (n > vec->capacity) {\
		vecType(vec)* oof = malloc(vec->capacity);\
\
		memcpy(oof, vec->arr, vec->size*sizeof(*oof));\
\
		vec->arr = realloc(vec->arr, n);\
\
		memcpy(vec->arr, oof, vec->size*sizeof(*oof));\
\
		free(oof);\
		vec->capacity = n;\
	}\
\
	if (n > vec->size && sizeof(lol)/sizeof(vecType(vec))) {\
		for (int i = vec->size; i < n; i++) {\
			vec->arr[i] = lol[0];\
		}\
\
		vec->size = n;\
	}\
}

#endif

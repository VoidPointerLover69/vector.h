#include "vector.h"
#include <stdio.h>

typedef char* string; //only works like this because of the pointer character
declVector(string);

void printVector(vector_string* vec) {
	for(int i = 0; i < vecSize(vec); i++) {
		printf("%s ", vecIndex(vec, i));
	}
}

int main() {
	newVector(a, string);
	
	push_front(a, "Hello");
	push_back(a, "...");
	push_back(a, "World!\n");
	push_front(a, "...");
	
	printVector(a);
	
	vecPop(a, vecSize(a)-1);
	printVector(a);
	
	newVector(b, string);
	vecCopy(b, a);
	push_back(b, "World");
	printVector(b);

	return 0;
}
/*
... Hello ... World!
 ... Hello ... ... Hello ... World
 
this should be the output*/

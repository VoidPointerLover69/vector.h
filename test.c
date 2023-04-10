#include "vector.h"

int main() {
    newVector(a, char*);
    push_back(a, "hello");
    push_back(a, ", ");
    push_back(a, "world!");
    newVector(b, char*);
    vecCopy(b, a);

    printf("%s", vecIndex(b, 0));
    printf("%s", vecIndex(b, 1));
    printf("%s\n", vecIndex(b, 2));
    vecPop(b, 1);
    printf("%s", vecIndex(b, 0));
    printf("%s", vecIndex(b, 1));
    return 0;
}
//output should be 
//hello, world!
//helloworld!

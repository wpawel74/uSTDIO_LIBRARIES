#ifndef MM_H
#define MM_H
#include <sys/types.h>

char* mm_malloc(int size);
void mm_free(int* target);

#endif // MM_H

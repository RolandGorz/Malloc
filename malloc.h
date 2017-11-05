#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define malloc(x) my_malloc(x,__FILE__,__LINE__)
#define calloc(x, y) my_calloc(x, y,__FILE__,__LINE__)
#define free(x) my_free(x,__FILE__,__LINE__)
#define realloc(x, y) my_realloc(x, y)

struct MemEntry
{
	char pattern[8];
	struct MemEntry *prev;
	struct MemEntry *succ;
	int isfree;
	int size;
	int linenum;
	char file[20];
};


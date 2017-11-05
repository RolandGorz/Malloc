#include "malloc.h"

#define BLOCKSIZE 1000000
static char big_block[BLOCKSIZE];

void *my_malloc (unsigned int size, char* file, int line) {
	if (size <= 0) {
		printf("The size must be greater than 0 in %s at line number %d\n", file, line);
		exit(0);
	}
	static int initialized = 0;
	static struct MemEntry *root;
	struct MemEntry *p;
	struct MemEntry *succ;
	
	if (!initialized) {
		root = (struct MemEntry *)big_block;
		root->prev = root->succ = 0;
		root ->size = BLOCKSIZE - sizeof(struct MemEntry);
		root->isfree = 1;
		strcpy(root->pattern, "RWG7890");
		root->linenum = 0;
		strcpy(root->file, "\0");
		initialized = 1;	
	} 
	p = root;
	do {
		if (p->size < size) {
			p = p->succ; //too small
		}
		else if (!p->isfree) { //in use
			p = p->succ;
		}
		else if (p->size < (size + sizeof(struct MemEntry))) {
			p->isfree = 0;
			strcpy(p->pattern, "RWG7890");
			p->linenum = line;
			strcpy(p->file, file);
			return (char*)p + sizeof(struct MemEntry);
		}
		else {
			succ = (struct MemEntry*)((char*)p + sizeof(struct MemEntry) + size);
			succ->prev = p;
			succ->succ = p->succ;
			if(p->succ != 0) p->succ->prev = succ;
			p->succ = succ;
			succ->size = p->size = sizeof(struct MemEntry) - size;
			succ->isfree = 1;
			p->size = size;
			p->isfree = 0;
			strcpy(p->pattern, "RWG7890");
			p->linenum = line;
			strcpy(p->file, file);
			return (char *)p + sizeof(struct MemEntry);
		}
	} while (p != 0);
	printf("Insufficent memory within the heap in the file %s on line number %d\n", file, line);
	return 0;
}

void my_free (void *p, char* file, int line) {
	struct MemEntry *ptr;
	struct MemEntry *pred;
	struct MemEntry *succ;

	ptr = (struct MemEntry *) ((char *)p - sizeof(struct MemEntry));
	if (ptr->isfree == 1) {
		printf("Already freed in the file %s at line number %d\n", file, line);
		exit(0);
	}
	if (strcmp(ptr->pattern, "RWG7890") != 0) {
		printf("Data has been found to be corrupted or is not valid allocated memory in the file %s at line number %d\n", file, line);
		return;
	}
	strcpy(ptr->pattern, "\0");
	if ((pred = ptr->prev) != 0 && pred->isfree) {
		pred->size += sizeof(struct MemEntry) + ptr->size;
		pred->succ = ptr->succ;
		if (ptr->succ != 0)
			ptr->succ->prev = pred;
	} else {
		ptr->isfree = 1;
		pred = ptr;
	}
	if ((succ = ptr->succ) != 0 && succ->isfree) {
		pred->size += sizeof(struct MemEntry) + succ->size;
		pred->succ = succ->succ;
		if (succ->succ != 0) 
			succ->succ->prev = pred;
	}
}

void *my_calloc (unsigned int num, unsigned int size, char* file, int line) {
	if (size <= 0) {
		printf("The size must be greater than 0 in %s at line number %d", file, line);
		exit(0);
	}
	static int initialized = 0;
	static struct MemEntry *root;
	struct MemEntry *p;
	struct MemEntry *succ;
	int i = 0;
	if (!initialized) {
		root = (struct MemEntry *)big_block;
		root->prev = root->succ = 0;
		root ->size = BLOCKSIZE - sizeof(struct MemEntry);
		root->isfree = 1;
		strcpy(root->pattern, "RWG7890");
		p->linenum = 0;
		strcpy(p->file, "\0");
		initialized = 1;	
	} 
	p = root;
	do {
		if (p->size < size) {
			p = p->succ; //too small
		}
		else if (!p->isfree) { //in use
			p = p->succ;
		}
		else if (p->size < (size + sizeof(struct MemEntry))) {
			p->isfree = 0;
			for (i = 0; i < num; i++) {
				((char*)p + sizeof(struct MemEntry))[i] = 0;
			}
			strcpy(p->pattern, "RWG7890");
			p->linenum = line;
			strcpy(p->file, file);
			return (char*)p + sizeof(struct MemEntry);
		}
		else {
			succ = (struct MemEntry*)((char*)p + sizeof(struct MemEntry) + size);
			succ->prev = p;
			succ->succ = p->succ;
			if(p->succ != 0) p->succ->prev = succ;
			p->succ = succ;
			succ->size = p->size = sizeof(struct MemEntry) - size;
			succ->isfree = 1;
			p->size = size;
			p->isfree = 0;
			for (i = 0; i < num; i++) {
				((char*)p + sizeof(struct MemEntry))[i] = 0;
			}
			strcpy(p->pattern, "RWG7890");
			p->linenum = line;
			strcpy(p->file, file);
			return (char *)p + sizeof(struct MemEntry);
		}
	} while (p != 0);
	return 0;
}

void *my_realloc(void *ptr, unsigned int num) {
	struct MemEntry *temp = malloc(num);
	*temp = *(struct MemEntry*)ptr;
	free(ptr);
	return temp;
} 

void cleanup() {
	struct MemEntry *ptr = (struct MemEntry*)big_block;
	while (ptr) {
		if (ptr->isfree == 0) {
			printf("Failure to free memory in the file %s at line number %d\n", ptr->file, ptr->linenum );
			ptr = ptr->succ;
		} else {
			ptr = ptr->succ;
		}
	}
}

int main(int argc, char **argv) {
	char * p = (char*)malloc(200);
	free(p);
	free(p);
}



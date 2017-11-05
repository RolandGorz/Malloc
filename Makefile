index: index.c
	gcc -g -Wall -o index index.c

clean:
	rm -f *.o
	rm -f index
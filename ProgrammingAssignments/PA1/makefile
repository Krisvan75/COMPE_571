CFLAGS= -Wall

all: seqWL mthreads mtasks

seqWL: sequential.c 
	gcc $(CFLAGS) -o seqWL sequential.c

mthreads: multithreading.c
	gcc $(CFLAGS) -o mthreads multithreading.c

mtasks: Multitasking.c
	gcc $(CFLAGS) -o mtasks Multitasking.c

clean:
	rm -f seqWL mthreads mtasks

.PHONY: all clean



CFLAGS = -Wall

all: RR, FCFS, MLFQ, SJF

RR: RoundRobin.c
    gcc ${CFLAGS} -o RR RoundRobin.c
FCFS: FCFS.c
    gcc ${CFLAGS} -o FCFS FCFS.c
MMLFQ: MLFQ.c
    gcc ${CFLAGS} -o MLFQ MLFQ.c
SJF: SJF.c
    gcc ${CFLAGS} -o SJF SJF.c

.PHONY: all clean

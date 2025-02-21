
sequential.c: this is Case1, baseline workload, it calculates the summation sequentially in a for loop from 1 to N

multithreading.c: this is Case2, implementation of the workload using multithreading with N and NUM_THREADS as inputs

Multitasking.c: this is Case3, implementation of the workload using multitasking. We opted to use the Option 1 approach where we create NUM_TASKs using fork()

run ./PA1.sh to compile and run all cases with all workloads.
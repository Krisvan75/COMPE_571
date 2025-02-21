/*
Multithreading implementation of WORKLOAD
*/


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

unsigned __int128 total_sum = 0;


void *Workload(void *args){
    unsigned __int128 *thread_args = (unsigned __int128*)args;
    unsigned __int128 local_sum = 0;
    for(unsigned __int128 i=thread_args[0]; i<thread_args[1]; i++){
        local_sum += i;
    }
    thread_args[2] = local_sum;

    pthread_exit(NULL);

}

int main(int argc, const char * argv[]) {
    unsigned long long N = atoll(argv[1]);
    int numThreads = atoi(argv[2]);
    // Create an array of threads
    pthread_t threads[numThreads];
    //clock_t Start, Stop;
    struct timespec start, end;

    unsigned __int128 thread_args[numThreads][3]; //Start index, stop index, and local sum for each thread
    unsigned long long Nrange = N/numThreads;

    for(int i=0; i<numThreads; i++){
        thread_args[i][0] =  i * Nrange;
        thread_args[i][1] = (i==numThreads-1) ? N : ((i+1)*Nrange);
    }


    //Start = clock();
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(int i=0; i<numThreads; i++){
        pthread_create(&threads[i], NULL, Workload, (void *)&thread_args[i]);   
    }

    for(int i=0; i<numThreads; i++){
        pthread_join(threads[i], NULL);
        total_sum += thread_args[i][2];
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    //Stop = clock();
    //printf("Sum: %lld \n",(unsigned long long)total_sum);
    //double timer = (double)(Stop - Start)/CLOCKS_PER_SEC;
    double timer = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Sum of %llu integers with %d threads\tTime taken to compute: %0.8f\n", N,numThreads,timer);
    
    return 0;
    
}
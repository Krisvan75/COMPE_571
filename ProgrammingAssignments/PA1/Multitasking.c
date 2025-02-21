/*
Multitasking implementation of WORKLOAD
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

/*
union{
    unsigned __int128 totalSum;
    unsigned long long output[2];
}i128toLL;
*/

unsigned __int128 totalSum = 0;


int main(int argc, const char *argv[]){
    unsigned long long N = atoll(argv[1]);
    int numProcesses = atoi(argv[2]);
    
    unsigned long long Nrange = N/numProcesses;
    int pipes[numProcesses][2];
    pid_t PIDs[numProcesses];
    //clock_t Start, Stop;
    struct timespec start, end;
    //Start = clock();
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(int i=0;i<numProcesses;i++){
        pipe(pipes[i]);
        PIDs[i] = fork();
        if(PIDs[i] == 0){
            close(pipes[i][0]);
            unsigned __int128 localSum = 0;
            
            unsigned long long start = i*Nrange;
            unsigned long long end = (i==numProcesses-1) ? N : ((i+1)*Nrange);
            
            
            for(unsigned long long j=start; j<end; j++){
                localSum += j;
            }
            write(pipes[i][1], &localSum, sizeof(localSum));
            close(pipes[i][1]);
            exit(0);
        }
    }

    for(int i=0; i<numProcesses; i++){
        unsigned __int128 local_sum;
        read(pipes[i][0], &local_sum, sizeof(local_sum));
        close(pipes[i][0]);
        totalSum += local_sum;
        wait(NULL);
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    //Stop = clock();
    //double timer = (double)(Stop - Start)/CLOCKS_PER_SEC;
    //printf("%llu\n",(unsigned long long)totalSum);
    //printf("Sum: %llu%llu\n",i128toLL.output[0],i128toLL.output[1]);
    double timer = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Sum of %llu integers with %d processes\tTime taken to compute: %0.8f\n", N,numProcesses,timer);
    return 0;
}
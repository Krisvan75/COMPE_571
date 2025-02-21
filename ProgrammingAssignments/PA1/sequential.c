//
//  baselineWORKLOAD
//

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include<time.h>

unsigned __int128 Sum=0;

int main(int argc, const char * argv[]) {
    unsigned long long N = atoll(argv[1]);
    

    //clock_t Start, Stop;
    //Start = clock();
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(unsigned long long i=1; i<N; i++){
        Sum += i;
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    //Stop = clock();
    //printf("Sum: %lld \n",Sum);
    //double timer = (double)(Stop - Start)/CLOCKS_PER_SEC;
    double timer = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Sum of %llu integers\tTime taken to compute: %0.8f\n", N,timer);
}

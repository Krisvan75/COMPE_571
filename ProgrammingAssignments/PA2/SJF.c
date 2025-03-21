#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

/************************************************************************************************ 
        These DEFINE statements represent the workload size of each task.
*************************************************************************************************/

#define WORKLOAD1 100000
#define WORKLOAD2 100000
#define WORKLOAD3 100000
#define WORKLOAD4 100000

/************************************************************************************************ 
                    DO NOT CHANGE THE FUNCTION IMPLEMENTATION
*************************************************************************************************/
void myfunction(int param) {
    int i = 2, j, k;
    while (i < param) {
        k = i;
        for (j = 2; j <= k; j++) {
            if (k % j == 0) {
                k = k / j;
                j--;
                if (k == 1) break;
            }
        }
        i++;
    }
}

/************************************************************************************************/
int main(int argc, char const *argv[]) {
    pid_t pid1, pid2, pid3, pid4;
    int running1, running2, running3, running4;

    pid1 = fork();
    if (pid1 == 0) {
        myfunction(WORKLOAD1);
        exit(0);
    }
    kill(pid1, SIGSTOP);

    pid2 = fork();
    if (pid2 == 0) {
        myfunction(WORKLOAD2);
        exit(0);
    }
    kill(pid2, SIGSTOP);

    pid3 = fork();
    if (pid3 == 0) {
        myfunction(WORKLOAD3);
        exit(0);
    }
    kill(pid3, SIGSTOP);

    pid4 = fork();
    if (pid4 == 0) {
        myfunction(WORKLOAD4);
        exit(0);
    }
    kill(pid4, SIGSTOP);

    /************************************************************************************************ 
            At this point, all newly-created child processes are stopped and ready for scheduling.
    *************************************************************************************************/

    /************************************************************************************************
            - Scheduling code starts here (Shortest Job First)
            - Using running flags and response time measurement
    ************************************************************************************************/

    struct timespec start_time[4], end_time[4];
    double response_time[4];
    double total_response_time = 0.0;  

    running1 = 1;
    running2 = 1;
    running3 = 1;
    running4 = 1;

    // Execute processes SJF scheduling: P4 → P3 → P2 → P1

    if (running4 > 0) {
        clock_gettime(CLOCK_MONOTONIC, &start_time[3]);
        kill(pid4, SIGCONT);
        waitpid(pid4, &running4, 0);
        clock_gettime(CLOCK_MONOTONIC, &end_time[3]);
        response_time[3] = (end_time[3].tv_sec - start_time[3].tv_sec) + 
                           (end_time[3].tv_nsec - start_time[3].tv_nsec) / 1e9;
        total_response_time += response_time[3];
    }

    if (running3 > 0) {
        clock_gettime(CLOCK_MONOTONIC, &start_time[2]);
        kill(pid3, SIGCONT);
        waitpid(pid3, &running3, 0);
        clock_gettime(CLOCK_MONOTONIC, &end_time[2]);
        response_time[2] = (end_time[2].tv_sec - start_time[2].tv_sec) + 
                           (end_time[2].tv_nsec - start_time[2].tv_nsec) / 1e9;
        total_response_time += response_time[2];
    }

    if (running2 > 0) {
        clock_gettime(CLOCK_MONOTONIC, &start_time[1]);
        kill(pid2, SIGCONT);
        waitpid(pid2, &running2, 0);
        clock_gettime(CLOCK_MONOTONIC, &end_time[1]);
        response_time[1] = (end_time[1].tv_sec - start_time[1].tv_sec) + 
                           (end_time[1].tv_nsec - start_time[1].tv_nsec) / 1e9;
        total_response_time += response_time[1];
    }

    if (running1 > 0) {
        clock_gettime(CLOCK_MONOTONIC, &start_time[0]);
        kill(pid1, SIGCONT);
        waitpid(pid1, &running1, 0);
        clock_gettime(CLOCK_MONOTONIC, &end_time[0]);
        response_time[0] = (end_time[0].tv_sec - start_time[0].tv_sec) + 
                           (end_time[0].tv_nsec - start_time[0].tv_nsec) / 1e9;
        total_response_time += response_time[0];
    }

    /************************************************************************************************
            - Scheduling code ends here
    ************************************************************************************************/

    printf("\nProcess Execution Order (Shortest Job First):\n");
    printf("Process 4 (Workload %d) - Response Time: %.6f seconds\n", WORKLOAD4, response_time[3]);
    printf("Process 3 (Workload %d) - Response Time: %.6f seconds\n", WORKLOAD3, response_time[2]);
    printf("Process 2 (Workload %d) - Response Time: %.6f seconds\n", WORKLOAD2, response_time[1]);
    printf("Process 1 (Workload %d) - Response Time: %.6f seconds\n", WORKLOAD1, response_time[0]);

    // Print overall response time
    printf("\nAverage Response Time: %.6f seconds\n", total_response_time/4.0);
    printf("**********************************************************************************\n");

    return 0;
}

#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h>  
#include <stdlib.h>  
#include <sys/wait.h> 
#include <string.h> 
#include <time.h> 
#include <signal.h>
#include <sys/time.h>
#include <stdbool.h>

/************************************************************************************************ 
		These DEFINE statements represent the workload size of each task and 
		the time quantum values for Round Robin scheduling for each task.
*************************************************************************************************/

#define WORKLOAD1 100000
#define WORKLOAD2 100000
#define WORKLOAD3 100000
#define WORKLOAD4 100000

#define QUANTUM1 1000
#define QUANTUM2 1000
#define QUANTUM3 1000
#define QUANTUM4 1000

/************************************************************************************************ 
					DO NOT CHANGE THE FUNCTION IMPLEMENTATION
*************************************************************************************************/
void myfunction(int param){

	int i = 2;
	int j, k;

	while(i < param){
		k = i; 
		for (j = 2; j <= k; j++)
		{
			if (k % j == 0){
				k = k/j;
				j--;
				if (k == 1){
					break;
				}
			}
		}
		i++;
	}
}
/************************************************************************************************/



int main(int argc, char const *argv[])
{
	pid_t pid1, pid2, pid3, pid4;
	int running1, running2, running3, running4;

	pid1 = fork();

	if (pid1 == 0){

		myfunction(WORKLOAD1);

		exit(0);
	}
	kill(pid1, SIGSTOP);

	pid2 = fork();

	if (pid2 == 0){

		myfunction(WORKLOAD2);

		exit(0);
	}
	kill(pid2, SIGSTOP);

	pid3 = fork();

	if (pid3 == 0){

		myfunction(WORKLOAD3);

		exit(0);
	}
	kill(pid3, SIGSTOP);

	pid4 = fork();

	if (pid4 == 0){

		myfunction(WORKLOAD4);

		exit(0);
	}
	kill(pid4, SIGSTOP);

	/************************************************************************************************ 
		At this point, all  newly-created child processes are stopped, and ready for scheduling.
	*************************************************************************************************/



	/************************************************************************************************
		- Scheduling code starts here
		- Below is a sample schedule. (which scheduling algorithm is this?)
		- For the assignment purposes, you have to replace this part with the other scheduling methods 
		to be implemented.
	************************************************************************************************/

    struct timespec start_time[4], end_time[4];
    bool moved_to_fcfs[4] = {false, false, false, false};
    int active_processes = 4;
    double response_time[4] = {0};  // Store response times in seconds
    double total_response_time = 0.0; 

    for (int i = 0; i < 4; i++) {
        start_time[i].tv_sec = 0;
        start_time[i].tv_nsec = 0;
    }

    int QUANTUM = atoi(argv[1]);




	running1 = 1;
	running2 = 1;
	running3 = 1;
	running4 = 1;

    int FCFSqueue = 0;

    while(active_processes>0){
        //first level queue: Round Robin
        for (int i = 0; i < 4; i++) {
            if (!moved_to_fcfs[i] && running1 > 0) {
                clock_gettime(CLOCK_MONOTONIC, &start_time[i]);
                kill((i == 0) ? pid1 : (i == 1) ? pid2 : (i == 2) ? pid3 : pid4, SIGCONT);
                usleep(QUANTUM);
                kill((i == 0) ? pid1 : (i == 1) ? pid2 : (i == 2) ? pid3 : pid4, SIGSTOP);
                
                int* running_ptr = (i == 0) ? &running1 : (i == 1) ? &running2 : (i == 2) ? &running3 : &running4;
                pid_t pid = (i == 0) ? pid1 : (i == 1) ? pid2 : (i == 2) ? pid3 : pid4;
        
                if (waitpid(pid, running_ptr, WNOHANG) == 0) { 
                    moved_to_fcfs[i] = true;
                    FCFSqueue++;
                } else {
                    clock_gettime(CLOCK_MONOTONIC, &end_time[i]);
                    response_time[i] = (end_time[i].tv_sec - start_time[i].tv_sec) +
                                       (end_time[i].tv_nsec - start_time[i].tv_nsec) / 1e9;
                    total_response_time += response_time[i];
                }
                active_processes--;
            }
        }
    }
    printf("FCFSqueue: %d\n", FCFSqueue); 
    while(FCFSqueue>0){
        //Second level queue: FCFS

        for (int i = 0; i < 4; i++) {
            if (moved_to_fcfs[i] && (i == 0 ? running1 : i == 1 ? running2 : i == 2 ? running3 : running4) > 0) {
        
                pid_t pid = (i == 0) ? pid1 : (i == 1) ? pid2 : (i == 2) ? pid3 : pid4;
                int *running_ptr = (i == 0) ? &running1 : (i == 1) ? &running2 : (i == 2) ? &running3 : &running4;
        
                kill(pid, SIGCONT);
                waitpid(pid, running_ptr, 0);
                clock_gettime(CLOCK_MONOTONIC, &end_time[i]);
        
                response_time[i] = (end_time[i].tv_sec - start_time[i].tv_sec) +
                                   (end_time[i].tv_nsec - start_time[i].tv_nsec) / 1e9;
                total_response_time += response_time[i];
                FCFSqueue--;
            }
        }
    }
        


	/************************************************************************************************
		- Scheduling code ends here
	************************************************************************************************/
    double avg_response_time = total_response_time / 4.0;
    printf("Process 1 Response Time: %.6f seconds\n", response_time[0]);
    printf("Process 2 Response Time: %.6f seconds\n", response_time[1]);
    printf("Process 3 Response Time: %.6f seconds\n", response_time[2]);
    printf("Process 4 Response Time: %.6f seconds\n", response_time[3]);

    printf("\nProcess Execution Order (Multi-Level Feedback Queue):\n");
    printf("Overall Average Response Time: %.6f seconds\n", avg_response_time);

	return 0;
}
#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h>  
#include <stdlib.h>  
#include <sys/wait.h> 
#include <string.h> 
#include <time.h> 
#include <signal.h>
#include <sys/time.h>

/************************************************************************************************ 
		These DEFINE statements represent the workload size of each task and 
		the time quantum values for Round Robin scheduling for each task.
*************************************************************************************************/

#define WORKLOAD1 100000
#define WORKLOAD2 50000
#define WORKLOAD3 25000
#define WORKLOAD4 10000

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

    //Round Robin Scheduling 

	running1 = 1;
	running2 = 1;
	running3 = 1;
	running4 = 1;

    struct timespec startTime[4], endTime[4];
	double responseTime[4];

	while (running1 > 0 || running2 > 0 || running3 > 0 || running4 > 0)
	{

		

		if (running1 > 0){
			clock_gettime(CLOCK_MONOTONIC, &startTime[0]);
			kill(pid1, SIGCONT);
			usleep(QUANTUM1);
			kill(pid1, SIGSTOP);
		}
		if (waitpid(pid1, &running1, WNOHANG)>0) {
			clock_gettime(CLOCK_MONOTONIC, &endTime[0]);
			responseTime[0] = (endTime[0].tv_sec - startTime[0].tv_sec) + (endTime[0].tv_nsec - startTime[0].tv_nsec) / 1e9;
		}
		if (running2 > 0){
			clock_gettime(CLOCK_MONOTONIC, &startTime[1]);
			kill(pid2, SIGCONT);
			usleep(QUANTUM2);
			kill(pid2, SIGSTOP);
		}
		if (waitpid(pid2, &running2, WNOHANG)>0) {
			clock_gettime(CLOCK_MONOTONIC, &endTime[1]);
			responseTime[1] = (endTime[1].tv_sec - startTime[1].tv_sec) + (endTime[1].tv_nsec - startTime[1].tv_nsec) / 1e9;
		}
		if (running3 > 0){
			clock_gettime(CLOCK_MONOTONIC, &startTime[2]);
			kill(pid3, SIGCONT);
			usleep(QUANTUM3);
			kill(pid3, SIGSTOP);
		}
		if (waitpid(pid3, &running3, WNOHANG)>0) {
			clock_gettime(CLOCK_MONOTONIC, &endTime[2]);
			responseTime[2] = (endTime[2].tv_sec - startTime[2].tv_sec) + (endTime[2].tv_nsec - startTime[2].tv_nsec) / 1e9;
		}
		if (running4 > 0){
			clock_gettime(CLOCK_MONOTONIC, &startTime[3]);
			kill(pid4, SIGCONT);
			usleep(QUANTUM4);
			kill(pid4, SIGSTOP);
		}
		if (waitpid(pid4, &running4, WNOHANG)>0) {
			clock_gettime(CLOCK_MONOTONIC, &endTime[3]);
			responseTime[3] = (endTime[3].tv_sec - startTime[3].tv_sec) + (endTime[3].tv_nsec - startTime[3].tv_nsec) / 1e9;
		}
		
	}


	/************************************************************************************************
		- Scheduling code ends here
	************************************************************************************************/

	double avg_response_time = (responseTime[0] + responseTime[1] + responseTime[2] + responseTime[3]) / 4.0;
	printf("Average response time: %f\n", avg_response_time);

	return 0;
}
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
	struct timeval start[4], response[4];

	gettimeofday(&start[0], NULL);
	pid1 = fork();

	if (pid1 == 0){

		myfunction(WORKLOAD1);

		exit(0);
	}
	kill(pid1, SIGSTOP);

	gettimeofday(&start[1], NULL);
	pid2 = fork();

	if (pid2 == 0){

		myfunction(WORKLOAD2);

		exit(0);
	}
	kill(pid2, SIGSTOP);

	gettimeofday(&start[2], NULL);
	pid3 = fork();

	if (pid3 == 0){

		myfunction(WORKLOAD3);

		exit(0);
	}
	kill(pid3, SIGSTOP);

	gettimeofday(&start[3], NULL);
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

	

	
	
	kill(pid1, SIGCONT);
	
	waitpid(pid1,NULL,0);
    gettimeofday(&response[0], NULL);

	
	
	kill(pid2, SIGCONT);
	
	waitpid(pid2,NULL,0);
    gettimeofday(&response[1], NULL);

	
	
	kill(pid3, SIGCONT);
	
	waitpid(pid3,NULL,0);
    gettimeofday(&response[2], NULL);

	
	
	kill(pid4, SIGCONT);
	waitpid(pid4,NULL,0);
    gettimeofday(&response[3], NULL);

	for (int i = 0; i < 4; i++) {
		double response_time = (response[i].tv_sec-start[i].tv_sec) * 1000 + (response[i].tv_usec - start[i].tv_usec)/1000;
		printf("Process %d Response Time: %.2f ms\n", i + 1, response_time);
	}
	

	/************************************************************************************************
		- Scheduling code ends here
	************************************************************************************************/

	return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int SIGTERM_calls_received = 0;
int SIGUSR1_calls_received = 0;

void my_terminate(int param){
	if(param == SIGTERM){
		if(++SIGTERM_calls_received <= 2){
			printf ("SIGTERM handler is being called %d time\n", SIGTERM_calls_received);
		} else {
			exit(param);
		}
	} else if(param == SIGUSR1){
		if(++SIGUSR1_calls_received <= 2){
			printf ("SIGUSR1 handler is being called %d time\n", SIGUSR1_calls_received);
		} else {
			exit(param);
		}
	} else {
		printf ("handler received unknown signal %d\n", param);
	}

}

int main (){

	void (*prev_fn)(int);

	prev_fn = signal(SIGUSR1, my_terminate);
	if(prev_fn == SIG_ERR){
		printf ("could not attach handler to SIGUSR1");
	}
	
	prev_fn = signal(SIGTERM, my_terminate);
	if(prev_fn == SIG_ERR){
		printf ("could not attach handler to SIGTERM");
	}

	while(1){}

	return 0;
}
#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
void sig_handler(int signo)
{
    if (signo == SIGALRM){
    	//sleep(2);
        printf("\tReceived timer\n");
    }
}


int main()
{
	struct itimerval s1;
	struct sigaction sa;
	
	s1.it_interval.tv_sec = 0;
	s1.it_interval.tv_usec = 250000;
	s1.it_value.tv_sec = 1;
	s1.it_value.tv_sec = 1000000;
	// s1.it_interval = s1.it_value;
	signal(SIGALRM, sig_handler);
	if(setitimer(ITIMER_REAL, &s1, NULL)==-1)
		printf("ERROR setting timer\n");

	while(1)
		pause(	);
	return 0;
}
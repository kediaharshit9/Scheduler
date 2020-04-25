#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h> 
#include <sys/wait.h>
#include <string.h>

static volatile int signalPid = -1;

struct prob
{
	int m;
	int n;
	int t;
	float p;
};
void sig_handler(int signo)
{
	if(signo == SIGALRM)
	{    
    	printf("Child running too long!\n");
    	// printf("%d\n", signalPid);
    	kill(signalPid, SIGSTOP);
    	printf("Stopping child\n");
  		sleep(2);
  		printf("child can continue\n");
  		kill(signalPid, SIGCONT);
  		alarm(1);
	}
	if(signo == SIGILL)
	{
		alarm(0);
		kill(getpid(), SIGINT);
	}
    // if(signo = SIGFPE)
    // {
    //     alarm(1);
    // }
}


void get_pid(int sig, siginfo_t *info, void *context)
{
    signalPid = info->si_pid;

    printf("PID of signal sender = %d\n", signalPid);

    if(sig == SIGUSR1)
    {
    	printf("Scheduler : %d is sleeping\n", signalPid);
    	// alarm(0);
    }

    else if(sig == SIGUSR2)
    {
    	printf("Scheduler : %d woke up!\n", signalPid);
    	// alarm(0);
    }

    else if(sig == SIGPWR)
    {
        sleep(1);
    	printf("Scheduler : %d ready now run\n", signalPid);
    	kill(signalPid, SIGUSR1);
    	// alarm(1);
    }
}

int main()
{
	printf("arrived in scheduler\n");
	// char* name = "cs3500";
 //  	int size = 256;
 //  	int fd = shm_open(name, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
 //  	ftruncate(fd, size);
 //  	char* shmem = mmap(NULL, 128, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); 
  	
 //  	int pidS = getpid();
 //  	char sch[10];
 //  	itoa(pidS, sch, 10);
 //  	memcpy(shmem, sch, 10);


	int rc = fork();
	// printf("%d\n", pidS);
	if(rc==0)
	{	
		// execvp("./generator",);

		execlp("./workload", "worklaod", 0);

	}
	//taken from stack overflow
	struct sigaction sleep;
	sleep.sa_flags = SA_SIGINFO;
    sleep.sa_sigaction = get_pid;
    signal(SIGALRM, sig_handler);
    signal(SIGILL, sig_handler);
    // signal(SIGFPE, sig_handler);
    while(1){
    	//pause();
    	sigaction(SIGPWR, &sleep, NULL);
    	sigaction(SIGUSR1, &sleep, NULL);
    	sigaction(SIGUSR2, &sleep, NULL);
	    pause();
		// sigaction
    }

    return 0;
}
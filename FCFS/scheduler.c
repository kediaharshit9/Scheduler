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
int N;	//number of processes
int count = 1;
int busy = 0;
struct timeval tt;
double begg;
double s_time;
double e_time;

struct proc
{
	int a;
	int m;
	int n;
	int t;
	double p;
	int state;	// -9:exited, -1:did not start, 0:ready
	int asleep;
};

struct proc pr[10];
int rc[10];

void get_pid(int sig, siginfo_t *info, void *context)
{
    signalPid = info->si_pid;
    int i;
    // printf("PID of signal sender = %d\n", signalPid);

    if(sig == SIGUSR1)
	{
    	// printf("Scheduler : %d is sleeping\n", signalPid);

		for(i=0; i<N; i++)
    	{
    		if(signalPid == rc[i])
    		{
    			pr[i].asleep = 1;
    			busy = 0; 
    			gettimeofday(&tt, NULL);
    			e_time = (tt.tv_usec/1000000.0) + tt.tv_sec -begg;
				
				printf("%lf, %lf, %d\n", s_time, e_time, i+1);
    			break;
    		}
    	}
    	if(busy==0)
    	{
    		int next = -1;
		    int min = 99999;
		    for(i=0; i<N; i++)
		    {
		    	if(pr[i].state > 0 && pr[i].state < min && pr[i].asleep == 0)
				{
					min = pr[i].state;
					next = i;

				}
		    }
		    // printf("%d\n", next);
			if(next != -1) 
			{
				busy = 1;
				// printf("Now starting %d\n", rc[next]);
				gettimeofday(&tt, NULL);
				// pr[next].s_time = tt.tv_sec+(tt.tv_usec/1000000.0) -begg;
				s_time = (tt.tv_usec/1000000.0) + tt.tv_sec -begg;
				
				// pr[next].s_time_sec = tt.tv_sec;
				kill(rc[next] ,SIGUSR1);
			}
    	}
	}
    
    else if(sig == SIGUSR2)
    {	
    	// printf("Scheduler : %d woke up!\n", signalPid);
    	for(i=0; i<N; i++)
    	{
    		if(signalPid == rc[i])
    		{
    			pr[i].asleep = 0; 
    			break;
    		}
    	}
    	if(busy==0)
    	{
    		int next = -1;
		    int min = 99999;
		    for(i=0; i<N; i++)
		    {
		    	if(pr[i].state > 0 && pr[i].state < min && pr[i].asleep == 0)
				{
					min = pr[i].state;
					next = i;

				}
		    }
		    // printf("%d\n", next);
			if(next != -1) 
			{
				busy = 1;
				// printf("Now starting %d\n", rc[next]);
				gettimeofday(&tt, NULL);
				// pr[next].s_time = tt.tv_sec+(tt.tv_usec/1000000.0) -begg;
				s_time = (tt.tv_usec/1000000.0) + tt.tv_sec -begg;
				
				// pr[next].s_time_sec = tt.tv_sec;
				kill(rc[next] ,SIGUSR1);
			}
    	}
	}

    else if(sig == SIGPWR)
    {
    	// printf("Scheduler : %d ready\n", signalPid);

    	for(i=0; i<N; i++)
    	{
    		if(signalPid == rc[i])
    		{
    			pr[i].state = count++; //ready
    		}
    	}
    	// kill(signalPid, SIGUSR1);
    	if(busy==0)
    	{
    		int next = -1;
		    int min = 99999;
		    for(i=0; i<N; i++)
		    {
		    	if(pr[i].state > 0 && pr[i].state < min && pr[i].asleep == 0)
				{
					min = pr[i].state;
					next = i;
				}
		    }
		    // printf("%d\n", next);
			if(next != -1) 
			{
				busy = 1;
				// printf("Now starting %d\n", rc[next]);

				gettimeofday(&tt, NULL);
				s_time = tt.tv_sec+(tt.tv_usec/1000000.0) -begg;
				kill(rc[next] ,SIGUSR1);
			}
    	}
    }

    else if(sig == SIGILL)
    {
    	// printf("%d Exited\n", signalPid);

    	for(i=0; i<N; i++)
    	{
    		if(signalPid ==	rc[i])
    		{
    			gettimeofday(&tt, NULL);
				e_time = (tt.tv_usec/1000000.0) + tt.tv_sec -begg;
    			pr[i].state = -9; //exited
    			printf("%lf, %lf, %d\n", s_time, e_time, i+1);
    			break;
    		}
    	}
    	busy = 0;
    	//find a new process to start
    	int next = -1;
    	int min = 99999;
    	for(i=0; i< N; i++)
    	{
    		if(pr[i].state > 0 && pr[i].state < min && pr[i].asleep == 0)
    		{
    			min = pr[i].state;
    			next = i;
    		}
    	}
    	// printf("%d\n", next);
    	if(next != -1) 
    	{
    		busy = 1;
    		// printf("Now starting %d\n", rc[next]);
    		gettimeofday(&tt, NULL);
			s_time = tt.tv_usec/1000000.0 + tt.tv_sec -begg;
    		kill(rc[next],SIGUSR1);
    	}
    	else
    	{
    		int c=0;
    		for(i=0; i<N; i++)
    		{
    			if(pr[i].state == -9)
    				c++;	
    		}
    		if(c>=N)
    		{
    			kill(getpid(), SIGINT);
    		}
    	}
    	
    }
    else if (sig == SIGINT)
    { 
    	exit(0);
 	}
}

int main()
{
	// printf("arrived in scheduler\n");
	// char* name = "cs3500";
 //  	int size = 256;
 //  	int fd = shm_open(name, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
 //  	ftruncate(fd, size);
 //  	char* shmem = mmap(NULL, 128, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); 
  	
 //  	int pidS = getpid();
 //  	char sch[10];
 //  	itoa(pidS, sch, 10);
 //  	memcpy(shmem, sch, 10);
	
	
	int pidS = getpid();

	//TAKE INPUT
	int i;
	scanf("%d", &N);
	for(i=0; i<N; i++)
	{
		scanf("%d %d %d %d %lf", &pr[i].a, &pr[i].m, &pr[i].n, &pr[i].t, &pr[i].p);
		pr[i].state = -1; //not started
		pr[i].asleep = 0; 
		// printf("%d %d %d %d %f\n", pr[i].a, pr[i].m, pr[i].n, pr[i].t, pr[i].p);
		fflush(stdout);
	}

	// fork required processes
	// printf("Input read!\n");
	
	for(i=0; i<N; i++)
	{
		rc[i] = fork();

		if(rc[i] == 0)
		{	
			// execvp("./generator",);
			char* exec_argv[8];
			int j;
			for(j=0; j<8; j++)
            	exec_argv[j] =(char*) malloc(100);

			exec_argv[0] = "workload";
			sprintf(exec_argv[1], "%d", pidS);
			sprintf(exec_argv[2], "%d", pr[i].a);
			sprintf(exec_argv[3], "%d", pr[i].m);
			sprintf(exec_argv[4], "%d", pr[i].n);
			sprintf(exec_argv[5], "%d", pr[i].t);
			sprintf(exec_argv[6], "%d",(int)(pr[i].p*100));
			exec_argv[7] = 0;
			execvp("./workload", exec_argv);

		}
	}

	//taken from stack overflow
	struct sigaction sl;
	sl.sa_flags = SA_SIGINFO;
    sl.sa_sigaction = get_pid;
    // signal(SIGALRM, sig_handler);
    // signal(SIGILL, sig_handler);

    sigaction(SIGPWR, &sl, NULL);
	sigaction(SIGUSR1, &sl, NULL);
	sigaction(SIGUSR2, &sl, NULL);
	sigaction(SIGILL, &sl, NULL);
	sigaction(SIGINT, &sl, NULL);
    // sleep(1);
    // printf("Starting SCHEDULING\n");
    
    gettimeofday(&tt, NULL);
    begg = tt.tv_sec + (tt.tv_usec/1000000.0);

    while(1){   	
	    pause();
		// sigaction
		sigaction(SIGPWR, &sl, NULL);
		sigaction(SIGUSR1, &sl, NULL);
		sigaction(SIGUSR2, &sl, NULL);
		sigaction(SIGILL, &sl, NULL);
    }

    return 0;
}

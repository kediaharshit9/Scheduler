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

void sig_handler(int signo)
{
    if (signo == SIGUSR1){
    	//sleep(2);
        // printf("\tReceived running\n");
    }
}

void io_emulation(int t, int pid)
{
	kill(pid, SIGUSR1);
	sleep(t);
	kill(pid, SIGUSR2);
}


int main(int argc, char* argv[])
{
	// printf("Arrived in workload   ");
// 	int pidS = getppid();
	// char* name = "cs3500";
	// int size = 256;
	// int fd = shm_open(name, O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
 	// ftruncate(fd, size);
 	// char* shmem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
 // 	struct prob p1;
 	// char *sch = (char *) calloc(10, sizeof(char));
 	// memcpy(sch, shmem, 10);
 	// pidS = atoi(sch);
 	// printf("%d %d\n", pidS, getpid());

 	int pidS;
 	int a, m, n, t, P;
 	double p=0;
 	// float p=0;
    int mypid = getpid();
    // printf("CHILD: %d\n", mypid);

 	pidS = atoi(argv[1]);
 	a = atoi(argv[2]);
 	m = atoi(argv[3]);
 	n = atoi(argv[4]);
    t = atoi(argv[5]);
    // n = atoi(argv[6]);
    P = atoi(argv[6]);
    p = P/100.0;
 	// printf("%d %d %d %d %d %lf\n", pidS, a, m, n, t, p);
    sleep(a);

	signal(SIGUSR1, sig_handler);

	//saying scheduler that ready, and waiting for run signal
	// printf("%d CHILD YIELDING\n", mypid);
	kill(pidS, SIGPWR);
	pause();

	srand(time(0));
	// printf("\t %d up and running\n", mypid);

	int i, j;
	long long int ans;

	for(j=0; j<n; j++)
	{
		int arr1[m];
		int arr2[m];
		ans = 0;
		for(i=0; i<m; i++)
		{
			arr1[i] = rand();
			arr2[i] = rand();
			ans += arr1[i]*arr2[i];
		}
		// printf("%d Random Answer %d %lld\n", mypid, j, ans);
		int pr = rand() % 10000;

		if(pr < p*10000)
		{
			//since FCFS, it will finish IO and wont shift control
			io_emulation(t, pidS);
		}
	}
	// printf("\t Child Exiting (%d)\n", mypid);
	kill(pidS, SIGILL);
	exit(0);
	return 0;
}
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

struct prob
{
	int m;
	int n;
	int t;
	float p;
};

void sig_handler(int signo)
{
    if (signo == SIGUSR1){
    	//sleep(2);
        printf("\tReceived running\n");
    }
}

void io_emulation(int t, int pid)
{
	kill(pid, SIGUSR1);
	sleep(t);
	kill(pid, SIGUSR2);
}


int main()
{
	printf("Arrived in workload\n");
	int pidS = getppid();
	// char* name = "cs3500";
	// int size = 256;

	// int fd = shm_open(name, O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
 	// ftruncate(fd, size);
  
 	// char* shmem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

 // 	struct prob p1;

 	// char *sch = (char *) calloc(10, sizeof(char));

 	// memcpy(sch, shmem, 10);

 	// printf("%d %d %d %f\n", p1.m, p1.n, p1.t, p1.p);
 	// pidS = atoi(sch);
 	// printf("%d %d\n", pidS, getpid());
 	
 	int m, n, t;
 	float p;

 	printf("Enter m, p, t, n: ");
	scanf("%d %f %d %d", &m, &p, &t, &n);

	// printf("%d %d %d %f\n",m, n, t, p);

	if(signal(SIGUSR1, sig_handler) == SIG_ERR)
		printf("Can't catch SIGUSR1\n");

	printf("CHILD YIELDING\n");
	kill(pidS, SIGPWR);

	pause();

	srand(time(0));
	printf("\tup and running\n");

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
		printf("Random Answer %d %lld\n", j, ans);
		int pr = rand() % 10000;
		sleep(2);

		if(pr < p*10000)
		{
			io_emulation(t, pidS);
			printf("\tyielding\n");
			kill(pidS, SIGPWR);
			pause();
			printf("\tup and running\n");
		}

	}
	printf("\t Child Exiting \n");
	kill(pidS, SIGILL);
	exit(0);
	return 0;
}
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


int main()
{
	// char* name = "cs3500";
	// int size = 256;

	// int fd = shm_open(name, O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
 // 	ftruncate(fd, size);
  
 // 	struct prob* shmem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);	

	// int m, n, t;
	// float p;
	// printf("Enter m, p, t, n: ");
	// scanf("%d %f %d %d", &m, &p, &t, &n);

	// struct prob p1;
	// p1.m = m;
	// p1.n = n;
	// p1.t = t;
	// p1.p = p;

	// memcpy((struct prob *)shmem, &p1, sizeof(struct prob));

	// int rc = fork();
	// if(rc==0)
	// {
	// 	execlp("./worklaod", "./worklod", 0);
	// }

	// wait(NULL);
	return 0;
	

}
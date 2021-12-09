#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/ipc.h>
#include<semaphore.h>
#include <sys/time.h>

#define SHM_SIZE 1024

struct Shm{
    int data[4];
    sem_t sem;
};

int main(int argc, char* argv[])
{
int count = atoi(argv[1]);
int M = atoi(argv[2]);
struct Shm* sharedmemory;
pid_t pid;
key_t key;
int shmid;
struct timeval ct;

/*forging the key*/

if ((key = ftok("test_shm", 'X')) < 0) 
{
    perror("ftok\n");
    exit(1);
}
		
/*creating shared memory*/

if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) < 0) 
{
    perror("shmget\n");
    exit(1);
}

sharedmemory = (struct Shm *)shmat(shmid,0,0);

sem_init(&sharedmemory[4].sem,2,1);//initializing semaphore in shared memory
sharedmemory[0].data[0] = 1;
sharedmemory[1].data[1] = 2;
sharedmemory[2].data[2] = 0;//sum variable
sharedmemory[3].data[3] = 0;//winner pid

gettimeofday(&ct, NULL);
//printf("seconds: %ld\nmicroseconds: %ld\n",ct.tv_sec,ct.tv_usec);
for(int i=0;i<count;i++)
{
    pid=fork();
			if(pid==-1)
			{
				printf("Something went wrong with fork\n");
				exit(0);
			}
			if(pid==0)
			{
			break;
			}
}

if(pid==0)
{
    while(1)
    {
        gettimeofday(&ct, NULL);
        //printf("Time before aquiring lock for process %d: microseconds: %ld\n",getpid(),ct.tv_usec);
        sem_wait(&sharedmemory[4].sem);//process enters critical section
        gettimeofday(&ct, NULL);
        //printf("Time after aquiring lock for process %d: microseconds: %ld\n",getpid(),ct.tv_usec);
        if(sharedmemory[3].data[3]!=0){
            sem_post(&sharedmemory[4].sem);
            exit(0);
        } else 
        {
            sharedmemory[2].data[2] = sharedmemory[0].data[0] + sharedmemory[1].data[1];
        }
        printf("Currently the sum is %d and is being modified by process %d\n",sharedmemory[2].data[2],getpid());
        if(sharedmemory[2].data[2]>M)
        {
            sharedmemory[3].data[3] = getpid();
            printf("Yippee! I am the winner with process id %d\n",sharedmemory[3].data[3]);
            sem_post(&sharedmemory[4].sem);
            exit(0);
        }
        else if(sharedmemory[0].data[0]>sharedmemory[1].data[1])
        {
            sharedmemory[1].data[1] = sharedmemory[2].data[2];
        }
        else sharedmemory[0].data[0] = sharedmemory[2].data[2];
        sem_post(&sharedmemory[4].sem);//process leaves critical section
    }
}
else if(pid>0)
{
    while (pid = waitpid(-1, NULL, 0))
        {
            if (errno == ECHILD)
            {
                break;
            }
        }
    printf("Winner is process with pid %d\n",sharedmemory[3].data[3]);
    printf("The final value of the sum was: %d\n",sharedmemory[2].data[2]);
    shmdt(&sharedmemory);
    shmctl(shmid, IPC_RMID, 0);
    sem_close(&sharedmemory[4].sem);
    gettimeofday(&ct, NULL);
    //printf("seconds: %ld\nmicroseconds: %ld\n",ct.tv_sec,ct.tv_usec);
    return 0;
}
}

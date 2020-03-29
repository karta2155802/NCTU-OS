#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;
    printf("Main process id : %d\n",getpid());
    pid = fork();
    int status;
    wait(NULL);
    if(pid == 0)
    {
        printf("Fork1, I'm the child %d, my parent is %d.\n",getpid(),getppid());
        pid = fork();
        wait(NULL);
        if(pid == 0)
        {
            printf("Fork2, I'm the child %d, my parent is %d.\n",getpid(),getppid());
        }
    }
    pid = fork();
    if(pid == 0)
    {
        printf("Fork3, I'm the child %d, my parent is %d.\n",getpid(),getppid());
    }
    return 0;
}

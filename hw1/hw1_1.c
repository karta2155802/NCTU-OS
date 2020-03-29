#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#define MAX_LINE 80

int main(int argc, char**argv)
{
	char *arg[MAX_LINE/2+1]; /*command line arguments*/
	int should_run = 1; /*flag to determine when to exit program*/

	while(should_run){
        char *parse;
        char input[100];
        int i = 0,bgflag = 0;

		printf("osh>");
		fflush(stdout); //clean output buffer
        fgets(input,sizeof(input),stdin);

        for(int k=0;k<MAX_LINE/2+1;k++){    //set command to NULL
            arg[k] = NULL;
        }

        parse = strtok(input, " \n");
        if(parse == NULL)continue;	//empty string
        while(parse != NULL){   //split the input
            arg[i] = parse;
            i++;
            parse = strtok(NULL, " \n");
        }

        if(strcmp(arg[0], "cd") == 0){  //use command in c to implement "cd"
            if(chdir(arg[1]) < 0){
                perror(arg[1]);
            }
            continue;
        }
        if(strcmp(arg[0], "exit") == 0){
            exit(0);
        }
        for(int k=0;k<MAX_LINE/2+1 && arg[k]!=NULL;k++){    //check if there's "&"
            if(strcmp(arg[k], "&") == 0){
                bgflag = 1;
                arg[k] = NULL;
                continue;
            }
        }


        pid_t child = fork(); //create child process
        if(child < 0 ){
            perror("fork() error");
        }
        if(child == 0){
            execvp(arg[0], arg);
            printf("%s : commmand not found\n",arg[0]);
        }
        else if(bgflag != 1){
            waitpid(child, NULL, 0);
        }
        /*if(child < 0 ){
            perror("fork() error");
        }
        if(child == 0){
            if(bgflag == 0){
                execvp(arg[0], arg);
                printf("%s : commmand not found\n",arg[0]);
            }
            else{
                pid_t grand = fork();
                if(grand ==  0){
                    execvp(arg[0], arg);
                    printf("%s : commmand not found\n",arg[0]);
                }
                else{
                    exit(0);
                }
            }
        }
        waitpid(child, NULL, 0);*/
        
    }
    return 0;
}

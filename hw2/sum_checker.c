#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUMBER_OF_THREADS 11 /* Hint */
#define PUZZLE_SIZE 9

/* example puzzle */
int puzzle[PUZZLE_SIZE+1][PUZZLE_SIZE+1] = {
			{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0}
		};

/* data structure for passing data to threads */
typedef struct
{
	int thread_number;
	int x;
	int y;
    int value;
    int result;
} parameters;

/* print puzzle */ 
#define LINE "====================== \n"
#define COLLINE "\n___________________________________ \n"
#define NAME "||  SUM   CHECKER  || \n"
void print_grid(int grid[10][10])
{
    int i,j;
    printf(LINE);
    printf(NAME);
    printf(LINE);

	for (i = 1; i < 10; i++)
	{
		for (j = 1; j < 10; j++)
		{
	        printf("|%1d |",grid[i][j]);
		}
        printf(COLLINE);
	}
    printf("\n");
}
// read file to check sudoku
void SetPuzzle(char filename[]){
    FILE *file = fopen(filename,"r");
    int i,j,ch,num;
    for (i=0;i<=9;i++){
        for(j=0;j<=9;j++){
            while(((ch = getc(file)) != EOF)){
                if(ch == EOF) break;
                if(ch == ',') break;
                if(ch == '\n') break;
                ungetc(ch,file);
                fscanf(file,"%d",&num);
                if(num!=-1)
                    puzzle[i][j] = num;            
            } 
        }
    }
    print_grid(puzzle);
    return;
}

void* checkrow(void *arg)
{
    parameters *data=(parameters *)arg;
    int i,j,sum_row=0;

    for(i=1;i<10;i++){
        //sleep(3);
        for(j=1;j<10;j++){
            sum_row = sum_row + puzzle[i][j];
        }
        if(data->value == -1)data->value = sum_row;
        else if(data->value != sum_row){
            data->result = 0;
            //printf("row exit with sum=%d\n",sum_row);
            pthread_exit(NULL);
        }
        sum_row = 0;
    }
    pthread_exit(NULL);
}

void* checkcol(void *arg)
{
    parameters *data=(parameters *)arg;
    int i,j,sum_col=0;

    for(i=1;i<10;i++){
        //sleep(3);
        for(j=1;j<10;j++){
            sum_col = sum_col + puzzle[j][i];
        }
        if(data->value == -1)data->value = sum_col;
        else if(data->value != sum_col){
            data->result = 0;
            //printf("col exit with sum=%d\n",sum_col);
            pthread_exit(NULL);
        }
        sum_col = 0;
    }
    pthread_exit(NULL);
}

void* checknine(void *arg)
{
    parameters *data=(parameters *)arg;
    int i=data->x,j=data->y,sum_nine=0;
    int p,q;
    for(p=i;p<i+3;p++){
        for(q=j;q<j+3;q++){
            //sleep(3);
            sum_nine = sum_nine + puzzle[p][q];   
        }
    }
    if(data->value == -1)data->value = sum_nine;
    else if(data->value != sum_nine){
        data->result = 0;
        //printf("count nine exit with sum=%d\n",sum_nine);
        pthread_exit(NULL);
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
	int rv = 1; // flag to check answer
    // input the sudoku file
    SetPuzzle("test2.txt");

    parameters data;
    data.result = 1;
    data.value = -1;
    int k=0;
    pthread_t t1,t2,t[9];
    pthread_create(&t1,NULL,checkrow,(void*) &data);
    pthread_create(&t2,NULL,checkcol,(void*) &data);
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            data.x = i*3+1;
            data.y = j*3+1;
            pthread_create(&t[k],NULL,checknine,(void*) &data);
            k++;
        } 
    }

    k=0;
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            pthread_join(t[k],NULL);
            k++;
        }
    }
   
    rv = data.result;

	if (rv == 1)
		printf("Successful :) \n");
	else
		printf("Must check again! :( \n");

	return 0;
}

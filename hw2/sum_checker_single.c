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

void* checkpuzzle(void *arg)
{
    //sleep(10);
    parameters *data=(parameters *)arg;
    int i,j,x,y,sum_nine=0,sum_row=0,sum_col=0,sum=-1;
    for(i=1;i<10;i++){
        for(j=1;j<10;j++){
            sum_row = sum_row + puzzle[i][j];
        }
        if(sum == -1)sum = sum_row;
        else if(sum != sum_row){
            data->result = 0;
            pthread_exit(NULL);
        }
        sum_row = 0;
    }

    for(j=1;j<10;j++){
        for(i=1;i<10;i++){
            sum_col = sum_col + puzzle[i][j];
        }
        if(sum != sum_col){
            data->result = 0;
            pthread_exit(NULL);
        }
        sum_col = 0;
    }
    for(i=0;i<3;i++){
        for(j=0;j<3;j++){
            int x=i*3+1,y=j*3+1;
            for(int p=x;p<x+3;p++){
                for(int q=y;q<y+3;q++){
                    sum_nine = sum_nine + puzzle[p][q];
                }
            }
            if(sum_nine != sum){
                data->result = 0;
                pthread_exit(NULL);
            }
            sum_nine = 0;
        }
    }
    
}
int main(int argc, char* argv[])
{
    int rv = 1; // flag to check answer
    // input the sudoku file
    SetPuzzle("test2.txt");
 
    pthread_t t;
    parameters data;
    data.result = 1;
    pthread_create(&t,NULL,checkpuzzle,(void*) &data);
    pthread_join(t,NULL);
    rv = data.result;

    if (rv == 1)
        printf("Successful :) \n");
    else
        printf("Must check again! :( \n");

    return 0;
}

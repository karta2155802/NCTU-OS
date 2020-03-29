// Student ID:
// Name      :
// Date      : 
#include "bmpReader.h"
#include "bmpReader.cpp"
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
using namespace std;

#define MYRED	2
#define MYGREEN 1
#define MYBLUE	0

int imgWidth, imgHeight;
int MEAN_FILTER_SIZE=9;
int SOBEL_FILTER_SIZE;
int FILTER_SCALE;
int *filter_gx, *filter_gy;

sem_t s_mean[6000][6000];

const char *inputfile_name[5] = {
	"input1.bmp",
	"input2.bmp",
	"input3.bmp",
	"input4.bmp",
	"input5.bmp"
};
const char *outputMed_name[5] = {
	"output1.bmp",
	"output2.bmp",
	"output3.bmp",
	"output4.bmp",
	"output5.bmp"
};


unsigned char *pic_in, *pic_grey, *pic_mean, *pic_gx, *pic_gy, *pic_sobel,*pic_final;

void* MeanFilter(void* args)
{
	for (int h = 0; h<imgHeight; h++) {
		for (int w = 0; w<imgWidth; w++){
			int tmp = 0;
			int a, b , window[9],k=0,sum=0;
			int ws = (int)sqrt((float)MEAN_FILTER_SIZE);
			for (int j = 0; j<ws; j++)
			for (int i = 0; i<ws; i++)
			{
				a = w + i - (ws / 2);
				b = h + j - (ws / 2);

				// detect for borders of the image
				if (a<0 || b<0 || a>=imgWidth || b>=imgHeight)continue;

				sum=sum+pic_grey[b*imgWidth + a];
			};

			tmp=sum/MEAN_FILTER_SIZE;
			
			if (tmp < 0) tmp = 0;
			if (tmp > 255) tmp = 255;
			pic_mean[h*imgWidth + w] = (unsigned char)tmp;

            sem_post(&s_mean[h][w]);
		}
	}
}

void* gxy_sobelFilter(void* args)
{
	for (int h = 0; h<imgHeight; h++) {
		for (int w = 0; w<imgWidth; w++){
			if(h < imgHeight-1 && w < imgWidth-1)sem_wait(&s_mean[h+1][w+1]);
            
			int tmp1 = 0, tmp2=0;
			int a, b;
			int ws = (int)sqrt((float)SOBEL_FILTER_SIZE);
			for (int j = 0; j<ws; j++)
			for (int i = 0; i<ws; i++)
			{
				a = w + i - (ws / 2);
				b = h + j - (ws / 2);

				// detect for borders of the image
				if (a<0 || b<0 || a>=imgWidth || b>=imgHeight) continue;

				tmp1 += filter_gx[j*ws + i] * pic_mean[b*imgWidth + a];
				tmp2 += filter_gy[j*ws + i] * pic_mean[b*imgWidth + a];
			};
			if (tmp1 < 0) tmp1 = 0;
			if (tmp1 > 255) tmp1 = 255;
			if (tmp2 < 0) tmp2 = 0;
			if (tmp2 > 255) tmp2 = 255;
			pic_gx[h*imgWidth + w] = (unsigned char)tmp1;
			pic_gy[h*imgWidth + w] = (unsigned char)tmp2;

			int tmp3 = 0;
			tmp3 = sqrt(pic_gx[h*imgWidth + w]*pic_gx[h*imgWidth + w] + pic_gy[h*imgWidth + w]*pic_gy[h*imgWidth + w]);
			if (tmp3 < 0) tmp3 = 0;
			if (tmp3 > 255) tmp3 = 255;
			pic_sobel[h*imgWidth + w] = (unsigned char)tmp3;

			pic_final[3 * (h*imgWidth + w) + MYRED] = pic_sobel[h*imgWidth + w];
			pic_final[3 * (h*imgWidth + w) + MYGREEN] = pic_sobel[h*imgWidth + w];
			pic_final[3 * (h*imgWidth + w) + MYBLUE] = pic_sobel[h*imgWidth + w];            
		}
	}
}

int main()
{
	// read mask file
	FILE* mask;

	mask = fopen("mask_Sobel.txt", "r");
	fscanf(mask, "%d", &SOBEL_FILTER_SIZE);

	filter_gx = new int[SOBEL_FILTER_SIZE];
	filter_gy = new int[SOBEL_FILTER_SIZE];

	for (int i = 0; i<SOBEL_FILTER_SIZE; i++)
		fscanf(mask, "%d", &filter_gx[i]);

	for (int i = 0; i<SOBEL_FILTER_SIZE; i++)
		fscanf(mask, "%d", &filter_gy[i]);

	fclose(mask);
	
	BmpReader* bmpReader = new BmpReader();
	for (int k = 0; k<5; k++){
		// read input BMP file
		pic_in = bmpReader->ReadBMP(inputfile_name[k], &imgWidth, &imgHeight);
		// allocate space for output image
		pic_grey = (unsigned char*)malloc(imgWidth*imgHeight*sizeof(unsigned char));
		pic_mean = (unsigned char*)malloc(imgWidth*imgHeight*sizeof(unsigned char));
		pic_gx = (unsigned char*)malloc(imgWidth*imgHeight*sizeof(unsigned char));
		pic_gy = (unsigned char*)malloc(imgWidth*imgHeight*sizeof(unsigned char));
		pic_sobel = (unsigned char*)malloc(imgWidth*imgHeight*sizeof(unsigned char));
		pic_final = (unsigned char*)malloc(3 * imgWidth*imgHeight*sizeof(unsigned char));

		for (int j = 0; j<imgHeight; j++) {
			for (int i = 0; i<imgWidth; i++){
				int tmp = (
				pic_in[3 * (j*imgWidth + i) + MYRED] +
				pic_in[3 * (j*imgWidth + i) + MYGREEN] +
				pic_in[3 * (j*imgWidth + i) + MYBLUE] )/3;
				if (tmp < 0) tmp = 0;
				if (tmp > 255) tmp = 255;				 			
				pic_grey[j*imgWidth + i] = (unsigned char)tmp;
				sem_init(&s_mean[j][i],0,0);
			}
		}
		
		pthread_t th1,th2;
		pthread_create(&th2, NULL, gxy_sobelFilter, NULL);
		pthread_create(&th1, NULL, MeanFilter, NULL);
			
		pthread_join(th2,NULL);	
		pthread_join(th1,NULL);

		bmpReader->WriteBMP(outputMed_name[k], imgWidth, imgHeight, pic_final);

		//free memory space
		free(pic_in);
		free(pic_grey);
		free(pic_mean);
		free(pic_final);
		free(pic_sobel);
		free(pic_gx);
		free(pic_gy);
	}

	return 0;
}
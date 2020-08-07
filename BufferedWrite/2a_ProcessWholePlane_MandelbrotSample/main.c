#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#define FILE_NAME_BUF 12+4
#define MAX_COLOR_VAL 255
#define PIXELS_PER_LINE 2
#define X_BOUND 1024
#define Y_BOUND 1024
//usage: ./main outfile t-value func


typedef struct Pixel
{
	uint8_t red;
	uint8_t grn;
	uint8_t blu;
} Pixel;

//this code is not mine
void mandelbrot(Pixel frame_buffer[X_BOUND][Y_BOUND],int x_bound,int y_bound,int t)
{
	int max = 255;
	Pixel colors[5] = {
			(Pixel) {.red=255,.grn=255,.blu=255},
			(Pixel) {.red=200,.grn=255,.blu=255},
			(Pixel) {.red=150,.grn=255,.blu=255},
			(Pixel) {.red=100,.grn=255,.blu=255},
			(Pixel) {.red=50,.grn=255,.blu=255}};
	for (int i=0;i<x_bound;i++) //i is row, x_bound is height
	{
		for (int j=0;j<y_bound;j++) //j is col, y_bound is width
		{
			double c_re = (j-y_bound)/2.0*4.0/y_bound;
			double c_im = (i-x_bound)/2.0*4.0/y_bound;
			double x=0;
			double y=0;

			int k = 0;
			while (x*x+y*y<=4 && k<max)
			{
				double x_new = x*x-y*y +c_re;
				y=2*x*y+c_im;
				x=x_new;
				k++;
			}
			if (k<max) frame_buffer[j][i] = colors[k%5];
			else frame_buffer[j][i] = (Pixel) {.red=0,.grn=0,.blu=0};
		}
	}
}
void writeframebuffer(Pixel[X_BOUND][Y_BOUND],int,int);
void writeimageheader(FILE*);
void writepixel(FILE*,int,int,Pixel[X_BOUND][Y_BOUND]);
int main(int argc, char**argv)
{
	//FILE *out = fopen(argv[1],"w");
	Pixel (*fptr)(Pixel[X_BOUND][Y_BOUND],int,int,int) = NULL;
	Pixel frame_buffer[X_BOUND][Y_BOUND];

	//verify argc and argv stuff here
	if (argc!=3)
	{
		printf("argc\n");
		return EXIT_FAILURE;
	}

	mandelbrot(frame_buffer,X_BOUND,Y_BOUND,atoi(argv[2]));
	writeframebuffer(frame_buffer,X_BOUND,Y_BOUND);

	return EXIT_SUCCESS;

}
void writeimageheader(FILE* out)
{
	fprintf(out,"P3\n");
	fprintf(out,"%d %d\n",X_BOUND,Y_BOUND);
	fprintf(out,"%d\n",MAX_COLOR_VAL);
}
void writepixel(FILE* out,int x,int y,Pixel frame_buffer[X_BOUND][Y_BOUND])
{
	Pixel cur = frame_buffer[x][y];
	fprintf(out,"%d %d %d ",cur.red,cur.grn,cur.blu);
}
void writeframebuffer(Pixel frame_buffer[X_BOUND][Y_BOUND],int x_bound,int y_bound)
{
	static int frame = 0;
	char framename[FILE_NAME_BUF] = {0};
	sprintf(framename,"%d",frame); //convert frame no to string
	strncat(framename,".ppm",4);
	FILE *out = fopen(framename,"w");

	writeimageheader(out);
	int pixels = 0;
	for (int i=0;i<X_BOUND;i++) //rows
	{
		for (int j=0;j<Y_BOUND;j++) //columns
		{
			writepixel(out,i,j,frame_buffer);
			if (pixels==PIXELS_PER_LINE) 
			{
				fprintf(out,"\n");
				pixels=0;
			}
			else pixels++;
		}
	}
	fclose(out);
}

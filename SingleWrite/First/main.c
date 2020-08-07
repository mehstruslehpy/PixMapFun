#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#define BUF_SIZE 3 
#define MAX_COLOR_VAL 255
#define PIXELS_PER_LINE 2
#define X_BOUND 1024
#define Y_BOUND 1024

typedef struct rgb_pixel
{
	uint8_t red;
	uint8_t grn;
	uint8_t blu;
} rgb_pixel;
//usage: ./main outfile t-value func
void writepixel(FILE* out,int x,int y,int t,rgb_pixel (*func)(int,int,int));
void writeimageheader(FILE*);
//this should be modified to work with overflows
rgb_pixel simpleline (int x, int y, int t)
{
	uint8_t red=255;
	uint8_t grn=255;
	uint8_t blu=255;
	if (x==y)
	{
		red=0;
		grn=0;
		blu=0;
	}
	else red=grn=blu=255;
	return (rgb_pixel) {.red=red,.grn=grn,.blu=blu};
}
rgb_pixel func1 (int x, int y, int t)
{
	uint8_t red=x*sin(y+t);
	uint8_t grn=y*sin(x+t);
	uint8_t blu=x*sin(y+t);
	return (rgb_pixel) {.red=red,.grn=grn,.blu=blu};
}
rgb_pixel func2 (int x, int y, int t)
{
	uint8_t red=x*sin(exp(t));
	uint8_t grn=y*sin(exp(t));
	uint8_t blu=x*sin(exp(t));
	return (rgb_pixel) {.red=red,.grn=grn,.blu=blu};
}
rgb_pixel mat_func_wrapper(int x, int y, int t)
{
	//eqn x_(n+1)=Ax_n+b where b = (t,t*x,t*y)
	int m[3][3] = {{1,2,3}, {2,3,1}, {2,1,3}};
	int b[3] = {1,50,20};
	static uint8_t red = 0;
	static uint8_t grn = 100;
	static uint8_t blu = 150;
	red = red*m[0][0]+grn*m[0][1]+blu*m[0][2];
	red+=b[0]*t;
	grn = red*m[1][0]+grn*m[1][1]+blu*m[1][2];
	grn+=b[1]*t*x;
	blu = red*m[2][0]+grn*m[2][1]+blu*m[2][2];
	blu+=b[2]*t*y;
	return (rgb_pixel) {.red=red,.grn=grn,.blu=blu};
}
int main(int argc, char**argv)
{
	FILE *out = fopen(argv[1],"w");
	rgb_pixel (*fptr)(int,int,int) = NULL;

	//verify argc and argv stuff here
	if (argc!=4)
	{
		printf("argc\n");
		return EXIT_FAILURE;
	}

	if (!strcmp(argv[3],"simpleline")) fptr = simpleline;
	else if (!strcmp(argv[3],"func1")) fptr = func1;
	else if (!strcmp(argv[3],"func2")) fptr = func2;
	else if (!strcmp(argv[3],"mat_func")) fptr = mat_func_wrapper;
	else
	{
		printf("func\n");
		return EXIT_FAILURE;
	}

	//build image file
	int pixels=0;
	writeimageheader(out);
	for (int i=0;i<X_BOUND;i++) //rows
	{
		for (int j=0;j<Y_BOUND;j++) //columns
		{
			writepixel(out,i,j,atoi(argv[2]),fptr);
			if (pixels==PIXELS_PER_LINE) 
			{
				fprintf(out,"\n");
				pixels=0;
			}
			else pixels++;
		}
	}

	fclose(out);
	return EXIT_SUCCESS;

}
void writeimageheader(FILE* out)
{
	fprintf(out,"P3\n");
	fprintf(out,"%d %d\n",X_BOUND,Y_BOUND);
	fprintf(out,"%d\n",MAX_COLOR_VAL);
}
void writepixel(FILE* out,int x,int y,int t,rgb_pixel (*func)(int,int,int))
{
	//static int newline = 0;
	rgb_pixel cur = func(x,y,t);
	//printf("DEBUG:%d %d %d \n",cur.red,cur.grn,cur.blu);
	fprintf(out,"%d %d %d ",cur.red,cur.grn,cur.blu);

	//if (newline==0);
	//else if (newline%PIXELS_PER_LINE==0) fprintf(out,"#newline=%d\n",newline);
	//else;

	//newline++;
}

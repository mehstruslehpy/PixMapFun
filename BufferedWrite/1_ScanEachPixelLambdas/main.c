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

//void writepixel(FILE* out,int x,int y,int t,rgb_pixel (*func)(int,int,int));
//this should be modified to work with overflows
Pixel simpleline (Pixel frame_buffer[X_BOUND][Y_BOUND],int x, int y, int t)
{
	uint8_t red=255;
	uint8_t grn=255;
	uint8_t blu=255;
	if (x==y)
	{
		frame_buffer[x][y].red=0;
		frame_buffer[x][y].grn=0;
		frame_buffer[x][y].blu=0;
	}
	else
	{
		frame_buffer[x][y].red=255;
		frame_buffer[x][y].grn=255;
		frame_buffer[x][y].blu=255;
	}
	return (Pixel) {.red=255,.grn=255,.blu=255};
}
void processframebuffer(Pixel frame_buffer[X_BOUND][Y_BOUND],int x_bound,int y_bound,int t,Pixel(*func)(Pixel[X_BOUND][Y_BOUND],int,int,int))
{
	for (int i = 0;i<x_bound;i++)
		for (int j = 0;j<y_bound;j++)
		{
			func(frame_buffer,i,j,t);
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
	if (argc!=4)
	{
		printf("argc\n");
		return EXIT_FAILURE;
	}

	if (!strcmp(argv[3],"simpleline")) fptr = simpleline;
	else
	{
		printf("func\n");
		return EXIT_FAILURE;
	}

	processframebuffer(frame_buffer,X_BOUND,Y_BOUND,atoi(argv[2]),simpleline);
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

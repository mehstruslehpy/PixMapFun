#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#define FILE_NAME_BUF 12+4
#define POINT_BUF_LEN 2048
#define MAX_COLOR_VAL 255
#define PIXELS_PER_LINE 2
#define X_BOUND 1024
#define Y_BOUND 1024
#define ITERATIONS 10000
//usage: ./main outfile t-value func


typedef struct Pixel
{
	uint8_t red;
	uint8_t grn;
	uint8_t blu;
} Pixel;

//could also store color
typedef struct Tuple
{
	int x;
	int y;
} Tuple;

void write_pt_arr_to_fbuf(Pixel[X_BOUND][Y_BOUND],Tuple*,int);
void write_fbuf_to_disk(Pixel[X_BOUND][Y_BOUND],int,int);
void writeimageheader(FILE*);
void writepixel(FILE*,int,int,Pixel[X_BOUND][Y_BOUND]);
int main(int argc, char**argv)
{
	Pixel frame_buffer[X_BOUND][Y_BOUND]={0};
	Tuple pt_buf[POINT_BUF_LEN];

	//verify argc and argv stuff here
	if (argc!=3)
	{
		printf("argc\n");
		return EXIT_FAILURE;
	}

	for (int i=0;i<POINT_BUF_LEN;i++) 
		pt_buf[i] = (Tuple) {.x=rand()%X_BOUND,.y=rand()%Y_BOUND};

	write_pt_arr_to_fbuf(frame_buffer,pt_buf,POINT_BUF_LEN);
	write_fbuf_to_disk(frame_buffer,X_BOUND,Y_BOUND);

	return EXIT_SUCCESS;

}
void write_pt_arr_to_fbuf(Pixel frame_buffer[X_BOUND][Y_BOUND],Tuple* pt_buf,int buf_len)
{
	for (int i = 0;i<buf_len;i++)
		frame_buffer[pt_buf[i].x][pt_buf[i].y] 
			= (Pixel) {.red=rand()%256,.grn=rand()%256,.blu=rand()%256};
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
void write_fbuf_to_disk(Pixel frame_buffer[X_BOUND][Y_BOUND],int x_bound,int y_bound)
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

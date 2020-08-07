#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#define FILE_NAME_BUF 12+4+5+5
#define MAX_COLOR_VAL 255
#define MAX_ITERATIONS 255
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

//could also store color
typedef struct Tuple
{
	double x;
	double y;
} Tuple;

double scale_to_interval(double,double,double,double,double);

//from:https://www.geeksforgeeks.org/julia-fractal-python/
Pixel multijulia_set(double xin,double yin,double* params,int pcnt)
{
	//should check pcnt here
	Pixel palette[10] = {(Pixel){.red=0,.grn=255,.blu=255},
		(Pixel){.red=25,.grn=255,.blu=255},
		(Pixel){.red=50,.grn=255,.blu=255},
		(Pixel){.red=75,.grn=255,.blu=255},
		(Pixel){.red=100,.grn=255,.blu=255},
		(Pixel){.red=125,.grn=255,.blu=255},
		(Pixel){.red=150,.grn=255,.blu=255},
		(Pixel){.red=200,.grn=255,.blu=255},
		(Pixel){.red=225,.grn=255,.blu=255},
		(Pixel){.red=255,.grn=255,.blu=255}};
	Pixel is_not_elt = {.red=0,.grn=0,.blu=0};
	double zoom = params[0]; //1
	double w = params[1]; //X_BOUND
	double h = params[2]; //Y_BOUND
	double movex = params[3]; //0
	double movey = params[4]; //0

	double cx = params[5]; //-0.7
	double cy = params[6]; //0.27015
	double zx = 1.5*(xin-w/2)/(0.5*zoom*w)+movex;
	double zy = 1.0*(yin-h/2)/(0.5*zoom*h)+movey;
	int iteration = 0;
	int max_iteration = params[7];
	int d = params[8];
	complex z = zx+zy*I;
	complex c = cx+cy*I;

	while (cabs(z)<=4 && iteration<max_iteration)
	{
		/*
		double xtemp = zx*zx-zy*zy+cx;
		zy=2*zx*zy+cy;
		zx=xtemp;
		*/
		z=cpow(z,d)+c;
		iteration++;
	}
	if (iteration==max_iteration) return is_not_elt;
	else return palette[iteration%sizeof(palette)];
}
//print info about tuple to std out
void debug_print_tuple(Tuple* tp, char* msg);
//scale number in interval [min,max] to [a,b]
//fill frame buffer with color
void fill_fbuf_color(Pixel[X_BOUND][Y_BOUND],int);
//write the frame buffer to disk
void write_fbuf_to_disk(Pixel[X_BOUND][Y_BOUND],int,int);
//write image header to disk
void writeimageheader(FILE*);
//write pixel to disk
void writepixel(FILE*,int,int,Pixel[X_BOUND][Y_BOUND]);
void process_fbuf(Pixel frame_buffer[X_BOUND][Y_BOUND],Pixel(*fn)(double,double,double*,int),double* params,int pcnt,int x_bound,int y_bound)
{
	for (int i=0;i<x_bound;i++)
		for (int j=0;j<y_bound;j++)
		{
			frame_buffer[i][j] = fn(i,j,params,pcnt);
			printf ("DEBUG: x=%d,y=%d,red=%d,grn=%d,blu=%d\n",i,j,frame_buffer[i][j].red,frame_buffer[i][j].grn,frame_buffer[i][j].blu);
		}
}

int main(int argc, char** argv)
{
	printf("DEBUG: Program start.\n");
	Pixel frame_buffer[X_BOUND][Y_BOUND];
	if (argc!=7)
	{
		printf("Usage: ./julia <zoom> <x-shift> <y-shift> <cx> <cy> <power>\n");
		printf("\texample ./julia 1 0 0 -0.7 0.27015 2\n");
		return EXIT_FAILURE;
	}

	//JULIA (like) SET
	printf("DEBUG: White out frame buffer.\n");
	fill_fbuf_color(frame_buffer,255);
	double params1[9] = { atof(argv[1]), //zoom
		X_BOUND,
		Y_BOUND,
		atof(argv[2]), //movex
		atof(argv[3]), //movey
		atof(argv[4]), //cx
		atof(argv[5]), //cx
		MAX_ITERATIONS,
		atof(argv[6])};

	printf("DEBUG: process frame buffer.\n");
	process_fbuf(frame_buffer,multijulia_set,params1,sizeof(params1),X_BOUND,Y_BOUND);
	printf("DEBUG: Write frame buffer to disk.\n");
	write_fbuf_to_disk(frame_buffer,X_BOUND,Y_BOUND);

	return EXIT_SUCCESS;

}
void debug_print_tuple(Tuple* tp, char* msg) {printf("%s {.x=%f, .y=%f}\n",msg,tp->x,tp->y);}
void fill_fbuf_color(Pixel frame_buffer[X_BOUND][Y_BOUND],int color)
{
	for (int i=0;i<X_BOUND;i++)
		for (int j=0;j<Y_BOUND;j++)
			frame_buffer[i][j] = (Pixel) {.red=color,.grn=color,.blu=color};
}
//scale points in interval [min,max] to [a,b]
double scale_to_interval(double x,double min,double max ,double a,double b)
{ 
	if (max-min==0) return max; //throw div by zero out of view
	else return (b-a)*(x-min)/(max-min)+a; //else scale to bounds
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
	strncat(framename,"multijulia.ppm",9+5);
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
	frame++;
}

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#define FILE_NAME_BUF 12+4+10
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

//scale number in interval [min,max] to [a,b]
double scale_to_interval(double,double,double,double,double);

//from the mandelbrot set wikipedia
Pixel mandelbrot_set(double xin,double yin,double* params,int pcnt)
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
	//Pixel is_elt = {.red=255,.grn=255,.blu=255};
	double x0 = scale_to_interval(xin,params[0],params[1],-2.5,1.0);
	double y0 = scale_to_interval(yin,params[2],params[3],-1.0,1.0);

	double x=0;
	double y=0;
	int iteration = 0;
	int max_iteration = params[4];

	while (x*x+y*y<=2*2 && iteration<max_iteration)
	{
		double xtemp = x*x-y*y +x0;
		y=2*x*y+y0;
		x=xtemp;
		iteration++;
	}
	if (iteration<max_iteration) return palette[iteration%sizeof(palette)];
	//if (iteration<max_iteration) return is_elt;
	else return is_not_elt;
}
//print info about tuple to std out
void debug_print_tuple(Tuple* tp, char* msg);
//fill frame buffer with color
void fill_fbuf_color(Pixel[X_BOUND][Y_BOUND],int);
//write the frame buffer to disk
void write_fbuf_to_disk(Pixel[X_BOUND][Y_BOUND],int,int);
//write image header to disk
void writeimageheader(FILE*);
//write pixel to disk
void writepixel(FILE*,int,int,Pixel[X_BOUND][Y_BOUND]);
//process each pixel in frame buffer via function ptr fn
void process_fbuf(Pixel[X_BOUND][Y_BOUND],Pixel(*fn)(double,double,double*,int),double*,int,int,int);

int main(int argc, char** argv)
{
	printf("DEBUG: Program start.\n");
	Pixel frame_buffer[X_BOUND][Y_BOUND];
	if (argc!=6)
	{
		printf("Usage: ./mandelbrot <int> <int> <int> <int> <int>\n");
		printf("\texample: ./mandelbrot 0 1024 0 1024 255\n");
		return EXIT_FAILURE;
	}
	//MANDELBROT SET
	printf("DEBUG: White out frame buffer.\n");
	fill_fbuf_color(frame_buffer,255);
	//double params2[3] = {X_BOUND,Y_BOUND,MAX_ITERATIONS};
	double params2[5] = {atof(argv[1]),atof(argv[2]),atof(argv[3]),atof(argv[4]),atof(argv[5])};
	printf("DEBUG: process frame buffer.\n");
	process_fbuf(frame_buffer,mandelbrot_set,params2,sizeof(params2),X_BOUND,Y_BOUND);
	printf("DEBUG: Write frame buffer to disk.\n");
	write_fbuf_to_disk(frame_buffer,X_BOUND,Y_BOUND);

	return EXIT_SUCCESS;

}
void process_fbuf(Pixel frame_buffer[X_BOUND][Y_BOUND],Pixel(*fn)(double,double,double*,int),double* params,int pcnt,int x_bound,int y_bound)
{
	for (int i=0;i<x_bound;i++)
		for (int j=0;j<y_bound;j++)
		{
			frame_buffer[i][j] = fn(i,j,params,pcnt);
			printf ("DEBUG: x=%d,y=%d,red=%d,grn=%d,blu=%d\n",i,j,frame_buffer[i][j].red,frame_buffer[i][j].grn,frame_buffer[i][j].blu);
		}
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
	strncat(framename,"mandelbrot.ppm",14);
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

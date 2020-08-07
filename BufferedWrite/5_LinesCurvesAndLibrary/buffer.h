#ifndef BUFFER_H
#define BUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
//#define MAX_COLOR_VAL 255
extern const int MAX_COLOR_VAL;
//#define MAX_ITERATIONS 255
//#define PIXELS_PER_LINE 2
extern const int PIXELS_PER_LINE;
//#define X_BOUND 1024
extern const int X_BOUND;
//#define Y_BOUND 1024
extern const int Y_BOUND;

//pixels store colors
typedef struct Pixel
{
	uint8_t red;
	uint8_t grn;
	uint8_t blu;
} Pixel;

//tuples store x,y coordinates
//could also store color
typedef struct Tuple
{
	double x;
	double y;
} Tuple;

//scale number in interval [min,max] to [a,b]
double scale_to_interval(double,double,double,double,double);
//print info about tuple to std out
void debug_print_tuple(char* msg,Tuple* tp);
//write the frame buffer to disk
void write_fbuf_to_disk(char*,Pixel[X_BOUND][Y_BOUND],int,int);
//write image header to disk
void writeimageheader(FILE*);
//write pixel to disk or frame buffer
void plot_disk(FILE*,int,int,Pixel[X_BOUND][Y_BOUND]);
void plot_fbuf(int,int,Pixel,Pixel[X_BOUND][Y_BOUND]);
//process the frame buffer using function fn
void process_fbuf(Pixel frame_buffer[X_BOUND][Y_BOUND],Pixel(*fn)(double,double,double*,int),double* params,int pcnt,int x_bound,int y_bound);
//fill entire frame buffer with a single color
void fill_fbuf_color(Pixel frame_buffer[X_BOUND][Y_BOUND],int,int,Pixel);

//build an array of pts (x,y) using a recurrence relation
//this requires you to pass (x0,y0) and f s.t. (x_n,y_n)=f(x_(n-1),y_(n-1))
void generate_pt_arr_to_fbuf(Tuple*,Tuple,Tuple(*fn)(Tuple,double*,int),double*,int,int,int,int);
//write an array of points to the frame buffer
void write_pt_arr_to_fbuf(Pixel[X_BOUND][Y_BOUND],Tuple*,int,int,int);
//read in an array of coords x0 y0 x1 y1 ... and plot line segments connecting them to the frame buffer
void bresenham_lines_array(Pixel[X_BOUND][Y_BOUND],Pixel,double *,int);
//plot a line segment between two points
void bresenham_plotline(Pixel[X_BOUND][Y_BOUND],Pixel,double,double,double,double);
#endif

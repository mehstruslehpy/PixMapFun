#ifndef BUFFER_H
#define BUFFER_H

#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#define NDEBUG

extern const int MAX_COLOR_VAL;
extern const int PIXELS_PER_LINE;
extern const int X_BOUND;
extern const int Y_BOUND;

//pixels store colors
typedef struct Pixel
{
	uint8_t red;
	uint8_t grn;
	uint8_t blu;
} Pixel;

//tuples store x,y,z coordinates
//could also store color
typedef struct Tuple
{
	double x;
	double y;
	double z;
} Tuple;

//Simple triangle structure
typedef struct Triangle 
{
	Tuple vert1;
	Tuple vert2;
	Tuple vert3;
} Triangle;

//allocate a frame buffer with the given bounds
Pixel** alloc_framebuf(int,int);
//deallocate a previously allocated frame buffer
void dealloc_framebuf(Pixel**,int);
//copy pixels from a window frame buffer into the viewport frame buffer
void copy_into_view(Pixel**,Tuple,Tuple,Pixel**,Tuple,Tuple);
//copy pixels from a window frame buffer into the viewport frame buffer using
//a precomputed xform matrix
void fast_copy_into_view(double**,Pixel**,Tuple,Tuple,Pixel**,Tuple,Tuple);

//scale number in interval [min,max] to [a,b]
double scale_to_interval(double,double,double,double,double);
//convert from some external models coordinates to viewport/frame buffer coordinates
Tuple xform_window_to_view(Tuple,Tuple,Tuple,Tuple,Tuple);
//faster window to view xform by precomputing the xform
double** alloc_window_to_view_matrix(Tuple wmax,Tuple wmin,Tuple vmax,Tuple vmin);
void dealloc_window_to_view_matrix(double** xform);
Tuple fast_window_view_xform(double** xform,Tuple pt);

//print info about tuple to std out
void debug_print_tuple(char* msg,Tuple* tp);
//void debug_print_matrix(double**,int,int);
//write the frame buffer to disk
void write_fbuf_to_disk(char*,Pixel**,int,int);
//write image header to disk
void writeimageheader(FILE*,int,int);
//write pixel to disk or frame buffer
void plot_disk(FILE*,int,int,Pixel**,int,int);
void plot_fbuf(int,int,Pixel,Pixel**,int,int);
//read pixel from frame buffer
Pixel read_fbuf(int,int,Pixel**,int,int);
//process the frame buffer using function fn
void process_fbuf(Pixel(*)(double,double,double*,int),double*,int,Pixel**,int,int);
//fill entire frame buffer with a single color
void fill_fbuf_color(Pixel,Pixel** frame_buffer,int,int);

//build an array of pts (x,y) using a recurrence relation
//this requires you to pass (x0,y0) and f s.t. (x_n,y_n)=f(x_(n-1),y_(n-1))
void generate_pt_arr_to_fbuf(Tuple*,Tuple,Tuple(*fn)(Tuple,double*,int),double*,int,int,int,int);
//write an array of points to the frame buffer
void write_pt_arr_to_fbuf(Tuple*,int,Pixel**,int,int);
//read in an array of coords x0 y0 x1 y1 ... and plot line segments connecting them to the frame buffer
void bresenham_lines_array(Pixel,double *,int,Pixel**,int,int);
//plot a line segment between two points
void bresenham_plotline(Pixel,double,double,double,double,Pixel**,int,int);
#endif

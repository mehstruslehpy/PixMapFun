#ifndef BUFFER_H
#define BUFFER_H

#include "matrix.h"
#include "vector.h"
#include "polygon.h"
#include "wireframe.h"
#include "bsptree.h"
#include "xform.h"
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
extern const int Z_BOUND;

//basic frame buffer functions
//allocate a frame buffer with the given bounds
Pixel** alloc_framebuf(int,int);
//deallocate a previously allocated frame buffer
void dealloc_framebuf(Pixel**,int);
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
void generate_pt_arr_to_fbuf(Vector*,Vector,Vector(*fn)(Vector,double*,int),double*,int,int,int,int);
//write an array of points to the frame buffer
void write_pt_arr_to_fbuf(Vector*,int,Pixel**,int,int);
#endif

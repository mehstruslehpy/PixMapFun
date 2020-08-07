#ifndef ZBUFFER_H
#define ZBUFFER_H

#include "buffer.h"
#define NDEBUG

double** alloc_zbuf(int,int);
void init_zbuf(double**,int,int);
void fill_zbuf(double,double**,int,int);
//deallocate a previously allocated frame buffer
void dealloc_zbuf(double**,int);
//write the frame buffer to disk
void write_zbuf_to_disk(char*,double**,int,int);
//write image header to disk
//void writeimageheader(FILE*,int,int);
//write pixel to disk or frame buffer
//void plot_disk(FILE*,int,int,Pixel**,int,int);
void plot_zbuf_fbuf(int,int,double,Pixel,Pixel**,double**,int,int);
//read pixel from frame buffer
//Pixel read_fbuf(int,int,Pixel**,int,int);
//process the frame buffer using function fn
//void process_zbuf_fbuf(Pixel(*)(double,double,double*,int),double*,int,Pixel**,int,int);
//fill entire frame buffer with a single color
void fill_zbuf(double,double**,int,int);
//build an array of pts (x,y) using a recurrence relation
//this requires you to pass (x0,y0) and f s.t. (x_n,y_n)=f(x_(n-1),y_(n-1))
//void generate_pt_arr_to_fbuf(Vector*,Vector,Vector(*fn)(Vector,double*,int),double*,int,int,int,int);
//write an array of points to the frame buffer
//void write_pt_arr_to_fbuf(Vector*,int,Pixel**,int,int);
#endif

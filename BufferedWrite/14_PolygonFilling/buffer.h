#ifndef BUFFER_H
#define BUFFER_H

#include "matrix.h"
#include "vector.h"
#include "polygon.h"
#include "wireframe.h"
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

//pixels store colors
typedef struct Pixel
{
	uint8_t red;
	uint8_t grn;
	uint8_t blu;
} Pixel;

//Simple triangle structure
typedef struct Triangle 
{
	Vector vert1;
	Vector vert2;
	Vector vert3;
} Triangle;

//Specifies a view volume
typedef struct ViewVolume 
{
	double l;
	double r;
	//double b;
	//double t;
	double t;
	double b;
	double n;
	double f;
} ViewVolume;

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

//draw various things
void draw_triangle(Pixel,Triangle,Pixel**,int,int);
void draw_segment(Pixel,Vector,Vector,Pixel**,int,int);
void draw_line(Pixel,Vector,Vector,Pixel**,int,int);
void draw_polygon(Pixel,Polygon,Pixel**,int,int);
void draw_wireframe(Pixel,Wireframe,Pixel**,int,int);
//read in an array of coords x0 y0 x1 y1 ... and plot line segments connecting them to the frame buffer
void bresenham_lines_array(Pixel,double *,int,Pixel**,int,int);
//plot a line segment between two points
void bresenham_plotline(Pixel,double,double,double,double,Pixel**,int,int);

//fill regions these do not have separate options for outlines
//we will follow the triangle rasterizing algorithm from the Shirley book
void fillcolor_triangle(Pixel,Triangle,Pixel**,int,int);
//any concave polygon can be decomposed into triangles hence we can use the above function to
//implement arbitrary concave polygons
void fillcolor_convex_polygon(Pixel,Polygon,Pixel**,int,int);
//later we can try to break convex polygons into concave polygons and apply fill in terms of these
//this is the helper function on pg 64 of the book for computing alpha beta gamma
double fij(Vector verti,Vector vertj,double x, double y);

//some functions for projections and transformations for windowing and viewing
void debug_print_view_volume1(char* msg,ViewVolume* vol);
void debug_print_view_volume2(char* msg,ViewVolume* vol);
Triangle orth_project_triangle(Triangle,Vector,Vector,Vector,ViewVolume,int,int);
Vector orth_project_vector(Vector,Vector,Vector,Vector,ViewVolume,int,int);
Triangle perspective_project_triangle(Triangle,Vector,Vector,Vector,ViewVolume,int,int);
Vector perspective_project_vector(Vector,Vector,Vector,Vector,ViewVolume,int,int);
//copy pixels from a window frame buffer into the viewport frame buffer
void copy_into_view(Pixel**,Vector,Vector,Pixel**,Vector,Vector);
//copy pixels from a window frame buffer into the viewport frame buffer using
//a precomputed xform matrix
void fast_copy_into_view(double**,Pixel**,Vector,Vector,Pixel**,Vector,Vector);
//scale number in interval [min,max] to [a,b]
double scale_to_interval(double,double,double,double,double);
//convert from some external models coordinates to viewport/frame buffer coordinates
Vector xform_window_to_view(Vector,Vector,Vector,Vector,Vector);
//faster window to view xform by precomputing the xform
double** alloc_window_to_view_matrix(Vector wmax,Vector wmin,Vector vmax,Vector vmin);
void dealloc_window_to_view_matrix(double** xform);
Vector fast_window_view_xform(double** xform,Vector pt);

#endif

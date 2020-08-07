#ifndef XFORM_H 
#define XFORM_H
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "polygon.h"
#include "matrix.h"
#define NDEBUG
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
//some functions for projections and transformations for windowing and viewing
void debug_print_view_volume1(char* msg,ViewVolume* vol);
void debug_print_view_volume2(char* msg,ViewVolume* vol);
Triangle orth_project_triangle(Triangle,Vector,Vector,Vector,ViewVolume,int,int);
Vector orth_project_vector(Vector,Vector,Vector,Vector,ViewVolume,int,int);
Triangle perspective_project_triangle(Triangle,Vector,Vector,Vector,ViewVolume,int,int);
Vector perspective_project_vector(Vector,Vector,Vector,Vector,ViewVolume,int,int);
//void perspective_project_bsp_tree(BSPNode*,Vector,Vector,Vector,ViewVolume,int,int);
void perspective_project_polygon(Polygon* poly,Vector e,Vector g,Vector t,ViewVolume vol,int x_bound,int y_bound);
//copy pixels from a window frame buffer into the viewport frame buffer
void copy_into_view(Pixel**,Vector,Vector,Pixel**,Vector,Vector);
//copy pixels from a window frame buffer into the viewport frame buffer using
//a precomputed xform matrix
void fast_copy_into_view(double**,Pixel**,Vector,Vector,Pixel**,Vector,Vector);
//scale number in interval [min,max] to [a,b]
double scale_to_interval(double,double,double,double,double);
//convert from some external models coordinates to viewport/frame buffer coordinates
Vector xform_window_to_view(Vector,Vector,Vector,Vector,Vector);
void xform_polygon_window_to_view(Polygon* poly,Vector wmax,Vector wmin,Vector vmax,Vector vmin);
//void xform_bsp_tree_window_to_view(BSPNode*,Vector,Vector,Vector,Vector);
//faster window to view xform by precomputing the xform
double** alloc_window_to_view_matrix(Vector wmax,Vector wmin,Vector vmax,Vector vmin);
void dealloc_window_to_view_matrix(double** xform);
Vector fast_window_view_xform(double** xform,Vector pt);
#endif

#ifndef POLYGON_H
#define POLYGON_H

#include "assert.h"
#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#define NDEBUG

//a polygon is an array of Vectors
typedef struct Polygon
{
	Vector* pt; //vertices
	Pixel* color; //color at vertex
	int count;
} Polygon;

//print a polygon for debugging purposes
void debug_print_polygon(char* msg,Polygon* tp);
//print a the colors of a polygon for debugging purposes
void debug_print_polygon_color(char* msg,Polygon* tp);
//print a the vectors of a polygon for debugging purposes
void debug_print_polygon_vector(char* msg,Polygon* tp);
//this is for the bsp algorithm in ch 8 of shirley
double f(Polygon poly,Vector p);
//for allocating the pt and color elements of a polygon dynamically
//you still have to allocate the actual polygon itself if you want 
//the whole thing to be dynamically allocated
void alloc_poly(Polygon*,int);
//for deallocating the contents of a polygon, this should be used
//with the previous function
void dealloc_poly(Polygon*);
//copy one polygon to another polygon
void copy_poly(Polygon*,Polygon*);
//copy the first n-points from one polygon to another polygon
void copy_poly_n(Polygon* src,Polygon* dst,int n);
//when given a convex polygon this function allocates and returns an array containing 
//the original polygon decomposed into triangular pieces
Polygon* triangulate_convex_poly(Polygon* poly);
#endif

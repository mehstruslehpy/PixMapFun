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
	Vector* pt;
	int count;
} Polygon;

void debug_print_polygon(char* msg,Polygon* tp);
//for allocating the array part of a polygon dynamically
//these are useful if you cannot compute the amount of vectors
//at compile time you can also point pt to a static array of vectors
void alloc_poly(Polygon*,int);
void dealloc_poly(Polygon*);
#endif

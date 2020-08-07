#ifndef VECTOR_H
#define VECTOR_H

#include "assert.h"
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#define NDEBUG

//tuples store x,y,z coordinates
//could also store color
typedef struct Vector
{
	double x;
	double y;
	double z;
} Vector;

void debug_print_vector(char* msg,Vector* tp);

//some vector math functions
Vector cross_product(Vector,Vector);
double dot_product(Vector,Vector);
Vector normalize(Vector);
Vector scalar_mul(double,Vector);
#endif

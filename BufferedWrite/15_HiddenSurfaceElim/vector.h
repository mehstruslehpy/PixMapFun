#ifndef VECTOR_H
#define VECTOR_H
//TODO: this should probably be split into two files
#include "assert.h"
#include <math.h>
#include <float.h>
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

//pixels are effectively a kind of vectore
//that stores colors
typedef struct Pixel
{
	uint8_t red;
	uint8_t grn;
	uint8_t blu;
} Pixel;

//some vector math functions
void debug_print_vector(char* msg,Vector* tp);
Vector cross_product(Vector,Vector);
double dot_product(Vector,Vector);
Vector normalize(Vector);
Vector scalar_mul(double,Vector);
Vector add(Vector,Vector);
Vector sub(Vector,Vector);
double determinant(Vector,Vector,Vector);
//some pixel math functions
void debug_print_pixel(char* msg,Pixel* tp);
Pixel cross_product_pixel(Pixel,Pixel);
double dot_product_pixel(Pixel,Pixel);
Pixel normalize_pixel(Pixel);
Pixel scalar_mul_pixel(double,Pixel);
Pixel add_pixel(Pixel,Pixel);
Pixel sub_pixel(Pixel,Pixel);
double fij(Vector verti,Vector vertj,double x, double y);
#endif

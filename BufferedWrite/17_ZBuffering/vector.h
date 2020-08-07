#ifndef VECTOR_H
#define VECTOR_H
//TODO: should I split this into two files?
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
	//double z_buf; //only for storing z buffer values
} Vector;

//pixels are effectively a kind of vectore
//that stores colors but in this case we are using
//discrete elements
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
//note that currently our normalize implementation
//will not check or handle div by zero
Vector normalize(Vector);
Vector scalar_mul(double,Vector);
int vector_equal(Vector,Vector);
Vector add(Vector,Vector);
Vector sub(Vector,Vector);
//calculate the determinant of the matrix containing
//the three given vectors as columns
double determinant(Vector,Vector,Vector);
//some pixel math functions
void debug_print_pixel(char* msg,Pixel* tp);
Pixel cross_product_pixel(Pixel,Pixel);
double dot_product_pixel(Pixel,Pixel);
Pixel normalize_pixel(Pixel);
Pixel scalar_mul_pixel(double,Pixel);
Pixel add_pixel(Pixel,Pixel);
Pixel sub_pixel(Pixel,Pixel);
//this functions is used for color interpolation in Shirley
double fij(Vector verti,Vector vertj,double x, double y);
Pixel interpolate_color(Vector,Vector,Vector,Vector,Pixel,Pixel,Pixel);
//some swap functions
void swap_double(double* a, double* b);
void swap_vector(Vector* a, Vector* b);
#endif

#include "buffer.h"
#ifndef MATRIX_H 
#define MATRIX_H
#define NDEBUG
//these functions are written under the assumption that the users always
//pass good pointers and correct dimensions

//some typedefs for clarity
typedef double** FourByFour;
typedef double** FourByOne;
typedef double** ThreeByThree;
typedef double** ThreeByOne;
typedef double** TwoByTwo; typedef double** TwoByOne;

//copy nxm matrix a to txu matrix b
void matrix_copy(double** a,int n,int m, double** b,int t,int u);
//write all zeros to the given n by m matrix
void matrix_clear(double** dst,int n,int m);
//clear nxu matrix dst then multiply nxm matrix a by txu matrix b store into dst
void matrix_mult(double** dst, double** a,int n,int m, double** b,int t,int u);
//nxu matrix dst is overwritten by nxm matrix a plus txu matrix b
void matrix_add(double** dst, double** a,int n,int m, double** b,int t,int u);
void debug_print_matrix(double**,int,int);

//a few basic functions for constructing matrices on the stack
FourByFour make_4by4(double,double,double,double,
		double,double,double,double,
		double,double,double,double,
		double,double,double,double);
FourByOne make_4by1(double,double,double,double);
ThreeByThree make_3by3(double,double,double,
		double,double,double,
		double,double,double);
TwoByTwo make_2by2(double,double,
		double,double);
ThreeByOne make_3by1(double,double,double);
TwoByOne make_2by1(double,double);
//for applying basic matrix transformations
TwoByOne apply_matrix_xform2(TwoByTwo,TwoByOne);
ThreeByOne apply_matrix_xform3(ThreeByThree,ThreeByOne);
FourByOne apply_matrix_xform4(FourByFour,FourByOne);
TwoByTwo compose_2(TwoByTwo,TwoByTwo);
ThreeByThree compose_3(ThreeByThree,ThreeByThree);
FourByFour compose_4(FourByFour,FourByFour);
#endif

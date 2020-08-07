#include "buffer.h"
#ifndef MATRIX_H 
#define MATRIX_H
#define NDEBUG
//TODO: add more functionality
//these functions are written under the assumption that the users always
//pass good pointers and correct dimensions

//copy nxm matrix a to txu matrix b
void matrix_copy(double** a,int n,int m, double** b,int t,int u);
//write all zeros to the given n by m matrix
void matrix_clear(double** dst,int n,int m);
//clear nxu matrix dst then multiply nxm matrix a by txu matrix b store into dst
void matrix_mult(double** dst, double** a,int n,int m, double** b,int t,int u);
//nxu matrix dst is overwritten by nxm matrix a plus txu matrix b
void matrix_add(double** dst, double** a,int n,int m, double** b,int t,int u);
//print an nxu matrix for debugging purposes
void debug_print_matrix(double**,int,int);
#endif

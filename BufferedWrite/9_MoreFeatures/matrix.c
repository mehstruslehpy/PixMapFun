#include "matrix.h"
void debug_print_matrix(double** A,int m, int n)
{
	for (int i=0;i<m;i++)
	{
		printf("[ ");
		for (int j=0;j<n;j++)
		{
			printf("%f ",A[i][j]);
		}
		printf("]\n");
	}
	//printf("\n");
}

void matrix_copy(double** a,int n,int m, double** b,int t,int u)
{
	assert(a!=NULL);
	assert(b!=NULL);
	assert(n==t);
	assert(m==u);

	//result is an nxu matrix
	for (int i=0;i<n;i++)
	{
		for (int j=0;j<m;j++)
		{
			a[i][j]=b[i][j];
		}
	}
}


void matrix_clear(double** dst,int n,int m)
{
	assert(dst!=NULL);

	for (int i=0;i<n;i++)
	{
		for (int j=0;j<m;j++)
		{
			dst[i][j]=0;
		}
	}
}


void matrix_mult(double** dst, double** a,int n,int m, double** b,int t,int u)
{
	assert(m==t);
	assert(a!=NULL);
	assert(b!=NULL);
	assert(dst!=NULL);

	//result is an nxu matrix
	matrix_clear(dst,n,u);
	for (int i=0;i<n;i++)
	{
		for (int j=0;j<u;j++)
		{
			//for (int k=0;k<u;k++)
			for (int k=0;k<m;k++)
			{
				dst[i][j]+=a[i][k]*b[k][j];
			}
		}
	}
}
void matrix_add(double** dst, double** a,int n,int m, double** b,int t,int u)
{
	assert(a!=NULL);
	assert(b!=NULL);
	assert(dst!=NULL);
	assert(n==t);
	assert(m==u);

	for (int i=0;i<n;i++)
	{
		for (int j=0;j<m;j++)
		{
			for (int k=0;k<u;k++)
			{
				dst[i][j]=a[i][j]+b[i][j];
			}
		}
	}
}
FourByFour make_4by4(double a11,double a12,double a13,double a14,
		double a21,double a22,double a23,double a24,
		double a31,double a32,double a33,double a34,
		double a41,double a42,double a43,double a44)
{
	return (double*[]){
		(double[]){a11,a12,a13,a14},
		(double[]){a21,a22,a23,a24},
		(double[]){a31,a32,a33,a34},
		(double[]){a41,a42,a43,a44}
	};
}
FourByOne make_4by1(double a,double b,double c,double d)
{
	return (double*[]){(double[]){a},(double[]){b},(double[]){c},(double[]){d}};
}
ThreeByThree make_3by3(double a11,double a12,double a13,
		double a21,double a22,double a23,
		double a31,double a32,double a33)
{
	return (double *[]){
		(double[]){a11,a12,a13},
		(double[]){a21,a22,a23},
		(double[]){a31,a32,a33}
	};
}

TwoByTwo make_2by2(double a11,double a12,
		double a21,double a22)
{
	return (double *[]){
		(double[]){a11,a12},
		(double[]){a21,a22}
	};
}
ThreeByOne make_3by1(double a,double b,double c)
{
	return (double*[]){(double[]){a},(double[]){b},(double[]){c}};
}
TwoByOne make_2by1(double a,double b)
{
	return (double*[]){(double[]){a},(double[]){b}};
}
FourByOne apply_matrix_xform4(FourByFour mat,FourByOne in)
{
	FourByOne out = make_4by1(0,0,0,0);
	matrix_mult(out,mat,4,4,in,4,1);
	return make_4by1(out[0][0],out[1][0],out[2][0],out[3][0]);
}
ThreeByOne apply_matrix_xform3(ThreeByThree mat,ThreeByOne in)
{
	ThreeByOne out = make_3by1(0,0,0);
	matrix_mult(out,mat,3,3,in,3,1);
	return make_3by1(out[0][0],out[1][0],out[2][0]);
}
TwoByOne apply_matrix_xform2(TwoByTwo mat,TwoByOne in)
{
	TwoByOne out = make_2by1(0,0);
	matrix_mult(out,mat,2,2,in,2,1);
	return make_2by1(out[0][0],out[1][0]);
}
TwoByTwo compose_2(TwoByTwo a,TwoByTwo b)
{
	TwoByOne out = make_2by2(0,0,0,0);
	matrix_mult(out,a,2,2,b,2,1);
	return make_2by2(out[0][0],out[0][1],
			out[1][0],out[1][1]);
}
ThreeByThree compose_3(ThreeByThree a,ThreeByThree b)
{
	ThreeByThree out = make_3by3(0,0,0,
			0,0,0,
			0,0,0);
	matrix_mult(out,a,3,3,b,3,1);
	return make_3by3(out[0][0],out[0][1],out[0][2],
			out[1][0],out[1][1],out[1][2],
			out[2][0],out[2][1],out[2][2]);
}
FourByFour compose_4(FourByFour a,FourByFour b)
{
	FourByFour out = make_4by4(0,0,0,0,
			0,0,0,0,
			0,0,0,0,
			0,0,0,0);
	matrix_mult(out,a,4,4,b,4,1);
	return make_4by4(out[0][0],out[0][1],out[0][2],out[0][3],
			out[1][0],out[1][1],out[1][2],out[1][3],
			out[2][0],out[2][1],out[2][2],out[2][3],
			out[3][0],out[3][1],out[3][2],out[3][3]);
}


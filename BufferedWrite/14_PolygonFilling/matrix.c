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

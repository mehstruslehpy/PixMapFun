#include "../buffer.h"
#include <time.h>
//draw a line between an array of points
int main(int argc, char** argv)
{
	double** A=(double*[]){(double[]){atof(argv[1]),atof(argv[2])}, (double[]){atof(argv[3]),atof(argv[4])}};
	double** B=(double*[]){(double[]){atof(argv[5])}, (double[]){atof(argv[6])}};
	double** C=(double*[]){(double[]){0}, (double[]){0}};

	printf("A:\n");
	debug_print_matrix(A,2,2);
	printf("B:\n");
	debug_print_matrix(B,2,1);

	matrix_mult(C,A,2,2,B,2,1);

	printf("C:\n");
	debug_print_matrix(C,2,1);
	printf("\n");

	return EXIT_SUCCESS;
}

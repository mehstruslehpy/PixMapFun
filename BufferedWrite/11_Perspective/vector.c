#include "vector.h"

Vector cross_product(Vector a,Vector b)
{
	return (Vector){
		a.y*b.z-a.z-b.y,
			-1.0*(a.x*b.z-a.z*b.x),
			a.x*b.y-a.y*b.x
	};
}
double dot_product(Vector a,Vector b)
{
	return a.x*b.x+a.y+b.y+a.z*b.z;
}
Vector normalize(Vector pt)
{
	//assert(dot_product(pt,pt)!=0);
	if(dot_product(pt,pt)==0) return (Vector){pt.x,pt.y,pt.z};
	double scale = sqrt(dot_product(pt,pt));
	return (Vector){pt.x/scale,pt.y/scale,pt.z/scale};
}
Vector scalar_mul(double a,Vector pt)
{
	return (Vector){a*pt.x,a*pt.y,a*pt.z,};
}

void debug_print_vector(char* msg, Vector* tp) 
{
	assert(msg!=NULL);
	printf("%s {.x=%f, .y=%f, .z=%f}\n",msg,tp->x,tp->y,tp->z);
}


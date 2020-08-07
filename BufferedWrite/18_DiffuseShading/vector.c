#include "vector.h"
int vector_equal(Vector a,Vector b)
{
	return a.x==b.x&&a.y==b.y&&a.z==b.z;
}
Vector normal_vector(Vector a,Vector b,Vector c)
{
	//return cross_product(sub(b,a),sub(c,a));
	//points are clockwise
	if (determinant(a,b,c)>0) return cross_product(sub(b,a),sub(c,a));
	else return cross_product(sub(c,a),sub(b,a));
}
//this could probably used to clean up the fill triangle function
//we should also probably write a more general version which isn't only for colors
Pixel interpolate_color(Vector new,Vector vert1,Vector vert2,Vector vert3,Pixel color1,Pixel color2 ,Pixel color3)
{
	double falpha = fij(vert2,vert3,vert1.x,vert1.y);
	double fbeta = fij(vert3,vert1,vert2.x,vert2.y);
	double fgamma = fij(vert1,vert2,vert3.x,vert3.y);
	double alpha = fij(vert2,vert3,new.x,new.y)/falpha;
	double beta = fij(vert3,vert1,new.x,new.y)/fbeta;
	double gamma = fij(vert1,vert2,new.x,new.y)/fgamma;
	Pixel temp_a=scalar_mul_pixel(alpha,color1);
	Pixel temp_b=scalar_mul_pixel(beta,color2);
	Pixel temp_c=scalar_mul_pixel(gamma,color3);
	Pixel new_color=add_pixel(temp_a,add_pixel(temp_b,temp_c));
	return new_color;
}
void swap_double(double* a, double* b)
{
	double temp = *a;
	*a = *b;
	*b = temp;
}
void swap_vector(Vector* a, Vector* b)
{
	Vector temp = *a;
	*a = *b;
	*b = temp;
}
double determinant(Vector a,Vector b,Vector c)
{
	double det1 = a.x*(b.y*c.z-c.y*b.z);	
	double det2 = -b.x*(a.y*c.z-c.y*a.z);
	double det3 = c.x*(a.y*b.z-b.y*a.z);
	return det1+det2+det3;
}
Vector cross_product(Vector a,Vector b)
{
	return (Vector){
		a.y*b.z-a.z*b.y,
			-1.0*(a.x*b.z-a.z*b.x),
			a.x*b.y-a.y*b.x
	};
}
double dot_product(Vector a,Vector b)
{
	return a.x*b.x+a.y*b.y+a.z*b.z;
}
Vector normalize(Vector pt)
{
	assert(dot_product(pt,pt)!=0);
	//if(dot_product(pt,pt)==0) return (Vector){pt.x,pt.y,pt.z};
	double scale = sqrt(dot_product(pt,pt));
	return (Vector){pt.x/scale,pt.y/scale,pt.z/scale};
}
Vector scalar_mul(double a,Vector pt)
{
	//a*pt.z_buf only matters when we are doing z buffering
	//return (Vector){a*pt.x,a*pt.y,a*pt.z,a*pt.z_buf};
	return (Vector){a*pt.x,a*pt.y,a*pt.z};
}

void debug_print_vector(char* msg, Vector* tp) 
{
	assert(msg!=NULL);
	printf("%s {.x=%f, .y=%f, .z=%f}\n",msg,tp->x,tp->y,tp->z);
}
Vector add(Vector a,Vector b)
{
	//a.z_buf+b.z_buf only matters when we are doing z buffering
	return (Vector){a.x+b.x,a.y+b.y,a.z+b.z};
	//return (Vector){a.x+b.x,a.y+b.y,a.z+b.z,a.z_buf+b.z_buf};
}
Vector sub(Vector a,Vector b)
{
	return add(a,scalar_mul(-1,b));
}
//pixel functions
void debug_print_pixel(char* msg, Pixel* tp) 
{
	assert(msg!=NULL);
	printf("%s {.red=%d, .grn=%d, .blu=%d}\n",msg,tp->red,tp->grn,tp->blu);
}

Pixel cross_product_pixel(Pixel a,Pixel b)
{
	return (Pixel){
		a.grn*b.blu-a.blu*b.grn,
			-1.0*(a.red*b.blu-a.blu*b.red),
			a.red*b.grn-a.grn*b.red
	};
}
double dot_product_pixel(Pixel a,Pixel b)
{
	return a.red*b.red+a.grn*b.grn+a.blu*b.blu;
}
Pixel normalize_pixel(Pixel pt)
{
	//assert(dot_product_pixel(pt,pt)!=0);
	if(dot_product_pixel(pt,pt)==0) return (Pixel){pt.red,pt.grn,pt.blu};
	double scale = sqrt(dot_product_pixel(pt,pt));
	return (Pixel){pt.red/scale,pt.grn/scale,pt.blu/scale};
}
Pixel scalar_mul_pixel(double a,Pixel pt)
{
	return (Pixel){a*pt.red,a*pt.grn,a*pt.blu};
}

Pixel add_pixel(Pixel a,Pixel b)
{
	return (Pixel){a.red+b.red,a.grn+b.grn,a.blu+b.blu,};
}
Pixel sub_pixel(Pixel a,Pixel b)
{
	return add_pixel(a,scalar_mul_pixel(-1,b));
}
double fij(Vector verti,Vector vertj,double x, double y)
{
	return (verti.y-vertj.y)*x+(vertj.x-verti.x)*y+verti.x*vertj.y-vertj.x*verti.y;
}

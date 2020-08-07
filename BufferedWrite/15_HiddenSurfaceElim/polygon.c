#include "polygon.h"

void alloc_poly(Polygon* p,int count)
{
	assert(p!=NULL);
	p->pt=malloc(count*sizeof(Vector));
	p->color=malloc(count*sizeof(Pixel));
	p->count=count;
}
void copy_poly(Polygon* src,Polygon* dst)
{
	assert(src->count==dst->count);
	copy_poly_n(src,dst,src->count);
}
void copy_poly_n(Polygon* src,Polygon* dst,int n)
{
	assert(src!=NULL);
	assert(dst!=NULL);
	assert(src->count>=n);
	assert(dst->count>=n);
	for (int i=0;i<n;i++)
	{
		dst->pt[i]=src->pt[i];
		dst->color[i]=src->color[i];
	}
}
void dealloc_poly(Polygon* p)
{
	free(p->pt);
	free(p->color);
	p->pt=NULL;
	p->color=NULL;
}
void debug_print_polygon(char* msg, Polygon* p) 
{
	assert(msg!=NULL);
	assert(p!=NULL);
	printf("%s\n",msg);
	printf("\tp.count=%d\n",p->count);
	for (int i=0;i<p->count;i++)
	{
		printf("\tpoint %d:",i);
		debug_print_vector("",&(p->pt[i]));
		printf("\tpoint %d:",i);
		debug_print_pixel("",&(p->color[i]));
	}
}
void debug_print_polygon_vector(char* msg, Polygon* p) 
{
	assert(msg!=NULL);
	assert(p!=NULL);
	printf("%s\n",msg);
	printf("\tp.count=%d\n",p->count);
	for (int i=0;i<p->count;i++)
	{
		printf("\tpoint %d:",i);
		debug_print_pixel("",&(p->color[i]));
	}
}
void debug_print_polygon_color(char* msg, Polygon* p) 
{
	assert(msg!=NULL);
	assert(p!=NULL);
	//printf("%s {.x=%f, .y=%f, .z=%f}\n",msg,tp->x,tp->y,tp->z);
	printf("%s\n",msg);
	printf("\tp.count=%d\n",p->count);
	for (int i=0;i<p->count;i++)
	{
		printf("\tpoint %d:",i);
		debug_print_pixel("",&(p->color[i]));
	}
}
//this only works for convex polygons
Polygon* triangulate_convex_poly(Polygon* poly)
{
	//expect the user to pass us something we can break up into more pieces
	assert(poly->count>3);
	int tri_count=(poly->count-2);
	printf("DEBUG: triangulate_convex_poly() splitting into tri_count=%d triangles\n",tri_count);
	Polygon* triangles = malloc(tri_count*sizeof(Polygon));
	alloc_poly(&(triangles[0]),3);
	triangles[0].pt[0]=poly->pt[0];
	triangles[0].pt[1]=poly->pt[1];
	triangles[0].pt[2]=poly->pt[2];
	triangles[0].color[0]=poly->color[0];
	triangles[0].color[1]=poly->color[1];
	triangles[0].color[2]=poly->color[2];
	
	for (int i = 1;i<poly->count-2;i++)
	{
		//printf("DEBUG: triangulate_convex_poly() poly->count=%d,triangles[%d].count=%d\n",poly->count,i,triangles[0].count);
		alloc_poly(&(triangles[i]),3);
		//copy_poly_n(poly,&(triangles[i]),3);
		//triangles[i].pt[0]=poly->pt[i];
		triangles[i].pt[0]=poly->pt[0];
		triangles[i].pt[1]=poly->pt[i+1];
		triangles[i].pt[2]=poly->pt[i+2];
		triangles[i].color[0]=poly->color[0];
		triangles[i].color[1]=poly->color[i+1];
		triangles[i].color[2]=poly->color[i+2];
	}
	return triangles;
}

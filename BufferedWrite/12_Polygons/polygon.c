#include "polygon.h"

void alloc_poly(Polygon* p,int count)
{
	p->count=count;
	p->pt=malloc(count*sizeof(Vector));
}
void dealloc_poly(Polygon* p)
{
	free(p->pt);
	p->pt=NULL;
}
void debug_print_polygon(char* msg, Polygon* p) 
{
	assert(msg!=NULL);
	assert(p!=NULL);
	//printf("%s {.x=%f, .y=%f, .z=%f}\n",msg,tp->x,tp->y,tp->z);
	printf("%s\n",msg);
	printf("\tp.count=%d\n",p->count);
	for (int i=0;i<p->count;i++)
	{
		printf("\tpoint %d:",i);
		debug_print_vector("",&(p->pt[i]));
	}
}


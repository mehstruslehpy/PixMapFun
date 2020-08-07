#ifndef WIREFRAME_H
#define WIREFRAME_H

#include "assert.h"
#include "polygon.h"
//#include "buffer.h"
#include "file.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define NDEBUG

//A wireframe is an array of polygons
typedef struct Wireframe
{
	Polygon* poly;
	int count;
} Wireframe;

void debug_print_wireframe(char* msg,Wireframe* w);
void read_wireframe_from_disk(char* wire_fname,char* color_fname ,Wireframe* w);
void alloc_wire(Wireframe*,int);
void dealloc_wire(Wireframe*);
#endif

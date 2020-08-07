#ifndef WIREFRAME_H
#define WIREFRAME_H

#include "assert.h"
#include "polygon.h"
#include "file.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define NDEBUG

typedef struct Wireframe
{
	Polygon* poly;
	int count;
} Wireframe;

void debug_print_wireframe(char* msg,Wireframe* w);
void read_wireframe_from_disk(char* fname,Wireframe* w);
void alloc_wire(Wireframe*,int);
void dealloc_wire(Wireframe*);
#endif

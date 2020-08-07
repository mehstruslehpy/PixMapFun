#ifndef BSPTREE_H
#define BSPTREE_H

#include "assert.h"
#include "wireframe.h"
#include <stdio.h>
#define NDEBUG
struct _BSPNode 
//typedef struct _BSPNode;
{
	Polygon* poly;
	struct _BSPNode* pos;
	struct _BSPNode* neg;
};
typedef struct _BSPNode BSPNode;

BSPNode* build_bsp_tree_triangulated(Wireframe);
BSPNode* build_bsp_tree_untriangulated(Wireframe);
BSPNode* new_node(Polygon*);
void delete_bsp_tree(BSPNode*);
void add_node(BSPNode*,Polygon*);
double f(Polygon,Vector);
#endif

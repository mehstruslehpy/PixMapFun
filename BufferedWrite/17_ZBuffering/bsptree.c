#include "bsptree.h"
#define EPSILON DBL_MIN
BSPNode* new_node(Polygon* p)
{
	BSPNode* new_node = malloc(sizeof(BSPNode));
	new_node->poly=malloc(sizeof(Polygon));
	alloc_poly(new_node->poly,p->count);
	copy_poly(p,new_node->poly);
	new_node->pos=NULL;
	new_node->neg=NULL;
	return new_node;
}
BSPNode* build_bsp_tree_triangulated(Wireframe w)
{
	assert(w->poly!=NULL);
	BSPNode* root = NULL;
	if (w.poly[0].count>3)
	{
		//if the polygon is not a triangle split it into triangles
		Polygon* subpoly=triangulate_convex_poly(&(w.poly[0]));
		root=new_node(&(subpoly[0]));
		for (int j=1;j<w.poly[0].count-2;j++)
		{
			add_node(root,&(subpoly[j]));
			dealloc_poly(&(subpoly[j]));
		}
		free(subpoly);
	}
	else root = new_node(&(w.poly[0]));

	//set up the rest of the tree
	for (int i=1;i<w.count;i++)
	{
		//if the polygon is not a triangle split it into triangles
		if (w.poly[i].count>3)
		{
			Polygon* subpoly=triangulate_convex_poly(&(w.poly[i]));
			for (int j=0;j<w.poly[i].count-2;j++)
			{
				add_node(root,&(subpoly[j]));
				dealloc_poly(&(subpoly[j]));
			}
			free(subpoly);
		}
		else add_node(root,&(w.poly[i]));
	}
	return root;
}
BSPNode* build_bsp_tree_untriangulated(Wireframe w)
{
	assert(w->poly!=NULL);
	BSPNode* root = new_node(&(w.poly[0]));

	//set up the rest of the tree
	for (int i=1;i<w.count;i++)
	{
		add_node(root,&(w.poly[i]));
	}
	return root;
}
void delete_bsp_tree(BSPNode* root)
{
	if (root->neg!=NULL) delete_bsp_tree(root->neg);
	if (root->pos!=NULL) delete_bsp_tree(root->pos);
	dealloc_poly(root->poly);
	free(root->poly);
	free(root);
	root=NULL;
}
void add_node(BSPNode* n,Polygon* p)
{
	//assume we have enough points in our polygon
	assert(poly.count>=3);

	Vector a=p->pt[0];
	Vector b=p->pt[1];
	Vector c=p->pt[2];
	Pixel color_a=p->color[0];
	Pixel color_b=p->color[1];
	Pixel color_c=p->color[2];
	Polygon poly = n->poly[0];

	double fa = f(poly,a);
	if (abs(fa)<EPSILON) fa=0;
	double fb = f(poly,b);
	if (abs(fb)<EPSILON) fb=0;
	double fc = f(poly,c);
	if (abs(fc)<EPSILON) fc=0;

	if (fa<=0&&fb<=0&&fc<=0)
	{
		if (n->neg==NULL) n->neg=new_node(p);
		else add_node(n->neg,p);
	}
	else if (fa>=0&&fb>=0&&fc>=0)
	{
		if (n->pos==NULL) n->pos=new_node(p);
		else add_node(n->pos,p);
	}
	else
	{
		if (fa*fc>=0)
		{
			swap_double(&fb,&fc);
			swap_vector(&b,&c);
			swap_double(&fa,&fb);
			swap_vector(&a,&b);
		}
		else if (fb*fc>=0)
		{
			swap_double(&fa,&fc);
			swap_vector(&a,&c);
			swap_double(&fa,&fb);
			swap_vector(&a,&b);
		}
		Vector norm=cross_product(sub(poly.pt[0],poly.pt[1]),sub(poly.pt[0],poly.pt[2]));
		double D=dot_product(scalar_mul(-1,norm),poly.pt[0]);
		double tA = -1*(dot_product(norm,a)+D)/dot_product(norm,sub(c,a));
		Vector A = add(a,scalar_mul(tA,sub(c,a)));
		double tB = -1*(dot_product(norm,b)+D)/dot_product(norm,sub(c,b));
		Vector B = add(b,scalar_mul(tB,sub(c,b)));

		Polygon* T1= malloc(sizeof(Polygon));
		alloc_poly(T1,3);
		T1->pt[0]=a;
		T1->pt[1]=b;
		T1->pt[2]=A;
		T1->color[0]=color_a;
		T1->color[1]=color_b;
		T1->color[2]=interpolate_color(A,a,b,c,color_a,color_b,color_c);
		T1->count=3;

		Polygon* T2= malloc(sizeof(Polygon));
		alloc_poly(T2,3);
		T2->pt[0]=b;
		T2->pt[1]=B;
		T2->pt[2]=A;
		T2->color[0]=color_b;
		T2->color[1]=interpolate_color(B,a,b,c,color_a,color_b,color_c);
		T2->color[2]=interpolate_color(A,a,b,c,color_a,color_b,color_c);
		T2->count=3;

		Polygon* T3= malloc(sizeof(Polygon));
		alloc_poly(T3,3);
		T3->pt[0]=A;
		T3->pt[1]=B;
		T3->pt[2]=c;
		T3->color[0]=interpolate_color(A,a,b,c,color_a,color_b,color_c);
		T3->color[1]=interpolate_color(B,a,b,c,color_a,color_b,color_c);
		T3->color[2]=color_c;
		T3->count=3;

		if (fc>=0)
		{
			if (n->neg==NULL) n->neg=new_node(T1);
			else add_node(n->neg,T1);
			add_node(n->neg,T2);
			if (n->pos==NULL) n->pos=new_node(T3);
			else add_node(n->pos,T3);
		}
		else
		{
			if (n->pos==NULL) n->pos=new_node(T1);
			else add_node(n->pos,T1);
			add_node(n->pos,T2);
			if (n->neg==NULL) n->neg=new_node(T3);
			else add_node(n->neg,T3);

		}
		dealloc_poly(T1);
		free(T1);
		dealloc_poly(T2);
		free(T2);
		dealloc_poly(T3);
		free(T3);
	}
}

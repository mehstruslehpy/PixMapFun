#include "xform.h"

void perspective_project_polygon(Polygon* poly,Vector eye_pos,Vector gaze_dir,Vector view_up,ViewVolume vol,int nx,int ny)
{
	for (int i=0;i<poly->count;i++) 
		poly->pt[i]=perspective_project_vector(poly->pt[i],eye_pos,gaze_dir,view_up,vol,nx,ny);
}
void xform_polygon_window_to_view(Polygon* poly,Vector wmax,Vector wmin,Vector vmax,Vector vmin)
{
	for (int i=0;i<poly->count;i++) 
		poly->pt[i]=xform_window_to_view(poly->pt[i],wmax,wmin,vmax,vmin);
}
Triangle perspective_project_triangle(Triangle tri,Vector e,Vector g,Vector t,ViewVolume vol,int x_bound,int y_bound)
{
	return (Triangle){
		perspective_project_vector(tri.vert1,e,g,t,vol,x_bound,y_bound),
			perspective_project_vector(tri.vert2,e,g,t,vol,x_bound,y_bound),
			perspective_project_vector(tri.vert3,e,g,t,vol,x_bound,y_bound)
	};
}

Vector perspective_project_vector(Vector pt,Vector e,Vector g,Vector t,ViewVolume vol,int nx,int ny)
{
	//coord system of viewing plane
	Vector w = scalar_mul(-1,normalize(g));
	Vector u = normalize(cross_product(t,w));
	Vector v = cross_product(w,u);

	//create mv matrix
	//mv first shifts e to be the origin
	//then converts the coord to the uvw basis wrt the new origin
	double** mv1 = (double*[]){
		(double[]){u.x,u.y,u.z,0},
			(double[]){v.x,v.y,v.z,0},
			(double[]){w.x,w.y,w.z,0},
			(double[]){0,0,0,1}
	};
	double** mv2 = (double*[]){
		(double[]){1,0,0,-1*e.x},
			(double[]){0,1,0,-1*e.y},
			(double[]){0,0,1,-1*e.z},
			(double[]){0,0,0,1}
	};
	double** mv = (double*[]){
		(double[]){1,0,0,0},
			(double[]){0,1,0,0},
			(double[]){0,0,1,0},
			(double[]){0,0,0,1}
	};
	matrix_mult(mv,mv1,4,4,mv2,4,4);

	//create mo matrix
	//xforms pt in view vol to [-0.5,nx-0.5]x[-0.5,ny-0.5]x[-1,1]
	double** mo1 = (double*[]){
		(double[]){nx*0.5,0,0,0.5*(nx-1)},
			(double[]){0,0.5*ny,0,0.5*(ny-1)},
			(double[]){0,0,1,0},
			(double[]){0,0,0,1}
	};
	double** mo2 = (double*[]){
		(double[]){2/(vol.r-vol.l),0,0,0},
			(double[]){0,2/(vol.t-vol.b),0,0},
			(double[]){0,0,2/(vol.n-vol.f),0},
			(double[]){0,0,0,1}
	};
	double** mo3 = (double*[]){
		(double[]){1,0,0,-0.5*(vol.l+vol.r)},
			(double[]){0,1,0,-0.5*(vol.b+vol.t)},
			(double[]){0,0,1,-0.5*(vol.n+vol.f)},
			(double[]){0,0,0,1},
	};
	double** mo = (double*[]){
		(double[]){1,0,0,0},
			(double[]){0,1,0,0},
			(double[]){0,0,1,0},
			(double[]){0,0,0,1}
	};
	matrix_mult(mv1,mo1,4,4,mo2,4,4);
	matrix_mult(mo,mv1,4,4,mo3,4,4);

	//create mp matrix
	double** mp = (double*[]){
		(double[]){vol.n,0,0,0},
			(double[]){0,vol.n,0,0},
			(double[]){0,0,vol.n+vol.f,-vol.f*vol.n},
			(double[]){0,0,1,0}
	};

	//create m matrix
	double** m = (double*[]){
		(double[]){1,0,0,0},
			(double[]){0,1,0,0},
			(double[]){0,0,1,0},
			(double[]){0,0,0,1}
	};
	matrix_mult(mv1,mo,4,4,mp,4,4);
	matrix_mult(m,mv1,4,4,mv,4,4);

	//create column matrix from input vector
	double** a = (double*[]){
		(double[]){pt.x},
			(double[]){pt.y},
			(double[]){pt.z},
			(double[]){1}
	};

	//build output vector and return it
	double** p = (double*[]){
		(double[]){1},
			(double[]){1},
			(double[]){1},
			(double[]){1}
	};
	matrix_mult(p,m,4,4,a,4,1);

	//return (Vector){.x=p[0][0]/p[3][0],.y=p[1][0]/p[3][0],.z=p[2][0]/p[3][0],.z_buf=p[2][0]/p[3][0]};
	return (Vector){.x=p[0][0]/p[3][0],.y=p[1][0]/p[3][0],.z=p[2][0]/p[3][0]};
}

Triangle orth_project_triangle(Triangle tri,Vector e,Vector g,Vector t,ViewVolume vol,int x_bound,int y_bound)
{
	return (Triangle){
		orth_project_vector(tri.vert1,e,g,t,vol,x_bound,y_bound),
			orth_project_vector(tri.vert2,e,g,t,vol,x_bound,y_bound),
			orth_project_vector(tri.vert3,e,g,t,vol,x_bound,y_bound)
	};
}

Vector orth_project_vector(Vector pt,Vector e,Vector g,Vector t,ViewVolume vol,int nx,int ny)
{
	//this is for orthographic projection
	Vector w = scalar_mul(-1,normalize(g));
	Vector u = normalize(cross_product(t,w));
	Vector v = cross_product(w,u);

	//create mv matrix
	double** mv1 = (double*[]){
		(double[]){u.x,u.y,u.z,0},
			(double[]){v.x,v.y,v.z,0},
			(double[]){w.x,w.y,w.z,0},
			(double[]){0,0,0,1}
	};
	double** mv2 = (double*[]){
		(double[]){1,0,0,-1*e.x},
			(double[]){0,1,0,-1*e.y},
			(double[]){0,0,1,-1*e.z},
			(double[]){0,0,0,1}
	};
	double** mv = (double*[]){
		(double[]){1,0,0,0},
			(double[]){0,1,0,0},
			(double[]){0,0,1,0},
			(double[]){0,0,0,1}
	};
	matrix_mult(mv,mv1,4,4,mv2,4,4);

	//create mo matrix
	double** mo1 = (double*[]){
		(double[]){nx*0.5,0,0,0.5*(nx-1)},
			(double[]){0,0.5*ny,0,0.5*(ny-1)},
			(double[]){0,0,1,0},
			(double[]){0,0,0,1}
	};
	double** mo2 = (double*[]){
		(double[]){2/(vol.r-vol.l),0,0,0},
			(double[]){0,2/(vol.t-vol.b),0,0},
			(double[]){0,0,2/(vol.n-vol.f),0},
			(double[]){0,0,0,1}
	};
	double** mo3 = (double*[]){
		(double[]){1,0,0,-0.5*(vol.l+vol.r)},
			(double[]){0,1,0,-0.5*(vol.b+vol.t)},
			(double[]){0,0,1,-0.5*(vol.n+vol.f)},
			(double[]){0,0,0,1},
	};
	double** mo = (double*[]){
		(double[]){1,0,0,0},
			(double[]){0,1,0,0},
			(double[]){0,0,1,0},
			(double[]){0,0,0,1}
	};
	matrix_mult(mv1,mo1,4,4,mo2,4,4);
	matrix_mult(mo,mv1,4,4,mo3,4,4);

	//create m matrix
	double** m = (double*[]){
		(double[]){1,0,0,0},
			(double[]){0,1,0,0},
			(double[]){0,0,1,0},
			(double[]){0,0,0,1}
	};
	matrix_mult(m,mo,4,4,mv,4,4);

	//create column matrix from input vector
	double** in = (double*[]){
		(double[]){pt.x},
			(double[]){pt.y},
			(double[]){pt.z},
			(double[]){1}
	};

	//build output vector and return it
	double** out = (double*[]){
		(double[]){1},
			(double[]){1},
			(double[]){1},
			(double[]){1}
	};
	matrix_mult(out,m,4,4,in,4,1);
	return (Vector){out[0][0],out[1][0],out[2][0]};
}

void debug_print_view_volume1(char* msg,ViewVolume* vol)
{
	assert(msg!=NULL);
	//lrbtnf
	printf("%s [%f,%f]x[%f,%f]x[%f,%f]\n",msg,vol->l,vol->r,vol->t,vol->b,vol->n,vol->f);
}

void debug_print_view_volume2(char* msg,ViewVolume* vol)
{
	assert(msg!=NULL);
	//lrbtnf
	printf("%s {.l=%f,.r=%f,.t=%f,.b=%f,.n=%f,.f=%f}\n",msg,vol->l,vol->r,vol->t,vol->b,vol->n,vol->f);
}
double** alloc_window_to_view_matrix(Vector wmax,Vector wmin,Vector vmax,Vector vmin)
{
	assert(wmax.x-wmin.x!=0); //must provide valid bounds for the xform
	assert(wmax.y-wmin.y!=0);

	double* m1_row1=(double[]){1.0,0.0,vmin.x};
	double* m1_row2=(double[]){0.0,1.0,vmin.y};
	double* m1_row3=(double[]){0.0,0.0,1.0};
	double** m1=(double*[]){m1_row1,m1_row2,m1_row3};

	double* m2_row1=(double[]){(vmax.x-vmin.x)/(wmax.x-wmin.x),0.0,0.0};
	double* m2_row2=(double[]){0.0,(vmax.y-vmin.y)/(wmax.y-wmin.y),0.0};
	double* m2_row3=(double[]){0.0,0.0,1.0};
	double** m2=(double*[]){m2_row1,m2_row2,m2_row3};

	double* m3_row1=(double[]){1.0,0.0,-1.0*wmin.x};
	double* m3_row2=(double[]){0.0,1.0,-1.0*wmin.y};
	double* m3_row3=(double[]){0.0,0.0,1.0};
	double** m3 = (double*[]){m3_row1,m3_row2,m3_row3};

	double** temp=(double*[]){(double[3]){0},(double[3]){0},(double[3]){0}};

	matrix_mult(temp,m1,3,3,m2,3,3);
	matrix_mult(m2,temp,3,3,m3,3,3);

	//allocate space for a three by three matrix
	double** xform = (double**)malloc(sizeof(double*)*3);
	//allocate space for three columns in each row
	xform[0] = (double*)malloc(sizeof(double)*3);
	xform[1] = (double*)malloc(sizeof(double)*3);
	xform[2] = (double*)malloc(sizeof(double)*3);

	//copy the transform into the allocated memory
	matrix_copy(xform,3,3,m2,3,3);

	return xform;
}
void dealloc_window_to_view_matrix(double** xform)
{
	free(xform[0]);
	free(xform[1]);
	free(xform[2]);
	free(xform);
}
Vector fast_window_view_xform(double** xform,Vector pt)
{
	double** in = (double*[]){(double[]){pt.x},(double[]){pt.y},(double[]){1.0}};
	double** out=(double*[]){(double[]){0},(double[]){0},(double[]){0}};
	matrix_mult(out,xform,3,3,in,3,1);
	return (Vector){.x=out[0][0],.y=out[1][0],.z=out[2][0]};
}
Vector xform_window_to_view(Vector pt,Vector wmax,Vector wmin,Vector vmax,Vector vmin)
{
	if (wmax.x-wmin.x==0) return (Vector){.x=0,.y=0,.z=0}; //throw div by zero out of view
	if (wmax.y-wmin.y==0) return (Vector){.x=0,.y=0,.z=0}; //throw div by zero out of view

	double** out=(double*[]){(double[]){0},(double[]){0},(double[]){0}};
	double* in_row1 = (double[]){pt.x};
	double* in_row2 = (double[]){pt.y};
	double* in_row3 = (double[]){1.0};
	double** in = (double*[]){in_row1,in_row2,in_row3};
	/*
	   double* m1_row1=(double[]){1.0,0.0,vmin.x};
	   double* m1_row2=(double[]){0.0,1.0,vmin.y};
	   double* m1_row3=(double[]){0.0,0.0,1.0};
	   double** m1=(double*[]){m1_row1,m1_row2,m1_row3};

	   double* m2_row1=(double[]){(vmax.x-vmin.x)/(wmax.x-wmin.x),0.0,0.0};
	   double* m2_row2=(double[]){0.0,(vmax.y-vmin.y)/(wmax.y-wmin.y),0.0};
	   double* m2_row3=(double[]){0.0,0.0,1.0};
	   double** m2=(double*[]){m2_row1,m2_row2,m2_row3};

	   double* m3_row1=(double[]){1.0,0.0,-1.0*wmin.x};
	   double* m3_row2=(double[]){0.0,1.0,-1.0*wmin.y};
	   double* m3_row3=(double[]){0.0,0.0,1.0};
	   double** m3 = (double*[]){m3_row1,m3_row2,m3_row3};

	   double** temp=(double*[]){(double[3]){0},(double[3]){0},(double[3]){0}};

	   matrix_mult(temp,m1,3,3,m2,3,3);
	   matrix_mult(m2,temp,3,3,m3,3,3);
	   matrix_mult(out,m2,3,3,in,3,1);
	   */
	double** xform=alloc_window_to_view_matrix(wmax,wmin,vmax,vmin);
	matrix_mult(out,xform,3,3,in,3,1);
	dealloc_window_to_view_matrix(xform);

	return (Vector){.x=out[0][0],.y=out[1][0],.z=out[2][0]};
}

double scale_to_interval(double wc,double wmin,double wmax ,double vmin,double vmax)
{ 
	//if (wmax-wmin==0) return vmax; //throw div by zero out of view
	if (wmax-wmin==0) return 0; //throw div by zero out of view
	else return (vmax-vmin)*(wc-wmin)/(wmax-wmin)+vmin; //else scale to bounds
}

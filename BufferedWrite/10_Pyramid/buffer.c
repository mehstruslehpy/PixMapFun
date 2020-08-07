#include "buffer.h"

const int MAX_COLOR_VAL=255;
const int PIXELS_PER_LINE=2;
const int X_BOUND=2048;
const int Y_BOUND=2048;
const int Z_BOUND=2048;

Triangle project_triangle(Triangle tri,Tuple e,Tuple g,Tuple t,ViewVolume vol,int x_bound,int y_bound)
{
	return (Triangle){
		project_tuple(tri.vert1,e,g,t,vol,x_bound,y_bound),
			project_tuple(tri.vert2,e,g,t,vol,x_bound,y_bound),
			project_tuple(tri.vert3,e,g,t,vol,x_bound,y_bound)
	};
}

Tuple cross_product(Tuple a,Tuple b)
{
	return (Tuple){
		a.y*b.z-a.z-b.y,
			-1.0*(a.x*b.z-a.z*b.x),
			a.x*b.y-a.y*b.x
	};
}
double dot_product(Tuple a,Tuple b)
{
	return a.x*b.x+a.y+b.y+a.z*b.z;
}
Tuple normalize(Tuple pt)
{
	assert(dot_product(pt,pt)!=0);
	double scale = sqrt(dot_product(pt,pt));
	return (Tuple){pt.x/scale,pt.y/scale,pt.z/scale};
}
Tuple scalar_mul(double a,Tuple pt)
{
	return (Tuple){a*pt.x,a*pt.y,a*pt.z};
}
Tuple project_tuple(Tuple pt,Tuple e,Tuple g,Tuple t,ViewVolume vol,int nx,int ny)
{
	ViewVolume debug_vol = (ViewVolume){-0.5,nx-0.5,-0.5,ny-0.5,-1,-1};
	debug_print_view_volume("DEBUG: projecting from volume",&vol);
	debug_print_view_volume("DEBUG: projecting from volume",&debug_vol);
	//this is for orthographic projection
	Tuple w = scalar_mul(-1,normalize(g));
	Tuple u = normalize(cross_product(t,w));
	Tuple v = cross_product(w,u);

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
	
	//create column matrix from input tuple
	double** in = (double*[]){
		(double[]){pt.x},
		(double[]){pt.y},
		(double[]){pt.z},
		(double[]){1}
	};

	//build output tuple and return it
	double** out = (double*[]){
		(double[]){1},
		(double[]){1},
		(double[]){1},
		(double[]){1}
	};
	matrix_mult(out,m,4,4,in,4,1);
	return (Tuple){out[0][0],out[1][0],out[2][0]};
}

Pixel** alloc_framebuf(int x_bound,int y_bound)
{
	Pixel** frame_buffer;
	frame_buffer = (Pixel**)malloc(x_bound*sizeof(Pixel*));
	for(int i=0; i<x_bound; i++)
		frame_buffer[i] = (Pixel*)malloc(y_bound*sizeof(Pixel));
	return frame_buffer;
}

void dealloc_framebuf(Pixel** frame_buffer,int x_bound)
{ 
	assert(frame_buffer!=NULL);

	for (int i=0; i<x_bound; i++)
	{
		free(frame_buffer[i]);
	}
	free(frame_buffer); 
}

void process_fbuf(Pixel(*fn)(double,double,double*,int),double* params,int pcnt,Pixel** frame_buffer,int x_bound,int y_bound)
{
	assert(frame_buffer!=NULL);
	assert(fn!=NULL);

	for (int i=0;i<x_bound;i++)
	{
		for (int j=0;j<y_bound;j++)
		{
			plot_fbuf(i,j,fn(i,j,params,pcnt),frame_buffer,x_bound,y_bound);
		}
	}
}

void debug_print_view_volume(char* msg,ViewVolume* vol)
{
	assert(msg!=NULL);
	//lrbtnf
	printf("%s [%f,%f]x[%f,%f]x[%f,%f]\n",msg,vol->l,vol->r,vol->t,vol->b,vol->n,vol->f);
}
void debug_print_tuple(char* msg, Tuple* tp) 
{
	assert(msg!=NULL);

	printf("%s {.x=%f, .y=%f, .z=%f}\n",msg,tp->x,tp->y,tp->z);
}

void fill_fbuf_color(Pixel color,Pixel** frame_buffer,int x_bound,int y_bound)
{
	assert(frame_buffer!=NULL);

	for (int i=0;i<x_bound;i++)
		for (int j=0;j<y_bound;j++)
			plot_fbuf(i,j,color,frame_buffer,x_bound,y_bound);
}

void copy_into_view(Pixel** frame_buffer,Tuple view_max,Tuple view_min,Pixel** window_model,Tuple window_max,Tuple window_min)
{
	for (int i=window_min.x;i<window_max.x;i++)
	{
		for (int j=window_min.y;j<window_max.y;j++)
		{
			//printf("DEBUG:i=%d,j=%d\n",i,j);
			Tuple oldcoord=(Tuple){.x=i,.y=j};
			Pixel color=read_fbuf(i,j,window_model,window_max.x,window_max.y);
			Tuple newcoord=xform_window_to_view(oldcoord,window_max,window_min,view_max,view_min);
			plot_fbuf(newcoord.x,newcoord.y,color,frame_buffer,view_max.x,view_max.y);
		}
	}
}
void fast_copy_into_view(double** xform,Pixel** frame_buffer,Tuple view_max,Tuple view_min,Pixel** window_model,Tuple window_max,Tuple window_min)
{
	for (int i=window_min.x;i<window_max.x;i++)
	{
		for (int j=window_min.y;j<window_max.y;j++)
		{
			//printf("DEBUG:i=%d,j=%d\n",i,j);
			Tuple oldcoord=(Tuple){.x=i,.y=j};
			Pixel color=read_fbuf(i,j,window_model,window_max.x,window_max.y);
			Tuple newcoord=fast_window_view_xform(xform,oldcoord);
			plot_fbuf(newcoord.x,newcoord.y,color,frame_buffer,view_max.x,view_max.y);
		}
	}
}
double** alloc_window_to_view_matrix(Tuple wmax,Tuple wmin,Tuple vmax,Tuple vmin)
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
Tuple fast_window_view_xform(double** xform,Tuple pt)
{
	double** in = (double*[]){(double[]){pt.x},(double[]){pt.y},(double[]){1.0}};
	double** out=(double*[]){(double[]){0},(double[]){0},(double[]){0}};
	matrix_mult(out,xform,3,3,in,3,1);
	return (Tuple){.x=out[0][0],.y=out[1][0],.z=out[2][0]};
}
Tuple xform_window_to_view(Tuple pt,Tuple wmax,Tuple wmin,Tuple vmax,Tuple vmin)
{
	if (wmax.x-wmin.x==0) return (Tuple){.x=0,.y=0,.z=0}; //throw div by zero out of view
	if (wmax.y-wmin.y==0) return (Tuple){.x=0,.y=0,.z=0}; //throw div by zero out of view

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

	return (Tuple){.x=out[0][0],.y=out[1][0],.z=out[2][0]};
}

double scale_to_interval(double wc,double wmin,double wmax ,double vmin,double vmax)
{ 
	//if (wmax-wmin==0) return vmax; //throw div by zero out of view
	if (wmax-wmin==0) return 0; //throw div by zero out of view
	else return (vmax-vmin)*(wc-wmin)/(wmax-wmin)+vmin; //else scale to bounds
}

void writeimageheader(FILE* out,int x_bound,int y_bound)
{
	assert(out!=NULL);

	fprintf(out,"P3\n");
	fprintf(out,"%d %d\n",x_bound,y_bound);
	fprintf(out,"%d\n",MAX_COLOR_VAL);
}

void plot_fbuf(int i,int j,Pixel color,Pixel** frame_buffer,int x_bound,int y_bound)
{
	assert(frame_buffer!=NULL);

	//reject points not within the frame_buffer for writing
	if(i<0||j<0) return;
	if(i>=x_bound||j>=y_bound) return;

	*(*(frame_buffer+i)+j)=color;
}
Pixel read_fbuf(int i,int j,Pixel** frame_buffer,int x_bound,int y_bound)
{
	assert(frame_buffer!=NULL);
	assert(i>=0&&i<x_bound);
	assert(i>=0&&j<y_bound);

	return *(*(frame_buffer+i)+j);
}
void plot_disk(FILE* out,int x,int y,Pixel** frame_buffer,int x_bound,int y_bound)
{
	assert(frame_buffer!=NULL);
	assert(out!=NULL);
	assert(x<x_bound);
	assert(y<y_bound);

	Pixel cur = read_fbuf(x,y_bound-y,frame_buffer,x_bound,y_bound);
	fprintf(out,"%d %d %d ",cur.red,cur.grn,cur.blu);
}

void write_fbuf_to_disk(char* filename,Pixel** frame_buffer,int x_bound,int y_bound)
{
	assert(filename!=NULL);
	assert(frame_buffer!=NULL);

	FILE *out = fopen(filename,"w");
	writeimageheader(out,x_bound,y_bound);
	int pixels = 0;
	for (int i=0;i<x_bound;i++) //rows
	{
		for (int j=0;j<y_bound;j++) //columns
		{
			plot_disk(out,i,j,frame_buffer,x_bound,y_bound);
			//we are using the constant rather than passing pixels per line
			//as a param to keep our output files consistent
			//this is not strictly necessary
			if (pixels==PIXELS_PER_LINE)
			{
				fprintf(out,"\n");
				pixels=0;
			}
			else pixels++;
		}
	}
	fclose(out);
}

void generate_pt_arr_to_fbuf(Tuple* pt_buf,Tuple pt0,Tuple(*fn)(Tuple,double*,int),double* params,int pcnt,int buf_len,int x_bound,int y_bound)
{
	assert(pt_buf!=NULL);
	assert(fn!=NULL);
	assert(buf_len>1);

	pt_buf[0] = pt0;
	double xmax = pt_buf[0].x;
	double xmin = pt_buf[0].x;
	double ymax = pt_buf[0].y;
	double ymin = pt_buf[0].y;
	for (int i=1;i<buf_len;i++) 
	{
		pt_buf[i]=fn(pt_buf[i-1],params,pcnt);
		//track mins and maxs
		if (xmax<pt_buf[i].x) xmax=pt_buf[i].x;
		if (xmin>pt_buf[i].x) xmin=pt_buf[i].x;
		if (ymax<pt_buf[i].y) ymax=pt_buf[i].y;
		if (ymin>pt_buf[i].y) ymin=pt_buf[i].y;
	}

	//scale point into frame buffer range
	for (int i=0;i<buf_len;i++) 
	{
		pt_buf[i].x=scale_to_interval(pt_buf[i].x,xmin,xmax,0,x_bound);
		pt_buf[i].y=scale_to_interval(pt_buf[i].y,ymin,ymax,0,y_bound);
	}
}

void write_pt_arr_to_fbuf(Tuple* pt_buf,int buf_len,Pixel** frame_buffer,int x_bound,int y_bound)
{
	assert(pt_buf!=NULL);
	assert(buf_len>0);
	assert(frame_buffer!=NULL);

	for (int i = 0;i<buf_len;i++)
	{
		//skip values out of range, we can also choose to fail more harshly if needed.
		if (pt_buf[i].x>=x_bound||pt_buf[i].x<0) ;
		else if (pt_buf[i].y>=y_bound||pt_buf[i].y<0) ;
		//write values in range
		else 
		{
			plot_fbuf(pt_buf[i].x,pt_buf[i].y,(Pixel){.red=0,.grn=0,.blu=0},frame_buffer,x_bound,y_bound);
		}
	}
}
void draw_triangle(Pixel color,Triangle tri,Pixel** frame_buffer, int x_bound ,int y_bound)
{
	double* arr=(double[]){tri.vert1.x,tri.vert1.y,
		tri.vert2.x,tri.vert2.y,
		tri.vert3.x,tri.vert3.y,
		tri.vert1.x,tri.vert1.y};
	bresenham_lines_array(color,arr,8,frame_buffer,x_bound,y_bound);
}
void draw_segment(Pixel color,Tuple pt1,Tuple pt2,Pixel** frame_buffer, int x_bound ,int y_bound)
{
	double* arr=(double[]){pt1.x,pt1.y,pt2.x,pt2.y};
	bresenham_lines_array(color,arr,4,frame_buffer,x_bound,y_bound);
}
void bresenham_lines_array(Pixel color,double* pts,int count,Pixel** frame_buffer, int x_bound ,int y_bound)
{
	assert(pts!=NULL);
	assert(count>=4); //at least two points
	assert(frame_buffer!=NULL);

	for (int i=0;i<count-2;i+=2) 
	{
		bresenham_plotline(color,pts[i],pts[i+1],pts[i+2],pts[i+3],frame_buffer,x_bound,y_bound);
	}
}
void bresenham_plotline(Pixel color,double x_0,double y_0,double x_1,double y_1,Pixel** frame_buffer,int x_bound,int y_bound)
{
	assert(frame_buffer!=NULL);

	//need integer coordinates
	int x0 = x_0;
	int y0 = y_0;
	int x1 = x_1;
	int y1 = y_1;

	//this is essentially the bresenham algo from rosetta code for the
	//c programming language
	int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
	int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
	int err = (dx>dy ? dx : -dy)/2, e2;
	for(;;)
	{
		plot_fbuf(x0,y0,color,frame_buffer,x_bound,y_bound);
		if (x0==x1 && y0==y1) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
}

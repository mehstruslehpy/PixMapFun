#include "buffer.h"

const int MAX_COLOR_VAL=255;
const int PIXELS_PER_LINE=2;
//TODO: Fix bug where asymmetric bounds write to disk wrong
const int X_BOUND=2048;
const int Y_BOUND=2048;

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

void debug_print_tuple(char* msg, Tuple* tp) 
{
	assert(msg!=NULL);

	printf("%s {.x=%f, .y=%f}\n",msg,tp->x,tp->y);
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
Tuple xform_window_to_view(Tuple pt,Tuple wmax,Tuple wmin,Tuple vmax,Tuple vmin)
{
	double vx=scale_to_interval(pt.x,wmin.x,wmax.x,vmin.x,vmax.x);
	double vy=scale_to_interval(pt.y,wmin.y,wmax.y,vmin.y,vmax.y);
	Tuple out=(Tuple){.x=vx,.y=vy};
	debug_print_tuple("DEBUGDEBUG:",&out);
	return out;
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

	Pixel cur = read_fbuf(x,y,frame_buffer,x_bound,y_bound);
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

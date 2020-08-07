#include "buffer.h"

const int MAX_COLOR_VAL=255;
const int PIXELS_PER_LINE=2;
const int X_BOUND=2048;
const int Y_BOUND=2048;
const int Z_BOUND=2048;

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
	frame_buffer=NULL;
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

void fill_fbuf_color(Pixel color,Pixel** frame_buffer,int x_bound,int y_bound)
{
	assert(frame_buffer!=NULL);

	for (int i=0;i<x_bound;i++)
		for (int j=0;j<y_bound;j++)
			plot_fbuf(i,j,color,frame_buffer,x_bound,y_bound);
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

void generate_pt_arr_to_fbuf(Vector* pt_buf,Vector pt0,Vector(*fn)(Vector,double*,int),double* params,int pcnt,int buf_len,int x_bound,int y_bound)
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
		//pt_buf[i].x=(x_bound-0)*(pt_buf[i].x-xmin)/(xmax-xmin)+0;
		pt_buf[i].y=scale_to_interval(pt_buf[i].y,ymin,ymax,0,y_bound);
		//pt_buf[i].y=(y_bound-0)*(pt_buf[i].y-ymin)/(ymax-ymin)+0;
	}
}

void write_pt_arr_to_fbuf(Vector* pt_buf,int buf_len,Pixel** frame_buffer,int x_bound,int y_bound)
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


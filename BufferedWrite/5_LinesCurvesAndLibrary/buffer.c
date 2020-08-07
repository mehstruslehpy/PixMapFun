#include "buffer.h"

const int MAX_COLOR_VAL=255;
const int PIXELS_PER_LINE=2;
const int X_BOUND=1024;
const int Y_BOUND=1024;

void process_fbuf(Pixel frame_buffer[X_BOUND][Y_BOUND],Pixel(*fn)(double,double,double*,int),double* params,int pcnt,int x_bound,int y_bound)
{
	for (int i=0;i<x_bound;i++)
		for (int j=0;j<y_bound;j++)
		{
			frame_buffer[i][j] = fn(i,j,params,pcnt);
			printf ("DEBUG: x=%d,y=%d,red=%d,grn=%d,blu=%d\n",i,j,frame_buffer[i][j].red,frame_buffer[i][j].grn,frame_buffer[i][j].blu);
		}
}

void debug_print_tuple(char* msg, Tuple* tp) {printf("%s {.x=%f, .y=%f}\n",msg,tp->x,tp->y);}

void fill_fbuf_color(Pixel frame_buffer[X_BOUND][Y_BOUND],int x_bound,int y_bound,Pixel color)
{
	for (int i=0;i<x_bound;i++)
		for (int j=0;j<y_bound;j++)
			frame_buffer[i][j] = color;
}

double scale_to_interval(double x,double min,double max ,double a,double b)
{ 
	if (max-min==0) return max; //throw div by zero out of view
	else return (b-a)*(x-min)/(max-min)+a; //else scale to bounds
}

void writeimageheader(FILE* out)
{
	fprintf(out,"P3\n");
	fprintf(out,"%d %d\n",X_BOUND,Y_BOUND);
	fprintf(out,"%d\n",MAX_COLOR_VAL);
}

void plot_fbuf(int i,int j,Pixel color,Pixel frame_buffer[X_BOUND][Y_BOUND])
{
	frame_buffer[i][j]=color;
}
void plot_disk(FILE* out,int x,int y,Pixel frame_buffer[X_BOUND][Y_BOUND])
{
	Pixel cur = frame_buffer[x][y];
	fprintf(out,"%d %d %d ",cur.red,cur.grn,cur.blu);
}

void write_fbuf_to_disk(char* filename,Pixel frame_buffer[X_BOUND][Y_BOUND],int x_bound,int y_bound)
{
	FILE *out = fopen(filename,"w");

	writeimageheader(out);
	int pixels = 0;
	for (int i=0;i<x_bound;i++) //rows
	{
		for (int j=0;j<y_bound;j++) //columns
		{
			plot_disk(out,i,j,frame_buffer);
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

void write_pt_arr_to_fbuf(Pixel frame_buffer[X_BOUND][Y_BOUND],Tuple* pt_buf,int buf_len,int x_bound,int y_bound)
{
	for (int i = 0;i<buf_len;i++)
	{
		//skip values out of range, can also choose to faile more harshly if needed
		if (pt_buf[i].x>=x_bound||pt_buf[i].x<0) ;
		else if (pt_buf[i].y>=y_bound||pt_buf[i].y<0) ;
		//write values in range
		else 
		{
			frame_buffer[(int) pt_buf[i].x][(int) pt_buf[i].y]=
				(Pixel){.red=0,.grn=0,.blu=0};
			//(Pixel){.red=rand()%256,.grn=rand()%256,.blu=rand()%256};
		}
	}
}
void bresenham_lines_array(Pixel frame_buffer[X_BOUND][Y_BOUND],Pixel color,double* pts,int count)
{
	for (int i=0;i<count-2;i+=2) 
	{
		printf("DEBUG: plotting %f %f to %f %f\n",pts[i],pts[i+1],pts[i+2],pts[i+3]);
		bresenham_plotline(frame_buffer,color,pts[i],pts[i+1],pts[i+2],pts[i+3]);
	}
}
void bresenham_plotline(Pixel frame_buffer[X_BOUND][Y_BOUND],Pixel color,double x_0,double y_0,double x_1,double y_1)
{
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
	for(;;){
		//setPixel(x0,y0);
		//frame_buffer[x0][y0]=color;
		plot_fbuf(x0,y0,color,frame_buffer);
		if (x0==x1 && y0==y1) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
}

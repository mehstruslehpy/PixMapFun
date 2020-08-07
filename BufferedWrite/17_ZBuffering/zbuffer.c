#include "zbuffer.h"

double** alloc_zbuf(int x_bound,int y_bound)
{
	double** zbuffer;
	zbuffer = (double**)malloc(x_bound*sizeof(double*));
	for(int i=0; i<x_bound; i++)
		zbuffer[i] = (double*)malloc(y_bound*sizeof(double));
	return zbuffer;
}
void dealloc_zbuf(double** zbuffer,int x_bound)
{ 
	assert(zbuffer!=NULL);

	for (int i=0; i<x_bound; i++)
	{
		free(zbuffer[i]);
	}
	free(zbuffer); 
	zbuffer=NULL;
}

void init_zbuf(double** zbuffer,int x_bound,int y_bound)
{
	assert(zbuffer!=NULL);
	fill_zbuf(INFINITY,zbuffer,x_bound,y_bound);
}
void fill_zbuf(double value,double** zbuffer,int x_bound,int y_bound)
{
	assert(zbuffer!=NULL);

	for (int i=0;i<x_bound;i++)
		for (int j=0;j<y_bound;j++)
			zbuffer[i][j]=value;
}

void plot_zbuf_fbuf(int i,int j,double z,Pixel color,Pixel** frame_buffer,double** zbuffer,int x_bound,int y_bound)
{
	assert(frame_buffer!=NULL);

	//reject points not within the frame_buffer for writing
	if(i<0||j<0) return;
	if(i>=x_bound||j>=y_bound) return;
	if (z<zbuffer[i][j])
	{
		zbuffer[i][j]=z;
		frame_buffer[i][j]=color;
	}
}

void plot_zbuf_disk(FILE* out,int x,int y,double** zbuffer,int x_bound,int y_bound)
{
	assert(frame_buffer!=NULL);
	assert(out!=NULL);
	assert(x<x_bound);
	assert(y<y_bound);
	//ideally we should scale this value
	double cur = zbuffer[x][y];
	if (!isfinite(cur)) cur=0;
	//this will obviously truncate pretty weirdly
	fprintf(out,"%d %d %d ",cur,cur,cur);
}

void write_zbuf_to_disk(char* filename,double** zbuffer,int x_bound,int y_bound)
{
	assert(filename!=NULL);
	assert(zbuffer!=NULL);

	FILE *out = fopen(filename,"w");
	writeimageheader(out,x_bound,y_bound);
	int pixels = 0;
	for (int i=0;i<x_bound;i++) //rows
	{
		for (int j=0;j<y_bound;j++) //columns
		{
			plot_zbuf_disk(out,i,j,zbuffer,x_bound,y_bound);
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

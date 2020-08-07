#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#define FILE_NAME_BUF 12+13
#define POINT_BUF_LEN 9999
#define MAX_COLOR_VAL 255
#define PIXELS_PER_LINE 2
#define X_BOUND 1024
#define Y_BOUND 1024
//usage: ./main outfile t-value func


typedef struct Pixel
{
	uint8_t red;
	uint8_t grn;
	uint8_t blu;
} Pixel;

//could also store color
typedef struct Tuple
{
	double x;
	double y;
	double z;
} Tuple;

//apparently this is called a pickover attractor or something
//from: https://nathanselikoff.com/training/tutorial-strange-attractors-in-c-and-opengl

//lorenz attractor equations:
//dx/dt=s(y-x)=F1(t,x,y,z)
double F1(double t,double x,double y,double z,double s) {return s*(y-x);}
//dy/dt=x(p-z)-y=F2(t,x,y,z)
double F2(double t,double x,double y,double z,double p) {return x*(p-z)-y;}
//dz/dt=xy-Bz=F3(t,x,y,z)
double F3(double t,double x,double y, double z,double b) {return x*y-b*z;}


Tuple rungekutta4(Tuple pt,double* params,int pcnt)
{
	//TODO: need to handle the case where count of params
	// does not match params passed
	double h = params[0]; //step size for rk4
	double s = params[1];
	double p = params[2];
	double b = params[3];

	double xn = pt.x;
	double yn = pt.y;
	double zn = pt.z;
	static double tn = 0;

	//runge kutta (rk4) equations:
	double m1 = F1(tn,xn,yn,zn,s); //m goes with x
	double k1 = F2(tn,xn,yn,zn,p); //k goes with y
	double u1 = F3(tn,xn,yn,zn,b); //u goes with z

	double m2 = F1(tn+(1.0/2.0)*h,xn+(1.0/2.0)*h*m1,yn+(1.0/2.0)*h*k1,zn+(1.0/2.0)*h*u1,s);
	double k2 = F2(tn+(1.0/2.0)*h,xn+(1.0/2.0)*h*m1,yn+(1.0/2.0)*h*k1,zn+(1.0/2.0)*h*u1,p);
	double u2 = F3(tn+(1.0/2.0)*h,xn+(1.0/2.0)*h*m1,yn+(1.0/2.0)*h*k1,zn+(1.0/2.0)*h*u1,b);

	double m3 = F1(tn+(1.0/2.0)*h,xn+(1.0/2.0)*h*m2,yn+(1.0/2.0)*h*k2,zn+(1.0/2.0)*h*u2,s);
	double k3 = F2(tn+(1.0/2.0)*h,xn+(1.0/2.0)*h*m2,yn+(1.0/2.0)*h*k2,zn+(1.0/2.0)*h*u2,p);
	double u3 = F3(tn+(1.0/2.0)*h,xn+(1.0/2.0)*h*m2,yn+(1.0/2.0)*h*k2,zn+(1.0/2.0)*h*u2,b);

	double m4 = F1(tn+h,xn+h*m3,yn+h*k3,zn+h*u3,s);
	double k4 = F2(tn+h,xn+h*m3,yn+h*k3,zn+h*u3,p);
	double u4 = F3(tn+h,xn+h*m3,yn+h*k3,zn+h*u3,b);

	double xnext = xn+(1.0/6.0)*h*(m1+2.0*m2+2.0*m3+m4);
	double ynext = yn+(1.0/6.0)*h*(k1+2.0*k2+2.0*k3+k4);
	double znext = zn+(1.0/6.0)*h*(u1+2.0*u2+2.0*u3+u4);

	tn++;
	return (Tuple){.x=xnext,.y=ynext,.z=znext};
}

//print info about tuple to std out
void debug_print_tuple(Tuple* tp, char* msg);
//scale number in interval [min,max] to [a,b]
double scale_to_interval(double,double,double,double,double);
//fill frame buffer with color
void fill_fbuf_color(Pixel[X_BOUND][Y_BOUND],int);
//build an array of pts (x,y) using a recurrence relation
//this requires you to pass (x0,y0) and f s.t. (x_n,y_n)=f(x_(n-1),y_(n-1))
void generate_pt_arr_to_fbuf(Tuple*,Tuple,Tuple(*fn)(Tuple,double*,int),double*,int,int,int,int);
//write an array of points to the frame buffer
void write_pt_arr_to_fbuf(Pixel[X_BOUND][Y_BOUND],Tuple*,int,int,int);
//write the frame buffer to disk
void write_fbuf_to_disk(Pixel[X_BOUND][Y_BOUND],int,int);
//write image header to disk
void writeimageheader(FILE*);
//write pixel to disk
void writepixel(FILE*,int,int,Pixel[X_BOUND][Y_BOUND]);

//usage ./main x0 y0 z0 h s p b
int main(int argc, char** argv)
{
	if (argc!=8) 
	{
		printf("DEBUG:argc/argv\n");
		return EXIT_FAILURE;
	}

	printf("DEBUG: Program start.\n");
	Pixel frame_buffer[X_BOUND][Y_BOUND];
	//Tuple pt_buf[POINT_BUF_LEN];
	Tuple* pt_buf = malloc(sizeof(Tuple)*POINT_BUF_LEN);
	Tuple pt0=(Tuple){.x=atof(argv[1]),.y=atof(argv[2]),.z=atof(argv[3])};

	//STRANGE ATTRACTOR
	printf("DEBUG: White out frame buffer.\n");
	fill_fbuf_color(frame_buffer,255);
	double params1[4] = { atof(argv[4]), atof(argv[5]), atof(argv[6]), atof(argv[7])};
	printf("DEBUG: Build pt array.\n");
	generate_pt_arr_to_fbuf(pt_buf,pt0,rungekutta4,params1,sizeof(params1),POINT_BUF_LEN,X_BOUND,Y_BOUND);
	printf("DEBUG: Write pt array to frame buffer.\n");
	write_pt_arr_to_fbuf(frame_buffer,pt_buf,POINT_BUF_LEN,X_BOUND,Y_BOUND);
	printf("DEBUG: Write frame buffer to disk.\n");
	write_fbuf_to_disk(frame_buffer,X_BOUND,Y_BOUND);

	free(pt_buf);
	return EXIT_SUCCESS;

}
void debug_print_tuple(Tuple* tp, char* msg) {printf("%s {.x=%f, .y=%f, .z=%f}\n",msg,tp->x,tp->y,tp->z);}
void fill_fbuf_color(Pixel frame_buffer[X_BOUND][Y_BOUND],int color)
{
	for (int i=0;i<X_BOUND;i++)
		for (int j=0;j<Y_BOUND;j++)
			frame_buffer[i][j] = (Pixel) {.red=color,.grn=color,.blu=color};
}
void generate_pt_arr_to_fbuf(Tuple* pt_buf,Tuple pt0,Tuple(*fn)(Tuple,double*,int),double* params,int pcnt,int buf_len,int x_bound,int y_bound)
{
	pt_buf[0] = pt0;
	double xmax = pt_buf[0].x;
	double xmin = pt_buf[0].x;
	double ymax = pt_buf[0].y;
	double ymin = pt_buf[0].y;
	for (int i=1;i<POINT_BUF_LEN;i++) 
	{
		pt_buf[i]=fn(pt_buf[i-1],params,pcnt);
		//track mins and maxs
		if (xmax<pt_buf[i].x) xmax=pt_buf[i].x;
		if (xmin>pt_buf[i].x) xmin=pt_buf[i].x;
		if (ymax<pt_buf[i].y) ymax=pt_buf[i].y;
		if (ymin>pt_buf[i].y) ymin=pt_buf[i].y;
	}

	//scale point into frame buffer range
	for (int i=0;i<POINT_BUF_LEN;i++) 
	{
		pt_buf[i].x=scale_to_interval(pt_buf[i].x,xmin,xmax,0,x_bound);
		pt_buf[i].y=scale_to_interval(pt_buf[i].y,ymin,ymax,0,y_bound);
	}
}
//scale points in interval [min,max] to [a,b]
double scale_to_interval(double x,double min,double max ,double a,double b)
{ 
	if (max-min==0) return max; //throw div by zero out of view
	else return (b-a)*(x-min)/(max-min)+a; //else scale to bounds
}

void write_pt_arr_to_fbuf(Pixel frame_buffer[X_BOUND][Y_BOUND],Tuple* pt_buf,int buf_len,int x_bound,int y_bound)
{
	for (int i = 0;i<buf_len;i++)
	{
		//skip values out of range, can also choose to faile more harshly if needed
		int xplot = pt_buf[i].x;
		int yplot = pt_buf[i].y;
		if (xplot>=x_bound||xplot<0) printf("Plot out of bounds.\n");
		else if (yplot>=y_bound||yplot<0) printf("Plot out of bounds.\n");
		//write values in range
		else 
		{
			frame_buffer[xplot][yplot]=(Pixel){.red=0,.grn=0,.blu=0};
			//(Pixel){.red=rand()%256,.grn=rand()%256,.blu=rand()%256};
		}
	}
}
void writeimageheader(FILE* out)
{
	fprintf(out,"P3\n");
	fprintf(out,"%d %d\n",X_BOUND,Y_BOUND);
	fprintf(out,"%d\n",MAX_COLOR_VAL);
}
void writepixel(FILE* out,int x,int y,Pixel frame_buffer[X_BOUND][Y_BOUND])
{
	Pixel cur = frame_buffer[x][y];
	fprintf(out,"%d %d %d ",cur.red,cur.grn,cur.blu);
}
void write_fbuf_to_disk(Pixel frame_buffer[X_BOUND][Y_BOUND],int x_bound,int y_bound)
{
	static int frame = 1;
	char framename[FILE_NAME_BUF] = {0};
	sprintf(framename,"%d",frame); //convert frame no to string
	strncat(framename,"attractor.ppm",13);
	FILE *out = fopen(framename,"w");

	writeimageheader(out);
	int pixels = 0;
	for (int i=0;i<X_BOUND;i++) //rows
	{
		for (int j=0;j<Y_BOUND;j++) //columns
		{
			writepixel(out,i,j,frame_buffer);
			if (pixels==PIXELS_PER_LINE) 
			{
				fprintf(out,"\n");
				pixels=0;
			}
			else pixels++;
		}
	}
	fclose(out);
	frame++;
}

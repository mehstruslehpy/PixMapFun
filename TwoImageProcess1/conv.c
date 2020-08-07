#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#define BUF_SIZE 3 
//usage ./mult.o infile1 infile2 outfile function
void xform(FILE *in1, FILE* in2, FILE* out, uint8_t(*func)(uint8_t,uint8_t));
uint8_t add(uint8_t x,uint8_t y) {return x+y;}
uint8_t mul(uint8_t x,uint8_t y) {return x*y;}
uint8_t avga(uint8_t x,uint8_t y) {return (x+y)/2;}
uint8_t avgg(uint8_t x,uint8_t y) {return sqrt(x*y);}
uint8_t sin1(uint8_t x,uint8_t y) {return y*sin(x);}
uint8_t sin2(uint8_t x,uint8_t y) {return x*sin(y);}
uint8_t sin3(uint8_t x,uint8_t y) {return x*y*sin(x*y);}
uint8_t sinh1(uint8_t x,uint8_t y) {return y*sinh(x);}
uint8_t sinh2(uint8_t x,uint8_t y) {return x*sinh(y);}
uint8_t sinh3(uint8_t x,uint8_t y) {return 100+(x*y*sinh(x*y));}
uint8_t quad1(uint8_t x,uint8_t y) {return x*y*x*y;}
uint8_t quad2(uint8_t x,uint8_t y) {return x*x*y;}
uint8_t quad3(uint8_t x,uint8_t y) {return x*y*y;}
uint8_t quadsum(uint8_t x,uint8_t y) {return 1.0*(x*x+y*y)/(x*y);}
uint8_t logarithm1(uint8_t x,uint8_t y) {return x*log(y);}
uint8_t logarithm2(uint8_t x,uint8_t y) {return y*log(x);}
uint8_t logarithm3(uint8_t x,uint8_t y) {return y*log(x*y);}
uint8_t exponential1(uint8_t x,uint8_t y) {return x*exp(-y);}
uint8_t exponential2(uint8_t x,uint8_t y) {return y*exp(-x);}
uint8_t exponential3(uint8_t x,uint8_t y) {return y*exp(x);}
uint8_t exponential4(uint8_t x,uint8_t y) {return y*exp(x);}
uint8_t modboth1(uint8_t x,uint8_t y) {return (x!=0)?y%x:y;}
uint8_t modboth2(uint8_t x,uint8_t y) {return (y!=0)?x%y:x;}
uint8_t xorboth(uint8_t x,uint8_t y) {return x^y;}
uint8_t andboth(uint8_t x,uint8_t y) {return x&y;}
uint8_t orboth(uint8_t x,uint8_t y) {return x|y;}
uint8_t conditional1(uint8_t x,uint8_t y) {return (~x)|y;}
uint8_t conditional2(uint8_t x,uint8_t y) {return (x)|(~y);}
uint8_t nchoosek1(uint8_t x,uint8_t y) 
{
	return lgamma(x)/(lgamma(y)*lgamma(x-y));
}
uint8_t fibs1(uint8_t x,uint8_t y) 
{
	uint8_t f1=0;
	uint8_t f2=1;
	uint8_t fibx=0;
	uint8_t fiby=0;
	for (int i=2; i<x; i++)  
	{  
		fibx=f1+f2;  
		f1=f2;  
		f2=fibx;  
	} 
	f1=0;
	f2=0;
	for (int i=2; i<y; i++)  
	{  
		fiby=f1+f2;  
		f1=f2;  
		f2=fiby;  
	} 
	
	return fibx+fiby;
}
uint8_t fibs2(uint8_t x,uint8_t y) 
{
	uint8_t f1=0;
	uint8_t f2=1;
	uint8_t fibx=0;
	uint8_t fiby=0;
	for (int i=2; i<x; i++)  
	{  
		fibx=f1+f2;  
		f1=f2;  
		f2=fibx;  
	} 
	f1=0;
	f2=0;
	for (int i=2; i<y; i++)  
	{  
		fiby=f1+f2;  
		f1=f2;  
		f2=fiby;  
	} 
	
	return fibx*fiby;
}
uint8_t xform1(uint8_t x,uint8_t y) 
{
	uint8_t z = 12; //just some constant
	static uint8_t n = 0;
	uint8_t sum  = 0;
	return (sum+=pow(x+y,-n));
}
uint8_t digitsum1(uint8_t x,uint8_t y) 
{
	uint8_t xsum = 0;
	uint8_t ysum = 0;
	while (x!=0) {xsum+=x%10;x/=10;}
	while (y!=0) {ysum+=y%10;y/=10;}
	return (xsum+ysum)*7; //make it brighter
}
uint8_t digitsum2(uint8_t x,uint8_t y) 
{
	uint8_t xy = x+y;
	uint8_t sum = 0;
	while (xy!=0) {sum+=xy%10;xy/=10;}
	return sum*8; //make it brighter
}
uint8_t runsum1(uint8_t x,uint8_t y) 
{
	//need to keep track of each color
	static int count = 0;
	static double sum_red = 0;
	static double sum_grn = 0;
	static double sum_blu = 0;
	if (count%2) return sum_grn += y*exp(-x);
	else if (count%3) return sum_blu += y*exp(-x);
	else return sum_red += y*exp(-x);
}
uint8_t runsum2(uint8_t x,uint8_t y) 
{
	//need to keep track of each color
	static int count = 0;
	static double sum_red = 0;
	static double sum_grn = 0;
	static double sum_blu = 0;
	if (count%2) return sum_grn += y*exp(x);
	else if (count%3) return sum_blu += y*exp(x);
	else return sum_red += y*exp(x);
}
uint8_t runsum3(uint8_t x,uint8_t y) 
{
	//need to keep track of each color
	static int count = 0;
	static double sum_red = 0;
	static double sum_grn = 0;
	static double sum_blu = 0;
	if (count%2) return sum_grn += y*sqrt(x);
	else if (count%3) return sum_blu += y*sqrt(x);
	else return sum_red += y*sqrt(x);
}
int main(int argc, char**argv)
{
	FILE *in1 = fopen(argv[1],"r");
	FILE *in2 = fopen(argv[2],"r");
	FILE *out = fopen(argv[3],"w");

	//verify argc and argv stuff here
	if (argc!=5)
	{
		printf("argc\n");
		return EXIT_FAILURE;
	}

	if (!strcmp(argv[4],"avga")) xform(in1,in2,out,avga);
	else if (!strcmp(argv[4],"avgg")) xform(in1,in2,out,avgg);
	else if (!strcmp(argv[4],"add")) xform(in1,in2,out,add);
	else if (!strcmp(argv[4],"sin1")) xform(in1,in2,out,sin1);
	else if (!strcmp(argv[4],"sin2")) xform(in1,in2,out,sin2);
	else if (!strcmp(argv[4],"sin3")) xform(in1,in2,out,sin3);
	else if (!strcmp(argv[4],"sinh1")) xform(in1,in2,out,sinh1);
	else if (!strcmp(argv[4],"sinh2")) xform(in1,in2,out,sinh2);
	else if (!strcmp(argv[4],"sinh3")) xform(in1,in2,out,sinh3);
	else if (!strcmp(argv[4],"log1")) xform(in1,in2,out,logarithm1);
	else if (!strcmp(argv[4],"log2")) xform(in1,in2,out,logarithm2);
	else if (!strcmp(argv[4],"log3")) xform(in1,in2,out,logarithm3);
	else if (!strcmp(argv[4],"quad1")) xform(in1,in2,out,quad1);
	else if (!strcmp(argv[4],"quad2")) xform(in1,in2,out,quad2);
	else if (!strcmp(argv[4],"quad3")) xform(in1,in2,out,quad3);
	else if (!strcmp(argv[4],"quadsum")) xform(in1,in2,out,quadsum);
	else if (!strcmp(argv[4],"mul")) xform(in1,in2,out,mul);
	else if (!strcmp(argv[4],"runsum1")) xform(in1,in2,out,runsum1);
	else if (!strcmp(argv[4],"runsum2")) xform(in1,in2,out,runsum2);
	else if (!strcmp(argv[4],"runsum3")) xform(in1,in2,out,runsum3);
	else if (!strcmp(argv[4],"exponential1")) xform(in1,in2,out,exponential1);
	else if (!strcmp(argv[4],"exponential2")) xform(in1,in2,out,exponential2);
	else if (!strcmp(argv[4],"exponential3")) xform(in1,in2,out,exponential3);
	else if (!strcmp(argv[4],"exponential4")) xform(in1,in2,out,exponential4);
	else if (!strcmp(argv[4],"logarithm1")) xform(in1,in2,out,logarithm1);
	else if (!strcmp(argv[4],"logarithm2")) xform(in1,in2,out,logarithm2);
	else if (!strcmp(argv[4],"logarithm3")) xform(in1,in2,out,logarithm3);
	else if (!strcmp(argv[4],"modboth1")) xform(in1,in2,out,modboth1);
	else if (!strcmp(argv[4],"modboth2")) xform(in1,in2,out,modboth2);
	else if (!strcmp(argv[4],"xorboth")) xform(in1,in2,out,xorboth);
	else if (!strcmp(argv[4],"orboth")) xform(in1,in2,out,orboth);
	else if (!strcmp(argv[4],"andboth")) xform(in1,in2,out,andboth);
	else if (!strcmp(argv[4],"conditional1")) xform(in1,in2,out,conditional1);
	else if (!strcmp(argv[4],"conditional2")) xform(in1,in2,out,conditional2);
	else if (!strcmp(argv[4],"digitsum1")) xform(in1,in2,out,digitsum1);
	else if (!strcmp(argv[4],"digitsum2")) xform(in1,in2,out,digitsum2);
	else if (!strcmp(argv[4],"fibs1")) xform(in1,in2,out,fibs1);
	else if (!strcmp(argv[4],"fibs2")) xform(in1,in2,out,fibs2);
	else if (!strcmp(argv[4],"xform1")) xform(in1,in2,out,xform1);
	else if (!strcmp(argv[4],"nchoosek1")) xform(in1,in2,out,nchoosek1);
	else
	{
		printf("option\n");
		return EXIT_FAILURE;
	}


	fclose(in1);
	fclose(in2);
	fclose(out);
	return EXIT_SUCCESS;

}
void xform(FILE *in1, FILE* in2, FILE* out, uint8_t (*func)(uint8_t,uint8_t))
{
	int height_max = 0;
	int width_max = 0;
	int height = 0;
	int width = 0;
	char file1in[BUF_SIZE] = {0} ;
	char file2in[BUF_SIZE] = {0} ;

	uint8_t outcolor = 0;
	uint8_t red1 = 0;
	uint8_t grn1 = 0;
	uint8_t blu1 = 0;

	uint8_t red2 = 0;
	uint8_t grn2 = 0;
	uint8_t blu2 = 0;


	//scan the headers
	printf("DEBUG: File 1 scan\n");
	fscanf(in1,"%s",&file1in);
	printf("\t%s\n",file1in);
	fscanf(in1,"%s",&file1in);
	width_max = atoi(file1in);
	printf("\t%s ",file1in);
	fscanf(in1,"%s",&file1in);
	printf("%s\n",file1in);
	height_max = atoi(file1in);
	printf("DEBUG: file1 width=%d, file1 height=%d\n ",width_max,height_max);

	printf("DEBUG: File 2 scan\n");
	fscanf(in2,"%s",&file2in);
	printf("\t%s\n",file2in);
	fscanf(in2,"%s",&file2in);
	width = atoi(file2in);
	printf("\t%s ",file2in);
	fscanf(in2,"%s",&file2in);
	printf("%s\n",file2in);
	height = atoi(file2in);
	printf("DEBUG: file2 width=%d, file2 height=%d\n",width,height);

	if (height_max>height) height_max=height_max;
	else height_max = height;
	if(width_max>width) width_max=width_max;
	else width_max = width;
	printf("DEBUG: file max width=%d, file max height=%d\n",width_max,height_max);

	//write file format type and magic number
	fscanf(in2,"%s",&file2in);
	fscanf(in1,"%s",&file1in);
	fprintf(out,"P3\n",file1in);
	fprintf(out,"%d %d\n",width_max,height_max);
	fprintf(out,"%s\n",file1in);

	for (int i = 0; i < height_max*width_max; i++)
	{
		if (i!=0&&i%3==0) fprintf(out,"\n");
		//read red color for current pixel 
		if (fscanf(in1,"%s",&file1in)) red1 = atoi(file1in);
		else red1 = 0;
		if (fscanf(in2,"%s",&file2in)) red2 = atoi(file2in);
		else red2 = 0;
		outcolor = func(red1,red2);
		printf("DEBUG: red1=%d, red2=%d, outcolor=%d\n",red1,red2,outcolor);
		fprintf(out,"%d ",outcolor);

		//read green color for current pixel 
		if (fscanf(in1,"%s",&file1in)) grn1 = atoi(file1in);
		else grn1 = 0;
		if (fscanf(in2,"%s",&file2in)) grn2 = atoi(file2in);
		else grn2 = 0;
		outcolor = func(grn1,grn2);
		printf("DEBUG: grn1=%d, grn2=%d, outcolor=%d\n",grn1,grn2,outcolor);
		fprintf(out,"%d ",outcolor);

		//read blue color for current pixel 
		if (fscanf(in1,"%s",&file1in)) blu1 = atoi(file1in);
		else blu1 = 0;
		if (fscanf(in2,"%s",&file2in)) blu2 = atoi(file2in);
		else blu2 = 0;
		outcolor = func(blu1,blu2);
		printf("DEBUG: blu1=%d, blu2=%d, outcolor=%d\n",blu1,blu2,outcolor);
		fprintf(out,"%d ",outcolor);
	}
}

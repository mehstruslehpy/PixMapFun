#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#define BUF_SIZE 3 
//usage ./mult.o infile1 infile2 outfile function
void xform(FILE *in1, FILE* in2, FILE* out, uint8_t(*func)(uint8_t,uint8_t));
uint8_t shadowed1(uint8_t x,uint8_t y) {return x+y*0.5;}
uint8_t shadowed2(uint8_t x,uint8_t y) {return x+(255-y*0.5);}
uint8_t max(uint8_t x,uint8_t y) {return (x>y)?x:y;}
uint8_t modd1(uint8_t x,uint8_t y) {return x+(x!=0)?y%x:0;}
uint8_t modd2(uint8_t x,uint8_t y) {return y+(255-(y==0)?0:x%y);}
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

	if (!strcmp(argv[4],"shadowed1")) xform(in1,in2,out,shadowed1);
	else if (!strcmp(argv[4],"shadowed2")) xform(in1,in2,out,shadowed2);
	else if (!strcmp(argv[4],"modd1")) xform(in1,in2,out,modd1);
	else if (!strcmp(argv[4],"modd2")) xform(in1,in2,out,modd2);
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

		//read green color for current pixel 
		if (fscanf(in1,"%s",&file1in)) grn1 = atoi(file1in);
		else grn1 = 0;
		if (fscanf(in2,"%s",&file2in)) grn2 = atoi(file2in);
		else grn2 = 0;

		//read blue color for current pixel 
		if (fscanf(in1,"%s",&file1in)) blu1 = atoi(file1in);
		else blu1 = 0;
		if (fscanf(in2,"%s",&file2in)) blu2 = atoi(file2in);
		else blu2 = 0;

		//compute max color and output to file
		outcolor=max(func(red1,red2),max(func(grn1,grn2),func(blu1,blu2)));
		fprintf(out,"%d ",outcolor);
		fprintf(out,"%d ",outcolor);
		fprintf(out,"%d ",outcolor);
	}
}

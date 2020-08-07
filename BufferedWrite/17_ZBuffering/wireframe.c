#include "wireframe.h"
#define FILE_BUFLEN 2048


void debug_print_wireframe(char* msg,Wireframe* w)
{
	assert(msg!=NULL);
	assert(w!=NULL);
	printf("%s\n",msg);
	printf("\tw.count=%d\n",w->count);
	for (int i=0;i<w->count;i++)
	{
		printf("\tpoly %d:",i);
		debug_print_polygon("",&(w->poly[i]));
	}

}
void read_wireframe_from_disk(char* wire_fname,char* color_fname,Wireframe* w)
{
	assert(wire_fname!=NULL);
	assert(color_fname!=NULL);
	//open files 
	FILE* fp_wire = fopen(wire_fname,"r");
	FILE* fp_color = fopen(color_fname,"r");

	//read line counts
	int wire_line_count = lines_per_file(fp_wire);
	int color_line_count = lines_per_file(fp_color);
	assert(wire_line_count==color_line_count);

	//buf_wire stores the current line of vertices
	//buf_color stores the current line of corresponding rgb values
	char buf_wire[FILE_BUFLEN] = {0};
	char buf_color[FILE_BUFLEN] = {0};

	//allocate the wireframe
	alloc_wire(w,wire_line_count);
	//read in the vertices and corresponding colors 
	for (int i=0;i<wire_line_count;i++)
	{
		//read words per line
		int words_wire = values_per_line(fp_wire);
		int words_color = values_per_line(fp_color);
		assert(words_wire%3==0);
		assert(words_color%3==0);
		assert(words_color==words_wire);
		//allocate a polygon of words/3 vectors
		alloc_poly(&(w->poly[i]),words_wire/3);
		//read line into buffer for tokenization
		fgets(buf_wire,FILE_BUFLEN,fp_wire);
		fgets(buf_color,FILE_BUFLEN,fp_color);

		//read first vector on the line from tokenized buffer
		//for the current polygon
		char* tokenx=strtok(buf_wire,",");//files are comma separated
		char* tokeny=strtok(NULL,",");
		char* tokenz=strtok(NULL,",");

		//store vector into current polygon
		w->poly[i].pt[0].x = atof(tokenx);
		w->poly[i].pt[0].y = atof(tokeny);
		w->poly[i].pt[0].z = atof(tokenz);
		//read the rest of the vectors into the current polygon
		for (int j=1;j<words_wire/3;j++)
		{
			tokenx=strtok(NULL,",");
			tokeny=strtok(NULL,",");
			tokenz=strtok(NULL,",");
			w->poly[i].pt[j].x = atof(tokenx);
			w->poly[i].pt[j].y = atof(tokeny);
			w->poly[i].pt[j].z = atof(tokenz);
		}

		//read first vector on the line from tokenized buffer
		//for the current polygon
		char* tokenred=strtok(buf_color,",");//files are comma separated
		char* tokengrn=strtok(NULL,",");
		char* tokenblu=strtok(NULL,",");
		//store vector into current polygon
		w->poly[i].color[0].red = atoi(tokenred);
		w->poly[i].color[0].grn = atoi(tokengrn);
		w->poly[i].color[0].blu = atoi(tokenblu);
		//read the rest of the vectors into the current polygon
		for (int j=1;j<words_color/3;j++)
		{
			tokenred=strtok(NULL,",");
			tokengrn=strtok(NULL,",");
			tokenblu=strtok(NULL,",");
			w->poly[i].color[j].red = atoi(tokenred);
			w->poly[i].color[j].grn = atoi(tokengrn);
			w->poly[i].color[j].blu = atoi(tokenblu);
		}
	}
	fclose(fp_wire);
	fclose(fp_color);
}
void alloc_wire(Wireframe* w,int count)
{
	w->count=count;
	w->poly=malloc(count*sizeof(Polygon));
}
void dealloc_wire(Wireframe* w)
{
	for (int i=0;i<w->count;i++)
		dealloc_poly(&(w->poly[i]));
	free(w->poly);
	w->poly=NULL;
}

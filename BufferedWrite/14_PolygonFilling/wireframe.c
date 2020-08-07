#include "wireframe.h"
#define BUFLEN 2048

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
void read_wireframe_from_disk(char* fname,Wireframe* w)
{
	//open file and get linecount
	FILE* fp = fopen(fname,"r");
	int line_count = lines_per_file(fp);
	//buf stores the current line
	char buf[BUFLEN] = {0};

	alloc_wire(w,line_count);
	for (int i=0;i<line_count;i++)
	{
		//read words per line
		int words = words_per_line(fp);
		assert(words%3==0);
		//allocate a polygon of words/3 vectors
		alloc_poly(&(w->poly[i]),words/3);
		//read line into buffer for tokenization
		fgets(buf,BUFLEN,fp);
		//read first vector on the line from tokenized buffer
		//for the current polygon
		char* tokenx=strtok(buf," ");
		char* tokeny=strtok(NULL," ");
		char* tokenz=strtok(NULL," ");
		//store vector into current polygon
		w->poly[i].pt[0].x = atof(tokenx);
		w->poly[i].pt[0].y = atof(tokeny);
		w->poly[i].pt[0].z = atof(tokenz);
		//read the rest of the vectors into the current polygon
		for (int j=1;j<words/3;j++)
		{

			tokenx=strtok(NULL," ");
			tokeny=strtok(NULL," ");
			tokenz=strtok(NULL," ");
			w->poly[i].pt[j].x = atof(tokenx);
			w->poly[i].pt[j].y = atof(tokeny);
			w->poly[i].pt[j].z = atof(tokenz);
		}
	}
	fclose(fp);
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

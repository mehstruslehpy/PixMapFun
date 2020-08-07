#include "file.h"
int lines_per_file(FILE* fp)
{
	assert(fp!=NULL);
	int count=0;
	char c = fgetc(fp);
	while (c!=EOF)
	{
		if (c=='\n')
		{
			count++;
		}
		c = fgetc(fp);
	}
	rewind(fp);
	return count;
}
//comma separated words
int values_per_line(FILE* fp)
{
	assert(fp!=NULL);

	//save current position in file for later
	fpos_t pos;
	fgetpos(fp,&pos);

	int count=0;
	char c = '\0';

	//count the number of commas
	//there's one more word than the count of commas 
	//per file
	c=fgetc(fp);
	if (c==',') count++;
	while (c!='\n'&&c!=EOF&&c!='\0')
	{
		if (c==',') count++;
		c=fgetc(fp);
	}
	//reset position in file
	fsetpos(fp,&pos);
	return count+1;
}
//whitespace separated words
int words_per_line(FILE* fp)
{
	assert(fp!=NULL);

	//save current position in file for later
	fpos_t pos;
	fgetpos(fp,&pos);

	int count=0;
	char c = '\0';

	//eat any leading spaces
	while ((c=fgetc(fp))==' ');

	//once we reach this point we are not at a space
	//we will now read to the end of the line
	while (1)
	{
		
		//consume the current word
		c=fgetc(fp);
		while (isalnum(c)) c=fgetc(fp);

		count++;
		if (c=='\n'||c==EOF||c=='\0') break;

		//consume trailing spaces
		c=fgetc(fp);
		while (isspace(c)) c=fgetc(fp);
	}

	//reset position in file
	fsetpos(fp,&pos);
	return count;
}

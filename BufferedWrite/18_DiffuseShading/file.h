#ifndef FILE_H 
#define FILE_H

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define NDEBUG
//read the count of lines in a file
int lines_per_file(FILE* fp);
//read the count of words on the current line of the file
int words_per_line(FILE* fp);
//read the count of (comma-separated) values on the current line of the file
int values_per_line(FILE* fp);
#endif

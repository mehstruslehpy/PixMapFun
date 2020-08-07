#ifndef FILE_H 
#define FILE_H

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define NDEBUG
int lines_per_file(FILE* fp);
int words_per_line(FILE* fp);
int values_per_line(FILE* fp);
#endif

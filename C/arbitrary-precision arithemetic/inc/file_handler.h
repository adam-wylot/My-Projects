#ifndef FILE_HANLDER_H_IS_INCLUDED
#define FILE_HANLDER_H_IS_INCLUDED

#include <stdio.h>

#define SPLIT_SPACE 3



typedef struct {
	FILE* in;			/* strumieñ wejœciowy */
	FILE* out;			/* strumieñ wyjœciowy */
	int line_counter;	/* licznik linii pliku wejœciowego */

} * file_handler_t;


/* === FUNKCJE === */
file_handler_t fh_init(char* in_path, char* out_path);
void fh_free(file_handler_t*);
FILE* fh_makeOutput(char* in_path, char* out_path);

int fh_getNumChar(file_handler_t);

/* "skoki" */
int fh_skipEndLines(file_handler_t);
int fh_skipToNextOper(file_handler_t);
int fh_skipSpaces(file_handler_t);

/* funkcje zapisu */
void fh_writeChar(file_handler_t, char);
void fh_writeString(file_handler_t, char* str, int spaceWidth);
void fh_makeSpace(file_handler_t, int width);


#endif /** FILE_HANLDER_H_IS_INCLUDED **/
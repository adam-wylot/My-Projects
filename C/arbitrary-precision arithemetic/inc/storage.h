#ifndef STORAGE_H_IS_INCLUDED
#define STORAGE_H_IS_INCLUDED

#include "file_handler.h"
#include "operation.h"



extern char* program_name;


int startApp(char* in_path, char* out_path);        /* G³ówna funkcja programu. Odpowiada za ca³¹ sekwencjê pocz¹tkow¹. */
int processOperations(file_handler_t, operation_t); /* Przetwarza plik wejœciowy oraz zawarte w nim i opisane dzia³ania. */
void setProgramName(char* program_path);            /* Pobiera argument wejœciowy o indeksie zero i na jego podstawie ustawia nazwê programu. */
void polishTheConsole(void);                        /* Ustawia obs³ugê znaków polskich w konsoli. */
void readArg(int argc, char** argv);                /* Czyta wstêpnie wprowadzone do programu argumenty i okreœla co ma zrobiæ program */


#endif /** STORAGE_H_IS_INCLUDED **/
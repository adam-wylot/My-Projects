#ifndef OPERATION_H_IS_INCLUDED
#define OPERATION_H_IS_INCLUDED

#include "number.h"
#include "file_handler.h"
#include "oper_type.h"

#define BUFSIZE 7



typedef struct {
	number_t (*func)(number_t, number_t);	/* wskaŸnik do funkcji operacyjnej */
	oper_type_t type;						/* opisuje jakiego typu wystêpuje operacja */
	short base;								/* system liczbowy */
	short conv_base;						/* docelowy system liczbowy — niepuste, je¿eli dzia³aniem jest konwersja systemów liczbowych */

} * operation_t;


/* === FUNKCJE === */
operation_t oper_init(void);
void oper_free(operation_t*);

int oper_define(operation_t, file_handler_t);
int oper_solveArithm(operation_t, file_handler_t);
int oper_solveConv(operation_t, file_handler_t);

#endif /** OPERATION_H_IS_INCLUDED **/
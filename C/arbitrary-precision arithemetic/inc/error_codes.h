#ifndef ERROR_CODES_H_IS_INCLUDED
#define ERROR_CODES_H_IS_INCLUDED



/*? --------------------------========== KODY B£ÊDÓW: ==========--------------------------- */
#define SUCCESS 0
#define FAILURE 1

/*** ============================== PRZESTRZEÑ ZAREZERWOWANA ============================== */
#define MIN_NOT_ERR 2
#define MAX_NOT_ERR 99
/*** ====================================================================================== */

/* B³êdy krytyczne: */
#define MEM_ALLOC_ERR 100       /*! B³¹d alokacji pamiêci */
#define NO_ARG_ERR 110          /*! Nie podano wymaganych argumentów */
#define EMPTY_FILE_ERR 120      /*! Podano pusty plik wejœciowy */

/* b³êdy wykonywania funkcji: */
#define SYNTAX_ERR 210          /* b³¹d formatu pliku */
#define NAN_ERR 220             /* Not a Number (NaN) */
#define DIGIT_OF_ERR 221        /* digit overflow */
#define EMPTY_OPER_ERR 222      /* pusta operacja */
#define INDEF_SIGN_ERR 223      /* wyst¹pi³ symbol nieoznaczony */
/*? --------------------------------------------------------------------------------------- */


#endif /** ERROR_CODES_H_IS_INCLUDED **/

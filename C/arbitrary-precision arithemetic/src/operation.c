#include "operation.h"
#include "num_math.h"
#include "error_codes.h"
#include "file_elems.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


operation_t oper_init(void) {
	operation_t op;

/* ALOKOWANIE POTRZEBNEJ PAMIÊCI */
	op = (operation_t) malloc(sizeof *op);
	if (op == NULL) {
		fprintf(stderr, "[!] B³¹d: niepowodzenie podczas alokowania pamiêci dla operacji!\n");
		return NULL;
	}

	/* Ustawianie wartoœci pocz¹tkowych */
	op->func = NULL;
	op->type = none;
	op->base = 0;
	op->conv_base = 0;
	
	return op;
}

int oper_define(operation_t op, file_handler_t fh) {
	char buf[BUFSIZE] = "\0";	/*** BUFSIZE 7 (6 na znaki + 1 na \0) */
	char buf_cpy[BUFSIZE] = "\0";
	int c = 0;
	int idx = 0;
	short len = 0;


	/* Wczytanie do 6 znaków do bufora, czyli wczytanie "ca³ego" wiersza operacji */
	while ((c = fgetc(fh->in)) != EOF && len < BUFSIZE - 1) {
		buf[len++] = (char) c;

		if (c == '\n' || c == '\r') {
			/* koniec wiersza */
			break;
		}
	} /*** po wykonaniu tej pêtli ostatnim wczytanym znakiem bêdzie znak zapisany w zmiennej c */
	buf[len] = '\0'; /* na dodatkowe miejsce w tablicy ustawiam znak koñcz¹cy ³añcuch znaków */
	fseek(fh->in, -1L, SEEK_CUR); /* zawsze przeczyta jeden znak za du¿o, wiêc cofam strumieñ */


/* === INTERPRETACJA WCZYTANEGO WIERSZA === */
	strcpy(buf_cpy, buf);
	if (c == '\n' || c == '\r') {
		buf_cpy[--len] = '\0';
	}

/* KOLEJNA LICZBA */
	if (isHexNumber(buf_cpy)) {
		fseek(fh->in, -len, SEEK_CUR);

		/* wiersz to prawdopodobnie liczba nale¿¹ca do poprzedniej operacji */
		if (op->type == none) {
			/* nie by³o poprzedniej operacji, wiêc jest to niepoprawnie zapisana operacja */
			return INVALID_OPER;
		}

		/* ! poprzednio zdefiniowana operacja pozostaje bez zmian ! */
		if (op->type == conversion) {
			return CONV_OPER;
		}

		return ARITHM_OPER;
	} /** * endif: prawdopodobnie liczba */


	/* Nie liczba, wiêc sprawdzam, jak¹ operacje opisuje wczytany wiersz zapisuj¹c go do pliku wyjœciowego */
	fh_writeString(fh, buf_cpy, 0);
	if (len < 3) {
		/* nie da siê zapisaæ ¿adnej operacji za pomoc¹ mniej ni¿ 4 znaków (znak + spacja + znak + '\n' == 4), wiêc b³¹d */
		fprintf(stderr, "[!] B³¹d w linii %d: wczytany wiersz z opisem operacji jest za krótki!\n", fh->line_counter);
		return INVALID_OPER;
	}
	op->conv_base = 0;


/* OPERACJA ARYTMETYCZNA */
	if (buf[0] == '+' || buf[0] == '-' || buf[0] == '*' || buf[0] == '/' || buf[0] == '^' || buf[0] == '%') {
	/* Sprawdzanie poprawnoœci wiersza: */
		if (buf[1] != ' ') {
			fprintf(stderr, "[!] B³¹d w linii %d: format wiersza opisuj¹cego operacjê jest nieprawid³owy!\n", fh->line_counter);
			return INVALID_OPER;
		}

		if (!isDecDigit(buf[2])) {
			/* tu powinna byæ cyfra 0-9 */
			fprintf(stderr, "[!] B³¹d w linii %d: format wiersza opisuj¹cego operacjê jest nieprawid³owy!\n", fh->line_counter);
			return INVALID_OPER;
		}
		op->base = (short)(buf[2] - '0');


		/* Okreœlenie d³ugoœci systemu liczbowego */
		if (len == 3) {
			/* operacja na jednocyfrowym systemie liczbowym */
			if (buf[3] != '\n' && buf[3] != '\r') {
				/* tu powinien byæ koniec — b³¹d */
				fprintf(stderr, "[!] B³¹d w linii %d: format wiersza opisuj¹cego operacjê jest nieprawid³owy!\n", fh->line_counter);
				return INVALID_OPER;
			}
		} /*** endif: len == 3 */
		else if (len == 4) {
			/* operacja na systemie liczbowym dwucyfrowym */
			op->base *= 10;

			if (!isDecDigit(buf[3])) {
				/* nie ma 2. cyfry tam, gdzie powinna byæ — b³¹d */
				fprintf(stderr, "[!] B³¹d w linii %d: format wiersza opisuj¹cego operacjê jest nieprawid³owy!\n", fh->line_counter);
				return INVALID_OPER;
			}
			op->base = (short)(op->base + buf[3] - '0');

			if (buf[4] != '\n' && buf[4] != '\r') {
				/* tu powinien byæ koniec — b³¹d */
				fprintf(stderr, "[!] B³¹d w linii %d: format wiersza opisuj¹cego operacjê jest nieprawid³owy!\n", fh->line_counter);
				return INVALID_OPER;
			}
		} /*** endif: len == 4 */
		else {
			/* nie mo¿e byæ takiej opcji */
			fprintf(stderr, "[!] B³¹d w linii %d: format wiersza opisuj¹cego operacjê jest nieprawid³owy!\n", fh->line_counter);
			return INVALID_OPER;
		} /*** endif: okreœlanie d³ugoœci cyfr w systemie liczbowym */


		/* sprawdzenie, czy taki system mo¿e istnieæ */
		if (op->base < MIN_BASE || op->base > MAX_BASE) {
			fprintf(stderr, "[!] B³¹d w linii %d: system liczbowy o podstawie %d nie mo¿e istnieæ!\n", fh->line_counter, op->base);
			fprintf(stderr, "=== Program obs³uguje systemy liczbowe w zakresie [%d-%d] ===\n", MIN_BASE, MAX_BASE);
			return INVALID_OPER;
		}

	/* USTAWIANIE WARTOŒCI OPERACJI */
		switch (buf[0]) {
			case '+': {
				op->func = &add;
				op->type = addition;
				break;
			}
			case '-': {
				op->func = &subtract;
				op->type = subtraction;
				break;
			}
			case '*': {
				op->func = &multiply;
				op->type = multiplication;
				break;
			}
			case '/': {
				op->func = &divide;
				op->type = division;
				break;
			}
			case '%': {
				op->func = &modulo;
				op->type = modulo_op;
				break;
			}
			case '^': {
				op->func = &expand;
				op->type = exponentiation;
				break;
			}
			default: {
				fprintf(stderr, "[!] B³¹d w linii %d: nie rozpoznano funkcji!\n", fh->line_counter);
				return INVALID_OPER;
			}
		}

	fh_makeSpace(fh, 2);

		op->conv_base = 0;
		return ARITHM_OPER;

	} /*** endif: definicja operacji */


/* KONWERSJA SYSTEMÓW LICZBOWYCH */
	idx = 0;
	if (!isDecDigit(buf[idx])) {
		/* pierwszy znak musi byæ cyfr¹ — nie ma innej mo¿liwoœci */
		fprintf(stderr, "[!] B³¹d w linii %d: format wiersza opisuj¹cego operacjê jest nieprawid³owy – nie rozpoznano znaku!\n", fh->line_counter);
		return INVALID_OPER;
	}
	op->base = (short)(buf[idx] - '0');
	++idx;

	if (isDecDigit(buf[idx])) {
		/* jest cyfr¹ — dwucyfrowy system liczbowy */
		op->base *= 10;
		op->base = (short)(op->base + buf[idx] - '0');
		++idx;
	}

	/* sprawdzenie, czy taki system mo¿e istnieæ */
	if (op->base < MIN_BASE || op->base > MAX_BASE) {
		fprintf(stderr, "[!] B³¹d w linii %d: system liczbowy o podstawie %d nie mo¿e istnieæ!\n", fh->line_counter, op->base);
		fprintf(stderr, "=== Program obs³uguje systemy liczbowe w zakresie [%d-%d] ===\n", MIN_BASE, MAX_BASE);
		return INVALID_OPER;
	}

	/* kolejny znak to musi byæ spacja oddzielaj¹ca cyfry systemów liczbowych */
	if (buf[idx] != ' ') {
		fprintf(stderr, "[!] B³¹d w linii %d: format wiersza opisuj¹cego operacjê jest nieprawid³owy!\n", fh->line_counter);
		return INVALID_OPER;
	}
	++idx;

	/* kolejny znak to musi byæ cyfra rozpoczynaj¹ca podstawê drugiego systemu liczbowego */
	if (!isDecDigit(buf[idx])) {
		fprintf(stderr, "[!] B³¹d w linii %d: format wiersza opisuj¹cego operacjê jest nieprawid³owy!\n", fh->line_counter);
		return INVALID_OPER;
	}
	op->conv_base = (short)(buf[idx] - '0');
	++idx;

	/* teraz cyfra lub koniec; je¿eli cyfra to po niej musi byæ koniec */
	if (isDecDigit(buf[idx])) {
		op->conv_base *= 10;
		op->conv_base = (short)(op->conv_base + buf[idx] - '0');
		++idx;
	}

	if (op->conv_base < MIN_BASE || op->conv_base > MAX_BASE) {
		fprintf(stderr, "[!] B³¹d w linii %d: system liczbowy o podstawie %d nie mo¿e istnieæ!\n", fh->line_counter, op->conv_base);
		fprintf(stderr, "=== Program obs³uguje systemy liczbowe w zakresie [%d-%d] ===\n", MIN_BASE, MAX_BASE);
		return INVALID_OPER;
	}


	if (buf[idx] != '\n' && buf[idx] != '\r') {
		/* nie ma koñca — b³¹d */
		fprintf(stderr, "[!] B³¹d w linii %d: format wiersza opisuj¹cego operacjê jest nieprawid³owy!\n", fh->line_counter);
		return INVALID_OPER;
	}

	fh_makeSpace(fh, 2);
	op->type = conversion;
	return CONV_OPER;
}

void oper_free(operation_t* oper_ptr) {
	/* zabezpieczenie przed podaniem pustego lub wczeœniej zwolnionego wskaŸnika */
	if (oper_ptr == NULL || *oper_ptr == NULL) {
		return;
	}

	free(*oper_ptr);
	*oper_ptr = NULL;
}

int oper_solveArithm(operation_t op, file_handler_t fh) {
	number_t num1 = NULL;
	number_t num2 = NULL;
	number_t result_num = NULL;
	char* str = NULL;
	int flag = 0;


/* ALOKACJA PAMIÊCI */
	num2 = num_init(op->base);
	if (num2 == NULL) {
		return MEM_ALLOC_ERR;
	}

	result_num = num_init(op->base);
	if (result_num == NULL) {
		num_free(&num2);
		return MEM_ALLOC_ERR;
	}


	/* pocz¹tkowe pominiêcie wolnego miejsca, o ile nie jest go za du¿o */
	flag = fh_skipEndLines(fh);
	if (flag == EOF) {
		num_free(&result_num);
		num_free(&num2);
		return EMPTY_OPER_ERR;
	}
	if (flag >= SPLIT_SPACE + 1) {
		num_free(&result_num);
		num_free(&num2);
		return SYNTAX_ERR;
	}


	/* czytanie pierwszej liczby */
	flag = num_read(result_num, fh);	/* ? @return: SUCCESS | MEM_ALLOC_ERR | NAN_ERR */
	if (flag != SUCCESS) {
		num_free(&result_num);
		num_free(&num2);
		return flag;
	}

	/* pominiêcie wolnych linii */
	flag = fh_skipEndLines(fh);
	if (flag == EOF) {
		num_free(&result_num);
		num_free(&num2);
		return EMPTY_OPER_ERR;
	}
	if (flag >= SPLIT_SPACE+1) {
		num_free(&result_num);
		num_free(&num2);
		return SYNTAX_ERR;
	}
	fh_makeSpace(fh, 2);


/* CZYTANIE KOLEJNYCH LICZB */
	do {
		/* przepisanie wyniku do liczby numer 1 */
		num1 = result_num;
		result_num = NULL;

		/* czytanie kolejnej liczby */
		flag = num_read(num2, fh);
		if (flag != SUCCESS) {
			num_free(&num1);
			num_free(&num2);
			return flag;
		}


		/* liczenie wyniku poœredniego */
		result_num = op->func(num1, num2);
		if (result_num == NULL) {
			int tmp_flag = FAILURE;

			if (op->type == exponentiation) {
				if (isZero(num1) && isZero(num2)) {
					/* 0^0 */
					tmp_flag = INDEF_SIGN_ERR;
				}
			}
			else if (op->type == division || op->type == modulo_op) {
				if (isZero(num2)) {
					/* 0/0 */
					tmp_flag = INDEF_SIGN_ERR;
				}
			}
			else if (op->type == subtraction) {
				if (compare(num1, num2) < 0) {
					/* x-y < 0 */
					tmp_flag = INDEF_SIGN_ERR;
				}
			}

			num_free(&num1);
			num_free(&num2);
			return tmp_flag;
		}

		/* resetowanie zmiennych */
		num_free(&num1);
		num2->length = 0;

		/* pomijanie przerwy */
		flag = fh_skipEndLines(fh);
		fh_makeSpace(fh, 2);
		if (flag == EOF) {
			break;
		}

	} while (flag < SPLIT_SPACE + 1);


/* KOÑCZENIE OPERACJI */
	/* wypisanie wyniku koñcowego */
	str = num_toString(result_num);
	fh_writeString(fh, str, 0);
	free(str);

	/* zwalnianie zajêtej pamiêci */
	num_free(&result_num);
	num_free(&num2);

	if (flag == EOF) {
		return EOF;
	}
	fh_makeSpace(fh, 2);

	return SUCCESS;
}

int oper_solveConv(operation_t op, file_handler_t fh) {
	number_t result_num = NULL;
	char* str = NULL;
	int flag = 0;

/* ALOKOWANIE PAMIÊCI */
	number_t num = num_init(op->base);
	if (num == NULL) {
		return MEM_ALLOC_ERR;
	}


	/* pocz¹tkowe pominiêcie wolnego miejsca, o ile nie jest go za du¿o */
	flag = fh_skipEndLines(fh);
	if (flag == EOF) {
		num_free(&num);
		return EMPTY_OPER_ERR;
	}
	if (flag >= SPLIT_SPACE + 1) {
		num_free(&num);
		return SYNTAX_ERR;
	}

/* CZYTANIE KOLEJNYCH LICZB */
	do {
		/* czytanie pojedynczej liczby */
		flag = num_read(num, fh); /* ? @return: SUCCESS | MEM_ALLOC_ERR | NAN_ERR */
		if (flag != SUCCESS) {
			num_free(&num);
			return flag;
		}
		fh_makeSpace(fh, 2);

		/* liczenie wyniku */
		result_num = anyToAny(num, op->conv_base);
		if (result_num == NULL) {
			num_free(&num);
			return MEM_ALLOC_ERR;
		}


		/* wypisanie wyniku */
		str = num_toString(result_num);
		fh_writeString(fh, str, 0);
		free(str);


		/* resetowanie zmiennych */
		num->length = 0;
		num_free(&result_num);

		flag = fh_skipEndLines(fh);
		if (flag == EOF) {
			num_free(&num);
			return EOF;
		}

		fh_makeSpace(fh, 2); /*** je¿eli nie koniec pliku to robi przerwê na kolejne liczby */
	} while (flag < SPLIT_SPACE + 1);


	num_free(&num);
	return SUCCESS;
}
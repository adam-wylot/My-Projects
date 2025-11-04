#include "storage.h"
#include "file_handler.h"
#include "error_codes.h"
#include "file_elems.h"
#include "operation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>



char* program_name = "filecalc.exe";

int startApp(char* in_path, char* out_path) {
/* INICJALIZACJA ZMIENNYCH */
	file_handler_t fh = NULL;
	operation_t op = NULL;
	int flag = SUCCESS;


/* PRZYGOTOWANIE PLIKÓW – WEJŒCIOWEGO I WYJŒCIOWEGO */
	fh = fh_init(in_path, out_path);
	if (fh == NULL) {
		return MEM_ALLOC_ERR;
	}


	/* je¿eli s¹ jakieœ puste linie na pocz¹tku pliku to je pomijam */
	if (fh_skipEndLines(fh) == EOF) {
		fh_free(&fh);
		return EMPTY_FILE_ERR;
	}



/* === START PROGRAMU === */
	op = oper_init();
	if (op == NULL) {
		fh_free(&fh);
		return MEM_ALLOC_ERR;
	}

	flag = processOperations(fh, op);


/* === KONIEC PROGRAMU === */
	oper_free(&op);
	fh_free(&fh);

	if (flag != SUCCESS) {
		return flag;
	}

	return SUCCESS;
}

int processOperations(file_handler_t fh, operation_t op) {
	int flag = SUCCESS;
	int counter = 0;

	/* Wykonujê kolejne operacje, dopóki ich wykonywanie koñczy siê sukcesem */
	while (flag == SUCCESS) {
		flag = oper_define(op, fh);
		++counter;

		switch (flag) {
			case INVALID_OPER: {
				fprintf(stderr, "[!] NIE UDA£O SIÊ PRZETWORZYÆ %d. OPERACJI.\n\n", counter);
				flag = fh_skipToNextOper(fh); /*? @return: EOF lub SUCCESS */
				op->func = NULL;
				op->type = none;
				op->base = 0;
				op->conv_base = 0;
				break;
			}
			case ARITHM_OPER: {
				flag = oper_solveArithm(op, fh); /*? @return: SUCCESS | FAILURE | EOF | SYNTAX_ERR | MEM_ALLOC_ERR | EMPTY_OPER_ERR | NAN_ERR */
				break;
			}
			case CONV_OPER: {
				flag = oper_solveConv(op, fh); /*? @return: SUCCESS | EOF | SYNTAX_ERR | MEM_ALLOC_ERR | EMPTY_OPER_ERR | NAN_ERR */
				break;
			}
			default: {
				flag = FAILURE;
				break;
			}
		}


		/* ! OBS£UGA B£ÊDÓW ! */
		if (flag == EOF) {
			return SUCCESS;
		}
		if (flag != SUCCESS) {
			switch (flag) {
				case SYNTAX_ERR: {
					fprintf(stderr, "[!] B³¹d: z³y format pliku wejœciowego!\n");
					break;
				}
				case EMPTY_FILE_ERR: {
					fprintf(stderr, "[!] B³¹d: w operacji nie by³o zawartej minimalnej iloœci argumentów!\n");
					break;
				}
				case NAN_ERR: {
					fprintf(stderr, "[!] B³¹d w linii %d: tekst nie by³ liczb¹!\n", fh->line_counter);
					break;
				}
				case INDEF_SIGN_ERR: {
					fprintf(stderr, "[!] B³¹d w linii %d: wynikiem operacji by³ symbol nieokreœlony!\n", fh->line_counter);
					break;
				}
				case MEM_ALLOC_ERR: {
					fprintf(stderr, "[!] B³¹d: wyst¹pi³ krytyczny b³¹d z pamiêci¹!\n");
					return MEM_ALLOC_ERR;
				}
				default: {
					fprintf(stderr, "[!] B³¹d: wyst¹pi³ nieznany b³¹d!\n");
					return FAILURE;
				}
			}

			fprintf(stderr, "[!] NIE UDA£O SIÊ PRZETWORZYÆ %d. OPERACJI.\n\n", counter);
			flag = fh_skipToNextOper(fh);
			if (flag == EOF) {
				return SUCCESS;
			}
		}
	}

	return flag;
}

void setProgramName(char* program_path) {
	char* moved_path = strrchr(program_path, '\\');
	if (moved_path == NULL) {
		/* nie ma œcie¿ki poprzedzaj¹cej */
		program_name = program_path;
		return;
	}

	++moved_path; /* pominiêcie znaku ('\') */
	program_name = moved_path;
}

void polishTheConsole(void) {
	/* Ustawienie Input/Output na kodowanie znaków Windowsa, a mianowicie 1250 */
	SetConsoleCP(1250);
	SetConsoleOutputCP(1250);

	/* Ustawienie regionu systemu na domyœlny systemowy */
	/*** setlocale(LC_ALL, ""); ***/
}

void readArg(int argc, char** argv) {
	/* za ma³a iloœæ argumentów */
	if (argc < 2) {
		fprintf(stderr, "[!] B³¹d: nie podano wymaganych argumentów do programu!\n");
		fprintf(stderr, "%s: Koñczê dzia³anie programu!\n", program_name);
		fprintf(stderr, "Kod b³êdu: %d\n", NO_ARG_ERR);

		printf("%s: nie otrzymano wymaganej liczby argumentów.\n", program_name);
		printf("----- uruchom z argumentem -h, aby uzyskaæ pomoc -----\n");

		exit(NO_ARG_ERR);
	}


	/* sprawdzenie, czy u¿ytkownik nie chcê instrukcji u¿ywania programu */
	/*? mo¿liwy do u¿ycia by³ getopt(), ale w tym przypadku by³by to overkill */
	if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-?") == 0) {
		printf("--------------------------------------{ %s }--------------------------------------\n", program_name);
		printf("\tU¿ycie: %s [-h] <wejœcie> [wyjœcie]\n\n\n", program_name);
		printf("\tOpcje:\n");
		printf("\t   -h\t| wyœwietla ten podrêcznik\n");
		printf("\t   <wejœcie>\t| œcie¿ka do pliku wejœciowego (wymagana, o ile nie podano -h)\n");
		printf("\t   [wyjœcie]\t| œcie¿ka do pliku wyjœciowego (opcjonalna). Jeœli nie podano\n");
		printf("\t\t\t  wynik zostanie zapisany w \"out_(nazwa pliku wejœciowego).txt\"\n\n\n");
		printf("\tPrzyk³ady:\n");
		printf("\t   %s -h\n", program_name);
		printf("\t   %s input.txt\n", program_name);
		printf("\t   %s input.txt output.txt\n", program_name);
		printf("----------------------------------------------------------------------------------------\n");
		exit(SUCCESS);
	}
}
#include "file_handler.h"
#include "file_elems.h"
#include "error_codes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>


file_handler_t fh_init(char* in_path, char* out_path) {
	file_handler_t fh = NULL;

/* SPRAWDZANIE POPRAWNOŒCI PODANEJ ŒCIE¯KI */
	if (in_path == NULL || strlen(in_path) < 1) {
		fprintf(stderr, "[!] B³¹d: handler plików nie otrzyma³ œcie¿ki do pliku wejœciowego!\n");
		return NULL;
	}


/* ALOKACJA POTRZEBNEJ PAMIÊCI */
	fh = (file_handler_t) malloc(sizeof *fh);
	if (fh == NULL) {
		fprintf(stderr, "[!] B³¹d: niepowodzenie podczas alokowania pamiêci dla handlera plików!\n");
		return NULL;
	}


/* USTALANIE ŒCIE¯KI DO PLIKU WYJŒCIOWEGO */
	fh->out = fh_makeOutput(in_path, out_path);
	if (fh->out == NULL) {
		fprintf(stderr, "[!] B³¹d: nie uda³o siê utworzyæ pliku wyjœciowego!\n");
		free(fh);
		return NULL;
	}



	/* Otwarcie pliku wejœciowego */
	fh->in = fopen(in_path, "r");
	if (fh->in == NULL) {
		fprintf(stderr, "[!] B³¹d: nie uda³o siê otworzyæ pliku: %s!\n", in_path);
		fclose(fh->out);
		free(fh);
		return NULL;
	}

	fh->line_counter = 1;

	return fh;
}

void fh_free(file_handler_t* fh_ptr) {
	/* zabezpieczenie przed podaniem pustego lub wczeœniej zwolnionego wskaŸnika */
	if (fh_ptr == NULL || *fh_ptr == NULL) {
		return;
	}

	/* zamykanie plików */
	if ((*fh_ptr)->in != NULL) {
		fclose((*fh_ptr)->in);
	}
	if ((*fh_ptr)->out != NULL) {
		fclose((*fh_ptr)->out);
	}

	/* zwolnienie pamiêci */
	free(*fh_ptr);
}

FILE* fh_makeOutput(char* in_path, char* out_path) {
	FILE* path = NULL;
	char* moved_path = NULL;
	char* tmp_buf = NULL;

	if (out_path == NULL || strcmp(in_path, out_path) == 0) { /* Nie podano nazwy pliku wyjœciowego */
		char* name_ptr = NULL;

		/* pominiêcie wszystkiego, co jest przed ostatnim backslashem, o ile coœ jest */
		name_ptr = strrchr(in_path, '\\');
		if (name_ptr == NULL) {
			name_ptr = in_path;
		}
		else {
			++name_ptr; /* pominiêcie backslasha ('\') */
		}


		tmp_buf = (char*) malloc((strlen(in_path) + 4 + 4 + 1) * sizeof(char));	/*** +4 dla "out_" | +4 dla .txt | +1 dla '\0' */
		if (tmp_buf == NULL) {
			fprintf(stderr, "[!] B³¹d: wyst¹pi³ b³¹d podczas tworzenia œcie¿ki dla pliku wyjœciowego!\n");
			return NULL;
		}

		strcpy(tmp_buf, in_path);
		moved_path = strrchr(tmp_buf, '\\');
		if (moved_path == NULL) {
			moved_path = tmp_buf;
		}
		else {
			++moved_path;
		}
		moved_path[0] = '\0';


		strcat(tmp_buf, "out_");
		strcat(tmp_buf, name_ptr);
	} /*** endif: brak nazwy pliku wyjœciowego */
	else {
		/* podano nazwê pliku wyjœciowego */
		tmp_buf = (char*) malloc((strlen(out_path) + 4 + 1) * sizeof(char));	/*** +4 dla .txt | +1 dla '\0' */
		if (tmp_buf == NULL) {
			fprintf(stderr, "[!] B³¹d: wyst¹pi³ b³¹d podczas tworzenia œcie¿ki dla pliku wyjœciowego!\n");
			return NULL;
		}

		strcpy(tmp_buf, out_path);
	} /*** endif: podano nazwê pliku wyjœciowego */


	/* dodanie lub zmiana rozszerzenia na *.txt */
	moved_path = strrchr(tmp_buf, '.');
	if (moved_path == NULL) {
		strcat(tmp_buf, ".txt");
	}
	else {
		moved_path = ".txt";
	}

	path = fopen(tmp_buf, "w");
	free(tmp_buf);
	return path;
}



int fh_getNumChar(file_handler_t fh) {
	int c = fgetc(fh->in); /*** char */

	/* koniec linii */
	if (c == '\n' || c == '\r') {
		fseek(fh->in, -1L, SEEK_CUR);
		return END_LINE;
	}
	/* koniec pliku */
	if (c == EOF) {
		return EOF;
	}

	/* skoro nie '\n', nie '\r' i nie EOF to je¿eli nie spacja — zapisujê znak w pliku wyjœciowym */
	if (c != ' ') {
		fh_writeChar(fh, (char)c);
	}


	/*** cyfry: */
	if (c >= '0' && c <= '9') {
		return c - '0';
	}
	if (c >= 'A' && c <= 'F') {
		return c - 'A' + 10;
	}
	if (c >= 'a' && c <= 'f') {
		return c - 'a' + 10;
	}
	/* inne dozwolone znaki: */
	if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^' || c == ' ') {
		return c;
	}

	return UNDEF_ELEM; /* nie rozpoznano znaku */
}



int fh_skipEndLines(file_handler_t fh) {
	int counter = 0;
	int c = 0; /*** char */

	while ((c = fgetc(fh->in)) == '\n' || c == '\r' || c == ' ') {
		if (c == '\n') {
			++counter;
		}
	}
	if (c == EOF) {
		return EOF;
	}
	fseek(fh->in, -1L, SEEK_CUR); /* ? zawsze przeczyta jeden znak za du¿o, wiêc cofam strumieñ */

	fh->line_counter += counter;

	return counter;
}

int fh_skipToNextOper(file_handler_t fh) {
	int c = 0;	/*** char */
	int skipped = fh_skipEndLines(fh);

	if (skipped == EOF) {
		return EOF;
	}
	fh_makeSpace(fh, skipped);

	/* pêtla, która pomija wszystkie znaki, jakie s¹, a¿ nie napotka nastêpnej operacji */
	while (skipped < SPLIT_SPACE + 1) {	/*** +1 poniewa¿ koniec poprzedniej linii te¿ ma znak '\n' */
		/* pomijam zapisan¹ liczbê */
		while ((c = fgetc(fh->in)) != '\n' && c != '\r' && c != EOF) {
			fh_writeChar(fh, (char) c);
		}
		if (c == EOF) {
			return EOF;
		}
		fseek(fh->in, -1L, SEEK_CUR); /* ? zawsze przeczyta jeden znak za du¿o, wiêc cofam strumieñ */


		/* pomijam puste linie */
		skipped = fh_skipEndLines(fh);
		if (skipped == EOF) {
			return EOF;
		}
		fh_makeSpace(fh, skipped);
	}

	return SUCCESS;
}

int fh_skipSpaces(file_handler_t fh) {
	int ch;
	int counter = 0;

	/* liczenie wyst¹pieñ spacji */
	while ((ch = fgetc(fh->in)) == ' ') {
		++counter;
	}
	if (ch == EOF) {
		return EOF;
	}
	fseek(fh->in, -1L, SEEK_CUR);

	return counter;
}



void fh_writeChar(file_handler_t fh, char ch) {
	fprintf(fh->out, "%c", ch);
}

void fh_writeString(file_handler_t fh, char* str, int spaceWidth) {
	fprintf(fh->out, "%s", str);
	fh_makeSpace(fh, spaceWidth);
}

void fh_makeSpace(file_handler_t fh, int width) {
	int i;
	for (i = 0; i < width; ++i) {
		fprintf(fh->out, "\r\n");
	}
}

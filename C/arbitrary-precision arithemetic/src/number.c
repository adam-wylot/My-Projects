#include "number.h"
#include "file_elems.h"
#include "error_codes.h"

#include <stdio.h>
#include <stdlib.h>



number_t num_init(short base) {
	/* Alokowanie pamiêci */
	number_t number = (number_t) malloc(sizeof *number);
	if (number == NULL) {
		fprintf(stderr, "[!] B³¹d: niepowodzenie podczas alokowania pamiêci dla liczby!\n");
		return NULL;
	}
	

	/* Ustawianie wartoœci */
	number->repr = (unsigned char*) malloc(4 * sizeof(unsigned char));
	if (number->repr == NULL) {
		fprintf(stderr, "[!] B³¹d: niepowodzenie podczas alokowania pamiêci dla reprezentacji znakowej liczby!\n");
		free(number);
		return NULL;
	}

	number->repr_size = 4;
	number->length = 0;
	number->base = base;

	return number;
}

number_t num_initWithSize(short base, size_t size) {
	/* Alokowanie pamiêci */
	number_t number = (number_t) malloc(sizeof *number);
	if (number == NULL) {
		fprintf(stderr, "[!] B³¹d: niepowodzenie podczas alokowania pamiêci dla liczby!\n");
		return NULL;
	}


	/* Ustawianie wartoœci */
	number->repr = (unsigned char*) malloc(size * sizeof(unsigned char));
	if (number->repr == NULL) {
		fprintf(stderr, "[!] B³¹d: niepowodzenie podczas alokowania pamiêci dla reprezentacji liczby!\n");
		free(number);
		return NULL;
	}

	number->repr_size = size;
	number->length = 0;
	number->base = base;

	return number;
}

number_t num_copyWithInit(number_t src) {
	size_t i = 0;
	number_t dest = num_initWithSize(src->base, (src->length + 1) / 2);
	if (dest == NULL) {
		return NULL;
	}

	dest->length = src->length;
	for (i=0; i < dest->repr_size; ++i) {
		dest->repr[i] = src->repr[i];
	}

	return dest;
}

int num_expand(number_t num) {
	/* Realokacja i powiêkszenie pamiêci */
	unsigned char* new_ptr = realloc(num->repr, 2 * num->repr_size * sizeof(unsigned char));
	if (new_ptr == NULL) {
		fprintf(stderr, "[!] B³¹d: nie uda³o siê rozszerzyæ pamiêci dla reprezentacji znakowej liczby!\n");
		free(num->repr);
		num->length = 0;
		num->repr_size = 0;

		return FAILURE;
	}

	/* zmiana adresu wskaŸnika */
	num->repr = new_ptr;
	num->repr_size *= 2;
	return SUCCESS;
}

void num_free(number_t* number) {
	/* zabezpieczenie przed podaniem pustego lub wczeœniej zwolnionego wskaŸnika */
	if (number == NULL || *number == NULL)
		return;

	/* Zwalnianie pamiêci */
	if ((*number)->repr != NULL) {
		free((*number)->repr);
	}

	free(*number);
	*number = NULL;
}





unsigned char num_getDigit(number_t num, size_t index) {
	unsigned char ch = num->repr[index / 2];
	unsigned char mask = 0x0F;

	if (index % 2 == 0) { /* lewa po³owa bajtu */
		ch >>= 4;
	}

	return ch & mask;
}

void num_setDigit(number_t num, size_t index, unsigned char digit) {
	unsigned right_mask = 0x0F;
	unsigned left_mask = 0xF0;
	unsigned char tmp = 0;

	/* teoretycznie nie powinno nigdy siê to wydarzyæ, ale jest zabezpieczenie na wszelki wypadek */
	if (digit >= num->base) {
		digit %= num->base;
	}

	/* ustawianie wartoœci */
	tmp = num->repr[index / 2];
	if (index % 2 == 0) { /*** lewa po³owa bajtu */
		tmp &= right_mask;
		digit <<= 4;
		digit += tmp;
	}
	else { /*** prawa po³owa bajtu */
		tmp &= left_mask;
		digit += tmp;
	}

	num->repr[index / 2] = digit;
}

int num_addDigit(number_t num, unsigned char digit) {
	size_t block_idx = 0;

	/* czy liczba jest w zakresie systemu liczbowego */
	if (digit >= num->base) {
		return DIGIT_OF_ERR;
	}

	if (num->repr_size <= num->length / 2) {
		/* ! na ma³o zaalokowanego miejsca — trzeba powiêkszyæ ! */
		if (num_expand(num) != SUCCESS) {
			return MEM_ALLOC_ERR;
		}
	}


	/* Szukanie bloku w tablicy i zapis cyfry do bloku */
	block_idx = num->length / 2;
	digit &= 0x0F;

	if (num->length % 2 == 0) {	/* trzeba dopisaæ po lewej stronie */
		num->repr[block_idx] = digit << 4;
	}
	else { /*** prawa strona */
		num->repr[block_idx] += digit;
	}
	++num->length;

	return SUCCESS;
}





int num_read(number_t num, file_handler_t fh) {
	int ch = 0;
	int flag = 0;

	/* Czytanie znaków */
	num->length = 0;

	while (( (ch = fh_getNumChar(fh)) >= 0 && ch < num->base ) || ch == ' ') {
		if (ch == ' ') {
			int space_count = 1 + fh_skipSpaces(fh);

			if ((ch = fgetc(fh->in)) == '\n' || ch == '\r' || ch == EOF ) {
				/* poprawna liczba */
				fseek(fh->in, -1L, SEEK_CUR);

				ch = 0;
				break;
			}
			fseek(fh->in, -1L, SEEK_CUR); /* cofniêcie przeczytanego znaku */

			/* wypisanie przeczytanych spacji do pliku wyjœciowego */
			while (space_count--) {
				fh_writeChar(fh, ' ');
			}

			return NAN_ERR;
		}

		/* poprawna cyfra, wiêc dodajê j¹ do liczby */
		flag = num_addDigit(num, ch);
		if (flag != SUCCESS) {
			return flag;
		}
	}


/* OBS£UGA B£ÊDÓW */
	/* Czy ch by³o poza systemem liczbowym */
	if (ch >= num->base && ch < MAX_BASE) {
		fprintf(stderr, "[!] B³¹d: napotkano cyfrê, która nie mieœci siê w systemie liczbowym o podanej podstawie!\n");
		return NAN_ERR;
	}

	/* Nierozpoznany znak */
	if (ch == UNDEF_ELEM) {
		fprintf(stderr, "[!] B³¹d: napotkano nieokreœlony znak!\n");
		return NAN_ERR;
	}


/* KOÑCZENIE CZYTANIA LICZBY */
	/* Obraca ³añcuch tak, ¿eby najmniej znacz¹ce cyfry by³y na pocz¹tku */
	if (num_reverse(num) != SUCCESS) {
		return MEM_ALLOC_ERR;
	}


	num_delFrontZeros(num);	/* ? usuwa zera z pocz¹tku liczby */
	return SUCCESS;
}

int num_reverse(number_t num) {
/* INICJALIZACJA ZMIENNYCH */
	unsigned char* new_repr = NULL;
	size_t i = 0;
	size_t j = 0;
	int in_flag = num->length % 2 == 0 ? 0 : 1;
	int out_flag = 0;
	unsigned char n = 0;
	unsigned mask = 0x0F;	/*** maska, która wyzeruje cztery najstarsze bity przez u¿ycie operatora AND {0F(16) == 00001111(2)} */

	if (num->length <= 1) {
		return SUCCESS;
	}

/* ALOKACJA PAMIÊCI */
	new_repr = (unsigned char*) malloc(num->repr_size * sizeof(unsigned char));
	if (new_repr == NULL) {
		fprintf(stderr, "[!] B³¹d: niepowodzenie podczas alokacji pamiêci dla obróconej wersji liczby!\n");
		return MEM_ALLOC_ERR;
	}

	/* Czytanie liczby cyfra po cyfrze */
	for (i = (num->length - 1) / 2;; --i) {
		if (in_flag == 0) { /*** cyfra jest na drugiej po³owie bajtu */
			n = num->repr[i] & mask;

			in_flag = 1;
			++i; /* jeszcze pierwsza po³owa bajtu jest do przeczytania, wiêc cofam iterator, ¿eby czytaæ ten sam bajt drugi raz */
		}
		else { /*** cyfra jest na pierwszej po³owie bajtu */
			n = num->repr[i] >> 4;
			n &= mask;

			in_flag = 0;
		}



		if (out_flag == 0) { /*** zapis cyfry na pierwszej po³owie bajtu */
			new_repr[j] = n << 4;

			out_flag = 1;
		}
		else { /*** zapis cyfry na drugiej po³owie bajtu */
			new_repr[j] += n;

			out_flag = 0;
			++j;
		}

		if (i==0) { break; } /*? size_t jest typem bezznakowym, wiêc nigdy nie bêdzie ujemne – trzeba utworzyæ w³asny break point */
	} /*** koniec pêtli: liczba przeczytana */


	/* Zamiana reprezentacji i zwolnienie pamiêci zajmowanej przez poprzedni¹ reprezentacjê cyfr liczby */
	free(num->repr);
	num->repr = new_repr;

	return SUCCESS;
}

char* num_toString(number_t num) {
	size_t i = 0;
	char* str = NULL;
	unsigned char ch = 0;

/* ALOKOWANIE PAMIÊCI */
	str = (char*) malloc((num->length + 1) * sizeof(char));
	if (str == NULL) {
		fprintf(stderr, "[!] B³¹d: niepowodzenie podczas alokowania pamiêci dla ³añcucha znaków!\n");
		return NULL;
	}

	for (i = 0; i < num->length; ++i) {
		ch = num_getDigit(num, num->length - 1 - i);

		if (ch <= 9) {
			str[i] = (char)(ch + '0');
		}
		else if (ch >= 10 && ch <= 15) {
			str[i] = (char)(ch + 'A' - 10);
		}
		else {
			str[i] = '?';
		}
	}
	str[num->length] = '\0';

	return str;
}

void num_delFrontZeros(number_t num) {
	unsigned char c = 0;
	size_t i = 0;

	for (i = num->length - 1; i > 0; --i) {	/* ostra wiêkszoœæ, poniewa¿ nawet je¿eli najm³odsza cyfra to zero, to nie chcê jej usuwaæ */
		c = num_getDigit(num, i);

		if (c != 0) {
			break;
		}

		--num->length;
	}
}
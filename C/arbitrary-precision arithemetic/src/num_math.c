#include "num_math.h"
#include "error_codes.h"

#include <stdio.h>
#include <stdlib.h>


number_t add(number_t num1, number_t num2) {
	number_t longer_num = NULL, shorter_num = NULL, result_num = NULL;
	size_t i = 0;
	short base = num1->base;
	unsigned char carry = 0, res = 0;


	/* sprawdzenie, czy systemy liczbowe na pewno siê zgadzaj¹ */
	if (num1->base != num2->base) {
		fprintf(stderr, "[!] B³¹d: podane liczby s¹ zapisane w ró¿nych systemach liczbowych!\n");
		return NULL;
	}

	/* rozdzielenie liczb na wiêksz¹ i mniejsz¹ */
	if (num1->length > num2->length) {
		longer_num = num1;
		shorter_num = num2;
	}
	else {
		longer_num = num2;
		shorter_num = num1;
	}


	/* inicjalizacja liczby wynikowej */
	result_num = num_initWithSize(base, longer_num->length / 2 + 1);
	if (result_num == NULL) {
		return NULL;
	}
	result_num->length = longer_num->length;



/* DODAWANIE: */
	/* dodawanie odpowiadaj¹cych sobie cyfr i przenoszenie overflow jako carry do nastêpnej cyfry */
	for (i = 0; i < shorter_num->length; ++i) {
		res = num_getDigit(num1, i) + num_getDigit(num2, i) + carry;
		carry = 0;

		if ((short)res >= base) {
			carry = 1;
			res -= base;
		}

		num_setDigit(result_num, i, res);
	}


	/* przepisywanie cyfr pozosta³ych w d³u¿szej liczbie i przenoszenie carry */
	for (; i < longer_num->length; ++i) {
		res = num_getDigit(longer_num, i) + carry;
		carry = 0;

		if ((short)res >= base) {
			carry = 1;
			res -= base;
		}

		num_setDigit(result_num, i, res);
	}

	
	/* dopisanie carry na najstarszej cyfry, o ile ró¿ne od zera */
	if (carry != 0) {
		num_addDigit(result_num, carry); /*? nie wyrzuci b³êdu, poniewa¿ jest ju¿ wczeœniej zaalokowana pamiêæ na tê cyfrê */
	}


	return result_num;
}

number_t subtract(number_t num1, number_t num2) {
	number_t result_num = NULL;
	size_t i = 0, j = 0;
	int res = 0;
	short base = num1->base;
	unsigned char ch1 = 0, ch2 = 0, tmp = 0;

	/* sprawdzenie, czy systemy liczbowe na pewno siê zgadzaj¹ */
	if (num1->base != num2->base) {
		fprintf(stderr, "[!] B³¹d: podane liczby s¹ zapisane w ró¿nych systemach liczbowych!\n");
		return NULL;
	}

	/* wstêpne porównanie tych liczb */
	res = compare(num1, num2);
	if (res == 0) {
		/* liczby s¹ takie same — wynik to zero */
		result_num = num_initWithSize(base, 1);
		if (result_num == NULL) {
			return NULL;
		}

		num_addDigit(result_num, 0); /*? nie wyrzuci b³êdu, poniewa¿ jest ju¿ wczeœniej zaalokowana pamiêæ na tê cyfrê */
		return result_num;
	}
	if (res == -1) {
		/* ! Wynik odejmowania bêdzie ujemny ! */
		fprintf(stderr, "[!] B³¹d: ujemna ró¿nica!\n");
		return NULL;
	}


/* === ALGORYTM ODEJMOWANIA === */
	/* KROK 1 — przepisanie num1 do result */
	result_num = num_copyWithInit(num1);
	if (result_num == NULL) {
		return NULL;
	}


	/* KROK 2 — odejmowanie odpowiadaj¹cych sobie cyfr result_num (num1) i num2 */
	for (i = 0; i < num2->length; ++i) {
		ch1 = num_getDigit(result_num, i);
		ch2 = num_getDigit(num2, i);

		if (ch1 < ch2) {
			/* cyfra za ma³a, wiêc nastêpuje "po¿yczenie" od nastêpnych */
			tmp = 0;
			j = 0;

			ch1 += base;

			for (j = i+1; j < num1->length; ++j) {
				tmp = num_getDigit(result_num, j);

				if (tmp > 0) {
					num_setDigit(result_num, j, tmp - 1);
					break;
				}

				num_setDigit(result_num, j, (unsigned char)base - 1);
			}
		}

		num_setDigit(result_num, i, ch1 - ch2);
	}


	/* KROK 3 — korekcja d³ugoœci liczby wynikowej */
	num_delFrontZeros(result_num);

	return result_num;
}

number_t multiply(number_t num1, number_t num2) {
	number_t longer_num = NULL, shorter_num = NULL, result_num = NULL;
	size_t i = 0, j = 0;
	int res = 0;
	short base = num1->base;
	unsigned char ch1 = 0, ch2 = 0, carry = 0;

	/* sprawdzenie, czy systemy liczbowe na pewno siê zgadzaj¹ */
	if (num1->base != num2->base) {
		fprintf(stderr, "[!] B³¹d: podane liczby s¹ zapisane w ró¿nych systemach liczbowych!\n");
		return NULL;
	}

	/* sprawdzenie, czy przypadkiem jeden ze sk³adników nie jest zerem — jeœli jest to mno¿enie jest zbêdne */
	if (isZero(num1) || isZero(num2)) {
		result_num = num_initWithSize(base, 1);
		if (result_num == NULL) {
			return NULL;
		}
		num_addDigit(result_num, 0); /*? nie wyrzuci b³êdu, poniewa¿ jest ju¿ wczeœniej zaalokowana pamiêæ na tê cyfrê */

		return result_num;
	}

	/* inicjalizacja zmiennej wynikowej */
	result_num = num_initWithSize(base, (num1->length + num2->length + 1) / 2);
	if (result_num == NULL) {
		return NULL;
	}

	/* okreœlanie, która liczba jest wiêksza */
	if (num1->length > num2->length) {
		longer_num = num1;
		shorter_num = num2;
	}
	else {
		longer_num = num2;
		shorter_num = num1;
	}



/* === ALGORYTM MNO¯ENIA: === */
	for (i = 0; i < (num1->length + num2->length + 1) / 2; ++i) {
		/* zerowanie pustych miejsc, ¿eby zapobiec b³êdom, gdy pojawi¹ siê zera na koñcu wyniku */
		result_num->repr[i] = 0;
	}

	for (i = 0; i < shorter_num->length; ++i) {
		/* pomijanie zer w mno¿eniu — wynik mno¿enia przez zero daje zero */
		ch1 = num_getDigit(shorter_num, i);
		if (ch1 == 0) {
			continue;
		}

		/* jak ju¿ znalaz³o cyfrê ró¿n¹ od zera to mno¿y przez ni¹ cyfry drugiej liczby */
		for (j = 0; j < longer_num->length; ++j) {
			ch2 = num_getDigit(longer_num, j);
			res = (int) num_getDigit(result_num, j+i);

			res += (int) ch1 * ch2 + carry;
			carry = (unsigned char)(res / base);
			res %= base;

			num_setDigit(result_num, j+i, (unsigned char)res);
		}

		/* je¿eli carry ró¿ne od zera to dopisuje je na najstarszym bicie */
		if (carry != 0) {
			num_setDigit(result_num, longer_num->length + i, carry);
			carry = 0;
		}
	}


	/* ustalanie d³ugoœci liczby wynikowej */
	if (num_getDigit(result_num, num1->length + num2->length - 1) == 0) {
		result_num->length = num1->length + num2->length - 1;
	}
	else {
		result_num->length = num1->length + num2->length;
	}


	return result_num;
}

number_t divide(number_t num1, number_t num2) {
	number_t result_num = NULL, subs1_num = NULL, subs2_num = NULL, rest_num = NULL;
	size_t i = 0, j = 0;	/*iteratory*/
	short base = num1->base;
	int res = 0;

	/* Obs³uga b³êdów */
	if (num1->base != num2->base) {
		fprintf(stderr, "[!] B³¹d: podane liczby s¹ zapisane w ró¿nych systemach liczbowych!\n");
		return NULL;
	}
	if (isZero(num2)) {
		fprintf(stderr, "[!] B³¹d: wyst¹pi³o dzielenie przez zero!\n");
		return NULL;
	}


/* Sprawdzanie szczególnych sytuacji */
	res = compare(num1, num2);

	if (isZero(num1) || res == -1) {
		/* licznik to zero lub dzielnik jest wiêkszy ni¿ dzielna — wynik to zero */
		result_num = num_initWithSize(base, 1);
		if (result_num == NULL) {
			return NULL;
		}
		num_addDigit(result_num, 0); /*? nie wyrzuci b³êdu, poniewa¿ jest ju¿ wczeœniej zaalokowana pamiêæ na tê cyfrê */
		return result_num;
	}
	if (res == 0) {
		/* dzielnik i dzielna s¹ takie same — wynik to jeden */
		result_num = num_initWithSize(base, 1);
		if (result_num == NULL) {
			return NULL;
		}
		num_addDigit(result_num, 1); /*? nie wyrzuci b³êdu, poniewa¿ jest ju¿ wczeœniej zaalokowana pamiêæ na tê cyfrê */
		return result_num;
	}


/* alokowanie potrzebnej pamiêci */
	result_num = num_initWithSize(base, num1->repr_size);
	if (result_num == NULL) {
		return NULL;
	}

	/*? wczeœniejsza alokacja pamiêci, ¿eby potem dodawanie cyfr nie musia³o byæ sprawdzane — nie ma szans na przekroczenie tego rozmiaru */
	subs1_num = num_initWithSize(base, num2->length / 2 + 1);
	if (subs1_num == NULL) {
		num_free(&result_num);
		return NULL;
	}


/* === ALGORYTM DZIELENIA === */
	for (i = num1->length - 1;; --i) {
		/* je¿eli jest reszta z dzielenia to j¹ przepisujê, ale od ty³u (najbardziej znacz¹ce cyfry z przodu) */
		if (rest_num != NULL) {
			if (rest_num->length > 0 && !isZero(rest_num)) {
				for (j = rest_num->length - 1;; --j) {
					num_addDigit(subs1_num, num_getDigit(rest_num, j));
					if (j==0) { break; } /*? size_t jest typem bezznakowym, wiêc nigdy nie bêdzie ujemne – trzeba utworzyæ w³asny break point */
				}
			}

			num_free(&rest_num);
		}


		/* ! dodanie kolejnej cyfry z dzielnej i obrócenie liczby ! */
		num_addDigit(subs1_num, num_getDigit(num1, i));
		num_reverse(subs1_num);

		/* proste dzielenie */
		res = simpleDiv(subs1_num, num2);
		num_addDigit(result_num, (unsigned char) res);

		/* mno¿enie */
		subs2_num = multByConst(num2, res);
		if (subs2_num == NULL) {
			num_free(&subs1_num);
			num_free(&result_num);
			return NULL;
		}

		/* odejmowanie */
		rest_num = subtract(subs1_num, subs2_num);
		if (rest_num == NULL) {
			num_free(&subs1_num);
			num_free(&subs2_num);
			num_free(&result_num);
			return NULL;
		}

		/* resetowanie zmiennych */
		subs1_num->length = 0;
		num_free(&subs2_num);
		subs2_num = NULL;

		if (i==0) { break; } /*? w³asny break point */
	}


	/* Zwalnianie pamiêci i koñczenie algorytmu */
	num_free(&subs1_num);
	num_free(&rest_num);

	num_reverse(result_num);
	num_delFrontZeros(result_num);
	return result_num;
}

number_t expand(number_t num1, number_t num2) {
	number_t result_num = NULL, tmp_num = NULL, copy_of_num1 = NULL, bin_num2 = NULL;
	size_t i = 0;
	unsigned char c = 0;

	/* Obs³uga specjalnych sytuacji */
	if (num1->base != num2->base) {
		fprintf(stderr, "[!] B³¹d: podane liczby s¹ zapisane w ró¿nych systemach liczbowych!\n");
		return NULL;
	}
	if (isZero(num1) && isZero(num2)) {
		/* 0 do potêgi 0 — symbol nieokreœlony */
		fprintf(stderr, "[!] B³¹d: nie mo¿na podnieœæ zera do potêgi zerowej!\n");
		return NULL;
	}
	if (isZero(num1)) {
		/* zero do potêgi jakiejœ — wynik to zero */
		result_num = num_initWithSize(num1->base, 1);
		if (result_num == NULL) {
			return NULL;
		}
		num_addDigit(result_num, 0); /*? nie wyrzuci b³êdu, poniewa¿ jest ju¿ wczeœniej zaalokowana pamiêæ na tê cyfrê */

		return result_num;
	}


/* INNE PRZYPADKI: */
	/* alokacja pamiêci */
	result_num = num_init(num1->base);
	if (result_num == NULL) {
		return NULL;
	}
	num_addDigit(result_num, 1); /*? nie wyrzuci b³êdu, poniewa¿ jest ju¿ wczeœniej zaalokowana pamiêæ na tê cyfrê */

	bin_num2 = anyToAny(num2, 2);
	if (bin_num2 == NULL) {
		num_free(&result_num);
		return NULL;
	}

	copy_of_num1 = num_copyWithInit(num1);
	if (copy_of_num1 == NULL) {
		num_free(&result_num);
		num_free(&bin_num2);
		return NULL;
	}


/* ALGORYTM */
	for (i = 0; i < bin_num2->length; ++i) {
		c = num_getDigit(bin_num2, i);

		if (c == 1) {
			tmp_num = result_num;
			result_num = multiply(result_num, copy_of_num1);
			num_free(&tmp_num);
		}

		tmp_num = copy_of_num1;
		copy_of_num1 = multiply(copy_of_num1, copy_of_num1);
		num_free(&tmp_num);
	}

	num_free(&bin_num2);
	num_free(&copy_of_num1);
	return result_num;
}

number_t modulo(number_t num1, number_t num2) {
	number_t result_num = NULL, tmp_num = NULL;
	int res = 0;

	if (num1->base != num2->base) {
		fprintf(stderr, "[!] B³¹d: podane liczby s¹ zapisane w ró¿nych systemach liczbowych!\n");
		return NULL;
	}
	if (isZero(num2)) {
		fprintf(stderr, "[!] B³¹d: wyst¹pi³o dzielenie przez zero!\n");
		return NULL;
	}

	res = compare(num1, num2);
	if (res == 0) {
		/* liczby s¹ takie same — reszta z dzielenia wynosi 0 */
		result_num = num_initWithSize(num1->base, 1);
		if (result_num == NULL) {
			return NULL;
		}
		num_addDigit(result_num, 0); /*? nie wyrzuci b³êdu, poniewa¿ jest ju¿ wczeœniej zaalokowana pamiêæ na tê cyfrê */

		return result_num;
	}
	if (res == -1) {
		/* dzielnik (num2) jest wiêkszy od dzielnej (num1) — wynik to dzielna (num1) */
		result_num = num_copyWithInit(num1);
		if (result_num == NULL) {
			return NULL;
		}

		return result_num;
	}


/* === ALGORYTM === */
	result_num = divide(num1, num2);
	if (result_num == NULL) {
		return NULL;
	}

	tmp_num = multiply(result_num, num2);
	if (tmp_num == NULL) {
		num_free(&result_num);
		return NULL;
	}
	num_free(&result_num);

	result_num = subtract(num1, tmp_num);
	if (result_num == NULL) {
		num_free(&tmp_num);
		return NULL;
	}
	num_free(&tmp_num);

	return result_num;
}

number_t decToAny(number_t num, short base) {
	number_t result_num = NULL, base_num = NULL, copy_of_num = NULL, tmp_num = NULL;
	int digit = 0;


/* OBS£UGA B£ÊDÓW I SYTUACJI WYJ¥TKOWYCH */
	if (num->base != 10) {
		fprintf(stderr, "[!] B³¹d: konwersja liczby dziesiêtnej na inny system liczbowy by³a niemo¿liwa, poniewa¿ podana liczba nie by³a dziesiêtna!\n");
		return NULL;
	}

	if (isZero(num)) {
		/* je¿eli zero to zero */
		number_t zero_num = num_initWithSize(base, 1);
		if (zero_num == NULL) {
			return NULL;
		}
		num_addDigit(zero_num, 0); /*? nie wyrzuci b³êdu, poniewa¿ jest ju¿ wczeœniej zaalokowana pamiêæ na tê cyfrê */
		return zero_num;
	}


/* INICJALIZACJA */
	result_num = num_init(base);
	if (result_num == NULL) {
		return NULL;
	}

	base_num = intToNum(base, 10);
	if (base_num == NULL) {
		num_free(&result_num);
		return NULL;
	}

	copy_of_num = num_copyWithInit(num);
	if (copy_of_num == NULL) {
		num_free(&result_num);
		num_free(&base_num);
		return NULL;
	}


/* === ALGORYTM === */
	while (!isZero(copy_of_num)) {
		/* wyznaczanie cyfry — reszta z dzielenia przez podstawê */
		/* ! Program dzia³a w systemach liczbowych o podstawie z zakresu [2-16], wiêc mam pewnoœæ, ¿e tmp_num <= 15(10) ! */
		tmp_num = modulo(copy_of_num, base_num);
		if (tmp_num == NULL) {
			num_free(&result_num);
			num_free(&base_num);
			num_free(&copy_of_num);

			return NULL;
		}

		digit = numToInt(tmp_num);

		num_free(&tmp_num);
		if (digit < 0) {
			/* obs³uga b³êdów */
			num_free(&result_num);
			num_free(&base_num);
			num_free(&copy_of_num);

			return NULL;
		}

		if (num_addDigit(result_num, (unsigned char)digit) != SUCCESS) {
			fprintf(stderr, "[!] B³¹d: nie uda³o siê dodaæ cyfry do liczby!\n");
			num_free(&result_num);
			num_free(&base_num);
			num_free(&copy_of_num);

			return NULL;
		}


		tmp_num = copy_of_num;
		copy_of_num = divide(copy_of_num, base_num);
		if (copy_of_num == NULL) {
			num_free(&tmp_num);
			num_free(&result_num);
			num_free(&base_num);

			return NULL;
		}

		num_free(&tmp_num);
	}


/* ZWALNIANIE PAMIÊCI */
	num_free(&base_num);
	num_free(&copy_of_num);

	return result_num;
}

number_t anyToDec(number_t num) {
	number_t result_num = NULL;
	number_t tmp_num = NULL;
	size_t i = 0;
	unsigned char digit = 0;

	if (isZero(num)) {
		/* je¿eli zero to zero */
		number_t zero_num = num_initWithSize(10, 1);
		if (zero_num == NULL) {
			return NULL;
		}
		num_addDigit(zero_num, 0); /*? nie wyrzuci b³êdu, poniewa¿ jest ju¿ wczeœniej zaalokowana pamiêæ na tê cyfrê */
		return zero_num;
	}

	result_num = num_init(10);
	if (result_num == NULL) {
		return NULL;
	}
	if (num_addDigit(result_num, 0) != SUCCESS) {
		fprintf(stderr, "[!] B³¹d: nie uda³o siê dodaæ cyfry do liczby!\n");
		num_free(&result_num);
		return NULL;
	}

	for (i = num->length - 1;; --i) {
		digit = num_getDigit(num, i);

		tmp_num = result_num;
		result_num = multByConst(result_num, (unsigned char) num->base);
		if (result_num == NULL) {
			num_free(&tmp_num);
			return NULL;
		}
		num_free(&tmp_num);

		addConst(result_num, digit);

		if (i==0) { break; } /*? size_t jest typem bezznakowym, wiêc nigdy nie bêdzie ujemne – trzeba utworzyæ w³asny break point */
	}

	return result_num;
}

number_t anyToAny(number_t num, short base) {
	number_t result_num = NULL;
	number_t tmp_num = NULL;

	tmp_num = anyToDec(num);
	if (tmp_num == NULL) {
		return NULL;
	}

	result_num = decToAny(tmp_num, base);
	if (result_num == NULL) {
		num_free(&tmp_num);
		return NULL;
	}
	num_free(&tmp_num);

	return result_num;
}



int compare(number_t num1, number_t num2) {
	size_t i = 0;
	unsigned char ch1 = 0, ch2 = 0;

	/**
	 * 1: num1 > num2
	 * 0: num1 == num2
	 * -1: num1 < num2
	 */

	/* sprawdzenie d³ugoœci liczb: wiêcej cyfr == wiêksza liczba */
	if (num1->length > num2->length) {
		return 1;
	}
	if (num1->length < num2->length) {
		return -1;
	}


	/* w przeciwnym wypadku maj¹ tyle samo cyfr, wiêc sprawdzam od tej najbardziej znacz¹cej, która jest wiêksza */
	for (i = (num1->length - 1) / 2;; --i) {
		if (num1->repr[i] != num2->repr[i]) {
			/* Skoro ca³y bajt (czyli dwie cyfry) ró¿ni siê od drugiego to jego po³owy te¿ (czyli pojedyñcze cyfry) */
			ch1 = num_getDigit(num1, 2*i + 1);
			ch2 = num_getDigit(num2, 2*i + 1);

			if (ch1 != ch2) {
				/*** prawa po³owa bajtu */
				if (ch1 > ch2) {
					return 1;
				}

				return -1;
			}

			/* lewa po³owa bajtu */
			ch1 = num_getDigit(num1, 2*i);
			ch2 = num_getDigit(num2, 2*i);

			if (ch1 > ch2) {
				return 1;
			}

			return -1;
		}

		if (i==0) { break; } /*? size_t jest typem bezznakowym, wiêc nigdy nie bêdzie ujemne – trzeba utworzyæ w³asny break point */
	}

	return 0;
}

int simpleDiv(number_t num1, number_t num2) {
	int res = 0;
	number_t result_num = NULL;
	number_t tmp_num = NULL;

	if (isZero(num2)) {
		/* dzielenie przez zero */
		return -1;
	}

	result_num = num_copyWithInit(num1);
	if (result_num == NULL) {
		return -2;
	}

	while (compare(result_num, num2) >= 0) {
		tmp_num = subtract(result_num, num2);
		num_free(&result_num);
		if (tmp_num == NULL) {
			fprintf(stderr, "[!] B³¹d: nieudane proste dzielenie przez nieznany b³¹d!\n");
			return -3;
		}
		result_num = tmp_num;
		++res;
	}

	num_free(&result_num);
	return res;
}

number_t multByConst(number_t num, unsigned char value) {
	number_t result_num = NULL;
	size_t i = 0;
	unsigned res = 0;
	unsigned char carry = 0;

	result_num = num_initWithSize(num->base, (num->length + 4) / 2);
	if (result_num == NULL)  {
		return NULL;
	}

	for (i = 0; i < num->length; ++i) {
		res = (unsigned)num_getDigit(num, i) * value + carry;
		carry = (unsigned char)(res / num->base);
		res %= num->base;

		num_addDigit(result_num, (unsigned char)res); /*? nie wyrzuci b³êdu, poniewa¿ jest ju¿ wczeœniej zaalokowana pamiêæ na tê cyfrê */
	}

	/* rozpisywanie pozosta³oœci */
	while (carry > 0) {
		num_addDigit(result_num, carry % num->base); /*? nie wyrzuci b³êdu, poniewa¿ jest ju¿ wczeœniej zaalokowana pamiêæ na tê cyfrê */
		carry /= num->base;
	}

	num_delFrontZeros(result_num); /* mo¿e pojawiæ siê mno¿enie przez zero i w wyniku bêdzie du¿a liczba zer */
	return result_num;
}

void addConst(number_t num, unsigned char value) {
	size_t i = 0;
	unsigned char res = 0;

	for (i = 0; i < num->length; ++i) {
		if (value == 0) {
			break;
		}

		res = num_getDigit(num, i) + value;
		value = res / num->base;
		res %= num->base;

		num_setDigit(num, i, res);
	}

	if (value != 0) {
		if (num_addDigit(num, value) != SUCCESS) {
			fprintf(stderr, "[!] B³¹d: nie uda³o siê dodaæ cyfry do liczby! [Overflow]\n");
		}
	}
}

int numToInt(number_t num) {
	size_t i = 0;
	int result = 0, ext = 1;
	short base = num->base;
	unsigned char c = 0;

	for (i = 0; i < num->length; ++i) {
		c = num_getDigit(num, i);
		if (result > INT_MAX - c * ext) {
			return -1;
		}

		result += c * ext;
		ext *= base;
	}

	return result;
}

number_t intToNum(int value, short base) {
	int digit = 0;

	number_t result_num = num_init(base);
	if (result_num == NULL) {
		return NULL;
	}

	while (value > 0) {
		digit = value % base;
		value /= base;
		if (num_addDigit(result_num, (unsigned char) digit) != SUCCESS) {
			fprintf(stderr, "[!] B³¹d: nie uda³o siê dodaæ cyfry do liczby!\n");
			num_free(&result_num);
			return NULL;
		}
	}

	return result_num;
}


bool isZero(number_t num) {
	num_delFrontZeros(num);

	if (num->length == 1 && num_getDigit(num, 0) == 0) {
		return true;
	}

	return false;
}

bool isHexNumber(char* str) {
	/* iteracja po ka¿dym elemencie ³añcucha, a¿ do napotkania '\0' */
	while (*str) {
		if (!isHexDigit(*str)) {
			/* je¿eli nie jest cyfr¹ to zwraca 0 (false) */
			return false;
		}

		++str;
	}

	return true;
}

bool isDecDigit(char c) {
	if (c >= '0' && c <= '9') {
		return true;
	}

	return false;
}

bool isHexDigit(char c) {
	/* sprawdza, czy znak nale¿y do zbioru 0-9 lub A-F z pominiêciem wielkoœci litery */
	if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) {
		return true;
	}

	return false;
}
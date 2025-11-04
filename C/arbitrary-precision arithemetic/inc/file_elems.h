#ifndef FILE_ELEMS_H_IS_INCLUDED
#define FILE_ELEMS_H_IS_INCLUDED



/*? W tym pliku znajduj¹ siê opisane wszystkie sytuacje, na jakie mo¿e trafiæ program czytaj¹c plik wejœciowy */

typedef enum {
	MIN_BASE = 2,
	/*** liczby [3-9] s¹ zarezerwowane dla cyfr o tych numerach */
	A_ = 10,
	B_ = 11,
	C_ = 12,
	D_ = 13,
	E_ = 14,
	F_ = 15,
	MAX_BASE = 16,

	NEXT_NUM = 20,
	END_LINE = 21,
	UNDEF_ELEM = 22,
	NEXT_OPER = 23,
	INVALID_OPER = 24,
  	ARITHM_OPER = 25,
	CONV_OPER = 26

} file_elems_t;

#endif /** FILE_ELEMS_H_IS_INCLUDED **/

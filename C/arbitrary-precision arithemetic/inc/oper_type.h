#ifndef OPER_TYPE_H_IS_INCLUDED
#define OPER_TYPE_H_IS_INCLUDED



/*? W tym pliku znajdują się wszystkie dostępne operacje */

typedef enum {
  none = 0,
  addition,
  subtraction,
  multiplication,
  division,
  modulo_op,
  exponentiation,
  conversion

} oper_type_t;

#endif /** OPER_TYPE_H_IS_INCLUDED **/

/*
 * Copyright (C) 2017 Tieto Poland Sp. z o. o.
 */

/** @file

  \brief Homekit floating point number library.

  Standard float type guarantees six digits precision. According to HomeKit specification the accuracy of
  'Current Ambient Light Level' characteristic is 10 digits (value in range 0.0001-100000 lux). The HAP float library
  is an implementation of floating point number which guarantees ten digits precision in range <-2147483647, 2147483648>
  and precision from 0 to 256. The value of HAP float is always round down.
*/

#ifndef _JSON_FLOAT_H_
#define _JSON_FLOAT_H_

#include <stdint.h>
#include "base.h"

/** @addtogroup HOMEKIT_ACCESSORY_PROTOCOL
@{
*/

/** Hap decimal fraction structure */
typedef struct
{
  A_INT32 value;     ///< Coded value
  A_UINT8 precision; ///< Required precision. Number of digits after decimal point.
} json_float_t;

/** Hap float sign definitions */
typedef enum
{
  JSON_FLOAT_SIGN_PLUS = 0, ///< For plus
  JSON_FLOAT_SIGN_MINUS     ///< For minus
} json_float_sign_t;

/** Hap float precision definitions required by Homekit specification */
typedef enum
{
  JSON_FLOAT_PRECISION_0 = 0,  ///< 0
  JSON_FLOAT_PRECISION_1,      ///< 0.1
  JSON_FLOAT_PRECISION_2,      ///< 0.01
  JSON_FLOAT_PRECISION_3,      ///< 0.001
  JSON_FLOAT_PRECISION_4,      ///< 0.0001
  JSON_FLOAT_PRECISION_UNUSED, ///< Larger precision. Not required by specification.
} json_float_precision_t;

/** HAP float constructor

  \param [in] sign Sign of the constructed HAP float
  \param [out] int_part Integer part
  \param [in] fract_part Fraction part
  \param [in] precision Required precision

  \return
    HAP float structure
*/
json_float_t json_float(json_float_sign_t sign, A_UINT32 int_part, A_UINT32 fract_part, A_UINT8 precision);

/** Calculate precision from string, e.g. "2233.45" returns 2

  \param [in] str Initialization string

  \return
    Precision
*/
A_UINT8 json_float_get_precision_from_string(const A_CHAR *str);

/** Construct HAP float from string

  \param [in] str Initialization string
  \param [in] precision Required precision. String precision is ignored.

  \return
    HAP float structure
*/
json_float_t json_float_from_string(const A_CHAR *str, A_UINT8 precision);

/** Convert HAP float to string

  \param [in] f Initialization HAP float
  \param [out] str Returned string with float representation. The function does not allocate memory for this buffer

  \return
    None
*/
void json_float_to_string(const json_float_t *f, A_CHAR *str);

/** Add two HAP floats

  \param [in] f1 First argument
  \param [in] f2 Second argument

  \return
    HAP float structure which is the sum of both arguments. If arguments have different precision, returned float will
    have higher precision.
*/
json_float_t json_float_add(json_float_t f1, json_float_t f2);

/** Subtract two HAP floats

  \param [in] f1 First argument
  \param [in] f2 Second argument

  \return
    HAP float structure which is the difference of both arguments. If arguments have different precision, returned float
    will have higher precision.
*/
json_float_t json_float_sub(json_float_t f1, json_float_t f2);

/** Multiply HAP float by constant

  \param [in] f Base HAP float
  \param [in] constant Constant multiplier

  \return
    HAP float structure which is the product of both arguments
*/
json_float_t json_float_mul_const(json_float_t f, float constant);

/** @} */ // End of HOMEKIT_ACCESSORY_PROTOCOL

#endif /* _JSON_FLOAT_H_ */

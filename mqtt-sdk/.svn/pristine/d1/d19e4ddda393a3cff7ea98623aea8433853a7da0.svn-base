/*
 * Copyright (C) 2017 Tieto Poland Sp. z o. o.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <qcom_mem.h>

#include "json_float.h"

/*
 * Private auxiliary function
 */

static uint32_t calculate_multiplier(uint8_t precision)
{
  uint32_t ret = 1;
  uint8_t i;

  for (i = 0; i < precision; ++i)
  {
    ret *= 10;
  }

  return ret;
}

static int string_to_fraction(char *fract_str, uint8_t precision)
{
  uint32_t fract;
  char *buff;

  if (precision == 0)
  {
    return 0;
  }

  /* Limit size of string */
  buff = qcom_mem_calloc(precision + 1, sizeof(char));

  strncpy(buff, fract_str, precision);

  buff[precision] = '\0';

  if (precision > strlen(buff))
  {
    memset(buff + strlen(buff), '0', precision - strlen(buff));
  }

  fract = atoi(buff);

  qcom_mem_free(buff);

  return fract;
}

static void precision_align_up(json_float_t *f1, json_float_t *f2)
{
  if (f1->precision < f2->precision)
  {
    uint32_t multiplier = calculate_multiplier(f2->precision - f1->precision);

    f1->value *= multiplier;
    f1->precision = f2->precision;
  }
  else if (f1->precision > f2->precision)
  {
    uint32_t multiplier = calculate_multiplier(f1->precision - f2->precision);

    f2->value *= multiplier;
    f2->precision = f1->precision;
  }
}

static void insert_char_back(char *str, char c, uint32_t pos)
{
  int32_t i;

  /* Remember about terminator character */
  ++pos;

  for (i = strlen(str) + 1; pos != -1; --i, --pos)
  {
    str[i + 1] = str[i];

    if (pos == 0)
    {
      str[i] = c;
    }
  }
}

/*
 * Public API function
 */

json_float_t json_float(json_float_sign_t sign, uint32_t int_part, uint32_t fract_part, uint8_t precision)
{
  uint32_t multiplier;
  int32_t value;

  multiplier = calculate_multiplier(precision);

  value = multiplier * int_part;

  /* Fit fractional part to requested precision */
  while (multiplier <= fract_part)
  {
    fract_part /= 10;
  }

  /* Join intager and fractional to one variable */
  value += fract_part;

  if (sign == JSON_FLOAT_SIGN_MINUS)
  {
    value *= -1;
  }

  return (json_float_t){value, precision};
}

uint8_t json_float_get_precision_from_string(const char *str)
{
  char *decimal_point;
  uint8_t i;

  if (str == NULL)
  {
    return 0;
  }

  decimal_point = strchr(str, '.');
  if (decimal_point == NULL)
  {
    return 0;
  }

  for(i = 0; decimal_point[i + 1] != '\0'; ++i);

  return i;
}

json_float_t json_float_from_string(const char *str, uint8_t precision)
{
  uint32_t fract_part = 0;
  uint32_t int_part = 0;
  char *decimal_point;
  json_float_sign_t sign = JSON_FLOAT_SIGN_PLUS;

  if (str[0] == '-')
  {
    sign = JSON_FLOAT_SIGN_MINUS;
  }

  int_part = strtoul(str + sign, NULL, 10);

  decimal_point = strchr(str, '.');
  if (decimal_point != NULL)
  {
    /* Get character after decimal point */
    fract_part = string_to_fraction(decimal_point + 1, precision);
  }

  return json_float(sign, int_part, fract_part, precision);
}

void json_float_to_string(const json_float_t *f, char *str)
{
  json_float_sign_t sign = JSON_FLOAT_SIGN_PLUS;
  uint8_t dot_position = f->precision;

  sprintf(str, "%d", f->value);

  if (str[0] == '-')
  {
    sign = JSON_FLOAT_SIGN_MINUS;
  }

  /* Insert '0' after decimal point if fractional part is too small */
  while (strlen(str) - sign < dot_position)
  {
    insert_char_back(str, '0', strlen(str) - sign);
  }

  /* Insert dot between intager and fractional part. If precision is equal zero, fractional part does not exist */
  if (dot_position != 0)
  {
    insert_char_back(str, '.', dot_position);
  }

  /* Append '0' to begin of string if necessary */
  if (str[sign] == '.')
  {
    insert_char_back(str, '0', strlen(str) - sign);
  }
}

json_float_t json_float_add(json_float_t f1, json_float_t f2)
{
  precision_align_up(&f1, &f2);

  return (json_float_t){f1.value + f2.value, f1.precision};
}

json_float_t json_float_sub(json_float_t f1, json_float_t f2)
{
  precision_align_up(&f1, &f2);

  return (json_float_t){f1.value - f2.value, f1.precision};
}

json_float_t json_float_mul_const(json_float_t f, float constant)
{
  return (json_float_t){f.value * constant, f.precision};
}

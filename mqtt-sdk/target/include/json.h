/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 *
 */

/*
  Copyright (c) 2009 Dave Gamble

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#ifndef json__h
#define json__h

#include "basetypes.h" //Definitions for A_INT32, A_CHAR etc.
#include <qcom/stdint.h>
//#include <malloc_api.h>
#include "json_float.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* json Types: */
#define json_False 0
#define json_True 1
#define json_NULL 2
#define json_Number 3
#define json_String 4
#define json_Array 5
#define json_Object 6
#define json_Float 7

#define json_IsReference 256
#define json_StringIsConst 512

/* The json_t structure: */
typedef struct json_t {
	struct json_t *next,*prev;	/* next/prev allow you to walk array/object chains. Alternatively, use GetArraySize/GetArrayItem/GetObjectItem */
	struct json_t *child;		/* An array or object item will have a child pointer pointing to a chain of the items in the array/object. */

	A_INT32 type;			/* The type of the item, as above. */

	A_CHAR *valuestring;		/* The item's string, if type==json_String */
	A_INT32 valueint;		/* The item's number, if type==json_Number */
	A_CHAR *string;			/* The item's name string, if this item is the child of, or is in the list of subitems of an object. */
	json_float_t valuefloat;	/* The item's float, if type==json_Float */
} json_t,pjson_t;

typedef struct json_hooks {
      void *(*malloc_fn)(size_t sz);
      void (*free_fn)(void *ptr);
} json_hooks;

/* Supply malloc, realloc and free functions to json_t */
extern void json_init_hooks(json_hooks* hooks);


/* Supply a block of JSON, and this returns a json_t object you can interrogate. Call json_delete when finished. */
extern json_t *json_decode(const A_CHAR *value);

/* Render a json_t entity to text for transfer/storage. Free the A_CHAR* when finished. */
extern A_CHAR  *json_encode(json_t *item);

/* Render a json_t entity to text for transfer/storage without any formatting. Free the A_CHAR* when finished. */
extern A_CHAR  *json_PrintUnformatted(json_t *item);

/* Render a json_t entity to text using a buffered strategy. prebuffer is a guess at the final size. guessing well reduces reallocation. fmt=0 gives unformatted, =1 gives formatted */
extern A_CHAR *json_PrintBuffered(json_t *item,A_INT32 prebuffer,A_INT32 fmt);

/* Delete a json_t entity and all subentities. */
extern void   json_delete(json_t *c);

/* Returns the number of items in an array (or object). */
extern A_INT32	  json_get_array_size(json_t *array);

/* Retrieve item number "item" from array "array". Returns NULL if unsuccessful. */
extern json_t *json_get_array_item(json_t *array,A_INT32 item);

/* Get item "string" from object. Case insensitive. */
extern json_t *json_get_object_item(json_t *object,const A_CHAR *string);

/* For analysing failed parses. This returns a pointer to the parse error. You'll probably need to look a few chars back to make sense of it. Defined when json_decode() returns 0. 0 when json_decode() succeeds. */
extern const A_CHAR *json_get_error_ptr(void);

/* These calls create a json_t item of the appropriate type. */
extern json_t *json_create_null(void);

extern json_t *json_create_true(void);

extern json_t *json_create_false(void);

extern json_t *json_create_bool(A_INT32 b);

extern json_t *json_create_float(json_float_t *json_float);

extern json_t *json_create_number(A_INT32 num);

extern json_t *json_create_string(const A_CHAR *string);

extern json_t *json_create_array(void);

extern json_t *json_create_object(void);

/* These utilities create an Array of count items. */
extern json_t *json_create_int_array(const A_INT32 *numbers,A_INT32 count);

extern json_t *json_create_string_array(const A_CHAR **strings,A_INT32 count);

/* Append item to the specified array/object. */
extern void     json_add_item_toarray(json_t *array, json_t *item);

extern void	json_add_item_to_object(json_t *object,const A_CHAR *string,json_t *item);

extern void	json_add_item_to_objectCS(json_t *object,const A_CHAR *string,json_t *item);	/* Use this when string is definitely const (i.e. a literal, or as good as), and will definitely survive the json_t object */

/* Append reference to item to the specified array/object. Use this when you want to add an existing json_t to a new json_t, but don't want to corrupt your existing json_t. */
extern void     json_add_item_reference_toarray(json_t *array, json_t *item);
extern void	json_add_item_reference_toObject(json_t *object,const A_CHAR *string,json_t *item);

/* Remove/Detatch items from Arrays/Objects. */
extern json_t *json_detach_item_from_array(json_t *array,A_INT32 which);
extern json_t *json_detach_item_from_object(json_t *object,const A_CHAR *string);

extern void   json_delete_item_from_array(json_t *array,A_INT32 which);
extern void   json_delete_item_from_object(json_t *object,const A_CHAR *string);

/* Update array items. */
extern void json_insert_item_in_array(json_t *array,A_INT32 which,json_t *newitem);	/* Shifts pre-existing items to the right. */
extern void json_replace_item_in_array(json_t *array,A_INT32 which,json_t *newitem);
extern void json_replace_item_inObject(json_t *object,const A_CHAR *string,json_t *newitem);

/* Duplicate a json_t item */
extern json_t *json_duplicate(json_t *item,A_INT32 recurse);
/* Duplicate will create a new, identical json_t item to the one you pass, in new memory that will
need to be released. With recurse!=0, it will duplicate any children connected to the item.
The item->next and ->prev pointers are always zero on return from Duplicate. */

/* ParseWithOpts allows you to require (and check) that the JSON is null terminated, and to retrieve the pointer to the final byte parsed. */
extern json_t *json_parse_with_opts(const A_CHAR *value,const A_CHAR **return_parse_end,A_INT32 require_null_terminated);

extern void json_minify(A_CHAR *json);

/* Macros for creating things quickly. */
#define json_add_null_toObject(object,name)	json_add_item_to_object(object, name, json_create_null())
#define json_add_true_toObject(object,name)	json_add_item_to_object(object, name, json_create_true())
#define json_add_false_toObject(object,name)	json_add_item_to_object(object, name, json_create_false())
#define json_add_bool_toObject(object,name,b)	json_add_item_to_object(object, name, json_create_bool(b))
#define json_add_number_toObject(object,name,n)	json_add_item_to_object(object, name, json_create_number(n))
#define json_add_string_toObject(object,name,s)	json_add_item_to_object(object, name, json_create_string(s))

/* When assigning an integer value, it needs to be propagated to valuedouble too. */
#define json_set_int_value(object,val)		((object)?(object)->valueint=(val):(val))
#define json_set_number_value(object,val)	((object)?(object)->valueint=(val):(val))

#define tolower(ch) ('A' <= ch && ch <= 'Z' ? 'a' + (ch - 'A') : ch)

#ifdef __cplusplus
}
#endif

#endif

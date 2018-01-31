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

/* JSON parser in C. */
#include "qcom/basetypes.h"
#include <string.h>
#include "json.h"
#include "malloc_api.h" //malloc(), free()
#include "json_float.h"

#define json_malloc malloc
#define json_free   free

static const A_CHAR *ep;

const A_CHAR *json_get_error_ptr(void) {return ep;}

extern int sprintf(char* s, const char* fmt, ...);
extern void *memset(void *dest, int val, unsigned int nbyte);

static A_INT32 json_strcasecmp(const A_CHAR *s1,const A_CHAR *s2)
{
	if (!s1)
	{
		return (s1==s2)?0:1;
	}
	if (!s2)
	{
		return 1;
	}
	for(; tolower((A_UCHAR)*s1) == tolower((A_UCHAR)*s2); ++s1, ++s2)
	{
		if(*s1 == 0)
		{
			return 0;
		}
	}
	return tolower(*(const A_UCHAR *)s1) - tolower(*(const A_UCHAR *)s2);
}

static A_CHAR* json_strdup(const A_CHAR* str)
{
    size_t len;
    A_CHAR* copy;
    len = strlen(str) + 1;
    if (!(copy = (A_CHAR*)json_malloc(len)))
	{
		return 0;
	}
    memcpy(copy,str,len);
    return copy;
}

void json_init_hooks(json_hooks* hooks)
{
#if 0
    if (!hooks) { /* Reset hooks */
        json_malloc = malloc;
        json_free = free;
        return;
    }

	json_malloc = (hooks->malloc_fn)?hooks->malloc_fn:malloc;
	json_free	 = (hooks->free_fn)?hooks->free_fn:free;
#endif //#if 0
}

/* Internal constructor. */
static json_t *json_new_item(void)
{
	json_t* node = (json_t*)json_malloc(sizeof(json_t));
	if (node)
	{
		memset(node,0,sizeof(json_t));
	}
	return node;
}

/* Delete a json_t structure. */
void json_delete(json_t *c)
{
	json_t *next;
	while (c)
	{
		next=c->next;
		if (!(c->type&json_IsReference) && c->child)
		{
			json_delete(c->child);
		}
		if (!(c->type&json_IsReference) && c->valuestring)
		{
			json_free(c->valuestring);
		}
		if (!(c->type&json_StringIsConst) && c->string)
		{
			json_free(c->string);
		}
		json_free(c);
		c=next;
	}
}

/* Parse the input text to generate a number, and populate the result into item. */
static const A_CHAR *parse_number(json_t *item,const A_CHAR *num)
{
	A_INT32 n=0,sign=1;

	if (*num=='-')
	{
		sign=-1;
		num++;	/* Has sign? */
	}
	if (*num=='0')
	{
		num++;			/* is zero */
	}
	if (*num>='1' && *num<='9')
	{
		do
		{
			n=(n*10)+(*num++ -'0');
		}while (*num>='0' && *num<='9');	/* Number? */
	}
	if (*num=='.' && num[1]>='0' && num[1]<='5')
	{
		do
		{
			num++;
		}while (*num>='0' && *num<='9');
	}
	if (*num=='.' && num[1]>='6' && num[1]<='9')
	{
		n =n+1;
		do
		{
			num++;
		}while (*num>='0' && *num<='9');
	}

	n=sign*n;

	item->valueint=(A_INT32)n;
	item->type=json_Number;
	return num;
}

static const A_CHAR *parse_float(json_t *item, const A_CHAR *num)
{
	A_CHAR i=0;
	A_CHAR *new_num;

	new_num = json_malloc(strlen(num) * sizeof(char));
	if(!new_num)
	{
		return 0;
	}

	if (*num=='-')
	{
		*new_num = *num;
		num++;
		i=1;
	}

	while(*num!='\0')
	{
		if ((*num=='.') || (*num>='0' && *num<='9'))
		{
			*(new_num+i) = *num++;
		}
		else
		{
			*(new_num+i) = '\0';
			break;
		}
		i++;
	}

	uint8_t precision = json_float_get_precision_from_string(new_num);
	json_float_t json_float = json_float_from_string(new_num,precision);
	item->valuefloat = json_float;
	item->type = json_Float;

	json_free(new_num);

	return num;
}

static A_INT32 pow2gt (A_INT32 x)
{
	--x;
	x|=x>>1;
	x|=x>>2;
	x|=x>>4;
	x|=x>>8;
	x|=x>>16;
	return x+1;
}

typedef struct {
	A_CHAR *buffer;
	A_INT32 length;
	A_INT32 offset;
} printbuffer;

static A_CHAR* ensure(printbuffer *p,A_INT32 needed)
{
	A_CHAR *newbuffer;
	A_INT32 newsize;
	if (!p || !p->buffer)
	{
		return 0;
	}
	needed += p->offset;
	if (needed <= p->length)
		return p->buffer + p->offset;

	newsize = pow2gt(needed);
	newbuffer = (A_CHAR*)json_malloc(newsize);
	if (!newbuffer)
	{
		json_free(p->buffer);
		p->length=0;
		p->buffer=0;
		return 0;
	}
	if (newbuffer)
	{
		memcpy(newbuffer,p->buffer,p->length);
	}
	json_free(p->buffer);
	p->length=newsize;
	p->buffer=newbuffer;
	return newbuffer+p->offset;
}

static A_INT32 update(printbuffer *p)
{
	A_CHAR *str;
	if (!p || !p->buffer)
	{
		return 0;
	}
	str = p->buffer + p->offset;
	return p->offset+strlen(str);
}

/* Render the number nicely from the given item into a string. */
static A_CHAR *print_number(json_t *item,printbuffer *p)
{
	A_CHAR *str = 0;
	A_INT32 d = item->valueint;
	if (d == 0)
	{
		if (p)
		{
			str = ensure(p,2);
		}
		else
		{
			str = ( A_CHAR*)json_malloc(2);	/* special case for 0. */
		}
		if (str)
			strcpy(str,"0");
	}
	else // if (fabs(((double)item->valueint)-d)<=DBL_EPSILON && d<=INT_MAX && d>=INT_MIN)
	{
		if (p)
			str = ensure(p,21);
		else
			str = (A_CHAR*)json_malloc(21);	/* 2^64+1 can be represented in 21 chars. */
		if (str)
        {
            memset(str, 0, 21);
            sprintf(str,"%d",item->valueint);
        }
	}
	return str;
}

static A_CHAR *print_float(json_t *item, printbuffer *p)
{
	A_CHAR *str = 0;

	json_float_t f = item->valuefloat;

	if (f.value == 0)
	{
		if (p)
		{
			str = ensure(p, 2);
		}
		else
		{
			str = ( A_CHAR*)json_malloc(2);	/* special case for 0. */
		}
		if (str)
			strcpy(str, "0");
	}
	else
	{
		if (p)
			str = ensure(p, 21);
		else
			str = (A_CHAR*)json_malloc(21);	/* 2^64+1 can be represented in 21 chars. */
		if (str)
		{
			memset(str, 0, 21);
			json_float_to_string(&f, str);
		}
	}
	return str;
}

static unsigned parse_hex4(const A_CHAR *str)
{
	unsigned h=0;
	if (*str >= '0' && *str <= '9')
		h += (*str) - '0';
	else if (*str >= 'A' && *str <= 'F')
		h += 10+(*str)-'A';
	else if (*str >= 'a' && *str <= 'f')
		h += 10 + (*str) - 'a';
	else
		return 0;
	h = h<<4;
	str++;
	if (*str >= '0' && *str <= '9')
		h+=(*str)-'0';
	else if (*str>='A' && *str<='F')
		h += 10 + (*str) - 'A';
	else if (*str>='a' && *str<='f')
		h += 10 + (*str) -'a';
	else
		return 0;
	h = h<<4;
	str++;
	if (*str >= '0' && *str <= '9')
		h += (*str) - '0';
	else if (*str >= 'A' && *str <= 'F')
		h += 10 + (*str) - 'A';
	else if (*str >= 'a' && *str <= 'f')
		h += 10 + (*str) - 'a';
	else
		return 0;
	h = h<<4;
	str++;
	if (*str >= '0' && *str <= '9')
		h += (*str) - '0';
	else if (*str >= 'A' && *str <= 'F')
		h+=10+(*str)-'A';
	else if (*str>='a' && *str<='f')
		h+=10+(*str)-'a';
	else
	return 0;
return h;
}

/* Parse the input text into an unescaped cstring, and populate item. */
static const A_UCHAR firstByteMark[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

static const A_CHAR *parse_string(json_t *item,const A_CHAR *str)
{
	const A_CHAR *ptr = NULL;
	A_CHAR *ptr2;
	A_CHAR *out;
	A_INT32 len=0;
	unsigned uc,uc2;

	if(str == NULL)
	{
		//Invalid input
		return NULL;
	}

	ptr = str + 1;

	if (*str != '\"')
	{
		ep=str;
		return 0;
	}	/* not a string! */

	while (*ptr != '\"' && *ptr && ++len)
	{
		if (*ptr++ == '\\')
			ptr++;	/* Skip escaped quotes. */
	}

	out = (A_CHAR*)json_malloc(len + 1);	/* This is how long we need for the string, roughly. */
	if (!out)
		return 0;

	ptr = str+1;
	ptr2 = out;
	while (*ptr != '\"' && *ptr)
	{
		if (*ptr!='\\')
			*ptr2++ = *ptr++;
		else
		{
			ptr++;
			switch (*ptr)
			{
				case 'b':
					*ptr2++ ='\b';
					break;
				case 'f':
					*ptr2++ = '\f';
					break;
				case 'n':
					*ptr2++ = '\n';
					break;
				case 'r':
					*ptr2++ = '\r';
					break;
				case 't':
					*ptr2++ = '\t';
					break;
				case 'u':	 /* transcode utf16 to utf8. */
					uc = parse_hex4(ptr+1);
					ptr+=4;	/* get the unicode A_CHAR. */

					if ((uc >= 0xDC00 && uc <= 0xDFFF) || uc == 0)
						break;	/* check for invalid.	*/

					if (uc >= 0xD800 && uc <= 0xDBFF)	/* UTF16 surrogate pairs.	*/
					{
						if (ptr[1] != '\\' || ptr[2] != 'u')
							break;	/* missing second-half of surrogate.	*/
						uc2 = parse_hex4(ptr+3);
						ptr += 6 ;
						if (uc2 < 0xDC00 || uc2 > 0xDFFF)
							break;	/* invalid second-half of surrogate.	*/
						uc = 0x10000 + (((uc&0x3FF)<<10) | (uc2&0x3FF));
					}

					len = 4;
					if (uc<0x80)
						len = 1;
					else if (uc<0x800)
						len = 2;
					else if (uc<0x10000)
						len=3;
					ptr2 += len;

					switch (len) {
						case 4:
							*--ptr2 = ((uc | 0x80) & 0xBF);
							uc >>= 6;
						case 3:
							*--ptr2 = ((uc | 0x80) & 0xBF);
							uc >>= 6;
						case 2:
							*--ptr2 = ((uc | 0x80) & 0xBF);
							uc >>= 6;
						case 1:
							*--ptr2 = (uc | firstByteMark[len]);
					}
					ptr2 += len;
					break;
				default:
					*ptr2++ = *ptr;
					break;
			}
			ptr++;
		}
	}
	*ptr2 = 0;
	if (*ptr == '\"')
		ptr++;
	item->valuestring = out;
	item->type = json_String;
	return ptr;
}

/* Render the cstring provided to an escaped version that can be printed. */
static A_CHAR *print_string_ptr(const A_CHAR *str,printbuffer *p)
{
	const A_CHAR *ptr;
	A_CHAR *ptr2,*out;
	A_INT32 len=0,flag=0;
	A_UCHAR token;

	for (ptr=str;*ptr;ptr++)
		flag |= ((*ptr>0 && *ptr<32)||(*ptr=='\"')||(*ptr=='\\'))?1:0;
	if (!flag)
	{
		len = ptr-str;
		if (p)
			out = ensure(p,len+3);
		else
			out = (A_CHAR*)json_malloc(len+3);
		if (!out)
			return 0;
		ptr2 = out;
		*ptr2++ = '\"';
		strcpy(ptr2,str);
		ptr2[len] = '\"';
		ptr2[len+1] =  0;
		return out;
	}

	if (!str)
	{
		if (p)
			out = ensure(p,3);
		else
			out = (A_CHAR*)json_malloc(3);
		if (!out)
			return 0;
		strcpy(out,"\"\"");
		return out;
	}
	ptr = str;
	while ((token=*ptr) && ++len)
	{
		if (strchr("\"\\\b\f\n\r\t",token))
			len++;
		else if (token<32)
			len+=5;
		ptr++;
	}

	if (p)
		out = ensure(p,len+3);
	else
		out = (A_CHAR*)json_malloc(len+3);
	if (!out)
		return 0;

	ptr2 = out;
	ptr=str;
	*ptr2++ = '\"';
	while (*ptr)
	{
		if ((A_UCHAR)*ptr > 31 && *ptr != '\"' && *ptr != '\\')
			*ptr2++ = *ptr++;
		else
		{
			*ptr2++='\\';
			switch (token = *ptr++)
			{
				case '\\':
					*ptr2++ = '\\';
					break;
				case '\"':
					*ptr2++ = '\"';
					break;
				case '\b':
					*ptr2++ = 'b';
					break;
				case '\f':
					*ptr2++ = 'f';
					break;
				case '\n':
					*ptr2++ = 'n';
					break;
				case '\r':
					*ptr2++ = 'r';
					break;
				case '\t':
					*ptr2++ = 't';
					break;
				default:
					sprintf(ptr2,"u%04x",token);
					ptr2 += 5;
					break;	/* escape and print */
			}
		}
	}
	*ptr2++ = '\"';
	*ptr2++ = 0;
	return out;
}
/* Invote print_string_ptr (which is useful) on an item. */
static A_CHAR *print_string(json_t *item,printbuffer *p)
{
	return print_string_ptr(item->valuestring,p);
}

/* Predeclare these prototypes. */
static const A_CHAR *parse_value(json_t *item,const A_CHAR *value);
static A_CHAR *print_value(json_t *item,A_INT32 depth,A_INT32 fmt,printbuffer *p);
static const A_CHAR *parse_array(json_t *item,const A_CHAR *value);
static A_CHAR *print_array(json_t *item,A_INT32 depth,A_INT32 fmt,printbuffer *p);
static const A_CHAR *parse_object(json_t *item,const A_CHAR *value);
static A_CHAR *print_object(json_t *item,A_INT32 depth,A_INT32 fmt,printbuffer *p);

/* Utility to jump whitespace and cr/lf */
static const A_CHAR *skip(const A_CHAR *in)
{
	while (in && *in && (A_UCHAR)*in <= 32)
		in++;
	return in;
}

/* Parse an object - create a new root, and populate. */
json_t *json_parse_with_opts(const A_CHAR *value,const A_CHAR **return_parse_end,A_INT32 require_null_terminated)
{
	const A_CHAR *end = 0;
	json_t *c = json_new_item();
	ep = 0;
	if (!c)
		return 0;       /* memory fail */

	end = parse_value(c,skip(value));
	if (!end)
	{
		json_delete(c);
		return 0;
	}	/* parse failure. ep is set. */

	/* if we require null-terminated JSON without appended garbage, skip and then check for a null terminator */
	if (require_null_terminated)
	{
		end = skip(end);
		if (*end)
		{
			json_delete(c);
			ep = end;
			return 0;
		}
	}
	if (return_parse_end)
		*return_parse_end = end;
	return c;
}
/* Default options for json_t_Parse */
json_t *json_decode(const A_CHAR *value)
{
	return json_parse_with_opts(value,0,0);
}

/* Render a json_t item/entity/structure to text. */
A_CHAR *json_encode(json_t *item)
{
	return print_value(item,0,1,0);
}
A_CHAR *json_PrintUnformatted(json_t *item)
{
	return print_value(item,0,0,0);
}

A_CHAR *json_PrintBuffered(json_t *item,A_INT32 prebuffer,A_INT32 fmt)
{
	printbuffer p;
	p.buffer = (A_CHAR*)json_malloc(prebuffer);
	p.length = prebuffer;
	p.offset = 0;
	return print_value(item,0,fmt,&p);
	return p.buffer;
}


static A_BOOL value_is_float(const A_CHAR *value)
{
	if(*value == '-')
	{
		value++;
	}

	while (*value>='0' && *value <='9')
	{
		value++;
	}

	if (*value =='.')
	{
		return 1;
	}

	return 0;
}

/* Parser core - when encountering text, process appropriately. */
static const A_CHAR *parse_value(json_t *item,const A_CHAR *value)
{
	if (!value)
		return 0;	/* Fail on null. */
	if (!strncmp(value,"null",4))
	{
		item->type = json_NULL;
		return value+4;
	}
	if (!strncmp(value,"false",5))
	{
		item->type = json_False;
		return value+5;
	}
	if (!strncmp(value,"true",4))
	{
		item->type = json_True;
		item->valueint = 1;
		return value+4;
	}
	if (*value == '\"')
	{
		return parse_string(item,value);
	}
	if (*value == '-' || (*value >= '0' && *value <= '9'))
	{
		if (value_is_float(value))
		{
			return parse_float(item,value);
		}
		else
		{
			return parse_number(item,value);
		}
	}
	if (*value == '[')
	{
		return parse_array(item,value);
	}
	if (*value=='{')
	{
		return parse_object(item,value);
	}

	ep=value;return 0;	/* failure. */
}

/* Render a value to text. */
static A_CHAR *print_value(json_t *item,A_INT32 depth,A_INT32 fmt,printbuffer *p)
{
	A_CHAR *out=0;
	if (!item)
		return 0;
	if (p)
	{
		switch ((item->type)&255)
		{
			case json_NULL:
				{
					out = ensure(p,5);
					if (out)
						strcpy(out,"null");
					break;
				}
			case json_False:
				{
					out = ensure(p,6);
					if (out)
						strcpy(out,"false");
					break;
				}
			case json_True:
				{
					out = ensure(p,5);
					if (out)
						strcpy(out,"true");
					break;
				}
			case json_Number:
				out = print_number(item,p);
				break;
			case json_Float:
				out = print_float(item,p);
				break;
			case json_String:
				out = print_string(item,p);
				break;
			case json_Array:
				out = print_array(item,depth,fmt,p);
				break;
			case json_Object:
				out = print_object(item,depth,fmt,p);
				break;
		}
	}
	else
	{
		switch ((item->type)&255)
		{
			case json_NULL:
				out = json_strdup("null");
				break;
			case json_False:
				out = json_strdup("false");
				break;
			case json_True:
				out = json_strdup("true");
				break;
			case json_Number:
				out = print_number(item,0);
				break;
			case json_Float:
				out = print_float(item,p);
				break;
			case json_String:
				out = print_string(item,0);
				break;
			case json_Array:
				out = print_array(item,depth,fmt,0);
				break;
			case json_Object:
				out = print_object(item,depth,fmt,0);
				break;
		}
	}
	return out;
}

/* Build an array from input text. */
static const A_CHAR *parse_array(json_t *item,const A_CHAR *value)
{
	json_t *child;
	if (*value!='[')
	{
		ep = value;
		return 0;
	}	/* not an array! */

	item->type = json_Array;
	value = skip(value+1);
	if (*value == ']')
		return value+1;	/* empty array. */

	item->child =  child = json_new_item();
	if (!item->child)
		return 0;		 /* memory fail */
	value = skip(parse_value(child,skip(value)));	/* skip any spacing, get the value. */
	if (!value)
		return 0;

	while (*value == ',')
	{
		json_t *new_item;
		if (!(new_item = json_new_item()))
			return 0; 	/* memory fail */
		child->next = new_item;
		new_item->prev = child;
		child = new_item;
		value = skip(parse_value(child,skip(value+1)));
		if (!value)
			return 0;	/* memory fail */
	}

	if (*value == ']')
		return value+1;	/* end of array */
	ep = value;
	return 0;	/* malformed. */
}

/* Render an array to text */
static A_CHAR *print_array(json_t *item,A_INT32 depth,A_INT32 fmt,printbuffer *p)
{
	A_CHAR **entries;
	A_CHAR *out = 0,*ptr,*ret;
	A_INT32 len = 5;
	json_t *child = item->child;
	A_INT32 numentries = 0,i = 0,fail = 0;
	size_t tmplen=0;

	/* How many entries in the array? */
	while (child)
	{
		numentries++;
		child=child->next;
	}
	/* Explicitly handle numentries==0 */
	if (!numentries)
	{
		if (p)
			out = ensure(p,3);
		else
			out = (A_CHAR*)json_malloc(3);
		if (out)
			strcpy(out,"[]");
		return out;
	}

	if (p)
	{
		/* Compose the output array. */
		i = p->offset;
		ptr = ensure(p,1);
		if (!ptr)
			return 0;
		*ptr = '[';
		p->offset++;
		child = item->child;
		while (child && !fail)
		{
			print_value(child,depth+1,fmt,p);
			p->offset = update(p);
			if (child->next)
			{
				len = fmt?2:1;
				ptr = ensure(p,len+1);
				if (!ptr)
					return 0;
				*ptr++=',';
				if(fmt)
					*ptr++=' ';
				*ptr=0;
				p->offset += len;
			}
			child = child->next;
		}
		ptr = ensure(p,2);
		if (!ptr)
			return 0;
		*ptr++=']';
		*ptr=0;
		out = (p->buffer) + i;
	}
	else
	{
		/* Allocate an array to hold the values for each */
		entries = (A_CHAR**)json_malloc(numentries*sizeof(A_CHAR*));
		if (!entries)
			return 0;
		memset(entries,0,numentries*sizeof(A_CHAR*));
		/* Retrieve all the results: */
		child = item->child;
		while (child && !fail)
		{
			ret = print_value(child,depth+1,fmt,0);
			entries[i++] = ret;
			if (ret)
				len += strlen(ret)+2+(fmt?1:0);
			else
				fail=1;
			child = child->next;
		}

		/* If we didn't fail, try to malloc the output string */
		if (!fail)
			out = (A_CHAR*)json_malloc(len);
		/* If that fails, we fail. */
		if (!out)
			fail = 1;

		/* Handle failure. */
		if (fail)
		{
			for (i = 0; i < numentries; i++)
			{
				if (entries[i])
					json_free(entries[i]);
			}
			json_free(entries);
			return 0;
		}

		/* Compose the output array. */
		*out = '[';
		ptr = out+1;
		*ptr = 0;
		for (i = 0; i < numentries; i++)
		{
			tmplen = strlen(entries[i]);
			memcpy(ptr,entries[i],tmplen);
			ptr += tmplen;
			if (i!=numentries-1)
			{
				*ptr++=',';
				if(fmt)
					*ptr++=' ';
				*ptr=0;
			}
			json_free(entries[i]);
		}
		json_free(entries);
		*ptr++ = ']';
		*ptr++ = 0;
	}
	return out;
}

/* Build an object from the text. */
static const A_CHAR *parse_object(json_t *item,const A_CHAR *value)
{
	json_t *child;
	if (*value!='{')
	{
		ep = value;
		return 0;
	}	/* not an object! */

	item->type = json_Object;
	value = skip(value+1);
	if (*value == '}')
		return value+1;	/* empty array. */

	item->child = child = json_new_item();
	if (!item->child)
		return 0;
	value = skip(parse_string(child,skip(value)));
	if (!value)
		return 0;
	child->string = child->valuestring;
	child->valuestring = 0;
	if (*value!=':')
	{
		ep=value;
		return 0;
	}	/* fail! */
	value = skip(parse_value(child,skip(value+1)));	/* skip any spacing, get the value. */
	if (!value)
		return 0;

	while (*value == ',')
	{
		json_t *new_item;
		if (!(new_item = json_new_item()))
			return 0; /* memory fail */
		child->next = new_item;
		new_item->prev = child;
		child = new_item;
		value = skip(parse_string(child,skip(value+1)));
		if (!value)
			return 0;
		child->string = child->valuestring;
		child->valuestring = 0;
		if (*value!=':')
		{
			ep=value;
			return 0;
		}	/* fail! */
		value = skip(parse_value(child,skip(value+1)));	/* skip any spacing, get the value. */
		if (!value)
			return 0;
	}

	if (*value == '}')
		return value+1;	/* end of array */
	ep = value;
	return 0;	/* malformed. */
}

/* Render an object to text. */
static A_CHAR *print_object(json_t *item,A_INT32 depth,A_INT32 fmt,printbuffer *p)
{
	A_CHAR **entries = 0,**names = 0;
	A_CHAR *out = 0,*ptr,*ret,*str;
	A_INT32 len=7,i = 0,j;
	json_t *child = item->child;
	A_INT32 numentries = 0,fail = 0;
	size_t tmplen = 0;
	/* Count the number of entries. */
	while (child)
	{
		numentries++;
		child=child->next;
	}
	/* Explicitly handle empty object case */
	if (!numentries)
	{
		if (p)
			out = ensure(p,fmt?depth+4:3);
		else
			out = (A_CHAR*)json_malloc(fmt?depth+4:3);
		if (!out)
			return 0;
		ptr = out;
		*ptr++ = '{';
		if (fmt)
		{
			*ptr++='\n';
			for (i=0;i<depth-1;i++)
				*ptr++='\t';
		}
		*ptr++ = '}';
		*ptr++ = 0;
		return out;
	}
	if (p)
	{
		/* Compose the output: */
		i = p->offset;
		len = fmt?2:1;
		ptr = ensure(p,len+1);
		if (!ptr)
			return 0;
		*ptr++ = '{';
		if (fmt)
			*ptr++='\n';
		*ptr=0;
		p->offset += len;
		child = item->child;
		depth++;
		while (child)
		{
			if (fmt)
			{
				ptr = ensure(p,depth);
				if (!ptr)
					return 0;
				for (j=0;j<depth;j++)
					*ptr++ = '\t';
				p->offset += depth;
			}
			print_string_ptr(child->string,p);
			p->offset = update(p);

			len = fmt?2:1;
			ptr = ensure(p,len);
			if (!ptr)
				return 0;
			*ptr++ = ':';
			if (fmt)
				*ptr++ = '\t';
			p->offset += len;

			print_value(child,depth,fmt,p);
			p->offset = update(p);

			len = (fmt?1:0)+(child->next?1:0);
			ptr = ensure(p,len+1);
			if (!ptr)
				return 0;
			if (child->next)
				*ptr++=',';
			if (fmt)
				*ptr++ = '\n';
			*ptr=0;
			p->offset += len;
			child = child->next;
		}
		ptr = ensure(p,fmt?(depth+1):2);
		if (!ptr)
			return 0;
		if (fmt)
		{
			for (i=0;i<depth-1;i++)
				*ptr++='\t';
		}
		*ptr++ = '}';
		*ptr=0;
		out = (p->buffer)+i;
	}
	else
	{
		/* Allocate space for the names and the objects */
		entries = (A_CHAR**)json_malloc(numentries*sizeof(A_CHAR*));
		if (!entries)
			return 0;
		names = (A_CHAR**)json_malloc(numentries*sizeof(A_CHAR*));
		if (!names)
		{
			json_free(entries);
			return 0;
		}
		memset(entries,0,sizeof(A_CHAR*)*numentries);
		memset(names,0,sizeof(A_CHAR*)*numentries);

		/* Collect all the results into our arrays: */
		child = item->child;
		depth++;
		if (fmt)
			len+=depth;
		while (child)
		{
			names[i] = str = print_string_ptr(child->string,0);
			entries[i++] = ret = print_value(child,depth,fmt,0);
			if (str && ret)
				len += strlen(ret) + strlen(str) + 2 + (fmt?2 + depth:0);
			else
			fail = 1;
			child = child->next;
		}

		/* Try to allocate the output string */
		if (!fail)
			out = (A_CHAR*)json_malloc(len);
		if (!out)
			fail=1;

		/* Handle failure */
		if (fail)
		{
			for (i = 0;i < numentries; i++)
			{
				if (names[i])
					json_free(names[i]);
				if (entries[i])
					json_free(entries[i]);
			}
			json_free(names);
			json_free(entries);
			return 0;
		}

		/* Compose the output: */
		*out = '{';
		ptr = out + 1;
		if (fmt)
			*ptr++ = '\n';
		*ptr=0;
		for (i = 0;i < numentries; i++)
		{
			if (fmt)
			{
				for (j=0;j<depth;j++)
					*ptr++='\t';
			}
			tmplen = strlen(names[i]);
			memcpy(ptr,names[i],tmplen);
			ptr += tmplen;
			*ptr++ = ':';
			if (fmt)
				*ptr++='\t';
			strcpy(ptr,entries[i]);
			ptr += strlen(entries[i]);
			if (i != numentries-1)
				*ptr++ = ',';
			if (fmt)
				*ptr++ = '\n';
			*ptr=0;
			json_free(names[i]);
			json_free(entries[i]);
		}

		json_free(names);
		json_free(entries);
		if (fmt)
		{
			for (i = 0;i < depth-1; i++)
				*ptr++ = '\t';
		}
		*ptr++ = '}';
		*ptr++ = 0;
	}
	return out;
}

/* Get Array size/item / object item. */
A_INT32    json_get_array_size(json_t *array)
{
	json_t *c= array->child;
	A_INT32 i = 0;
	while(c)
	{
		i++;
		c = c->next;
	}
	return i;
}
json_t *json_get_array_item(json_t *array,A_INT32 item)
{
	json_t *c=array->child;
	while (c && item>0)
	{
		item--;
		c = c->next;
	}
	return c;
}
json_t *json_get_object_item(json_t *object,const A_CHAR *string)
{
	json_t *c = object->child;
	while (c && json_strcasecmp(c->string,string))
		c=c->next;
	return c;
}

/* Utility for array list handling. */
static void suffix_object(json_t *prev,json_t *item)
{
	prev->next = item;
	item->prev = prev;
}
/* Utility for handling references. */
static json_t *create_reference(json_t *item)
{
	json_t *ref = json_new_item();
	if (!ref)
		return 0;
	memcpy(ref,item,sizeof(json_t));
	ref->string = 0;
	ref->type |= json_IsReference;
	ref->next = ref->prev =0;
	return ref;
}

/* Add item to array/object. */
void   json_add_item_toarray(json_t *array, json_t *item)
{
	json_t *c = array->child;
	if (!item)
		return;
	if (!c)
	{
		array->child = item;
	}
	else
	{
		while (c && c->next)
			c = c->next;
		suffix_object(c,item);
	}
}
void   json_add_item_to_object(json_t *object,const A_CHAR *string,json_t *item)
{
	if (!item)
		return;
	if (item->string)
		json_free(item->string);
	item->string = json_strdup(string);
	json_add_item_toarray(object,item);
}
void   json_add_item_to_objectCS(json_t *object,const A_CHAR *string,json_t *item)
{
	if (!item)
		return;
	if (!(item->type&json_StringIsConst) && item->string)
		json_free(item->string);
	item->string = (A_CHAR*)string;
	item->type |= json_StringIsConst;
	json_add_item_toarray(object,item);
}
void	json_add_item_reference_toarray(json_t *array, json_t *item)
{
	json_add_item_toarray(array,create_reference(item));
}
void	json_add_item_reference_toObject(json_t *object,const A_CHAR *string,json_t *item)
{
	json_add_item_to_object(object,string,create_reference(item));
}
json_t *json_detach_item_from_array(json_t *array,A_INT32 which)
{
	json_t *c = array->child;
	while (c && which > 0)
	{
		c = c->next;
		which--;
	}
	if (!c)
		return 0;
	if (c->prev)
		c->prev->next = c->next;
	if (c->next)
		c->next->prev = c->prev;
	if (c == array->child)
		array->child = c->next;
	c->prev=c->next=0;
	return c;
}
void   json_delete_item_from_array(json_t *array,A_INT32 which)
{
	json_delete(json_detach_item_from_array(array,which));
}
json_t *json_detach_item_from_object(json_t *object,const A_CHAR *string)
{
	A_INT32 i = 0;
	json_t *c = object->child;
	while (c && json_strcasecmp(c->string,string))
	{
		i++;
		c = c->next;
	}
	if (c)
		return json_detach_item_from_array(object,i);
	return 0;
}
void   json_delete_item_from_object(json_t *object,const A_CHAR *string)
{
	json_delete(json_detach_item_from_object(object,string));
}

/* Replace array/object items with new ones. */
void   json_insert_item_in_array(json_t *array,A_INT32 which,json_t *newitem)
{
	json_t *c = array->child;
	while (c && which>0)
	{
		c = c->next;
		which--;
	}
	if (!c)
	{
		json_add_item_toarray(array,newitem);
		return;
	}
	newitem->next = c;
	newitem->prev = c->prev;
	c->prev = newitem;
	if (c==array->child)
		array->child = newitem;
	else
		newitem->prev->next=newitem;
}
void   json_replace_item_in_array(json_t *array,A_INT32 which,json_t *newitem)
{
	json_t *c = array->child;
	while (c && which>0)
	{
		c = c->next;
		which--;
	}
	if (!c)
		return;
	newitem->next = c->next;
	newitem->prev = c->prev;
	if (newitem->next)
		newitem->next->prev = newitem;
	if (c == array->child)
		array->child = newitem;
	else
		newitem->prev->next = newitem;
	c->next=c->prev=0;
	json_delete(c);
}
void   json_replace_item_inObject(json_t *object,const A_CHAR *string,json_t *newitem)
{
	A_INT32 i = 0;
	json_t *c = object->child;
	while(c && json_strcasecmp(c->string,string))
	{
		i++;
		c=c->next;
	}
	if(c)
	{
		newitem->string = json_strdup(string);
		json_replace_item_in_array(object,i,newitem);
	}
}

/* Create basic types: */
json_t *json_create_null(void)
{
	json_t *item=json_new_item();
	if(item)
		item->type = json_NULL;
	return item;
}
json_t *json_create_true(void)
{
	json_t *item = json_new_item();
	if(item)item->type = json_True;
	return item;
}
json_t *json_create_false(void)
{
	json_t *item = json_new_item();
	if(item)
		item->type = json_False;
	return item;
}
json_t *json_create_bool(A_INT32 b)
{
	json_t *item = json_new_item();
	if(item)
		item->type = b?json_True:json_False;
	return item;
}
json_t *json_create_number(A_INT32 num)
{
	json_t *item=json_new_item();
	if(item)
	{
		item->type = json_Number;
		item->valueint = (A_INT32)num;
	}
	return item;
}

json_t *json_create_float(json_float_t *json_valuefloat)
{
	json_t *item=json_new_item();
	if(item)
	{
		item->type = json_Float;
		item->valuefloat = *json_valuefloat;
	}
	return item;
}

json_t *json_create_string(const A_CHAR *string)
{
	json_t *item=json_new_item();
	if(item)
	{
		item->type = json_String;
		item->valuestring=json_strdup(string);
	}
	return item;
}
json_t *json_create_array(void)
{
	json_t *item = json_new_item();
	if(item)
		item->type = json_Array;
	return item;
}
json_t *json_create_object(void)
{
	json_t *item = json_new_item();
	if(item)
		item->type = json_Object;
	return item;
}

/* Create Arrays: */
json_t *json_create_int_array(const A_INT32 *numbers,A_INT32 count)	
{
	A_INT32 i;
	json_t *n = 0,*p = 0,*a = json_create_array();
	for(i = 0;a && i<count;i++)
	{
		n = json_create_number(numbers[i]);
		if(!i)
			a->child = n;
		else
			suffix_object(p,n);
		p=n;
	}
	return a;
}
json_t *json_create_string_array(const A_CHAR **strings,A_INT32 count)	
{
	A_INT32 i;
	json_t *n = 0,*p = 0,*a = json_create_array();
	for(i = 0;a && i < count; i++) 
	{
		n = json_create_string(strings[i]);
		if(!i)
			a->child=n;
		else
			suffix_object(p,n);
		p=n;
	}
	return a;
}

/* Duplication */
json_t *json_duplicate(json_t *item,A_INT32 recurse)
{
	json_t *newitem,*cptr,*nptr=0,*newchild;
	/* Bail on bad ptr */
	if (!item) 
		return 0;
	/* Create new item */
	newitem = json_new_item();
	if (!newitem)
		return 0;
	/* Copy over all vars */
	newitem->type = item->type&(~json_IsReference);
	newitem->valueint = item->valueint;
	if (item->valuestring)
	{
		newitem->valuestring = json_strdup(item->valuestring);	
		if (!newitem->valuestring)
		{
			json_delete(newitem);
			return 0;
		}
	}
	if (item->string)		
	{
		newitem->string=json_strdup(item->string);	
		if (!newitem->string)	
		{
			json_delete(newitem);
			return 0;
		}
	}
	/* If non-recursive, then we're done! */
	if (!recurse) 
		return newitem;
	/* Walk the ->next chain for the child. */
	cptr=item->child;
	while (cptr)
	{
		newchild = json_duplicate(cptr,1);		/* Duplicate (with recurse) each item in the ->next chain */
		if (!newchild)
		{
			json_delete(newitem);
			return 0;
		}
		if (nptr)
		{
			nptr->next =  newchild;
			newchild->prev=nptr;
			nptr=newchild;
		}	/* If newitem->child already set, then crosswire ->prev and ->next and move on */
		else
		{
			newitem->child=newchild;
			nptr=newchild;
		}					/* Set newitem->child and move to it */
		cptr = cptr->next;
	}
	return newitem;
}

void json_minify(A_CHAR *json)
{
	A_CHAR *into = json;
	while (*json)
	{
		if (*json == ' ') 
			json++;
		else if (*json == '\t') 
			json++;	/* Whitespace characters. */
		else if (*json == '\r') 
			json++;
		else if (*json == '\n') 
			json++;
		else if (*json == '/' && json[1] == '/')  
			while (*json && *json!='\n') json++;	/* double-slash comments, to end of line. */
		else if (*json == '/' && json[1 ]== '*')
		{
			while (*json && !(*json == '*' && json[1] == '/')) 
				json++;
			json+=2;
		}	/* multiline comments. */
		else if (*json=='\"')
		{
			*into++ = *json++;
			while (*json && *json!='\"')
			{
				if (*json=='\\') 
					*into++=*json++;
				*into++=*json++;
			}
			*into++=*json++;
		} /* string literals, which are \" sensitive. */
		else 
			*into++=*json++;			/* All other characters. */
	}
	*into=0;	/* and null-terminate. */
}


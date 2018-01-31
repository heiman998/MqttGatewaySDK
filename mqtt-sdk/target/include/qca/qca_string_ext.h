
#ifndef _STRING_EXT_H_
#define _STRING_EXT_H_




/*
***************************************************************************
*@brief		:	Sprintf
*@param[in]	:	
*@return	:	
*@warning	:	see sprintf for detail
*@see		:
***************************************************************************
*/
#define sprintf_ext			sprintf

/*
***************************************************************************
*@brief		:	Copy a string to another string buffer
*@param[in]	:	strDest: dest string
*				strSrc: source string
*@return	:	strDest
*@warning	:	strDest buffer size => strSrc string length
*@see		:
***************************************************************************
*/
char * strcpy_ext(char *strDest, const char *strSrc);

/*
***************************************************************************
*@brief		:	Search the character in the string(search start from head)
*@param[in]	:	str: string that search from
*				c: character that want to search
*@return	:	NULL: failed(or not found)
*          		other: the location of the character in the string
*@warning	:
*@see		:
***************************************************************************
*/
char * strchr_ext(char * str, char c);

/*
***************************************************************************
*@brief		:	Search the character in the string(search start from tail)
*@param[in]	:	str: string that search from
*				c: character that want to search
*@return	:	NULL: failed(or not found)
*          		other: the location of the character in the string
*@warning	:
*@see		:
***************************************************************************
*/
char * strrchr_ext(char * str, char c);

/*
***************************************************************************
*@brief		:	Append the second string to the first string's tail
*@param[in]	:	strDest: the first string
*				strSrc: the second string
*@return	:	NULL: failed
*          		other: strDest
*@warning	:	strDest buffer size >= strDest string length + strSrc string length
*@see		:
***************************************************************************
*/
char * strcat_ext(char *strDest, const char *strSrc);

/*
***************************************************************************
*@brief		:	Search the sub string in another string
*@param[in]	:	s1: string that search from
*				s2: the sub string
*@return	:	NULL: failed(or not found)
*          		other: the location of the sub string in the string
*@warning	:
*@see		:
***************************************************************************
*/
char * strstr_ext(const char *s1, const char *s2);

/*
***************************************************************************
*@brief		:	Compare two string
*@param[in]	:	str1: the first string
*				str2: the second string
*@return	:	0: str1 == str2
*          		-1: str1 != str2
*@warning	:
*@see		:
***************************************************************************
*/
int strcmp_ext(const char *str1,const char *str2);

/*
***************************************************************************
*@brief		:	Compare two string's n characters
*@param[in]	:	str1: the first string
*				str2: the second string
*@return	:	0: the first n character of str1 == the first n character of str2
*          		-1: the first n character of str1 != the first n character of str2
*@warning	:
*@see		:
***************************************************************************
*/
int strncmp_ext(const char *str1,const char *str2, int count);

/*
***************************************************************************
*@brief		:	sscanf
*@param[in]	:	pChar
*				ret
*@return	:	none
*@warning	:
*@see		:
***************************************************************************
*/
void sscanf_ext(unsigned char *pChar, unsigned char *ret);

/*
***************************************************************************
*@brief		:	Convert a string's all character to lower
*@param[in]	:	s: the string that want to convert
*@return	:	s
*@warning	:
*@see		:
***************************************************************************
*/
char *strtolower_ext(char *s);

/*
***************************************************************************
*@brief		:	Convert the first section of the string to a decimalism
*@param[in]	:	str: string that want to convert
*@return	:	convert result
*@warning	:
*@see		:
***************************************************************************
*/
int atoi_ext(char *str);


char* strtok_ext(char * string, const char * control);


#endif







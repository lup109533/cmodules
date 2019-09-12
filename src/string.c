#include <stdlib.h>
#include <wctype.h>

struct __cmodules_string_library string = {
	.new      = &__cmodules_string_new,
	.del      = &__cmodules_string_del,
	.set      = &__cmodules_string_set,
	.copy     = &__cmodules_string_copy,
	.slice    = &__cmodules_string_slice,
	.alloc    = &__cmodules_string_alloc,
	.resize   = &__cmodules_string_resize,
	.to_lower = &__cmodules_string_to_lower,
	.to_upper = &__cmodules_string_to_upper,
	.strip    = &__cmodules_string_strip,
	.replace  = &__cmodules_string_replace,
	.search   = &__cmodules_string_search,
	.equals   = &__cmodules_string_equals,
	.concat   = &__cmodules_string_concat,
	.insert   = &__cmodules_string_insert,
	.fill     = &__cmodules_string_fill,
	.clear    = &__cmodules_string_clear,
	.filter   = &__cmodules_string_filter,
	.map      = &__cmodules_string_map,
};
inline Char __cmodules_get_string(String S, size_t pos) {
    #ifdef __CMODULES_ERROR_CHECK
	if (pos >= clen(S)) {
		cerror = BOUNDS_VIOLATION;
		return cval(S)[0];
	}
    #endif
	return cval(S)[pos];
}
inline Char* __cmodules_iter_beg_string(String S) {
	return chead(S);
}
inline Char* __cmodules_iter_end_string(String S) {
	return cval(S) + clen(S);
}
inline Char* __cmodules_iter_next_string(Char* C) {
	return C + 1;
}

/*************************/
/*** Create new String ***/
/*************************/
String __cmodules_string_new(const Char* s) {

    #ifdef __CMODULES_ERROR_CHECK
	if ((cerror = (s != NULL) ? NO_ERROR : NULL_ARGUMENT) != NO_ERROR)
		return NULL;
    #endif

	size_t len = wcslen(s);
	#ifndef __CMODULES_STRING_BUFFER_SIZE
	Char*  str = calloc(len+1, sizeof(Char));
	size_t mem = (len+1)*sizeof(Char);
	#else
	Char* str = calloc(__CMODULES_STRING_BUFFER_SIZE+1, sizeof(Char));
	size_t mem = (__CMODULES_STRING_BUFFER_SIZE+1)*sizeof(Char);
	#endif

    #ifdef __CMODULES_ERROR_CHECK
	if ((cerror = (str != NULL) ? NO_ERROR : MALLOC_FAILURE) != NO_ERROR)
		return NULL;
    #endif

	String S = malloc(sizeof(struct __cmodules_string));
    #ifdef __CMODULES_ERROR_CHECK
	if ((cerror = (S != NULL) ? NO_ERROR : MALLOC_FAILURE) != NO_ERROR)
	{
		free(str);
		return NULL;
	}
    #endif

	wcscpy(str, s);
	cval(S) = str;
	clen(S) = len;
	cmem(S) = mem;

	return S;
}

/****************************/
/*** Safely delete String ***/
/****************************/

Error __cmodules_string_del(String S) {

    #ifdef __CMODULES_ERROR_CHECK
	if ((cerror = (S != NULL) ? NO_ERROR : NULL_ARGUMENT) != NO_ERROR)
		return cerror;
    #endif

	free(cval(S));
	free(S);

    #ifdef __CMODULES_ERROR_CHECK
	return (cerror = NO_ERROR);
    #else
	return NO_ERROR;
    #endif
};

/**************************************************************/
/*** Assign new value to the String, expanding if necessary ***/
/**************************************************************/
Error __cmodules_string_set(String S, const Char* s) {

    #ifdef __CMODULES_ERROR_CHECK
	if (cval(S) == NULL || s == NULL)
		return (cerror = NULL_ARGUMENT);
    #endif

	wmemset(cval(S), L'\0', clen(S));

	size_t newlen = wcslen(s);
	size_t newmem = (newlen+1)*sizeof(Char);
	if (newmem > cmem(S)) {
		Char* str = calloc(cmem(S)*2-1, sizeof(Char));
        #ifdef __CMODULES_ERROR_CHECK
		if ((cerror = (str != NULL) ? NO_ERROR : MALLOC_FAILURE) != NO_ERROR)
			return cerror;
        #endif
		free(cval(S));
		cval(S) = str;
		cmem(S) = newmem;
	}
	wcscpy(cval(S), s);
	clen(S) = newlen;

    #ifdef __CMODULES_ERROR_CHECK
	return (cerror = NO_ERROR);
    #else
	return NO_ERROR;
    #endif
};

/***********************************/
/*** Return copy of given String ***/
/***********************************/
String __cmodules_string_copy(String S) {

    #ifdef __CMODULES_ERROR_CHECK
	if ((cerror = (S != NULL) ? NO_ERROR : NULL_ARGUMENT) != NO_ERROR)
		return NULL;
    #endif

	Char* str = malloc(cmem(S));
    #ifdef __CMODULES_ERROR_CHECK
	if ((cerror = (str != NULL) ? NO_ERROR : MALLOC_FAILURE) != NO_ERROR)
		return NULL;
    #endif

	String S2 = malloc(sizeof(struct __cmodules_string));
    #ifdef __CMODULES_ERROR_CHECK
	if ((cerror = (S != NULL) ? NO_ERROR : MALLOC_FAILURE) != NO_ERROR)
	{
		free(str);
		return NULL;
	}
    #endif

	wcscpy(str, cval(S));
	cval(S2) = str;
	clen(S2) = clen(S);
	cmem(S2) = cmem(S);

	return S2;
}

/****************************************************************************/
/*** Extract slice of String (characters between a and b) as a new String ***/
/****************************************************************************/
String __cmodules_string_slice(String S, size_t a, size_t b) {

    #ifdef __CMODULES_ERROR_CHECK
	if ((cerror = (S != NULL) ? NO_ERROR : NULL_ARGUMENT) != NO_ERROR)
		return NULL;
    #endif

    #ifdef __CMODULES_ERROR_CHECK
	if (a > b || a > clen(S) || b > clen(S)) {
		cerror = BOUNDS_VIOLATION;
		return NULL;
	}
    #endif

	size_t len = (b - a) + 1;
	#ifndef __CMODULES_STRING_BUFFER_SIZE
	Char*  str = calloc(len+1, sizeof(Char));
	size_t mem = (len+1)*sizeof(Char);
    #else
	Char* str = calloc(__CMODULES_STRING_BUFFER_SIZE+1, sizeof(Char));
	size_t mem = (__CMODULES_STRING_BUFFER_SIZE+1)*sizeof(Char);
	#endif

    #ifdef __CMODULES_ERROR_CHECK
	if ((cerror = (str != NULL) ? NO_ERROR : MALLOC_FAILURE) != NO_ERROR)
		return NULL;
    #endif

	String sub = malloc(sizeof(struct __cmodules_string));
    #ifdef __CMODULES_ERROR_CHECK
	if ((cerror = (sub != NULL) ? NO_ERROR : MALLOC_FAILURE) != NO_ERROR)
	{
		free(str);
		return NULL;
	}
    #endif

	wcsncpy(str, cval(S)+a, len);
	cval(sub) = str;
	clen(sub) = len;
	cmem(sub) = mem;

	return sub;
};

/*************************************************/
/*** Pre-allocate space for a String of size n ***/
/*************************************************/
String __cmodules_string_alloc(size_t n) {

	Char* str = calloc(n+1, sizeof(Char));
    #ifdef __CMODULES_ERROR_CHECK
	if ((cerror = (str != NULL) ? NO_ERROR : MALLOC_FAILURE) != NO_ERROR)
		return NULL;
    #endif

	String S = malloc(sizeof(struct __cmodules_string));
    #ifdef __CMODULES_ERROR_CHECK
	if ((cerror = (S != NULL) ? NO_ERROR : MALLOC_FAILURE) != NO_ERROR)
		return NULL;
    #endif

	cval(S) = str;
	clen(S) = 0;
	cmem(S) = (n+1)*sizeof(Char);

	return S;
};

/********************************************************************/
/*** Resize a String to size n, copies old string into new buffer ***/
/********************************************************************/
Error __cmodules_string_resize(String S, size_t n) {

    #ifdef __CMODULES_ERROR_CHECK
	if (cval(S) == NULL)
		return (cerror = NULL_ARGUMENT);
    #endif

	Char* str = calloc(n+1, sizeof(Char));
    #ifdef __CMODULES_ERROR_CHECK
	if ((cerror = (str != NULL) ? NO_ERROR : MALLOC_FAILURE) != NO_ERROR)
		return cerror;
    #endif

	wcsncpy(str, cval(S), min(n,clen(S)));
	free(cval(S));
	cval(S) = str;
	cmem(S) = (n+1)*sizeof(Char);

    #ifdef __CMODULES_ERROR_CHECK
	return (cerror = NO_ERROR);
    #else
	return NO_ERROR;
    #endif
};

/*********************************************************/
/*** Convert all characters in the String to lowercase ***/
/*********************************************************/
Error __cmodules_string_to_lower(String S) {

    #ifdef __CMODULES_ERROR_CHECK
	if (cval(S) == NULL)
		return (cerror = NULL_ARGUMENT);
    #endif

	for_range(i,clen(S),1) {
		cval(S)[i] = towlower(cval(S)[i]);
	}

    #ifdef __CMODULES_ERROR_CHECK
	return (cerror = NO_ERROR);
    #else
	return NO_ERROR;
    #endif
};

/**********************************************************/
/*** Convert all characters int the String to lowercase ***/
/**********************************************************/
Error __cmodules_string_to_upper(String S) {

    #ifdef __CMODULES_ERROR_CHECK
	if (cval(S) == NULL)
		return (cerror = NULL_ARGUMENT);
    #endif

	for_range(i,clen(S),1) {
		cval(S)[i] = towupper(cval(S)[i]);
	}

    #ifdef __CMODULES_ERROR_CHECK
	return (cerror = NO_ERROR);
    #else
	return NO_ERROR;
    #endif
};

/*********************************************************************************/
/*** Strips all whitespace characters from the beginning and end of the String ***/
/*********************************************************************************/
Error __cmodules_string_strip(String S) {

    #ifdef __CMODULES_ERROR_CHECK
	if (cval(S) == NULL || clen(S) == 0)
		return (cerror = NULL_ARGUMENT);
    #endif

	Char* old = cval(S);
	size_t a = 0;
	while (iswspace(*old)){
		a++;
		old++;
	}

	old = cval(S) + clen(S) - 1;
	size_t b = clen(S);
	while (iswspace(*old)) {
		b--;
		old--;
	}

	size_t newlen = b - a;
	if (newlen < clen(S)) {
		Char* str = calloc(cmem(S), sizeof(Char));
        #ifdef __CMODULES_ERROR_CHECK
		if ((cerror = (str != NULL) ? NO_ERROR : MALLOC_FAILURE) != NO_ERROR)
			return cerror;
        #endif
		wcsncpy(str, cval(S)+a, (b-a));
		free(cval(S));
		cval(S) = str;
		clen(S) = newlen;
	}

    #ifdef __CMODULES_ERROR_CHECK
	return (cerror = NO_ERROR);
    #else
	return NO_ERROR;
    #endif
};

/************************************************/
/*** Replace all instances of s1 in S with s2 ***/
/************************************************/
Error __cmodules_string_replace(String S, const Char* s1, const Char* s2) {

    #ifdef __CMODULES_ERROR_CHECK
	if (cval(S) == NULL || s1 == NULL || s2 == NULL)
		return (cerror = NULL_ARGUMENT);
    #endif

	Char*  str     = NULL;
	size_t s1_len  = wcslen(s1);
	size_t s2_len  = wcslen(s2);
	size_t str_mem = cmem(S);
	size_t newlen  = clen(S);
	size_t j       = 0;
	for_range(i,clen(S),0) {
		size_t n = 0;
		while (cval(S)[i+n] == s1[n] && (i+n) < clen(S))
			n++;
		if (n == s1_len) {
			if (str == NULL) {
				str = calloc(str_mem, sizeof(Char));
                #ifdef __CMODULES_ERROR_CHECK
				if ((cerror = (str != NULL) ? NO_ERROR : MALLOC_FAILURE) != NO_ERROR)
					return cerror;
                #endif
				wcsncpy(str, cval(S), i);
			}
			newlen = (newlen - s1_len) + s2_len;
			if ((newlen+1)*sizeof(Char) > str_mem) {
                #ifdef __CMODULES_ERROR_CHECK
				Char* old = str;
                #endif
				str_mem = sizeof(Char)*(newlen+1);
				str = realloc(str, str_mem);
                #ifdef __CMODULES_ERROR_CHECK
				if ((cerror = (str == old) ? NO_ERROR : MALLOC_FAILURE) != NO_ERROR) {
					free(str);
					return cerror;
				}
                #endif
				str[newlen] = L'\0';
			}
			wcsncpy(str+j, s2, s2_len);
			i += s1_len;
			j += s2_len;
		} else {
			if (str != NULL)
				str[j] = cval(S)[i];
			i += 1;
			j += 1;
		}
	}

	if (str != NULL) {
		free(cval(S));
		cval(S) = str;
		clen(S) = newlen;
		cmem(S) = str_mem;
	}

    #ifdef __CMODULES_ERROR_CHECK
	return (cerror = NO_ERROR);
    #else
	return NO_ERROR;
    #endif
}

/********************************************************************************************/
/*** Search first instance of s in S (starting from position pos) and return its position ***/
/********************************************************************************************/
size_t __cmodules_string_search(String S, const Char* s, size_t pos) {

    #ifdef __CMODULES_ERROR_CHECK
	if (S == NULL || s == NULL)
		return (cerror = NULL_ARGUMENT);
	if (pos > clen(S)-1)
		return (cerror = BOUNDS_VIOLATION);
    #endif

	size_t s_len = wcslen(s);
	for_range(cur,clen(S),1) {
		if (cur >= pos) {
			size_t n = 0;
			while (cval(S)[cur+n] == s[n] && cur+n < clen(S)) {
				n++;
			}
			if (n == s_len)
				return cur;
		}
	}

	return NOT_FOUND;
}

/******************************************************************/
/*** Returns true if String S is equal to s, else returns false ***/
/******************************************************************/
bool __cmodules_string_equals(String S, const Char* s) {

    #ifdef __CMODULES_ERROR_CHECK
	if (S == NULL || s == NULL) {
		cerror = NULL_ARGUMENT;
		return false;
	}
    #endif

	size_t s_len = wcslen(s);
	if (s_len != clen(S))
		return false;

	size_t n = 0;
	while (cval(S)[n] == s[n] && n < clen(S))
		n++;

	if (n == clen(S))
		return true;

	return false;
}

/**********************************/
/*** Concatenates s to String S ***/
/**********************************/
Error __cmodules_string_concat(String S, const Char* s) {

    #ifdef __CMODULES_ERROR_CHECK
	if (S == NULL || s == NULL)
		return (cerror = NULL_ARGUMENT);
    #endif

	size_t s_len  = wcslen(s);
	size_t newlen = s_len + clen(S);
	if ((newlen+1) * sizeof(Char) > cmem(S)) {
		Char* str = calloc(newlen+1, sizeof(Char));
        #ifdef __CMODULES_ERROR_CHECK
		if (str == NULL)
			return (cerror = MALLOC_FAILURE);
        #endif
		wcscpy(str, cval(S));
		wcscat(str, s);
		free(cval(S));
		cval(S) = str;
		cmem(S) = (newlen+1) * sizeof(Char);
	}
	else {
		wcscat(cval(S), s);
	}
	clen(S) = newlen;

    #ifdef __CMODULES_ERROR_CHECK
	return (cerror = NO_ERROR);
    #else
	return NO_ERROR;
    #endif
}

/***********************************************/
/*** Inserts s into String S at position pos ***/
/***********************************************/
Error __cmodules_string_insert(String S, const Char* s, size_t pos) {

    #ifdef __CMODULES_ERROR_CHECK
	if (S == NULL || s == NULL)
		return (cerror = NULL_ARGUMENT);
	if (pos > clen(S))
		return (cerror = BOUNDS_VIOLATION);
    #endif

	size_t s_len  = wcslen(s);
	size_t newlen = s_len + clen(S);
	if ((newlen+1) * sizeof(Char) > cmem(S)) {
		Char* str = calloc(newlen+1, sizeof(Char));
        #ifdef __CMODULES_ERROR_CHECK
		if (str == NULL)
			return (cerror = MALLOC_FAILURE);
        #endif
		wcsncpy(str, cval(S), pos);
		wcscpy(str+pos, s);
		wcsncpy(str+pos+s_len, cval(S)+pos, clen(S)-pos);
		free(cval(S));
		cval(S) = str;
		cmem(S) = (newlen+1) * sizeof(Char);
	}
	else {
		wcsncpy(cval(S)+pos+s_len, cval(S)+pos, clen(S)-pos);
		wcscpy(cval(S)+pos, s);
	}
	clen(S) = newlen;

    #ifdef __CMODULES_ERROR_CHECK
	return (cerror = NO_ERROR);
    #else
	return NO_ERROR;
    #endif
}

/******************************************************/
/*** Fills String S with consecutive instances of s ***/
/******************************************************/
Error __cmodules_string_fill(String S, const Char* s) {

    #ifdef __CMODULES_ERROR_CHECK
	if (S == NULL || s == NULL)
		return (cerror = NULL_ARGUMENT);
    #endif

	size_t s_len = wcslen(s);
	size_t S_len = cmem(S)/sizeof(Char) - 1;
	for_range(i,S_len,s_len) {
		if (i + s_len > S_len)
			wcsncpy(cval(S)+i, s, S_len-i);
		else
			wcscpy(cval(S)+i,s);
	}
	clen(S) = S_len;

    #ifdef __CMODULES_ERROR_CHECK
	return (cerror = NO_ERROR);
    #else
	return NO_ERROR;
    #endif
}

/********************************/
/*** Sets all bytes of S to 0 ***/
/********************************/
Error __cmodules_string_clear(String S) {

    #ifdef __CMODULES_ERROR_CHECK
	if (S == NULL)
		return (cerror = NULL_ARGUMENT);
    #endif

	wmemset(cval(S), L'\0', cmem(S)/sizeof(Char));
	clen(S) = 0;

    #ifdef __CMODULES_ERROR_CHECK
	return (cerror = NO_ERROR);
    #else
	return NO_ERROR;
    #endif
}

/*****************************************************************/
/*** Applies filter function fun to the characters of String S ***/
/*****************************************************************/
Error __cmodules_string_filter(String S, bool (*fun)(Char)) {

    #ifdef __CMODULES_ERROR_CHECK
	if (S == NULL || fun == NULL)
		return (cerror = NULL_ARGUMENT);
    #endif

	Char* str = calloc(cmem(S),1);
	size_t newlen = 0;
    #ifdef __CMODULES_ERROR_CHECK
	if (str == NULL)
		return (cerror = MALLOC_FAILURE);
    #endif

	foreach(c,S) {
		if (fun(*c)) {
			str[newlen++] = *c;
		}
	}

	free(cval(S));
	cval(S) = str;
	clen(S) = newlen;

    #ifdef __CMODULES_ERROR_CHECK
	return (cerror = NO_ERROR);
    #else
	return NO_ERROR;
    #endif
}

/**************************************************************************/
/*** Maps characters in String S to ones defined by mapping function fun***/
/**************************************************************************/
Error __cmodules_string_map(String S, Char (*fun)(Char)) {

    #ifdef __CMODULES_ERROR_CHECK
	if (S == NULL || fun == NULL)
		return (cerror = NULL_ARGUMENT);
    #endif

	for_range(i,clen(S),1) {
		cval(S)[i] = fun(cval(S)[i]);
	}

    #ifdef __CMODULES_ERROR_CHECK
	return (cerror = NO_ERROR);
    #else
	return NO_ERROR;
    #endif
}


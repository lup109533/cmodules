#include <stdio.h>
#include <stdlib.h>
#include <wctype.h>

struct __cmodules_string_library string = {
	.new      = &__cmodules_string_new,
	.del      = &__cmodules_string_del,
	.set      = &__cmodules_string_set,
	.alloc    = &__cmodules_string_alloc,
	.resize   = &__cmodules_string_resize,
	.to_lower = &__cmodules_string_to_lower,
	.to_upper = &__cmodules_string_to_upper,
	.strip    = &__cmodules_string_strip,
};

/*************************/
/*** Create new String ***/
/*************************/
String __cmodules_string_new(const Char* s) {

#ifdef __CMODULES_ERROR_CHECK
	if ((cerror = (s != NULL) ? NO_ERROR : NULL_ARGUMENT) != NO_ERROR)
		return NULL;
#endif

	size_t len = wcslen(s);
	Char* str = calloc(len, sizeof(Char));
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
	clen(S) = wcslen(s);
	cmem(S) = (clen(S)+1)*sizeof(Char);

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
		Char* str = calloc(newlen, sizeof(Char));
	#ifdef __CMODULES_ERROR_CHECK
		if ((cerror = (str != NULL) ? NO_ERROR : MALLOC_FAILURE) != NO_ERROR)
			return NULL;
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

/**********************************************************/
/*** Convert all characters int the String to lowercase ***/
/**********************************************************/
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
		Char* str = calloc(newlen, sizeof(Char));
	#ifdef __CMODULES_ERROR_CHECK
		if ((cerror = (str != NULL) ? NO_ERROR : MALLOC_FAILURE) != NO_ERROR)
			return NULL;
	#endif
		wcsncpy(str, cval(S)+a, (b-a));
		free(cval(S));
		cval(S) = str;
		clen(S) = newlen;
		cmem(S) = (newlen+1)*sizeof(Char);
	}
		
#ifdef __CMODULES_ERROR_CHECK
	return (cerror = NO_ERROR);
#else
	return NO_ERROR;
#endif
};
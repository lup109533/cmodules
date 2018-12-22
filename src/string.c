#include <stdlib.h>

struct __cmodules_string_library string = {
	.new   = &__cmodules_string_new,
	.del   = &__cmodules_string_del,
	.alloc = &__cmodules_string_alloc,
};

String __cmodules_string_new(const Char* s) {

#ifdef __CMODULES_ERROR_CHECK
	if ((cerror = (s != NULL) ? NO_ERROR : NULL_ARGUMENT) != NO_ERROR)
		return NULL;
#endif

	size_t len = wcslen(s);
	Char* str = calloc(len+1, sizeof(Char));

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

	cval(S) = wcscpy(str, s);
	clen(S) = len;

	return S;
}

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
	clen(S) = n;

	return S;
};

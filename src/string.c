#include <stdlib.h>

struct __cmodules_string_library string = {
	.new = &new,
	.del = &del,
};

String new(const Char* s) {

#if __CMODULES_ERROR_CHECK == true
	if ((cerror = (s != NULL) ? NO_ERROR : NULL_ARGUMENT) != NO_ERROR)
		return NULL;
#endif

	size_t len = wcslen(s);
	Char* str = calloc(len+1, sizeof(Char));

#if __CMODULES_ERROR_CHECK == true
	if ((cerror = (str != NULL) ? NO_ERROR : MALLOC_FAILURE) != NO_ERROR)
		return NULL;
#endif

	String S = malloc(sizeof(struct __cmodules_string));

#if __CMODULES_ERROR_CHECK == true
	if ((cerror = (S != NULL) ? NO_ERROR : MALLOC_FAILURE) != NO_ERROR)
	{
		free(str);
		return NULL;
	}
#endif

	cval(S) = strcpy(str, s);
	clen(S) = len;

	return S;
}

Error del(String S) {

#if __CMODULES_ERROR_CHECK == true
	if ((cerror = (S != NULL) ? NO_ERROR : NULL_ARGUMENT) != NO_ERROR)
		return cerror;
#endif

	free(cval(S));
	free(S);

#if __CMODULES_ERROR_CHECK == true
	return (cerror = NO_ERROR);
#else
	return NO_ERROR;
#endif
};

#include <stdlib.h>
#include <string.h>

struct __cmodules_string_library string = {
	.new = &new,
	.del = &del,
};

String new(const char* s) {
	if ((cerror = (s != NULL) ? NONE : NULL_ARGUMENT) != NONE)
		return NULL;

	size_t len = strlen(s);
	char* str = calloc(len+1, sizeof(char));

	if ((cerror = (str != NULL) ? NONE : MALLOC_FAILURE) != NONE)
		return NULL;

	String S = malloc(sizeof(struct __cmodules_string));

	if ((cerror = (S != NULL) ? NONE : MALLOC_FAILURE) != NONE)
	{
		free(str);
		return NULL;
	}

	cval(S) = strcpy(str, s);
	clen(S) = len;

	return S;
}

Error del(String S) {
	if ((cerror = (S != NULL) ? NONE : NULL_ARGUMENT) != NONE)
		return cerror;

	free(cval(S));
	free(S);

	return (cerror = NONE);
};

#include <stdio.h>
#include <stdarg.h>

struct info {
	unsigned int length;
	unsigned int size;
};

int vfscanf_call(FILE *f, char *format, ...)
{
	int res;
	va_list arglist;
	va_start(arglist, format);
	res = vfscanf(f, format, arglist);
	va_end(arglist);
	return res;
}

int acquire_info(FILE *f, struct info *inf)
{
	int res = vfscanf_call(f, "%u %u\n", &inf->length, &inf->size);
	if (res < 2 || res == EOF)
		return 0;
	return 1;
}

int acquire_row(FILE *f, int *vec, struct info *inf)
{
	int res = 0;
	for (int i = 0; i < inf->length - 1; ++i) {
		if (vfscanf_call(f, "%d ", vec + i) < 1)
			break;
	}
	res = vfscanf_call(f, "%d\n", vec + (inf->length - 1));
	return res == 1;
}

int cross(int *vec, int *row, struct info *inf)
{
	int res = 0;
	for (int i = 0; i < inf->length; ++i)
		res += vec[i] * row[i];
	return res;
}


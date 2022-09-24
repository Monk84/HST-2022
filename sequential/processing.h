#include <stdio.h>

struct info {
	unsigned int length;
	unsigned int size;
};

int cross(int *vec, int *row, struct info *inf)
{
	int res = 0;
	for (int i = 0; i < inf->length; ++i)
		res += vec[i] * row[i];
	return res;
}


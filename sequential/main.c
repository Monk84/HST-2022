#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include "processing.h"

int main(int argc, char *argv[])
{
	int res = 0;
	if (argc < 3) {
		printf("Usage: %s infname outfname\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	FILE *infile = fopen(argv[1], "r");
	if (!infile) {
		printf("Error %d while opening %s\n", errno, argv[1]);
		exit(EXIT_FAILURE);
	}
	size_t fsize;
	fseek(infile, 0, SEEK_END);
	fsize = ftell(infile);
	fseek(infile, 0, SEEK_SET);
		
	struct info inf;
	if (!acquire_info(infile, &inf)) {
		printf("Error while acquiring length and size of matrix!\n");
		exit(EXIT_FAILURE);
	}
	
	int *vec = malloc(inf.length * sizeof(*vec));
	if (!vec) {
		printf("Bad malloc, try again!\n");
		res = -EXIT_FAILURE;
		goto bad_malloc_vec;
	}
	
	if (!acquire_row(infile, vec, &inf)) {
		printf("Couldn't acquire vector!\n");
		res = -EXIT_FAILURE;
		goto bad_acquire_vec;
	}

	int *row = malloc(inf.length * sizeof(*row));
	if (!row) {
		printf("Bad malloc, try again!\n");
		res = -EXIT_FAILURE;
		goto bad_malloc_row;
	}
	
	FILE *outfile = fopen(argv[2], "w");
	if (!outfile) {
		printf("Error %d while opening %s\n", errno, argv[2]);
		res = -EXIT_FAILURE;
		goto bad_outfile;
	}
	int cross_res;
	struct timespec start;
	timespec_get(&start, TIME_UTC);
	printf("Starting processing...");
	for (int i = 0; i < inf.size; ++i) {
		if (!acquire_row(infile, row, &inf)) {
			printf("\nCouldn't acquire row from file - not enough rows!\n");
			res = -EXIT_FAILURE;
			break;
		}
		cross_res = cross(vec, row, &inf);
		fprintf(outfile, "%d ", cross_res);
	}
	struct timespec end;
	timespec_get(&end, TIME_UTC);
	long int microsecs = (end.tv_sec - start.tv_sec) * 1000000 +
		(end.tv_nsec - start.tv_nsec) / 1000;
	printf(" %ld micros elapsed, %lu MBs processed\n", microsecs, fsize >> 20);
	fprintf(outfile, "\n%ld micros elapsed, %lu MBs processed\n", microsecs, fsize >> 20);
	if (!res) {
		printf("Success\n");
		res = EXIT_SUCCESS;
	}
	fclose(outfile);
bad_outfile:
	free(row);
bad_malloc_row:
bad_acquire_vec:
	free(vec);
bad_malloc_vec:
	fclose(infile);
	return res;
}


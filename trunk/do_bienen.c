/*
 * do_bienen.c
 * Implementierung Bienenalgorithmus
 *
 * $HeadURL$
 * $Id$
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <ctype.h>
#include <stdarg.h>
#include "bienen.h"
#include "globals.h"

double get_utime() {
	struct rusage rusage;
	getrusage(RUSAGE_SELF, &rusage);
	return (double) rusage.ru_utime.tv_sec + (double) rusage.ru_utime.tv_usec / 1000000.0;
}

void header() {
	printf("use2Opt, use2OptE, w, p_star, p, theta, a, b, B_max, experiment, run, iteration, time, lopt\n");
}

void output(int use2Opt, int use2OptE, double w, double p_star, double p, double theta, 
 double a, double b, int B_max, int run, int iteration, double time_used, char *experiment) {
	printf("%d, %d, %f, %f, %f, %f, %f, %f, %d, %s, %d, %d, %f, %f\n", 
	 use2Opt, use2OptE, w, p_star, p, theta, a, b, B_max, experiment, run, iteration, time_used, lopt);
}

int main(int argc, char **argv)
{
	double B_max = 10, w = 0.5, b = 5, p_star = 0.05, p = 0.7, 
	 theta = 0.3, a = 0.001, z = 0, use2Opt = 1, use2OptE = 1, 
	 repeat = 1, z_max = 1000;
	struct tag {
		char *tag;
		double *var;
	} tags[] = {
		{"B_max", &B_max}, 
		{"w", &w}, 
		{"b", &b}, 
		{"p_star", &p_star}, 
		{"p", &p}, 
		{"theta", &theta}, 
		{"a", &a},
		{"z", &z}, 
		{"use2Opt", &use2Opt}, 
		{"use2OptE", &use2OptE},
		{"repeat", &repeat},
		{"z_max", &z_max},
		{NULL, NULL}
	};

	if (argc != 3) {
		fprintf(stderr, "SYNTAX: %s <filename of graph> <filename of control file>\n", argv[0]);
		exit(1);
	}
	if (loadGraph(argv[1]))
		printf("File %s not found.\n", argv[1]);
	header();
	FILE *f = fopen(argv[2], "r");
	if (f == NULL) {
		printf("Cannot open file %s.\n", argv[2]);
		exit(1);
	}
	char buffer[256];
	while (fgets(buffer, 255, f)) {
		char this_tag[256], this_experiment[256];
		char *bp = buffer;
		while ((*bp != 0) && isspace(*bp))
			bp++;
		if (*bp == 0)
			continue;
		if (strncmp(buffer, "run", 3) == 0) {
			sscanf(buffer, "%s %s", this_tag, this_experiment);
			for (int run = 0; run < repeat; run++) {
				fprintf(stderr, "Run %s (%d / %.0lf)\n", this_experiment, run, repeat);
				initialize();
				double time_used = 0;
				for (int z = 1; z <= z_max; z++) {
					double start_time = get_utime();
					iteration(B_max, w, b, p_star, p, theta, a, (int) z, (int) use2Opt, (int) use2OptE);
					time_used += get_utime() - start_time;
					output((int) use2Opt, (int) use2OptE, w, p_star, p, theta, a, b, (int) B_max, 
					 run, z, time_used, this_experiment);
				}
			}
		} else {
			char this_tag[256];
			double this_value;
			
			sscanf(buffer, "%s %lf", this_tag, &this_value);
			struct tag *t;
			for (t = tags; t->tag; t++) {
				if (strcmp(this_tag, t->tag) == 0) {
					*t->var = this_value;
					break;
				}
			}
			if (!t->tag) {
				fprintf(stderr, "Unrecognized tag %s in control file.\n", this_tag);
				exit(1);
			}
		}
	}

	return 0;
}

/*
 * globals.c
 * Implementierung Bienenalgorithmus
 *
 * $HeadURL: svn://localhost/nadia/diplomarbeit/software/globals.c $
 * $Id: globals.c 220 2008-08-17 09:48:14Z kthul $
 */

#include <math.h>
#include "bienen.h"

int numKnoten = 0;
struct {
	double x, y;
} Knoten[MAX_KNOTEN];

double distance[MAX_KNOTEN][MAX_KNOTEN];

double *n[MAX_ITERATION];
double n_sum[MAX_KNOTEN][MAX_KNOTEN];
double n_sum_max;

int path_available;
int path[MAX_KNOTEN];
int path_alltimes[MAX_KNOTEN];

struct Biene bienen[MAX_BIENEN];
int numBienen;

double lopt = MAXFLOAT;
int start_node = -1;
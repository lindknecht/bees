/*
 * globals.h
 * Implementierung Bienenalgorithmus
 *
 * $HeadURL$
 * $Id$
 */

extern int numKnoten;
extern struct {
	double x, y;
} Knoten[MAX_KNOTEN];

extern double distance[MAX_KNOTEN][MAX_KNOTEN];

extern double *n[MAX_ITERATION];
extern double n_sum[MAX_KNOTEN][MAX_KNOTEN];
extern double n_sum_max;

extern int path_available;
extern int path[MAX_KNOTEN];
extern int path_alltimes[MAX_KNOTEN];

extern struct Biene bienen[MAX_BIENEN];
extern int numBienen;

extern double lopt;
extern int start_node;
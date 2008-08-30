/*
 * bienen.h
 * Implementierung Bienenalgorithmus
 *
 * $HeadURL$
 * $Id$
 */

#define MAX_KNOTEN 2000
#define MAX_BIENEN 200
#define MAX_ITERATION 2000

struct Biene {
	int activ;
	int path[MAX_KNOTEN];
	int steps_path;
	int dance;
	int keep;
	int count;
};

int loadGraph(const char *file);
double iteration(int B_max, double w, int b, double p_star, double p, double theta, double a, 
 int z, int use2Opt, int use2OptE);
void initialize();
int stage(int u, int B_max, double w, int b, double p_star, double p, double theta, double a, int z, int use2Opt, int use2OptE);

/*
 * bienen.c
 * Implementierung Bienenalgorithmus
 *
 * $HeadURL: svn://localhost/nadia/diplomarbeit/software/bienen.c $
 * $Id: bienen.c 220 2008-08-17 09:48:14Z kthul $
 */

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include "bienen.h"
#include "globals.h"

double length_path(struct Biene *b, int include_return) {
	double length = 0;
	for (int j = 0; j < b->steps_path; j++)
		length += distance[b->path[j]][b->path[j + 1]];
	if (include_return)
		length += distance[b->path[b->steps_path]][b->path[0]];
	return length;
}

void swap(int *p, int i, int j)
{
	int temp = p[i];
	p[i] = p[j];
	p[j] = temp;
}

void twoOptN(struct Biene *b) 
{
	double d0 = length_path(b, 1);
	for (int i = 0; i < b->steps_path; i++) {
		for (int j = i + 1; j < b->steps_path; j++) {
			swap(b->path, i, j);
			double l = length_path(b, 1); 
			if (l >= d0)
				swap(b->path, i, j);
			else
				d0 = l;
		}
	}
}

void revert(int *path, int i, int j, int l)
{
	int buffer[MAX_KNOTEN];
	
	for (int k = 0; k <= l; k++)
		buffer[k] = path[k];
	int i0 = i, j0 = j;
	while (1) {
		path[j0] = buffer[i0];
		path[i0] = buffer[j0];
		if (i0 == j)
			break;
		if (++i0 > l)
			i0 = 0;
		if (--j0 < 0)
			j0 = l;
	}
}

void twoOptE(struct Biene *b) 
{
	for (int i = 0; i < b->steps_path; i++) {
		int suc_i = i + 1;
		if (suc_i > b->steps_path)
			suc_i = 0;
		int k = suc_i;
		for (int j = 2; j < b->steps_path - 2; j++) {
			if (++k > b->steps_path)
				k = 0;
			int suc_k = k + 1;
			if (suc_k > b->steps_path)
				suc_k = 0;
			if (distance[b->path[i]][b->path[suc_i]] 
			 + distance[b->path[k]][b->path[suc_k]] 
			 > distance[b->path[k]][b->path[i]] 
			 + distance[b->path[suc_k]][b->path[suc_i]])
				revert(b->path, suc_i, k, b->steps_path);
		}
	}
}
		
void initialize()
{
	for (int i = 0; i < numKnoten; i++)
		for (int j = 0; j < numKnoten; j++)
			n_sum[i][j] = 1;
	for (int z = 0; z < MAX_ITERATION; z++) {
		if (n[z] != NULL)
			free(n[z]);
		n[z] = NULL;
	}
	n_sum_max = 1;
	lopt = MAXFLOAT;
}

int activate_bienen(int B_max, double w)
{
	int num_activ = 0;
	for (int i = 0; i < B_max; i++) {
		if (bienen[i].activ)
			num_activ++;
		else if ((double) random() / RAND_MAX < w) {
			bienen[i].activ = 1;
			bienen[i].dance = 0;
			bienen[i].keep = 0;	
			bienen[i].steps_path = 0;		
			num_activ++;
		}
	}
	return num_activ;
}

void initialize_bienen(int B_max, double w)
{
	do {
		start_node = floor((double) random() / RAND_MAX * numKnoten);
	} while (start_node >= numKnoten);
	for (int i = 0; i < B_max; i++) {
		bienen[i].path[0] = start_node;
		bienen[i].steps_path = 0;
		bienen[i].dance = 0;
		bienen[i].keep = 0;
		bienen[i].activ = 0;
	}
	while (activate_bienen(B_max, w) == 0)
		;
}

void food_source(int B_max, double a, double z)
{
	for (int i = 0; i < B_max; i++) {
		double p[numKnoten];
		double p_total = 0;
		int cur_pos = bienen[i].path[bienen[i].steps_path];
		for (int j = 0; j < numKnoten; j++) {
			for (int k = 0; k <= bienen[i].steps_path; k++)
				if (bienen[i].path[k] == j) {
					p[j] = -1;
					goto already_visit;
				}
			p[j] = exp(-a * distance[cur_pos][j] * z / n_sum[cur_pos][j]);
			p_total += p[j];
		  already_visit:;
		}
		if (p_total < 1E-10) {
			p_total = 0;
			for (int j = 0; j < numKnoten; j++)
				if (p[j] >= 0) {
					p[j] = 1;
					p_total += 1; 
				}
		}
		for (int j = 0; j < numKnoten; j++)
			p[j] /= p_total;
		double pp = ((double ) random()) / RAND_MAX;
		int j;
		for (j = 0; j < numKnoten; j++) {
			if (p[j] < 0)
				continue;
			pp -= p[j];
			if (pp < 0)
				break;
		}
		bienen[i].path[++bienen[i].steps_path] = j;
	}
}

double get_best_path(int B_max, int include_return, int *best_biene)
{
	int bb = 0;
	double L_min = length_path(&bienen[0], include_return);
	for (int k = 0; k < B_max; k++) {
		double l = length_path(&bienen[k], include_return); 
		if (l < L_min) {
			L_min = l;
			bb = k; 
		}	
	}
	if (best_biene)
		*best_biene = bb;
	return L_min;
}

void hive(int B_max, double u, double z, double p_star, double w)
{
	double L_min = get_best_path(B_max, 0, NULL);
	for (int k = 0; k < B_max; k++) {
		if (bienen[k].activ) {
			if (bienen[k].steps_path > 0) {
				double pk = exp(-(length_path(&bienen[k], 0) - L_min)/(u*z));
				if ((double) random() / RAND_MAX < pk) {
					// Biene verfolgt eigenen Pfad
					bienen[k].keep = 1;
					if ((double) random() / RAND_MAX < p_star)
						bienen[k].dance = 1;
					else
						bienen[k].dance = 0;
				} else
					bienen[k].keep = 0;
			}
		}
	}
	activate_bienen(B_max, w);
}

int compare_path(const struct Biene *b1, const struct Biene *b2)
{
	if (b1->steps_path != b2->steps_path)
		return 0;
	for (int i = 0; i < b1->steps_path; i++) {
		if (b1->path[i] != b2->path[i])
			return 0;		
	}
	return 1;
}

void dance_floor(int B_max, double p, double theta)
{
	struct Biene dancers[B_max];
	double rel_p[B_max];
	int num_dancers = 0;
	for (int i = 0; i < B_max; i++) {
		if (bienen[i].dance) {
			for (int j = 0; j < num_dancers; j++) {
				if (compare_path(&bienen[i], &dancers[j])) {
					dancers[j].count++;
					goto path_exists;
				}
			}
			dancers[num_dancers] = bienen[i];
			dancers[num_dancers].count = 1;
			num_dancers++;
		  path_exists:;
		}
	}
	if (num_dancers == 0) {
		int imin;
		double lmin = MAXFLOAT;
		for (int i = 0; i < B_max; i++) {
			if (!bienen[i].activ || (bienen[i].steps_path == 0))
				continue;
			double l = length_path(&bienen[i], 0);
			if (l < lmin) {
				lmin = l;
				imin = i;
			}
		}
		for (int i = 0; i < B_max; i++) {
			if (bienen[i].steps_path == 0)
				bienen[i] = bienen[imin];
		}
	} else {
		int min_count, max_count;
		double min_length, max_length;
		min_count = max_count = dancers[0].count;
		min_length = max_length = length_path(&dancers[0], 0);
		for (int i = 1; i < num_dancers; i++) {
			if (dancers[i].count < min_count)
				min_count = dancers[i].count;
			if (dancers[i].count > max_count)
				max_count = dancers[i].count;
			double l = length_path(&dancers[i], 0);
			if (l < min_length)
				min_length = l;
			if (l > max_length)
				max_length = l;
		}
		int delta_count = max_count - min_count;
		double delta_length = max_length - min_length;
		double sum_rel_p = 0;
		for (int i = 0; i < num_dancers; i++) {
			double alpha = (delta_length == 0) 
			 ? 1 : (length_path(&dancers[i], 0) - min_length) / (delta_length); 
			double beta = (delta_count == 0) ? 1 : (dancers[i].count - min_count) / (delta_count); 
			rel_p[i] = exp(p*beta - theta * alpha);
			sum_rel_p += rel_p[i];
		}
		for (int i = 0; i < num_dancers; i++)
			rel_p[i] /= sum_rel_p;
		for (int i = 0; i < B_max; i++) {
			if (!bienen[i].activ || bienen[i].keep)
				continue;
			double rn = ((double) random()) / RAND_MAX;
			int j;
			for (j = 0; j < num_dancers; j++) {
				rn -= rel_p[j];
				if (rn < 0)
					break;
			}
			bienen[i] = dancers[j];
		}
	}
}


int stage(int u, int B_max, double w, int b, double p_star, double p, double theta, double a, int z, int use2Opt, int use2OptE) {
	if (u == 1) {
		numBienen = B_max;
		initialize_bienen(B_max, w);
		n[z] = malloc(sizeof(double) * numKnoten * numKnoten);
		for (int i = 0; i < numKnoten * numKnoten; i++)
			n[z][i] = 1;
	}
	if (u < numKnoten) {
		food_source(B_max, a, z);
		hive(B_max, u, z, p_star, w);	
		dance_floor(B_max, p, theta);
		++u;
	} else {
		if (use2Opt)
			for (int i = 0; i < B_max; i++)
				twoOptN(&bienen[i]);
		if (use2OptE)
			for (int i = 0; i < B_max; i++)
				twoOptE(&bienen[i]);
		int best_biene = 0;
		double L_min = get_best_path(B_max, 1, &best_biene);
		path_available = 1;
		for (int i = 0; i < numKnoten; i++)
			path[i] = bienen[best_biene].path[i];
		if (L_min < lopt) {
			lopt = L_min;
			for (int i = 0; i < numKnoten; i++)
				path_alltimes[i] = path[i];
		}
		for (int i = 0; i < B_max; i++) {
			int j;
			for (j = 0; j < bienen[i].steps_path; j++)
				n[z][bienen[i].path[j] * numKnoten + bienen[i].path[j + 1]]++;
			n[z][bienen[i].path[j] * numKnoten + bienen[i].path[0]]++;			
		}
		n_sum_max = 0;
		for (int i = 0; i < numKnoten; i++) {
			for (int j = 0; j < numKnoten; j++) {
				n_sum[i][j] += n[z][i * numKnoten + j];
				if (z > b)
					n_sum[i][j] -= n[z - b][i * numKnoten + j];			
				if (n_sum[i][j] > n_sum_max)
					n_sum_max = n_sum[i][j];
			}
		}
		u = 1;
	}		
	return u;
}

double iteration(int B_max, double w, int b, double p_star, double p, double theta, double a, int z, int use2Opt, int use2OptE) {
	numBienen = B_max;
	initialize_bienen(B_max, w);
	n[z] = malloc(sizeof(double) * numKnoten * numKnoten);
	for (int i = 0; i < numKnoten * numKnoten; i++)
		n[z][i] = 1;
	for (int u = 1; u < numKnoten; u++) {
		food_source(B_max, a, z);
		hive(B_max, u, z, p_star, w);	
		dance_floor(B_max, p, theta);
	}
	if (use2Opt)
		for (int i = 0; i < B_max; i++)
			twoOptN(&bienen[i]);
	if (use2OptE)
		for (int i = 0; i < B_max; i++)
			twoOptE(&bienen[i]);
	int best_biene = 0;
	double L_min = get_best_path(B_max, 1, &best_biene);
	path_available = 1;
	for (int i = 0; i < numKnoten; i++)
		path[i] = bienen[best_biene].path[i];
	if (L_min < lopt) {
		lopt = L_min;
		for (int i = 0; i < numKnoten; i++)
			path_alltimes[i] = path[i];
	}
	for (int i = 0; i < B_max; i++) {
		int j;
		for (j = 0; j < bienen[i].steps_path; j++)
			n[z][bienen[i].path[j] * numKnoten + bienen[i].path[j + 1]]++;
		n[z][bienen[i].path[j] * numKnoten + bienen[i].path[0]]++;			
	}
	n_sum_max = 0;
	for (int i = 0; i < numKnoten; i++) {
		for (int j = 0; j < numKnoten; j++) {
			n_sum[i][j] += n[z][i * numKnoten + j];
			if (z > b)
				n_sum[i][j] -= n[z - b][i * numKnoten + j];			
			if (n_sum[i][j] > n_sum_max)
				n_sum_max = n_sum[i][j];
		}
	}
	return L_min;
}

int loadGraph(const char *file) {
	char buffer[1024];
	FILE *f = fopen(file, "r");
	if (f == NULL)
		return 1;
	numKnoten = 0;
	while(fgets(buffer, 1023, f) != NULL) {
		int i = 0;
		while (isspace(buffer[i]))
			i++;
		if (isdigit(buffer[i])) {
			sscanf(buffer, "%*lf %lf %lf", &Knoten[numKnoten].x, &Knoten[numKnoten].y);
			numKnoten++;
		}
	}
	for (int i = 0; i < numKnoten; i++) {
		distance[i][i] = 0.0;
		for (int j = 0; j < i; j++) {
			double dx = Knoten[i].x - Knoten[j].x;
			double dy = Knoten[i].y - Knoten[j].y;			
			distance[i][j] = distance[j][i] = sqrt(dx*dx + dy*dy);
		}
	}
	path_available = 0;
	lopt = 0;
	return 0;
}
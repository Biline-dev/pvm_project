/* TP algorithmique parallele
 * maitrise
 * LL
 * 13/03/2018
 * utilise gnuplot au lieu de jgraph
 */

/*
 * upper.c
 *
 * programme principal en sequentiel
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "point.h"
#include "pvm3.h"

/*
 * calcul recursif d'enveloppe
 * convexe par bissection
 */
void upper_hull(point *pts)
{
	point *upper, *pts2;

	upper = point_UH(pts); /* retourne 0 si plus de 4 points */
	if (!upper) {
		pts2 = point_part(pts);
		upper_hull(pts);
		upper_hull(pts2);
		point_merge_UH(pts, pts2);
	}
}

/*
 * upper <nb points>
 * exemple :
 * % upper 200 
 * % evince upper_hull.pdf
 */

int main(int argc, char **argv)
{
	//pvm_catchout(stdout);
	point *pts;

	if (argc != 2) {
		fprintf(stderr, "usage: %s <nb points>\n", *argv);
		exit(-1);
	}

	pts = point_random(atoi(argv[1]));
	print_point_list(pts);
	
	int child;
	pvm_spawn("/home/ivan/Documents/pvm/pvm_project/slave",NULL,0,NULL,1,&child);

	send_points_liste(pts,child);

	point * new_pts = receive_points_liste(child);
	print_point_list(new_pts);
	
	// point_print_gnuplot(pts, 0); /* affiche l'ensemble des points */
	// upper_hull(pts);
	// point_print_gnuplot(pts, 1); /* affiche l'ensemble des points restant, i.e l'enveloppe, en reliant les points */

	point_free(pts);
	point_free(new_pts);
	pvm_exit();
}
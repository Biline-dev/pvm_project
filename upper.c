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
#include "point.h"
#include "pvm3.h" 
/*
 * calcul recursif d'enveloppe
 * convexe par bissection
 */

/*
 * upper <nb points>
 * exemple :
 * % upper 200
 * % evince upper_hull.pdf
 */

int main(int argc, char **argv)
{
	pvm_catchout(stdout);
	point *pts;
	point *upper, *pts2;
	
	if (pvm_parent() == PvmNoParent)
	{
		if (argc != 2)
		{
			fprintf(stderr, "usage: %s <nb points>\n", *argv);
			exit(-1);
		}
		pts = point_random(atoi(argv[1]));
		point_print_gnuplot(pts, 0); /* affiche l'ensemble des points */
	}
	else
	{
		pts = receive_points_liste(pvm_parent());
	}

	upper = point_UH(pts); /* retourne 0 si plus de 4 points */
	if (!upper)
	{
		print_point_list(pts);
		pts2 = point_part(pts);
		int child1, child2;
		pvm_spawn("/home/ivan/Documents/pvm/pvm_project/upper",NULL,0,NULL,1,&child1);
		pvm_spawn("/home/ivan/Documents/pvm/pvm_project/upper",NULL,0,NULL,1,&child2);

		send_points_liste(pts,child1);
		send_points_liste(pts2,child2);

		pts = receive_points_liste(child1);
		pts2 = receive_points_liste(child2);

		pts = point_merge_UH(pts, pts2);
	}

	if (pvm_parent() != PvmNoParent)
	{
		send_points_liste(pts,pvm_parent());
	}
	else
	{
		point_print_gnuplot(pts, 1); /* affiche l'ensemble des points restant, i.e l'enveloppe, en reliant les points */
		point_free(pts); 	
	}

	pvm_exit();
	exit(0);
}
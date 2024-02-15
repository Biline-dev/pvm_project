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

#include "stdio.h"
#include "stdlib.h"
#include "point.h"
#include "pvm3.h"
/*
 * calcul recursif d'enveloppe
 * convexe par bissection
 */
void upper_hull(point *pts)
{
	point *upper, *pts2;
	int nbPointRecu;

	if (pvm_parent() != PvmNoParent)
	{
		printf("Fils");
		upper = receive_points_array(&nbPointRecu,pvm_parent());
		upper = array_to_list(upper,nbPointRecu);
	}
	upper = point_UH(pts); /* retourne 0 si plus de 4 points */
	if (!upper)
	{
		pts2 = point_part(pts);
		int child1, child2, nbPoint1, nbPoint2;
		nbPoint1 = point_nb(pts);
		nbPoint2 = point_nb(pts2);
		pvm_spawn("/home/ivan/Documents/pvm/TP4/UH/upper",NULL,0,NULL,1,&child1);
		pvm_spawn("/home/ivan/Documents/pvm/TP4/UH/upper",NULL,0,NULL,1,&child2);
		pts = list_to_array(pts);
		pts2 = list_to_array(pts2);
		send_points_array(pts,nbPoint1,child1);
		send_points_array(pts,nbPoint2,child2);
		printf("Jusque là ça va : \n");
		pts = receive_points_array(&nbPoint1,child1);
		pts2 = receive_points_array(&nbPoint2,child2);
		array_to_list(pts,nbPoint1);
		array_to_list(pts2,nbPoint2);
		point_merge_UH(pts, pts2);
	}
	else
	{
		if (pvm_parent() != PvmNoParent)
		{
			printf("Fils");
			int nbPoint = point_nb(upper);
			upper = list_to_array(upper);
			send_points_array(upper,nbPoint,pvm_parent());
		}
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
	point *pts;

	if (argc != 2) {
		fprintf(stderr, "usage: %s <nb points>\n", *argv);
		exit(-1);
	}

	pts = point_random(atoi(argv[1]));
	point_print_gnuplot(pts, 0); /* affiche l'ensemble des points */
	upper_hull(pts);
	point_print_gnuplot(pts, 1); /* affiche l'ensemble des points restant, i.e
					l'enveloppe, en reliant les points */
	point_free(pts); 
}


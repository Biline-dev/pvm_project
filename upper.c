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
	point *pts;

	if (argc != 2) {
		fprintf(stderr, "usage: %s <nb points>\n", *argv);
		exit(-1);
	}

	pts = point_random(atoi(argv[1]));
	print_point_list(pts);

	int nombre_de_points = point_nb(pts);
	point * pts_tableau = list_to_array(pts);
	print_point_array(pts_tableau,nombre_de_points);

	// point * retour_en_liste = array_to_list(pts_tableau,nombre_de_points);
	// print_point_list(retour_en_liste);

	int child;
	int codeSpawn, codePkByte, codeSend;
	codeSpawn = pvm_spawn("/home/ivan/Documents/pvm/TP4/UH/slave",NULL,0,NULL,1,&child);
	// pvm_initsend(PvmDataDefault);
	// codePkByte = pvm_pkint(&nombre_de_points,1,1);
	// printf("Code Spawn : %d, Code PkByte : %d; Code Send : %d\n",codeSpawn,codePkByte,codeSend);
	// codePkByte = pvm_pkbyte((char *)pts_tableau,sizeof(point) * nombre_de_points,1);
	// codeSend = pvm_send(child,1);
	// printf("Code Spawn : %d, Code PkByte : %d; Code Send : %d\n",codeSpawn,codePkByte,codeSend);

	// point * new_pts = malloc(sizeof(point) * nombre_de_points);
	// pvm_recv(child,1);
	// pvm_upkbyte((char*)new_pts,sizeof(point) * nombre_de_points,1);

	send_points_array(pts_tableau,nombre_de_points,child);

	int nombreDePointsRecu;

	printf(" Nombre points : %d\n",nombreDePointsRecu);
	point * new_pts = receive_points_array(&nombreDePointsRecu,child);
	print_point_array(new_pts,nombreDePointsRecu);

	// int child;
	// point unPoint;
	// unPoint.x = 3;
	// unPoint.y = 2;
	// int codeSpawn, codePkByte, codeSend;
	// codeSpawn = pvm_spawn("/home/ivan/Documents/pvm/TP4/UH/slave",NULL,0,NULL,1,&child);
	// pvm_initsend(PvmDataDefault);
	// codePkByte = pvm_pkbyte((char *)&unPoint,sizeof(point),1);
	// codeSend = pvm_send(child,1);
	// printf("Code Spawn : %d, Code PkByte : %d; Code Send : %d\n",codeSpawn,codePkByte,codeSend);
	// printf("Point Envoyé : {%d,%d}\n",unPoint.x,unPoint.y);

	// point pointTest;
	// pvm_recv(child,1);
	// pvm_upkbyte((char*)&pointTest,sizeof(point),1);
	// printf("Point Reçu : {%d,%d}\n",pointTest.x,pointTest.y);
	
	// point_print_gnuplot(pts, 0); /* affiche l'ensemble des points */
	// upper_hull(pts);
	// point_print_gnuplot(pts, 1); /* affiche l'ensemble des points restant, i.e l'enveloppe, en reliant les points */

	point_free(pts);
	pvm_exit();
}
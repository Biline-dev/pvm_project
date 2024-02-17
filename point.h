#pragma once
/*
 * upper.h
 * SR 		13/04/94
 * LL 		14/10/97
 *
 *  Definition du programme upper
 */

#define XMAX (1024)
#define YMAX (1024)


/* 
 * structure associee a chaque point 
 */
typedef struct st_point point;
struct st_point {
	int x, y;
	point *next;	/* liste chainee des points de l'enveloppe */
};

/*
 * dans point.c
 * utilitaire de calcul pour le TAD point
 */
extern point *point_alloc();
extern void point_free();
extern void point_print();
extern void point_print_gnuplot();
extern point *point_random();
extern point *point_UH();
extern int point_nb();
extern point *point_part();
extern point *point_merge_UH();
extern void upper_hull();
extern point *list_to_array(point *pts);
extern point *array_to_list(point *pts, int nbPoints);
extern void print_point_array(point *pts, int nbPoints);
extern void print_point_list(point *pts);
extern void send_points_array(point * tableau_point,int nb_point, int tid);
extern point * receive_points_array(int * nb_point, int tid);
extern void send_points_liste(point * liste_point, int tid);
extern point * receive_points_liste(int tid);

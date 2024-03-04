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
#define pb_calcul 1
#define pb_merge 2
#define pb_quit 3

/* 
 * structure associee a chaque point 
 */
typedef struct st_point point;
struct st_point {
	int x, y;
	point *next;	/* liste chainee des points de l'enveloppe */
};

struct st_pb
{
	//Type 1 : point_UH, 2 : point_merge_UH, -1 : Terminer le programme
	int type;
	point * data1;
	point * data2;
};

typedef struct st_pb pb_t;

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
extern void send_pb(pb_t * pb, int tid);
extern pb_t * receive_pb(int tid, int * sender);
extern void print_pb(pb_t * pb);
extern point * ajouter_point_fin_liste(point * liste, point * pts);
extern pb_t * alloc_pb();
extern point * get_last_point(point *  pts);
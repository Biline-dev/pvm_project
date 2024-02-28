#include <stdio.h>
#include <stdlib.h>

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
 * (de)allocation
 */

//Affiche une liste de point (Plus joli que point_print)
void print_point_list(point * pts)
{
	if(pts == NULL)
	{
		printf("NULL");
	}
	
	while (pts)
	{
		printf("(%d,%d)",pts->x,pts->y);
		if(pts->next != NULL)
			printf("->");
		pts = pts->next;
	}
	
	printf("\n");
}

point *point_alloc()
{
	point *pt;
	pt = (point *)malloc(sizeof(point));
	pt->x = pt->y = 0;
	pt->next = NULL;
	return pt;
}

/*
 * dealloue une liste de points
 */
void point_free(point *pts)
{
	if (!pts) return;
	point_free(pts->next);
	free(pts);
}

/*
 * genere une liste de points, avec ligne si solid == 1
 * generation au format jgraph
 */

void point_print(point *pts, int solid)
	/* solid: enveloppe ou non */
{
	if (!solid) {
		printf("newgraph\nlegend top\n");
	}

	printf("newcurve pts\n");
	while (pts) {
		printf("%d %d\n",pts->x, pts->y);
		pts=pts->next;
	}
	if (solid)
		printf("linetype solid\ntitle\n");
}

/*
 * nombre de points dans une liste
 */
int point_nb(point *pts)
{
	int nb;
	point *cur;

	for (cur=pts, nb=0; cur != NULL; cur = cur->next, nb++);

	return nb;
}

/* pente definie par 2 points */
#define PENTE(pt1,pt2) ( (float)((pt2)->y - (pt1)->y) / (float)((pt2)->x-(pt1)->x) )


/* 
   Calcule l'enveloppe convexe haute de 4 points au plus.
   Les points de l'enveloppe sont lies par une liste
   Les autres sont elimines de la liste.
 */

point *point_UH(point *pts) /* ensemble des points a` "envelopper" */
{
	int nb;
	point *cur, *tmp;

	nb = point_nb(pts);
	if (nb < 3) return pts;
	if (nb > 4) return NULL;

	if (nb == 3) {
		if (PENTE(pts, pts->next) < PENTE(pts, pts->next->next)) {
			tmp = pts->next;
			pts->next = tmp->next;
			free(tmp);
		}
		return pts;
	}

	cur = pts;
	if ( ( PENTE(cur, cur->next) < PENTE(cur, cur->next->next) ) ||
			( PENTE(cur, cur->next) < PENTE(cur, cur->next->next->next) ) ) {
		/* le deuxieme n'est pas sur l'enveloppe */
		tmp = cur->next;
		cur->next = tmp->next;
		free(tmp);
	}
	else
		cur = cur->next;

	if ( PENTE(cur, cur->next) < PENTE(cur, cur->next->next) ) {
		/* le troisieme point n'est pas sur l'enveloppe */
		tmp = cur->next;
		cur->next = tmp->next;
		free(tmp);
	}

	return pts;
}

/*
 * retourne 1 si la droite (pt1,pt2) est tangente
 * a l'enveloppe pts
 * (pt2 appartient a pts)
 */
int point_is_tangent(point *pt1, point *pt2, point *pts)
{
	point *next, *prev;
	float pente;

	for (prev=pts; prev!=NULL; prev=prev->next)
		if (prev->next == pt2) break;
	next = pt2->next;

	pente = PENTE(pt1, pt2);

	return 	((prev ? PENTE(prev,pt2)>=pente : 1) && 
			(next ? PENTE(pt2,next)<=pente : 1)) ? 1 : 0;
}

point *point_tangent(point *pt, point *pts)
{
	point *pt2;

	for (pt2=pts; pt2!=NULL; pt2=pt2->next)
		if (point_is_tangent(pt, pt2, pts)) return pt2;

	if (pt2==NULL) {
		printf("Warning : no tangent : \n");
		point_print(pt, 0);
		point_print(pts, 1);
		exit(-1);
	}
	return NULL;
}

/*
 * retourne dans ppt1 et ppt2 les points de tangence
 * de la tangeante commune des deux enveloppes pts1 et pts2
 */
void point_common_tangent(point *pts1, point *pts2, point **ppt1, point **ppt2)
{
	point *pt1, *pt2;

	for (pt1=pts1; pt1!=NULL; pt1=pt1->next) {
		pt2 = point_tangent(pt1, pts2);
		if (point_is_tangent(pt2, pt1, pts1))
			break;
	}

	*ppt1 = pt1;
	*ppt2 = pt2;
}

/*
 * calcule l'enveloppe convexe commune de 
 * deux enveloppes convexes
 */
point *point_merge_UH(point *pts1, point *pts2)
{
	point *pt1, *pt2, *cur, *tmp;

	point_common_tangent(pts1, pts2, &pt1, &pt2);
	point_free(pt1->next);
	for (cur=pts2; cur!=pt2; cur=tmp) {
		tmp = cur->next;
		free(cur);
	}
	pt1->next = pt2;
	return pts1;
}

point * ajouter_point_fin_liste(point * liste, point * pts)
{
	if(liste == NULL)
	{
		liste = point_alloc();
		liste->x = pts->x;
		liste->y = pts->y;
		return liste;
	}
	else
	{
		point *current = liste;
        while (current->next != NULL)
		{
            current = current->next;
        }
        // À ce stade, current est le dernier point de la liste
        current->next = point_alloc();
        current->next->x = pts->x;
        current->next->y = pts->y;
        return liste;
	}
}

void print_pb(pb_t * pb)
{
	printf("Pb { \nType : %d\n",pb->type);
	printf("Data 1 : ");
	print_point_list(pb->data1);
	printf("Data 2 : ");
	print_point_list(pb->data2);
	printf("}\n");
}

int main(int argc, char const *argv[])
{
    // pb_t pb1;
    // point * test1;
	// point * test2;
    // point * resultat;
	// point p1,p2,p3,p4,p5;
	// p1.x = 563; p1.y = 927;
	// p2.x = 886; p2.y = 346;
	// p3.x = 359; p3.y = 966;
	// p4.x = 487; p4.y = 909;
	// p5.x = 498; p5.y = 763;
    // test1 = ajouter_point_fin_liste(test1,&p1);
    // test1 = ajouter_point_fin_liste(test1,&p2);
    // test2 = ajouter_point_fin_liste(test2,&p3);
    // test2 = ajouter_point_fin_liste(test2,&p4);
    // test2 = ajouter_point_fin_liste(test2,&p5);
    // print_point_list(test1);
    // print_point_list(test2);
    // pb1.data1 = test1;
    // pb1.data2 = test2;
    // print_pb(&pb1);
    // resultat = point_merge_UH(pb1.data1,pb1.data2);
    // print_point_list(resultat);
    // return 0;

	pb_t pb1;
    point * test1;
	point * test2;
    point * resultat;
	point p1,p2,p3,p4,p5,p6,p7;

	p1.x = 489; p1.y = 62;
	p2.x = 498; p2.y = 763;
	p3.x = 561; p3.y = 88;

	p4.x = 302; p4.y = 611;
	p5.x = 359; p5.y = 966;
	p6.x = 795; p6.y = 488;
	p7.x = 886; p7.y = 346;

    test1 = ajouter_point_fin_liste(test1,&p1);
    test1 = ajouter_point_fin_liste(test1,&p2);
    test1 = ajouter_point_fin_liste(test1,&p3);
    test2 = ajouter_point_fin_liste(test2,&p4);
    test2 = ajouter_point_fin_liste(test2,&p5);
	test2 = ajouter_point_fin_liste(test2,&p6);
    test2 = ajouter_point_fin_liste(test2,&p7);
    print_point_list(test1);
    print_point_list(test2);
    pb1.data1 = test1;
    pb1.data2 = test2;
    print_pb(&pb1);
    resultat = point_merge_UH(pb1.data1,pb1.data2);
    print_point_list(resultat);
    return 0;
}

/*
 *  point.c    
 *  SR 		13/04/94
 *  LL      13/03/18
 *
 *  Fonctions utiles dans le calcul de la
 *  enveloppe convexe
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include "point.h"
#include "pvm3.h"

/*
 * (de)allocation
 */

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
   static int compareX(i,j)

   comparaison des abscisses de 2 points
   pour la fonction qsort
 */

static int compareX(point **i, point **j)
{ return (*i)->x - (*j)->x; }

/* 
   Construit un ensemble de points d'abscisse comprise
   entre 0 et XMAX et d'ordonnee entre 0 et YMAX. 
   Tous les points ont des abscisses et des ordonnees 
   differentes. Les points sont trie's par ordre croissant
   des abscisses.
 */

point *point_random(int nbPts)/* nombre de points a` tirer */
{
	int i,j;
	point **pts;

	pts = (point **)malloc(nbPts*sizeof(point *));
	for(i=0; i < nbPts; i++) {
		pts[i] = point_alloc();
		do {
			pts[i]->x = rand() % XMAX;
			for(j=0; j<i && pts[i]->x!=pts[j]->x; j++);
		} while( j != i );
		do {
			pts[i]->y = rand() % YMAX;
			for(j=0; j<i && pts[i]->y!=pts[j]->y; j++);
		} while( j != i );
	}

	qsort(pts, nbPts, sizeof(point *), (__compar_fn_t) compareX);
	for (i=0; i<nbPts-1; i++)
		pts[i]->next = pts[i+1];

	return (point *)*pts;
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

// au format gnuplot, dans 2 fichiers.
// appele ensuite gnuplot 
void point_print_gnuplot(point *pts, int solid)
	/* solid: enveloppe ou non */
{
	FILE *fp;
	if (!solid) 
		fp = fopen("points_gnuplot", "w");
	else
		fp = fopen("uh_gnuplot", "w");
	while (pts) {
		fprintf(fp, "%d %d\n",pts->x, pts->y);
		pts=pts->next;
	}
	fclose(fp);
	if (solid) {
		fp = fopen("cmd_gnuplot", "w");


		fprintf(fp, "set terminal pdf\n");
		fprintf(fp, "set output \"upper_hull.pdf\"\n");
		fprintf(fp, "plot \"points_gnuplot\" ps 2, \"uh_gnuplot\" with lines lw 4\n");

		fclose(fp);
		system("gnuplot cmd_gnuplot");
		sleep(1);
		//system("ps2pdf upper_hull.ps");
		system("rm cmd_gnuplot points_gnuplot uh_gnuplot");
		//system("rm cmd_gnuplot points_gnuplot uh_gnuplot upper_hull.ps");
		printf ("courbe dans upper_hull.pdf\n");
		printf ("evince upper_hull.pdf\n");
	}
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

/*
 * partitionne une liste en deux.
 * retourne le milieu de la liste 
 */

point *point_part(point *pts)
{
	int next;
	point *cur, *mid, *midpred;

	midpred = NULL;
	for (mid=cur=pts, next=0; cur!=NULL; cur = cur->next, next=1-next)
		if (next) {
			midpred = mid;
			mid = mid->next;
		}
	if (midpred)
		midpred->next = NULL;
	return mid;
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

//Affiche un tableau de point
void print_point_array(point * pts, int nbPoints)
{
	for (size_t i = 0; i < nbPoints; i++)
	{
		printf("[%d,%d]",pts[i].x,pts[i].y);
	}
	printf("\n");
}

//Affiche une liste de point (Plus joli que point_print)
void print_point_list(point * pts)
{
	while (pts)
	{
		printf("(%d,%d)",pts->x,pts->y);
		if(pts->next != NULL)
			printf("->");
		pts = pts->next;
	}
	
	printf("\n");
}

//Transforme une liste en tableau
//Retourne un pointeur sur le tableau
point * list_to_array(point * pts)
{
	int nbPoints = point_nb(pts);
	point * result = (point *) malloc(sizeof(point) * nbPoints);
	for (size_t i = 0; i < nbPoints; i++)
	{
		result[i].x = pts->x;
		result[i].y = pts->y;
		pts = pts->next;
	}
	return result;
}


//Tranforme un tableau de point en liste
//Retourne le premier point de la liste
point * array_to_list(point * pts, int nbPoints)
{
	point * result = point_alloc();
	point * temp = result;
	for (int i = 0; i < nbPoints ; i++)
	{
		temp->x = pts[i].x;
		temp->y = pts[i].y;
		if(i < (nbPoints - 1))
		{
			temp->next = point_alloc();
			temp = temp->next;
		}
	}

	return result;
}

/*
* Envoie un tableau de point à un processus pvm
* tableau_point : pointeur sur le tableau de point
* nb_point : nombre de point dans le tableau
* tid : id du processus pvm qui recevra le tableau
*/
void send_points_array(point * tableau_point,int nb_point, int tid)
{
	pvm_initsend(PvmDataDefault);
	pvm_pkint(&nb_point,1,1);
	pvm_pkbyte((char *)tableau_point,sizeof(point) * nb_point,1);
	pvm_send(tid,1);
}

/*
* Reçoie un tableau de point d'un processus pvm
* nb_point : recupère le nombre de point du tableau reçu
* tid : id du processus pvm qui envoi le tableau
*/
point * receive_points_array(int * nb_point, int tid)
{
	pvm_recv(tid,1);
	pvm_upkint(nb_point,1,1);
	point * new_pts = malloc(sizeof(point) * (*nb_point));
	pvm_upkbyte((char*)new_pts,sizeof(point) * (*nb_point),1);

	return new_pts;
}

/*
* Envoie une liste de point à un processus pvm
* liste_point : pointeur sur la liste de point
* tid : id du processus pvm qui recevra la liste
*/
void send_points_liste(point * liste_point, int tid)
{
	int nb_point = point_nb(liste_point);
	point * tableau_point = list_to_array(liste_point);
	send_points_array(tableau_point,nb_point,tid);
}

/*
* Reçoie une liste de point d'un processus pvm
* tid : id du processus pvm qui envoi la liste
*/
point * receive_points_liste(int tid)
{
	int nb_point;
	point * new_pts = receive_points_array(&nb_point,tid);
	new_pts = array_to_list(new_pts,nb_point);
	return new_pts;
}

void send_pb(pb_t * pb, int tid)
{
	pvm_initsend(PvmDataDefault);
	pvm_pkint(&pb->type,1,1);
	pvm_send(tid,1);
	send_points_liste(pb->data1,tid);
	if (pb->type == 1)
	{
		send_points_liste(pb->data2,tid);	
	}
}

pb_t * receive_pb(int tid, int * sender)
{
	pb_t * pb = (pb_t *) malloc(sizeof(pb_t));
	int bufid, bytes_received, info_tag, info_tid;
	bufid = pvm_recv(tid,1);
	pvm_bufinfo(bufid, &bytes_received, &info_tag, &info_tid);
	*sender = info_tid;
	pvm_upkint(&pb->type,1,1);
	pb->data1 = receive_points_liste(tid);
	if(pb->type == 1)
	{
		pb->data2 = receive_points_liste(tid);
	}
	return pb;
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
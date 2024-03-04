#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "point.h"
#include "pvm3.h"

point * merge_list(point * pts1, point * pts2)
{
    point * result = point_alloc();

    if(pts1 == NULL || pts2 == NULL)
    {
        printf("Fusion Impossible, une des listes est NULL\n");
        exit(-1);
    }

    if(pts1->x > pts2->x)
    {
        *result = *pts2;
        pts2 = pts2->next;
    }
    else
    {
        *result = *pts1;
        pts1 = pts1->next;
    }

    point * temp =  result;

    while(pts1 != NULL || pts2 != NULL)
    {
        temp->next = point_alloc();
        temp = temp->next;
        if(pts1 != NULL && pts2 != NULL)
        {
            if(pts1->x > pts2->x)
            {
                *temp = *pts2;
                pts2 = pts2->next;
            }
            else
            {
                *temp = *pts1;
                pts1 = pts1->next;
            }
        }
        else
        {
            if(pts1 == NULL)
            {
                *temp = *pts2;
                pts2 = pts2->next;
            }
            else
            {
                *temp = *pts1;
                pts1 = pts1->next;
            }
        }
    }

    return result;

}

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

int main(int argc, char **argv)
{
    setvbuf(stdout, NULL, _IONBF, 0);  // Pas de tampon
    int parentNode = pvm_parent();
    pb_t * pb;
    point * resultat;
    int sender;
    
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

    while (1)
    {
        printf("En attente de reception d'un probleme\n");
        pb = receive_pb(parentNode,&sender);
        printf("\nProblème reçu : ");
        print_pb(pb);
        
        if(pb->type == pb_quit)
        {
            pvm_exit();
            exit(0);
        }

        if(pb->type == pb_calcul)
        {
            resultat = point_UH(pb->data1);
            pb->data1 = resultat;
            printf("\nProblème de truc traité : ");
            print_pb(pb);
        }
        else
        {
            printf("Problème de type 2 reçu\n");
            resultat = merge_list(pb->data1,pb->data2);
            upper_hull(resultat);
            pb->data1 = resultat;
            pb->data2 = NULL;
            printf("\nProblème de fusion traité : ");
            print_pb(pb);
        }
        printf("Envoi du problème au maitre\n");
        send_pb(pb,parentNode);
        printf("Problème envoyé au maitre\n");
    }
}
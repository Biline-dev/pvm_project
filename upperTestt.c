/* TP algorithmique parallele
 * maitrise
 * LL
 * 13/03/2018
 * utilise gnuplot au lieu de jgraph
 */

#include <stdio.h>
#include <stdlib.h>
#include "point.h"
#include "pvm3.h" 

#define NB_SLAVE 4

static pb_t ** pile;
static int nbPb, index_pile = 0;

/*
 * empile ou depile des problèmes dans la pile
 */

pb_t *depile()
{
	if (index_pile > 0)
		return pile[--index_pile];
	else
		return NULL;
}

void empile(pb_t * pb)
{
	pile[index_pile++] = pb;
}

void init_pile(point *pts)
{
    if (pts == NULL)
	{
        printf("Aucun point fourni \n");
        return;
    }

    int nbPoint = point_nb(pts);
	nbPb = nbPoint / 4;
	if (nbPoint % 4 > 0)
		nbPb++;


    pile = (pb_t**)malloc(sizeof(pb_t*) * nbPb);
    if (pile == NULL)
	{
        perror("Echec de l'allocation de la pile");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nbPb; i++)
	{
        pb_t *pb = (pb_t*)malloc(sizeof(pb_t));
        if (pb == NULL) //Si l'allocation échoue libérer les ressources et quitter le programme
		{
            perror("Echec de l'allocation du probleme");
            for (int j = 0; j < i; j++)
			{
                free(pile[j]);
            }
            free(pile);
            exit(EXIT_FAILURE);
        }

        pb->data1 = NULL;
		pb->data2 = NULL;
		pb->type = 1;

        for (int j = 0; j < 4 && pts != NULL; j++)
		{
            pb->data1 = ajouter_point_fin_liste(pb->data1, pts);
            pts = pts->next;
        }

        empile(pb);
    }
}

void print_pile()
{
    for (size_t i = 0; i < index_pile; i++)
	{
        print_pb(pile[i]);
    }
}

int main(int argc, char **argv)
{
	pvm_catchout(stdout);
	int tid[NB_SLAVE];
	int test;

	pvm_spawn("/home/ivan/Documents/pvm/pvm_project/slave",NULL,0,NULL,NB_SLAVE,tid);

	pvm_recv(-1,1);
	pvm_upkint(&test,1,1);
	printf("Ceci est le nombre reçu ! %d",test);
	pvm_recv(-1,1);
	pvm_upkint(&test,1,1);
	printf("Ceci est le nombre reçu ! %d",test);
	pvm_recv(-1,1);
	pvm_upkint(&test,1,1);
	printf("Ceci est le nombre reçu ! %d",test);
	pvm_recv(-1,1);
	pvm_upkint(&test,1,1);
	printf("Ceci est le nombre reçu ! %d",test);	

	// while(1)
	// {
	// 	if((pb = depile()) != NULL)
	// 	{
	// 		send_pb(pb,tid[index_slave]);
	// 		index_slave++;
	// 		if(index_slave == NB_SLAVE)
	// 		{
	// 			index_slave = 0;
	// 		}
	// 	}
	// }
	
	pvm_exit();
	exit(0);
}
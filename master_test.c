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
	setvbuf(stdout, NULL, _IONBF, 0);  // Pas de tampon
	pvm_catchout(stdout);
	int tid;

	pvm_spawn("/home/ivan/Documents/pvm/pvm_project/test_slave",NULL,0,NULL,1,&tid);

	//PB 1
	pb_t pb;
	pb.type = 1;
	pb.data1 = point_random(3);
	pb.data2 = point_random(3);
	print_pb(&pb);
	send_pb(&pb,tid);

	//PB 2
	pb_t pb2;
	pb2.type = 1;
	pb2.data2 = point_random(6);
	print_pb(&pb2);
	send_pb(&pb2,tid);

	//PB 3
	pb_t pb3;
	pb3.type = 1;
	pb3.data1 = point_random(6);
	print_pb(&pb3);
	send_pb(&pb3,tid);

	//PB 4
	pb_t pb4;
	pb4.type = 1;
	print_pb(&pb4);
	send_pb(&pb4,tid);

	pvm_exit();
	return 0;
}
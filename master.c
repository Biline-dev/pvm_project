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
	point *pts;
	pb_t * pb;
	int sender;
	int nbPoint = 0, nbPbEnvoye = 0, nbPbRecu = 0, nbBoucle = 0;
	int tid[NB_SLAVE];
	if (argc != 2) 
	{
		fprintf(stderr, "usage: %s <nb points>\n", *argv);
		exit(-1);
	}

	pts = point_random(atoi(argv[1]));
	point_print_gnuplot(pts, 0); /* affiche l'ensemble des points */

	//Récupération du nombre de points
	nbPoint = point_nb(pts);

	//Calcul du nombre de problème
	nbPb = nbPoint / 4;
	if (nbPoint % 4 > 0)
		nbPb++;

	//Estimation du nombre de tour de boucle nécessaire
    int impair;
	int tempNbPb = nbPb;
	nbBoucle = nbPb;
    while (tempNbPb > 1)
    {
        impair = tempNbPb % 2;
        tempNbPb = tempNbPb / 2;
        nbBoucle = nbBoucle + tempNbPb;
        if(impair)
            nbBoucle++;
    }
	printf("Le nombre de tour de boucle normalement est : %d\n",nbBoucle);

	//Création des problèmes et placement des problèmes dans la pile
	init_pile(pts);
	print_pile();

	//Créations des processus esclaves
	pvm_spawn("/home/ivan/Documents/pvm/pvm_project/slave",NULL,0,NULL,NB_SLAVE,tid);

	//Envoie un premier problème à chaque fils
	for (int i = 0; i < NB_SLAVE; i++)
	{
		pb = depile();
		send_pb(pb,tid[i]);
		nbPbEnvoye++;
	}

	printf("Les pb ont été envoyé\n");


	//for (int i = 0; i < nbBoucle; i++)
	while(1)
	{
		printf("En attente de reception d'un probleme\n");
		pb = receive_pb(-1, &sender);
		nbPbRecu++;
		pb->type = 2;
		printf("\nProblème reçu N°%d : \n",nbPbRecu);
		print_pb(pb);
		empile(pb);

		pb = depile();

		if (pb->type == 1)
		{
			send_pb(pb,sender);
			nbPbEnvoye++;
			printf("\nProblème envoyé : \n");
			print_pb(pb);
		}
		else
		{
			pb_t *pb2 = depile();

			if (pb2 == NULL)
			{ 
				empile(pb);
				printf("\nUn seul probleme de fusion dans la pile on ne fait rien\n");
			}
			else
			{
				if (pb2->type == 1)
				{
					send_pb(pb2,sender);
					nbPbEnvoye++;
					printf("\nProblème envoyé : \n");
					print_pb(pb2);
					empile(pb);
				}
				else
				{
					pb->data2 = pb2->data1;
					send_pb(pb,sender);
					nbPbEnvoye++;
					printf("\nProblème envoyé : \n");
					print_pb(pb);
					free(pb2);
				}
			}
		}
		printf("Nombre de problem recu : %d\n",nbPbEnvoye);
		//printf("Boucle n°%d sur %d\n",i,nbBoucle);
	}

	// for (int i = 0; i < NB_SLAVE; i++)
	// {
	// 	pb_t * pbquit = alloc_pb();
	// 	pbquit->type = 3;
	// 	send_pb(pb,tid[i]);
	// 	nbPbEnvoye++;
	// }

	printf("Nombre de problem recu : %d\n",nbPbEnvoye);

	pb = depile();
	print_pb(pb);
	point_print_gnuplot(pb->data1, 1);
	pvm_exit();
	exit(0);
}
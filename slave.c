#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "point.h"
#include "pvm3.h"

int main(int argc, char **argv)
{
    setvbuf(stdout, NULL, _IONBF, 0);  // Pas de tampon
    int parentNode = pvm_parent();
    pb_t * pb;
    point * resultat;
    int sender;
    

    while (1)
    {
        printf("En attente de reception d'un probleme\n");
        pb = receive_pb(parentNode,&sender);
        printf("\nProblème reçu : ");
        print_pb(pb);
        
        if(pb->type == 3)
        {
            pvm_exit();
            exit(0);
        }

        if(pb->type == 1)
        {
            resultat = point_UH(pb->data1);
            pb->data1 = resultat;
            printf("\nProblème de truc traité : ");
            print_pb(pb);
        }
        else
        {
            printf("Problème de type 2 reçu\n");
            if(pb->data1->x > pb->data2->x)
            {
                printf("Inveserion\n");
                resultat = point_merge_UH(pb->data2,pb->data1);
            }
            else
            {
                printf("Pas d'inversion");
                resultat = point_merge_UH(pb->data1,pb->data2);
            }
            if(resultat == NULL)
            {
                printf("Test\n");
                pvm_exit();
                exit(0);
            }
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
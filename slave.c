#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "point.h"
#include "pvm3.h"

int main(int argc, char **argv)
{
    int parentNode = pvm_parent();
    pb_t * pb;
    point * resultat;
    int sender;

    // while (1)
    // {
    //     pb = receive_pb(parentNode,&sender);   
    //     if(pb->type == 1)
    //     {
    //         resultat = point_UH(pb->data1);
    //         pb->data1 = resultat;
    //     }
    //     if(pb->type == 2)
    //     {
    //         resultat = point_merge_UH(pb->data1,pb->data2);
    //         pb->data1 = resultat;
    //     }
    //     send_pb(pb,parentNode);
    //     fflush(stdout);
    // }

    pb = receive_pb(parentNode,&sender);   
    print_pb(pb);
    if(pb->type == 1)
    {
        resultat = point_UH(pb->data1);
        pb->data1 = resultat;
    }
    if(pb->type == 2)
    {
        resultat = point_merge_UH(pb->data1,pb->data2);
        pb->data1 = resultat;
    }
    send_pb(pb,parentNode);
    fflush(stdout);
    return 0;
}

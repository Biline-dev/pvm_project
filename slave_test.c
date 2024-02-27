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
    int sender;

    pb = receive_pb(parentNode,&sender);
    print_pb(pb);

    pb = receive_pb(parentNode,&sender);
    print_pb(pb);

    pb = receive_pb(parentNode,&sender);
    print_pb(pb);

    pb = receive_pb(parentNode,&sender);
    print_pb(pb);

    pvm_exit();
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "point.h"
#include "pvm3.h"

int main(int argc, char **argv)
{
    int parentNode = pvm_parent();

    point *pts;
    pts = receive_points_liste(parentNode);

    point * temp = pts;
    while (temp != NULL)
    {
        temp->x = temp->x * 2;
        temp->x = temp->x * 2;
        temp = temp->next;
    }

    send_points_liste(pts,parentNode);
    pvm_exit();

    return 0;
}

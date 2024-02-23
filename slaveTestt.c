#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "point.h"
#include "pvm3.h"

int main(int argc, char **argv)
{
    printf("Test\n");
    int parentNode = pvm_parent();
    int test = 8;
    pvm_initsend(PvmDataDefault);
    pvm_pkint(&test,1,1);
	pvm_send(parentNode,1);
}

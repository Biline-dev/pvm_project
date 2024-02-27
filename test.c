#include <stdio.h>

int main(int argc, char const *argv[])
{
    int nbPb = 16;
    int result = 0;
    int impair;
    while (nbPb > 1)
    {
        impair = nbPb % 2;
        nbPb = nbPb / 2;
        result = result + nbPb;
        if(impair)
            result++;
    }
    printf("Nb boucle : %d\n",result);
    return 0;
}

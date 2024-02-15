#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "point.h"
#include "pvm3.h"

int main(int argc, char **argv)
{
    int parentNode = pvm_parent();
    int nbPoints;
    point *pts;

    pts = receive_points_array(&nbPoints, parentNode);
    // Modifier les coordonnées des points
    for (size_t i = 0; i < nbPoints; i++)
    {
        pts[i].x = pts[i].x * 2;
        pts[i].y = pts[i].y * 2;
    }

    // Écrire le tableau de points dans un fichier texte
    FILE *file = fopen("/home/ivan/Documents/pvm/pvm_project/points_output.txt", "w");
	pvm_catchout(file);
    if (file == NULL)
    {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier.\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < nbPoints; i++)
    {
        fprintf(file, "%.2f %.2f\n", pts[i].x, pts[i].y);
    }

	printf("Test");

    fclose(file);

    // Envoyer le tableau de points modifié au parent
    send_points_array(pts, nbPoints, parentNode);
    pvm_exit();

    return 0;
}

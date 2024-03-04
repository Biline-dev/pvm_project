#include <stdio.h>
#include <stdlib.h>

/* 
 * structure associee a chaque point 
 */
typedef struct st_point point;
struct st_point {
	int x, y;
	point *next;	/* liste chainee des points de l'enveloppe */
};

point *point_alloc()
{
	point *pt;
	pt = (point *)malloc(sizeof(point));
	pt->x = pt->y = 0;
	pt->next = NULL;
	return pt;
}

point * ajouter_point_fin_liste(point * liste, point * pts)
{
	if(liste == NULL)
	{
		liste = point_alloc();
		liste->x = pts->x;
		liste->y = pts->y;
		return liste;
	}
	else
	{
		point *current = liste;
        while (current->next != NULL)
		{
            current = current->next;
        }
        // À ce stade, current est le dernier point de la liste
        current->next = point_alloc();
        current->next->x = pts->x;
        current->next->y = pts->y;
        return liste;
	}
}

//Affiche une liste de point (Plus joli que point_print)
void print_point_list(point * pts)
{
	if(pts == NULL)
	{
		printf("NULL");
	}
	
	while (pts)
	{
		printf("(%d,%d)",pts->x,pts->y);
		if(pts->next != NULL)
			printf("->");
		pts = pts->next;
	}
	
	printf("\n");
}

point * merge_list(point * pts1, point * pts2)
{
    point * result = point_alloc();

    if(pts1 == NULL || pts2 == NULL)
    {
        printf("Fusion Impossible, une des listes est NULL\n");
        exit(-1);
    }

    if(pts1->x > pts2->x)
    {
        *result = *pts2;
        pts2 = pts2->next;
    }
    else
    {
        *result = *pts1;
        pts1 = pts1->next;
    }

    point * temp =  result;

    while(pts1 != NULL || pts2 != NULL)
    {
        temp->next = point_alloc();
        temp = temp->next;
        if(pts1 != NULL && pts2 != NULL)
        {
            if(pts1->x > pts2->x)
            {
                *temp = *pts2;
                pts2 = pts2->next;
            }
            else
            {
                *temp = *pts1;
                pts1 = pts1->next;
            }
        }
        else
        {
            if(pts1 == NULL)
            {
                *temp = *pts2;
                pts2 = pts2->next;
            }
            else
            {
                *temp = *pts1;
                pts1 = pts1->next;
            }
        }
    }

    return result;

}

int main()
{
    setvbuf(stdout, NULL, _IONBF, 0);  // Pas de tampon
    point * test1 = NULL;
	point * test2 = NULL;
    point * resultat = NULL;
	point p1,p2,p3,p4,p5,p6,p7,p8,p9;

	p1.x = 124; p1.y = 706;
	p2.x = 186; p2.y = 939;
	p3.x = 201; p3.y = 922;
	p4.x = 302; p4.y = 611;

	p5.x = 74; p5.y = 236;
	p6.x = 84; p6.y = 1016;
	p7.x = 359; p7.y = 966;
	p8.x = 563; p8.y = 927;
	p9.x = 886; p9.y = 346;

    test1 = ajouter_point_fin_liste(test1,&p1);
    test1 = ajouter_point_fin_liste(test1,&p2);
    test1 = ajouter_point_fin_liste(test1,&p3);
	test1 = ajouter_point_fin_liste(test1,&p4);

    test2 = ajouter_point_fin_liste(test2,&p5);
    test2 = ajouter_point_fin_liste(test2,&p6);
	test2 = ajouter_point_fin_liste(test2,&p7);
    test2 = ajouter_point_fin_liste(test2,&p8);
	test2 = ajouter_point_fin_liste(test2,&p9);
    print_point_list(test1);
    print_point_list(test2);
    resultat = merge_list(test1,test2);
    print_point_list(resultat);
}

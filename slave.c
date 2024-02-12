#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "point.h"
#include "pvm3.h"

int main(int argc, char **argv)
{
	// point pts;
	// int parentNode = pvm_parent();
	// pvm_recv(parentNode,1);
	// pvm_upkbyte((char*)&pts,sizeof(point),1);
	// pts.x = pts.x * 2;
	// pts.y = pts.y * 2;
	// pvm_initsend(PvmDataDefault);
	// pvm_pkbyte((char*)&pts,sizeof(point),1);
	// pvm_send(parentNode,1);


	int parentNode = pvm_parent();
	int nbPoints;
	point * pts;

	pts = receive_points_array(&nbPoints,parentNode);

	// pvm_recv(parentNode,1);
	// pvm_upkint(&nbPoints,1,1);
	// point * pts = malloc(sizeof(point) * nbPoints);
	// pvm_upkbyte((char*)pts,sizeof(point) * nbPoints,1);

	for (size_t i = 0; i < nbPoints; i++)
	{
		pts[i].x = pts[i].x * 2;
		pts[i].y = pts[i].y * 2;
	}

	send_points_array(pts,nbPoints,parentNode);

	// pvm_initsend(PvmDataDefault);
	// pvm_pkbyte((char *)pts,sizeof(point) * nbPoints,1);
	// pvm_send(parentNode,1);
	
	// pvm_initsend(PvmDataDefault);
	// pvm_pkbyte((char*)&pts,sizeof(point),1);
	// pvm_send(parentNode,1);

	pvm_exit();
}
	// int child;
	// point unPoint;
	// unPoint.x = 3;
	// unPoint.y = 2;
	// int codeSpawn, codePkByte, codeSend;
	// codeSpawn = pvm_spawn("/home/ivan/Documents/pvm/TP4/UH/slave",NULL,0,NULL,1,&child);
	// pvm_initsend(PvmDataDefault);
	// codePkByte = pvm_pkbyte((char *)&unPoint,sizeof(point),1);
	// codeSend = pvm_send(child,1);
	// printf("Code Spawn : %d, Code PkByte : %d; Code Send : %d\n",codeSpawn,codePkByte,codeSend);
	// printf("Point Envoyé : {%d,%d}\n",unPoint.x,unPoint.y);

	// point pointTest;
	// pvm_recv(child,1);
	// pvm_upkbyte((char*)&pointTest,sizeof(point),1);
	// printf("Point Reçu : {%d,%d}\n",pointTest.x,pointTest.y);
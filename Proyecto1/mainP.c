/*

 Archivo: mainP.c

 Nombres:
	Alejandra Cordero / Carnet: 12-10645
	Pablo Maldonado   / Carnet: 12-10561

 Descripcion:

 Ultima modificacion: 09/05/2015

*/

// Directivas de Preprocesador:

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include "lib_procesos.h"

//----------------------------------------------------------------------------//
//                          INICIO DEL CODIGO PRINCIPAL                       //
//----------------------------------------------------------------------------//

int main(int argc, char *argv[]) {

	/*

		Definicion de la funcion: 
			Dado un entero, un archivo de entrada y una de salida,
		este programa realiza los algoritmos de map y reduce sobre el archvio de 
		entrada recibido.El resultado  de dichas operaciones se escriben en el 
		archivo de salida.

			Para realizar map el programa crea n numero de proceso en forma de
		abanico siendo n el parametro que se le pasa al programa al momento de 
		ejecucion. 
			Una vez que finalizado el map se volvera a crear n 
		procesos mas para que realicen el trabajo de reduce, una vez culminado
		dicho trabajo el programa se encargara de escribir en el archivo de salida
		todo el resultado final del trabajo realizado por los procesos.	
	
	 	Parametros de salida:

	 		- Ninguno.
	
	*/

	// Declaracion de variables:
	int status;
	int i;
	int numeroDeProcesos;
	char *archivoDeEntrada;
	char *archivoDeSalida;
	char numeroArchivo[20];
	LISTAAMIGOS *listaDeAmigos;
	listaDeAmigos = NULL;

	if (argc == 3) {

		numeroDeProcesos = 1;
		archivoDeEntrada = argv[1];
		archivoDeSalida = argv[2];

	}

	else if (argc == 5) {

		if ( strcmp(argv[1],"-n") == 0) {

			numeroDeProcesos = atoi(argv[2]);
			archivoDeEntrada = argv[3];
			archivoDeSalida = argv[4];

		}

		else {

			printf("Error: No se han pasado los argumentos de forma correcta.\n");
			exit(0);

		}

	}

	else {

		printf("Error: No se han pasado los argumentos de forma correcta .\n");
		exit(0);

	}

	// Se lee el archivo de entrada :
	LeerArchivo(archivoDeEntrada,numeroDeProcesos);


	// Se crean los proceso hijos para que realicen map a las listas:
	pid_t childpid[numeroDeProcesos];
	for(i = 0;i < numeroDeProcesos;i++){
		
		if ((childpid[i] = fork()) < 0) {
			perror("Error en el fork");
			exit(0);
		}

		if (childpid[i] == 0) {
			sprintf(numeroArchivo, "%d", i);
			if (execlp("./map",numeroArchivo,NULL) < 0) {
				perror("Fallo en la ejecucion de exec");	
			}
			
		}

	}
	
	// Se espera que los proceso hijos terminen:
	for(i=0;i<numeroDeProcesos;i++){
		wait(&status);
		
		}

	// El proceso padre lee el trabajo que realizaron los 
	//  procesos hijos en map y lo almacena en una lista enlazada:
	LeerTrabajoMap(numeroDeProcesos,&listaDeAmigos,childpid);


	//printf(" %s",listaDeAmigos->persona1);

	/*
	while (aux != NULL) {

		printf("--------------------------\n");
		printf("Persona 1: %s\n",aux->persona1);
		printf("Persona 2: %s\n",aux->persona2);
		printf("Amigos 1: %s\n",aux->amigos1);
		printf("Amigos 2: %s\n",aux->amigos2);
		printf("--------------------------\n");

		aux = aux->siguiente;

	}

	*/
	
	// El proceso padre toma la estructura creada anteriormente y reparte 	
	// el trabajo de manera equitativa en archivos para que procesos hijos 
	//puedan hacer reduce.
	ReparteTrabajoReduce(numeroDeProcesos,listaDeAmigos);

	// Se elimina la estructura listaAmigos creada:
	EliminarListaAmigos(&listaDeAmigos);

	// Se realiza fork para crear los proceso que van a realizar reduce.
	for(i = 0;i < numeroDeProcesos;i++){
		
		if ((childpid[i] = fork()) < 0) {
			perror("Error en el fork");
			exit(0);
		}

		if (childpid[i] == 0) {
			sprintf(numeroArchivo, "%d", i);
			if (execlp("./reduce",numeroArchivo,NULL) < 0) {
				perror("Fallo en la ejecucion de exec");	
			}
			
		}

	}

	// Se espera a que los proceso hijos terminen:
	for(i=0;i<numeroDeProcesos;i++){
		wait(&status);
	
		}

	// El proceso padre lee todos los archivos creados por sus hijos para
	// crear el archivo de salida:
	LeerTrabajoReduce(numeroDeProcesos,archivoDeSalida,childpid);

	

	return(0);

}

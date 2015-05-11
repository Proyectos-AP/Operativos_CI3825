/*
*
* Archivo: map.c
*
* Nombres:
*	Alejandra Cordero / Carnet: 12-10645
*	Pablo Maldonado   / Carnet: 12-10561
*
* Descripcion:
*
* Ultima modificacion: 09/05/2015
*
*/

// Directivas de Preprocesador:

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

// Definicion de tipos:


typedef struct lista {

	char *elem;
	struct lista *siguiente;

} LISTA;


// Inicio del codigo principal:

void main(int argc, char *argv[]) {

	/*
	*
	* Definicion de la funcion:	
	*
	* Parametros de entrada:
	*
	* Parametros de salida:
	*
	*/

	// Se obtiene el PID del proceso:
	int miPID = getpid();

	// Se convierte el PID en string
	char pidStr[20];
	sprintf(pidStr, "%d",miPID);

	// Se concatena el PID con ".txt"
	// para obtener el nombre del archivo de salida:
	char nombreSalida[30];
	char inicio[40];
	strcpy(inicio,pidStr);
	strcpy(nombreSalida,inicio);
	strcat(nombreSalida, ".txt");

	int i;

	printf("soy el hijo %d con %d asignaciones\n",miPID,argc);

	// Se abre el archivo de salida:

	FILE *archivoSalida;

	archivoSalida = fopen(nombreSalida,"a");


	LISTA *listaAmigos;
	LISTA *aux;
	LISTA *nueva_caja;
	LISTA *amigos;

	listaAmigos = NULL;

	for (i = 0; i < argc; i++) {

		nueva_caja = (LISTA*)malloc(sizeof(LISTA));

		nueva_caja->elem = strtok(argv[i]," ->");

		listaAmigos = nueva_caja;
		aux = nueva_caja;


		while (aux->elem != NULL) {

			printf("Soy el token: %s.\n",aux->elem);
			nueva_caja = (LISTA*)malloc(sizeof(LISTA));
			nueva_caja->elem = strtok(NULL," ");
			aux->siguiente = nueva_caja;
			aux = aux->siguiente;
		}


		aux = listaAmigos->siguiente->siguiente;

		while( aux->elem != NULL) {

			amigos = listaAmigos->siguiente->siguiente;

			fprintf(archivoSalida,"(%s %s) -> ",listaAmigos->elem,aux->elem );

			while (amigos->elem != NULL) {

				fprintf(archivoSalida,"%s ",amigos->elem);

				amigos = amigos->siguiente;

			}

			fprintf(archivoSalida,"\n");

			aux = aux->siguiente;

		}

	}


	// Se cierra el archivo de salida:
	fclose(archivoSalida);
	exit(0);

}
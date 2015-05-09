/*
*
* Archivo: main.c
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
#include <errno.h>

// Definicion de Tipos:

typedef struct lista {

	char *nombre;
	struct lista *siguiente;

} LISTA;

typedef struct listaAmigos {

	char *personas;
	LISTA *amigos1;
	LISTA *amigos2;
	struct listaAmigos *siguiente;

} LISTAAMIGOS;

// Definicion de funciones:


int lineasArchivo(char *nombre_archivo) {

	/*
	*
	* Definicion de la funcion:	
	*
	* Parametros de entrada:
	*
	* Parametros de salida:
	*
	*/

	FILE *archivo;
	char caracter;
	int lineas = 0;

	if ( (fopen(nombre_archivo,"r")) == NULL) {

		perror("Error: El archivo indicado no fue encontrado ");
		printf("errno = %d. \n",errno);
		exit(1);
	}

	else { 

		archivo = fopen(nombre_archivo,"r");

		while(!feof(archivo)) {

			caracter = fgetc(archivo);

			if (caracter == '\n') {
				lineas++;
			}

		}

	}

	return lineas; 

}


// Inicio del codigo principal:

void  main(int argc, char *argv[]) {

	/*
	*
	* Definicion de la funcion:	
	*
	* Parametros de entrada:
	*
	* Parametros de salida:
	*
	*/

	int numeroProcesos;
	char *archivoEntrada;
	char *archivoSalida;

	if (argc == 3) {

		numeroProcesos = 1;
		archivoEntrada = argv[1];
		archivoSalida = argv[2];

	}

	else if (argc == 5) {

		if ( strcmp(argv[1],"-n") == 0) {

			numeroProcesos = atoi(argv[2]);
			archivoEntrada = argv[3];
			archivoSalida = argv[4];

		}

		else {

			printf("Error: No se han pasado los argumentos de forma correcta.\n");

		}


	}

	else {
//
		printf("Error: No se han pasado los argumentos de forma correcta.\n");

	}



	int numeroLineas = lineasArchivo(archivoEntrada);

	printf("numero lineas es %d\n",numeroLineas);



	LISTA *apuntador; 

	apuntador = (LISTA*) malloc(sizeof(LISTA));

	char* nombre = (char*) malloc(sizeof(char)*10);

	apuntador->nombre = "que poco creativo soy";

	pid_t childpid;





	childpid = fork();

	if (childpid == 0) {

		execlp("./map",apuntador->nombre,NULL);

	} 

	else {
		wait();
		printf("mi hijo termino\n");
	}




}
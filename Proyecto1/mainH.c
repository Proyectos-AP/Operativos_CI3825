/*
*
* Archivo: main.h
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

// Directivas de preprocesador:

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

// Definicion de hilos:

void *hiloMap(void *arg) {
	/*
	*
	* Definicion del hilo:	
	*
	* Parametros de entrada:
	*
	* Parametros de salida:
	*
	*/
	printf("Soy el hilo map");


}

//----------------------------------------------------------------------------//

void *hiloReduce(void *arg) {

	/*
	*
	* Definicion del hilo:	
	*
	* Parametros de entrada:
	*
	* Parametros de salida:
	*
	*/




}



//----------------------------------------------------------------------------//

// Definicion de funciones:




//----------------------------------------------------------------------------//

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

	int numeroHilos;
	char *archivoEntrada;
	char *archivoSalida;

	if (argc == 3) {

		numeroHilos = 1;
		archivoEntrada = argv[1];
		archivoSalida = argv[2];

	}

	else if (argc == 5) {

		if ( strcmp(argv[1],"-n") == 0) {

			numeroHilos = atoi(argv[2]);
			archivoEntrada = argv[3];
			archivoSalida = argv[4];

		}

		else {

			printf("Error: No se han pasado los argumentos de forma correcta.\n");

		}


	}

	else {

		printf("Error: No se han pasado los argumentos de forma correcta.\n");

	}

	// Se crea la lista enlazada generica sobre la que trabajaran los hilos:

	int i;
	for (i = 0; i < numeroHilos; i++) {




	}



	// Se inicializan los hilos:


	// Se escribe la informacion en el archivo de salida:


	exit(0);
}

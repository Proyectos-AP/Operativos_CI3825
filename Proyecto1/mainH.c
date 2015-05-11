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

void LeerArchivo(char *nombre_archivo,int numeroProcesos,LISTA **Arreglo){
	
	int numeroLinea=0;
	int resto;
	char *Linea;
	FILE *archivo;
		
	// Creo arreglo de auxiliares:
	LISTA *Aux[numeroProcesos];
	
	// Creo el temporal:
	
	LISTA *temporal;
	
	if ( (fopen(nombre_archivo,"r")) == NULL) {

		perror("Error: El archivo indicado no fue encontrado ");
		printf("errno = %d. \n",errno);
		exit(1);
	}
	
	else {
		
		archivo = fopen(nombre_archivo,"r");
		
		// Inicializo las cabeceras y sus respectivos auxiliares
		int i;
		for(i=0; i < numeroProcesos; i++){
			Arreglo[i]=NULL;
			Aux[i]=NULL;
		}
		
		// Se inicializan el numeroElementos:
		
		for(i=0; i < numeroProcesos; i++){
			numeroElementos[i]=0;
		}
		
		// Empiezo a leer el archivo:
		
		while(1){
			// Se reserva el espacio de memoria para la nueva linea
			Linea = (char*)malloc(sizeof(char)*101);
			fscanf(archivo, " %[^\n]\n" ,Linea);
			//printf("%s\n",Linea);
			temporal =(LISTA*)malloc(sizeof(LISTA));
			
			temporal->nombre = Linea;
			resto = numeroLinea%numeroProcesos;
			
			// Caso en el que temporal es el primer elemento de la lista
			
			if (Aux[resto] == NULL) {
				Arreglo[resto] = temporal;
				Aux[resto] = temporal;
				numeroElementos[resto]++;
			}

			// Caso en el que temporal no es el primer elemento de la lista
			else {
				Aux[resto]->siguiente=temporal;	
				Aux[resto]=temporal;
				numeroElementos[resto]++;
			}
			numeroLinea++;
			
			// Se verifica si se ha llegado al fin del archivo
			if (feof(archivo)) {
				break;
				
				}
			}
		}
	}

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
	
	// Se lee el archivo:

	// Se crea la lista enlazada generica sobre la que trabajaran los hilos:

	int i;
	for (i = 0; i < numeroHilos; i++) {


	}



	// Se inicializan los hilos:


	// Se escribe la informacion en el archivo de salida:


	exit(0);
}

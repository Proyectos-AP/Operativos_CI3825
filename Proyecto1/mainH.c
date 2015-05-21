/*
*
* Archivo: main.h
*
* Nombres:
*	Alejandra Cordero / Carnet: 12-10645
*	Pablo Maldonado   / Carnet: 12-10561
*
* Descripcion: Programa que dada una lista
* de personas con sus respectivos amigos 
* devuelve en un archivo de salida los 
* amigos en comun existentes entre todas
* ellas. Se hace uso del modelo de 
* programacion map / reduce.
* 
*
* Ultima modificacion: 15/05/2015
*
*/

//----------------------------------------------------------------------------//
//                          DIRECTIVAS DE PREPROCESADOR                       //
//----------------------------------------------------------------------------//

#include <stdio.h>    // Uso de la entrada/salida estandar (standard I/O).
#include <unistd.h>   // Uso de hilos en POSIX.
#include <pthread.h>  // Uso de hilos en POSIX.
#include <stdlib.h>   // Uso de la funcion exit()
#include <errno.h>    // Uso de la funcion perror()
#include <string.h>   // Uso de la funcion para tokenizar strings: strtok()
#include "lib_hilos.h"
//----------------------------------------------------------------------------//
typedef struct lista {

	/* Descripcion de la estructura: Lista enlazada de strings. */
	
	char *elem;               // Apuntador a char que contendra un string.
	struct lista *siguiente;  // Apuntador al siguiente elemento de la lista.

} LISTA;

//----------------------------------------------------------------------------//

typedef struct listaAmigos {

	/* Descripcion de la estructura: Lista enlazada que se utilizara para 
	almacenar de forma global los pares de usuarios con sus respectivos amigos 
	luego de que cada hilo ejecute map. Ejemplo: Si se tiene como entrada lo 
	siguiente: Ana -> Bernardo Cristina David / Bernardo -> Ana Jose Alejandra, 
	un elemento de esta lista sera:
		ejemplo->reduced = 0
		ejemplo->listo = 1
		ejemplo->persona1 = Ana
		ejemplo->persona2 = Bernardo
		ejemplo->amigos1 = Bernardo Cristina David
		ejemplo->amigos2 = Ana Jose Alejandra
		ejemplo->siguiente = "direccion del siguiente elemento de la lista"
	 */
	int reduced;                    // Indica si se le ha hecho reduce 	
									// al elemento.

	int listo;                      // Indica si el elemento tiene ambos 
									// "amigos" ocupados.

	char *persona1;                 // Apuntador a la caja que almacena el 
									// nombre de la primera persona.

	char *persona2;                 // Apuntador a la caja que almacena el 
									// nombre de la segunda persona.

	LISTA *amigos1;                 // Apuntador a la lista enlazada que 
									// contiene los amigos de la combinacion 
									// (persona1 persona2).

	LISTA *amigos2;    	            // Apuntador a la lista enlazada que 
									// contiene los amigos de la combinacion 
									// (persona2 persona1)

	struct listaAmigos *siguiente;  // Apuntador al siguiente elemento de la 
									// lista enlazada.

} LISTAAMIGOS;

//----------------------------------------------------------------------------//

typedef struct listaMapH {

	/* Descripcion de la estructura: Estructura que utilizaran los hilos para
	hacer map a cada una de las lineas del archivo de entrada.  */

	char *persona;                // Apuntador a string que contiene el nombre 
								  // de una persona.
								  
	char *amigos;                  // Apuntador a string que contiene el nombre 
								  // de cada unos de los amigos de la "persona".
								  
	struct listaMapH *siguiente;  // Apuntador al siguiente elemento.
	
} LISTAMAPH;

//----------------------------------------------------------------------------//

typedef struct listaCabeceraMapH {

	// Descripcion de la estructura: Estructura que almacena un apuntador 
	// a cabeceras de tipo LISTAMAPH.

	LISTAMAPH *elem;                     // Apuntador a estructura de tipo LISTAMAP
	struct listaCabeceraMapH *siguiente; // Apuntador al siguiente elemento.

} LISTACABECERAMAPH;

//----------------------------------------------------------------------------//
//                      DEFINICION DE VARIABLES GLOBALES                      //
//----------------------------------------------------------------------------//

// Mutex para que los hilos escriban sobre la estructura global listaAmigosPadre.
pthread_mutex_t semaforoListaAmigos = PTHREAD_MUTEX_INITIALIZER;

// Mutex para que los hilos escriban en el archivo de salida.
pthread_mutex_t semaforoArchivoSalida = PTHREAD_MUTEX_INITIALIZER;
 
// Mutex para que los hilos escriban sobre la estructura global listaAmigosPadre.
pthread_mutex_t semaforoReduce = PTHREAD_MUTEX_INITIALIZER; 

// Estructura que almacenara apuntadores a estrucutas de tipo LISTAMAPH donde se
// encontrara el trabajo asignado a cada hilos.
LISTACABECERAMAPH *listaMapH;

// Estrucuta que almacenara el resultado de map
LISTAAMIGOS *listaAmigosPadre;

//----------------------------------------------------------------------------//
//                  DEFINICION DE FUNCIONES DE HILOS (FIRMAS)                 //
//----------------------------------------------------------------------------//

void *hiloMap(void *arg);
void *hiloReduce(void *arg);

//----------------------------------------------------------------------------//
//                    DEFINICION DE FUNCIONES DE HILOS                        //
//----------------------------------------------------------------------------//

void *hiloMap(void *arg) {
	/*
	
	 Definicion de la funcion de hilo: Funcion ejecutada por cada hilo para
		realizar el trabajo de map.
	 
	 Parametros de entrada:
		- argc : String que representa la posicion del nodo que le corresponde 
				trabajar al hilo en la estrucura LISTACABECERAMAPH.
	
	 Parametros de salida:
	 	- Ninguno.
	
	*/

	// Declaracion de variables:
	LISTACABECERAMAPH *temp1 = listaMapH; 
	LISTAMAPH *cabeceraHilo;
	LISTAMAPH *auxHilo;
	int contador = 0;
	char contadorStr[20];
	
	// Se busca en la posicion almaceada en la variable contador el apuntador
	// de la estructura que debe trabajar el hilo.
	sprintf(contadorStr, "%d", contador);
	while ( strcmp(contadorStr,arg) != 0) {

		temp1 = temp1->siguiente;
		contador++;

		sprintf(contadorStr, "%d", contador);

	}


	cabeceraHilo = temp1->elem;
	auxHilo = cabeceraHilo;

	// Se recorre toda la estructura a la que el hilo le debe hacer map.
	while (auxHilo != NULL) {

		LISTA *amigos = NULL;
		LISTA *auxAmigos = amigos;
		LISTAAMIGOS *auxCabecera;
		LISTAAMIGOS *nuevaCaja;
		
		// Se construye una lista enlazada de amigos:
		CreaListaAmigos(auxHilo,&amigos);	
		
		auxAmigos = amigos;
		
		// Se recorre la lista enlazada de amigos:
		while ( auxAmigos != NULL) {
			auxCabecera = listaAmigosPadre;

			// Caso en el que el nodo sea el primer elemento a insertar 
			//en la lista.
			if (auxCabecera == NULL) {
				
				pthread_mutex_lock(&semaforoListaAmigos);
				// Se crea un nuevo nodo:
				nuevaCaja=(LISTAAMIGOS*)malloc(sizeof(LISTAAMIGOS));
				nuevaCaja->reduced = 0;
				nuevaCaja->persona1= auxHilo->persona;
				nuevaCaja->persona2=auxAmigos->elem;
				nuevaCaja->amigos1=amigos;
				nuevaCaja->amigos2 = NULL;
				listaAmigosPadre = nuevaCaja; 
				pthread_mutex_unlock(&semaforoListaAmigos);

			}
			// Caso en el que el nodo no sea el primer elemento a insertar 
			//en la lista.
			else {

				// Se verifica redundancia:
				while (auxCabecera != NULL) {

					// Caso en el que ya se elimino la redundancia:
					if (auxCabecera->listo == 1 ) {

						if (auxCabecera->siguiente == NULL) {

							// Se crea un nuevo nodo			
							pthread_mutex_lock(&semaforoListaAmigos);
							auxCabecera = listaAmigosPadre;
							while (auxCabecera->siguiente!=NULL) {
								auxCabecera = auxCabecera->siguiente;
							}
							
							nuevaCaja=(LISTAAMIGOS*)malloc(sizeof(LISTAAMIGOS));
							nuevaCaja->reduced = 0;
							nuevaCaja->persona1= auxHilo->persona;
							nuevaCaja->persona2=auxAmigos->elem;
							nuevaCaja->amigos1=amigos;
							nuevaCaja->amigos2 = NULL;
							nuevaCaja->siguiente=NULL;
							auxCabecera->siguiente = nuevaCaja;
							pthread_mutex_unlock(&semaforoListaAmigos);
							break;

						}

						else {

							auxCabecera = auxCabecera->siguiente;
						}
					}
					// Caso en el que no se ha eliminado la redundacia:
					else {

						// Caso en donde se localiza una redundancia:
						if ( ((strcmp(auxCabecera->persona1,auxAmigos->elem)) == 0) \
						&& (strcmp(auxCabecera->persona2,auxHilo->persona) == 0) )  {

							if (auxCabecera->amigos1 == NULL) {
								pthread_mutex_lock(&semaforoListaAmigos);
								auxCabecera->amigos1 = amigos;
								auxCabecera->amigos2 = NULL;
								pthread_mutex_unlock(&semaforoListaAmigos);

							}

							else if (auxCabecera->amigos2 == NULL) {
								pthread_mutex_lock(&semaforoListaAmigos);
								auxCabecera->amigos2 = amigos;
								auxCabecera->listo = 1;
								pthread_mutex_unlock(&semaforoListaAmigos);

							}

							break;

						}
						// Caso en donde no se localiza una redundancia:
						else {

							if (auxCabecera->siguiente == NULL) {

								// Se crea un nuevo nodo							
								pthread_mutex_lock(&semaforoListaAmigos);
								while (auxCabecera->siguiente!=NULL) {
									auxCabecera = auxCabecera->siguiente;
								}	
								nuevaCaja=(LISTAAMIGOS*)malloc(sizeof(LISTAAMIGOS));
								nuevaCaja->reduced = 0;
								nuevaCaja->persona1= auxHilo->persona;
								nuevaCaja->persona2=auxAmigos->elem;
								nuevaCaja->amigos1=amigos;
								nuevaCaja->amigos2 = NULL;
								nuevaCaja->siguiente=NULL;
								auxCabecera->siguiente = nuevaCaja;
								pthread_mutex_unlock(&semaforoListaAmigos);
								break;

							}

							else {

								auxCabecera = auxCabecera->siguiente;
							}


						}

					}

				}

			}

			auxAmigos = auxAmigos->siguiente;

		}

		//EliminarEstructuraLista(&amigos);
		auxHilo = auxHilo->siguiente;

	}

	return(0);

}
//----------------------------------------------------------------------------//

void *hiloReduce(void *arg) {
	/*
	
	 Definicion del hilo:	Funcion ejecutada por cada hilo para
		realizar el trabajo de reduce.
	
	 Parametros de entrada:
	 
		-arg: String que representa el nombre del archivo de salida donde se 
				escribira todo el resultado obtenido o reduce.

	 Parametros de salida:
	
		-Ninguno.
	
	*/

	// Declaracion de variables:
	int amigosEnComun;
	FILE *archivo_salida;
	LISTAAMIGOS *auxListaPadres;
	LISTA *CabeceraLista=NULL;
	
	// Inicializacion de variables:
	auxListaPadres = listaAmigosPadre;

	// Se itera sobre la lista enazada global que posee el trabajo realizado por 
	// reduce.
	while (auxListaPadres != NULL) {

		// Caso en el que ha hecho reduce sobre la informacion guardada por el 
		// nodo:
		if (auxListaPadres->reduced == 0) {

			pthread_mutex_lock(&semaforoReduce);
			auxListaPadres->reduced = 1;
			pthread_mutex_unlock(&semaforoReduce);

			// Caso en donde dos personas no poseen amigos en comun:
			if ( auxListaPadres->listo == 0) {
				
				// Se imprime en el archivo de salida: 
				pthread_mutex_lock(&semaforoArchivoSalida);

				archivo_salida = fopen(arg,"a");
				fprintf(archivo_salida,"(%s %s) -> -None-\n",\
				auxListaPadres->persona1,auxListaPadres->persona2);
				fclose(archivo_salida);

				pthread_mutex_unlock(&semaforoArchivoSalida);

			}
			// Caso en donde dos personas pueden tener amigos en comun:
			else {
				
				// Se realiza el algoritmo de reduce:
				amigosEnComun=ReduceDeHilos(auxListaPadres,&CabeceraLista);
				
				// Se imprime en el archivo de salida: 
				pthread_mutex_lock(&semaforoArchivoSalida);
				
				archivo_salida = fopen(arg,"a");
				ImprimirEnArchivoSalida(auxListaPadres,archivo_salida,\
				CabeceraLista,amigosEnComun);
				fclose(archivo_salida);

				pthread_mutex_unlock(&semaforoArchivoSalida);

			}

		}

		auxListaPadres = auxListaPadres->siguiente;

	}

	return(0);

}

//----------------------------------------------------------------------------//
//                          INICIO DEL CODIGO PRINCIPAL                       //
//----------------------------------------------------------------------------//

int main(int argc, char *argv[]) {
	/*
	
	 Definicion de la funcion:	codigo principal. Se encarga de la creacion y 
	 manejo de los hilos que realizaran la busqueda de los amigos en comun
	 con el uso de las funciones map y reduce.
	
	 Parametros de entrada:
	 	argc : Numero de elementos pasador por consola en argv
	 	argv : Arreglo que contiene los parametros pasados por consola
			   al momento de ejecutar el programa.
	
	 Parametros de salida:
		int  : Indica culminacion  de la ejecucion del programa. 
		(0 - Exitosa / 1 - Fallida)
	
	*/

	// Declaracion de variables:
	int numeroHilos;
	int i;
	char *archivoEntrada;
	char *archivoSalida;
	FILE *archivo_Salida;

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

	// Se lee el archivo de entrada:
	LeerArchivo(archivoEntrada,numeroHilos,&listaMapH);
	
	// Se crea la lista enlazada generica sobre la que se trabajaran los hilos:
	int r[numeroHilos];
	pthread_t hilos[numeroHilos];	
	char argumentos[numeroHilos][20];

	// Se inicializan los hilos:
	for (i = 0; i < numeroHilos; i++) {

		sprintf(argumentos[i], "%d", i);

		if( (r[i] = pthread_create(&hilos[i],NULL,&hiloMap,argumentos[i])) ) {

			printf("FAILED\n");
		}

	}

	// Se elimina la estructura que se creo al leer el archivo:
	//EliminarEstructuraLista(Cabecera,numeroHilos);
	
	// Se espera a que todos los hilos terminen:
	for (i = 0; i < numeroHilos; i++) {
			pthread_join(hilos[i],NULL);
	}

	

	// Se eliminan las redundancias de la lista enlazada actualizada por 
	//los hilos:
	EliminarRedundancia(&listaAmigosPadre);
	
	
	// Se crea el archivo de salida:
	archivo_Salida = fopen(archivoSalida,"a");
	fclose(archivo_Salida);
	
	// Se crean los hilos que haran reduce y escribiran en el archivo
	// de salida:

	for (i = 0; i < numeroHilos; i++) {

		sprintf(argumentos[i], "%d", i);

		if( (r[i] = pthread_create(&hilos[i],NULL,&hiloReduce,archivoSalida)) ) {

			printf("FAILED\n");
		}

	}

	// Se espera a que todos los hilos terminen:
	for (i = 0; i < numeroHilos; i++) {
			pthread_join(hilos[i],NULL);
	}

	// Por eliminar
	archivo_Salida = fopen(archivoSalida,"a");
	fprintf(archivo_Salida,"\n\n");
	fclose(archivo_Salida);

	//EliminarListaAmigos(&listaAmigosPadre);
	printf("termine\n");

	return(0);

}

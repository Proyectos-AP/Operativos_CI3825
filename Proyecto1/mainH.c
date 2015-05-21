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

//----------------------------------------------------------------------------//
//                            DEFINICION DE ESTRUCTURAS                       //
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

	/* Descripcion de la estructura: Estructura que recibiran los hilos para
	hacer map a cada una de las lineas del archivo de entrada.  */

	char *persona;                // Apuntador a string que contiene el no
	char *amigos;                 //
	struct listaMapH *siguiente;  //
	
} LISTAMAPH;

//----------------------------------------------------------------------------//

typedef struct listaCabeceraMapH {

	// Descripcion de la estructura:

	LISTAMAPH *elem;                      //
	struct listaCabeceraMapH *siguiente;  //

} LISTACABECERAMAPH;

//----------------------------------------------------------------------------//

typedef struct listaCabeceraAmigos {

	// Descripcion de la estructura:

	LISTAAMIGOS *elem;                      //
	struct listaCabeceraAmigos *siguiente;  //

} LISTACABECERAAMIGOS;

//----------------------------------------------------------------------------//
//                      DEFINICION DE VARIABLES GLOBALES                      //
//----------------------------------------------------------------------------//

// 
pthread_mutex_t semaforoListaAmigos = PTHREAD_MUTEX_INITIALIZER;
//
pthread_mutex_t semaforoArchivoSalida = PTHREAD_MUTEX_INITIALIZER; 
//
pthread_mutex_t semaforoReduce = PTHREAD_MUTEX_INITIALIZER; 
//
LISTACABECERAMAPH *listaMapH;
//
LISTAAMIGOS *listaAmigosPadre;
//
LISTACABECERAAMIGOS *listaCabeceraAmigosPadre;


//----------------------------------------------------------------------------//
//                  DEFINICION DE FUNCIONES DE HILOS (FIRMAS)                 //
//----------------------------------------------------------------------------//

void *hiloMap(void *arg);
void *hiloReduce(void *arg);

//----------------------------------------------------------------------------//
//                     DEFINICION DE FUNCIONES DE HILOS                       //
//----------------------------------------------------------------------------//


//----------------------------------------------------------------------------//

void EliminarEstructuraLista(LISTA **Cabecera){

	/*

		Definicion de la funcion:	
			Esta funcion destruye una estructura del tipo LISTA.
	
		Parametros de entrada:

			- Cabecera : Direccion de memoria de una caberera de una estructuta 
						del tipo LISTA.
	
	 	Parametros de salida:
	 		Ninguno.
	
	*/

	// Declaracion de variables:
	LISTA *aux;
	aux = *Cabecera;


	if (*Cabecera==NULL){
		;
	}
	else{

		while(*Cabecera!=NULL){

			aux = *Cabecera;
			*Cabecera= aux->siguiente;
			//free(aux->elem);
			free(aux);

		}

	}

}

//----------------------------------------------------------------------------//

void *hiloMap(void *arg) {
	/*
	*
	* Definicion de la funcion de hilo: Cada hilo que ejecute esta funcion
	* 
	*
	* Parametros de entrada:
	*	- argc : 
	*
	* Parametros de salida:
	* 	- Ninguno.
	*
	*/

	// Declaracion de variables:


	LISTACABECERAMAPH *temp1 = listaMapH; 
	int contador = 0;
	char contadorStr[20];


	sprintf(contadorStr, "%d", contador);

	while ( strcmp(contadorStr,arg) != 0) {

		temp1 = temp1->siguiente;
		contador++;

		sprintf(contadorStr, "%d", contador);

	}

	LISTAMAPH *cabeceraHilo = temp1->elem;
	LISTAMAPH *auxHilo = cabeceraHilo;

	while (auxHilo != NULL) {

		LISTA *amigos = NULL;
		LISTA *auxAmigos = amigos;
		LISTA *nuevaCajaAmigos;
		LISTAAMIGOS *auxCabecera;
		LISTAAMIGOS *nuevaCaja;
		char *token;
		char *saveptr;

		// Se construye la lista enlazada de amigos:

		nuevaCajaAmigos = (LISTA*)malloc(sizeof(LISTA));

		token = strtok_r(auxHilo->amigos," ",&saveptr);

		if (strcmp(token,"-None-") == 0) {
			;
		}

		else {

			
			nuevaCajaAmigos->elem = token;

			amigos = nuevaCajaAmigos;
			auxAmigos = nuevaCajaAmigos;


			while (token != NULL) {

				token = strtok_r(NULL," ",&saveptr);

					if (token != NULL ){ 

						nuevaCajaAmigos = (LISTA*)malloc(sizeof(LISTA));
						nuevaCajaAmigos->elem = token;
						auxAmigos->siguiente = nuevaCajaAmigos;
						auxAmigos = auxAmigos->siguiente;

					}
			}	


			auxAmigos = amigos;

			while ( auxAmigos != NULL) {


				auxCabecera = listaAmigosPadre;

				if (auxCabecera == NULL) {
				
					nuevaCaja=(LISTAAMIGOS*)malloc(sizeof(LISTAAMIGOS));
					nuevaCaja->reduced = 0;
					nuevaCaja->persona1= auxHilo->persona;
					nuevaCaja->persona2=auxAmigos->elem;
					nuevaCaja->amigos1=amigos;
					nuevaCaja->amigos2 = NULL;

					pthread_mutex_lock(&semaforoListaAmigos);
					listaAmigosPadre = nuevaCaja; 
					pthread_mutex_unlock(&semaforoListaAmigos);

					auxCabecera = nuevaCaja;

				}


				else {

					while (auxCabecera != NULL) {

							if (auxCabecera->listo == 1 ) {

								if (auxCabecera->siguiente == NULL) {

										// Se crea un nuevo nodo
										
									nuevaCaja=(LISTAAMIGOS*)malloc(sizeof(LISTAAMIGOS));
									nuevaCaja->reduced = 0;
									nuevaCaja->persona1= auxHilo->persona;
									nuevaCaja->persona2=auxAmigos->elem;
									nuevaCaja->amigos1=amigos;
									nuevaCaja->amigos2 = NULL;
									nuevaCaja->siguiente=NULL;

									pthread_mutex_lock(&semaforoListaAmigos);
									auxCabecera = listaAmigosPadre;
									while (auxCabecera->siguiente!=NULL) {
										auxCabecera = auxCabecera->siguiente;
									}
									//printf("HICE UNA CAJA\n");
									auxCabecera->siguiente = nuevaCaja;
									pthread_mutex_unlock(&semaforoListaAmigos);


									auxCabecera = nuevaCaja;
									break;

								}

								else {

									auxCabecera = auxCabecera->siguiente;
								}
							}

							else {


								if ( ((strcmp(auxCabecera->persona1,auxAmigos->elem)) == 0) && (strcmp(auxCabecera->persona2,auxHilo->persona) == 0) )  {

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

								else {

									if (auxCabecera->siguiente == NULL) {

										// Se crea un nuevo nodo
										
										nuevaCaja=(LISTAAMIGOS*)malloc(sizeof(LISTAAMIGOS));
										nuevaCaja->reduced = 0;
										nuevaCaja->persona1= auxHilo->persona;
										nuevaCaja->persona2=auxAmigos->elem;
										nuevaCaja->amigos1=amigos;
										nuevaCaja->amigos2 = NULL;
										nuevaCaja->siguiente=NULL;

										pthread_mutex_lock(&semaforoListaAmigos);
										//printf("HICE UNA CAJA\n");
										while(auxCabecera->siguiente!=NULL){
											auxCabecera = auxCabecera->siguiente;
										}
										auxCabecera->siguiente = nuevaCaja;
										pthread_mutex_unlock(&semaforoListaAmigos);

										auxCabecera = nuevaCaja;

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

		}

		auxHilo = auxHilo->siguiente;

	}

	return(0);

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

	// Declaracion de variables:

	//printf("entre al reduce\n");


	FILE *archivo_salida;
	LISTAAMIGOS *auxListaPadres;
	LISTA *aux1;
	LISTA *aux2;

	LISTA *CabeceraLista=NULL;
	LISTA *auxLista;
	LISTA *nuevaCajaLista;
	int amigosEnComun = 0;

	auxListaPadres = listaAmigosPadre;

	while (auxListaPadres != NULL) {

		if (auxListaPadres->reduced == 0) {

			pthread_mutex_lock(&semaforoReduce);
			auxListaPadres->reduced = 1;
			pthread_mutex_unlock(&semaforoReduce);

			if ( auxListaPadres->listo == 0) {

				pthread_mutex_lock(&semaforoArchivoSalida);

				archivo_salida = fopen(arg,"a");

				fprintf(archivo_salida,"(%s %s) -> -None-\n",auxListaPadres->persona1,auxListaPadres->persona2);

				fclose(archivo_salida);

				pthread_mutex_unlock(&semaforoArchivoSalida);

			}

			else {

				// Se hace reduce:

				aux1 = auxListaPadres->amigos1;
				auxLista=CabeceraLista;
				amigosEnComun = 0;

				while (aux1 != NULL) {


						aux2 = auxListaPadres->amigos2;

						while(aux2 != NULL) {

							if (strcmp(aux1->elem,aux2->elem) == 0) {

								amigosEnComun = 1;

								if (auxLista == NULL) {

									nuevaCajaLista = (LISTA*)malloc(sizeof(LISTA));
									nuevaCajaLista->elem = aux1->elem;
									CabeceraLista = nuevaCajaLista;
									auxLista = CabeceraLista;
								}
								else{
									auxLista = CabeceraLista;
									while(auxLista->siguiente!=NULL){
										auxLista = auxLista->siguiente;
									}
									nuevaCajaLista = (LISTA*)malloc(sizeof(LISTA));
									nuevaCajaLista->elem = aux1->elem;
									auxLista->siguiente=nuevaCajaLista;

								}

								
								break;

							}

							aux2 = aux2->siguiente;


						}

						aux1 = aux1->siguiente;

					}


		
				pthread_mutex_lock(&semaforoArchivoSalida);

				archivo_salida = fopen(arg,"a");

				fprintf(archivo_salida,"(%s %s) -> ",auxListaPadres->persona1,auxListaPadres->persona2);

				if (amigosEnComun == 0) {

					fprintf(archivo_salida,"-None-");

				}

				else {

					aux1 = CabeceraLista;
					
					while (CabeceraLista != NULL) {

						aux1 = CabeceraLista;
						CabeceraLista = aux1->siguiente;

						fprintf(archivo_salida,"%s ",aux1->elem);

						free(aux1);

					}

				}

				fprintf(archivo_salida,"\n");

				fclose(archivo_salida);

				pthread_mutex_unlock(&semaforoArchivoSalida);


			}

		}

		auxListaPadres = auxListaPadres->siguiente;

	}

	//printf("sali del reduce\n");

	return(0);

}


void EliminarListaAmigos(LISTAAMIGOS **lista) {

	/*

		Definicion de la funcion:	
			Esta funcion destruye una estructura del tipo LISTAAMIGOS.

		Parametros de entrada:

			- lista : Direccion de la cabecera de una estructura del 
						tipopLISTAAMIGOS.
		
	 	Parametros de salida:
	 		Ninguno.
	
	*/

	// Declaracion de variables:
	LISTAAMIGOS *aux;

	// Caso en donde la estructura no tiene elementos.
	if(*lista==NULL){
		;
	}

	// Caso en donde la estructura tiene al menos un elemento.
	else{

		while(*lista!=NULL){

			aux=*lista;
			*lista=aux->siguiente;
			//free(aux->persona1);
			//free(aux->persona2);

			if(aux->amigos1!=NULL){
				EliminarEstructuraLista(&aux->amigos1);
			}

			if(aux->amigos2!=NULL){
				EliminarEstructuraLista(&aux->amigos2);
			}
			free(aux);
		}
	}
}

//----------------------------------------------------------------------------//






//----------------------------------------------------------------------------//
//                          DEFINICION DE FUNCIONES                           //
//----------------------------------------------------------------------------//

void LeerArchivo(char *nombre_archivo,int numeroProcesos,LISTACABECERAMAPH **CabeceraMapH) {

	/*
	*
	* Definicion del hilo:	
	*
	* Parametros de entrada:
	*
	* Parametros de salida:
	*
	*/

	// Declaracion de variables:
	
	int numeroLinea = 0;
	int Contador=0;
	int resto;
	char *Persona;
	char *Amigos;
	int i;
	FILE *archivo;
		
	// Creo arreglo de auxiliares:
	LISTAMAPH *Aux[numeroProcesos];
	
	// Creo el temporal:
	
	LISTAMAPH *temporal;
	LISTACABECERAMAPH *nuevaCajaCabecera;
	LISTACABECERAMAPH *auxTemp;

	auxTemp = *CabeceraMapH;

	

	for (i = 0; i < numeroProcesos; i++) {

		if (auxTemp == NULL) {

			nuevaCajaCabecera = (LISTACABECERAMAPH*)malloc(sizeof(LISTACABECERAMAPH));
			nuevaCajaCabecera->elem = NULL;
			*CabeceraMapH = nuevaCajaCabecera;
			auxTemp = nuevaCajaCabecera;

		}

		else {

			nuevaCajaCabecera = (LISTACABECERAMAPH*)malloc(sizeof(LISTACABECERAMAPH));
			nuevaCajaCabecera->elem = NULL;
			auxTemp->siguiente = nuevaCajaCabecera;
			auxTemp = nuevaCajaCabecera;

		}

	}


	if ( (fopen(nombre_archivo,"r")) == NULL) {

		perror("Error: El archivo indicado no fue encontrado ");
		printf("errno = %d. \n",errno);
		exit(1);
	}
	
	else {
		
		archivo = fopen(nombre_archivo,"r");

		LISTACABECERAMAPH *tmp;
		tmp = *CabeceraMapH;
		
		// Inicializo las cabeceras y sus respectivos auxiliares
		int i;

		
		for(i=0; i < numeroProcesos; i++){
			Aux[i]=tmp->elem;
			tmp = tmp->siguiente;
		}
	

		// Empiezo a leer el archivo:
		
		while(1){

			Contador = 0;
			// Se reserva el espacio de memoria para la nueva linea
			Persona = (char*)malloc(sizeof(char)*15);
			Amigos = (char*)malloc(sizeof(char)*30);
			fscanf(archivo," %[^ ->] -> %[^\n]\n" ,Persona,Amigos);

			temporal =(LISTAMAPH*)malloc(sizeof(LISTAMAPH));
			temporal->persona = Persona;
			temporal->amigos = Amigos;
			temporal->siguiente=NULL;
			resto = numeroLinea%numeroProcesos;
			tmp = *CabeceraMapH;

			while(Contador!=resto){
				tmp = tmp->siguiente;
				Contador++;
			}

			// Caso en el que temporal es el primer elemento de la lista	
			if (tmp->elem == NULL) {
				tmp->elem = temporal;
				Aux[resto] = temporal;
			}

			// Caso en el que temporal no es el primer elemento de la lista
			else {
				Aux[resto]->siguiente=temporal;	
				Aux[resto]=temporal;
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
//                          INICIO DEL CODIGO PRINCIPAL                       //
//----------------------------------------------------------------------------//

int main(int argc, char *argv[]) {
	/*
	*
	* Definicion de la funcion:	codigo principal. Se encarga de la creacion y 
	* manejo de los hilos que realizaran la busqueda de los amigos en comun
	* con el uso de las funciones map y reduce.
	*
	* Parametros de entrada:
	* 	argc : Numero de elementos pasador por consola en argv
	* 	argv : Arreglo que contiene los parametros pasados por consola
	*		   al momento de ejecutar el programa.
	*
	* Parametros de salida:
	*	int  : Indica culminacion  de la ejecucion del programa. 
	*	(0 - Exitosa / 1 - Fallida)
	*
	*/

	// Declaracion de variables:

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

	LeerArchivo(archivoEntrada,numeroHilos,&listaMapH);
	
	// Se crea la lista enlazada generica sobre la que trabajaran los hilos:
	int r[numeroHilos];
	pthread_t hilos[numeroHilos];	
	char argumentos[numeroHilos][20];


	// Se crea el archivo de salida:

	FILE *archivo_Salida;
	archivo_Salida = fopen(archivoSalida,"a");
	fclose(archivo_Salida);

	// Se inicializan los hilos:
	int i;
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

	//printf(" %s",listaDeAmigos->persona1);

	// Se eliminan las redundancias de la lista enlazada actualizada por 
	//los hilos:

	LISTAAMIGOS *aux1;
	LISTAAMIGOS *aux2;
	LISTAAMIGOS *anterior;

	aux1 = listaAmigosPadre;

	while (aux1 != NULL) {

		//printf("aqui es 3\n");
		aux2 = aux1->siguiente;
		anterior = aux1->siguiente;
		//printf("aqui es 4\n");

		while (aux2 != NULL) {

			//printf("--------------------------\n");
			//printf("Estoy comparando: %s  %s \n",aux1->persona1,aux1->persona2);
			//printf(" Con: %s  %s \n",aux2->persona1,aux2->persona2);
			//printf("--------------------------\n");

			if ( ((strcmp(aux1->persona1,aux2->persona2)) == 0) && (strcmp(aux1->persona2,aux2->persona1) == 0) ) {


				if (aux1->amigos1 == NULL) {

					aux1->amigos1 = aux2->amigos1;

				}

				else if (aux1->amigos2 == NULL) {

					aux1->amigos2 = aux2->amigos1;


				}

				aux2->siguiente = NULL;

				anterior->siguiente = aux2->siguiente;
				aux2 = anterior;


			}

			
			anterior = aux2;
			aux2 = aux2->siguiente;

		}

		//printf("pase 1\n");
		aux1 = aux1->siguiente;
		//printf("pase 2\n");
	}


	LISTAAMIGOS *aux;

	aux = listaAmigosPadre;

	
	while (aux != NULL) {

		//printf("--------------------------\n");
		//printf("Persona 1: %s\n",aux->persona1);
		//printf("Persona 2: %s\n",aux->persona2);

		LISTA *aux1;
		LISTA *aux2;

		aux1 = aux->amigos1;

		//printf("Amigos 1: ");
		while (aux1 != NULL) {

			//printf("%s ",aux1->elem);
			aux1 = aux1->siguiente;
		}

		//printf("\n");

		aux2 = aux->amigos2;

		//printf("Amigos 2: ");
		while (aux2 != NULL) {

			//printf("%s ",aux2->elem);
			aux2 = aux2->siguiente;
		}

		//printf("\n--------------------------\n");

		aux = aux->siguiente;

	}

	
	

	// Se crean los hilos que haran reduce y escribiran en el archivo
	// de salida:

	for (i = 0; i < numeroHilos; i++) {

		sprintf(argumentos[i], "%d", i);

		if( (r[i] = pthread_create(&hilos[i],NULL,&hiloReduce,archivoSalida)) ) {

			printf("FAILED\n");
		}

	}

	for (i = 0; i < numeroHilos; i++) {
			pthread_join(hilos[i],NULL);
	}

	archivo_Salida = fopen(archivoSalida,"a");
	fprintf(archivo_Salida,"\n\n");
	fclose(archivo_Salida);

	EliminarListaAmigos(&listaAmigosPadre);
	printf("termine\n");

	return(0);

}
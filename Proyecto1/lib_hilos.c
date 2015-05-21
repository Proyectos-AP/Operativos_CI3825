/*
*
* Archivo: lib_hilos.h
*
* Nombres:
*	Alejandra Cordero / Carnet: 12-10645
*	Pablo Maldonado   / Carnet: 12-10561
*
* Descripcion: Almaena las funciones utilizadas en la version de hilos

* 
*
* Ultima modificacion: 21/05/2015
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
//                        DEFINICION DE FUNCIONES                             //
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
			//free(aux);

		}

	}

}

//----------------------------------------------------------------------------//

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
			//free(aux);
		}
	}
}

//----------------------------------------------------------------------------//

void LeerArchivo(char *nombre_archivo,int numeroProcesos,LISTACABECERAMAPH **CabeceraMapH) {

	/*

		Definicion de la funcion:
			Esta funcion dado el nombre de un archivo lo lee y almacena sus lineas
		en diferentes listas enlazadas de tipo LISTAMAP que a su vez seran almacenadas
		en una estructura de tipo LISTACABECERAMAPH.

		Parametros de entrada:

			- nombre_archivo: Nombre del archivo que sera leido.
		
			- numeroHilos: Numero de hilos que se ejecutaran.
			
			- CabeceraMapH: Direccion de memoria de tipo LISTACABECERAMAPH que
							almacenara todo el trabajo realizado por a funcion.
							* 
	 	Parametros de salida:
	 
	 		Ninguno.
	
	*/

	// Declaracion de variables:
	int numeroLinea = 0;
	int Contador=0;
	int resto;
	char *Persona;
	char *Amigos;
	int i;
	FILE *archivo;
	LISTAMAPH *Aux[numeroProcesos];
	LISTAMAPH *temporal;
	LISTACABECERAMAPH *tmp;
	LISTACABECERAMAPH *nuevaCajaCabecera;
	LISTACABECERAMAPH *auxTemp;
		
	auxTemp = *CabeceraMapH;

	// Se crea la lista enlazada de cabeceras:
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

	// Se abre el archivo de entrada:
	if ( (fopen(nombre_archivo,"r")) == NULL) {

		perror("Error: El archivo indicado no fue encontrado ");
		printf("errno = %d. \n",errno);
		exit(1);
	}
	
	else {
		
		archivo = fopen(nombre_archivo,"r");

		
		tmp = *CabeceraMapH;
		
		// Se inicializan las cabeceras y sus respectivos auxiliares
		
		for(i=0; i < numeroProcesos; i++){
			Aux[i]=tmp->elem;
			tmp = tmp->siguiente;
		}
	

		// Se empieza a leer el archivo:
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

			// Se itera hasta llegar al nodo que se encuenta en la posicion
			// correspondiente al numero que almacena la variable Contador.
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
void CreaListaAmigos(LISTAMAPH *auxHilo,LISTA **amigos){
	
	
	/*

		Definicion de la funcion:
			Esta funcion dado un apuntador a una estructura LISTAMAPH donde se 
		almacena el nombre de una persona y una lista de amigos en forma de 
		string divide la misma y almacena el nombre de cada amigo en una lista 
		enlazada.

		Parametros de entrada:

			- auxHilo: Apuntador a ua estructura LISTAMAP que brindara la 
			informacion para crear la lista enlazada de amigos.
		
			- amigos: Direccion de memoria de una cabecera de tipo LISTA en 
					donde se almacenara la lista enlazada de amigos.

	 	Parametros de salida:
	 
	 		Ninguno.
	
	*/
	

	// Declaracion de variables:
	LISTA *auxAmigos = *amigos;
	LISTA *nuevaCajaAmigos;
	char *token;
	char *saveptr;

	// Se construye la lista enlazada de amigos:
	token = strtok_r(auxHilo->amigos," ",&saveptr);
	
	// Si la persona no tiene amigos no se hace nada.
	if (strcmp(token,"-None-") == 0) {
			;
	}

	else {
		// Se guarda el primer elemento de la lista
		nuevaCajaAmigos = (LISTA*)malloc(sizeof(LISTA));
		nuevaCajaAmigos->elem = token;
		*amigos = nuevaCajaAmigos;
		auxAmigos = nuevaCajaAmigos;

		// Se va dividiendo la lista de amigos
		while (token != NULL) {

			token = strtok_r(NULL," ",&saveptr);

				if (token != NULL ){ 
					// Se crea un nuevo nodo:
					nuevaCajaAmigos = (LISTA*)malloc(sizeof(LISTA));
					nuevaCajaAmigos->elem = token;
					auxAmigos->siguiente = nuevaCajaAmigos;
					auxAmigos = auxAmigos->siguiente;

				}
		}	

	}

}

//----------------------------------------------------------------------------//

void EliminarRedundancia(LISTAAMIGOS **listaAmigosPadre){
	
	/*

		Definicion de la funcion:	
			Dada la direccion de memoria de una estructura de tipo LISTAAMIGOS, 
		esta funcion elimina las redundancias entre las personas que alli se 
		encuentran. Ejemplo: si en la estructura se tiene almacenado los nombres 
		de Adriana y Pedro en un nodo y en otro nodo estan Pedro y Adriana esta 
		funcion fucionara estos dos nodos en uno solo.

		Parametros de entrada:

			- listaAmigosPadre : Direccion de memoria de la cabecera de una 
							estructura del tipopLISTAAMIGOS.
		
	 	Parametros de salida:
	 		Ninguno.
	
	*/
	
	// Declaracion de variables:
	LISTAAMIGOS *aux1;
	LISTAAMIGOS *aux2;
	LISTAAMIGOS *anterior;

	aux1 = *listaAmigosPadre;

	while (aux1 != NULL) {

		aux2 = aux1->siguiente;
		anterior = aux1->siguiente;
	
		while (aux2 != NULL) {

			// Se comparan los elementos de dos nodos
			if ( ((strcmp(aux1->persona1,aux2->persona2)) == 0) &&\
			 (strcmp(aux1->persona2,aux2->persona1) == 0) ) {

				// Caso en el que existan redundancias:
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
			// Se olvido hacer free.
			anterior = aux2;
			aux2 = aux2->siguiente;

		}

		aux1 = aux1->siguiente;

	}
	
}

//----------------------------------------------------------------------------//

int ReduceDeHilos(LISTAAMIGOS *auxListaPadres,LISTA **CabeceraLista){
	
	/*

		Definicion de la funcion:	
			Dado el apuntador a una estructura de tipo LISTAAMIGOS que esta 
		funcion crea una lista enlazada con los amigos en comun de las personas
		que estan contenidas en dicha estructura.

		Parametros de entrada:

			- auxListaPadres : Apuntador a un nodo de tipo LISTAAMIGOS.

			-CabeceraLista: Direccion de memoria de la cabecera que contendra
						la lista enlazada de los amigos en comun
		
	 	Parametros de salida:
	 		- amigosEnComun: 0 si las personas que estan contenidas en 
						auxListaPadres no poseen amigos en comun y 1 si poseen
						amigos en comun
	
	*/
	
	// Declaracion de variables:
	LISTA *nuevaCajaLista;
	LISTA *auxLista;
	LISTA *aux1;
	LISTA *aux2;
				
				
	aux1 = auxListaPadres->amigos1;
	auxLista=*CabeceraLista;
	int amigosEnComun = 0;

	while (aux1 != NULL) {
					
		aux2 = auxListaPadres->amigos2;

		while(aux2 != NULL) {

			// Se va comparando la lista de amigos1 con la lista de amigos2:
			if (strcmp(aux1->elem,aux2->elem) == 0) {

				amigosEnComun = 1;
				// Caso en que la lista enlazada este vacia.
				if (auxLista == NULL) {
	
					// Se agrega un nodo al inicio de la lista enlazada.
					nuevaCajaLista = (LISTA*)malloc(sizeof(LISTA));
					nuevaCajaLista->elem = aux1->elem;
					*CabeceraLista = nuevaCajaLista;
					auxLista = *CabeceraLista;
				}
				// Caso en que la lista enlazada no este vacia.
				else{
					
					// Se agrega un nodo al final de la lista enlazada.
					auxLista = *CabeceraLista;
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
					
	return amigosEnComun; 
}
	
//----------------------------------------------------------------------------//

void ImprimirEnArchivoSalida(LISTAAMIGOS *auxListaPadres,FILE *archivo_salida,\
							LISTA *CabeceraLista,int amigosEnComun){

	/*

		Definicion de la funcion:
			Esta funcion esribe en un archivo de salida los resultados 
			obtenidos en el reduce

		Parametros de entrada:

			- auxListaPadres: Apuntador a ua estructura LISTAAMIGOS que sera 
			utilizado para imprimir el nombre de las dos personas a las que se 
			le buscan los amigos en comun.
		
			- archivo_salida: Nombre del archivo en donde se escribira la salida

			- CabeceraLista: Apuntador a una estructura LISTA que almacena los 
							amigos en comun de dos personas.
				
			- amigosEnComun: Entero que almacena 0 si dos personas no poseen 
							amigos en comun y 1 si lo poseen.
		
	 	Parametros de salida:
	 
	 		Ninguno.
	
	*/
	
	// Declaracion de variables:
	LISTA *aux1;
	
	
	// Se escribe en el archivo de salida el nombre de las dos personas a 
	// las que se le estan buscando amigos en comun
	fprintf(archivo_salida,"(%s %s) -> ",\
	auxListaPadres->persona1,auxListaPadres->persona2);

	if (amigosEnComun == 0) {

		fprintf(archivo_salida,"-None-");

	}

	// Se imprimen los amigos en comun de las dos personas
	else {

		aux1 = CabeceraLista;
					
		while (CabeceraLista != NULL) {

			aux1 = CabeceraLista;
			CabeceraLista = aux1->siguiente;

			fprintf(archivo_salida,"%s ",aux1->elem);

			//free(aux1);

		}

	}
	
	fprintf(archivo_salida,"\n");
	
	}
	



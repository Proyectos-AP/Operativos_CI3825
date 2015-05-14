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
#include <string.h>

// Definicion de Tipos:

typedef struct lista {

	char *elem;
	struct lista *siguiente;

} LISTA;

typedef struct listaAmigos {

	int listo;
	char *persona1;
	char *persona2;
	LISTA *amigos1;
	LISTA *amigos2;
	struct listaAmigos *siguiente;

} LISTAAMIGOS;

typedef struct listaMapH {

	char *persona;
	char *amigos;
	struct listaMapH *siguiente;
	
} LISTAMAPH;

typedef struct listaCabeceraMapH {

	LISTAMAPH *elem;
	struct listaCabeceraMapH *siguiente;

} LISTACABECERAMAPH;

typedef struct listaCabeceraAmigos {

	LISTAAMIGOS *elem;
	struct listaCabeceraAmigos *siguiente;

	
} LISTACABECERAAMIGOS;

// DEFINICION DE VARIABLES GLOBALES:
int counter = 0;
void *hiloMap(void *arg);
pthread_mutex_t semaforoListaAmigos = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t semaforoArchivoSalida = PTHREAD_MUTEX_INITIALIZER; 
LISTACABECERAMAPH *listaMapH;
LISTAAMIGOS *listaAmigosPadre;
LISTACABECERAAMIGOS *listaCabeceraAmigosPadre;

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
	//printf("Soy el hilo map\n");
	//printf("recibi el argumento: %s\n",arg);

	pthread_mutex_lock(&semaforoArchivoSalida);
	counter++;
	pthread_mutex_unlock(&semaforoArchivoSalida);

	LISTACABECERAMAPH *temp1 = listaMapH; 
	int contador = 0;

	char contadorStr[20];

	sprintf(contadorStr, "%d", contador);

	while ( strcmp(contadorStr,arg) != 0) {

		temp1 = temp1->siguiente;
		contador++;

		sprintf(contadorStr, "%d", contador);

		//printf("El contador Str vale: %s\n",contadorStr);


	}

	LISTAMAPH *cabeceraHilo = temp1->elem;
	LISTAMAPH *auxHilo = cabeceraHilo;

	while (auxHilo != NULL) {


		//printf("----------------------\n");
		//printf("La persona es: %s.\n",auxHilo->persona);
		//printf("----------------------\n");

		LISTA *amigos = NULL;
		LISTA *auxAmigos = amigos;
		LISTA *nuevaCajaAmigos;
		LISTAAMIGOS *auxCabecera;
		LISTAAMIGOS *nuevaCaja;
		char *token;

		// Se construye la lista enlazada de amigos:

		nuevaCajaAmigos = (LISTA*)malloc(sizeof(LISTA));

		//printf("Los amigos son: %s.\n",auxHilo->amigos);

		token = strtok(auxHilo->amigos," ");

		nuevaCajaAmigos->elem = token;

		amigos = nuevaCajaAmigos;
		auxAmigos = nuevaCajaAmigos;


		while (token != NULL) {

			token = strtok(NULL," ");

				if (token != NULL ){ 

					//printf("Soy el token: %s.\n",auxAmigos->elem);
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
				nuevaCaja->persona1= auxHilo->persona;
				nuevaCaja->persona2=auxAmigos->elem;
				nuevaCaja->amigos1=amigos;

				pthread_mutex_lock(&semaforoListaAmigos);
				listaAmigosPadre = nuevaCaja; 
				pthread_mutex_unlock(&semaforoListaAmigos);

				auxCabecera = nuevaCaja;

			}


			else {

				while (auxCabecera != NULL) {

						if (auxCabecera->listo == 1 ) {
							auxCabecera = auxCabecera->siguiente;
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
									nuevaCaja->persona1= auxHilo->persona;
									nuevaCaja->persona2=auxAmigos->elem;
									nuevaCaja->amigos1=amigos;
									nuevaCaja->amigos2 = NULL;
									nuevaCaja->siguiente=NULL;

									pthread_mutex_lock(&semaforoListaAmigos);
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

		auxHilo = auxHilo->siguiente;




	}

	
	LISTAAMIGOS *auxCabecera = listaAmigosPadre;
	LISTA *auxAmigos1;
	LISTA *auxAmigos2;

	/*while (auxCabecera != NULL) {

		printf("La persona 1 es: %s\n",auxCabecera->persona1);
		printf("La persona 2 es: %s\n",auxCabecera->persona2);

		auxAmigos1 = auxCabecera->amigos1;
		auxAmigos2 = auxCabecera->amigos2;

		while (auxAmigos1 != NULL) {

			printf("El amigo 1 es: %s\n",auxAmigos1->elem);


			auxAmigos1 = auxAmigos1->siguiente;

		}

			while (auxAmigos2 != NULL) {


			printf("El amigo 2 es: %s\n",auxAmigos2->elem);

			auxAmigos2 = auxAmigos2->siguiente;

		}



		auxCabecera = auxCabecera->siguiente;



	}*/

	
	//auxCabecera=(LISTAAMIGOS*)malloc(sizeof(LISTAAMIGOS));
	//auxCabecera->persona1= "hola";
	//auxCabecera->persona2="hola";
	//auxCabecera->siguiente=NULL;

	//listaAmigosPadre = auxCabecera;


	// Pendiente revisar:
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

	// Pendiente revisar:
	return(0);

}
//----------------------------------------------------------------------------//
void EliminarEstructuraLista(LISTA **Cabecera,int num){

	LISTA *aux;
	int i;
	for (i = 0; i < num; ++i){

		aux = Cabecera[i];
		if (aux==NULL){
			;
		}
		else{

			while(aux!=NULL){

				Cabecera[i] = aux-> siguiente;
				free(aux->elem);
				free(aux);
				aux = Cabecera[i];

			}

		}

		//printf("Soy la cabecera dentro de la funcion: %s\n",Cabecera[i]);
	}
}

//----------------------------------------------------------------------------//

// Definicion de funciones:

void LeerArchivo(char *nombre_archivo,int numeroProcesos,LISTACABECERAMAPH **CabeceraMapH) {
	
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
			//printf("%s\n",Linea);


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


int main(int argc, char *argv[]) {

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

	//LISTA *Cabecera[numeroHilos];
	LeerArchivo(archivoEntrada,numeroHilos,&listaMapH);


	// Como prueba se imprime la lista de asignacion:

	LISTACABECERAMAPH *tmp1 = listaMapH;
	LISTAMAPH *tmp2;
	int counter = 0;


	while(tmp1 != NULL){

		tmp2 = tmp1->elem;
		//printf("Estoy en la lista numero: %d\n",counter);
		//while(tmp2!=NULL){
		//	printf("La persona es :%s\n",tmp2->persona);
		//	printf("Los amigos son :%s\n",tmp2->amigos);
		//	tmp2=tmp2->siguiente;
		//}

		tmp1=tmp1->siguiente;
		counter++;

	}
	
	// Se crea la lista enlazada generica sobre la que trabajaran los hilos:
	int r[numeroHilos];
	pthread_t hilos[numeroHilos];	
	char argumentos[numeroHilos][20];


	// Se inicializan los hilos:
	int i;
	for (i = 0; i < numeroHilos; i++) {

		sprintf(argumentos[i], "%d", i);

		if( (r[i] = pthread_create(&hilos[i],NULL,&hiloMap,argumentos[i])) ) {

			printf("FAILED\n");
		}

	}



	printf("El valor del contador es: %d\n",counter);


	// Se elimina la estructura que se creo al leer el archivo:
	//EliminarEstructuraLista(Cabecera,numeroHilos);
	
	// Se escribe la informacion en el archivo de salida:

	// Se espera a que todos los hilos terminen:
	for (i = 0; i < numeroHilos; i++) {
			pthread_join(hilos[i],NULL);
	}

	LISTAAMIGOS *auxCabecera = listaAmigosPadre;
	LISTA *auxAmigos1;
	LISTA *auxAmigos2;


	// Se imprime la lista global:

	while (auxCabecera != NULL) {

		printf("La persona 1 es: %s\n",auxCabecera->persona1);
		printf("La persona 2 es: %s\n",auxCabecera->persona2);

		auxAmigos1 = auxCabecera->amigos1;
		auxAmigos2 = auxCabecera->amigos2;

		while (auxAmigos1 != NULL) {

			printf("El amigo 1 es: %s\n",auxAmigos1->elem);


			auxAmigos1 = auxAmigos1->siguiente;

		}

			while (auxAmigos2 != NULL) {


			printf("El amigo 2 es: %s\n",auxAmigos2->elem);

			auxAmigos2 = auxAmigos2->siguiente;

		}



		auxCabecera = auxCabecera->siguiente;



	}

	// Se crean los hilos que haran reduce y escribiran en el archivo
	// de salida:


	// Se hace reduce y se imprimen en el archivo de 
	// salida los resultados:



	return(0);


}

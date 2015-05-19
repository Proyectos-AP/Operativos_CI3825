/*

 Archivo: lib_procesos.c

 Nombres:
	Alejandra Cordero / Carnet: 12-10645
	Pablo Maldonado   / Carnet: 12-10561

 Descripcion: lib_procesos.c

 Ultima modificacion: 26/04/2015

*/

// Definicion de tipos:


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

//----------------------------------------------------------------------------//
//                            DEFINICION DE ESTRUCTURAS                       //
//----------------------------------------------------------------------------//
typedef struct lista {

	/* Descripcion de la estructura: Lista enlazada de strings. */

	char *elem;					// Apuntador a char que contendra un string.
	struct lista *siguiente;	// Apuntador al siguiente elemento de la lista.

} LISTA;

//----------------------------------------------------------------------------//

typedef struct listaAmigos {

	/* Descripcion de la estructura: Lista enlazada que se utilizara para 
	almacenar de forma global los pares de usuarios con sus respectivos amigos 
	luego de que cada proceso ejecute map. Ejemplo: Si se tiene como entrada lo 
	siguiente: (Ana Bernardo) -> Bernardo Cristina David 
	/ (Bernardo Cristina)-> Ana Jose Alejandra, 
	un elemento de esta lista sera:

		ejemplo->listo = 1
		ejemplo->persona1 = Ana
		ejemplo->persona2 = Bernardo
		ejemplo->amigos1 = Bernardo Cristina David
		ejemplo->amigos2 = Ana Jose Alejandra
		ejemplo->siguiente = "direccion del siguiente elemento de la lista"
	 */

	int listo; 						// Indica si el elemento tiene ambos 
									// "amigos" ocupados.

	char *persona1;					// Apuntador a la caja que almacena el 
									// nombre de la primera persona.

	char *persona2;					// Apuntador a la caja que almacena el 
									// nombre de la segunda persona.

	char *amigos1;					// Apuntador a la caja que almacena el 
									// nombre de los amigos de la primera persona.

	char *amigos2;					// Apuntador a la caja que almacena el 
									// nombre de los amigos de la seguna persona.

	struct listaAmigos *siguiente;	// Apuntador al siguiente elemento de la 
									// lista enlazada.


} LISTAAMIGOS;


//----------------------------------------------------------------------------//
//                           DEFINICION DE FUNCIONES                          //
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


	if (aux==NULL){
		;
	}
	else{

		while(aux!=NULL){

			*Cabecera= aux-> siguiente;
			free(aux->elem);
			free(aux);
			aux = *Cabecera;

		}

	}

}

//----------------------------------------------------------------------------//

void EliminarListaAmigos(LISTAAMIGOS **lista){

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
			free(aux->persona1);
			free(aux->persona2);

			if(aux->amigos1!=NULL){
				free(aux->amigos1);
			}

			if(aux->amigos2!=NULL){
				free(aux->amigos2);	
			}
			free(aux);
		}
	}
}

//----------------------------------------------------------------------------//
void LeerArchivo(char *nombre_archivo,int numeroProcesos){

	/*

		Definicion de la funcion:	

			Dado el nombre de un archivo y un numero entero, esta funcion crea 
		n archivos donde n es igual al parametro numeroProcesos; lee
		linea a linea el archivo recibido como parametro
		y va repartiendo de manera equitativa cada linea en cada uno de los
		archivos creados.
	
		Parametros de entrada:

			- nombre_archivo : Nombre del archivo que sera leido.
			- numeroProcesos : Numero de archivos que seran creados.
	
	 	Parametros de salida:
	 		Ninguno.
	
	*/

	// Declaracion de variables:
	int numeroLinea=0;
	int resto;
	int i;
	char nombreString[10];
	char *Linea;
	FILE *archivo;
	FILE *archivoSalida[numeroProcesos];
	
	if ( (archivo=fopen(nombre_archivo,"r")) == NULL) {

		perror("Error: El archivo indicado no fue encontrado ");
		printf("errno = %d. \n",errno);
		exit(1);
	}
	
	else {
		
		//archivo = fopen(nombre_archivo,"r");
		fseek(archivo,0,SEEK_END);

		 if (ftell(archivo) == 0 )	{		
   			//El archivo esta vacio.
   			printf("---------------------------------.\n");
   			printf("El archivo de entrada esta vacio.\n");
   			printf("---------------------------------.\n");
   			exit(0);
    	}

    	else{

			fseek(archivo,0,SEEK_SET);
			// Se abren los archivos
			for (i = 0; i < numeroProcesos; ++i)
			{
				sprintf(nombreString,"%d",i);
				archivoSalida[i] = fopen(nombreString,"w");
			}

			
			// Se empieza a leer el archivo:
			while(1){

				// Se reserva el espacio de memoria para la nueva linea
				Linea = (char*)malloc(sizeof(char)*101);
				fscanf(archivo, " %[^\n]\n" ,Linea);
			
				resto = numeroLinea%numeroProcesos;

				// Se escribe en alguno de los archivos creados.
				fprintf(archivoSalida[resto],"%s \n",Linea);
				numeroLinea++;
				free(Linea);
				// Se verifica si se ha llegado al fin del archivo
				if (feof(archivo)) {
					fclose(archivo);
					break;
					
					}
				}

			// Se cierran todos los archivos abiertos:
			for (i = 0; i < numeroProcesos; ++i){

				fclose(archivoSalida[i]);

				}

			}
			
		}
	}


//----------------------------------------------------------------------------//

void mapProcesos(char *Persona,char *Amigos, FILE *archivoSalida){

	/*

		Definicion de la funcion:	

			Esta funcion, dado el nombre de una persona y una lista de amigos 
		realiza el algortimo de map y escribe el resultado en un archivo de
		salida que es pasado por los parametros de la funcion.
	
		Parametros de entrada:

			- Persona : Nombre de una persona.
			- Amigos : Lista de amigos de la variable Persona.
			- archivoSalida : Archivo donde se escribira el resultado de map.	
	 	Parametros de salida:
	 		Ninguno.
	
	*/

	// Declaracion de variables:
	char *token;	
	LISTA *listaAmigos;
	LISTA *aux;
	LISTA *nueva_caja;
	LISTA *amigos;

	listaAmigos = NULL;


	// A continuacion se creara una lista enlazada que almacenara
	// cada nombre de los amigos de la variable Persona.
	token=strtok(Amigos," ");

	if (strcmp(token,"-None-") == 0) {
		//free(Persona);
		//free(Amigos);
	}
	else {

		// Se crea el primer elemento de la lista de amigos
		nueva_caja = (LISTA*)malloc(sizeof(LISTA));
		nueva_caja->elem = token;
		listaAmigos = nueva_caja;
		aux = nueva_caja;

		while (aux->elem != NULL) {
			nueva_caja = (LISTA*)malloc(sizeof(LISTA));
			nueva_caja->elem = strtok(NULL," ");
			aux->siguiente = nueva_caja;
			aux = aux->siguiente;
		}

		// Se procede a escribir en el archivo de salida con el formato del algoritmo de map.
		aux = listaAmigos;

		while( aux->elem != NULL) {

			amigos = listaAmigos;
			// Se imprime la variable Persona y a uno de sus amigos
			fprintf(archivoSalida,"(%s %s) -> ",Persona,aux->elem );
				
			// Se imprimen todos los amigos de la variable Persona	
			while (amigos->elem != NULL) {

				fprintf(archivoSalida,"%s ",amigos->elem);
				amigos = amigos->siguiente;

			}

			fprintf(archivoSalida,"\n");
			aux = aux->siguiente;
		}

		// Se destruye la estructura creada
		//EliminarEstructuraLista(&listaAmigos);

	}
}

//----------------------------------------------------------------------------//

void LeerTrabajoMap(int numeroArchivos,LISTAAMIGOS **listaAmigos, int NombreArchivos[]){

	/*

		Definicion de la funcion:	
			Esta funcion se encarga de leer todos los archivos creados por los
		procesos al momento de realizar map.
			A medida que se va leyendo los archivos se van introduciendo los
		datos en una lista enlazada de tipo LISTAAMIGOS.

		Parametros de entrada:

			- numeroArchivos : Entero que representa el numero de archivos a leer.
			- listaAmigos : Direccion de memeria de la cabecera que se ira
							creando a medida que se vayan leyendo los archivos.

			- NombreArchivos : Arreglo que almacena los nombres de los archivos 
							   a leer
		
	 	Parametros de salida:
	 		Ninguno.
	
	*/

	// Declaracion de variables:
	int FinalArchivo;
	int i;
	char *Persona1;
	char *Persona2;
	char *Amigos;
	char nombreSalida[30];
	LISTAAMIGOS *aux;
	LISTAAMIGOS *nueva_caja;
	FILE *archivoProcesos;

	for (i = 0; i < numeroArchivos; i++) {
	
		// Se transforma el nombre del archivo en string:
		sprintf(nombreSalida, "%d",NombreArchivos[i]);
		// Se concatena el nombre del archivo con ".txt"
		// para obtener el nombre del archivo de salida:
		strcat(nombreSalida, ".txt");

		if ( (archivoProcesos=fopen(nombreSalida,"r")) == NULL ) {

			perror("Error: El archivo indicado no fue encontrado ");
			printf("errno = %d. \n",errno);
			exit(1);
		}

		else {

			//archivoProcesos = fopen(nombreSalida,"r");
			fseek(archivoProcesos,0,SEEK_END);

	   		if (ftell(archivoProcesos) == 0 )	{		
	   			//El archivo esta vacio.
				fclose(archivoProcesos);
	   			remove(nombreSalida);

	    	}
				
	    	else {
	    		fseek(archivoProcesos,0,SEEK_SET);
	    		FinalArchivo = 0;
				while (FinalArchivo == 0) {

					Persona1 = (char*)malloc(sizeof(char)*15);
					Persona2 = (char*)malloc(sizeof(char)*15);
					Amigos = (char*)malloc(sizeof(char)*30);

					fscanf(archivoProcesos," ( %[^ (] %[^ )] ) -> %[^\n]\n" ,Persona1,Persona2,Amigos);
						
					// Se agrega la informacion a la lista enlazada de personas:

					aux = *listaAmigos;

					// La lista enlazada esta vacia:
					if (aux == NULL) {

						// Se crea un nuevo nodo
						nueva_caja = (LISTAAMIGOS*)malloc(sizeof(LISTAAMIGOS));
						nueva_caja->listo = 0;
						nueva_caja->persona1=Persona1;
						nueva_caja->persona2=Persona2;
						nueva_caja->amigos1=Amigos;
						nueva_caja->amigos2 = NULL;
						nueva_caja->siguiente=NULL;

						*listaAmigos=nueva_caja;

					}

					else {

						while (aux != NULL) {

							// El nodo ya posee toda la informacion que se requiere
							if (aux->listo == 1 ) {
								aux = aux->siguiente;
							}

							else {

								// Se comparan los nombre se las personas para
								// eliminar redundancias
								if ( ((strcmp(aux->persona1,Persona2)) == 0) \
									&& (strcmp(aux->persona2,Persona1) == 0) ) {

									if (aux->amigos1 == NULL) {
										aux->amigos1 = Amigos;
										aux->amigos2 = NULL;
									}

									else if (aux->amigos2 == NULL) {
										aux->amigos2 = Amigos;
										aux->listo = 1;
									}
									
									free(Persona1);
									free(Persona2);
									break;

								}

								else {

									// Se inserta un nuevo elemento al final de 
									//la lista enlazada.
									if (aux->siguiente == NULL) {

										// Se crea un nuevo nodo
										nueva_caja = (LISTAAMIGOS*)malloc(sizeof(LISTAAMIGOS));
										nueva_caja->listo = 0;
										nueva_caja->persona1=Persona1;
										nueva_caja->persona2=Persona2;
										nueva_caja->amigos1=Amigos;
										nueva_caja->amigos2 = NULL;
										nueva_caja->siguiente=NULL;
										aux->siguiente = nueva_caja;
										break;

									}
									else{
										
										}
									aux = aux->siguiente;

								}

							}

						}

					}

					// Se verifica si se llego al final del archivo.
					if (feof(archivoProcesos)){
						FinalArchivo = 1;
						fclose(archivoProcesos);	
						remove(nombreSalida);

					}

				}

			}

		}

	}
}

//----------------------------------------------------------------------------//
void ReparteTrabajoReduce(int numeroArchivos,LISTAAMIGOS *listaAmigos){

	/*

		Definicion de la funcion:	
			Esta funcion dada una lista enlazada de tipo LISTAAMIGOS reparte de 
		manera equitativa la informacion que posee cada nodo de la misma en n 
		archivos.Siendo n el parametro numeroProcesos recibido por la funcion.

		Parametros de entrada:

			- numeroArchivos : Entero que representa el numero de archivos a 
							   escribir.
			- listaAmigos : Cabecera de una lista enlazada de tipo LISTAAMIGOS 
							cuya informacion se ira escribiendo en los archivos 
							creados.
		
	 	Parametros de salida:
	 		Ninguno.
	
	*/

	// Declaracion de variables:
	int contadorReduce = 0; // Entero que almacenara el numero de elementos de 
							//la lista enlazada.
	int restoReduce = 0;
	char nombreReduce[10];
	FILE *archivosReduce[numeroArchivos];
	LISTAAMIGOS *aux;


	aux = listaAmigos;

	while (aux != NULL) {

		// Se abren los archivos para repartir las tareas:

		restoReduce = contadorReduce % numeroArchivos;
		sprintf(nombreReduce,"%d",restoReduce);
		archivosReduce[restoReduce] = fopen(nombreReduce,"a");

		// Caso en donde la informacion del nodo esta completa
		if ( aux->listo == 1) {

			fprintf(archivosReduce[restoReduce],"%d (%s %s) -> %s, %s\n",\
				aux->listo,aux->persona1,aux->persona2,aux->amigos1,aux->amigos2);

		}

		else {

			// Caso en donde una de las personas de la tupla (ej :(Ana Bernardo)) 
			// tiene amigos.
			if (aux->amigos1 != NULL) {

				fprintf(archivosReduce[restoReduce],"%d (%s %s) -> %s\n",\
					aux->listo,aux->persona1,aux->persona2,aux->amigos1);
			}

			// Caso en donde niguna de las personas de la tupla tiene amigos.
			else {

				fprintf(archivosReduce[restoReduce],"%d (%s %s) -> -None-\n",\
					aux->listo,aux->persona1,aux->persona2);

			}

		}

		contadorReduce++;
		aux = aux->siguiente;

		// Se cierra el archivo.
		fclose(archivosReduce[restoReduce]);
		
	}
}

//----------------------------------------------------------------------------//
void ReduceProcesos(char *Persona1,char *Persona2,char *Amigos,FILE *archivoSalida){

	/*

		Definicion de la funcion:	

			Esta funcion, dado el nombre de dos personas y una lista de amigos 
		realiza el algortimo de reduce y escribe el resultado en un archivo de
		salida que es pasado por los parametros de la funcion.
	
		Parametros de entrada:

			- Persona1 : Nombre de una persona.
			- Persona2 : Nombre de una persona.
			- Amigos : Lista de amigos de las variables Persona1 y Persona2.
			- archivoSalida : Archivo donde se escribira el resultado de reduce.

	 	Parametros de salida:
	 		Ninguno.
	
	*/

	// Declaracion de variables:
	int amigosEnComun = 0;
	char *token;
    LISTA *listaAmigos1=NULL;
	LISTA *aux1;
	LISTA *nueva_caja1;

	LISTA *listaAmigos2=NULL;
	LISTA *aux2;
	LISTA *nueva_caja2;

    fprintf(archivoSalida,"(%s %s) -> ",Persona1,Persona2);

 	// Se almacenan los amigos en listas enlazadas:

					
	// Se crea el primer nodo:
	token = strtok(Amigos," ");
	nueva_caja1 = (LISTA*)malloc(sizeof(LISTA));
	nueva_caja1->elem = token;
	listaAmigos1 = nueva_caja1;
	aux1 = nueva_caja1;
					

	while( strcmp(token,",") != 0)  {

		token = strtok(NULL," ");
		if (strcmp(token,",") != 0) {

			nueva_caja1 = (LISTA*)malloc(sizeof(LISTA));
			nueva_caja1->elem = token;
			aux1->siguiente = nueva_caja1;
			aux1 = nueva_caja1;

		}

	}
			
	while(token != NULL) {

		if(listaAmigos2==NULL){

			nueva_caja2 = (LISTA*)malloc(sizeof(LISTA));
			token = strtok(NULL," ");
			nueva_caja2->elem = token;
			listaAmigos2 = nueva_caja2;
			aux2 = nueva_caja2;

		}

		else{
			token = strtok(NULL," ");
			if (token != NULL) {
				nueva_caja2 = (LISTA*)malloc(sizeof(LISTA));
				nueva_caja2->elem = token;
				aux2->siguiente = nueva_caja2;
				aux2 = nueva_caja2;

				}
			}

	}
						
	aux1 = listaAmigos1;

	while (aux1 != NULL) {

		aux2 = listaAmigos2;

		while(aux2 != NULL) {

			if (strcmp(aux1->elem,aux2->elem) == 0) {
				amigosEnComun = 1;
				fprintf(archivoSalida,"%s ",aux1->elem);
				break;

			}

			aux2 = aux2-> siguiente;
		}

		aux1 = aux1->siguiente;

	}

	if (amigosEnComun == 0) {
		fprintf(archivoSalida,"-None-");
	}
	
	fprintf(archivoSalida,"\n");

	// Se eliminan las estructuras creadas
	//EliminarEstructuraLista(&listaAmigos1);
	//EliminarEstructuraLista(&listaAmigos2);

}


//----------------------------------------------------------------------------//

void LeerTrabajoReduce(int numeroArchivos,char *archivoSalida,int nombre_archivos[]){

	/*

		Definicion de la funcion:	
			Esta funcion lee toda la informacion de varios archivos y las escribe 
		en un unico archivo cuyo nombre sera dado por el parametro nombreSalida.

		Parametros de entrada:

			- numeroArchivos : Entero que representa el numero de archivos a leer.
			- listaAmigos : Cabecera de una lista enlazada de tipo LISTAAMIGOS cuya 
							informacion se ira escribiendo en los archivos creados.
		
	 	Parametros de salida:
	 		Ninguno.
	
	*/

	// Declaracion de variables:
	int FinalArchivo;
	int i;
	char *Linea;
	char archivoProceso[30];
	FILE *archivoFinal;
	FILE *archivoHijos;

	archivoFinal = fopen(archivoSalida,"w");

	for (i = 0; i < numeroArchivos; i++) {

		FinalArchivo=0;
		// Se transforma el nombre de los archivos en string
		sprintf(archivoProceso, "%d",nombre_archivos[i]);
		strcat(archivoProceso, ".txt");

		if ( (archivoHijos=fopen(archivoProceso,"r")) == NULL ) {

			perror("Error: El archivo indicado no fue encontrado ");
			printf("errno = %d. \n",errno);
			exit(1);
		}

		else{

			//archivoHijos = fopen(archivoProceso,"r");
			fseek(archivoHijos,0,SEEK_END);

   			if (ftell(archivoHijos) == 0 )	{
   			
   				//El archivo esta vacio.
				fclose(archivoHijos);
   				remove(archivoProceso);

    		}
    		else{

    			fseek(archivoHijos,0,SEEK_SET);
    			while(FinalArchivo == 0){

    				Linea = (char*)malloc(sizeof(char)*101);
					fscanf(archivoHijos, " %[^\n]\n" ,Linea);
					fprintf(archivoFinal,"%s\n",Linea);
					free(Linea);

    				// Se verifica si se ha llegado al fin del archivo
					if(feof(archivoHijos)== 1){
						FinalArchivo = 1;
						fclose(archivoHijos);	
						remove(archivoProceso);
					}
    			}
    			
    		}

		}


	}

	fclose(archivoFinal);
}


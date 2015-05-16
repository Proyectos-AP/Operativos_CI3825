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

void mapProcesos(char *Persona,char *Amigos, FILE *archivoSalida){

	/*

		Definicion de la funcion:	

			Esta funcion, dado el nombre de una persona y una lista de amigos 
		realiza el algortimo de map y escribe el resultado en un archivo de
		salida que es pasado por los parametros de la funcion.
	
		Parametros de entrada:

			- Persona : Nombre del archivo que sera leido.
			- Amigos : Numero de archivos que seran creados.
			- archivoSalida : Numero de archivos que seran creados.	
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

// Inicio del codigo principal:

int main(int argc, char *argv[]) {

	/*

		Definicion de la funcion:	

			Dado el nombre de un archivo esta funcion lee cada 
		linea del mismo y realiza map sobre cada una de ellas.
	
		Parametros de entrada:

			- argv[0]: Nombre del archivo que sera leido.
	
	 	Parametros de salida:
	 		Ninguno.
	
	*/

	// Declaracion de variables:
	int FinalArchivo=0;
	char nombreSalida[30];
	char *persona;
	char *amigos;
	FILE *archivoEntrada;
	FILE *archivoDeSalida;

	// Se obtiene el PID del proceso:
	int miPID = getpid();
	
	// Se convierte el PID en string
	sprintf(nombreSalida, "%d",miPID);
	// Se concatena el PID con ".txt"
	// para obtener el nombre del archivo de salida:
	strcat(nombreSalida, ".txt");


	// Se abre el archivo de salida:
	archivoDeSalida = fopen(nombreSalida,"a");


	// Se procede a leer el archivo de entrada.

	// No existe el archivo
	if ( fopen(argv[0],"r") == NULL ) {

		exit(1);
		}

	else {

		archivoEntrada = fopen(argv[0],"r");
		fseek(archivoEntrada,0,SEEK_END);

   		if (ftell(archivoEntrada) == 0 )	{
   			
   			//El archivo esta vacio.
			fclose(archivoEntrada);
   			remove(argv[0]);
   			exit(0);
    	}

    	else {

    		fseek(archivoEntrada,0,SEEK_SET);
	

			while(FinalArchivo == 0){

				persona = (char*)malloc(sizeof(char)*15);
				amigos = (char*)malloc(sizeof(char)*30);
				fscanf(archivoEntrada," %[^ ->] -> %[^\n]\n" ,persona,amigos);

				mapProcesos(persona,amigos,archivoDeSalida);
				free(persona);
				free(amigos);
				// Se verifica si se ha llegado al fin del archivo
				if(feof(archivoEntrada)== 1){
					FinalArchivo = 1;
					fclose(archivoEntrada);	
					remove(argv[0]);
				}

			}

		}
	}
	// Se cierra el archivo de salida:
	fclose(archivoDeSalida);	
	return(0);

}
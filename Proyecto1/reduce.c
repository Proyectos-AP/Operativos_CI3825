/*
*
* Archivo: reduce.c
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

	// Se obtiene el PID del proceso:
	int miPID = getpid();
	char nombreSalida[30];
	int FinalArchivo=0;
	int realizaReduce;
	char *Persona1;
	char *Persona2;
	char *Amigos;


	// Se lee el archivo de entrada
	FILE *archivoEntrada;
	FILE *archivoSalida;

	
	// Se convierte el PID en string
	sprintf(nombreSalida, "%d",miPID);
	strcat(nombreSalida, ".txt");

	if ( fopen(argv[0],"r") == NULL ) {

		exit(1);
	}

	else{


		archivoEntrada = fopen(argv[0],"r");
		fseek(archivoEntrada,0,SEEK_END);

		archivoSalida = fopen(nombreSalida,"w");


   		if (ftell(archivoEntrada) == 0 )	{
   			
   			//El archivo esta vacio.
			fclose(archivoEntrada);	
			remove(argv[0]);
   			exit(0);
    	}

    	else{

    		fseek(archivoEntrada,0,SEEK_SET);

    		while(FinalArchivo == 0){

  
    			realizaReduce = fgetc(archivoEntrada);
    			realizaReduce = realizaReduce - 48;

    			if (realizaReduce == 0) {

    				Persona1 = (char*)malloc(sizeof(char)*15);
    				Persona2 = (char*)malloc(sizeof(char)*15);

    				fscanf(archivoEntrada," (%[^ (] %[^ )] ) \n" ,Persona1,Persona2);
					fprintf(archivoSalida,"(%s %s) -> -None-\n",Persona1,Persona2);

					while (fgetc(archivoEntrada) != 10);


    			}

    			else if (realizaReduce == 1) {

					Persona1 = (char*)malloc(sizeof(char)*15);
    				Persona2 = (char*)malloc(sizeof(char)*15);
					Amigos = (char*)malloc(sizeof(char)*100);

    				fscanf(archivoEntrada," (%[^ (] %[^ )] ) -> %[^\n]\n",Persona1,Persona2,Amigos);

    				// Se aplica reduce
    				ReduceProcesos(Persona1,Persona2,Amigos,archivoSalida);

    			}


    			// Se verifica si se ha llegado al fin del archivo
				if(feof(archivoEntrada)== 1){
					FinalArchivo = 1;
					fclose(archivoEntrada);	
					remove(argv[0]);
					fclose(archivoSalida);
				}

    		}


    	}


	}

	return(0);

}
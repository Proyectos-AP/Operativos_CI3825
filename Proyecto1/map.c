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


// Inicio del codigo principal:

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
	
	// Se convierte el PID en string
	char pidStr[20];
	sprintf(pidStr, "%d",miPID);
	int FinalArchivo=0;

	// Se concatena el PID con ".txt"
	// para obtener el nombre del archivo de salida:
	char nombreSalida[30];
	char inicio[40];
	strcpy(inicio,pidStr);
	strcpy(nombreSalida,inicio);
	strcat(nombreSalida, ".txt");

	printf("EL PARAMETRO QUE ME PASARON ES : %s\n",argv[0]);


	// Se lee el archivo de entrada
	FILE *archivoEntrada;

	// Se abre el archivo de salida:

	FILE *archivoSalida;

	archivoSalida = fopen(nombreSalida,"a");



	if ( fopen(argv[0],"r") == NULL ) {

		printf("No tengo trabajo asignado\n");
		exit(1);
		}

	else {

		archivoEntrada = fopen(argv[0],"r");
		fseek(archivoEntrada,0,SEEK_END);

   		if (ftell(archivoEntrada) == 0 )	{
   			
   			//El archivo esta vacio.
   			printf("No tengo trabajo asignado\n");
			fclose(archivoEntrada);
   			remove(argv[0]);
   			exit(0);
    	}

    	else {

    		fseek(archivoEntrada,0,SEEK_SET);
			char *Persona;
			char *Amigos;
			char *token;	

			while(FinalArchivo == 0){

				Persona = (char*)malloc(sizeof(char)*15);
				Amigos = (char*)malloc(sizeof(char)*30);
				fscanf(archivoEntrada," %[^ ->] -> %[^\n]\n" ,Persona,Amigos);
				printf("La Persona : %s -> %s \n",Persona,Amigos);

				LISTA *listaAmigos;
				LISTA *aux;
				LISTA *nueva_caja;
				LISTA *amigos;

				listaAmigos = NULL;

				token=strtok(Amigos," ");

				if (strcmp(token,"-None-") == 0) {
					printf("HABIA UN NONE\n");
					free(Persona);
					free(Amigos);
				}
				else {

					nueva_caja = (LISTA*)malloc(sizeof(LISTA));
					nueva_caja->elem = token;
					listaAmigos = nueva_caja;
					aux = nueva_caja;


					while (aux->elem != NULL) {

						printf("Soy el token: %s.\n",aux->elem);
						nueva_caja = (LISTA*)malloc(sizeof(LISTA));
						nueva_caja->elem = strtok(NULL," ");
						aux->siguiente = nueva_caja;
						aux = aux->siguiente;
					}

					aux = listaAmigos;

					while( aux->elem != NULL) {

						amigos = listaAmigos;

						fprintf(archivoSalida,"(%s %s) -> ",Persona,aux->elem );
					
						while (amigos->elem != NULL) {

							fprintf(archivoSalida,"%s ",amigos->elem);

							amigos = amigos->siguiente;

						}

						fprintf(archivoSalida,"\n");

						aux = aux->siguiente;

					}

				}

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
	fclose(archivoSalida);	

	return(0);


}
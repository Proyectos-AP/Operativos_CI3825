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

	// Se obtiene el PID del proceso:
	int miPID = getpid();
	char nombreSalida[30];
	int FinalArchivo=0;
	int realizaReduce;
	char *Persona1;
	char *Persona2;
	char *Amigos;
	char *Amigos1;
	char *Amigos2;
	char probando;	


	// Se lee el archivo de entrada
	FILE *archivoEntrada;
	FILE *archivoSalida;

	
	// Se convierte el PID en string
	char pidStr[20];
	sprintf(nombreSalida, "%d",miPID);
	strcat(nombreSalida, ".txt");
	printf("reduce %s\n",argv[0]);




	if ( fopen(argv[0],"r") == NULL ) {

		printf("No tengo trabajo asignado\n");
		exit(1);
	}

	else{


		archivoEntrada = fopen(argv[0],"r");
		fseek(archivoEntrada,0,SEEK_END);

		archivoSalida = fopen(nombreSalida,"w");


   		if (ftell(archivoEntrada) == 0 )	{
   			
   			//El archivo esta vacio.
   			printf("No tengo trabajo asignado\n");
   			exit(0);
    	}

    	else{

    		fseek(archivoEntrada,0,SEEK_SET);

    		int i = 0;

    		while(FinalArchivo == 0){

  
    			realizaReduce = fgetc(archivoEntrada);
    			realizaReduce = realizaReduce - 48;

				printf("Realiza Reduce: %d\n",realizaReduce);

    			if (realizaReduce == 0) {
    				printf("No tengo amigos en comun\n");
    				Persona1 = (char*)malloc(sizeof(char)*15);
    				Persona2 = (char*)malloc(sizeof(char)*15);
					Amigos1 = (char*)malloc(sizeof(char)*30);

    				fscanf(archivoEntrada," (%[^ (] %[^ )] ) \n" ,Persona1,Persona2);

    				//printf("////////////////////////////\n");
    				//printf("Persona 1: %s.\n",Persona1);
					//printf("Persona 2: %s.\n",Persona2);
					//printf("////////////////////////////\n");

					fprintf(archivoSalida,"(%s %s) -> -None-\n",Persona1,Persona2);

					while (fgetc(archivoEntrada) != 10);


    			}

    			else if (realizaReduce == 1) {

    				printf("TENGA AMIGOS EN COMUN\n");
					Persona1 = (char*)malloc(sizeof(char)*15);
    				Persona2 = (char*)malloc(sizeof(char)*15);
					Amigos = (char*)malloc(sizeof(char)*100);

    				fscanf(archivoEntrada," (%[^ (] %[^ )] ) -> %[^\n]\n",Persona1,Persona2,Amigos);

    				fprintf(archivoSalida,"(%s %s) -> ",Persona1,Persona2);

    				int amigosEnComun = 0;
    				// Se almacenan los amigos en listas enlazadas:

    				char *token;

    				LISTA *listaAmigos1=NULL;
					LISTA *aux1;
					LISTA *nueva_caja1;

					LISTA *listaAmigos2=NULL;
					LISTA *aux2;
					LISTA *nueva_caja2;
					
					// Se crea la primera caja:
					token = strtok(Amigos," ");
					nueva_caja1 = (LISTA*)malloc(sizeof(LISTA));
					nueva_caja1->elem = token;
					listaAmigos1 = nueva_caja1;
					aux1 = nueva_caja1;
					

					while( strcmp(token,",") != 0)  {

						//printf("SOY EL TOKEN: %s.\n",token);
						nueva_caja1 = (LISTA*)malloc(sizeof(LISTA));
						token = strtok(NULL," ");
						if (strcmp(token,",") != 0) {
							nueva_caja1->elem = token;
							aux1->siguiente = nueva_caja1;
							aux1 = nueva_caja1;

						}

					}
			
					//if (strtok_r(Amigos2," ") < 0) {
					//	printf("ERROR DE LECTURA\n");
					//}

					while(token != NULL) {

						if(listaAmigos2==NULL){

							nueva_caja2 = (LISTA*)malloc(sizeof(LISTA));
							token = strtok(NULL," ");
							nueva_caja2->elem = token;
							listaAmigos2 = nueva_caja2;
							aux2 = nueva_caja2;

						}

						else{
							//printf("SOY EL TOKEN 2: %s.\n",token);
							nueva_caja2 = (LISTA*)malloc(sizeof(LISTA));
							token = strtok(NULL," ");
							if (token != NULL) {

								nueva_caja2->elem = token;
								aux2->siguiente = nueva_caja2;
								aux2 = nueva_caja2;

							}
						}

					}

					aux1 = listaAmigos1;
					aux2 = listaAmigos2;

					while(aux1!=NULL){
						printf("%s Estoy en la lista1 \n",aux1->elem);
						aux1 = aux1->siguiente;
					}

					while(aux2!=NULL){
						printf("%s Estoy en la lista2 \n",aux2->elem);
						aux2 = aux2->siguiente;
					}

					/*
					

					aux1 = listaAmigos1;
					aux2 = listaAmigos2;

					while (aux1 != NULL) {

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

					*/
					fprintf(archivoSalida,"\n");



    				printf("---------------------------\n");
    				printf("Persona 1: %s.\n",Persona1);
					printf("Persona 2: %s.\n",Persona2);
					printf("amigos 1: %s.\n",Amigos1);
					printf("amigos 2: %s.\n",Amigos2);
					printf("---------------------------\n");

    			}


    			else {
    				printf("entre aqui y el valor de realizaReduce es %d",realizaReduce);
    			}



    			// Se verifica si se ha llegado al fin del archivo
				if(feof(archivoEntrada)== 1){
					FinalArchivo = 1;
					//remove(argv[0]);
					fclose(archivoEntrada);	
					fclose(archivoSalida);
				}


				//Persona1 = (char*)malloc(sizeof(char)*15);
				//Amigos1 = (char*)malloc(sizeof(char)*30);
				//Amigos2 = (char*)malloc(sizeof(char)*30);

				//fscanf(archivoEntrada,"%d (%[^ (] %[^ )] ) -> %[^\n]\n" ,&realizaReduce,Persona1,Amigos1,Amigos2);

				//fscanf(archivoEntrada," %[^ ->] -> %[^\n]\n" ,Persona1,Amigos1);
				//printf("Persona 1: %s.\n",Persona1);
				///printf("Persona 2: %s.\n",Persona2);
				//printf("amigos 1: %s.\n",Amigos1);
				//printf("amigos 2: %s.\n",Amigos2);





    		}


    	}




	}










}
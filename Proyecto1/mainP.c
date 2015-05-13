/*
*
* Archivo: main.c
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
#include <errno.h>

// Definicion de Tipos:

typedef struct lista {

	char *nombre;
	struct lista *siguiente;

} LISTA;

typedef struct listaAmigos {

	int listo;
	char *persona1;
	char *persona2;
	char *amigos1;
	char *amigos2;
	struct listaAmigos *siguiente;

} LISTAAMIGOS;

// Definicion de funciones:

void LeerArchivo(char *nombre_archivo,int numeroProcesos){

	/*

		Definicion de la funcion:	
	
		Parametros de entrada:
	
	 	Parametros de salida:
	
	*/

	
	int numeroLinea=0;
	int resto;
	char nombreString[10];
	int i;
	//char nombreArchivoSalida;
	char *Linea;
	FILE *archivo;
	FILE *archivoSalida[numeroProcesos];
	
	if ( (fopen(nombre_archivo,"r")) == NULL) {

		perror("Error: El archivo indicado no fue encontrado ");
		printf("errno = %d. \n",errno);
		exit(1);
	}
	
	else {
		
		archivo = fopen(nombre_archivo,"r");
		
		// Abro los archivos
		for (i = 0; i < numeroProcesos; ++i)
		{
			sprintf(nombreString,"%d",i);
			archivoSalida[i] = fopen(nombreString,"w");
		}

		
		// Empiezo a leer el archivo:
		
		while(1){
			// Se reserva el espacio de memoria para la nueva linea
			Linea = (char*)malloc(sizeof(char)*101);
			fscanf(archivo, " %[^\n]\n" ,Linea);
			//printf("%s\n",Linea);

			resto = numeroLinea%numeroProcesos;
			fprintf(archivoSalida[resto],"%s \n",Linea);
			
			numeroLinea++;
			
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
	
// Inicio del codigo principal:

void  main(int argc, char *argv[]) {

	/*

		Definicion de la funcion:	
	
		Parametros de entrada:
	
	 	Parametros de salida:
	
	*/

	int numeroProcesos;
	char *archivoEntrada;
	char *archivoSalida;

	if (argc == 3) {

		numeroProcesos = 1;
		archivoEntrada = argv[1];
		archivoSalida = argv[2];

	}

	else if (argc == 5) {

		if ( strcmp(argv[1],"-n") == 0) {

			numeroProcesos = atoi(argv[2]);
			archivoEntrada = argv[3];
			archivoSalida = argv[4];

		}

		else {

			printf("Error: No se han pasado los argumentos de forma correcta.\n");
			exit(0);

		}


	}

	else {

		printf("Error: No se han pasado los argumentos de forma correcta .\n");
		exit(0);

	}

	// Se lee el archivo de entrada :
	LeerArchivo(archivoEntrada,numeroProcesos);
	int i;
	char numeroArchivo[20];
	

	int k;
	pid_t childpid[numeroProcesos];

	for(k = 0;k < numeroProcesos;k++){
		
		if ((childpid[k] = fork()) < 0) {
			perror("Error en el fork");
			exit(0);
		}

		if (childpid[k] == 0) {
			sprintf(numeroArchivo, "%d", k);
			if (execlp("./map",numeroArchivo,NULL) < 0) {
				perror("Fallo en la ejecucion de exec");	
			}
			
		}

	}
	
	
	// Espero que los proceso hijos terminen:
	for(i=0;i<numeroProcesos;i++){
		wait();
		printf("Mi hijo termino %d \n",childpid[i]);
		
		}

	// El padre lee el trabajo realizado por los hijos:

	char *Persona1;
	char *Persona2;
	char *Amigos;

	LISTAAMIGOS *listaAmigos;
	LISTAAMIGOS *aux;
	LISTAAMIGOS *nueva_caja;
	FILE *archivoProcesos;
	int FinalArchivo = 0;

	listaAmigos = NULL;

	for (i = 0; i < numeroProcesos; i++) {
	
		// Se convierte el PID en string:
		char pidStr[20];
		sprintf(pidStr, "%d",childpid[i]);

		// Se concatena el PID con ".txt"
		// para obtener el nombre del archivo de salida:
		char nombreSalida[30];
		char inicio[40];
		int FinalArchivo = 0;
		strcpy(inicio,pidStr);
		strcpy(nombreSalida,inicio);
		strcat(nombreSalida, ".txt");

		printf("Soy el proceso:  %d\n",childpid[i]);
		printf("Mi archivo de salida es:  %s\n",nombreSalida);


		if ( fopen(nombreSalida,"r") == NULL ) {

		printf("No tengo trabajo asignado\n");
		exit(1);
		}

		else {

		archivoProcesos = fopen(nombreSalida,"r");
			

		while (FinalArchivo == 0) {

				Persona1 = (char*)malloc(sizeof(char)*101);
				Persona2 = (char*)malloc(sizeof(char)*15);
				Amigos = (char*)malloc(sizeof(char)*30);

				//fscanf(archivoProcesos,"(%s %s) -> %s",Persona1,Persona2,Amigos);

				//fscanf(archivoProcesos," %[^ ->] -> %[^\n]\n" ,Persona1,Persona2);
				fscanf(archivoProcesos," ( %[^ (] %[^ )] ) -> %[^\n]\n" ,Persona1,Persona2,Amigos);
				
				//printf("La persona1 es: %s \n",Persona1);
				//printf("La persona2 es: %s \n",Persona2);
				//printf("Los amigos son: %s \n",Amigos);

				// Se agrega la informacion a la lista enlazada de personas:

				aux = listaAmigos;

				if (aux == NULL) {

					// Se crea un nuevo nodo
					nueva_caja = (LISTAAMIGOS*)malloc(sizeof(LISTAAMIGOS));
					nueva_caja->listo = 0;
					nueva_caja->persona1=Persona1;
					nueva_caja->persona2=Persona2;
					nueva_caja->amigos1=Amigos;
					nueva_caja->amigos2 = NULL;
					nueva_caja->siguiente=NULL;

					listaAmigos=nueva_caja;

				}

				else {

					while (aux != NULL) {


						if (aux->listo == 1 ) {
							aux = aux->siguiente;
						}

						else {


							if ( ((strcmp(aux->persona1,Persona2)) == 0) && (strcmp(aux->persona2,Persona1) == 0) )  {

								if (aux->amigos1 == NULL) {
									aux->amigos1 = Amigos;
									aux->amigos2 = NULL;
								}

								else if (aux->amigos2 == NULL) {
									aux->amigos2 = Amigos;
									aux->listo = 1;
								}

								break;

							}

							else {

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

								aux = aux->siguiente;

							}

						}

					}

				}


				if (feof(archivoProcesos)){
					FinalArchivo = 1;
					fclose(archivoProcesos);	
					printf("FINAL ARCHIVO\n");

					// hay que descomentar esto:
					//remove(nombreSalida);
					
					if (archivoProcesos == NULL) {

						perror("problemas");
					}

				}

		}

		// Se imprime la lista enlazada:

		aux = listaAmigos;

		while (aux != NULL) {

			printf("Persona1 es: %s.\n",aux->persona1);
			printf("Persona2 es: %s.\n",aux->persona2);
			printf("Amigos1 es: %s.\n",aux->amigos1);
			printf("Amigos2 es: %s\n",aux->amigos2);

			aux = aux->siguiente;

			












			

		}

		printf("me sali del ciclo\n");

		

		}

	}
}

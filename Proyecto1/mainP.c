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

	char *personas;
	LISTA *amigos1;
	LISTA *amigos2;
	struct listaAmigos *siguiente;

} LISTAAMIGOS;

// Definicion de funciones:

void LeerArchivo(char *nombre_archivo,int numeroProcesos){
	
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
	*
	* Definicion de la funcion:	
	*
	* Parametros de entrada:
	*
	* Parametros de salida:
	*
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
	char *numeroArchivo;

	for(i=0;i<numeroProcesos;i++){
		pid_t childpid;
		childpid = fork();

		if (childpid == 0) {
			sprintf(numeroArchivo, "%d", i);
			if (execlp("./map",numeroArchivo,NULL) < 0) {
				perror("Fallo en la ejecucion de exec");	
			}
			
		}

	}
	
	for(i=0;i<numeroProcesos;i++){
		wait();
		printf("Mi hijo termino \n");
		
		}
}

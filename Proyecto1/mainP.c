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

void LeerArchivo(char *nombre_archivo,int numeroProcesos,LISTA **Arreglo,int numeroElementos[]){
	
	int numeroLinea=0;
	int resto;
	char *Linea;
	FILE *archivo;
		
	// Creo arreglo de auxiliares:
	LISTA *Aux[numeroProcesos];
	
	// Creo el temporal:
	
	LISTA *temporal;
	
	if ( (fopen(nombre_archivo,"r")) == NULL) {

		perror("Error: El archivo indicado no fue encontrado ");
		printf("errno = %d. \n",errno);
		exit(1);
	}
	
	else {
		
		archivo = fopen(nombre_archivo,"r");
		
		// Inicializo las cabeceras y sus respectivos auxiliares
		int i;
		for(i=0; i < numeroProcesos; i++){
			Arreglo[i]=NULL;
			Aux[i]=NULL;
		}
		
		// Se inicializan el numeroElementos:
		
		for(i=0; i < numeroProcesos; i++){
			numeroElementos[i]=0;
		}
		
		// Empiezo a leer el archivo:
		
		while(1){
			// Se reserva el espacio de memoria para la nueva linea
			Linea = (char*)malloc(sizeof(char)*101);
			fscanf(archivo, " %[^\n]\n" ,Linea);
			//printf("%s\n",Linea);
			temporal =(LISTA*)malloc(sizeof(LISTA));
			
			temporal->nombre = Linea;
			resto = numeroLinea%numeroProcesos;
			
			// Caso en el que temporal es el primer elemento de la lista
			
			if (Aux[resto] == NULL) {
				Arreglo[resto] = temporal;
				Aux[resto] = temporal;
				numeroElementos[resto]++;
			}

			// Caso en el que temporal no es el primer elemento de la lista
			else {
				Aux[resto]->siguiente=temporal;	
				Aux[resto]=temporal;
				numeroElementos[resto]++;
			}
			numeroLinea++;
			
			// Se verifica si se ha llegado al fin del archivo
			if (feof(archivo)) {
				break;
				
				}
			}
		}
	}
	

int lineasArchivo(char *nombre_archivo) {

	/*
	*
	* Definicion de la funcion:	
	*
	* Parametros de entrada:
	*
	* Parametros de salida:
	*
	*/

	FILE *archivo;
	char caracter;
	int lineas = 0;

	if ( (fopen(nombre_archivo,"r")) == NULL) {

		perror("Error: El archivo indicado no fue encontrado ");
		printf("errno = %d. \n",errno);
		exit(1);
	}

	else { 

		archivo = fopen(nombre_archivo,"r");

		while(!feof(archivo)) {

			caracter = fgetc(archivo);

			if (caracter == '\n') {
				lineas++;
			}

		}

	}

	return lineas; 

}

void ListaEnlazadaArreglo(LISTA *listaEnlazada,int size,char *Array[]){
	
		int i;
		int Contador=0;
		LISTA *temp;
		temp = listaEnlazada;
		char *Arreglo[size+1];
						
		while(temp!=NULL){
			
			//Arreglo[Contador]=temp->nombre;
			Array[Contador]=temp->nombre;
			//printf("Estoy en la funcion %s \n",Array[Contador]);
			temp=temp->siguiente;
			Contador++;
				
			}
		Array[size]=NULL;
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

		}


	}

	else {
//
		printf("Error: No se han pasado los argumentos de forma correcta.\n");

	}



	int numeroLineas = lineasArchivo(archivoEntrada);

	printf("numero lineas es %d \n",numeroLineas);

	LISTA *apuntador; 

	// Creo arreglo de cabeceras:
	int numElementos[numeroProcesos];
	LISTA *Cabecera[numeroProcesos];
	LeerArchivo(archivoEntrada,numeroProcesos,Cabecera,numElementos);
	int i;
	int j;
	
	for(i=0;i<numeroProcesos;i++){
		
		char *Array[numElementos[i]*1];
		ListaEnlazadaArreglo(Cabecera[i],numElementos[i],Array);
		
		// Imprimo los elementos de la cabecera:
		for(j =0; j<numElementos[i];j++){
			
			//LISTA *temp;
			//temp = Cabecera[i];
			//printf("Soy el arreglo %s \n",Array[j]);				
			//printf("Soy el numero %d  \n",i);
			//printf("El numero de elementos de mi arreglo es %d  \n",numElementos[i]);
			//while(temp!=NULL){
			//	printf("%s \n",temp->nombre)
			//	temp=temp->siguiente;
			
			}
			
			
		//printf("%s",prueba->nombre);

		pid_t childpid;
		childpid = fork();

		if (childpid == 0) {
			
			if (execvp("./map",Array) < 0) {
				perror("Fallo en la ejecucion de exec");
				exit(1);	
			}
			
		}
		//else{
		//		wait();	
		//		} 
		
	}
		
	for(i=0;i<numeroProcesos;i++){
		wait();
		printf("Mi hijo termino \n");
		
		}
}

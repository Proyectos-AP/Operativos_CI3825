/*
*
* Archivo: Main.c 
*
* Nombres:
*	Alejandra Cordero / Carnet: 12-10645
*	Pablo Maldonado   / Carnet: 12-10561
*
* Descripcion: Clase Principal (Arreglar)
*
* Ultima modificacion: 14/04/2015
*
*/


// Directivas de Preprocesador:

#include <stdio.h>
#include <stdlib.h>

// Definicion de Tipos:

typedef struct pregunta { 

	int codigo; 
	int nivel;
	char area;
	char *pregunta[100];
	char *opcion1[100];
	char *opcion2[100];
	char *opcion3[100];
	int respuesta;
	struct pregunta *siguiente;

} PREGUNTA; 


// Definicion de funciones: 


// Inicio del codigo principal:

int main(int argc, char *argv[]) {

	while (1) {

		int respuesta;

		printf("Bienvenido a la base de datos de preguntas: \n");

		printf("Sus opciones son:\n");

		printf("0.- Leer la base de datos.\n");
		printf("1.- Consultar todas las preguntas que estan en la base de datos.\n");
		printf("2.- Consultar todas las preguntas de un determinado nivel de complejidad.\n");
		printf("3.- Eliminar una pregunta.\n");
		printf("4.- Insertar una pregunta.\n");
		printf("5.- Salvar la base de datos.\n");
		printf("6.- Salir.\n");


		// Como hacer esto robusto (pendiente)
		printf("Inserte la opcion que desea ejecutar: ");
		scanf("%d",&respuesta); 

		// Leer la base de datos:
		if (respuesta == 0) {

			FILE *archivo;
			int codigo,complejidad;
			char tipo;
			char pregunta[100];


			if ( (fopen(argv[1],"r")) == NULL) {
				printf("Error: El archivo no existe\n");
				printf("El programa finalizara su ejecucion.\n");
				return 0;
			}

			else {

				archivo = fopen(argv[1],"r");

				//fscanf(archivo,"%d %d %c %s",&codigo,&complejidad,&tipo,pregunta);

				//printf("%d %d %c %s\n",codigo,complejidad,tipo,pregunta);

				//fclose(archivo);

				int in_char;	

				while((in_char = getc(archivo)) != EOF ) {

					printf("%c",in_char);

				}

			}

		}

		// Consultar todas las preguntas que estan en la base de datos:
		else if (respuesta == 1) {


		}

		// Consultar todas las preguntas de un determinado nivel de complejidad:
		else if (respuesta == 2) {

		}

		// Eliminar una pregunta:
		else if (respuesta == 3) {

		}

		// Insertar una pregunta:
		else if (respuesta == 4) {

		}

		// Salvar la base de datos:
		else if (respuesta == 5) {


		}

		// Salir:
		else if (respuesta == 6) {

			// Se tiene que guardar la partida antes de salir

			printf("El programa finalizara su ejecucion.\n");
			return 0;

		}

		// Respuesta invalida: 
		else {
			printf("\nError: La respuesta no es valida.\n");
			printf("Intente de nuevo.\n");

		}

		printf("\n");

	}


}
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
* Ultima modificacion: 16/04/2015
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
	char *pregunta[101];
	char *opcion1[101];
	char *opcion2[101];
	char *opcion3[101];
	int respuesta;
	struct pregunta *siguiente;

} PREGUNTA; 


// Definicion de funciones: 


// Inicio del codigo principal:

int main(int argc, char *argv[]) {

	char respuesta;

	while (1) {

		printf("Bienvenido a la base de datos de preguntas: \n");
		printf("Sus opciones son:\n");
		printf("0.- Leer la base de datos.\n");
		printf("1.- Consultar todas las preguntas que estan en la base de datos.\n");
		printf("2.- Consultar todas las preguntas de un determinado nivel de complejidad.\n");
		printf("3.- Eliminar una pregunta.\n");
		printf("4.- Insertar una pregunta.\n");
		printf("5.- Salvar la base de datos.\n");
		printf("6.- Salir.\n");
	

		// Entrada de datos:
		printf("Inserte la opcion que desea ejecutar: ");
		scanf(" %c",&respuesta);

		switch (respuesta) {

			// Leer la base de datos:
			case '0':
				; // Declaracion despues de label
				FILE *archivo;
				int codigo,complejidad;
				char tipo;
				char pregunta[100];


				if ( (fopen(argv[1],"r")) == NULL) {
					printf("\nError: El archivo indicado no fue encontrado\n");
					printf("El programa finalizara su ejecucion.\n");
					return 0;
				}

				else {

					archivo = fopen(argv[1],"r");

					//fscanf(archivo,"%d %d %c \"%s\" ",&codigo,&complejidad,&tipo,pregunta);

					//printf("%d %d %c %s\n",codigo,complejidad,tipo,pregunta);

					//int in_char;	

					//while((in_char = getc(archivo)) != EOF ) {

						//printf("%c",in_char);

					//}

					//printf("\nLos datos han sido cargados correctamente a memoria.\n");
					fclose(archivo);

				}
				
				break;

			// Consultar todas las preguntas que estan en la base de datos:
			case '1':

				printf("\nLas preguntas que se encuentran la base de datos son:\n");


			break;

			// Consultar todas las preguntas de un determinado nivel de complejidad:
			case '2':
				; // Declaracion despues de label
				char resp_complejidad;

				printf("\nIntroduzca el nivel de complejidad,\n");
				printf("(0 Basico, 1 Intermedio, 2 Avanzado): ");
				// Revisar: este scanf no esta funcionando
				scanf("%c",&resp_complejidad);
				printf("\n");			


			break;

			// Eliminar una pregunta:
			case '3':
			break;

			// Insertar una pregunta:
			case '4':
			break;

			// Salvar la base de datos:
			case '5':
			break;

			// Salir:
			case '6':
				// Se tiene que guardar la partida antes de salir
				printf("El programa finalizara su ejecucion.\n");
				return 0;
			break;

			// Respuesta invalida: 
			default:
				printf("\nError: La respuesta no es valida.\n");
				printf("Intente de nuevo.\n");
			break;

		}

		printf("\n");

	}

}


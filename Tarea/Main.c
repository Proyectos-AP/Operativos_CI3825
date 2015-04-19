/*
*
* Archivo: Main.c 
*
* Nombres:
*	Alejandra Cordero / Carnet: 12-10645
*	Pablo Maldonado   / Carnet: 12-10561
*
* Descripcion: Interfaz por consola
* que presenta al usuario un conjunto
* de opciones para el manejo de una base
* de datos de preguntas almacenadas en un
* archivo de texto plano.
*
* Ultima modificacion: 17/04/2015
*
*/


// Directivas de Preprocesador:

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib_archivos.h"

int main(int argc, char *argv[]) {

	printf("Bienvenido a la base de datos de preguntas: \n");

	// archivoLeido es 0 si no se ha leido la base de 
	// datos y es 1 si se ha leido.
	int archivoLeido = 0;

	//archivoSalvado es 0 si no se ha salvado la 
	//base de datos y 1 si se ha salvado.
	int archivoSalvado = 0;
	char respuesta;
	PREGUNTA *cabeceraFile;

	while (1) {

		printf("Menu de opciones: \n");
		printf("  0.- Leer la base de datos.\n");
		printf("  1.- Consultar todas las preguntas que estan en la base de datos.\n");
		printf("  2.- Consultar todas las preguntas de un determinado nivel de complejidad.\n");
		printf("  3.- Eliminar una pregunta.\n");
		printf("  4.- Insertar una pregunta.\n");
		printf("  5.- Salvar la base de datos.\n");
		printf("  6.- Salir.\n");
	
		// Entrada de datos:
		printf("Inserte la opcion que desea ejecutar: ");
		scanf(" %c",&respuesta);

		switch (respuesta) {

			// Leer la base de datos:
			case '0':
				; // Declaracion despues de label

				if (archivoLeido == 0) {
					cabeceraFile = LeerBaseDeDatos(argv[1]);
					archivoLeido = 1;
				}

				else {
					printf("\n--------------------\n");
					printf("\nLas preguntas ya se encuentran cargadas en memoria.\n");
					printf("\n--------------------\n\n");

				}
				
				break;

			// Consultar todas las preguntas que estan en la base de datos:
			case '1':
				; // Declaracion despues de label


				if (archivoLeido == 0) {
					printf("\n--------------------\n");
					printf("\nError: Las preguntas no han sido cargadas en memoria.\n");
					printf("\n--------------------\n\n");

				}

				else {

					imprimirPreguntas(cabeceraFile);

				}


			break;

			// Consultar todas las preguntas de un determinado nivel de complejidad:
			case '2':
				; // Declaracion despues de label
				char resp_complejidad;

				

				if (archivoLeido == 0) {
					printf("\n--------------------\n");
					printf("\nError: Las preguntas no han sido cargadas en memoria.\n");
					printf("\n--------------------\n\n");
				}

				else {

					char resp_complejidad;

					while(1) {

						printf("\nIntroduzca el nivel de complejidad,\n");
						printf("(0 Basico, 1 Intermedio, 2 Avanzado): ");
						scanf(" %c",&resp_complejidad);
						printf("\n");	

						if ( resp_complejidad >= 48 && resp_complejidad <= 50) {
							break;
						}

						printf("\nError: La respuesta no es valida.\n");
						printf("Intente de nuevo.\n");

					}

					imprimirPreguntasComplejidad(cabeceraFile,resp_complejidad);

				}

				

			break;

			// Eliminar una pregunta:
			case '3':
				; // Declaracion despues de label

				if (archivoLeido == 0) {
					printf("\n--------------------\n");
					printf("\nError: Las preguntas no han sido cargadas en memoria.\n");
					printf("\n--------------------\n\n");
				}

				else {

					int claveAeliminar;

					printf("\nIntroduzca la clave de la pregunta que desea eliminar: ");
					scanf(" %d",&claveAeliminar);
					printf("\n");
					Eliminar(cabeceraFile,claveAeliminar);
					archivoSalvado = 0;
					
				}
				
			break;

			// Insertar una pregunta:
			case '4':
				; // Declaracion despues de label

				if (archivoLeido == 0) {
					cabeceraFile = LeerBaseDeDatos(argv[1]);	
					archivoLeido = 1;	

				}

				insertarPregunta(cabeceraFile);
				archivoSalvado = 0;


			break;

			// Salvar la base de datos:
			case '5':
				; // Declaracion despues de label

				if (archivoLeido == 0) {
					cabeceraFile = LeerBaseDeDatos(argv[1]);	
					archivoLeido = 1;	

				}

				escribirArchivo(cabeceraFile,argv[1]);
				archivoSalvado = 1;
				printf("\n--------------------\n");
				printf("\nSe ha guardado la base de datos de preguntas en %s \n",argv[1]);
				printf("\n--------------------\n\n");
			break;

			// Salir:
			case '6':

				if (archivoSalvado == 0) {

					if (archivoLeido == 0) {
						cabeceraFile = LeerBaseDeDatos(argv[1]);	
						archivoLeido=1;	

					}

					printf("\nSe ha guardado la base de datos de preguntas en %s \n",argv[1]);
					escribirArchivo(cabeceraFile,argv[1]);


				}

				printf("El programa finalizara su ejecucion.\n");
				exit(0);
			break;

			// Respuesta invalida: 
			default:
				printf("\n--------------------\n");
				printf("\nError: La respuesta no es valida.\n");
				printf("Intente de nuevo.\n");
				printf("\n--------------------\n\n");
			break;

	
		printf("\n");

	}

}

}


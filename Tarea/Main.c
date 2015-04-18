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
* Ultima modificacion: 17/04/2015
*
*/


// Directivas de Preprocesador:

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definicion de Tipos:

typedef struct pregunta { 

	int codigo; 
	int nivel;
	char area;
	char *pregunta;
	char *opcion1;
	char *opcion2;
	char *opcion3;
	int respuesta;
	struct pregunta *siguiente;

} PREGUNTA; 


// Definicion de funciones: 

void CambiarSaltoDeLinea(char *frase) {
	// Descripcion de la funcion:
	char *ptr_Caracter;
	ptr_Caracter = strchr(frase,10);
	if (ptr_Caracter != NULL){
		//Si hay un caracter nulo se agrega un espacio en blanco
       	*ptr_Caracter=32;
    }
}



//------------------------------------------------------------//


PREGUNTA* LeerBaseDeDatos(char *nombre_archivo) {
	// Descripcion de la funcion:

	if ( (fopen(nombre_archivo,"r")) == NULL) {
		printf("\nError: El archivo indicado no fue encontrado\n");
		printf("El programa finalizara su ejecucion.\n");
		exit(1);
	}

	else {

		FILE *archivo;
		archivo = fopen(nombre_archivo,"r");
		int codigo,complejidad;
		char tipo;
		int resp_correcta;
		PREGUNTA *primeraPregunta;
		PREGUNTA *temporal;
		PREGUNTA *anterior;


		//Se crea la cabecera
		primeraPregunta=(PREGUNTA*)malloc(sizeof(PREGUNTA));
		primeraPregunta->siguiente=NULL;
		anterior=primeraPregunta;

		while (1)  {

			// Verifica si se ha llegado al fin del archivo
			// o si esta vacio para no reservar espacio
			// de memoria innecesario
			if (feof(archivo)) {
				break;
			}
						

			// Se reserva el espacio de memoria para cada
			// pregunta y sus posibles respuestas:

			char *pregunta1;
			pregunta1 = (char*)malloc(sizeof(char)*101);
			char *resp_1;
			resp_1 = (char*)malloc(sizeof(char)*101);

			char *resp_2;	
			resp_2 = (char*)malloc(sizeof(char)*101);

			char *resp_3;
			resp_3 = (char*)malloc(sizeof(char)*101);

			char *pch;										  
			fscanf(archivo," %d %d %c \"%[^\"]\" \"%[^\"]\" \"%[^\"]\" \"%[^\"]\" %d ", \
			&codigo,&complejidad,&tipo,pregunta1,resp_1,resp_2,resp_3,&resp_correcta);

			//Se verifica si hay un salto de linea
			CambiarSaltoDeLinea(pregunta1);
			CambiarSaltoDeLinea(resp_1);
			CambiarSaltoDeLinea(resp_2);
			CambiarSaltoDeLinea(resp_3);


			// Hay que verificar si el codigo de la pregunta ya existe
			// tambien hay que verificar el tema
			temporal=(PREGUNTA*)malloc(sizeof(PREGUNTA));
			temporal->codigo=codigo;
			temporal->nivel=complejidad;
			temporal->area=tipo;
			temporal->pregunta=pregunta1;
			temporal->opcion1=resp_1;
			temporal->opcion2=resp_2;
			temporal->opcion3=resp_3;
			temporal->respuesta=resp_correcta;
			temporal->siguiente=NULL;
			anterior->siguiente=temporal;
			anterior=temporal;
		 	
		}

		printf("\nLos datos han sido cargados correctamente a memoria.\n");
		fclose(archivo);

		return primeraPregunta;

	}


}

//------------------------------------------------------------//


void imprimirPreguntas(PREGUNTA *CabeceraArchivo) {

	PREGUNTA *aux;
				
	aux = CabeceraArchivo->siguiente;

	printf("\nLas preguntas que se encuentran la base de datos son:\n\n");
	while (aux != NULL) {

		printf("Pregunta: %s? \n",aux->pregunta);
		printf("Codigo: %d / Complejidad: %d / Area: %c \n",aux->codigo,aux->nivel,aux->area);
		printf("Las opciones son: \n");
		printf("	1.- %s. \n",aux->opcion1);
		printf("	2.- %s. \n",aux->opcion2);
		printf("	3.- %s. \n",aux->opcion3);
		printf("Respuesta correcta: %d \n",aux->respuesta);
		printf("\n--------------------\n\n");
		aux = aux->siguiente;

	}

}

//------------------------------------------------------------//

void imprimirPreguntasComplejidad(PREGUNTA *CabeceraArchivo,char complejidad) {

	int num_complejidad = (int)(complejidad-48); 
	PREGUNTA *aux;				
	aux = CabeceraArchivo->siguiente;

	printf("\nLas preguntas que se encuentran la base de datos son:\n\n");
	while (aux != NULL) {

		if (aux->nivel==num_complejidad) {

			printf("Pregunta: %s? \n",aux->pregunta);
			printf("Codigo: %d / Complejidad: %d / Area: %c \n",aux->codigo,aux->nivel,aux->area);
			printf("Las opciones son: \n");
			printf("	1.- %s. \n",aux->opcion1);
			printf("	2.- %s. \n",aux->opcion2);
			printf("	3.- %s. \n",aux->opcion3);
			printf("Respuesta correcta: %d \n",aux->respuesta);
			printf("\n--------------------\n\n");

		}

		aux = aux->siguiente;

	}

}

//------------------------------------------------------------//


// Inicio del codigo principal:

int main(int argc, char *argv[]) {

	int archivoLeido = 0;
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

				PREGUNTA *CabeceraArchivo; 
				CabeceraArchivo = LeerBaseDeDatos(argv[1]);			
				archivoLeido = 1;
				
				break;

			// Consultar todas las preguntas que estan en la base de datos:
			case '1':
				; // Declaracion despues de label


				if (archivoLeido == 0) {
					CabeceraArchivo = LeerBaseDeDatos(argv[1]);	

				}

				imprimirPreguntas(CabeceraArchivo);

			break;

			// Consultar todas las preguntas de un determinado nivel de complejidad:
			case '2':
				; // Declaracion despues de label
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

				if (archivoLeido == 0) {
					CabeceraArchivo = LeerBaseDeDatos(argv[1]);	

				}

				imprimirPreguntasComplejidad(CabeceraArchivo,resp_complejidad);

			break;

			// Eliminar una pregunta:
			case '3':
				; // Declaracion despues de label
			break;

			// Insertar una pregunta:
			case '4':
				; // Declaracion despues de label

			break;

			// Salvar la base de datos:
			case '5':
				; // Declaracion despues de label

			/*
				for () {

				}

			*/


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
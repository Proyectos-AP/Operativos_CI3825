/*
*
* Archivo: lib_archivos.c
*
* Nombres:
*	Alejandra Cordero / Carnet: 12-10645
*	Pablo Maldonado   / Carnet: 12-10561
*
* Descripcion: Libreria de funciones para
* el manejo de una base de datos de preguntas
* almacenadas en un archivo de texto plano.
* 
* Ultima modificacion: 26/04/2015
*
*/

// Directivas de preprocesador:

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

// Definicion de tipos:

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
	/* Descripcion de la funcion:
		Esta funcion dada una frase verifica si esta 
	contiene un salto de linea, de ser asi, este salto 
	de linea es remplzado por un espacio en blanco
	*/

	// Declaracion de variables:
	char *ptr_Caracter;

	// Se verifica si la frase contiene un espacio en blanco
	ptr_Caracter = strchr(frase,10);

	if (ptr_Caracter != NULL){
		// Si hay un caracter nulo se agrega un espacio en blanco
       	*ptr_Caracter=32;
    }
}

//------------------------------------------------------------//


PREGUNTA* LeerBaseDeDatos(char *nombre_archivo) {
	/*  Descripcion de la funcion:
		Esta funcion dado el nombre de un archivo lee todo su 
	contenido.Si el archivo no esta vacio se crea una lista enlazada 
	de elementos de tipo PREGUNTA, este contiene los siguientes 
	(componentes):
	 		-codigo: Entero que representa el codigo de una 
	 				 pregunta.
	 		-nivel:  Entero que representa el nivel de dificultad 
	 				 de una pregunta.
	 		-area:   Char que representa el area de conocimiento
	 				 a la que pertenece una pregunta.
	 		-pregunta: String que representa la pregunta
	 		-opcion1:  String que representa una posible respuesta 1
	 		-opcion2:  String que representa una posible respuesta 2
	 		-opcion3:  String que representa una posible respuesta 3
	 		-respuesta: Entero que representa cual es el numero
	 					de la opcion con la respuesta correcta.
	 		-siguiente: Apuntador al siguiente elemento de tipo 
	 					PREGUNTA
	 Si el archivo esta vacio la funcion solo creara un apuntador a 
	 null. 
	*/

	// Declaracion de variables a utilizar:
	PREGUNTA *primeraPregunta;
	PREGUNTA *temporal;
	PREGUNTA *anterior;
	FILE *archivo;
	int codigo,complejidad,resp_correcta;
	char tipo; // Variable que almacenara el area de la pregunta
		   // ingresada por el usuario.

	if ( (fopen(nombre_archivo,"r")) == NULL) {

		perror("Error: El archivo indicado no fue encontrado ");
		printf("errno = %d. \n",errno);
		exit(1);

	}
	
	else {
		archivo = fopen(nombre_archivo,"r");

		primeraPregunta = NULL;
		anterior = primeraPregunta;

		// Se verifica si el archivo esta vacio
		fseek(archivo,0,SEEK_END);

   		if (ftell(archivo) == 0 )	{
   			printf("\n--------------------\n");
   			printf("\nAviso: El archivo esta vacio.\n");
   			printf("\n--------------------\n\n");

    	}

    	else {

    		fseek(archivo,0,SEEK_SET);

			while (1)  {

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
										  
				fscanf(archivo," %d %d %c \"%[^\"]\" \"%[^\"]\" \"%[^\"]\" \"%[^\"]\" %d ", \
				&codigo,&complejidad,&tipo,pregunta1,resp_1,resp_2,resp_3,&resp_correcta);

				// Se verifica si hay un salto de linea para ser cambiado 
				// por un espacio en blanco
				CambiarSaltoDeLinea(pregunta1);
				CambiarSaltoDeLinea(resp_1);
				CambiarSaltoDeLinea(resp_2);
				CambiarSaltoDeLinea(resp_3);


				// Se asigna la informacion leida al espacio de una nueva pregunta:
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

				// Caso en el que temporal es el primer elemento de la lista
				if (anterior == NULL) {

					primeraPregunta = temporal;
					anterior = temporal;

				}

				// Caso en el que temporal no es el primer elemento de la lista
				else {
					anterior->siguiente=temporal;	
					anterior=temporal;
				}
				

				// Se verifica si se ha llegado al fin del archivo
				if (feof(archivo)) {
						break;
					}
			}

			printf("\n--------------------\n");
			printf("\nLos datos han sido cargados correctamente a memoria.\n");
			printf("\n--------------------\n\n");
			fclose(archivo);
		}

		return primeraPregunta;

	}
	
}

//------------------------------------------------------------//

void EliminarLista(PREGUNTA *CabeceraData){

	/*  Descripcion de la funcion:
		Esta funcion destruye una lista enlazada de una 
	estructura de datos de tipo PREGUNTA.

	*/

	//Declaracion de variables:
	PREGUNTA *aux;

	// Caso en donde la estructura no tiene elementos
	if(CabeceraData == NULL) {
		;
	}

	// Caso en donde la estructura tiene al menos un elemento
	else {

		while(CabeceraData!=NULL){

			aux=CabeceraData;
			CabeceraData=aux->siguiente;
			free(aux->pregunta);
			free(aux->opcion1);
			free(aux->opcion2);
			free(aux->opcion3);
			free(aux);
		}

	}
}

//------------------------------------------------------------//


void imprimirPreguntas(PREGUNTA *CabeceraArchivo) {

	/*  Descripcion de la funcion:
			Esta funcion dado un apuntador a una lista 
			enlazada de preguntas, imprime todas los 
			elementos que se encuentran en ella.
	*/

	// Declaracion de variables:
	PREGUNTA *aux;
	
	// Inicializacion de variables:			
	aux = CabeceraArchivo;

	if (aux == NULL) {
		printf("\n--------------------\n");
		printf("\nAviso: No hay preguntas para mostrar.\n");
		printf("\n--------------------\n\n");
	}

	else { 

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


}

//------------------------------------------------------------//

void imprimirPreguntasComplejidad(PREGUNTA *CabeceraArchivo,char complejidad) {
	/*  Descripcion de la funcion:
		Esta funcion imprime las preguntas almacenadas en una 
	estructura de datos llamada PREGUNTAS que poseen un cierto nivel
	de complejidad.
	*/

	// Declaracion de variables:
	PREGUNTA *aux;				
	aux = CabeceraArchivo;

	// Inicializacion de variables:
	int num_complejidad = (int)(complejidad-48); 
	int existePregunta = 0;

	while (aux != NULL) {

		if (aux->nivel==num_complejidad) {

			existePregunta = existePregunta + 1;

			if (existePregunta == 1) {
				
				printf("\nLas preguntas que se encuentran la base de datos son:\n\n");
			}

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

	if (existePregunta == 0) {

		printf("\n--------------------\n");
		printf("\nAviso: No hay preguntas cargadas en memoria ");
		printf("con la complejidad dada\n");			
		printf("\n--------------------\n");


	}
	
}

//------------------------------------------------------------//

int Eliminar(PREGUNTA** Cabecera,int clave) {

	/*  Descripcion de la funcion:
		Esta funcion dada una clave y la direccion de memoria
		del apuntador al primer elemento de la lista enlazada, 
		elimina al elemento de la misma que posea dicha clave.
		Devuelve un entero que indica si la eliminacion fue 
		exitosa o no.
	*/

	// Declaracion de variables:
	PREGUNTA *aux;
	PREGUNTA *anterior;	
	int SeElimino = 0;
	anterior= *Cabecera;			
	aux = *Cabecera;

	if (aux == NULL) {
		;
	}

	else {
		// Caso en donde se elimina el primer elemento de la lista.
		if (aux->codigo == clave) {
	
			*Cabecera = aux->siguiente;
			free(aux->pregunta);
			free(aux->opcion1);
			free(aux->opcion2);
			free(aux->opcion3);
			free(aux);
			SeElimino = 1;

			printf("\n--------------------\n");
			printf("\nLa pregunta %d",clave);
			printf(" ha sido eliminada\n");
			printf("\n--------------------\n\n"); 


	}
		// Caso en donde se elimina un elemento de la lista
		// (Este elemento no es el primero de la lista).
		else {

			while (aux != NULL) {

				if (aux->codigo == clave) {
					anterior->siguiente=aux->siguiente;
					aux->siguiente=NULL;
					free(aux->pregunta);
					free(aux->opcion1);
					free(aux->opcion2);
					free(aux->opcion3);
					free(aux);

					printf("\n--------------------\n");
					printf("\nLa pregunta %d",clave);
					printf(" ha sido eliminada\n");
					printf("\n--------------------\n\n"); 
					SeElimino = 1;
					return SeElimino;

				}
				anterior = aux;
				aux = aux->siguiente;
			}		
		}

	}
	
	return SeElimino;

}

//------------------------------------------------------------//

void insertarPregunta(PREGUNTA** CabeceraArchivo) {
	/*  Descripcion de la funcion:
		Esta funcion inserta una nueva pregunta en una lista
	enlazada de elementos de tipo PREGUNTA.
	*/

	// Declaracion de variables:
	PREGUNTA *aux;				
	PREGUNTA *nueva_pregunta;
	int codigo_nuevo; 
	int nivel_nuevo;
	char area_nueva;
	int respuesta_nueva;
	int codigoEsta;
	char confirmacion;
	int guardarPregunta;

	// Inicializacion de variables:
	aux = *CabeceraArchivo;
	guardarPregunta = 0;

	char *pregunta_nueva;
	pregunta_nueva = (char*)malloc(sizeof(char)*101);

	char *opcion1_nueva;
	opcion1_nueva = (char*)malloc(sizeof(char)*101);

	char *opcion2_nueva;
	opcion2_nueva = (char*)malloc(sizeof(char)*101);

	char *opcion3_nueva;
	opcion3_nueva = (char*)malloc(sizeof(char)*101);


	//Se pide el codigo de la pregunta al usuario

	while(1) {

		printf("\nIntroduzca el codigo correspondiente a su pregunta: ");

		if (scanf(" %d",&codigo_nuevo) == 0) {

			printf("\nError: No ha ingresado un entero. \n");
			printf("El programa finalizara su ejecucion\n");
			exit(1);

		}

		else {

			codigoEsta = verificarCodigo(*CabeceraArchivo,codigo_nuevo);

			if (codigoEsta == 0) {
				break;
			}

			printf("\n--------------------\n");
			printf("\nError: El codigo de la pregunta ya existe.");
			printf(" Intente de nuevo.\n");
			printf("\n--------------------\n");
					
		}

	}
		
	// Se pide el nivel de complejidad al usuario

	while(1) {


		printf("\nIntroduzca el nivel de complejidad de su pregunta\n");
		printf("(0 Basico, 1 Intermedio, 2 Avanzado): ");

				
		if (scanf(" %d",&nivel_nuevo) == 0) {

			printf("\nError: No ha ingresado un entero. \n");
			printf("El programa finalizara su ejecucion\n");
			exit(1);

		}

		else {

			if ( nivel_nuevo >= 0 && nivel_nuevo <= 2) {

				break;
			}

			printf("\nError: La respuesta no es valida.\n");
			printf("Intente de nuevo.\n");
					
		}

	}

	// Se pide el tema correspondiente de la pregunta 
	// ingresada por el usuario
	while(1) {


		printf("\nIntroduzca el caracter correspondiente al tema de su pregunta\n");
		printf("(H - Historia, C - Cine, L - Literatura, G - Geografia\n");
		printf("M - Matematicas, T - Tecnologia e Informatica): ");
		scanf(" %c",&area_nueva);	

		if ( area_nueva == 'H' || area_nueva == 'C' || area_nueva == 'L' || \
			area_nueva == 'G' || area_nueva == 'M' || area_nueva == 'T')  {
				break;
		}

		printf("\nError: La respuesta no es valida.\n");
		printf("Intente de nuevo.\n");

	}

	// Se pide la pregunta 
	printf("\nIntroduzca su pregunta entre comillas y sin signo de interrogacion\n");
	printf("(Ejemplo: \"Cuál es la capital de Alemania\"): ");
	scanf(" \"%100[^\"]\"",pregunta_nueva);

	// Se pide la opcion 1
	printf("\nIntroduzca su primera opcion de respuesta entre comillas \n(Ejemplo: \"Hamburg\"): ");
	scanf(" \"%100[^\"]\"",opcion1_nueva);

	// Se pide la opcion 2
	printf("\nIntroduzca su segunda opcion de respuesta entre comillas \n(Ejemplo: \"Stuttgart\"): ");
	scanf(" \"%100[^\"]\"",opcion2_nueva);

	// Se pide la opcion 3
	printf("\nIntroduzca su tercera posible respuesta entre comillas \n(Ejemplo: \"Berlin\"): ");
	scanf(" \"%100[^\"]\"",opcion3_nueva);


	// Se solicita el numero de la opcion que sera marcada como
	// respuesta correcta
	while(1) {


		printf("\nIntroduzca el numero correspondiente a la respuesta correcta\n");
		printf("(1 - primera opcion, 2 - segunda opcion, 3 - tercera opcion): ");

		if (scanf(" %d",&respuesta_nueva) == 0) {

			printf("\nError: No ha ingresado un entero.\n");
			printf("El programa finalizara su ejecucion\n");
			exit(1);

		}


		else {

			if ( respuesta_nueva >= 1 && respuesta_nueva <= 3) {
				break;

			}

			printf("\nError: La respuesta no es valida.\n");
			printf("Intente de nuevo.\n");
					
		}

	}

	printf("Los datos introducidos son:\n");
	printf("  - Codigo: %d \n",codigo_nuevo);
	printf("  - Complejidad: %d\n",nivel_nuevo);
	printf("  - Area: %c \n",area_nueva);
	printf("  - Pregunta: %s \n",pregunta_nueva);
	printf("  - Opcion 1: %s \n",opcion1_nueva);
	printf("  - Opcion 2: %s \n",opcion2_nueva);
	printf("  - Opcion 3: %s \n",opcion3_nueva);
	printf("  - Respuesta: %d \n",respuesta_nueva);

	// Se solicita confirmacion al usuario.
	while(1) {

		printf("Desea guardar la siguiente informacion? [Y/n]: ");
		scanf(" %c",&confirmacion);

		if (confirmacion == 'y' | confirmacion == 'Y') {
			guardarPregunta = 1;
			break;
		}

		else if (confirmacion == 'n' | confirmacion == 'N') {
			// El usuario desea introducir los datos de nuevo
			break;

		}

	}

				
	// Se guarda la pregunta en memoria.
	if (guardarPregunta == 1) {

		nueva_pregunta = (PREGUNTA*)malloc(sizeof(PREGUNTA));

		// Se almacena en la estructura de datos toda la 
		// informacion suministrada por el usuario
		nueva_pregunta->codigo=codigo_nuevo;
		nueva_pregunta->nivel=nivel_nuevo;
		nueva_pregunta->area=area_nueva;
		nueva_pregunta->pregunta=pregunta_nueva;
		nueva_pregunta->opcion1=opcion1_nueva;
		nueva_pregunta->opcion2=opcion2_nueva;
		nueva_pregunta->opcion3=opcion3_nueva;
		nueva_pregunta->respuesta=respuesta_nueva;
		nueva_pregunta->siguiente=NULL;

		// Se agrega la nueva pregunta a la lista enlazada
		if (aux == NULL) {
			*CabeceraArchivo = nueva_pregunta;	
		}

		else {

			while (aux->siguiente != NULL) {
				aux = aux->siguiente;
			}

		aux->siguiente = nueva_pregunta;
			
		}
		
		printf("\n--------------------\n");
		printf("\nSu pregunta ha sido agregada a la base de datos.\n");
		printf("\n--------------------\n\n");
				
	}

	// No se guardara la pregunta en la estructura
	else {

		// Se liberan los espacios de memoria reservados
		free(pregunta_nueva);
		free(opcion1_nueva);
		free(opcion2_nueva);
		free(opcion3_nueva);		

	}

}

//------------------------------------------------------------//


void escribirArchivo(PREGUNTA *CabeceraArchivo,char *nombre_archivo) {
	/*  Descripcion de la funcion:
		Dada una estructura de datos de tipo PREGUNTA 
	y el nombre de un archivo esta funcion creara un archivo
	con el nombre dado y escribira en el archivo toda la 
	informacion almacenada en la estructura de datos suministrada.
	*/

	// Declaracion de variables:
	PREGUNTA *aux;				
	aux = CabeceraArchivo;
	FILE *archivoSalida;

	archivoSalida = fopen(nombre_archivo,"w");

		while (aux != NULL) {

			fprintf(archivoSalida,"%d %d %c \"%s\" \"%s\" \"%s\" \"%s\" %d \n", \
				aux->codigo,aux->nivel,aux->area,aux->pregunta, \
				aux->opcion1,aux->opcion2,aux->opcion3,aux->respuesta);

			aux = aux->siguiente;

		}

	fclose(archivoSalida);

	}


//------------------------------------------------------------//
int verificarCodigo(PREGUNTA *CabeceraArchivo, int codigo) {
/* Descripcion de la funcion:
	Dado un entero, la funcion verifica si este corresponde
	al codigo de alguna de las preguntas que se encuentran
	en la lista enlazada dada.
	Si el codigo de la pregunta se encuentra en alguna de las
	preguntas almacenadas en la estructura la funcion devuelve 1
	(True).Por otra parte, si el codigo de la pregunta no se encuentra 
	en la estructura la funcion devuelve 0 (False).
*/
	//Declaracion de variables:
	int EstaCodigo = 0;

	if (CabeceraArchivo == NULL) {
		return EstaCodigo;
	}

	else {
		PREGUNTA *aux = CabeceraArchivo;
		while (aux != NULL) {

			if (aux->codigo == codigo) {
				EstaCodigo = 1;
				return EstaCodigo;
			}
			aux = aux->siguiente;
		}
	return EstaCodigo;
	}
}
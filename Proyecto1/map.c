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

	// Se convierte el PID en string
	char pidStr[20];
	sprintf(pidStr, "%d",miPID);

	// Se concatena el PID con ".txt"
	// para obtener el nombre del archivo de salida:
	char nombreSalida[30];
	char inicio[40];
	strcpy(inicio,pidStr);
	strcpy(nombreSalida,inicio);
	strcat(nombreSalida, ".txt");


	// Se abre el archivo de salida:

	FILE *archivoSalida;

	archivoSalida = fopen(nombreSalida,"a");

	printf("el nombre del archivo de salida es %s\n",nombreSalida);

	int contador = 0;
	char Probando[] = "Ana -> Bernardo Cristina David";

	char *prueba = strtok(Probando, " ->");
	char *amigos;

	while (prueba != NULL) {
		printf("el valor es %s..,\n",prueba);
		amigos = strtok(NULL,"");
	}


	char Probando2[] = "Ana -> Bernardo Cristina David";

	char *prueba2 = strtok(Probando2, " ->");
	char *amigos2;

	while (prueba2 != NULL) {
		printf("el valor es %s..,\n",prueba);
		amigos2 = strtok(NULL,"");
	}

	char *p = strtok (amigos2," ");

  	while (p != NULL) {

  		if (contador == 0) {
  			;
  		}

 		else {

			fprintf(archivoSalida,"(%s %s) %s\n",prueba,p,amigos);

 		}
 		
    	p = strtok (NULL," ");
 		contador++;

	 }

	 fprintf(archivoSalida, "\n");

	// Se cierra el archivo de salida:

	fclose(archivoSalida);

	exit(0);


















	

}
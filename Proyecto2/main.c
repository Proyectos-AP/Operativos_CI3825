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
* Ultima modificacion: 07/06/2015
*
*/

// Directivas de Preprocesador:

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include "lib_cuentos.h"


//------------------------------------------------------------------------------------------------//
//                                    INICIO DEL CODIGO PRINCIPAL                                 //
//------------------------------------------------------------------------------------------------//

int main(int argc, char *argv[]) {
	/*
	*
	* Definicion de la funcion: 
	*
	* Parametros de entrada:
	*
	* Parametros de salida:
	*
	*/

	// Declaracion de variables:
	int status;
	int i;
	int numeroHijos;
	int numeroTextos;
	char *ruta;
	char *archivoSalida;

	if (argc == 6) {

		// Se considera la ruta dada para la lectura de los textos:
		printf("6 Argumentos\n");

		if ( strcmp(argv[1],"-d") == 0) {

			ruta = argv[2];
			numeroHijos = atoi(argv[3]);
			numeroTextos = atoi(argv[4]); 
			archivoSalida = argv[5];

			printf("La ruta dada es: %s\n",ruta);


		}

		else {

			printf("Error: No se han pasado los argumentos de forma correcta.\n");
			exit(1);

		}

	}

	else if (argc == 4) {
		// Se considera la ruta actual para la lectura de los textos:
		printf("4 Argumentos\n");

		ruta = "./";
		numeroHijos = atoi(argv[1]);
		numeroTextos = atoi(argv[2]); 
		archivoSalida = argv[3];

		printf("La ruta dada: %s\n",ruta);

	}

	else {
		// El pase de argumentos no se realizo de forma correcta:
		printf("Error: No se han pasado los argumentos de forma correcta.\n");
		exit(1);
	}

	// Se crean los procesos hijos que leeran los directorios:
	pid_t childpid[numeroHijos];
	int numerosAleatorios[numeroHijos]; 
	int agregarNumero;
	int numeroActual;
	int numeroArchivos;

	//int pipes[numeroHijos][2];

	//for (i = 0; i < numeroHijos; i++) { 

	//	pipe(pipes[i]);
	//	printf("Se hizo un pipe\n");

	//}

	time_t t;
	srand((unsigned) time(&t));

	generarNumerosAleatorios(numerosAleatorios,numeroHijos,10,ruta);

	for (i = 0; i < numeroHijos; i++) {

		printf("El numero aleatorio fuerda de la funcion es %d\n",numerosAleatorios[i]);		
	}

	// Se crean los proceso hijos para que lean los archivos de cada directorio:
	char numeroDirectorio[3];

	for (i = 0; i < numeroHijos;i++) {
		
		if ((childpid[i] = fork()) < 0) {
			perror("Error en el fork");
			exit(0);
		}

		if (childpid[i] == 0) {

			//int pipeIn = pipes[i][0];
			//close(pipeIn);
			//int pipeOut = pipes[i][1];

			// Se convierte el numero del Directorio en string:
			sprintf(numeroDirectorio, "%d",numerosAleatorios[i]);

			// Se concatena la ruta con el numero de directorio a verificar:
			// para obtener el nombre del archivo de salida:
			strcat(ruta,numeroDirectorio);

			printf("La ruta es: %s\n",ruta);

			numeroArchivos = contarDirectorios(ruta);
			
			printf("Soy el hijo %d y lei %d archivos\n",getpid(),numeroArchivos);

			int numerosAleatoriosArchivos[numeroArchivos];

			struct timeval t2;
			gettimeofday(&t2,NULL);
			srand(t2.tv_usec * t2.tv_sec);

			printf("El valor de la semilla es: %d \n",t2.tv_usec * t2.tv_sec);

			generarNumerosAleatorios(numerosAleatoriosArchivos,numeroTextos,numeroArchivos,ruta);

			for (i = 0; i < numeroTextos; i++) {

				printf("Soy el hijo %d  y el numero aleatorio del archivo es %d\n",getpid(),numerosAleatoriosArchivos[i]);		
			}	


			leerDirectorio(ruta,numerosAleatoriosArchivos,numeroTextos);

			//char frase = "Esto es un mensaje";

			//write(pipeOut,frase,strlen(frase) + 1);

			exit(0);
		}

	}


	// Se espera que los proceso hijos terminen:
	for(i = 0; i< numeroHijos;i++) {
		wait(&status);
		
	}


	//for (i = 0; i < numeroHijos; i++) {

	//	char message[100];
	//	int bytesleidos;
	//	close(pipes[i][1]);

	//	read(pipes[i][0],message,100);

	//	printf("Lo que se recibio es: \n",message);

	//}


	//for (i = 0; i < numeroHijos; i++) {

	//	printf("El numero aleatorio es %d\n",numerosAleatorios[i]);		

	//}
	
	// Se espera que los proceso hijos terminen:
	//for (i = 0; i < numeroHijos; i++) {

	//	wait(&status);

	//}

	return(0);

}

//------------------------------------------------------------------------------------------------//
//                                     FIN DEL CODIGO PRINCIPAL                                   //
//------------------------------------------------------------------------------------------------//

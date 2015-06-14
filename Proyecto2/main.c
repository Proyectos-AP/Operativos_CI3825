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
#include <fcntl.h>
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
	int numeroArchivos;

	//int pipes[numeroHijos][2];

	//for (i = 0; i < numeroHijos; i++) { 

	//	pipe(pipes[i]);
	//	printf("Se hizo un pipe\n");

	//}

	time_t t;
	srand((unsigned) time(&t));

	generarNumerosAleatoriosDirectorio(numerosAleatorios,numeroHijos,10,ruta);

	//for (i = 0; i < numeroHijos; i++) {

	//	printf("El numero aleatorio fuerda de la funcion es %d\n",numerosAleatorios[i]);		
	//}

	// Se crean los proceso hijos para que lean los archivos de cada directorio:
	char numeroDirectorio[3];

	// Se crea el pipe con el que los hijos enviaran informacion al padre:
	int pipePadre[2];
	pipe(pipePadre);

	for (i = 0; i < numeroHijos;i++) {
		
		if ((childpid[i] = fork()) < 0) {
			perror("Error en el fork");
			exit(0);
		}

		if (childpid[i] == 0) {

			//char numeroPID[20];
			//sprintf(numeroPID,"%d",getpid());
			//strcat(numeroPID,"\n");
			//write(pipePadre[1],numeroPID,strlen(numeroPID));

			//Se convierte el numero del Directorio en string:
			sprintf(numeroDirectorio, "%d",numerosAleatorios[i]);

			// Se concatena la ruta con el numero de directorio a verificar:
			// para obtener el nombre del archivo de salida:
			strcat(ruta,numeroDirectorio);

			//printf("La ruta es: %s\n",ruta);

			numeroArchivos = contarDirectorios(ruta);
			
			//printf("Soy el hijo %d y lei %d archivos\n",getpid(),numeroArchivos);

			int numerosAleatoriosArchivos[numeroTextos];

			struct timeval t2;
			gettimeofday(&t2,NULL);
			srand(t2.tv_usec * t2.tv_sec);

			//printf("El valor de la semilla es: %d \n",t2.tv_usec * t2.tv_sec);

			generarNumerosAleatoriosArchivo(numerosAleatoriosArchivos,numeroTextos,numeroArchivos,ruta);

			for (i = 1; i < numeroTextos+1; i++) {

					printf("Soy el hijo %d  y el numero aleatorio del archivo es %d\n",getpid(),numerosAleatoriosArchivos[i-1]);		
			}	

			//leerDirectorio(ruta,numerosAleatoriosArchivos,numeroTextos);

			//char frase = "Esto es un mensaje";

			//write(pipeOut,frase,strlen(frase) + 1);

			//exit(0);

			char* arregloRutas[numeroTextos + 2];

			printf("El restulado es %d \n",numeroTextos + 2);
			char numeroArchivo[20];

			arregloRutas[0] = (char*)calloc(1,sizeof(char)*50);	
			arregloRutas[0] = "cat";
			
			for (i = 1; i < numeroTextos + 1; ++i) {

				//printf("Ciclo \n");
				arregloRutas[i] = (char*)calloc(1,sizeof(char)*50);
				//arregloRutas[i] = "";
				sprintf(numeroArchivo,"%d",numerosAleatoriosArchivos[i-1]);
				printf("El valor de la i es : %d\n", i );
				printf("El numero numero es %d y el string es %s\n",numerosAleatoriosArchivos[i-1],numeroArchivo);
				strcat(arregloRutas[i],ruta);
				printf("La ruta 0 es: %s \n",arregloRutas[i]);
				strcat(arregloRutas[i],"/");
				printf("La ruta 1 es: %s \n",arregloRutas[i]);
				strcat(arregloRutas[i],numeroArchivo);
				printf("La ruta 2 es: %s \n",arregloRutas[i]);

			}

			arregloRutas[numeroTextos + 1] = NULL;

			for (i = 1; i < numeroTextos+1; i++) {

					printf("[DESPUES] Soy el hijo %d  y el numero aleatorio del archivo es %d\n",getpid(),numerosAleatoriosArchivos[i-1]);		
			}


			for (i = 0; i < numeroTextos +2; ++i) {

				printf("El elemento actual es %s \n",arregloRutas[i]);
				/* code */
			}

			int statusHijo;
			pid_t childpid;

			close(pipePadre[0]);
			dup2(pipePadre[1],1);
			close(pipePadre[1]);

			if ((childpid = fork()) < 0) {
				perror("Error en el fork");
				exit(0);

			}


			if (childpid == 0) {

				if ( (execvp("cat",arregloRutas)) < 0) {

					printf("Error en el EXEC\n");
					exit(0);
				}

			}

			else {

				wait(&statusHijo);

				for (i = 0; i < numeroTextos +2; ++i) {

					free(arregloRutas[i]);
					/* code */
				
				}

				exit(0); // aqui hay que agregar lo de los archivos leidos

			}

		}

	}


	// Se espera que los proceso hijos terminen:
	for(i = 0; i< numeroHijos;i++) {
		wait(&status);

		printf("El valor que recibi es: %d \n",status>>8);
		
	}

	char rutaSalida[50];
	strcat(rutaSalida,"./");
	strcat(rutaSalida,archivoSalida);
	int descriptorSalida;
	descriptorSalida = open(rutaSalida,O_WRONLY | O_CREAT,0600);
	dup2(descriptorSalida,1);
	close(descriptorSalida);

	char message[300];
	int countbytes;
	close(pipePadre[1]);


	while ((countbytes = read(pipePadre[0],message,300)) > 0) {
    	
    	write(1,message,countbytes);
  
  	}


	//countbytes = read(pipePadre[0],message,300);

	//printf("El mensaje leido es %s",message);

	//write(1,message,strlen(message));

	//close(descriptorSalida);
	close(pipePadre[0]);



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

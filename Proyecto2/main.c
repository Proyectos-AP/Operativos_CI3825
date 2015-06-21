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

	int i;
	int status;
	int esArchivo;
	//int esDirectorio;
	int numeroHijos;
	int countbytes;
	int pipePadre[2]; // Se crea el pipe con el que los hijos enviaran informacion al padre
	int numeroTextos;
	int numeroArchivos;
	int descriptorSalida;
	int archivosLeidos = 1;
	char *aux;
	char *ruta;
	char message[300];
	char *archivoSalida;
	char numeroArchivo[20];
	char numeroDirectorio[3];
	pipe(pipePadre);
	char *errorConversion1;
	char *errorConversion2;

	time_t t;

	if (argc == 6) {

		// Se considera la ruta dada para la lectura de los textos:
		//printf("6 Argumentos\n");

		if ( strcmp(argv[1],"-d") == 0) {

			ruta = argv[2];

			if (verificarDirectorio(ruta) == 0) {
				printf("Error: %s no corresponde a un directorio.\n",ruta);
				printf("El programa finalizara su ejecucion\n");
				exit(0);

			}

			numeroHijos = strtol(argv[3],&errorConversion1,10);

			if (*errorConversion1 != 0) {

				printf("Error: El numero de hijos no fue pasado de forma correcta.\n");
				printf("El programa finalizara su ejecucion\n");
				exit(1);

			}

			if (numeroHijos < 0 || numeroHijos > 10) {

				printf("Error: El numero de hijos a generar pasado como argumento fue %d.\n",numeroHijos);
				printf("Ejecute de nuevo con un valor que se encuentre en el intervalo [1,10].\n");
				exit(1);

			}

			if ( numeroHijos == 0 ) {

				printf("Error: El numero de hijos a generar pasado como argumento fue %d.\n",numeroHijos);
				printf("No se generaran historias.\n");
				exit(0);
			}

			numeroTextos = strtol(argv[4],&errorConversion2,10); 

			if (*errorConversion2 != 0) {

				printf("Error: El numero de textos a leer no fue pasado de forma correcta.\n");
				printf("El programa finalizara su ejecucion\n");
				exit(1);

			} 


			if (numeroTextos < 0 || numeroTextos >= 20) {

				printf("Error: El numero de textos a leer pasado como argumento fue %d.\n",numeroTextos);
				printf("Ejecute de nuevo con un valor que se encuentre en el intervalo [1,20].\n");
				exit(1);

			}

			if ( numeroTextos == 0) {

				printf("Error: El numero de textos a generar pasado como argumento fue %d.\n",numeroTextos);
				printf("No se generaran historias.\n");
				exit(0);
			}

			archivoSalida = argv[5];

			printf("La ruta dada es: %s\n",ruta);


		}

		else {

			printf("Error: No se han pasado los argumentos de forma correcta.\n");
			printf("El programa finalizara su ejecucion\n");
			exit(1);

		}

	}

	else if (argc == 4) {
		// Se considera la ruta actual para la lectura de los textos:
		printf("4 Argumentos\n");

		ruta = "./";

		numeroHijos = strtol(argv[1],&errorConversion1,10);

		if (*errorConversion1 != 0) {

			printf("Error: El numero de hijos no fue pasado de forma correcta.\n");
			printf("El programa finalizara su ejecucion\n");
			exit(1);

		}

		if (numeroHijos < 0 || numeroHijos > 10) {

				printf("Error: El numero de hijos a generar pasado como argumento fue %d.\n",numeroHijos);
				printf("Ejecute de nuevo con un valor que se encuentre en el intervalo [1,10].\n");
				exit(1);

			}


		if ( numeroHijos == 0 ) {

				printf("Error: El numero de hijos a generar pasado como argumento fue %d.\n",numeroHijos);
				printf("No se generaran historias.\n");
				exit(0);
		}


		numeroTextos = strtol(argv[2],&errorConversion2,10); 

		if (*errorConversion2 != 0) {

			printf("Error: El numero de textos a leer no fue pasado de forma correcta.\n");
			printf("El programa finalizara su ejecucion\n");
			exit(1);

		} 

		if (numeroTextos < 0 || numeroTextos >= 20) {

				printf("Error: El numero de textos a leer pasado como argumento fue %d.\n",numeroTextos);
				printf("Ejecute de nuevo con un valor que se encuentre en el intervalo [1,20].\n");
				exit(1);

			}


		if ( numeroTextos == 0) {

				printf("Error: El numero de textos a generar pasado como argumento fue %d.\n",numeroTextos);
				printf("No se generaran historias.\n");
				exit(0);
		}


		archivoSalida = argv[3];

		printf("La ruta dada: %s\n",ruta);

	}

	else {
		// El pase de argumentos no se realizo de forma correcta:
		printf("Error: No se han pasado los argumentos de forma correcta.\n");
		printf("El programa finalizara su ejecucion\n");
		exit(1);
	}

	// Declaracion de variables
	pid_t childpid[numeroHijos];
	int numerosAleatorios[numeroHijos]; 

	srand((unsigned) time(&t));
	generarNumerosAleatoriosDirectorio(numerosAleatorios,numeroHijos,10,ruta);

	// Se crean los procesos hijos que leeran los directorios:
	for (i = 0; i < numeroHijos;i++) {
		
		if ((childpid[i] = fork()) < 0) {
			perror("Error en el fork");
			exit(0);
		}

		if (childpid[i] == 0) {

			//Se convierte el numero del Directorio en string:
			sprintf(numeroDirectorio, "%d",numerosAleatorios[i]);

			// Se concatena la ruta con el numero de directorio a verificar:
			// para obtener el nombre del archivo de salida:
			strcat(ruta,numeroDirectorio);

			// Se verifica que el valor asignado corresponda con un directorio:

			if (verificarDirectorio(ruta))  {

				// Declaracion de variables
				int statusHijo;
				pid_t childpid;

				// Se cuenta el numero de elementos que existen dentro del 
				// directorio
				numeroArchivos = contarDirectorios(ruta);

				int numerosAleatoriosArchivos[numeroTextos];
				struct timeval t2;
				gettimeofday(&t2,NULL);
				srand(t2.tv_usec * t2.tv_sec);

				// Se generan numeros aleatorios correspondientes a los archivos regulares
				int numeroArchivosLeidos = generarNumerosAleatoriosArchivo(numerosAleatoriosArchivos,\
																	numeroTextos,numeroArchivos,ruta);

				// Caso en el que existen archivos para leer
				if (numeroArchivosLeidos!=0){
					char* arregloRutas[numeroArchivosLeidos + 2];
				

					arregloRutas[0] = (char*)calloc(1,sizeof(char)*50);	
					strcat(arregloRutas[0],"cat");


					// Se almacenan las rutas de los archivos que se van leer en un arreglo
					for (i = 1; i < numeroTextos + 1; ++i) {

						// Se construye la ruta del archivo que se va a leer.
						aux = (char*)calloc(1,sizeof(char)*100);
						sprintf(numeroArchivo,"%d",numerosAleatoriosArchivos[i-1]);
						strcat(aux,ruta);
						strcat(aux,"/");
						strcat(aux,numeroArchivo);

						esArchivo = verificarArchivo(aux);

						if (esArchivo==1){

							arregloRutas[archivosLeidos] = aux;
							archivosLeidos++;
								
						}
						else {

							free(aux);
						}

						//if (esArchivo!=-1){
						//	archivosLeidos++;
						//}
							
							
					}

					arregloRutas[numeroArchivosLeidos + 1] = NULL;

					
					close(pipePadre[0]);
					dup2(pipePadre[1],1);
					close(pipePadre[1]);

					if ((childpid = fork()) < 0) {
						perror("Error en el fork");
						exit(0);

					}

					if (childpid == 0) {

						// Se concatenan todos los archivos
						if ( (execvp("cat",arregloRutas)) < 0) {

							printf("Error en el EXEC\n");
							exit(0);
						}

					}

					// Se espera por el hijo
					wait(&statusHijo);

					// Se limpia memoria
					for (i = 0; i < numeroArchivosLeidos + 1; ++i) {
						free(arregloRutas[i]);
					}


				}

				// Se envia al padre el numero de archivos leidos.
				exit(numeroArchivosLeidos); 


			}

			else {

				printf("-----------------------------\n" 
				"PID Hijo: %d\nEl directorio asignado no existe\n"
				"No tengo trabajo por realizar.\n"
				"-----------------------------\n",getpid());
				exit(0);
			}

		}

	}

	int archivosRecibidos;
	archivosRecibidos = 0;
	// Se espera que los proceso hijos terminen:
	for(i = 0; i< numeroHijos;i++) {
		wait(&status);
		archivosRecibidos = archivosRecibidos + (status>>8);
		printf("El valor que recibi es: %d \n",status>>8);
		
	}

	printf("El numero de archivos leidos es %d\n",archivosRecibidos);

	if (archivosRecibidos > 0) {



		// Se arma la ruta del archivo de salida del programa
		char *rutaSalida = (char*)calloc(1,sizeof(char)*50);	
		strcat(rutaSalida,"./");
		strcat(rutaSalida,archivoSalida);

		// Se crea el archivo de salida
		descriptorSalida = open(rutaSalida,O_WRONLY | O_CREAT,0755);
		//dup2(descriptorSalida,1);
		//close(descriptorSalida);

		close(pipePadre[1]);

		// Se lee todo el contenido del pipe y el mismo es escrito
		// en el standard output del padre (el archivo de salida).
		while ((countbytes = read(pipePadre[0],message,300)) > 0) {

	    	write(descriptorSalida,message,countbytes);
	  
	  	}

		close(descriptorSalida);
		descriptorSalida = open(rutaSalida,O_RDONLY);

		printf("-----------------------------\n\n");
		printf("La Historia aleatoria generada es: \n\n");

		// Se lee la historia del archivo generado y se imprime por
		// pantalla: 
		while ((countbytes = read(descriptorSalida,message,300)) > 0) {

	    	write(1,message,countbytes);
	  
	  	}

	  	printf("\n-----------------------------\n");
		close(pipePadre[0]);
		free(rutaSalida);
		return(0);

	}

	else {

		printf("Aviso: Ningun hijo leyo archivos.\n");
		printf("No se generaron historias\n");
		exit(0);

	}


}

//------------------------------------------------------------------------------------------------//
//                                     FIN DEL CODIGO PRINCIPAL                                   //
//------------------------------------------------------------------------------------------------//

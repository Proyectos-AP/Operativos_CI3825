/*
*
* Archivo: main.c
*
* Nombres:
*	Alejandra Cordero / Carnet: 12-10645
*	Pablo Maldonado   / Carnet: 12-10561
*
* Descripcion: Programa que genera cuentos aleatorios a partir de archivos regulares que contienen
* textos, los cuales se encuentran en un directorio indicado por el usuario. La lectura de los 
* textos se hace de forma concurrente con el uso de procesos hijos y comunicacion entre procesos
* por medio de pipes 
* 
*
* Ultima modificacion: 22/06/2015
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
	*	El programa genera cuentos aleatorios a partir de archivos regulares que contienen
	*   textos, los cuales se ubican en un directorio especificado al momento de la ejecucion.
	*
	* Parametros de entrada:
	*			argc : Numero de parametros pasados al momento de la ejecucion. 
	*
	*			argv : Argumentos pasados a la funcion principal. En este caso pueden ser
	*			los siguientes:
	*                -ruta,
    *                   Ruta en donde se encuentra el directorio desde el cual se encuentran
    *                   las carpetas que serán asignadas a los hijos al momento de la ejecución.
    *                   Este argumento deberá finalizar en '/'. Ejemplo: historias/
	*
    *           	-numeroHijos,
    *                   Número de hijos que se van a generar para la lectura de los directorios.
	*
	*               -numeroTextos,
    *                   Número de textos que leerá, si es posible, 
    *                   cada hijo de su directorio asignado.
	*
  	*               -nombreSalida,
 	*	                Nombre del archivo en el cuál se escribirá el cuento aleatorio generado.
	*
	*			    Ejemplo:  ./miCuento -d [ruta] [numeroHijos] [numeroTextos] [nombreSalida]
	*
	* Parametros de salida:
	*	- Entero que indica el tipo de culminacion del programa
	*			0 : exitosa
	*			1 : ocurrio algun tipo de error que se indica en pantalla.
	*
	*/

	// Declaracion de variables:

	int i;						// Variable de iteracion.
	int status;					// Se utiliza para que el padre reciba el numero de archivos leidos.
	int esArchivo;				// Indica si un nombre dado corresponde a un archivo regular.
	int numeroHijos;			// Numero de hijos que deben crearse para leer los directorios.
	int countbytes;				// Se utiliza para la lectura de la informacion del pipe
	int numeroTextos;			// Numero de textos que deberan leer los hijos (si es posible)
	int numeroArchivos;			// Indica el numero de archivos de un directorio
	int descriptorSalida;		// Descriptor del archivo de salida
	int archivosLeidos = 1;		// Indice del arreglo de numeros aleatorios de archivos
	char *aux;					// Variable auxiliar para constrir la ruta de los archivos a leer.
	char *ruta;					// Ruta pasada como argumento donde se encuentran las carpetas.
	char message[300];			// Se utiliza para la lectura de informacion del pipe.
	char *archivoSalida;		// Nombre del archivo de salida en el que se escribira el cuento
	char numeroArchivo[20];		// Variable utilizada para convertir el numero de archivo a str
	char numeroDirectorio[3];	// Variable utilizada para concatenar la ruta
	char *errorConversion1;		// Indica si la conversion del argumento numeroHijos fue exitosa
	char *errorConversion2;		// Indica si la conversion del argumento numeroTextos fue exitosa.
	time_t t;					// Variable utilizada para la generacion de numeros aleatorios.
	int pipePadre[2]; 			// Pipe que permitira la comunicacion entre el padre y sus hijos.
	pipe(pipePadre);			// Se generan los descriptores del pipe.
	int archivosRecibidos;		// Indica el numero de archivos que recibio el padre de sus hijos.

	if (argc == 6) {

		//printf("6 Argumentos\n");  (NO DEBE ESTAR EN LA ENTREGA FINAL)

		// Se considera la ruta dada para la lectura de los textos:
		
		if ( strcmp(argv[1],"-d") == 0) {

			ruta = argv[2];

			// Se verifica que la ruta corresponda efectivamente con un directorio:
			if (verificarDirectorio(ruta) == 0) {
				printf("Error: %s no corresponde a un directorio.\n",ruta);
				printf("El programa finalizara su ejecucion\n");
				exit(0);

			}

			// Se convierte el numero de hijos dado como argumento a entero:
			numeroHijos = strtol(argv[3],&errorConversion1,10);

			// Se verifica que el argumento pasado sea efectivamente un entero:
			if (*errorConversion1 != 0) {

				printf("Error: El numero de hijos no fue pasado de forma correcta.\n");
				printf("El programa finalizara su ejecucion\n");
				exit(1);

			}

			// Se verifica que el entero pasado se encuentre dentro del rango establecido:
			if (numeroHijos < 0 || numeroHijos > 10) {

				printf("Error: El numero de hijos a generar pasado como argumento fue %d.\n",numeroHijos);
				printf("Ejecute de nuevo con un valor que se encuentre en el intervalo [1,10].\n");
				exit(1);

			}

			// Se verifica el caso en el que no haya que generar procesos hijos:
			if ( numeroHijos == 0 ) {

				printf("El numero de hijos a generar pasado como argumento fue %d.\n",numeroHijos);
				printf("No se generaran historias.\n");
				exit(0);
			}

			// Se convierte el numero de textos dado como argumento a entero:
			numeroTextos = strtol(argv[4],&errorConversion2,10); 

			// Se verifica que el argumento pasado sea efectivamente un entero:
			if (*errorConversion2 != 0) {

				printf("Error: El numero de textos a leer no fue pasado de forma correcta.\n");
				printf("El programa finalizara su ejecucion\n");
				exit(1);

			} 

			// Se verifica que el entero pasado se encuentre dentro del rango establecido:
			if (numeroTextos < 0 || numeroTextos > 20) {

				printf("Error: El numero de textos a leer pasado como argumento fue %d.\n",numeroTextos);
				printf("Ejecute de nuevo con un valor que se encuentre en el intervalo [1,20].\n");
				exit(1);

			}

			// Se verifica el caso en el que los hijos no van a leer ningun archivo:
			if ( numeroTextos == 0) {

				printf("El numero de textos a generar pasado como argumento fue %d.\n",numeroTextos);
				printf("No se generaran historias.\n");
				exit(0);
			}

			archivoSalida = argv[5];

			// printf("La ruta dada es: %s\n",ruta); (ESTO NO DEBE ESTAR EN LA ENTREGA FINAL)


		}

		else {

			// Los parametros no fueron pasados de forma correcta:
			printf("Error: No se han pasado los argumentos de forma correcta.\n");
			printf("El programa finalizara su ejecucion\n");
			exit(1);

		}

	}

	else if (argc == 4) {

		// printf("4 Argumentos\n"); (NO DEBE ESTAR EN LA ENTREGA FINAL)

		// Se considera la ruta actual para la lectura de los textos:
		ruta = "./";

		// Se convierte el numero de hijos dado como argumento a entero:
		numeroHijos = strtol(argv[1],&errorConversion1,10);

		// Se verifica que el argumento pasado sea efectivamente un entero:
		if (*errorConversion1 != 0) {

			printf("Error: El numero de hijos no fue pasado de forma correcta.\n");
			printf("El programa finalizara su ejecucion\n");
			exit(1);

		}

		// Se verifica que el entero pasado se encuentre dentro del rango establecido:
		if (numeroHijos < 0 || numeroHijos > 10) {

				printf("Error: El numero de hijos a generar pasado como argumento fue %d.\n",numeroHijos);
				printf("Ejecute de nuevo con un valor que se encuentre en el intervalo [1,10].\n");
				exit(1);

			}


		// Se verifica el caso en el que no haya que generar procesos hijos:
		if ( numeroHijos == 0 ) {

				printf("El numero de hijos a generar pasado como argumento fue %d.\n",numeroHijos);
				printf("No se generaran historias.\n");
				exit(0);
		}


		numeroTextos = strtol(argv[2],&errorConversion2,10); 

		// Se verifica que el argumento pasado sea efectivamente un entero:
		if (*errorConversion2 != 0) {

			printf("Error: El numero de textos a leer no fue pasado de forma correcta.\n");
			printf("El programa finalizara su ejecucion\n");
			exit(1);

		} 

		// Se verifica que el entero pasado se encuentre dentro del rango establecido:
		if (numeroTextos < 0 || numeroTextos > 20) {

				printf("Error: El numero de textos a leer pasado como argumento fue %d.\n",numeroTextos);
				printf("Ejecute de nuevo con un valor que se encuentre en el intervalo [1,20].\n");
				exit(1);

			}

		// Se verifica el caso en el que los hijos no van a leer ningun archivo:
		if ( numeroTextos == 0) {

				printf("Error: El numero de textos a generar pasado como argumento fue %d.\n",numeroTextos);
				printf("No se generaran historias.\n");
				exit(0);
		}

		archivoSalida = argv[3];

		// printf("La ruta dada: %s\n",ruta); (ESTO NO DEBEE ESTAR EN LA ENTREGA FINAL)

	}

	else {

		// El pase de argumentos no se realizo de forma correcta:
		printf("Error: No se han pasado los argumentos de forma correcta.\n");
		printf("El programa finalizara su ejecucion\n");
		exit(1);
	}

	// Declaracion de variables:

	pid_t childpid[numeroHijos];			// Se crea un arreglo que contiene el PID de los hijos
	int numerosAleatorios[numeroHijos]; 	// Se crea el arreglo de numeros aleatorios de los 
											// directorios

	// Se cambia la semilla de la funcion rand():
	srand((unsigned) time(&t));

	// Se generan los numeros aleatorios que seran asignados a los hijos para leer los directorios:
	generarNumerosAleatoriosDirectorio(numerosAleatorios,numeroHijos,10,ruta);

	// Se crean los procesos hijos que leeran los directorios:
	for (i = 0; i < numeroHijos;i++) {
		
		// Se verifica si ocurrio un error en la generacion de los hijos:
		if ((childpid[i] = fork()) < 0) {
			perror("Error en el fork");
			exit(1);
		}

		// Codigo que ejecutaran los hijos:
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

				int numerosAleatoriosArchivos[numeroTextos]; 	// Arreglo de numeros aleatorios
																// para la lectura de los archivos

				struct timeval t2;								// Se utiliza para los numeros
																// aleatorios

				// Se cambia la semilla del numero aleatorio:
				gettimeofday(&t2,NULL);							
				srand(t2.tv_usec * t2.tv_sec);

				// Se generan numeros aleatorios correspondientes a los archivos regulares
				int numeroArchivosLeidos = generarNumerosAleatoriosArchivo(numerosAleatoriosArchivos,\
																	numeroTextos,numeroArchivos,ruta);

				// Caso en el que existen archivos para leer:
				if ( numeroArchivosLeidos != 0 ){
					char* arregloRutas[numeroArchivosLeidos + 2];
				
					// Se inicializa al arreglo de ruta con el cual se ejecutara el cat
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

						// Se verifica que la ruta a agregar corresponda a un archivo regular:
						esArchivo = verificarArchivo(aux);

						if (esArchivo==1){

							arregloRutas[archivosLeidos] = aux;
							archivosLeidos++;
								
						}
						else {

							free(aux);
						}

						//if (esArchivo!=-1){   // ESTO NO DEBE ESTAR EN LA ENTREGA FINALL
						//	archivosLeidos++;
						//}
							
							
					}

					// Se agrega la terminacion de los argumentos para el execvp:
					arregloRutas[numeroArchivosLeidos + 1] = NULL;

					// Se cambia la salida estandar del hijo por el descriptor de escritura
					// del pipe del padre. Se cierra el descriptor de lectura del pipe del padre:
					close(pipePadre[0]);
					dup2(pipePadre[1],1);
					close(pipePadre[1]);

					// Se crea otro hijo que hara exec de cat y pasara la informacion al padre 
					// principal:
					if ((childpid = fork()) < 0) {
						perror("Error en el fork");
						exit(0);

					}

					// Codigo del segundo hijo:
					if (childpid == 0) {

						// Se concatenan los archivos y el resultado es enviado por el pipe al
						// padre principal:
						if ( (execvp("cat",arregloRutas)) < 0) {

							printf("Error en el EXEC\n");
							exit(0);
						}

					}

					// Se espera por el hijo:
					wait(&statusHijo);

					// Se limpia memoria:
					for (i = 0; i < numeroArchivosLeidos + 1; ++i) {
						free(arregloRutas[i]);
					}


				}

				// Se envia al padre el numero de archivos leidos:
				exit(numeroArchivosLeidos); 


			}

			else {

				// En caso de que el hijo no tenga trabajo asignado lo indica por un mensaje
				// en pantalla:
				printf("-----------------------------\n" 
				"PID Hijo: %d\nEl directorio asignado no existe\n"
				"No tengo trabajo por realizar.\n"
				"-----------------------------\n",getpid());
				exit(0);
			}

		}

	}

	// Se inicializa el numero de archivos recibidos por el padre:
	archivosRecibidos = 0;

	// Se espera que los proceso hijos terminen:
	for(i = 0; i< numeroHijos;i++) {

		wait(&status);
		archivosRecibidos = archivosRecibidos + (status>>8);
		// printf("El valor que recibi es: %d \n",status>>8); (NO DEBE ESTAR EN LA ENTREGA FINAL)
		
	}

	// printf("El numero de archivos leidos es %d\n",archivosRecibidos); (NO DEBE ESTAR EN LA ENTREGA FINAL)

	if (archivosRecibidos > 0) {

		// Se arma la ruta del archivo de salida del programa:
		char *rutaSalida = (char*)calloc(1,sizeof(char)*50);	
		strcat(rutaSalida,"./");
		strcat(rutaSalida,archivoSalida);

		// Se abre o se crea el archivo de salida:
		descriptorSalida = open(rutaSalida,O_WRONLY | O_CREAT,0666);
		//dup2(descriptorSalida,1); (ESTO NO DEBE ESTAR EN LA ENTREGA FINAL)
		//close(descriptorSalida);

		// Se cierra el descriptor de escritura del pipe del padre:
		close(pipePadre[1]);

		// Se lee todo el contenido del pipe y el mismo es escrito
		// en el archivo de salida:
		while ((countbytes = read(pipePadre[0],message,300)) > 0) {

	    	write(descriptorSalida,message,countbytes);
	  
	  	}

	  	// Se cierra el descriptor de lectura del pipe del padre:
		close(pipePadre[0]);
	
	  	// Se cierra el descriptor de salida de solo escritura y se 
	  	// abre como solo lectura para imprimir por pantalla el cuento generado:
		close(descriptorSalida);
		descriptorSalida = open(rutaSalida,O_RDONLY);

		// Se libera la memoria reservada para la ruta del archivo de salida:
		free(rutaSalida);

		// Se lee la historia del archivo generado y se imprime por
		// pantalla: 
		printf("-----------------------------\n\n");
		printf("La Historia aleatoria generada es: \n\n");

		while ((countbytes = read(descriptorSalida,message,300)) > 0) {

	    	write(1,message,countbytes);
	  
	  	}

	  	printf("\n-----------------------------\n");

		// Finaliza la ejecucion del programa:
		return(0);

	}

	else {
		// En caso de que no se hayan leido ningun archivo y por tanto no se 
		// generaron historias:
		printf("Aviso: Ningun hijo leyo archivos.\n");
		printf("No se generaron historias\n");
		exit(0);

	}

}

//------------------------------------------------------------------------------------------------//
//                                     FIN DEL CODIGO PRINCIPAL                                   //
//------------------------------------------------------------------------------------------------//

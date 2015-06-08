/*
*
* Archivo: lib_cuentos.c
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

// Directivas de preprocesador:

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//------------------------------------------------------------------------------------------------//
//                                     DEFINICION DE ESTRUCTURAS                                  //
//------------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------------//
//                                    DEFINICION DE FUNCIONES                                     //
//------------------------------------------------------------------------------------------------//

void generarNumerosAleatorios(int *arregloNumeros,int numeroElementos,int numeroMaximo) {
	/*
	*
	* Definicion de la funcion: La funcion 
	*
	* Parametros de entrada:
	*	- arregloNumeros:
	*	- numeroElementos:
	*	- numeroMaximo:
	* Parametros de salida: Ninguno
	*
	*/

	int agregarNumero;
	int numeroActual;
	int i = 0;
	int j = 0;

	while (i < numeroElementos) {
	
		agregarNumero = 1;
		numeroActual = (rand() % numeroMaximo) + 1;

		for (j = 0; j < i; j++) {

			if (numeroActual == arregloNumeros[j]) {
				agregarNumero = 0;
				break;
			}

		}

		if (agregarNumero == 1) {

			arregloNumeros[i] = numeroActual;
			i++;

		}
	

	}


	//for (i = 0; i < numeroElementos; i++) {

	//	printf("El numero aleatorio en la funcion es %d\n",arregloNumeros[i]);		

	//}



}


//------------------------------------------------------------------------------------------------//

int contarDirectorios(char *rutaDirectorio) {
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
	int contadorArchivos = 0;
	DIR *dirp;
	struct dirent *direntp;
	
	if ((dirp = opendir(rutaDirectorio)) == NULL) {
		fprintf(stderr,"No se puede abrir el directorio %s\n",rutaDirectorio);
		strerror(errno);
		exit(1);
	}

	while ((direntp = readdir(dirp)) != NULL ) {

		contadorArchivos++;
	}

	contadorArchivos = contadorArchivos - 2;
	closedir(dirp);

	return(contadorArchivos);

}

//------------------------------------------------------------------------------------------------//


void leerDirectorio(char *rutaDirectorio,int *arregloAleatorios,int numeroElementos) {
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
	char numeroArchivo[3];
	DIR *dirp;
	struct dirent *direntp;
	char *Linea;
	FILE *archivo;


	
	if ((dirp = opendir(rutaDirectorio)) == NULL) {
		fprintf(stderr,"No se puede abrir el directorio %s\n",rutaDirectorio);
		strerror(errno);
		exit(1);
	}

	else {

		for (i = 1; i < numeroElementos; i++) {

			//Se convierte el numero del Directorio en string:
			sprintf(numeroArchivo, "%d",arregloAleatorios[i]);


			if ( (archivo=fopen(numeroArchivo,"r")) == NULL) {

				perror("Error: El archivo indicado no fue encontrado ");
				printf("errno = %d. \n",errno);
				exit(1);
			}
	
			else {
		
				//archivo = fopen(nombre_archivo,"r");
				fseek(archivo,0,SEEK_END);

				 if (ftell(archivo) == 0 )	{		
   					//El archivo esta vacio.
   					printf("---------------------------------.\n");
   					printf("El archivo de entrada esta vacio.\n");
   					printf("---------------------------------.\n");
   					exit(0);
    			}

    			else {

					fseek(archivo,0,SEEK_SET);
			
					// Se empieza a leer el archivo:
					while(1){

						// Se reserva el espacio de memoria para la nueva linea
						Linea = (char*)malloc(sizeof(char)*101);
						fscanf(archivo, " %[^\n]\n" ,Linea);
				
						// Se escribe en alguno de los archivos creados.


						printf("Soy el hijo %d y lei la linea: %s \n",getpid(),Linea);



						free(Linea);
						// Se verifica si se ha llegado al fin del archivo
						if (feof(archivo)) {
							fclose(archivo);
							break;
						
						}
					}


				}
			
			}

			printf("La Ruta del Archivo es: %s\n", direntp->d_name);

		} 

		
	}

	closedir(dirp);

	exit(0);

}




//------------------------------------------------------------------------------------------------//
//                                FIN DE LA DEFINICION DE FUNCIONES                               //
//------------------------------------------------------------------------------------------------//

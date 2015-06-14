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
#include <sys/stat.h>


//------------------------------------------------------------------------------------------------//
//                                     DEFINICION DE ESTRUCTURAS                                  //
//------------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------------//
//                                    DEFINICION DE FUNCIONES                                     //
//------------------------------------------------------------------------------------------------//



int verificarArchivo(char *rutaArchivo){
	/*
	*
	* Definicion de la funcion: 
	*
	* Parametros de entrada:
	*
	* Parametros de salida:
	*
	*/

	struct stat st_info;
	
	if (stat(rutaArchivo, &st_info) < 0) {
		return 0;
	}
 
	return S_ISREG(st_info.st_mode);

}









//------------------------------------------------------------------------------------------------//


int verificarDirectorio(char *nombreDirectorio) {
	/*
	*
	* Definicion de la funcion: 
	*
	* Parametros de entrada:
	*
	* Parametros de salida:
	*
	*/

	struct stat statbuf;

	printf("El nombreDirectorio es: %s\n",nombreDirectorio);

   	if (stat(nombreDirectorio,&statbuf) ==-1) {
        fprintf(stderr,"No se puede obtener el stat del archivo %s:%s\n",nombreDirectorio, strerror(errno));
        exit(1);
    }

   	if (statbuf.st_mode & S_IFDIR) {
        // Es un directorio:
        return(1);
   	} 

  	else {

        // No es un directorio
        return(0);
  	}

}


//------------------------------------------------------------------------------------------------//





void generarNumerosAleatoriosDirectorio(int *arregloNumeros,int numeroElementos,int numeroMaximo, char *rutaDirectorio) {
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

	char numeroDirectorio[20];
	
	int agregarNumero;
	int numeroActual;
	int esDirectorio;
	int i = 0;
	int j = 0;

	while (i < numeroElementos) {

		char *rutaAux;
		rutaAux = (char*)calloc(1,sizeof(char)*100);
		agregarNumero = 1;
		numeroActual = (rand() % numeroMaximo) + 1;

	
		// Se convierte el numero del directorio en string:
		sprintf(numeroDirectorio, "%d",numeroActual);
		strcat(rutaAux,rutaDirectorio);
		strcat(rutaAux,numeroDirectorio);
		esDirectorio = verificarDirectorio(rutaAux);
		printf("esDirectorio : %d  y la ruta es %s \n",esDirectorio,rutaAux);
		if (esDirectorio == 1) {

			for (j = 0; j < i; j++) {

				if (numeroActual == arregloNumeros[j]) {
					agregarNumero = 0;
					break;
				}

			}

			if (agregarNumero == 1){

				arregloNumeros[i] = numeroActual;
				i++;

			}
			
		}
		
		free(rutaAux);
	}

}

//------------------------------------------------------------------------------------------------//

int generarNumerosAleatoriosArchivo(int *arregloNumeros,int numeroElementos,int numeroMaximo, char *rutaArchivo) {
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

	char numeroArchivo[20];
	
	int agregarNumero;
	int numeroActual;
	int esArchivo;
	int i = 0;
	int j = 0;

	while (i < numeroElementos) {

		char *rutaAux2;
		rutaAux2 = (char*)calloc(1,sizeof(char)*50);
		agregarNumero = 1;
		numeroActual = (rand() % numeroMaximo) + 1;

		// Se convierte el numero del directorio en string:
		printf("LA RUTA ES %s \n",rutaAux2);
		sprintf(numeroArchivo, "%d",numeroActual);
		strcat(rutaAux2,rutaArchivo);
		strcat(rutaAux2,"/");
		strcat(rutaAux2,numeroArchivo);

		// Se verifica si la ruta pasada por argumento corresponde a la de un
		// archivo regular.
		esArchivo = verificarArchivo(rutaAux2);
		printf("esArchivo : %d  y la ruta es %s \n",esArchivo ,rutaAux2);
		//printf("esDirectorio : %d  y la ruta es %s \n",esDirectorio,rutaDirectorio);
		if (esArchivo == 1) {

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
		free(rutaAux2);
	
	}

	int k;

	for (k = 0; k < numeroElementos; ++k) {
		
		printf("El elemento del arreglo es %d\n",arregloNumeros[k]);

	}

	return(i);


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

	/*
	int i;
	char numeroArchivo[3];
	DIR *dirp;
	struct dirent *direntp;
	char *Linea;
	FILE *archivo;

	printf("La rura inicial es: %s\n",rutaDirectorio);

	
	if ((dirp = opendir(rutaDirectorio)) == NULL) {
		fprintf(stderr,"No se puede abrir el directorio %s\n",rutaDirectorio);
		strerror(errno);
		exit(1);
	}

	else {
		
		char *Directorio;
		Directorio = (char*)calloc(1,sizeof(char)*50);
		for (i = 1; i < numeroElementos; i++) {
			//Se convierte el numero del Directorio en string:
			//printf("El archivo0 es: %s\n",Directorio);
			//printf("hola es: %s\n",hola);
			sprintf(numeroArchivo, "%d",arregloAleatorios[i]);
			//printf("El archivo1 es: %s\n",Directorio);
			strcat(Directorio, rutaDirectorio);
			//printf("El archivo1 es: %s\n",Directorio);
			strcat(Directorio,"/");
			//printf("El archivo2 es: %s\n",Directorio);
			strcat(Directorio,numeroArchivo);
			//printf("El archivo0 es: %s\n",Directorio);
			//printf("El directorio es: %s\n",rutaDirectorio);
			//printf("Numero archivo: %s\n",numeroArchivo);
			
			printf("EL DIRECTORIO ES: %s\n",Directorio);
			if ( (archivo=fopen(Directorio,"r")) == NULL) {

				printf("El archivo no encontrado es: %s\n",Directorio);

				perror("Error: El archivo indicado no fue encontrado \n");
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
						Linea = (char*)calloc(1,sizeof(char)*101);
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

			//printf("La Ruta del Archivo es: %s\n", direntp->d_name);

		} 

		//free(Directorio);
		
	}

	closedir(dirp);

	exit(0); */

}




//------------------------------------------------------------------------------------------------//
//                                FIN DE LA DEFINICION DE FUNCIONES                               //
//------------------------------------------------------------------------------------------------//

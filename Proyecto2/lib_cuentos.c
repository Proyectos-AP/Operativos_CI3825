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
* Ultima modificacion: 22/06/2015
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
//                                    DEFINICION DE FUNCIONES                                     //
//------------------------------------------------------------------------------------------------//

int verificarArchivo(char *rutaArchivo) {
	/*
	*
	* Definicion de la funcion: 
	*	Esta funcion dada la ruta de un archivo verifica si el mismo es regular o no.
	*
	* Parametros de entrada:
	* 	- rutaArchivo: Ruta del archivo
	*
	* Parametros de salida:	
	*   - Entero que indica si es un archivo regular.
	*
	*/

	// Definicion de variables:
	struct stat st_info; 	// Se utiliza para verificar si es un archivo regular:
	
	// Se verifica si es un archivo regular:
	if (stat(rutaArchivo, &st_info) < 0) {
		// printf("STAT %d - %s \n",stat(rutaArchivo, &st_info),rutaArchivo); (ESTO NO DEBE ESTAR EN LA ENTREGA FINAL)
		return -1;

	}
 
	return S_ISREG(st_info.st_mode);

}

//------------------------------------------------------------------------------------------------//

int verificarDirectorio(char *nombreDirectorio) {
	/*
	*
	* Definicion de la funcion: 
	*	Esta funcion dada la ruta de un directorio verifica si el mismo es directorio o no.
	*
	* Parametros de entrada:
	*	- nombreDirectorio : Ruta del directorio.
	*
	* Parametros de salida:
	*	- Entero que indica si es un directorio.
	*
	*/

	// Definicion de variables:
	struct stat statbuf; 	// Se utiliza para verificar si es un directorio 

	// printf("El nombreDirectorio es: %s\n",nombreDirectorio); (ESTO NO DEBE ESTAR EN LA ENTREGA FINAL)

	// Se verifica si es un directorio:
   	if (stat(nombreDirectorio,&statbuf) ==-1) {
        //fprintf(stderr,"No se puede obtener el stat del archivo %s:%s\n",nombreDirectorio, strerror(errno));
        // No es un directorio
        return(0);
        //exit(1);
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
	* Definicion de la funcion: 
	*	Esta genera numeros random y los almacena en un arreglo.
	*
	* Parametros de entrada:
	*	- arregloNumeros: Arreglo que almacena todos los numeros random.
	*
	*	- numeroElementos: Numero de elementos del arreglo que almacena
	*						los numeros random.
	*
	*	- numeroMaximo : Limite superior del rango de numeros de donde 
	*					se escogeran los random.
	*
	*	- rutaDirectorio: Ruta de un directorio.
	*
	* Parametros de salida:	
	*	- Ninguno
	*
	*/

	// Declaracion de variables:
	int agregarNumero; 	// Variable que indica si el numero aleatorio puede agregarse al arreglo.
	int numeroActual; 	// Numero aleatorio actual generado.
	int i = 0; 			// Variable de iteracion.
	int j = 0; 			// Variable de iteracion.

	//char numeroDirectorio[20]; (NO DEBE ESTAR EN LA ENTREGA FINAL)
	//int esDirectorio; (NO DEBE ESTAR EN LA ENTREGA FINAL)

	while (i < numeroElementos) {

		//char *rutaAux; (NO DEBE ESTAR EN LA ENTREGA FINAL)
		//rutaAux = (char*)calloc(1,sizeof(char)*100); (NO DEBE ESTAR EN LA ENTREGA FINAL)
		agregarNumero = 1;
		numeroActual = (rand() % numeroMaximo) + 1;

		// (NO DEBE ESTAR EN LA ENTREGA FINAL)	
		// Se convierte el numero del directorio en string:
		//sprintf(numeroDirectorio, "%d",numeroActual);
		//strcat(rutaAux,rutaDirectorio);
		//strcat(rutaAux,numeroDirectorio);
		//esDirectorio = verificarDirectorio(rutaAux);
		//printf("esDirectorio : %d  y la ruta es %s \n",esDirectorio,rutaAux);

		//if (esDirectorio == 1) {

		// Se verifica si el numero aleatorio generado ya se habia generado:
		for (j = 0; j < i; j++) {

			if (numeroActual == arregloNumeros[j]) {
				agregarNumero = 0;
				break;
		}

		}

		// Se agrega el nuevo numero aleatorio al arreglo:
		if (agregarNumero == 1) {

			//printf("El numero actual es %d \n",numeroActual); (ESTO NO DEBE ESTAR EN LA ENTREGA FINAL)
			arregloNumeros[i] = numeroActual;
			i++;

		}
		
		//}

		//free(rutaAux);
	}

}

//------------------------------------------------------------------------------------------------//

int generarNumerosAleatoriosArchivo(int *arregloNumeros,int numeroElementos,int numeroMaximo, char *rutaArchivo) {
	/*
	*
	* Definicion de la funcion: 
	*	Esta genera numeros aleatorios y los almacena en un arreglo.
	*
	* Parametros de entrada:
	*	- arregloNumeros: Arreglo que almacena todos los numeros aleatorios.
	*
	*	- numeroElementos: Numero de elementos del arreglo que almacena
	*	los numeros aleatorios.
	*
	*	- numeroMaximo : Limite superior del rango de numeros de donde 
	*	se escogeran los aleatorios.
	*
	*	- rutaArchivo: Ruta.
	*
	* Parametros de salida:		
	*	- numeroArchivosRegulares : Numero de archivos regulares dentro
	*	del arreglo de numeros aleatorios.
	*
	*/

	// Declaracion de variables:
	char numeroArchivo[20];				// Se utiiliza para convertir el numero de la carpeta a str
	char *rutaAux2;						// Se utiliza para obtener la ruta a verificar
	int agregarNumero;					// Indica si se agrega el numero al arreglo de aleatorios.
	int numeroActual;					// Numero aleatorio generado actual.
	int numeroArchivosRegulares = 0;	// Variabla que indicca el numero de archivos regulares.
	int esArchivo;						// Variable que indica si es un archivo regular.
	int i = 0;							// Variable de iteracion.
	int j = 0;							// Variable de iteracion.

	while (i < numeroElementos) {

		agregarNumero = 1;
		rutaAux2 = (char*)calloc(1,sizeof(char)*50);
		numeroActual = (rand() % 20) + 1;

		// Se convierte el numero del directorio en string:
		sprintf(numeroArchivo, "%d",numeroActual);
		strcat(rutaAux2,rutaArchivo);
		strcat(rutaAux2,"/");
		strcat(rutaAux2,numeroArchivo);

		// Se verifica si la ruta pasada por argumento corresponde a la de un
		// archivo regular.
		esArchivo = verificarArchivo(rutaAux2);
	
		// Se verifica si el numero aleatorio ya se habia generado:
		for (j = 0; j < i; j++) {

			if (numeroActual == arregloNumeros[j]) {
				agregarNumero = 0;
				break;
			}

		}

		// Se agrega el nuevo numero aleatorio al arreglo de aleatorios:
		if (agregarNumero == 1) {

			arregloNumeros[i] = numeroActual;
			i++;

			if (esArchivo==1){

				numeroArchivosRegulares++;
			}

		}
		free(rutaAux2);

	}

	return(numeroArchivosRegulares);

}

//------------------------------------------------------------------------------------------------//

int contarDirectorios(char *rutaDirectorio) {
	/*
	*
	* Definicion de la funcion: 
	*	Esta funcion dada la ruta de un directorio cuenta cuantos
	*	archivos hay en el mismo.
	*
	* Parametros de entrada:
	*	- rutaDirectorio: Ruta de un directorio.
	*
	* Parametros de salida:
	*	- contadorArchivos : Numero de archivos dentro del directorio.
	*				
	*/

	// Declaracion de variables:
	int contadorArchivos = 0;	// Se inicializa la variable que contara el numero de carpetas.
	DIR *dirp;					// Variable que se utilizara para abrir el directorio.
	struct dirent *direntp;		// Estructura que permite leer los archivos.
	
	// Se verifica si se puede abrir el directorio:
	if ((dirp = opendir(rutaDirectorio)) == NULL) {

		fprintf(stderr,"No se puede abrir el directorio %s\n",rutaDirectorio);
		strerror(errno);
		exit(1);
	}

	// Se cuenta el numero de archivos del directorio:
	while ((direntp = readdir(dirp)) != NULL ) {

		contadorArchivos++;
	}

	// Se descartan las entradas "." y ".." correspondientes al directorio actual y padre:
	contadorArchivos = contadorArchivos - 2;
	closedir(dirp);

	// Se devuelve el numero de archivos del directorio:
	return(contadorArchivos);

}

//------------------------------------------------------------------------------------------------//
//                                FIN DE LA DEFINICION DE FUNCIONES                               //
//------------------------------------------------------------------------------------------------//
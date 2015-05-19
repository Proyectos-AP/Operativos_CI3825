/*

 Archivo: map.c

 Nombres:
	Alejandra Cordero / Carnet: 12-10645
	Pablo Maldonado   / Carnet: 12-10561

 Descripcion:

 Ultima modificacion: 09/05/2015

*/

// Directivas de Preprocesador:

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "lib_procesos.h"

//----------------------------------------------------------------------------//
//                          INICIO DEL CODIGO PRINCIPAL                       //
//----------------------------------------------------------------------------//

int main(int argc, char *argv[]) {

	/*

		Definicion de la funcion:	

			Dado el nombre de un archivo esta funcion lee cada 
		linea del mismo y realiza map sobre cada una de ellas.
	
		Parametros de entrada:

			- argv[0]: Nombre del archivo que sera leido.
	
	 	Parametros de salida:
	 		Ninguno.
	
	*/

	// Declaracion de variables:
	int FinalArchivo=0;
	char nombreSalida[30];
	char *persona;
	char *amigos;
	FILE *archivoEntrada;
	FILE *archivoDeSalida;

	// Se obtiene el PID del proceso:
	int miPID = getpid();
	
	// Se convierte el PID en string
	sprintf(nombreSalida, "%d",miPID);
	// Se concatena el PID con ".txt"
	// para obtener el nombre del archivo de salida:
	strcat(nombreSalida, ".txt");


	// Se abre el archivo de salida:
	archivoDeSalida = fopen(nombreSalida,"a");


	// Se procede a leer el archivo de entrada.

	// No existe el archivo
	if ( fopen(argv[0],"r") == NULL ) {

		exit(1);
		}

	else {

		archivoEntrada = fopen(argv[0],"r");
		fseek(archivoEntrada,0,SEEK_END);

   		if (ftell(archivoEntrada) == 0 )	{
   			
   			//El archivo esta vacio.
			fclose(archivoEntrada);
   			remove(argv[0]);
   			exit(0);
    	}

    	else {

    		fseek(archivoEntrada,0,SEEK_SET);
	

			while(FinalArchivo == 0){

				persona = (char*)malloc(sizeof(char)*15);
				amigos = (char*)malloc(sizeof(char)*30);
				fscanf(archivoEntrada," %[^ ->] -> %[^\n]\n" ,persona,amigos);

				mapProcesos(persona,amigos,archivoDeSalida);
				free(persona);
				free(amigos);
				// Se verifica si se ha llegado al fin del archivo
				if(feof(archivoEntrada)== 1){
					FinalArchivo = 1;
					fclose(archivoEntrada);	
					//remove(argv[0]);
				}

			}

		}
	}
	// Se cierra el archivo de salida:
	fclose(archivoDeSalida);	
	return(0);

}
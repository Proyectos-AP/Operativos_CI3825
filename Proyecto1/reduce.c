/*

 Archivo: reduce.c

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
		linea del mismo y realiza reduce sobre cada una de ellas.
	
		Parametros de entrada:

			- argv[0]: Nombre del archivo que sera leido.
	
	 	Parametros de salida:
	 		Ninguno.
	
	*/

	// Se obtiene el PID del proceso:
	int miPID = getpid();
	int FinalArchivo=0;
	int realizaReduce;
	char nombreSalida[30];
	char *Persona1;
	char *Persona2;
	char *Amigos;
	FILE *archivoEntrada;
	FILE *archivoSalida;

	
	// Se convierte el PID en string
	sprintf(nombreSalida, "%d",miPID);
	strcat(nombreSalida, ".txt");

	// Se lee el archivo de entrada
	if ( fopen(argv[0],"r") == NULL ) {

		exit(1);
	}

	else{


		archivoEntrada = fopen(argv[0],"r");
		fseek(archivoEntrada,0,SEEK_END);

		archivoSalida = fopen(nombreSalida,"w");


   		if (ftell(archivoEntrada) == 0 )	{
   			
   			//El archivo esta vacio.
			fclose(archivoEntrada);	
			remove(argv[0]);
   			exit(0);
    	}

    	else{

    		fseek(archivoEntrada,0,SEEK_SET);

    		while(FinalArchivo == 0){

  
    			realizaReduce = fgetc(archivoEntrada);
    			realizaReduce = realizaReduce - 48;

    			if (realizaReduce == 0) {

    				Persona1 = (char*)malloc(sizeof(char)*15);
    				Persona2 = (char*)malloc(sizeof(char)*15);

    				fscanf(archivoEntrada," (%[^ (] %[^ )] ) \n" ,Persona1,Persona2);
					fprintf(archivoSalida,"(%s %s) -> -None-\n",Persona1,Persona2);
					free(Persona1);
					free(Persona2);
					while (fgetc(archivoEntrada) != 10);

    			}

    			else if (realizaReduce == 1) {

					Persona1 = (char*)malloc(sizeof(char)*15);
    				Persona2 = (char*)malloc(sizeof(char)*15);
					Amigos = (char*)malloc(sizeof(char)*100);

    				fscanf(archivoEntrada," (%[^ (] %[^ )] ) -> %[^\n]\n",Persona1,Persona2,Amigos);

    				// Se aplica reduce
    				ReduceProcesos(Persona1,Persona2,Amigos,archivoSalida);
    				free(Persona1);
					free(Persona2);
					free(Amigos);

    			}

    			// Se verifica si se ha llegado al fin del archivo
				if(feof(archivoEntrada)== 1){
					FinalArchivo = 1;
					fclose(archivoEntrada);	
					remove(argv[0]);
					fclose(archivoSalida);
				}

    		}

    	}

	}

	return(0);

}

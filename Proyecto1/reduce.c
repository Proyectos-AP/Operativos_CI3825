/*
*
* Archivo: reduce.c
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

void main(int argc, char *argv[]) {

	/*
	*
	 Definicion de la funcion:	
	
	 Parametros de entrada:
	
	 Parametros de salida:
	
	*/

	 printf("EL PARAMETRO QUE ME PASARON ES : %s\n",argv[0]);
	 // Se crea un archivo con el nombre del pid del proceso 
	 //  que esta ejecutando el codigo.

	 	// Se obtiene el PID del proceso:
	int miPID = getpid();
	int FinalArchivo=0;
	int seRealizaReduce;
	FILE *archivoEntrada;
	FILE *archivoSalida;
	char *Persona1;
	char *Persona2;
	char *Amigos1;
	char *Amigos2;
	
	// Se convierte el PID en string
	char pidStr[20];
	char nombreSalida[25];
	sprintf(nombreSalida, "%d",miPID);

	// Se concatena el PID con ".txt"
	// para obtener el nombre del archivo de salida:
	strcat(nombreSalida, ".txt");

	// Se abre el archivo de entrada:
	archivoEntrada = fopen(argv[0],"r");

	// Se abre el archivo de salida:
	archivoSalida = fopen(nombreSalida,"a");


	if ( fopen(argv[0],"r") == NULL ) {

		printf("No tengo trabajo asignado\n");
		exit(1);
		}

	else {

		archivoEntrada = fopen(argv[0],"r");
		fseek(archivoEntrada,0,SEEK_END);

   		if (ftell(archivoEntrada) == 0 )	{
   			
   			//El archivo esta vacio.
   			printf("No tengo trabajo asignado\n");
   			exit(0);
    	}
    	else{

    		fseek(archivoEntrada,0,SEEK_SET);
    		while( FinalArchivo == 0 ){
	    		Persona1 = (char*)malloc(sizeof(char)*15);
				Persona2 = (char*)malloc(sizeof(char)*15);
				Amigos1 = (char*)malloc(sizeof(char)*30);
				Amigos2 = (char*)malloc(sizeof(char)*30);
	    		fscanf(archivoEntrada," ( %[^ (] %[^ )] ) -> %[^\n]\n" ,Persona1,Persona2,Amigos1);

	    		printf("-----------------------------\n");
	    		printf("Persona1 es: %s.\n",Persona1);
				printf("Persona2 es: %s.\n",Persona2);
				//printf("Amigos1 es: %s.\n",Amigos1);
				printf("-----------------------------\n");

	    		// Se verifica si se ha llegado al fin del archivo
				if(feof(archivoEntrada)== 1){
					FinalArchivo = 1;
					remove(argv[0]);

					// Se cierra el archivo de entrada
					fclose(archivoEntrada);	
				}
    		}

    	}

		

	}

	// Se cierra el archivo de salida
	fclose(archivoSalida);

}
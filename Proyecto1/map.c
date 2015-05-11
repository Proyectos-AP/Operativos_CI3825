/*
*
* Archivo: map.c
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


// Inicio del codigo principal:

void main(int argc, char *argv[]) {

	/*
	*
	* Definicion de la funcion:	
	*
	* Parametros de entrada:
	*
	* Parametros de salida:
	*
	*/

	// Se obtiene el PID del proceso:
	int miPID = getpid();
	
	// Se convierte el PID en string
	char pidStr[20];
	sprintf(pidStr, "%d",miPID);
	int FinalArchivo=0;

	// Se concatena el PID con ".txt"
	// para obtener el nombre del archivo de salida:
	char nombreSalida[30];
	char inicio[40];
	strcpy(inicio,pidStr);
	strcpy(nombreSalida,inicio);
	strcat(nombreSalida, ".txt");

	printf("EL PARAMETRO QUE ME PASARON ES : %s\n",argv[0]);


	// Se lee el archivo de entrada
	FILE *archivoEntrada;

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
			char *Persona;
			char *Amigos;	

			while(FinalArchivo == 0){

				Persona = (char*)malloc(sizeof(char)*15);
				Amigos = (char*)malloc(sizeof(char)*30);
				fscanf(archivoEntrada," %[^ ->] -> %[^\n]\n" ,Persona,Amigos);
				printf("Persona: %s -> %s \n",Persona,Amigos);

				// Se verifica si se ha llegado al fin del archivo
				if(feof(archivoEntrada)== 1){
					FinalArchivo = 1;
					remove(argv[0]);
					fclose(archivoEntrada);
				}

			}
		}

	}

	char Probando[] = "Ana -> Bernardo Cristina David";

	char *token;
	char *rest = Probando;

  	char *p;
  
  	p = strtok (Probando," ");
  
  	while (p != NULL) {
    	printf ("%s\n", p);
    	p = strtok (NULL, " ");
 	 }

	// Se abre el archivo de salida:

	FILE *archivoSalida;

	archivoSalida = fopen(nombreSalida,"a");

	fprintf(archivoSalida,"(%s %s) -> %s",Probando,Probando,Probando);


	// Se cierra el archivo de salida:
	fclose(archivoSalida);	
	exit(0);
}
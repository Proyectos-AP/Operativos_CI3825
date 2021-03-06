/*
*
* Archivo: lib_cuentos.h
*
* Nombres:
*	Alejandra Cordero / Carnet: 12-10645
*	Pablo Maldonado   / Carnet: 12-10561
*
* Descripcion: Encabezado del archivo 
* lib_cuentos.c 
*
* Ultima modificacion: 22/06/2015
*
*/

#ifndef FUNCIONES_H
#define FUNCIONES_H

void generarNumerosAleatoriosDirectorio(int *arregloNumeros,int numeroElementos,int numeroMaximo, char *rutaDirectorio);
int generarNumerosAleatoriosArchivo(int *arregloNumeros,int numeroElementos,int numeroMaximo, char *rutaArchivo);
int verificarDirectorio(char *nombreDirectorio);
int verificarArchivo(char *rutaArchivo);
int contarDirectorios(char *rutaDirectorio);
void leerDirectorio(char *rutaDirectorio,int *arregloAleatorios,int numeroElementos);

#endif

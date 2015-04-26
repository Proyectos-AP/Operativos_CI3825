/*
*
* Archivo: lib_archivos.h
*
* Nombres:
*	Alejandra Cordero / Carnet: 12-10645
*	Pablo Maldonado   / Carnet: 12-10561
*
* Descripcion: Encabezado del archivo
* lib_archivos.c
*
* Ultima modificacion: 18/04/2015
*
*/

#ifndef FUNCIONES_H
#define FUNCIONES_H

// Definicion de Tipos:

typedef struct pregunta PREGUNTA; 

void CambiarSaltoDeLinea(char *frase);

PREGUNTA* LeerBaseDeDatos(char *nombre_archivo);

void EliminarLista(PREGUNTA *CabeceraData);

void imprimirPreguntas(PREGUNTA *CabeceraArchivo);

void imprimirPreguntasComplejidad(PREGUNTA *CabeceraArchivo,char complejidad);

int Eliminar(PREGUNTA** Cabecera,int clave);

void  insertarPregunta(PREGUNTA** CabeceraArchivo);

void escribirArchivo(PREGUNTA *CabeceraArchivo,char *nombre_archivo);

int verificarCodigo(PREGUNTA *CabeceraArchivo, int codigo);

#endif
/*

 Archivo: lib_procesos.h

 Nombres:
	Alejandra Cordero / Carnet: 12-10645
	Pablo Maldonado   / Carnet: 12-10561

 Descripcion: Encabezado del archivo
 lib_procesos.c

 Ultima modificacion: 26/04/2015

*/

#ifndef FUNCIONES_H
#define FUNCIONES_H

// Definicion de Tipos:

typedef struct lista LISTA;
typedef struct listaAmigos LISTAAMIGOS;
void EliminarEstructuraLista(LISTA **Cabecera);
void EliminarListaAmigos(LISTAAMIGOS **lista);
void LeerArchivo(char *nombre_archivo,int numeroProcesos);
void mapProcesos(char *Persona,char *Amigos, FILE *archivoSalida);
void LeerTrabajoMap(int numeroArchivos,LISTAAMIGOS **listaAmigos, int NombreArchivos[]);
void ReparteTrabajoReduce(int numeroArchivos,LISTAAMIGOS *listaAmigos);
void ReduceProcesos(char *Persona1,char *Persona2,char *Amigos,FILE *archivoSalida);
void LeerTrabajoReduce(int numeroArchivos,char *archivoSalida,int nombre_archivos[]);

#endif

/*

 Archivo: lib_procesos.h

 Nombres:
	Alejandra Cordero / Carnet: 12-10645
	Pablo Maldonado   / Carnet: 12-10561

 Descripcion: Encabezado del archivo
 lib_hilos.c

 Ultima modificacion: 26/04/2015

*/

#ifndef FUNCIONES_H
#define FUNCIONES_H

typedef struct lista LISTA;
typedef struct listaAmigos LISTAAMIGOS;
typedef struct listaMapH LISTAMAPH;
typedef struct listaCabeceraMapH LISTACABECERAMAPH;
typedef struct listaCabeceraAmigos LISTACABECERAAMIGOS;
void EliminarEstructuraLista(LISTA **Cabecera);
void EliminarListaAmigos(LISTAAMIGOS **lista);
void EliminarRedundancia(LISTAAMIGOS **listaAmigosPadre);
int ReduceDeHilos(LISTAAMIGOS *auxListaPadres,LISTA **CabeceraLista);
void ImprimirEnArchivoSalida(LISTAAMIGOS *auxListaPadres,FILE *archivo_salida,LISTA *CabeceraLista,int amigosEnComun);
void CreaListaAmigos(LISTAMAPH *auxHilo,LISTA **amigos);
void LeerArchivo(char *nombre_archivo,int numeroProcesos,LISTACABECERAMAPH **CabeceraMapH);

#endif

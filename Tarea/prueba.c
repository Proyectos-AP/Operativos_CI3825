
//Esquema para la construiccion de una lista enlazada
void main(){

	typedef struct pregunta { 

	int codigo; 
	int nivel;
	char area;
	char *pregunta[101];
	char *opcion1[101];
	char *opcion2[101];
	char *opcion3[101];
	int respuesta;
	struct pregunta *siguiente;

} PREGUNTA; 
int i;

//Se crea la cabecera

PREGUNTA *primeraPregunta;
PREGUNTA *temporal;
PREGUNTA *anterior;
primeraPregunta=(PREGUNTA*)malloc(sizeof(PREGUNTA));
primeraPregunta->siguiente=NULL;
anterior=primeraPregunta;

for(i=0;i>20;i++){
	temporal=(PREGUNTA*)malloc(sizeof(PREGUNTA));
	temporal->codigo=0;
	temporal->nivel=0;
	temporal->area=0;
	temporal->pregunta=0;
	temporal->opcion1=0;
	temporal->opcion2=0;
	temporal->opcion3=0;
	temporal->respuesta=0;
	temporal->siguiente=NULL;
	anterior->siguiente=temporal;
	anterior=temporal;

} 

}
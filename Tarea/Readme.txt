Universidad Simón Bolívar
Departamento de Computación y Tecnología de la Información
Sistemas de Operación I (CI3825)
Nombres: 
	Alejandra Cordero / Carnet: 12-10645
	Pablo Maldonado   / Carnet: 12-10561


- Detalles de implementación:

	1.- La lectura de datos por consola se implementó de forma robusta para evitar tanto
	fallos en el código, como para evitar que el programa tuviera que ejecutarse de nuevo
	al introducir un dato erróneo, a excepción de los casos en los que se solicita un número
	entero para insertar el código de una nueva pregunta y eliminar una existente. 

	2.- Al momento de insertar, la base de datos se leerá de forma implícita para crear
	la estructura de lista enlazada que almacenará las preguntas durante la ejecucion del
	programa.

	3.- La lista enlazada posee una "Cabecera" que consiste en un apuntador 
	a la primera pregunta almacenada.

	4.- La memoria reservada para las preguntas es liberada al momento de 
	finalizar la ejecución del programa.

	5.- Se implementó una función que sustituye los posibles saltos de linea que pudiesen
	aparecer en el contenido de una pregunta por un espacio en blanco al momento de 
	realizar la lectura en el archivo de entrada.

- Referencias consultadas:

	1.- http://www.tutorialspoint.com/cprogramming/c_command_line_arguments.htm
		Tema: Pase de argumentos al main().
		Última visita realizada: 18/04/2015

	2.-	Dirección: https://gsamaras.wordpress.com/code/caution-when-reading-char-with-scanf-c/
		Tema: Lectura de chars con varios scanfs. 
		Última visita realizada: 18/04/2015

	3.- http://stackoverflow.com/questions/18496282/why-do-i-get-a-label-can-only-be-part-of-a-statement-and-a-declaration-is-not-a
		Tema: Declaración de variable luego de un label.
		Última visita realizada: 18/04/2015

	4.- http://stackoverflow.com/questions/9935401/c-programming-end-of-file-operation
		Tema: Verificación de final de archivo.
		Última visita realizada: 18/04/2015

	5.- http://bytes.com/topic/c/answers/811451-fscanf-read-string-between-mark
		Tema: Leer un string según un formato específico.
		Última visita realizada: 18/04/2015

	6.- http://stackoverflow.com/questions/797318/how-to-split-a-string-literal-across-multiple-lines-in-c-objective-c
		Tema: Dividir una instrucción en varias líneas.
		Última visita realizada: 18/04/2015
	
	7.- http://solocodigo.com/46029/c-fgets-y-linea-innecesaria-solucionado/
		Tema: Cambio de caracter en un string.
		Última visita realizada: 18/04/2015

	8.- http://stackoverflow.com/questions/7259238/how-to-forward-typedefd-struct-in-h
		Tema: Definición de un header en C.
		Última visita realizada: 18/04/2015


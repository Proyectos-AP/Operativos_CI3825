#
# Archivo: Makefile
#
# Nombres:
#	Alejandra Cordero / Carnet: 12-10645
#	Pablo Maldonado   / Carnet: 12-10561
#
# Ultima modificacion: 18/04/2015
#
#
Main: Main.o  
	gcc Main.c lib_archivos.c -o pregunta
pregunta.o: Main.c lib_archivos.h
	gcc -c Main.c
clean:
	rm pregunta

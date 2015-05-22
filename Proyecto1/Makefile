#
# Archivo: Makefile
#
# Nombres:
#	Alejandra Cordero / Carnet: 12-10645
#	Pablo Maldonado   / Carnet: 12-10561
#
# Ultima modificacion: 09/05/2015
#
#

Main:
	gcc -Wall -g mainP.c lib_procesos.c -o friendfindP
	gcc -Wall -g mainH.c -pthread lib_hilos.c -o friendfindH
	gcc -Wall -g map.c lib_procesos.c -o map
	gcc -Wall -g reduce.c lib_procesos.c -o reduce

friendfindP.o : mainP.c lib_procesos.h
	gcc -c mainP.c

friendfindH.o : mainH.c lib_hilos.h
	gcc -c mainH.c

map.o : map.c lib_procesos.h
	gcc -c map.c

reduce.o : reduce.c lib_procesos.h
	gcc -c reduce.c

clean:
	rm friendfindP
	rm friendfindH
	rm map
	rm reduce

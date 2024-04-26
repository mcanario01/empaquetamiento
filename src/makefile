all:
	g++ -c funciones.cpp -lwiringPi
	g++ -c main.cpp -o -lwiringPi
	g++ comunicacion main.o funciones.o -lwiringPi
	rm *.o

run:
	g++ -c funciones.cpp -lwiringPi
	g++ -c main.cpp -o -lwiringPi
	g++ comunicacion main.o funciones.o -lwiringPi
	rm *.o
	cd ../
	sudo ./comunicacion


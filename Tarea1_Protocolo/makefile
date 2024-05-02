all:
	g++ -c funciones.cpp -lwiringPi
	g++ -c emisor.cpp -o -lwiringPi
	g++ emisor emisor.o funciones.o -lwiringPi
	g++ -c receptor.cpp -o -lwiringPi
	g++ receptor receptor.o funciones.o -lwiringPi
	rm *.o

receptor:
	g++ -c funciones.cpp -lwiringPi
	g++ -c receptor.cpp -o -lwiringPi
	g++ receptor receptor.o funciones.o -lwiringPi
	rm *.o
	cd ../
	sudo ./receptor

emisor:
	g++ -c funciones.cpp -lwiringPi
	g++ -c emisor.cpp -o -lwiringPi
	g++ emisor emisor.o funciones.o -lwiringPi
	rm *.o
	cd ../
	sudo ./emisor
all:
	g++ -c funciones.cpp 
	g++ -c emisor.cpp -o emisor.o
	g++ emisor.o funciones.o -o emisor
	g++ -c receptor.cpp -o receptor.o
	g++ receptor.o funciones.o -o receptor
	del *.o

receptor:
	g++ -c funciones.cpp 
	g++ -c receptor.cpp -o receptor.o
	g++ receptor.o funciones.o -o receptor
	del *.o
	cd ../
	sudo ./receptor

emisor:
	g++ -c funciones.cpp 
	g++ -c emisor.cpp -o emisor.o
	g++ emisor.o funciones.o -o emisor
	del *.o
	cd ../
	sudo ./emisor
ifeq ($(OS),Windows_NT)
	uname_S := Windows
else
	uname_S := $(shell uname -s)
endif

ifeq ($(uname_S), Windows)
all:
	g++ -c funciones.cpp -o funciones.o
	g++ -c emisor.cpp -o emisor.o
	g++ emisor.o funciones.o -o emisor
	g++ -c receptor.cpp -o receptor.o
	g++ receptor.o funciones.o -o receptor
	del *.o
	
run-emisor:
	./emisor.exe

run-receptor:
	./receptor.exe

clear:
	del *.o
	del *.exe

endif

ifeq ($(uname_S), Linux)
all:
	g++ -c funciones.cpp -o funciones.o -lwiringPi
	g++ -c emisor.cpp -o emisor.o -lwiringPi
	g++ emisor.o funciones.o -o emisor -lwiringPi
	g++ -c receptor.cpp -o receptor.o -lwiringPi
	g++ receptor.o funciones.o -o receptor -lwiringPi
	rm *.o

run-emisor:
	sudo ./emisor

run-receptor:
	sudo ./receptor

clear:
	rm -rf *.o
	rm ./emisor
	rm ./receptor

endif
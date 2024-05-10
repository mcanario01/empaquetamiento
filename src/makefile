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
	g++ envia_bytes.cpp -o envia_bytes
	g++ recibe_bytes.cpp -o recibe_bytes
	g++ delay.cpp -o delay
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
	g++ -c funciones.cpp -lwiringPi
	g++ -c envia_bytes.cpp -lwiringPi
	g++ -c recibe_bytes.cpp -lwiringPi
	g++ -o emisor envia_bytes.o funciones.o -lwiringPi
	g++ -o receptor recibe_bytes.o funciones.o -lwiringPi
	g++ -o delay delay.cpp -lwiringPi

run-emisor:
	sudo ./emisor

run-receptor:
	sudo ./receptor

clear:
	rm -rf *.o
	rm ./emisor
	rm ./receptor

endif
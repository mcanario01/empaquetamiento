MAKEFLAGS += --silent

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
	g++ emisor.cpp -o emisor
	g++ receptor.cpp -o receptor
	g++ delay.cpp -o delay
	del *.o
	
run-emisor:
	./emisor.exe

run-receptor:
	./receptor.exe

clear-all:
	del *.o
	del *.exe

endif

ifeq ($(uname_S), Linux)
all:
	clear
	echo "Compilando librerias..."
	g++ -c funciones.cpp -lwiringPi
	echo "Compilando emisor..."
	g++ -c emisor.cpp -lwiringPi
	echo "Compilando receptor..."
	g++ -c receptor.cpp -lwiringPi
	echo "Uniendo archivos..."
	g++ -o emisor emisor.o funciones.o -lwiringPi
	g++ -o receptor receptor.o funciones.o -lwiringPi
	echo "Compilando delay..."
	g++ -o delay delay.cpp -lwiringPi
	echo "Borrando archivos temporales..."
	rm -rf *.o
	clear

run-emisor:
	clear
	echo "Compilando libreria..."
	g++ -c funciones.cpp -lwiringPi
	echo "Compilando emisor..."
	g++ -c emisor.cpp -lwiringPi
	echo "Uniendo archivos..."
	g++ -o emisor emisor.o funciones.o -lwiringPi
	echo "Borrando archivos temporales..."
	rm -rf *.o
	echo "Ejecutando emisor..."
	clear
	sudo ./emisor

run-receptor:
	clear
	echo "Compilando libreria..."
	g++ -c funciones.cpp -lwiringPi
	echo "Compilando receptor..."
	g++ -c receptor.cpp -lwiringPi
	echo "Uniendo archivos..."
	g++ -o receptor receptor.o funciones.o -lwiringPi
	echo "Borrando archivos temporales..."
	rm -rf *.o
	echo "Ejecutando receptor..."
	clear
	sudo ./receptor

run-delay:
	clear
	echo "Compilando delay..."
	g++ -o delay delay.cpp -lwiringPi
	echo "Borrando archivos temporales..."
	rm -rf *.o
	echo "Ejecutando delay..."
	clear
	sudo ./delay

clear-all:
	clear
	echo "Borrando archivos objeto..."
	rm -rf *.o
	echo "Borrando ejecutables..."
	rm ./emisor
	rm ./receptor
	clear

endif
.PHONY: all
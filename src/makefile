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
	echo "\nCompilando librerias...\n"
	g++ -c funciones.cpp -lwiringPi
	echo "\nCompilando emisor...\n"
	g++ -c emisor.cpp -lwiringPi
	echo "\nCompilando receptor...\n"
	g++ -c receptor.cpp -lwiringPi
	echo "\nUniendo archivos...\n"
	g++ -o emisor emisor.o funciones.o -lwiringPi
	g++ -o receptor receptor.o funciones.o -lwiringPi
	echo "\nCompilando delay...\n"
	g++ -o delay delay.cpp -lwiringPi
	echo "\nBorrando archivos temporales...\n"
	clear
	rm -rf *.o

run-emisor:
	clear
	echo "\nCompilando libreria...\n"
	g++ -c funciones.cpp -lwiringPi
	echo "\nCompilando emisor...\n"
	g++ -c emisor.cpp -lwiringPi
	echo "\nUniendo archivos...\n"
	g++ -o emisor emisor.o funciones.o -lwiringPi
	echo "\nBorrando archivos temporales...\n"
	rm -rf *.o
	echo "\nEjecutando emisor...\n"
	clear
	sudo ./emisor

run-receptor:
	clear
	echo "\nCompilando libreria...\n"
	g++ -c funciones.cpp -lwiringPi
	echo "\nCompilando receptor...\n"
	g++ -c receptor.cpp -lwiringPi
	echo "\nUniendo archivos...\n"
	g++ -o receptor receptor.o funciones.o -lwiringPi
	echo "\nBorrando archivos temporales...\n"
	rm -rf *.o
	echo "\nEjecutando receptor...\n"
	clear
	sudo ./receptor

run-delay:
	clear
	echo "\nCompilando delay...\n"
	g++ -o delay delay.cpp -lwiringPi
	echo "\nBorrando archivos temporales...\n"
	rm -rf *.o
	echo "\nEjecutando delay...\n"
	clear
	sudo ./delay

clear-all:
	clear
	echo "\nBorrando archivos temporales y ejecutables...\n"
	rm -rf *.o
	rm ./emisor
	rm ./receptor
	clear

endif
.PHONY: all
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

clear:
	del *.o
	del *.exe

endif

ifeq ($(uname_S), Linux)
all:
	g++ -c funciones.cpp -lwiringPi
	g++ -c emisor.cpp -lwiringPi
	g++ -c receptor.cpp -lwiringPi
	g++ -o emisor emisor.o funciones.o -lwiringPi
	g++ -o receptor receptor.o funciones.o -lwiringPi
	g++ -o delay delay.cpp -lwiringPi
	rm -rf *.o

run-emisor:
	g++ -c funciones.cpp -lwiringPi
	g++ -c emisor.cpp -lwiringPi
	g++ -o emisor emisor.o funciones.o -lwiringPi
	rm -rf *.o
	sudo ./emisor

run-receptor:
	g++ -c funciones.cpp -lwiringPi
	g++ -c receptor.cpp -lwiringPi
	g++ -o receptor receptor.o funciones.o -lwiringPi
	rm -rf *.o
	sudo ./receptor

run-delay:
	g++ -o delay delay.cpp -lwiringPi
	rm -rf *.o
	sudo ./delay

clear:
	rm -rf *.o
	rm ./emisor
	rm ./receptor

endif
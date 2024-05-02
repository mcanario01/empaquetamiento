all:
	g++ envia_bytes.cpp -o enviar -lwiringPi
	g++ recibe_bytes.cpp -o recibir -lwiringPi
	g++ delay.cpp -o clock -lwiringPi

Enviar:
	sudo ./enviar

Recibir:
	sudo ./recibir

Delay:
	sudo ./delay &

todo:
	sudo ./delay &
	sudo ./recibir &
	sudo ./enviar

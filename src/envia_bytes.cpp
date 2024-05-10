// INCLUSIONES
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"
#include <cstring>

// MACROS
#define DELAY_PIN 0
#define TX_PIN 2

#define BYTE unsigned char // definición de tipo de dato BYTE

#define CANTIDAD_DE_BYTES 63

#define BYTES_EXTRAS 4

// DECLARACION DE PROTOTIPOS
void cb(void);			   // función de interrupción
void iniciarTransmision(); // función para iniciar la transmisión

// VARIABLES GLOBALES
volatile int numero_bits_transmitidos = 0;							  // guarda el número de bits transmitidos
volatile int numero_bytes_transmitidos = 0;						  // guarda el número de bytes transmitidos

Protocolo mensaje;

int numero_de_unos = 0;							  // es el número de unos en el byte
bool transmisionIniciada = false;				  // indica si la transmisión está activa

int main()
{
	// INICIA WIRINGPI
	if (wiringPiSetup() == -1)
		exit(1); // si hay error al iniciar wiringPi, termina el programa

	// CONFIGURA INTERRUPCION PIN CLOCK (PUENTEADO A PIN PWM)
	if (wiringPiISR(DELAY_PIN, INT_EDGE_RISING, &cb) < 0)
	{
		printf("Unable to start interrupt function\n"); // si hay error al configurar la interrupción, imprime mensaje
	}

	// CONFIGURA PINES DE ENTRADA SALIDA
	pinMode(TX_PIN, OUTPUT);

	obtenerInformacion(mensaje); // obtiene la información del usuario
	empaquetar(mensaje);			// empaqueta el mensaje

	iniciarTransmision();		// inicia la transmisión
	while (transmisionIniciada) // mientras la transmisión esté activa
		delay(2000);			// espera 2 segundos para que termine la transmisión

	return 0;
}

void cb(void)
{
	if (transmisionIniciada)
	{ // si la transmisión está activa
		// Escribe en el pin TX
		if (numero_bits_transmitidos != 0)
		{							 // si el numero de bits es 0
			digitalWrite(TX_PIN, (mensaje.Frames[numero_bytes_transmitidos] >> (numero_bits_transmitidos - 1)) & 0x01);
		}
		else
		{ // Bit de dato
			digitalWrite(TX_PIN, 0); // Si el numero de bits es 0, el bit de dato es 0
		}

		// Actualiza contador de bits
		numero_bits_transmitidos++;

		// Actualiza contador de bytes
		if (numero_bits_transmitidos == 9)
		{
			numero_bits_transmitidos = 1;
			numero_bytes_transmitidos++;

			// Finaliza la comunicación
			if (numero_bytes_transmitidos == CANTIDAD_DE_BYTES + BYTES_EXTRAS)
			{
				transmisionIniciada = false;
				numero_bytes_transmitidos = 0;
				numero_bits_transmitidos = 0;
			}
		}
	}
	else
	{
		// Canal en reposo
		digitalWrite(TX_PIN, 1);
	}
}

void iniciarTransmision()
{
	transmisionIniciada = true;
}

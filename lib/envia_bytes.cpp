// INCLUSIONES
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

// MACROS
#define DELAY_PIN 0
#define TX_PIN 2

#define BYTE unsigned char // definición de tipo de dato BYTE

// DECLARACION DE PROTOTIPOS
void cb(void);			   // función de interrupción
void iniciarTransmision(); // función para iniciar la transmisión

// VARIABLES GLOBALES
volatile int numero_bits_transmitidos = 0;							  // guarda el número de bits transmitidos
volatile int numero_bytes_transmitidos = 0;						  // guarda el número de bytes transmitidos
int largo_de_bytes = 10;						  // es el número de bytes a transmitir

BYTE cadena[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; // guarda los bytes a transmitir

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

	//  printf("Delay\n");
	//  delay(5000);
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
		if (numero_bits_transmitidos == 0)
		{							 // si el numero de bits es 0
			digitalWrite(TX_PIN, 0); // Bit de inicio
		}
		else if (numero_bits_transmitidos < largo_de_bytes - 1)
		{
			digitalWrite(TX_PIN, (cadena[numero_bytes_transmitidos] >> (numero_bits_transmitidos - 1)) & 0x01); // Bit de dato
																		 // printf("%d",(cadena[numero_bytes_transmitidos]>>(numero_bits_transmitidos-1))&0x01);
		}
		else if (numero_bits_transmitidos == largo_de_bytes - 1)
		{
			//      printf("\n");
			numero_de_unos = (cadena[numero_bytes_transmitidos] & 0x01) + ((cadena[numero_bytes_transmitidos] & 0x02) >> 1) + ((cadena[numero_bytes_transmitidos] & 0x04) >> 2) +
					((cadena[numero_bytes_transmitidos] & 0x08) >> 3) + ((cadena[numero_bytes_transmitidos] & 0x10) >> 4) + ((cadena[numero_bytes_transmitidos] & 0x20) >> 5) +
					((cadena[numero_bytes_transmitidos] & 0x40) >> 6) + ((cadena[numero_bytes_transmitidos] & 0x80) >> 7);
			digitalWrite(TX_PIN, numero_de_unos % 2 == 0); // Bit de paridad
		}
		else
		{
			digitalWrite(TX_PIN, 1); // Canal libre durante 2 clocks
		}

		// Actualiza contador de bits
		numero_bits_transmitidos++;

		// Actualiza contador de bytes
		if (numero_bits_transmitidos == 11)
		{
			numero_bits_transmitidos = 0;
			numero_bytes_transmitidos++;

			// Finaliza la comunicación
			if (numero_bytes_transmitidos == largo_de_bytes)
			{
				transmisionIniciada = false;
				numero_bytes_transmitidos = 0;
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

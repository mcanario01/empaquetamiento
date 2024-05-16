// INCLUSIONES
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"

// MACROS
#define DELAY_PIN 23
#define RX_PIN 3
#define LARGO_MENSAJE 68

#define BYTE unsigned char

// PROTOTIPOS
void processBit(bool level);
void cb(void);

// VARIABLES GLOBALES
volatile int errors = 0;
volatile int nbits = 0;
volatile int nbytes = 0;
bool transmissionStarted = false;
bool parity = 0;
int nones = 0;
BYTE bytes[LARGO_MENSAJE];

bool parityError = 0;
volatile BYTE len = 10;

int main()
{
	if (wiringPiSetup() == -1)
		exit(1);
	//  piHiPri(99);
	// CONFIGURA PINES DE ENTRADA SALIDA
	pinMode(RX_PIN, INPUT);

	// CONFIGURA INTERRUPCION PIN CLOCK (PUENTEADO A PIN PWM)
	if (wiringPiISR(DELAY_PIN, INT_EDGE_RISING, &cb) < 0)
	{
		printf("Unable to start interrupt function\n");
	}

	printf("Delay\n");
	while (nbytes < 15)
		delay(300);

	// if (proto.CMD==2){
	//  return 0;}

	/*
	if(proto.CMD)==4){
	  MensajesRecibidos();
	}
	*/

	for (int i = 0; i < nbytes; i++)
	{
		printf("Byte %d: %d\n", i, bytes[i]);
	}
	printf("Errores: %d\n", errors);
	// printf("Nbits: %d. Errors: %d\n", nbits, errors);

	imprimirBytes(bytes, nbytes);

	return 0;
}

void cb(void)
{
	bool level = digitalRead(RX_PIN); // Lee el nivel del pin RX
	//  printf("%d",level);
	if (transmissionStarted)
	{
		processBit(level);
	}
	else if (level == 0 && !transmissionStarted)
	{
		transmissionStarted = true;
		nbits = 1;
	}
}

void processBit(bool level)
{
	if (nbits < 9)
	{
		bytes[nbytes] |= level << (nbits - 1);
	}
	else if (nbits == 9)
	{
		//    printf("\n");
		parity = level;
		nones = (bytes[nbytes] & 0x01) + ((bytes[nbytes] & 0x02) >> 1) + ((bytes[nbytes] & 0x04) >> 2) + ((bytes[nbytes] & 0x08) >> 3) + ((bytes[nbytes] & 0x10) >> 4) + ((bytes[nbytes] & 0x20) >> 5) + ((bytes[nbytes] & 0x40) >> 6) + ((bytes[nbytes] & 0x80) >> 7);
		if (parity != (nones % 2 == 0))
		{
			parityError = true;
		}
		nbytes++;
		transmissionStarted = false;
	}
	nbits++;
}

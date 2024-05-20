// INCLUSIONES
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"

// MACROS
#define DELAY_PIN 23
#define RX_PIN 3

#define BYTE unsigned char

// PROTOTIPOS
// Procesa un bit recibido
void processBit(bool signal);

// Inicializa la transmisión, reestableciendo las variables globales.
void inicializarTransmision();

// FUNCIÓN DE CALLBACK
// Se encarga de leer el nivel del pin RX y procesar el bit
void cb(void);

// VARIABLES GLOBALES
// Guarda la cantidad de errores
volatile int errors = 0;
// Guarda la cantidad de bits recibidos
volatile int numero_de_bits = 0;
// Guarda la cantidad de bytes recibidos
volatile int numero_de_bytes = 0;
// Indica si la transmisión está activa
bool transmissionStarted = false;
// Guarda el valor de la paridad
bool paridad = 0;
// Guarda la cantidad de unos en un byte
int cantidad_de_unos = 0;
// Guarda si hay un error de paridad
bool paridadError = 0;
// Guarda el largo del mensaje (despreciado)
volatile BYTE len = 0;
Protocolo mensaje;
bool estado;

// Indica si la transmisión ha finalizado
volatile bool fin_transmision = false;

// Guarda la cantidad de bits en reposo
volatile int contador_de_reposo = 0;

int main()
{
	system("clear");
	printf("Iniciando...\n");
	if (wiringPiSetup() == -1)
		exit(1);
	//  piHiPri(99);
	// CONFIGURA PINES DE ENTRADA SALIDA
	printf("Configurando pines...\n");
	pinMode(RX_PIN, INPUT);

	// CONFIGURA INTERRUPCION PIN CLOCK (PUENTEADO A PIN PWM)
	printf("Configurando interrupción...\n");
	if (wiringPiISR(DELAY_PIN, INT_EDGE_RISING, &cb) < 0)
	{
		printf("Imposible iniciar la interrupción\n");
	}

	while(1)
	{
		
		inicializarTransmision();
		printf("Escuchando...\n");

		while ((numero_de_bytes < len + BYTES_EXTRAS)&& !fin_transmision)
		{
			delay(100);
		}

		estado = desempaquetar(mensaje);

		switch (mensaje.CMD)
		{
		case 1:
		{
			leerMensaje(mensaje, estado);
			imprimirCampos(mensaje);
			imprimirBytes(mensaje.Frames, mensaje.LNG + BYTES_EXTRAS);
			break;
		}
		case 2:
		{
			// a
			break;
		}
		case 3:
		{
			EncontrarArchivo(mensaje);
			break;
		}
		case 4:
		{
			MensajesRecibidos();
			break;
		}
		case 5:
		{
			printf("Apagando...\n");
			system("read -s -n 1 -p \"Press any key to continue . . .\"");
 			system("echo \"\"");
			return 0;
		}
		default:
		{
			printf("Ha ocurrido un error, intente nuevamente\n");
		}
		}

	}
}

void cb(void)
{

	bool signal = digitalRead(RX_PIN); // Lee el nivel del pin RX
	//  printf("%d",signal);
	if (transmissionStarted)
	{
		processBit(signal);		  // Procesa el bit
		if(numero_de_bytes == 2)
		{
			len = mensaje.Frames[1] & 0x3F;
		}
	}
	else if (signal == 0 && !transmissionStarted) // Detección del bit de comienzo
	{
		transmissionStarted = true; // Se inicia la transmisión
		numero_de_bits = 1;			// Se inicializa el contador de bits
	}
	else
	{
		contador_de_reposo++;
		if (contador_de_reposo > 8 && numero_de_bytes > 4)
		{
			printf("Reposo de final de transmisión...\n");
			fin_transmision = true;
			transmissionStarted = false;
			contador_de_reposo = 0;
		}
	}
}

void processBit(bool signal)
{
	if (numero_de_bits < 9)
	{
		mensaje.Frames[numero_de_bytes] |= signal << (numero_de_bits - 1);
	}
	else if (numero_de_bits == 9)
	{
		//    printf("\n");
		paridad = signal;
		for (size_t i = 0; i < 8; i++)
		{
			cantidad_de_unos += (mensaje.Frames[numero_de_bytes] >> i) & 0x01;
		}
		if (paridad != (cantidad_de_unos % 2 == 0))
		{
			paridadError = true;
		}
		numero_de_bytes++;
		transmissionStarted = false;
		imprimirBits(mensaje.Frames[numero_de_bytes - 1]);
	}
	numero_de_bits++;
}

void inicializarTransmision()
{
	fin_transmision = false;
	numero_de_bytes = 0;
	len = 0;
	limpiarMensaje(mensaje);
	transmissionStarted = false;
	paridad = 0;
	cantidad_de_unos = 0;
	paridadError = 0;
}
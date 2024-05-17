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
// Procesa un bit recibido
void processBit(bool signal);

// Obtiene el largo del mensaje
BYTE obtenerLNG(BYTE *_Frames);

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
// Guarda los bytes recibidos (despreciado)
BYTE bytes[LARGO_MENSAJE];

bool paridadError = 0;
// Guarda el largo del mensaje (despreciado)
volatile BYTE len = 10;
Protocolo mensaje;
bool estado;

volatile bool fin_transmision = false;

volatile int contador_de_reposo = 0;

FILE * mensajes=fopen("mensajes.txt","a");
FILE * errores=fopen("errores.txt","a");
fclose(mensajes);
fclose(errores);

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

	
	printf("Escuchando...\n");
	mensaje.LNG = 10;
	while (numero_de_bytes < mensaje.LNG + 4)
		delay(500);

	if(fin_transmision){
		//leer lng
	estado = desempaquetar(mensaje);}
	switch (mensaje.CMD)
	{
	case 1:
	{
		leerMensaje(mensaje, estado);
		imprimirCampos(mensaje);
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
		return 0;
	}
	default:
	{
		printf("Ha ocurrido un error, intente nuevamente\n");
	}
	}

	// if (mensaje.CMD==2){
	//  return 0;}

	/*
	if(mensaje.CMD)==4){
	  MensajesRecibidos();
	}
	*/

	for (int i = 0; i < numero_de_bytes; i++)
	{
		printf("Byte %d: %d\n", i, mensaje.Frames[i]);
	}
	printf("Errores: %d\n", errors);
	// printf("Nbits: %d. Errors: %d\n", numero_de_bits, errors);

	imprimirBytes(mensaje.Frames, numero_de_bytes);

	return 0;
}

void cb(void)
{

	bool signal = digitalRead(RX_PIN); // Lee el nivel del pin RX
	//  printf("%d",signal);
	if (transmissionStarted)
	{
		processBit(signal);		  // Procesa el bit
		mensaje.LNG = obtenerLNG(mensaje.Frames); // Obtiene el largo del mensaje
	}
	else if (signal == 0 && !transmissionStarted) // Detección del bit de comienzo
	{
		transmissionStarted = true; // Se inicia la transmisión
		numero_de_bits = 1;			// Se inicializa el contador de bits
	}
	else
	{
		contador_de_reposo++;
		if (contador_de_reposo > 4)
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
	}
	numero_de_bits++;
}

BYTE obtenerLNG(BYTE *_Frames)
{
	BYTE _aux;
	if (numero_de_bytes == 2)
	{
		_aux = ((_Frames[numero_de_bytes - 2]) >> 7) & 0x01 | (_Frames[numero_de_bytes - 1]) & 0x1F;
		printf("El largo del mensaje es: %d\n", _aux);
		imprimirBits(_aux);
		return _aux;
	}
}
#include <stdio.h>
#include "funciones.h"
#include <stdlib.h>
#include <cstring>

#define RASPBERRY_PI
#ifdef RASPBERRY_PI
#include <wiringPi.h>
#endif
#ifndef RASPBERRY_PI
#define wiringPiSetupGpio()
#define pinMode(a, b)
#define digitalWrite(a, b)
#define delayMicroseconds(a)
#define delay(a)
#define HIGH 1
#define LOW 0
#endif

// MACROS
#define DELAY_PIN 0
#define TX_PIN 2

#define BYTE unsigned char // definición de tipo de dato BYTE

#define CANTIDAD_DE_BYTES 63

#define BYTES_EXTRAS 4

// DECLARACION DE PROTOTIPOS
void cb(void);							 // función de interrupción
void iniciarTransmision();				 // función para iniciar la transmisión
void limpiarMensaje(Protocolo &mensaje); // función para limpiar el mensaje
void detectarEmisor(int pin, bool &valor_aux);

// VARIABLES GLOBALES
volatile int numero_bits_transmitidos = 0;	// guarda el número de bits transmitidos
volatile int numero_bytes_transmitidos = 0; // guarda el número de bytes transmitidos

Protocolo mensaje; // Se crea un mensaje de prueba

BYTE buffer_de_datos[CANTIDAD_DE_BYTES + BYTES_EXTRAS]; // buffer de datos

int numero_de_unos = 0;			  // es el número de unos en el byte
bool transmisionIniciada = false; // indica si la transmisión está activa

int main()
{
	if (wiringPiSetup() == -1)
	{
		printf("Error");
		return 0;
	}

	// CONFIGURA INTERRUPCION PIN CLOCK (PUENTEADO A PIN PWM)
	
	
	if (wiringPiISR(DELAY_PIN, INT_EDGE_RISING, &cb) < 0)
	{
		printf("Unable to start interrupt function\n"); // si hay error al configurar la interrupción, imprime mensaje
	}
	

	// CONFIGURA PINES DE ENTRADA SALIDA
	pinMode(TX_PIN, OUTPUT);

	//int Mensajes_Enviados = ContarMensajes("mensajes");
	//printf("---------------------------------\n");
	//printf("Mensajes enviados: %d\n", Mensajes_Enviados);
	system("clear");
	printf("MENU PRINCIPAL\n");
	printf("Seleccione una opcion\n[1]Enviar mensaje de texto\n[2]Enviar mensaje de prueba\n[3]Mostrar contenido\n[4]Contador de mensajes\n[5]Cerrar el receptor\n[6]Salir del programa\n");
	int opcion;
	scanf("%d", &opcion);
	getchar();
	switch (opcion)
	{
	case 1:
	{

		limpiarMensaje(mensaje); // Se limpia el mensaje
		mensaje.CMD = opcion;
		obtenerInformacion(mensaje); // Se solicita al usuario el mensaje a enviar
		empaquetar(mensaje);		 // Se empaqueta el mensaje
		memcpy(buffer_de_datos, mensaje.Frames, mensaje.LNG + BYTES_EXTRAS); // Se copia el mensaje empaquetado al buffer de datos
		imprimirCampos(mensaje);

		iniciarTransmision();		// inicia la transmisión
		//bool valor_pin_aux = 0;
		while(transmisionIniciada)
		{
			delay(100);
		}
		// Enviar mensaje de texto y ser guardado en un archivo mensajes.txt
		break;
	}
	case 2:
	{
		// Enviar mensaje de prueba varias veces y el receptor debe contar los enviados correctamente, con error y con error no detectado, además de calcular los procentajes
		// de acierto y error y mostrarlo por consola del receptor
		printf("¿Cuántas veces desea enviar el mensaje de prueba?\n"); // Se solicita la cantidad de mensajes a enviar
		int cantidad_mensajes;										   // Se guarda la cantidad de mensajes a enviar
		scanf("%d", &cantidad_mensajes);							   // Se lee la cantidad de mensajes a enviar
		int contador_aciertos = 0;									   // Se inicializa el contador de aciertos
		Protocolo mensaje;											   // Se crea un mensaje de prueba
		mensaje.CMD = opcion;
		obtenerInformacion(mensaje);
		printf("Enviando mensaje de prueba %d veces...\n", cantidad_mensajes);
		for (int i = 0; i < cantidad_mensajes; i++) // Se envía el mensaje de prueba la cantidad de veces solicitada
		{
			Protocolo mensaje2;
			memcpy(mensaje2.Frames, mensaje.Frames, sizeof(mensaje.Frames) + 1);
			//contador_aciertos += leerMensaje(mensaje2);
		}
		float porcentaje_aciertos = (contador_aciertos / cantidad_mensajes) * 100;
		break;
	}
	case 3:
		mensaje.CMD = opcion;
		BuscarArchivo(mensaje);
		empaquetar(mensaje);
		break;
	case 4:
		mensaje.CMD = opcion;
		break;
	case 5:
		// Cerrar el programa del receptor
		mensaje.CMD = opcion;
		empaquetar(mensaje);
		iniciarTransmision();		// inicia la transmisión
		//bool valor_pin_aux = 0;
		while(transmisionIniciada)
		{
			delay(500);
		}
		break;
	case 6:
		break;
	default:
		printf("La opcion ingresada no es valida, intente nuevamente\n");
		break;
	}
	return 0;
}

void cb(void)
{
	if (transmisionIniciada)
	{
		// Escribe en el pin TX
		if (numero_bits_transmitidos == 0)
		{							 // si el numero de bits es 0
			digitalWrite(TX_PIN, 0); // Bit de inicio
		}
		else if (numero_bits_transmitidos < (mensaje.LNG + BYTES_EXTRAS) - 1)
		{
			digitalWrite(TX_PIN, (buffer_de_datos[numero_bytes_transmitidos] >> (numero_bits_transmitidos - 1)) & 0x01); // Bit de dato
																														// printf("%d",(buffer_de_datos[numero_bytes_transmitidos]>>(numero_bits_transmitidos-1))&0x01);
		}
		else if (numero_bits_transmitidos == mensaje.LNG + BYTES_EXTRAS - 1)
		{
			//      printf("\n");
			numero_de_unos = (buffer_de_datos[numero_bytes_transmitidos] & 0x01) + ((buffer_de_datos[numero_bytes_transmitidos] & 0x02) >> 1) + ((buffer_de_datos[numero_bytes_transmitidos] & 0x04) >> 2) +
							 ((buffer_de_datos[numero_bytes_transmitidos] & 0x08) >> 3) + ((buffer_de_datos[numero_bytes_transmitidos] & 0x10) >> 4) + ((buffer_de_datos[numero_bytes_transmitidos] & 0x20) >> 5) +
							 ((buffer_de_datos[numero_bytes_transmitidos] & 0x40) >> 6) + ((buffer_de_datos[numero_bytes_transmitidos] & 0x80) >> 7);
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
			if (numero_bytes_transmitidos == mensaje.LNG + BYTES_EXTRAS)
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

void limpiarMensaje(Protocolo &mensaje)
{
	memset(mensaje.DATA, 0, sizeof(mensaje.DATA));
	memset(mensaje.FCS, 0, sizeof(mensaje.FCS));
	memset(mensaje.Frames, 0, sizeof(mensaje.Frames));
	mensaje.CMD = 0;
	mensaje.LNG = 0;
}

void detectarEmisor(int pin, bool &valor_aux)
{

    bool valor_pin = digitalRead(pin);

    if(valor_pin && (valor_aux != valor_pin)){
        cb();
        valor_aux = valor_pin;
    }
}

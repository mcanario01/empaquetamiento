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


// VARIABLES GLOBALES
volatile int numero_bits_transmitidos = 0;	// guarda el número de bits transmitidos
volatile int numero_bytes_transmitidos = 0; // guarda el número de bytes transmitidos

Protocolo mensaje; // Se crea un mensaje de prueba

BYTE buffer_de_datos[CANTIDAD_DE_BYTES + BYTES_EXTRAS]; // buffer de datos

volatile int numero_de_unos = 0;			  // es el número de unos en el byte
bool transmisionIniciada = false; // indica si la transmisión está activa

volatile int cantidad_de_mensajes_enviados = 0; // cantidad de mensajes enviados

int main()
{
	if (wiringPiSetup() == -1)
	{
		printf("Imposible configurar la función wiringPiSetup\n"); // si hay error al configurar la función wiringPiSetup, imprime mensaje
		printf("Codigo de error: 1");
		return 1;
	}

	// CONFIGURA INTERRUPCION PIN CLOCK (PUENTEADO A PIN PWM)
	if (wiringPiISR(DELAY_PIN, INT_EDGE_RISING, &cb) < 0)
	{
		printf("Imposible iniciar la funcion de interrupción\n"); // si hay error al configurar la interrupción, imprime mensaje
		printf("Codigo de error: 2");
		return 2;
	}
	
	// CONFIGURA PINES DE ENTRADA SALIDA
	pinMode(TX_PIN, OUTPUT);

	system("clear"); // limpia la consola
	while(1)
	{
		printf("MENU PRINCIPAL\n\n");
		/*
		El emisor debe poder mostrar un contador de mensajes enviados.
		*/
		printf("Cantidad de mensajes enviados: %d\n\n", cantidad_de_mensajes_enviados); // imprime la cantidad de mensajes enviados
		printf("Seleccione una opcion\n[1]Guardar frase en archivo en receptor.\n[2]Enviar 10 mensajes de prueba.\n[3]Imprimir archivo en receptor.\n[4]Cambiar archivo de destino.\n[5]Imprimir resumen de mensajes en receptor.\n[6]Listar todos los .txt en el receptor.\n[7]Cerrar el receptor.\n[8]Salir del programa.\n");
		printf("Opcion: ");
		int opcion;
		scanf("%d", &opcion);
		getchar();
		limpiarMensaje(mensaje); // Se limpia el mensaje
		limpiarBuffer(buffer_de_datos); // Se limpia el buffer de datos
		switch (opcion)
			{
			case 1:
			{
				// Mediante un comando se debe enviar un mensaje de texto y ser guardado en un archivo mensajes.txt
				mensaje.CMD = opcion;
				obtenerInformacion(mensaje); // Se solicita al usuario el mensaje a enviar
				empaquetar(mensaje);		 // Se empaqueta el mensaje
				imprimirBytes(mensaje.Frames, mensaje.LNG + BYTES_EXTRAS); // Se imprime el mensaje empaquetado (para verificar que se empaquetó correctamente
				memcpy(buffer_de_datos, mensaje.Frames, mensaje.LNG + BYTES_EXTRAS); // Se copia el mensaje empaquetado al buffer de datos

				printf("Iniciando transmisión...\n");
				printf("Enviando mensaje de texto para guardar en archivo...\n");
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
				/*
				Mediante un comando, enviar un mensaje de prueba. Este mensaje debe enviarse 10 veces, y
				el receptor ir contando internamente:
					- Los mensajes recibidos correctamente.
					- Los mensajes recibidos con error detectado.
					- Los mensajes recibidos con error no detectado.
				Mediante este comando el receptor debe calcular los porcentajes de acierto y error
				asociados a los mensajes enviados, y mostrar éstos por la consola del receptor.
				*/
				mensaje.CMD = opcion;
				mensaje.LNG = 17; // Se establece el largo del mensaje
				memcpy(mensaje.DATA, "mensaje de prueba", 17); // Se copia el mensaje de prueba al campo DATA del mensaje
				empaquetar(mensaje);
				memcpy(buffer_de_datos, mensaje.Frames, mensaje.LNG + BYTES_EXTRAS); // Se copia el mensaje empaquetado al buffer de datos

				printf("Iniciando transmisión...\n");
				printf("Enviando mensaje de prueba 10 veces...\n");
				for (int i = 0; i < 10; i++) // Se envía el mensaje de prueba la cantidad de veces solicitada
				{
					iniciarTransmision();		// inicia la transmisión
					while(transmisionIniciada)
					{
						delay(100);
					}
					cantidad_de_mensajes_enviados--;
				}
				break;
			}

			case 3:
			{
				/*
				Mediante un comando se debe enviar un mensaje con el nombre de un archivo de texto de
				prueba en el receptor, con dicho comando, el receptor debe mostrar el contenido del archivo
				consola en caso de que este exista, en caso contrario el receptor debe mostrar por consola
				que el archivo no existe.
				*/
				mensaje.CMD = opcion;
				obtenerInformacion(mensaje); // Se solicita al usuario el mensaje a enviar
				empaquetar(mensaje);		 // Se empaqueta el mensaje
				memcpy(buffer_de_datos, mensaje.Frames, mensaje.LNG + BYTES_EXTRAS); // Se copia el mensaje empaquetado al buffer de datos

				printf("Iniciando transmisión...\n");
				printf("Enviando nombre de archivo al receptor...\n");
				iniciarTransmision();		// inicia la transmisión
				//bool valor_pin_aux = 0;
				while(transmisionIniciada)
				{
					delay(100);
				}
				// Enviar mensaje de texto y ser guardado en un archivo mensajes.txt
				break;
			}
			case 4:
			{
				/*
				Mediante un comando crear un archivo con un nombre ingresado por teclado, y registrar el
				siguiente mensaje recibido en ese archivo (puede ser mensaje normal o de prueba)..
				*/
				mensaje.CMD = opcion;
				obtenerInformacion(mensaje); // Se solicita al usuario el mensaje a enviar
				empaquetar(mensaje);		 // Se empaqueta el mensaje
				memcpy(buffer_de_datos, mensaje.Frames, mensaje.LNG + BYTES_EXTRAS); // Se copia el mensaje empaquetado al buffer de datos

				printf("Iniciando transmisión...\n");
				printf("Enviando nombre de nuevo archivo de destino...\n");
				iniciarTransmision();		// inicia la transmisión
				//bool valor_pin_aux = 0;
				while(transmisionIniciada)
				{
					delay(100);
				}
				// Enviar mensaje de texto y ser guardado en un archivo mensajes.txt
				break;
			}
			case 5:
			{
				// Mediante un comando el receptor debe imprimir por pantalla el contador de los mensajes
				// recibidos junto con las estadísticas de los mensajes recibidos correctamente y con error (sin
				// contar los de prueba).
				mensaje.CMD = opcion;
				empaquetar(mensaje);		 // Se empaqueta el mensaje
				memcpy(buffer_de_datos, mensaje.Frames, mensaje.LNG + BYTES_EXTRAS); // Se copia el mensaje empaquetado al buffer de datos

				printf("Iniciando transmisión...\n");
				printf("Enviando instrucción para imprimir estadísticas globales...\n");
				iniciarTransmision();		// inicia la transmisión
				//bool valor_pin_aux = 0;
				while(transmisionIniciada)
				{
					delay(100);
				}
				// Enviar mensaje de texto y ser guardado en un archivo mensajes.txt
				break;
			}
			case 6:
			{
				/*
				Mediante un comando listar todos los archivos de texto en la carpeta del receptor.
				*/
				mensaje.CMD = opcion;
				empaquetar(mensaje);
				memcpy(buffer_de_datos, mensaje.Frames, mensaje.LNG + BYTES_EXTRAS); // Se copia el mensaje empaquetado al buffer de datos
				printf("Iniciando transmisión...\n");
				printf("Enviando instrucción para listar todos los .txt del receptor...\n");
				iniciarTransmision();		// inicia la transmisión
				while(transmisionIniciada)
				{
					delay(100);
				}
				break;
			}
			case 7:
			{
				// Cerrar el programa del receptor
				mensaje.CMD = opcion;
				empaquetar(mensaje);
				memcpy(buffer_de_datos, mensaje.Frames, mensaje.LNG + BYTES_EXTRAS); // Se copia el mensaje empaquetado al buffer de datos
				printf("Iniciando transmisión...\n");
				printf("Enviando comando para cerrar el receptor...\n");
				iniciarTransmision();		// inicia la transmisión
				while(transmisionIniciada)
				{
					delay(100);
				}
				break;
			}
			case 8:
			{
				// Salir del programa
				printf("Saliendo del programa...\n");
				return 0;
			}
			default:
			{
				printf("El comando recibido no es válido, intenta nuevamente.\n");
				break;
			}
		}
	}
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
		else if (numero_bits_transmitidos < 9)
		{
			digitalWrite(TX_PIN, (buffer_de_datos[numero_bytes_transmitidos] >> (numero_bits_transmitidos - 1)) & 0x01); // Bit de dato
																														// printf("%d",(buffer_de_datos[numero_bytes_transmitidos]>>(numero_bits_transmitidos-1))&0x01);
		}
		else if (numero_bits_transmitidos == 9)
		{
			//      printf("\n");
			numero_de_unos = calcularNumeroDeUnos(buffer_de_datos[numero_bytes_transmitidos]);
			digitalWrite(TX_PIN, numero_de_unos % 2 == 0); // Bit de paridad
		}
		else
		{
			digitalWrite(TX_PIN, 1); // Canal libre durante 2 clocks
		}
		// Actualiza contador de bits
		numero_bits_transmitidos++;

		// Actualiza contador de bytes
		if (numero_bits_transmitidos == 11) // si el numero de bits es 11
		{
			numero_bits_transmitidos = 0;
			numero_bytes_transmitidos++;

			// Finaliza la comunicación
			if (numero_bytes_transmitidos == mensaje.LNG + BYTES_EXTRAS)
			{
				transmisionIniciada = false;
				numero_bytes_transmitidos = 0;
				numero_bits_transmitidos = 0;
				cantidad_de_mensajes_enviados++;
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
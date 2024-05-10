#include <stdio.h>
#include "funciones.h"
#include <stdlib.h>
#include <string.h>
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

#define SPEED 100000

int main()
{

	if (wiringPiSetupGpio() == -1)
	{
		printf("Error");
		return 0;
	}
	pinMode(17, OUTPUT); // pin receptor */
	printf("MENU PRINCIPAL\n");
	printf("Seleccione una opcion\n[1]Enviar mensaje de texto\n[2]Enviar mensaje de prueba\n[3]Mostrar contenido\n[4]Contador de mensajes\n[5]Cerrar el emisor\n[6]Salir del programa\n");
	int opcion;
	scanf("%d", &opcion);
	getchar();
	switch (opcion)
	{
	case 1:
	{
		Protocolo mensaje; // Se crea un mensaje de prueba
		obtenerInformacion(mensaje); // Se solicita al usuario el mensaje a enviar
		Protocolo mensaje2; // Se crea un mensaje de prueba
		memcpy(mensaje2.Frames, mensaje.Frames, sizeof(mensaje.Frames) + 1); // Se "envia" el mensaje
		leerMensaje(mensaje2);
		// Enviar mensaje de texto y ser guardado en un archivo mensajes.txt
		break;
	}
	case 2:
	{
		// Enviar mensaje de prueba varias veces y el receptor debe contar los enviados correctamente, con error y con error no detectado, además de calcular los procentajes
		// de acierto y error y mostrarlo por consola del receptor
		printf("¿Cuántas veces desea enviar el mensaje de prueba?\n"); // Se solicita la cantidad de mensajes a enviar
		int cantidad_mensajes; // Se guarda la cantidad de mensajes a enviar
		scanf("%d", &cantidad_mensajes); // Se lee la cantidad de mensajes a enviar
		int contador_aciertos = 0; // Se inicializa el contador de aciertos
		Protocolo mensaje; // Se crea un mensaje de prueba
		obtenerInformacion(mensaje);
		printf("Enviando mensaje de prueba %d veces...\n", cantidad_mensajes);
		for (int i = 0; i < cantidad_mensajes; i++) // Se envía el mensaje de prueba la cantidad de veces solicitada
		{
			Protocolo mensaje2;
			memcpy(mensaje2.Frames, mensaje.Frames, sizeof(mensaje.Frames) + 1);
			contador_aciertos += leerMensaje(mensaje2);
		}
		float porcentaje_aciertos = (contador_aciertos / cantidad_mensajes) * 100;
		break;
	}
	case 3:
		// Se debe enviar un mensaje con el nombre de un archivo de texto de prueba en el receptor, con dicho comando, el receptor debe mostrar el contenido del archivo
		// consola en caso de que este exista, en caso contrario el receptor debe mostrar por consola que el archivo no existe.
		break;
	case 4:
		// El receptor debe imprimir los mensajes recibidos correctamente y con error (no cuentan los de prueba)
		break;
	case 5:
		// Cerrar el programa del receptor
		break;
	case 6:
		break;
	default:
		printf("La opcion ingresada no es valida, intente nuevamente\n");
		break;
	}
	return 0;
}

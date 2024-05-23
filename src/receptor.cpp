// INCLUSIONES
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "funciones.h"

// MACROS
#define DELAY_PIN 23
#define RX_PIN 3

#define BYTE unsigned char

// PROTOTIPOS
// Procesa un bit recibido
void processBit(bool signal);

// Inicializa la transmisión, reestableciendo las variables globales.
void restablecerDatos();

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
bool envio_de_informacion = false;
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
volatile bool transmision_iniciada = false;

// Guarda la cantidad de bits en reposo
volatile int contador_bits_reposo = 0;

int contador_de_mensajes_de_prueba = 0;
int contador_de_mensajes_de_prueba_correctos = 0;

int contador_de_mensajes_recibidos = 0;
int contador_de_mensajes_recibidos_correctos = 0;

char nombre_archivo[LARGO_DATA] = "mensajes";

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
		printf("Restableciendo datos...\n");
		restablecerDatos();

		// Espera a que se reciba un mensaje completo
		printf("Esperando mensaje...\n");
		while ((numero_de_bytes < len + BYTES_EXTRAS))
		{
			delay(100);
		}
		printf("Mensaje recibido...\n");
		printf("Procesando mensaje...\n");
		// Procesa el mensaje
		estado = desempaquetar(mensaje);
		if(estado)
		{
			contador_de_mensajes_recibidos++; // Se incrementa el contador de mensajes recibidos
			contador_de_mensajes_recibidos_correctos++; // Se incrementa el contador de mensajes recibidos correctamente
		}
		else
		{
			contador_de_mensajes_recibidos++; // Se incrementa el contador de mensajes recibidos
			/*
			Extra: Registrar en otro archivo los mensajes recibidos con error.
			*/
			EscribirArchivo("errores", mensaje); // Guarda el mensaje con error en un archivo
		}

		switch (mensaje.CMD)
		{
		case 1:
		{
			// Mediante un comando se debe enviar un mensaje de texto y ser guardado en un archivo mensajes.txt
			leerMensaje(mensaje, estado);
			printf("Escribiendo mensaje en archivo %s.txt ...\n", nombre_archivo);
			EscribirArchivo(nombre_archivo, mensaje);
			break;
		}
		case 2:
		{
			// Recibe 10 mensajes de prueba y calcula el porcentaje de error
			printf("Mensaje de prueba recibido\n");
			leerMensaje(mensaje, estado);
			contador_de_mensajes_de_prueba++;
			contador_de_mensajes_recibidos--;
			contador_de_mensajes_de_prueba_correctos += estado;
			contador_de_mensajes_recibidos_correctos -= estado;
			if(contador_de_mensajes_de_prueba == 10)
			{
				if(contador_de_mensajes_de_prueba == contador_de_mensajes_de_prueba_correctos)
				{
					printf("Los diez mensajes de prueba han sido recibidos correctamente\n");
					printf("Porcentaje de error: %g \%\n", (float)(contador_de_mensajes_de_prueba_correctos - contador_de_mensajes_de_prueba) / contador_de_mensajes_de_prueba * 100);
					printf("Se recibieron correctamente %d mensajes de prueba\n", contador_de_mensajes_de_prueba_correctos);
					printf("Se recibieron incorrectamente %d mensajes de prueba\n", contador_de_mensajes_de_prueba - contador_de_mensajes_de_prueba_correctos);
				}
				else
				{
					printf("Ha ocurrido un error en la recepción de mensajes de prueba\n");
					printf("Porcentaje de error: %g \%\n", (float)(contador_de_mensajes_de_prueba_correctos - contador_de_mensajes_de_prueba) / contador_de_mensajes_de_prueba * 100);
					printf("Se recibieron correctamente %d mensajes de prueba\n", contador_de_mensajes_de_prueba_correctos);
					printf("Se recibieron incorrectamente %d mensajes de prueba\n", contador_de_mensajes_de_prueba - contador_de_mensajes_de_prueba_correctos);
				}
			}
			break;
		}
		case 3:
		{
			// Mediante un comando se debe enviar un mensaje con el nombre de un archivo de texto de
			// prueba en el receptor, con dicho comando, el receptor debe mostrar el contenido del archivo
			// consola en caso de que este exista, en caso contrario el receptor debe mostrar por consola
			// que el archivo no existe.
			leerMensaje(mensaje, estado);
			printf("Buscando archivo...\n");
			EncontrarArchivo(mensaje);
			break;
		}
		case 4:
		{
			/*
			Mediante un comando crear un archivo con un nombre ingresado por teclado, y registrar el
			siguiente mensaje recibido en ese archivo (puede ser mensaje normal o de prueba)..
			*/
			leerMensaje(mensaje, estado);
			printf("Cambiando archivo destino a: %s.txt ...\n", mensaje.DATA);
			for(size_t i = 0; i < sizeof(nombre_archivo); i++)
			{
				nombre_archivo[i] = 0;
			}
			memcpy(nombre_archivo, mensaje.DATA, mensaje.LNG);
			break;
		}
		case 5:
		{
			// Mediante un comando el receptor debe imprimir por pantalla el contador de los mensajes
			// recibidos junto con las estadísticas de los mensajes recibidos correctamente y con error (sin
			// contar los de prueba).
			printf("Resumen de mensajes:\n");
			printf("Contador de mensajes recibidos: %d\n", contador_de_mensajes_recibidos);
			printf("Contador de mensajes recibidos correctamente: %d\n", contador_de_mensajes_recibidos_correctos);
			printf("Contador de mensajes recibidos incorrectamente: %d\n", contador_de_mensajes_recibidos - contador_de_mensajes_recibidos_correctos);
			break;
		}
		case 6:
		{
			// Lista todos los archivos .txt en la carpeta del receptor.
			printf("Listando archivos de texto...\n");
			listarArchivosDeTexto();
			break;
		}
		case 7:
		{
			printf("Apagando...\n");
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
	if (envio_de_informacion)
	{
		processBit(signal);		  // Procesa el bit
		if(numero_de_bytes == 2)
		{
			len = mensaje.Frames[1] & 0x3F;
		}
	}
	else if(signal)
	{
		contador_bits_reposo++;
	}
	else // Detección del bit de comienzo
	{
		envio_de_informacion = true; // Se inicia la transmisión
		numero_de_bits = 1;			// Se inicializa el contador de bits
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
		envio_de_informacion = false;
	}
	numero_de_bits++;
}

void restablecerDatos()
{
	errors = 0;
	numero_de_bits = 0;
	numero_de_bytes = 0;
	cantidad_de_unos = 0;
	transmision_iniciada = false;
	len = 0;
	limpiarMensaje(mensaje);
	envio_de_informacion = false;
	paridad = 0;
	paridadError = 0;
	contador_bits_reposo = 0;
}
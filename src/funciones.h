#ifndef FUNCIONES_H
#define FUNCIONES_H

#include "protocolo.h"

// Guarda todos los campos CMD y DATA en el arreglo de bytes Frames
// Calcula el largo del mensaje y lo guarda en Frames.
// Calcula el chksum y lo guarda en el arreglo de bytes FCS.
int empaquetar(Protocolo & proto);

// Retorna verdadero si la paridad es correcta, falso si la paridad no es correcta.
bool desempaquetar(Protocolo &proto);

// retorna la cadena de chksum.
unsigned int fcs(BYTE *arr, int size);

// Guarda CMD y LNG en la estructura proto.
void obtenerInformacion(Protocolo &proto);

// recibe un mensaje e imprime por pantalla sus datos.
bool leerMensaje(Protocolo proto,bool estado);

// Imprime por pantalla los bits de un byte.
void imprimirBits(BYTE byte);

// Imprime por pantalla los bytes de un arreglo.
void imprimirBytes(BYTE *arr, int size);

// Imprime por pantalla los campos de un mensaje.
void imprimirCampos(Protocolo proto);

// Inicia la transmisión de un mensaje.
void iniciarTransmision();

//Lee el archivo entregado y devuelve el numero de mensajes enviados
int ContarMensajes(const char *arch); 

// Busca un archivo en el directorio actual y lo envía al receptor.
void BuscarArchivo(Protocolo &proto);

// Envia un archivo al receptor.
void EncontrarArchivo(Protocolo mensaje);

// Lee el archivo entregado y devuelve el numero de mensajes enviados
void MensajesRecibidos();

EscribirArchivo(const char* arch,Protocolo proto);

//detecta el pin del clock y ejecuta la función de callback
void detectarEmisor(int pin);

#endif

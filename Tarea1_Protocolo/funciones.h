#ifndef FUNCIONES_H
#define FUNCIONES_H

#include "protocolo.h"

//retorna LNG + 2.
int empaquetar(Protocolo & proto);

//Retorna verdadero si la paridad es correcta, falso si la paridad no es correcta. 
bool desempaquetar(Protocolo & proto, int size);

//retorna la cadena de chksum.
int fcs(BYTE * arr, int size);

//cierra el programa.
void Cerrar(bool comando);

//envia un mansaje
void EnviarMensaje();

//recibe un mensaje e imprime por pantalla sus datos.
void recibirMensaje(Protocolo proto);

#endif
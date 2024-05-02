#ifndef FUNCIONES_H
#define FUNCIONES_H

#include "protocolo.h"

int empaquetar(Protocolo & proto);
bool desempaquetar(Protocolo & proto, int size);
int fcs(BYTE * arr, int size);
void Cerrar(bool comando); //cierra el programa
void EnviarMensaje();

#endif
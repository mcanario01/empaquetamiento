#include <stdio.h>
#include "funciones.h"
#include <string.h>

Protocolo protocolo;

int main()
{
    BYTE CMD = 1;
    BYTE LNG = 5;
    BYTE DATA[] = "hola";

    protocolo.CMD = 1;
    protocolo.LNG = 5;

    memcpy(protocolo.DATA, DATA sizeof(DATA));
    return 0;
}
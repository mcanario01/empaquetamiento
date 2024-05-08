#include <stdio.h>
//#include <wiringPi.h>
#include "funciones.h"
#include <stdlib.h>

#define SPEED 100000

int main()
{
    Protocolo proto;
   /* if (wiringPiSetupGpio () == -1)
    {
        printf("Error");
        return 0;
    } */
    EnviarMensaje(proto);
    recibirMensaje(proto);
}
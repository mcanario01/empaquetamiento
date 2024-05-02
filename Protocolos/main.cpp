#include <stdio.h>
#include "funciones.h"
#include <iostream>
#include <string.h>

Protocolo proto_rx;
Protocolo proto_tx;
int main(){
    // Emisor
    std::cout <<"Ingrese un comando :"<< std::endl;// C++
    std::cin >>proto_tx.CMD;
    printf("Ingrese un mensaje : \n");
    scanf("%s",proto_tx.DATA);
    proto_tx.LNG = strlen((const char *)proto_tx.DATA);
    empaquetar(proto_tx);
    // Simulación de transmisión
    memcpy(proto_rx.trama,proto_tx.trama,proto_tx.LNG +2); // Se copia el arreglo de bytes del transmisor al receptor
    // Receptor
    bool estado = desempaquetar(proto_rx,proto_tx.LNG+2);
    printf("Se recibió un mensaje con estado %s\n",estado? "Correcto":"Incorrecto");
    printf("El comando es : %d \n", proto_rx.CMD);
    printf("El largo del mensaje es %d \n",proto_rx.LNG);
    printf("El mensaje es : %s \n", proto_rx.DATA);
    return  0;
}
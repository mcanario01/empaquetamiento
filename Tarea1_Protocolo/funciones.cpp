#include <stdlib.h>
#include <stdio.h>
#include "funciones.h"


int empaquetar(Protocolo & proto)
{
    //Protocolo
    // CMD 7 bits / LNG 6 bits / DATA 63 bytes / FCS 10 bits

    proto.Frames[0] = proto.CMD & 0x7F | proto.LNG & 0x01 << 7; // guardar CMD(7) y LNG(1), no sobra nada
    proto.Frames[1] = (proto.LNG >> 1) & 0x1F; // guarda los siguientes 5 bits, faltan 3
    if(proto.LNG > 0) // si quedan datos por enviar, se encapsulan
    {                       // 4 bits
        proto.Frames[1] |= (proto.DATA[0] & 0x07) << 5; // llena los 3 espacios que le queda a frames[1], no falta nada que llenar
        for(size_t i = 0; i < proto.LNG; i++)
        {                   // MSB del dato anterior + LSB del dato nuevo.
            proto.Frames[i + 2] = proto.DATA[i] >> 3 | (proto.DATA[i] & 0x07) << 5;
        }
        proto.Frames[proto.LNG] = proto.DATA[proto.LNG - 1] >> 3; // guarda el último dato de data en frames
    }
    proto.FCS = fcs(proto.Frames, proto.LNG + 1);

    proto.Frames[proto.LNG] |= (proto.FCS & 0x07) << 5; //calcula y guarda fsc de Frames en frames
    proto.Frames[proto.LNG + 1] = proto.FCS & 0x1F;
    return proto.LNG + 2;
}

int fcs(BYTE * arr, int size)
{
    int suma_bits = 0;
    for(int i = 0; i < size; i++)
    {
        int bit_v;
        for(int j = 0; j < 8; j++)
        {
            bit_v = (arr[i] >> j) & 0x01;
            suma_bits += bit_v;
        }
    }
    return suma_bits;
}

bool desempaquetar(Protocolo & proto, int size)
{
    proto.CMD = proto.Frames[0] & 0x7F;
    proto.LNG = (proto.Frames[0] >> 7) & 0x01 | proto.Frames[1] & 0x1F;
    if(size != (proto.LNG + 2)) // detección de tamaños incorrectos
    {
        return false;
    }
    
    if (proto.LNG > 0 && (proto.LNG <= LARGO_DATA))
    {                       // 4 bits
        for (int i = 1; i < proto.LNG; i++)
        {                       
            proto.DATA[i] = (proto.Frames[i] >> 3 | (proto.Frames[i+1] & 0x07 << 5));
        }                           // 5 bits guardados en parte menos significativa
    }
    int _fcs = fcs(proto.Frames, proto.LNG);
    BYTE arr_aux[1] = {proto.Frames[proto.LNG] & 0x0F};
    _fcs += fcs(arr_aux, 1);

    proto.FCS = proto.Frames[proto.LNG] >> 5 | ( (proto.Frames[proto.LNG+1] & 0x1F) << 3);
    if (_fcs != proto.FCS){
        return false;
    }
    return true;
}

void Cerrar(int comando){
    //limpiar memoria de receptor.cpp
    if (comando=5)
    exit(0);
}

void EnviarMensaje(Protocolo proto){
    printf("Ingrese su mensaje\n");
    scanf("%62s",proto.DATA);
    digitalWrite(17,mensaje);
    proto.LNG=strlen((const char*)proto.DATA);
    empaquetar(proto);
    memcpy(proto.trama,proto.trama,proto.LNG+2);
    bool estado=desempaquetar(proto,proto.LNG+2);
    printf("Se recibio un mensaje con estado %s\n",estado?"Correcto":"Incorrecto");
    printf("El largo del mensaje es $d\n",proto.LNG);
    printf("El mensajes es %s\n",proto.DATA);

}

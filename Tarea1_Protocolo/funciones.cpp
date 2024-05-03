#include <stdlib.h>
#include <stdio.h>
#include "funciones.h"
#include <string.h>


int empaquetar(Protocolo & proto)
{
    //Protocolo
    // CMD 7 bits / LNG 6 bits / DATA 63 bytes / FCS 10 bits

    proto.Frames[0] = proto.CMD & 0x7F | (proto.LNG & 0x01) << 7; // guardar CMD(7) y LNG(1), no sobra nada
    proto.Frames[1] = (proto.LNG >> 1) & 0x1F; // guarda los siguientes 5 bits, faltan 3
    printf("aten%d,largo %d\n",(proto.Frames[0] >> 7) & 0x01 | (proto.Frames[1] & 0x1F),proto.LNG);
    if(proto.LNG > 0) // si quedan datos por enviar, se encapsulan
    {
        for(size_t i = 0; i < proto.LNG; i++)
        {                   // MSB del dato anterior + LSB del dato nuevo.
            proto.Frames[i + 2] = proto.DATA[i];
        }
    }
    unsigned int fcsValue = fcs(proto.Frames, proto.LNG+2);
    proto.FCS[0] = (fcsValue >> 8) & 0xFF;  
    proto.FCS[1] = fcsValue & 0xFF;  
    proto.Frames[proto.LNG+2] = proto.FCS[0]; //calcula y guarda fsc de Frames en frames
    proto.Frames[proto.LNG + 3] = proto.FCS[1];
    return proto.LNG + 4;
}

int fcs(BYTE * arr,int tam){
    int sum_bits=0;
    for (int i = 0; i < tam; i++) // recorrer el arreglo
    {
        for (int j = 0; j < 8; j++)
        {
            sum_bits += (arr[i] >> j) & 0x01;
        }
        
    }
    return sum_bits;
}

bool desempaquetar(Protocolo & proto, int size)
{
    proto.CMD = proto.Frames[0] & 0x7F;  //7 bits cargados
    proto.LNG = (proto.Frames[0] >> 7) & 0x01 | (proto.Frames[1] & 0x1F);  //1 bit restante cargado
    printf("%d",(proto.Frames[0] >> 7) & 0x01 | (proto.Frames[1] & 0x1F));

    if(size != (proto.LNG + 4)) // detección de tamaños incorrectos
    {
        printf("fue en este coso we %d %d\n",size,proto.LNG+4);
        return false;
    }
    
    if (proto.LNG > 0 && (proto.LNG <= LARGO_DATA))
    {                       // 4 bits
        for (int i = 0; i < proto.LNG; i++)
        {                       
            proto.DATA[i] = (proto.Frames[i + 2]);// guarda el byte completo
        }
    }
    unsigned int _fcs = fcs(proto.Frames, proto.LNG+2); //calcula el fcs de los datos enviados
    proto.FCS[0] = proto.Frames[proto.LNG+2];  //recupera el fcs recibido en el primer espacio del arreglo
    proto.FCS[1] = ((proto.Frames[proto.LNG + 3]));
    if (_fcs != (proto.FCS[0] | proto.FCS[1])){
        return false;
    }
    return true;
}

void EnviarMensaje(Protocolo & proto){
    printf("Ingrese su mensaje:\n");
    scanf("%62s",proto.DATA); //guarda un mensaje ingresado
    proto.LNG= strlen((const char *)proto.DATA);
    proto.LNG = empaquetar(proto);
    memcpy(proto.Frames,proto.Frames,proto.LNG+4);
}



void recibirMensaje(Protocolo proto)
{
    bool estado = desempaquetar(proto, proto.LNG); //ejecuta desempaquetar en el proto dado
    printf("Se recibio un mensaje con estado %s\n",estado?"Correcto":"Incorrecto"); //imprime por pantalla si el mensaje es correcto
    printf("El largo del mensaje es %d\n",proto.LNG);
    printf("El mensaje es %s\n",proto.DATA);
    system("pause");   
}

void Cerrar(int comando){
    //limpiar memoria de receptor.cpp
    if (comando=5)
    exit(0);
}

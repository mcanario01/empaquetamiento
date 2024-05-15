#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "funciones.h"


int empaquetar(Protocolo & proto)
{
    //Protocolo
    // CMD 7 bits / LNG 6 bits / DATA 63 bytes / FCS 10 bits
    if (proto.LNG%2==0){
        proto.Frames[0] = (proto.CMD & 0x7F) | 2*((proto.LNG & 0x01) << 7); // guardar CMD(7) y LSB de LNG(1) en Frames[0]
        proto.Frames[1] = 2*((proto.LNG >> 1) & 0x1F); // guardar los 5 bits siguientes y el siguiente bit de LNG en Frames[1]
    }else{
        proto.Frames[0] = (proto.CMD & 0x7F) | 2*((proto.LNG & 0x01) << 7); // guardar CMD(7) y LSB de LNG(1) en Frames[0]
        proto.Frames[1] = 2*((proto.LNG >> 1) & 0x1F)+1; // guardar los 5 bits siguientes y el siguiente bit de LNG en Frames[1]       
    }
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

	printf("Empaquetando mensaje...\n");
	printf("Su mensaje es %s,\n de largo %d\n\n", proto.DATA, proto.LNG);

	imprimirBytes(proto.Frames, proto.LNG + 4);

    return proto.LNG + 4;
}

// FCS retorna la cantidad de bits en 1 de un arreglo de bytes, se usa para verificar la integridad de los datos
unsigned int fcs(BYTE *arr, int size)
{
	unsigned int sum_bits = 0;
	for (int i = 0; i < size; i++) // recorrer el arreglo
	{
		for (int j = 0; j < 8; j++)
		{
			sum_bits += (arr[i] >> j) & 0x01;
		}
	}
	return sum_bits;
}

bool desempaquetar(Protocolo & proto)
{
	proto.CMD = proto.Frames[0] & 0x7F;									  // 7 bits cargados, sobran 1 bit
	proto.LNG = (proto.Frames[0] >> 7) & 0x01 | (proto.Frames[1] & 0x1F); // 1 bit restante cargado mas 5 bits, sobran 2 bits de relleno

	for (size_t i = 0; i < proto.LNG; i++)
	{
		proto.DATA[i] = (proto.Frames[i + 2]);
	}

	unsigned int _fcs = fcs(proto.Frames, proto.LNG + 2); // calcula el fcs de los datos enviados
	proto.FCS[0] = proto.Frames[proto.LNG + 3];			  // recupera el fcs recibido en el primer espacio del arreglo
	proto.FCS[1] = proto.Frames[proto.LNG + 4];
	if (_fcs != proto.FCS[0] | (proto.FCS[1] << 8))
	{
		return false;
	}
	return true;
}

void obtenerInformacion(Protocolo &proto)
{
    printf("Ingrese su mensaje:\n");
    fgets((char *)proto.DATA, sizeof(proto.DATA), stdin);
    proto.LNG = strlen((const char *)proto.DATA);
    if (proto.LNG > 0 && proto.DATA[proto.LNG - 1] == '\n') {
        proto.DATA[proto.LNG - 1] = '\0';
        proto.LNG--;
    }
    memcpy(proto.Frames, proto.Frames, proto.LNG + 4);
}

bool leerMensaje(Protocolo proto)
{
	bool estado = desempaquetar(proto);										  // ejecuta desempaquetar en el proto dado
	printf("Se recibio un mensaje con estado: %s\n", estado ? "Correcto" : "Incorrecto"); // imprime por pantalla si el mensaje es correcto
	printf("El largo del mensaje es: %d\n", proto.LNG);
	printf("El mensaje es: %s\n", proto.DATA);
	system("pause");
	return estado;
}

void imprimirBits(BYTE byte)
{
	for (int i = 7; i >= 0; i--)
	{
		printf("%d", (byte >> i) & 0x01);
	}
	printf("\n");
}

void imprimirBytes(BYTE *arr, int size)
{
	for (int i = 0; i < size; i++)
	{
		imprimirBits(arr[i]);
	}
}


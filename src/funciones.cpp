#include <stdlib.h>
#include <stdio.h>
#include "funciones.h"
#include <string.h>

void empaquetar(Protocolo & proto)
{
	proto.Frames[0] = (proto.CMD & 0x7F) | ((proto.LNG & 0x01) << 7); // guardar CMD(7) y LSB de LNG(1) en Frames[0]
	proto.Frames[1] = ((proto.LNG >> 1) & 0x1F);					  // guardar los 5 bits siguientes y el siguiente bit de LNG en Frames[1]
	proto.LNG = strlen((const char*)proto.DATA);									  // asigna la longitud del mensaje
	if (proto.LNG > 0) // si quedan datos por enviar, se encapsulan
	{
		for (size_t i = 0; i < proto.LNG; i++)
		{ 
			proto.Frames[i + 2] = proto.DATA[i]; // guarda los datos de DATA en Frames
		}
	}
	unsigned int fcsValue = fcs(proto.Frames, proto.LNG + 2); // calcula el fcs de los datos enviados
	proto.Frames[proto.LNG + 3] =  fcsValue & 0xFF; // calcula y guarda fsc de Frames en frames, LSB
	proto.Frames[proto.LNG + 4] = fcsValue >> 8 & 0x03; // calcula y guarda fsc de Frames en frames, MSB
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
	printf("Ingrese el comando:\n");
	scanf("%d", &proto.CMD);
	printf("Ingrese su mensaje a enviar:\n");
	scanf("%s", proto.DATA);
	printf("Su mensaje es %s, de %d caracteres\n", proto.DATA, proto.LNG);
	if (proto.LNG > 0 && proto.DATA[proto.LNG - 1] == '\n')
	{
		proto.DATA[proto.LNG - 1] = '\0';
		proto.LNG--;
	}

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

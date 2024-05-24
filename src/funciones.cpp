#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <wiringPi.h>
#include "funciones.h"


int empaquetar(Protocolo & proto)
{
    //Protocolo
    // CMD 7 bits / LNG 6 bits / DATA 63 bytes / FCS 10 bits

    proto.Frames[0] = (proto.CMD & 0x7F); // guardar los 7 bits de CMD en Frames[0]
    proto.Frames[1] = (proto.LNG& 0x3F); // guardar los 6 bits de LNG en Frames[1]
    
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
	proto.CMD = proto.Frames[0] & 0x7F;		// 7 bits de comando
	proto.LNG = (proto.Frames[1] & 0x3F); // 6 bits de largo

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
	char linea[LARGO_DATA];
    int posicion=1;
    printf("Ingrese su mensaje:\n");
    fgets((char *)proto.DATA, sizeof(proto.DATA), stdin);
    proto.LNG = strlen((const char *)proto.DATA);
    if (proto.LNG > 0 && proto.DATA[proto.LNG - 1] == '\n') {
        proto.DATA[proto.LNG - 1] = '\0';
        proto.LNG--;
    }
    memcpy(proto.Frames, proto.Frames, proto.LNG + 4);
}

void leerMensaje(Protocolo proto, bool estado)
{										  // ejecuta desempaquetar en el proto dado
	printf("Se recibio un mensaje con estado: %s\n", estado ? "Correcto" : "Incorrecto"); // imprime por pantalla si el mensaje es correcto
    printf("El comando es: %d\n", proto.CMD);
	printf("El largo del mensaje es: %d\n", proto.LNG);
	printf("El mensaje es: %s\n", proto.DATA);
    printf("El FCS es: %d\n", proto.FCS[0] << 8 | proto.FCS[1]);
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

void imprimirCampos(Protocolo proto)
{
    printf("CMD:");
    imprimirBits(proto.CMD);
    printf("\n");
    printf("LNG:");
    imprimirBits(proto.LNG);
    printf("\n");
    printf("DATA:\n");
    imprimirBytes(proto.DATA, proto.LNG);
    printf("\n");
    printf("FCS:");
    imprimirBytes(proto.FCS, sizeof(proto.FCS));
    printf("\n");
    return;
}


int ContarMensajes(const char *arch){
    char nombreArch[LARGO_DATA];
    strcpy(nombreArch, arch);
    strcat(nombreArch,".txt");
    FILE * texto =fopen(nombreArch,"r");
    char linea[LARGO_DATA];
    int numero=0;
    while (fgets(linea, sizeof(linea), texto) != NULL) {
    numero++;
    }
    fclose(texto);
    return numero;
} 

void BuscarArchivo(Protocolo &proto)
{
    printf("Ingrese su mensaje:\n");
    fgets((char *)proto.DATA, sizeof(proto.DATA), stdin);
    proto.LNG = strlen((const char *)proto.DATA);
    if (proto.LNG > 0 && proto.DATA[proto.LNG - 1] == '\n') {
        proto.DATA[proto.LNG - 1] = '\0';
        proto.LNG--;
    }
    memcpy(proto.Frames, proto.Frames, proto.LNG + 4);
    return;
}

void EncontrarArchivo(Protocolo mensaje){
    char linea[LARGO_DATA];
    strcat((char*)mensaje.DATA,".txt");
    FILE * archivo = fopen((char*)mensaje.DATA,"r");
    if(archivo != NULL){
        while (fgets(linea, sizeof(linea), archivo) != NULL) {
            printf("%s", linea);
        }
        fclose(archivo);
    } else {
        printf("El archivo %s no existe o no se encontro\n",(char*)mensaje.DATA);
    }
    return;

}

void MensajesRecibidos(){
    int Correctos=ContarMensajes("mensajes");
    int Incorrectos=ContarMensajes("Errores");
    int Total=Correctos+Incorrectos;
    printf("Mensajes enviados correctamente:%d\nMensajes enviados Incorrectamente:%d\nTotal de mensajes enviados:%d\n",Correctos,Incorrectos,Total);
    return;
}

void EscribirArchivo(const char* arch,Protocolo proto){
	char nombreArch[LARGO_DATA];
	strcpy(nombreArch, arch);
    strcat(nombreArch,".txt");
	char linea[LARGO_DATA];
	int posicion=0;
	FILE * texto=fopen(nombreArch,"a+");
    while (fgets(linea, sizeof(linea), texto) != NULL) 
    {
        posicion++;
    } 
	fprintf(texto,"%s\n",proto.DATA);
	fclose(texto);
}

BYTE calcularNumeroDeUnos(BYTE byte)
{
    BYTE contador = 0;
    for (int i = 0; i < 8; i++)
    {
        contador += (byte >> i) & 0x01;
    }
    return contador;
}

BYTE calcularNumeroDeUnos(BYTE *arr)
{
    BYTE contador = 0;
    for (int i = 0; i < sizeof(arr); i++)
    {
        for (size_t j = 0; j < 8; j++)
        {
            contador += (arr[i] >> j) & 0x01;
        }
        
    }
    return contador;
}

void limpiarMensaje(Protocolo &mensaje)
{
    mensaje.CMD = 0;
    mensaje.LNG = 0;
    for(size_t i = 0; i < sizeof(mensaje.DATA); i++)
    {
        mensaje.DATA[i] = 0;
    }
    for(size_t i = 0; i < sizeof(mensaje.Frames); i++)
    {
        mensaje.Frames[i] = 0;
    }
    for(size_t i = 0; i < sizeof(mensaje.FCS); i++)
    {
        mensaje.FCS[i] = 0;
    }
}

void limpiarBuffer(BYTE *buffer)
{
    memset(buffer, 0, sizeof(buffer));
}

void listarArchivosDeTexto()
{
    system("ls *.txt");
    return;
}

bool compararFrames(BYTE *Frame1, BYTE *Frame2, int size)
{
    for (size_t i = 0; i < size; i++)
    {
        if(Frame1[i] != Frame2[i])
        {
            return false;
        }
    }
    return true;
}

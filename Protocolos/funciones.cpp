#include "funciones.h"

bool desempaquetar(Protocolo & proto,int tam){
                        // 1 bit            3 bits
    proto.CMD = proto.trama[0] & 0x01;
    proto.LNG = (proto.trama[0] >> 1) & 0x07;
    if (tam != (proto.LNG+2)){ // Tamaños incorrectos
        return false;
    }
    if (proto.LNG > 0 && (proto.LNG <= LARGO_DATA))
    {                       // 4 bits
        for (int i = 0; i < proto.LNG; i++)
        {                       
            proto.DATA[i] = ((proto.trama[i] >> 4) & 0x0F) | ((proto.trama[i+1] & 0x0F )<< 4);
        }                           // 4 bits guardados en parte menos significativa
    }
    int _fcs = fcs(proto.trama,proto.LNG); // Se calcula el FCS solo hasta el byte anterior en donde se almacena FCS
    BYTE arr_aux[1] = { (BYTE) (proto.trama[proto.LNG] & 0x0F)}; // se aíslan solo los datos
    _fcs+= fcs(arr_aux,1);
    proto.FCS = proto.trama[proto.LNG] >> 4 | ( (proto.trama[proto.LNG+1] & 0x03) << 4);
    if (_fcs != proto.FCS){
        return false;
    }
    return true;
}

int empaquetar(Protocolo & proto){
                        // 1 bit            3 bits
    proto.trama[0] = (proto.CMD & 0x01) | ((proto.LNG & 0x07) << 1);
    if (proto.LNG > 0 && (proto.LNG <= LARGO_DATA))
    {                       // 4 bits
        proto.trama[0] |= (proto.DATA[0] & 0x0F) << 4;
        for (int i = 0; i < proto.LNG-1; i++)
        {                       // MSB de data anterior + LSB de data siguiente
            proto.trama[i+1] = proto.DATA[i] >> 4 | ((proto.DATA[i+1] & 0x0F) << 4);
        }                           // 4 bits guardados en parte menos significativa
        proto.trama[proto.LNG] = (proto.DATA[proto.LNG-1] >> 4 ) & 0x0F;
    }
    proto.FCS = fcs(proto.trama,proto.LNG+1); // Se calcula el FCS
                            
    proto.trama[proto.LNG] |= (proto.FCS & 0x0F) << 4;
    proto.trama[proto.LNG+1] = (proto.FCS >> 4) & 0x03;
    return proto.LNG+2;
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
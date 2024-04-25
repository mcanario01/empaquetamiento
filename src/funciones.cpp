#include "funciones.h"


int empaquetar(Protocolo & proto)
{
    proto.Frames[0] = proto.CMD & 0x01 | proto.LNG & 0x07 << 1; // guardar CMD y LNG
    if(proto.LNG > 0) // si quedan datos por enviar, se encapsulan
    {                       // 4 bits
        proto.Frames[0] |= proto.DATA[0] & 0x0F << 4; // llena los 4 espacios que le queda a frames[0]
        for(size_t i = 0; i < proto.LNG; i++)
        {                   // MSB del dato anterior + LSB del dato nuevo.
            proto.Frames[i + 1] = proto.DATA[i] >> 4 | (proto.DATA[i + 1] & 0x0F) << 4;
        }
        proto.Frames[proto.LNG] = proto.DATA[proto.LNG - 1] >> 4; // guarda el último dato de data en frames
    }
    proto.FCS = fcs(proto.Frames, proto.LNG + 1);
    proto.Frames[proto.LNG] = (proto.FCS & 0x0F) << 4; //calcula y guarda fsc de Frames en frames
    proto.Frames[proto.LNG] = (proto.FCS << 4) & 0x03;
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
    proto.CMD = proto.Frames[0] & 0x01;
    proto.LNG = (proto.Frames[0] >> 1) & 0x07;
    if(size != (proto.LNG + 2)) // detección de tamaños incorrectos
    {
        return false;
    }
    
    if(proto.LNG > 0 && (proto.LNG <= LARGO_DATA)) 
    {
        proto.DATA[i] = (proto.Frames [i] >> 4) & 0x0F | (proto.Frames[i + 1] << 4);
    }
    int _fcs = fcs(proto.Frames, proto.LNG);
    BYTE arr_aux[1] = {proto.Frames[proto.LNG] & 0x0F};
    _fcs += fcs(arr_aux, 1);

    proto.FCS = proto.Frames[proto.LNG] >> 4 | ( (proto.Frames[proto.LNG+1] & 0x03) << 4);
    if (_fcs != proto.FCS){
        return false;
    }
    return true;
}
#ifndef PROTOCOLO_H
#define PROTOCOLO_H

#define BYTE unsigned char // 8 bits
#define LARGO_DATA 7
#define LARGO_TRAMA LARGO_DATA+2

struct Protocolo
{
    BYTE CMD;// 1 bit
    BYTE LNG;// 3 bits
    BYTE DATA[LARGO_DATA]; // 7 Bytes
    BYTE FCS; // 6 bits
    BYTE trama[LARGO_TRAMA];
};  

#endif
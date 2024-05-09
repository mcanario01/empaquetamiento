#ifndef FRAMES_H
#define FRAMES_H

#define BYTE unsigned char
#define LARGO_DATA 63
#define BYTES_EXTRAS 4

struct Protocolo
{
    BYTE CMD;   // 7 bit utiles, 1 bit de relleno
    BYTE LNG;   // 6 bits utiles, 2 bits de relleno
    BYTE DATA[LARGO_DATA];  // 63 bytes
    BYTE FCS[2]; // 10 bits utiles, 6 bits de relleno
    BYTE Frames[LARGO_DATA + BYTES_EXTRAS];
};


#endif
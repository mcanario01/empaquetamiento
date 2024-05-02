#ifndef FRAMES_H
#define FRAMES_H

#define BYTE unsigned char
#define LARGO_DATA 7

struct Protocolo
{
    BYTE CMD;   // 7 bit
    BYTE LNG;   // 6 bits
    BYTE DATA[LARGO_DATA];  // 63 bytes
    BYTE FCS; // 10 bits
    BYTE Frames[LARGO_DATA + 2];
};


#endif
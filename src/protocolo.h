#ifndef FRAMES_H
#define FRAMES_H

#define BYTE unsigned char
#define LARGO_DATA 7

struct Protocolo
{
    BYTE CMD;   // 1 bit
    BYTE LNG;   // 3 bits
    BYTE DATA[LARGO_DATA];  // 7 bytes
    BYTE FCS; // 6 bits
    BYTE Frames[LARGO_DATA + 2];
};


#endif
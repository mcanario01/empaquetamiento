
//INCLUSIONES
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

//MACROS
#define CLOCK_PIN 23
#define TX_PIN 2
#define RX_PIN 3

#define BYTE unsigned char


int main(){
  //INICIA WIRINGPI
  if(wiringPiSetup() == -1)
    exit(1);

  //CONFIGURA PINES DE ENTRADA SALIDA
  pinMode(RX_PIN, INPUT);
  pinMode(TX_PIN, OUTPUT);
  pinMode(CLOCK_PIN,INPUT);

while(true){

  printf("%d %d %d\n",digitalRead(CLOCK_PIN),digitalRead(TX_PIN),digitalRead(RX_PIN));
}

  return 0;
}

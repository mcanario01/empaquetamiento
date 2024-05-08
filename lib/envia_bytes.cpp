//INCLUSIONES
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

//MACROS
#define DELAY_PIN 0
#define TX_PIN 2

#define BYTE unsigned char

//DECLARACION DE PROTOTIPOS
void cb(void);
void startTransmission();

//VARIABLES GLOBALES
volatile int nbits = 0;
BYTE bytes[10] = {1,2,3,4,5,6,7,8,9,10};
volatile int nbytes = 0;
BYTE len = 10;
int nones = 0;
bool transmissionStarted = false;

int main(){
  //INICIA WIRINGPI
  if(wiringPiSetup() == -1)
    exit(1);

  //CONFIGURA INTERRUPCION PIN CLOCK (PUENTEADO A PIN PWM)
  if(wiringPiISR(DELAY_PIN, INT_EDGE_RISING, &cb) < 0){
    printf("Unable to start interrupt function\n");
  }

  //CONFIGURA PINES DE ENTRADA SALIDA
  pinMode(TX_PIN, OUTPUT);
  
//  printf("Delay\n");
//  delay(5000);
  startTransmission();
  while(transmissionStarted)
    delay(2000);
  
  return 0;
}

void cb(void){
  if(transmissionStarted){
    //Escribe en el pin TX
    if(nbits == 0){
      digitalWrite(TX_PIN, 0); //Bit de inicio
    }else if(nbits < 9){
      digitalWrite(TX_PIN, (bytes[nbytes] >> (nbits-1)) & 0x01); //Bit de dato
//      printf("%d",(bytes[nbytes]>>(nbits-1))&0x01);
    }else if(nbits == 9){
//      printf("\n");
      nones = (bytes[nbytes]&0x01) + ((bytes[nbytes]&0x02)>>1) + ((bytes[nbytes]&0x04)>>2) + 
        ((bytes[nbytes]&0x08)>>3) + ((bytes[nbytes]&0x10)>>4) + ((bytes[nbytes]&0x20)>>5) + 
        ((bytes[nbytes]&0x40)>>6) + ((bytes[nbytes]&0x80)>>7);
      digitalWrite(TX_PIN, nones%2==0); //Bit de paridad
    }else{
      digitalWrite(TX_PIN, 1); //Canal libre durante 2 clocks
    }

    //Actualiza contador de bits
    nbits++;

    //Actualiza contador de bytes
    if(nbits == 11){
      nbits = 0;
      nbytes++;

      //Finaliza la comunicación
      if(nbytes==len){
        transmissionStarted = false;
        nbytes = 0;
      }
    }
  }else{
    //Canal en reposo
    digitalWrite(TX_PIN, 1);
  }
}

void startTransmission(){
  transmissionStarted = true;
}

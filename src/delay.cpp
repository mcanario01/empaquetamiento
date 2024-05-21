#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>

#define PWM_PIN 1 // => 18 BCM
#define PWM_RANGE 1024
#define PWM_FREQUENCY 1
//Frequency = 19.2 Mhz / clock_divisor / range
//500 Hz = 19.2 MHz / 19.2 / 20
//100 Hz = 19.2 MHz / 19.2 / 100
//9.765625 Hz = 19.2 MHz / 19.2 / 1024



int obtenerRange(int frequency);
int obtenerDivisor(int frequency);

int main(){
  if(wiringPiSetup() == -1)
    exit(1);

  //CONFIGURA PWM
  printf("Configurando pin de PWM...\n");
  pinMode(PWM_PIN, PWM_OUTPUT);
  printf("Configurando PWM...\n");
  pwmSetMode(PWM_MODE_MS); //Mark Space Mode
  printf("Configurando rango y divisor de clock...\n");
  pwmSetRange(PWM_RANGE);
  pwmSetClock(obtenerDivisor(PWM_FREQUENCY));
  
  printf("PWM configurado a %d Hz\n", PWM_FREQUENCY);
  pwmWrite(PWM_PIN, PWM_RANGE/2);// 50% del ciclo

  printf("Ejecutando reloj PWM...\n");
  printf("Presiona Ctrl+C para salir\n");
  while(1){
    delay(500);
  }

  return 0;
}



int obtenerDivisor(int frequency){
  return 19200000 / frequency / PWM_RANGE;
}

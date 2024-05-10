#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>

#define PWM_PIN 1 // => 18 BCM
#define PWM_CLOCK_DIVISOR 192
#define PWM_RANGE 4000
//Frequency = 19.2 Mhz / clock_divisor / range
//500 Hz = 19.2 MHz / 192 / 200
//100 Hz = 19.2 MHz / 192 / 1000

int main(){
  if(wiringPiSetup() == -1)
    exit(1);

  //CONFIGURA PWM
  pinMode(PWM_PIN, OUTPUT);
  //pwmSetMode(PWM_MODE_MS); //Mark Space Mode
  //wmSetRange(PWM_RANGE);
  //pwmSetClock(PWM_CLOCK_DIVISOR);
  //pwmWrite(PWM_PIN, 1000);// 50% del ciclo

  while(1){
    printf("Encendido\n")
    digitalWrite(PWM_PIN, HIGH);
    delayMilliseconds(500);
    printf("Apagado\n")
    digitalWrite(PWM_PIN, LOW);
    delayMilliseconds(500);
  }

  return 0;
}
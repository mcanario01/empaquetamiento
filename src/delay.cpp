#include <wiringPi.h>
#include <stdlib.h>

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
  pinMode(PWM_PIN, PWM_OUTPUT);
  pwmSetMode(PWM_MODE_MS); //Mark Space Mode
  pwmSetRange(PWM_RANGE);
  pwmSetClock(PWM_CLOCK_DIVISOR);
  pwmWrite(PWM_PIN, 2000);// 50% del ciclo

  while(1){
    delay(500);
  }

  return 0;
}
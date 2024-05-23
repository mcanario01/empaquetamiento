#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>

#define PWM_PIN 1 // => 18 BCM
#define PWM_RANGE 1024
#define PWM_FREQUENCY 128

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

  system("clear");
  while(1){
    //Menu
    
    printf("DELAY\n");
    printf("1. Cambiar frecuencia\n");
    printf("2. Salir\n");
    printf("Opcion: ");
    int opcion;
    scanf("%d", &opcion);
    getchar();
    if(opcion == 1)
    {
      printf("Frecuencia: ");
      int frequency;
      scanf("%d", &frequency);
      getchar();
      pwmSetClock(obtenerDivisor(frequency));
      printf("PWM configurado a %d Hz\n", frequency);
    }
    else if(opcion == 2)
    {
      break;
    }
    else
    {
      printf("Opcion no valida\n");
    }
  }

  return 0;
}

// Funcion que obtiene el divisor del clock
// usando la formula de la documentacion de wiringPi
// que es clock = 19.2 MHz / clock_divisor / range
int obtenerDivisor(int frecuencia){
  return 19200/frecuencia;
}


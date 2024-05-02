#include <stdio.h>
#include <wiringPi.h>
#define SPEED 100000

int main(){

if (wiringPiSetupGpio () == -1){
printf("Error");
return 0;
}
pinMode(18,INPUT);
while(true){

bool level=digitalRead(18);

}
return 0;
}
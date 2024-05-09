#include <stdio.h>
#include "funciones.h"
#include <stdlib.h>
#include <string.h>
#ifdef RASPBERRY_PI
#include <wiringPi.h>
#endif
#ifndef RASPBERRY_PI
#define wiringPiSetupGpio() 0
#define pinMode(a,b)
#define digitalWrite(a,b)
#define delayMicroseconds(a)
#define delay(a)
#define HIGH 1
#define LOW 0
#endif

#define SPEED 100000

int main(){
    
    /*
    if(argc != 2){
        printf("Error, debe ingresar un argumento\n");
        return 0;
    }
    if(strcmp(argv[1],"Windows") != 0 || strcmp(argv[1],"Linux") != 0){
        printf("El sistema operativo ingresado no tiene soporte.\n");
    return 0;
    }
    */

    int opcion=0; //variable para seleccionar una opcion del menú
    int contmensajes=0; //variable para contar los mensajes enviados
    Protocolo proto; //estructura para enviar mensajes
    proto.CMD=7; //comando de envio de mensaje
    proto.LNG=6; //longitud del mensaje



    if (wiringPiSetupGpio() == -1)
    {
        printf("Error");
        return 0;
    }
    pinMode(17,OUTPUT); //pin receptor */



    while(opcion !=6){
        printf("-----------------------------------------------\n");
        printf("mensajes enviados:%d\n", contmensajes);
        printf("-----------------------------------------------\n");
        printf("MENU PRINCIPAL\n");
        printf("Seleccione una opcion\n[1]Enviar mensaje de texto\n[2]Enviar mensaje de prueba\n[3]Mostrar contenido\n[4]Contador de mensajes\n[5]Cerrar el emisor\n[6]Salir del programa\n");
        scanf("%d",&opcion);
        getchar();
        switch(opcion){
            case 1:
                EnviarMensaje(proto);
                recibirMensaje(proto);
                //Enviar mensaje de texto y ser guardado en un archivo mensajes.txt
                break;
            case 2:
                //Enviar mensaje de prueba 10 veces y el receptor debe contar los enviados correctamente, con error y con error no detectado, además de calcular los procentajes
                //de acierto y error y mostrarlo por consola del receptor
                for(int i = 0; i < 10; i++){
                    EnviarMensaje(proto);
                    recibirMensaje(proto);
                }
                break;
            case 3:
                //Se debe enviar un mensaje con el nombre de un archivo de texto de prueba en el receptor, con dicho comando, el receptor debe mostrar el contenido del archivo
                //consola en caso de que este exista, en caso contrario el receptor debe mostrar por consola que el archivo no existe.
                break;
            case 4:
                //El receptor debe imprimir los mensajes recibidos correctamente y con error (no cuentan los de prueba)
                break;
            case 5:
                //Cerrar el programa del receptor
                break;
            case 6:
                break;
            default:
                printf("La opcion ingresada no es valida, intente nuevamente\n");
                break;
        }
    }
    return 0;
}

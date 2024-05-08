#include <stdio.h>
//#include <wiringPi.h>
#include "funciones.h"
#include <stdlib.h>
#include <string.h>

#define SPEED 100000

int main(int argc, char *argv[]){
    

    if(argc != 2){
        printf("Error, debe ingresar un argumento\n");
        return 0;
    }
    if(strcmp(argv[1],"Windows") != 0 || strcmp(argv[1],"Linux") != 0){
        printf("El sistema operativo ingresado no tiene soporte.\n");
    return 0;
    }

    int opcion=0; //variable para seleccionar una opcion del menú
    int contmensajes=0; //variable para contar los mensajes enviados
    Protocolo proto;
    proto.CMD=7;
    proto.LNG=6;


    if(argv[1] == "Linux"){
        #include <wiringPi.h>
        if (wiringPiSetupGpio () == -1)
        {
            printf("Error");
            return 0;
        }
        pinMode(17,OUTPUT); //pin receptor */
    }


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
# Tarea 1 - Protocolo de empaquetado y desempaquetado.

>**Autores:** Argel J, Canario M.\
>**Docente:** José Sanchez.\
>**Asignatura:** Protocolos de comunicación y sistemas embebidos.

## Compilando

Para compilar este código hay que seguir una serie de pasos, de manera preeliminar, el código funciona en las siguentes plataformas:

- Raspberry Pi 2 (arm7v), para la conección final.
- Windows (MinGW), para pruebas.

### Prerequisitos

#### Windows

Instalar msys2 a través de winget:

    winget install MSYS2.MSYS2

Agrega el compilador al path:

    [Environment]::SetEnvironmentVariable("Path", $env:Path + ";C:\msys64\mingw64\bin", [EnvironmentVariableTarget]::User)

#### Raspberry Pi

Tener las herramientas de compilado instaladas:

    sudo apt-get update && sudo apt-get install wiringPi build-essential -y

### Descarga
Clona el repositorio con:

    git clone https://github.com/mcanario01/empaquetamiento.git

Accede a la carpeta del projecto:

    cd ./empaquetamiento-main/src

### Construcción

Para compilar el algoritmo base de enviado y recepción de datos, usa:

    make

Para ejecutar el emisor, escribe:

    make run-emisor

Para ejecutar el receptor, escribe:

    make run-receptor

Para limpiar todos los ejecutables:

    make clear

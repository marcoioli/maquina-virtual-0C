#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mv.h"


int main(int argc, char *argv[]) {
    TVM VM;
    char VecFunciones[CANTFUNC][5]; //5 Es la cantidad de caracteres que tiene como maximo el nombre de la funcion.
    char VecRegistros[CANTREG][4];
    char nomarch[50];
    THeader header;
    int i;

    
    // 1. Buscar si se pasó "-p"
    int pos_p = -1;
    i = 1;
    while (i < argc && pos_p == -1) {
        if (strcmp(argv[i], "-p") == 0) {
            pos_p = i;
        }
       i++;
    }

    // 2. Calcular cantidad de parámetros reales
    int cantParams = 0;
    if (pos_p != -1) {
        cantParams = argc - (pos_p + 1);
    }

    // 3. Si hay parámetros, crear el Param Segment
    if (cantParams > 0) {
        cargaParametros(&VM, cantParams, &argv[pos_p + 1]);
    } else {
        VM.param_size = 0;
    }

    strcpy(nomarch,argv[1]); //si ejecutas main.exe prueba.vmx se guarda prueba.vmc en nomarch
    leoArch(&VM, argv[1], cantParams, &argv[pos_p + 1]);


    inicializoVecFunciones(VecFunciones);
    inicializoVecRegistros(VecRegistros);

    if (argc > 2) {
        if (!strcmp(argv[2],"-d")) { //si ejecutas main.exe prueba.vmx -d se ejecuta el dissasembler
            LeoDissasembler(&VM,VecFunciones,VecRegistros);
        }
    }

    printf("Comienza a leer IP \n");
    leeIP(&VM);
 

    return 0;
}



#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mv.h"


int main(int argc, char *argv[]) {
    TVM VM;
    char VecFunciones[CANTFUNC][5]; //5 Es la cantidad de caracteres que tiene como maximo el nombre de la funcion.
    char VecRegistros[CANTREG][4];
    char nomarch[50];

    strcpy(nomarch,argv[1]); //si ejecutas main.exe prueba.vmx se guarda prueba.vmc en nomarch
    leoArch(&VM,nomarch);

    inicializoVecFunciones(VecFunciones);
    inicializoVecRegistros(VecRegistros);

    if (argc > 2) {
        if (!strcmp(argv[2],"-d")) { //si ejecutas main.exe prueba.vmx -d se ejecuta el dissasembler
            LeoDissasembler(&VM,VecFunciones,VecRegistros);
        }
    }

    printf("Comienza a leer IP \n");
    leeIP(&VM);
 
    
    printf("EAX = %d (0x%08X)\n", VM.reg[EAX], VM.reg[EAX]);
    printf("EBX = %d (0x%08X)\n", VM.reg[EBX], VM.reg[EBX]);
    printf("ECX = %d (0x%08X)\n", VM.reg[ECX], VM.reg[ECX]);
    printf("EDX = %d (0x%08X)\n", VM.reg[EDX], VM.reg[EDX]);
    printf("AC  = %d (0x%08X)\n", VM.reg[AC], VM.reg[AC]);
    printf("CC  = %d (0x%08X)\n", VM.reg[CC], VM.reg[CC]);
    

    return 0;
}



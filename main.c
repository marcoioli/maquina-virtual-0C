#include <stdio.h>
#include <stdint.h>
#include "mv.h"


int main() {
    TVM VM;

    leoArch(&VM);
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


//flujo
/* 
leer header, guardo base y tamanio de codigo y data segment
cargo ds y cs
cs = inicio(leido) offset
ds = 1?????????

inicializo ip, al principio ip=cs (punteros) 
leo ip

73 -> 0111 0011 
Primero 2 bits -> getop1
Segundos 2 bits -> getop2 
5 bits menos significativos -> opc -> instrucciones[opc]
(xxx1 0000) = 10
(xxx0 0000) = 0

avanzo ip -> ip+=1byte + bytesop1 + bytes op2

si op1 o op2 es memoria
cargo mar lar mbr

hago la instruccion correspondiente
*/
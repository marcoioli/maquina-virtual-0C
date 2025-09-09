#include <stdio.h>
#include <stdint.h>
#include "mv.h"


void declaraFunciones (vFunciones funciones) {
    //declara las funciones, cuando haga funciones[0] se ejecuta el sys
  Funciones[0]=SYS;
  Funciones[1]=JMP;
  Funciones[2]=JZ;
  Funciones[3]=JP;
  Funciones[4]=JN;
  Funciones[5]=JNZ;
  Funciones[6]=JNP;
  Funciones[7]=JNN;
  Funciones[8]=NOT;
  Funciones[10]=MOV;
  Funciones[11]=ADD;
  Funciones[12]=SUB;
  Funciones[13]=MUL;
  Funciones[14]=DIV;
  Funciones[15]=CMP;
  Funciones[16]=SHL;
  Funciones[17]=SHR;
  Funciones[18]=SAR;
  Funciones[19]=AND;
  Funciones[20]=OR;
  Funciones[21]=XOR;
  Funciones[22]=SWAP;
  Funciones[23]=LDL;
  Funciones[24]=LDH;
  Funciones[25]=RND;
  Funciones[26]=STOP;
}

void iniciaRegs(TVM * VM,int tam) {
    VM->reg[CS] = 0; 
    VM->reg[DS] = tam; //tamanio?? 
    VM->reg[IP] = VM->reg[CS];
}

void cargaSegmentos(TVM * VM,int tam) {
    VM->segmentos[]
    /*aca me conviene un vector con cs y ds, cargando parte alta y parte baja
    o un vector de registros (base y tamanio) y en cada uno guardo el valor
    */
}

void leoArch(char nombrearch[],TMV * VM) {
    FILE * archb;
    THeader header;
    char t1,t2;
    
    if(archb = fopen(nombrearch,"rb")==NULL)
        printf("No se pudo abrir el archivo .asm");
    else {
        fread(&header.id,sizeof(char*TAMID),1,archb);
        fread(&header.version,siezof(2*char),1,archb);
        //como se lee el tamanio??
        
        fread(&t1,sizeof(char),1,archb); //2D necesito cargar 8 y 8
        //c celda es de 32 bits, almaceno 002D, 0000 0000 0010 
        fread(&t2,sizeof(char),1,archb);
        header.tam = t1(<<8) | t2; //002D
        //pregutnar

        if (strcmp(header.id, "VMX25")) {
            if (header.version == 1) {
                cargaSegmentos(VM,header.tam);
                iniciaRegs();
                //carga memoria
            }
        }
    


    }
    
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

void MOV(Instruccion inst) 
{
  
}

int getOp2(... leido) {
  return (leido>>6) & 0x03;
}

int getOp1(... leido) {
  return (leido>>4) & 0x03;
}

int getOpc(... leido) {
  return (leido & 0x0F);
}

void cargaMemoria() {
  /*
  lar = segmento offest??
  mar = cantidad direccion fisica
  mbr = valor a cargar
  



}

void getInstruction() {
  Instruccion inst;
  int tipoOp1,tipoOp2,opc;

  int ip = reg[REG_IP];
  int leido = memory[ip];

  switch(tipoOp2) {
    //tipoop es la cantidad de bytes a leeer
    case OP_REGISTRO: 
          
    break;
  }

  inst.opc = opc;
  inst.op1.tipoOp = tipoOp1;
  inst.op2.tipoOp = tipoOp2;

  reg[REG_OPC]= inst.opc;
  reg[REG_OP1] = (inst.op1.tipoOp << 24 | (inst.op1.valor & MASK_OP)); //mover tipo a los 8 bits mas significativos y valor al resto, el OR concatena bit a bit
  reg[REG_OP2] = (inst.op2.tipoOp << 24 | (inst.op2.valor & MASK_OP));
   
}

*/
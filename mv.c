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
    VM->reg[CS] = 0x00000000; 
    VM->reg[DS] = tam; //tamanio?? 
    VM->reg[IP] = VM->reg[CS];
}

void cargaSegmentos(TVM * VM,int tam) {
    VM->segmentos[CS] = tam;
    /*aca me conviene un vector con cs y ds, cargando parte alta y parte baja
    o un vector de registros (base y tamanio) y en cada uno guardo el valor
    */
    VM->segmentos[DS] = (tam << 16) | (MEMORY_SIZE - tam); //los primeros 2 bytes son la base y los otros dos el tam
}

void leoArch(char nombrearch[],TVM * VM) {
    FILE * archb;
    THeader header;
    char t1,t2;
    int i=0;
    
    if(archb = fopen(nombrearch,"rb")==NULL)
        printf("No se pudo abrir el archivo .asm");
    else {
        fread(&header.id,sizeof(char*TAMID),1,archb);
        fread(&header.version,siezof(2*char),1,archb);
        //como se lee el tamanio??

        //VMX25 1 tamanio 
        
        fread(&t1,sizeof(char),1,archb); //2D necesito cargar 8 y 8
        //c celda es de 32 bits, almaceno 002D, 0000 0000 0010 
        fread(&t2,sizeof(char),1,archb);
        header.tam = t1(<<8) | t2; //002D
        //pregutnar

        if (strcmp(header.id, "VMX25")) {
            if (header.version == "1") {
                cargaSegmentos(VM,header.tam);
                iniciaRegs();
                //carga memoria

                while(fread(&(MV->MEM[i]),1,1,arch)==1) {
                     //lee de a 1 byte y carga la memoria con todo el codigo del .asm
                    i++;
                }
            }
        }
        fclose(archb);

    }
    
}

int getBase(int valor) {
  return (valor & 0xFFFF0000)>>16;
}

int getTam(int valor) {
  return (valor & 0x0000FFFF);
}

int direccionamiento_logtofis(TVM VM, int puntero){
    int DirBase, Offset, DirFisica, TamSeg, LimiteSup;
    
    DirBase = getBase(VM.segmentos[(puntero & 0xFFFF0000) >> 16]);
    Offset = puntero & 0x0000FFFF;

    DirFisica = DirBase + Offset;
    TamSeg = getTam(VM.segmentos[(puntero & 0xFFFF0000) >> 16]);

    // 5. Límite superior (última celda válida)
    LimiteSup = DirBase + TamSeg;

    if (!( (DirBase <= DirFisica) && (DirFisica + 4 <= LimiteSup) )) {
        generaerror(2);   // fallo de segmento
        return -1;        // nunca llega si generaerror aborta
    } else {
        return DirFisica; // dirección física válida
    }
}

void ComponentesInstruccion(TVM VM, int DirFisica, Instruccion *instr, int *CantOp, unsigned char *CodOp){
   unsigned char Instruccion = VM.memory[DirFisica];

  instr->sizeB= (Instruccion & 0x000000C0) >> 6;
  instr->sizeA = (Instruccion & 0x00000030) >> 4;
  *CodOp = Instruccion & 0x1F;
  *CantOp=2;

  //Si no pasa por ningun if significa que tiene dos operandos.

  if (instr->sizeA == 0){ //No existe OpA
      if (instr->sizeB == 0){ //No existe opB
        *CantOp=0;
      }
      else{ //Existe solo un operando
          *CantOp=1;
      }
  }
}

void SeteoValorOp(TVM VM,int DirFisicaActual,Instruccion *instr){
    instr->op1 = 0;
    instr->op2 = 0;

    for (int i=0;i<instr->sizeB;i++){
        instr->sizeB += VM.memory[++DirFisicaActual];
        if ((instr->sizeB-i) > 1)
            instr->op2 = instr->op2 << 8;
    }

    for (int i=0;i<instr->op1;i++){
        instr->op1 += VM.memory[++DirFisicaActual];
        if ((instr->sizeA-i) > 1)
            instr->op1 = instr->op1 << 8;
    }
}


void leeIP(TVM * MV) {

    Instruccion instru;
    vFunciones funciones;

    declaraFunciones(funciones);

    while (VM->reg[IP] < getTam(VM->reg[CS>>16])+getBase(VM->reg[CS>>16])) { //[>>16 porque vas a donde seta el segmento par aconseguir la base y el tamanio]

    }
    
}

void DefinoRegistro(int *CodReg, int Op){ 
  *CodReg = Op & 0x1F;
}// Devuelve codigo de registro

void DefinoAuxRegistro(int *AuxR,TVM VM, int CodReg){
  *AuxR = VM.reg[CodReg];
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
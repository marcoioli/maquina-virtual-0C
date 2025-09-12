#include <stdio.h>
#include <stdint.h>
#include "mv.h"


void declaraFunciones (vFunciones Funciones) {
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
    int indicesegmento;

    indicesegmento = (puntero & 0xFFFF0000) >>16;

    DirBase = getBase(VM.segmentos[indicesegmento]);
    Offset = puntero & 0x0000FFFF;

    DirFisica = DirBase + Offset;
    TamSeg = getTam(VM.segmentos[indicesegmento]);

    // 5. Límite superior (última celda válida)
    LimiteSup = DirBase + TamSeg;

    if (!( (DirBase <= DirFisica) && (DirFisica + 4 <= LimiteSup) )) { //dir fisica + 4 ????
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
          //en este caso, uso siempre op1
            instr->sizeA = instr->sizeB;
            instr->sizeB = 0;
          *CantOp=1;        
      }
  }
}

void SeteoValorOp(TVM VM,int DirFisicaActual,Instruccion *instr){
    instr->valorA = 0;
    instr->valorB = 0;

    for (int i=0;i<instr->sizeB;i++){
        instr->valorB += VM.memory[++DirFisicaActual];
        if ((instr->sizeB-i) > 1)
            instr->valorB = instr->valorB << 8;
    }

    for (int i=0;i<instr->op1;i++){
        instr->valorA += VM.memory[++DirFisicaActual];
        if ((instr->sizeA-i) > 1)
            instr->valorA = instr->valorA << 8;
    }
}

/*
void SeteoValorOp(TVM *VM, int dirFisicaActual, Instruccion *instr) {
    instr->valorA = 0;
    instr->valorB = 0;

    // --- Operando B ---
    for (int i = 0; i < instr->sizeB; i++) {
        instr->valorB = (instr->valorB << 8) | VM->memory[++dirFisicaActual];
    }

    // --- Operando A ---
    for (int i = 0; i < instr->sizeA; i++) {
        instr->valorA = (instr->valorA << 8) | VM->memory[++dirFisicaActual];
    }
}
*/


void leeIP(TVM * VM) {
    int cantOp,DirFisicaActual,indiceseg;
    unisgned char codOp;
    Instruccion instru;
    vFunciones Funciones;

    declaraFunciones(funciones);
    indiceseg = (VM->reg[CS]>>16)
    //[>>16 porque vas a donde seta el segmento par aconseguir la base y el tamanio]

    while (VM->reg[IP] < getTam(indiceseg)+getBase(indiceseg)) { //
      DirFisicaActual = direccionamiento_logtofis(*VM,VM->R[IP]);

      ComponentesInstruccion(*MV,DirFisicaActual,&instruc,&CantOp,&CodOp);

      if (cantOp > 0) {
        SeteoValorOp(*MV,DirFisicaActual,&instruc);
      }
      else {
          instruc.op1 = 0;
          instruc.op2 = 0;
      }

      if (!((codOp<=8) || (codOp>=10 && codOp<=26))) {
        //error
      }
      else {
        MV->reg[IP] += instruc.sizeA + instruc.sizeB;
        Funciones[codOp](MV,instruc);
      }

    }
    
}

void DefinoRegistro(int *CodReg, int Op){ 
  *CodReg = Op & 0x1F;
}// Devuelve codigo de registro

void DefinoAuxRegistro(int *AuxR,TVM VM, int CodReg){
  *AuxR = VM.reg[CodReg];
}

/*
MAR   
0004 
cantidad - fisica(base fiscia(DS) + offset(valor de op1))
LAR
logica(base del ds) - nuevo offsetr
MBR
valor a cargar(el del op2) 

*/

void escribeMemoria(TVM * MV,int dirLogica, itn valor, itn size) {
int dirFis;

     // 1. Cargar LAR
    VM->reg[LAR] = dirLogica;

    // 2. Traducir dirección lógica a física
    int dirFis = direccionamiento_logtofis(*VM, dirLogica);

    // 3. Cargar MAR (parte alta: size, parte baja: dirección física)
    VM->reg[MAR] = (size << 16) | (dirFis & 0xFFFF);

    // 4. Cargar en MBR
    VM->reg[MBR] = valor;

    // 5. Escribir en memoria (big-endian: byte más significativo primero)
    for (int i = 0; i < size; i++) {
        VM->memory[dirFis + (size - 1 - i)] = (valor >> (8 * i)) & 0xFF;

}

uint32_t leerMemoria(TVM *VM, int dirLogica, int size) {
    // 1. Cargar LAR (dirección lógica completa: segmento + offset)
    VM->reg[LAR] = dirLogica;

    // 2. Traducir dirección lógica a física
    int dirFis = direccionamiento_logtofis(*VM, dirLogica);

    // 3. Cargar MAR (parte alta: size, parte baja: dirección física)
    VM->reg[MAR] = (size << 16) | (dirFis & 0xFFFF);

    // 4. Leer memoria → acumular en valor
    int valor = 0;
    for (int i = 0; i < size; i++) {
        valor = (valor << 8) | (VM->memory[dirFis + i] & 0xFF);
    }

    // 5. Guardar en MBR
    VM->reg[MBR] = valor;

    return valor;
}

void MOV(TVM * VM,Insruccion instruc) {

  int valor,codReg;
  //MOV A,B;
  switch(instruc.sizeB) { 
    case 2: valor = intruc.valorB;
            break;
    case 1: DefinoRegistro(&codReg,intruc.valorB);
            valor = VM->reg[codReg];
            break;
    case 3: valor = leeMemoria();
            break;
  }   

  switch (instruc.sizeA) {
     case 1: DefinoRegistro(&codReg,intruc.valorA);
            VM->reg[codReg]=valor;
            break;
     case 3: escribeMemoria(VM,instruc.valorA,valor,4);
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
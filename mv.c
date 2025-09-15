#include <stdio.h>
#include <stdint.h>
#include "mv.h"


void declaraFunciones(vFunciones Funciones[]){//declara las funciones, cuando haga funciones[0] se ejecuta el sys
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
    /*aca me conviene un vector con cs yaads, cargando parte alta y parte baja
    o un vector de registros (base y tamanio) y en cada uno guardo el valor
    */
    VM->segmentos[DS] = (tam << 16) | (MEMORY_SIZE - tam); //los primeros 2 bytes son la base y los otros dos el tam
}

void leoArch(TVM * VM,char nombreacrh[]) {
    FILE * archb;
    THeader header;
    char t1,t2;
    int i=0;

    if(archb = fopen("prueba.vmx","rb")==NULL)
        printf("No se pudo abrir el archivo .asm");
    else {
        fread(&header.id,sizeof(char),5,archb);
        fread(&header.version,sizeof(char),2,archb);
        //como se lee el tamanio??

        //VMX25 1 tamanio

        fread(&t1,sizeof(char),1,archb); //2D necesito cargar 8 y 8
        //c celda es de 32 bits, almaceno 002D, 0000 0000 0010
        fread(&t2,sizeof(char),1,archb);
        header.tam = t1<<8 | t2; //002D
        //pregutnar

        if (strcmp(header.id, "VMX25")) {
            if (header.version == "1") {
                cargaSegmentos(VM,header.tam);
                iniciaRegs(VM,header.tam);
                //carga memoria

                while(fread(&(VM->memory[i]),1,1,archb)==1) {
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

int direccionamiento_logtofis(TVM * VM, int puntero){ //usar un parametro size si no hay que leer 4
    int DirBase, Offset, DirFisica, TamSeg, LimiteSup;
    int indicesegmento;

    indicesegmento = (puntero & 0xFFFF0000) >>16;

    DirBase = getBase(VM->segmentos[indicesegmento]);
    Offset = puntero & 0x0000FFFF;

    DirFisica = DirBase + Offset;
    TamSeg = getTam(VM->segmentos[indicesegmento]);

    // 5. Límite superior (última celda válida)
    LimiteSup = DirBase + TamSeg;

    if (!( (DirBase <= DirFisica) && (DirFisica + 4 <= LimiteSup) )) { //dir fisica + 4 ????
        generaerror(2);   // fallo de segmento
        return -1;        // nunca llega si generaerror aborta
    } else {
        return DirFisica; // dirección física válida
    }
}

void ComponentesInstruccion(TVM * VM, int DirFisica, Instruccion *instr, int *CantOp, unsigned char *CodOp){
   unsigned char Instruccion = VM->memory[DirFisica];

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
/*
void SeteoValorOp(TVM * VM,int DirFisicaActual,Instruccion *instr){
    instr->valorA = 0;
    instr->valorB = 0;

    for (int i=0;i<instr->sizeB;i++){
        instr->valorB += VM->memory[++DirFisicaActual];
        if ((instr->sizeB-i) > 1)
            instr->valorB = instr->valorB << 8;
    }

    for (int i=0;i<instr->op1;i++){
        instr->valorA += VM.memory[++DirFisicaActual];
        if ((instr->sizeA-i) > 1)
            instr->valorA = instr->valorA << 8;
    }
}
*/

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



void leeIP(TVM * VM) {
    int cantOp,DirFisicaActual,indiceseg;
    unsigned char codOp;
    Instruccion instruc;
    vFunciones Funciones;

    declaraFunciones(Funciones);
    indiceseg = (VM->reg[CS]>>16);
    //[>>16 porque vas a donde seta el segmento par aconseguir la base y el tamanio]

    while (VM->reg[IP] < getTam(indiceseg)+getBase(indiceseg)) { //
      DirFisicaActual = direccionamiento_logtofis(VM,VM->reg[IP]);

      ComponentesInstruccion(VM,DirFisicaActual,&instruc,&cantOp,&codOp);

      if (cantOp > 0) {
        SeteoValorOp(VM,DirFisicaActual,&instruc);
      }
      else {
          instruc.valorA = 0;
          instruc.valorB = 0;
      }

      if (!((codOp<=8) || (codOp>=10 && codOp<=26))) {
        //error
      }
      else {
        VM->reg[IP] += instruc.sizeA + instruc.sizeB;
        Funciones[codOp](VM,instruc);
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

void escribeMemoria(TVM * VM,int dirLogica, int valor, int size) {
int dirFis;

     // 1. Cargar LAR
    VM->reg[LAR] = dirLogica;

    // 2. Traducir dirección lógica a física
    dirFis = direccionamiento_logtofis(VM, dirLogica);

    // 3. Cargar MAR (parte alta: size, parte baja: dirección física)
    VM->reg[MAR] = (size << 16) | (dirFis & 0xFFFF);

    // 4. Cargar en MBR
    VM->reg[MBR] = valor;

    // 5. Escribir en memoria (big-endian: byte más significativo primero)
    for (int i = 0; i < size; i++) {
        VM->memory[dirFis + (size - 1 - i)] = (valor >> (8 * i)) & 0xFF;

}

}
int leerMemoria(TVM *VM, int dirLogica, int size){
    // 1. Cargar LAR (dirección lógica completa: segmento + offset)
    VM->reg[LAR] = dirLogica;

    // 2. Traducir dirección lógica a física
    int dirFis = direccionamiento_logtofis(VM, dirLogica);

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

void actualizaCC(TVM *VM, int resultado) {
    VM->reg[CC] = 0;
    if (resultado == 0) VM->reg[CC] |= 0x40000000; /* Z */
    if (resultado < 0)  VM->reg[CC] |= 0x80000000; /* N */
}


int guardaB(TVM *VM, Instruccion instruc) {
    int valorB = 0, codReg;

    switch (instruc.sizeB) {
        case 2: // inmediato
            valorB = instruc.valorB;
            break;

        case 1: // registro
            DefinoRegistro(&codReg, instruc.valorB);
            valorB = VM->reg[codReg];
            break;

        case 3: // memoria
            valorB = leerMemoria(VM, instruc.valorB, 4);
            break;

        default:
            // No debería pasar
            valorB = 0;
            break;
    }

    return valorB;
}


void MOV(TVM * VM,Instruccion instruc) {

  int valor,codReg;
  //MOV A,B;

  valor = guardaB(VM,instruc);

  switch (instruc.sizeA) {
     case 1: DefinoRegistro(&codReg,instruc.valorA);
            VM->reg[codReg]=valor;
            break;
     case 3: escribeMemoria(VM,instruc.valorA,valor,4);
            break;
 }
}

void ADD(TVM *VM, Instruccion instruc) {
    int valorA = 0, valorB = 0, codReg;
    int resultado = 0;

    valorB = guardaB(VM,instruc);
    switch (instruc.sizeA) {
        case 1:
            DefinoRegistro(&codReg, instruc.valorA);
            valorA = VM->reg[codReg];
            break;
        case 3: 
            valorA = leerMemoria(VM, instruc.valorA, 4);
            break;
    }
    resultado = valorA + valorB;

    //guardar en A
    switch (instruc.sizeA) {
        case 1: //reg
            DefinoRegistro(&codReg, instruc.valorA);
            VM->reg[codReg] = resultado;
            break;
        case 3: //memoria
            escribeMemoria(VM, instruc.valorA, resultado, 4);
            break;
    }
    actualizaCC(VM, resultado);
}

void SUB(TVM *VM, Instruccion instruc) {
    int valorA = 0, valorB = 0, resultado = 0, codReg;

    //valor b
    valorB = guardaB(VM, instruc);

    //obtiene valor de a
    switch (instruc.sizeA) {
        case 1: // registro
            DefinoRegistro(&codReg, instruc.valorA);
            valorA = VM->reg[codReg];
            break;

        case 3: // memoria
            valorA = leerMemoria(VM, instruc.valorA, 4);
            break;
    }

    resultado = valorA - valorB;

    actualizaCC(VM, resultado);

    //guarda
    switch (instruc.sizeA) {
        case 1: // registro
            DefinoRegistro(&codReg, instruc.valorA);
            VM->reg[codReg] = resultado;
            break;

        case 3: // memoria
            escribeMemoria(VM, instruc.valorA, resultado, 4);
            break;
    }
}

void MUL(TVM *VM, Instruccion instruc) {
    int valorA = 0, valorB = 0, resultado = 0, codReg;

    valorB = guardaB(VM, instruc);

    switch (instruc.sizeA) {
        case 1:
            DefinoRegistro(&codReg, instruc.valorA);
            valorA = VM->reg[codReg];
            break;
        case 3:
            valorA = leerMemoria(VM, instruc.valorA, 4);
            break;
    }

    resultado = valorA * valorB;

    actualizaCC(VM, resultado);

    switch (instruc.sizeA) {
        case 1:
            DefinoRegistro(&codReg, instruc.valorA);
            VM->reg[codReg] = resultado;
            break;
        case 3:
            escribeMemoria(VM, instruc.valorA, resultado, 4);
            break;
    }
}

void DIV(TVM *VM, Instruccion instruc) {
    int valorA = 0, valorB = 0, cociente = 0, resto = 0, codReg;

    valorB = guardaB(VM, instruc);

    switch (instruc.sizeA) {
        case 1:
            DefinoRegistro(&codReg, instruc.valorA);
            valorA = VM->reg[codReg];
            break;
        case 3:
            valorA = leerMemoria(VM, instruc.valorA, 4);
            break;
    }
    if (valorB!= 0) {
      cociente = valorA / valorB;
    else 
      //genera error 3
    }
    resto = valorA % valorB;

    actualizaCC(VM, cociente);

    // Guardar cociente en A
    switch (instruc.sizeA) {
        case 1:
            DefinoRegistro(&codReg, instruc.valorA);
            VM->reg[codReg] = cociente;
            break;
        case 3:
            escribeMemoria(VM, instruc.valorA, cociente, 4);
            break;
    }

    // Guardar resto en AC
    VM->reg[AC] = resto;
}

void CMP(TVM *VM, Instruccion instruc) {
    int valorA = 0, valorB = 0, resultado = 0, codReg;

    valorB = guardaB(VM, instruc);  
    switch (instruc.sizeA) {
        case 1: // registro
            DefinoRegistro(&codReg, instruc.valorA);
            valorA = VM->reg[codReg];
            break;

        case 3: // memoria
            valorA = leerMemoria(VM, instruc.valorA, 4);
            break;
    }

    resultado = valorA - valorB;
    actualizaCC(VM, resultado);

    // no guarda resultado
}

void SHL(TVM *VM, Instruccion instruc) {
    int valorA = 0, desplazamientos = 0, resultado = 0, codReg;

    // 
    desplazamientos = guardaB(VM, instruc);

    switch (instruc.sizeA) {
        case 1:
            DefinoRegistro(&codReg, instruc.valorA);
            valorA = VM->reg[codReg];
            break;
        case 3:
            valorA = leerMemoria(VM, instruc.valorA, 4);
            break;
    }

    resultado = valorA << desplazamientos;

    actualizaCC(VM, resultado);

    switch (instruc.sizeA) {
        case 1:
            VM->reg[codReg] = resultado;
            break;
        case 3:
            escribeMemoria(VM, instruc.valorA, resultado, 4);
            break;
    }
}

void SHR(TVM *VM, Instruccion instruc) {
    int valorA = 0, desplazamientos = 0, resultado = 0, codReg;

    desplazamientos = guardaB(VM, instruc);

    switch (instruc.sizeA) {
        case 1:
            DefinoRegistro(&codReg, instruc.valorA);
            valorA = VM->reg[codReg];
            break;
        case 3:
            valorA = leerMemoria(VM, instruc.valorA, 4);
            break;
    }

    resultado = (unsigned int)valorA >> desplazamientos; // corrimiento lógico

    actualizaCC(VM, resultado);

    switch (instruc.sizeA) {
        case 1:
            VM->reg[codReg] = resultado;
            break;
        case 3:
            escribeMemoria(VM, instruc.valorA, resultado, 4);
            break;
    }
}

void SAR(TVM *VM, Instruccion instruc) {
    int valorA = 0, desplazamientos = 0, resultado = 0, codReg;

    desplazamientos = guardaB(VM, instruc);

    //valor a desplazar
    switch (instruc.sizeA) {
        case 1:
            DefinoRegistro(&codReg, instruc.valorA);
            valorA = VM->reg[codReg];
            break;
        case 3:
            valorA = leerMemoria(VM, instruc.valorA, 4);
            break;
    }

    resultado = valorA >> desplazamientos; // corrimiento aritmético (mantiene signo en int)

    actualizaCC(VM, resultado);

    switch (instruc.sizeA) {
        case 1:
            VM->reg[codReg] = resultado;
            break;
        case 3:
            escribeMemoria(VM, instruc.valorA, resultado, 4);
            break;
    }
}

void AND(TVM *VM, Instruccion instruc) {
    int codReg, resultado, valorA, valorB;

    // 1. Leer operandos
    valorB = guardaB(VM, instruc);

    // --- Obtener OpA ---
    switch (instruc.sizeA) {
        case 1: // registro
            DefinoRegistro(&codReg, instruc.valorA);
            valorA = VM->reg[codReg];
            break;

        case 3: // memoria
            valorA = leerMemoria(VM, instruc.valorA, 4);
            break;

        default:
            valorA = 0;
            break;
    }

    // 2. Ejecutar AND
    resultado = valorA & valorB;

    // 3. Guardar resultado en OpA
    switch (instruc.sizeA) {
        case 1: // registro
            VM->reg[codReg] = resultado;
            break;

        case 3: // memoria
            escribeMemoria(VM, instruc.valorA, resultado, 4);
            break;
    }

    // 4. Actualizar CC
    actualizaCC(VM, resultado);
}


void OR(TVM *VM, Instruccion instruc) {
    int codReg, resultado, valorA, valorB;

    // 1. Leer operandos
    valorB = guardaB(VM, instruc);

    // --- Obtener OpA ---
    switch (instruc.sizeA) {
        case 1: // registro
            DefinoRegistro(&codReg, instruc.valorA);
            valorA = VM->reg[codReg];
            break;

        case 3: // memoria
            valorA = leerMemoria(VM, instruc.valorA, 4);
            break;

        default:
            valorA = 0;
            break;
    }

    // 2. Ejecutar OR
    resultado = valorA | valorB;

    // 3. Guardar resultado en OpA
    switch (instruc.sizeA) {
        case 1: // registro
            VM->reg[codReg] = resultado;
            break;

        case 3: // memoria
            escribeMemoria(VM, instruc.valorA, resultado, 4);
            break;
    }

    // 4. Actualizar CC
    actualizaCC(VM, resultado);
}

void XOR(TVM *VM, Instruccion instruc) {
    int codReg, resultado, valorA, valorB;

    // 1. Leer operandos
    valorB = guardaB(VM, instruc);

    // --- Obtener OpA ---
    switch (instruc.sizeA) {
        case 1: // registro
            DefinoRegistro(&codReg, instruc.valorA);
            valorA = VM->reg[codReg];
            break;

        case 3: // memoria
            valorA = leerMemoria(VM, instruc.valorA, 4);
            break;

        default:
            valorA = 0;
            break;
    }

    // 2. Ejecutar XOR
    resultado = valorA ^ valorB;

    // 3. Guardar resultado en OpA
    switch (instruc.sizeA) {
        case 1: // registro
            VM->reg[codReg] = resultado;
            break;

        case 3: // memoria
            escribeMemoria(VM, instruc.valorA, resultado, 4);
            break;
    }

    // 4. Actualizar CC
    actualizaCC(VM, resultado);
}

void SWAP(TVM *VM, Instruccion instruc) {
    int codRegA, codRegB;
    int valorA = 0, valorB = 0, temp;

    // --- Obtener OpA ---
    switch (instruc.sizeA) {
        case 1: // registro
            DefinoRegistro(&codRegA, instruc.valorA);
            valorA = VM->reg[codRegA];
            break;
        case 3: // memoria
            valorA = leerMemoria(VM, instruc.valorA, 4);
            break;
    }

    // --- Obtener OpB con guardaB ---
    valorB = guardaB(VM, instruc);

    // --- Intercambiar ---
    temp   = valorA;
    valorA = valorB;
    valorB = temp;

    // --- Guardar OpA ---
    switch (instruc.sizeA) {
        case 1: VM->reg[codRegA] = valorA; break;
        case 3: escribeMemoria(VM, instruc.valorA, valorA, 4); break;
    }

    // --- Guardar OpB ---
    switch (instruc.sizeB) {
        case 1: // registro
            DefinoRegistro(&codRegB, instruc.valorB);
            VM->reg[codRegB] = valorB;
            break;

        case 3: // memoria
            escribeMemoria(VM, instruc.valorB, valorB, 4);
            break;

        //case 2:  inmediato
            // no se puede swap con inmediato, hay que generar error, 
            // pero como no se pide, nose si hacerlo.
        //    return;
    }

    // --- Actualizar CC ---
    actualizaCC(VM, valorA);
}

void LDL(TVM *VM, Instruccion instruc) {
    int codReg, valorB, resultado;

    // --- OpA (destino) debe ser registro?? o puede ser de memoria tambien?? ---
    DefinoRegistro(&codReg, instruc.valorA);

    // --- OpB ---
    valorB = guardaB(VM, instruc);

    // --- LDL: reemplaza los 16 bits bajos ---
    resultado = (VM->reg[codReg] & 0xFFFF0000) | (valorB & 0xFFFF);

    // --- Guardar ---
    VM->reg[codReg] = resultado;

    // --- Actualizar banderas ---
    actualizaCC(VM, resultado);
}

void LDH(TVM *VM, Instruccion instruc) {
    int codReg, valorB, resultado;

    // --- OpA (destino) ---
    DefinoRegistro(&codReg, instruc.valorA);

    // --- OpB ---
    valorB = guardaB(VM, instruc);

    // --- LDH: reemplaza los 16 bits altos ---
    resultado = (VM->reg[codReg] & 0x0000FFFF) | ((valorB & 0xFFFF) << 16);

    // --- Guardar ---
    VM->reg[codReg] = resultado;

    // --- Actualizar banderas ---
    actualizaCC(VM, resultado);
}










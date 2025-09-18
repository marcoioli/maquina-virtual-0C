#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "mv.h"

void inicializoVecFunciones(char VecFunciones[CANTFUNC][5]){
    //2 Operandos
    strcpy(VecFunciones[0x10], "MOV");
    strcpy(VecFunciones[0x11], "ADD");
    strcpy(VecFunciones[0x12], "SUB");
    strcpy(VecFunciones[0x13], "MUL");
    strcpy(VecFunciones[0x14], "DIV");
    strcpy(VecFunciones[0x15], "CMP");
    strcpy(VecFunciones[0x16], "SHL");
    strcpy(VecFunciones[0x17], "SHR");
    strcpy(VecFunciones[0x18], "SAR");
    strcpy(VecFunciones[0x19], "AND");
    strcpy(VecFunciones[0x1A], "OR");
    strcpy(VecFunciones[0x1B], "XOR");
    strcpy(VecFunciones[0x1C], "SWAP");
    strcpy(VecFunciones[0x1D], "LDL");
    strcpy(VecFunciones[0x1E], "LDH");
    strcpy(VecFunciones[0x1F], "RND");

    //1 Operando
    strcpy(VecFunciones[0x00], "SYS");
    strcpy(VecFunciones[0x02], "JZ");
    strcpy(VecFunciones[0x03], "JP");
    strcpy(VecFunciones[0x01], "JMP");
    strcpy(VecFunciones[0x04], "JN");
    strcpy(VecFunciones[0x05], "JNZ");
    strcpy(VecFunciones[0x06], "JNP");
    strcpy(VecFunciones[0x07], "JNN");
    strcpy(VecFunciones[0x08], "NOT");

    //0 Operandos
    strcpy(VecFunciones[0x0F], "STOP");

}

void inicializoVecRegistros(char VecRegistros[CANTREG][4]){
    strcpy(VecRegistros[LAR],"LAR");
    strcpy(VecRegistros[MAR],"MAR");
    strcpy(VecRegistros[MBR],"MBR");
    strcpy(VecRegistros[IP], "IP");
    strcpy(VecRegistros[OPC], "OPC");
    strcpy(VecRegistros[OP1], "OP1");
    strcpy(VecRegistros[OP2], "OP2");
    strcpy(VecRegistros[7], "");
    strcpy(VecRegistros[8], "");
    strcpy(VecRegistros[9], "");
    strcpy(VecRegistros[EAX], "EAX");
    strcpy(VecRegistros[EBX], "EBX");
    strcpy(VecRegistros[ECX], "ECX");
    strcpy(VecRegistros[EDX], "EDX");
    strcpy(VecRegistros[EEX], "EEX");
    strcpy(VecRegistros[EFX], "EFX");
    strcpy(VecRegistros[CC], "CC");
    strcpy(VecRegistros[AC], "AC");
    strcpy(VecRegistros[CS], "CS");
    strcpy(VecRegistros[DS], "DS");
}


void declaraFunciones(vFunciones Funciones){//declara las funciones, cuando haga funciones[0] se ejecuta el sys
 // Instrucciones sin operandos
    //Funciones[0x0F] = STOP;  // No implementada aún
    
    // Instrucciones con un operando  
    Funciones[0x00] = SYS;   
    Funciones[0x01] = JMP;  
    Funciones[0x02] = JZ;    
    Funciones[0x03] = JP;
    Funciones[0x04] = JN;
    Funciones[0x05] = JNZ;
    Funciones[0x06] = JNP;
    Funciones[0x07] = JNN;
    Funciones[0x08] = NOT;   
    
    // Instrucciones con dos operandos 
    Funciones[0x10] = MOV;   // 16 decimal
    Funciones[0x11] = ADD;   // 17 decimal  
    Funciones[0x12] = SUB;   // 18 decimal
    Funciones[0x13] = MUL;   // 19 decimal
    Funciones[0x14] = DIV;   // 20 decimal
    Funciones[0x15] = CMP;   // 21 decimal
    Funciones[0x16] = SHL;   // 22 decimal
    Funciones[0x17] = SHR;   // 23 decimal
    Funciones[0x18] = SAR;   // 24 decimal
    Funciones[0x19] = AND;   // 25 decimal
    Funciones[0x1A] = OR;    // 26 decimal
    Funciones[0x1B] = XOR;   // 27 decimal
    Funciones[0x1C] = SWAP;  // 28 decimal
    Funciones[0x1D] = LDL;   // 29 decimal
    Funciones[0x1E] = LDH;   // 30 decimal
    Funciones[0x1F] = RND;   
}

void generaerror(int codigo) {
    switch (codigo) {
        case ERROR_INSTRUCCION:
            printf("[ERROR] Instrucción inválida\n");
            break;
        case ERROR_DIVISION_POR_CERO:
            printf("[ERROR] División por cero\n");
            break;
        case ERROR_SEGMENTO:
            printf("[ERROR] Fuera de los limites del segmento\n");
            break;
        case ERROR_OPERANDO:
            printf("[ERROR] Operando inválido\n");
            break;
    }
    exit(EXIT_FAILURE); // Aborta la ejecución
}

void iniciaRegs(TVM * VM,int tam) {
    VM->reg[CS] = 0x00000000;
    VM->reg[DS] = (1<<16) | 0; //tamanio??
    VM->reg[IP] = VM->reg[CS];
}

void cargaSegmentos(TVM * VM,int tam) {
    VM->segmentos[SEG_CS] = (0 << 16) | tam; 
    /*aca me conviene un vector con cs yaads, cargando parte alta y parte baja
    o un vector de registros (base y tamanio) y en cada uno guardo el valor
    */
    VM->segmentos[SEG_DS] = (tam << 16) | (MEMORY_SIZE - tam); //los primeros 2 bytes son la base y los otros dos el tam
}

void leoArch(TVM * VM, char nomarch[]) {
    FILE * archb;
    THeader header;
    char t1,t2;
    int i=0;

    archb = fopen(nomarch,"rb");
    if(archb==NULL)
        printf("No se pudo abrir el archivo .asm");
    else {
        fread(&header.id,sizeof(char),5,archb);
        fread(&header.version,sizeof(char),1,archb);
        //como se lee el tamanio??

        //VMX25 1 tamanio

        fread(&t1,sizeof(char),1,archb); //2D necesito cargar 8 y 8
        //c celda es de 32 bits, almaceno 002D, 0000 0000 0010
        fread(&t2,sizeof(char),1,archb);
        header.tam = t1<<8 | t2; //002D
        //pregutnar

        printf("Header: id=%s, version=%d, tam=%d\n", header.id, header.version, header.tam);

        //AGREGAR VALIDACIONES DE HEADER
                cargaSegmentos(VM,header.tam);
                printf("Segmentos: CS=%08X, DS=%08X\n", VM->segmentos[CS], VM->segmentos[DS]);
                iniciaRegs(VM,header.tam);
                printf("Registros iniciales: CS=%08X, DS=%08X, IP=%08X\n", VM->reg[CS], VM->reg[DS], VM->reg[IP]);
                //carga memoria

                while(fread(&(VM->memory[i]),1,1,archb)==1) {
                     //lee de a 1 byte y carga la memoria con todo el codigo del .asm
                    i++;
                }
            printf("Contenido de memoria (primeros 64 bytes):\n");
            for (int j = 0; j < 64 && j < i; j++) {
             printf("%02X ", VM->memory[j]);
             if ((j+1) % 16 == 0) printf("\n");
}
        printf("\n");
        fclose(archb);

    }

}

int getBase(int valor) {
  return (valor & 0xFFFF0000)>>16;
}

int getTam(int valor) {
  return (valor & 0x0000FFFF);
}

int getDirfisica(TVM *VM, int offset,int segmento, int size) {

    int base,tam;

    if (segmento > CANTSEGMENTOS) {
        generaerror(ERROR_SEGMENTO);
    }
    else {
        base = (VM->segmentos[segmento] & 0xFFFF0000) >> 16; //base del segmento 
        tam = (VM->segmentos[segmento]&0x0000FFFF);
        int dirFisica = base+offset;
        if (dirFisica >= base && dirFisica < tam ) //+ inicial
            return dirFisica;
        else {
            printf("[ERROR] ACCEDIENDO A DIRECCION MENOR A LA BASE O MAYOR AL TAMANIO \n");
            printf("DIRFISICA %08X, BASE=%08X, TAMANIO= %08X \n",dirFisica, base, tam);
            return -1;
        }
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

/*void SeteoValorOp(TVM * VM, int dirFisicaActual, Instruccion *instr) {
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

    void SeteoValorOp(TVM * VM, int dirFisicaActual, Instruccion *instr) {
    instr->valorA = 0;
    instr->valorB = 0;

    // --- Operando B ---
    for (int i = 0; i < instr->sizeB; i++) {
        instr->valorB = (instr->valorB << 8) | VM->memory[++dirFisicaActual];
    }
    // Propagación de signo para inmediatos (sizeB == 2)
    if (instr->sizeB == 2) {
        int bytes = instr->sizeB;
        int msb = 1 << ((bytes * 8) - 1);
        if (instr->valorB & msb) {
            instr->valorB |= ~((1 << (bytes * 8)) - 1); // Extiende el signo a 32 bits
        }
    }

    // --- Operando A ---
    for (int i = 0; i < instr->sizeA; i++) {
        instr->valorA = (instr->valorA << 8) | VM->memory[++dirFisicaActual];
    }
    // Propagación de signo para inmediatos (sizeA == 2)
    if (instr->sizeA == 2) {
        int bytes = instr->sizeA;
        int msb = 1 << ((bytes * 8) - 1);
        if (instr->valorA & msb) {
            instr->valorA |= ~((1 << (bytes * 8)) - 1); // Extiende el signo a 32 bits
        }
    }
}

void leeIP(TVM *VM) {
    int segIndex, base, size;
    int offset, dirFisica;
    int cantOp;
    unsigned char codOp;
    Instruccion instruc;
    vFunciones Funciones;
    int ejecutando = 1;  // bandera de control

    declaraFunciones(Funciones);

    // El segmento de código siempre está en el índice guardado en CS
    segIndex = SEG_CS; //cambiar segun funcion log-to fis
    base = getBase(VM->segmentos[segIndex]);
    size = getTam(VM->segmentos[segIndex]);

    printf("[DEBUG] Inicio ejecución | segIndex=%d base=%d size=%d\n ac=%d", segIndex, base, size,VM->reg[AC]);
    printf("[SEGMENTOS] CODE SEGMENT %08X -- DATA SEGMENT %08X \n",VM->segmentos[SEG_CS],VM->segmentos[SEG_DS]);
    printf("VALORES INICIALES CS=%08X DS=%08X IP=%08X \n",VM->reg[CS],VM->reg[DS],VM->reg[IP]);

    // Ciclo de ejecución
    while (ejecutando) {
        offset = VM->reg[IP] & 0xFFFF;

        // Validar que no nos pasamos del segmento de código
        if (offset >= size) {
            printf("[STOP] IP fuera de rango (offset=%d, size=%d)\n", offset, size);
            ejecutando = 0; 
        } else {
            // Dirección física = base + offset
            dirFisica = base + offset; //base del code segment y offset de ip
            unsigned char rawInstr = VM->memory[dirFisica];
            ComponentesInstruccion(VM, dirFisica, &instruc, &cantOp, &codOp);

            printf("[FETCH] IP=%08X | DirFisica=%04X | rawInstr=%02X | codOp=%02X | sizeA=%d sizeB=%d cantOp=%d AC=%d\n",
                   VM->reg[IP], dirFisica, rawInstr, codOp, instruc.sizeA, instruc.sizeB, cantOp,VM->reg[AC]);

            // lee operandos
            if (cantOp > 0) {
                SeteoValorOp(VM, dirFisica, &instruc);
            } else {
                instruc.valorA = 0;
                instruc.valorB = 0;
            }

            // Guardar en registros op
            VM->reg[OPC] = codOp;
            VM->reg[OP1] = (instruc.sizeA << 24) | (instruc.valorA & 0x00FFFFFF);
            VM->reg[OP2] = (instruc.sizeB << 24) | (instruc.valorB & 0x00FFFFFF);

            printf("OPC = %08X \n",VM->reg[OPC]);
            printf("OP1 = %08X \n",VM->reg[OP1]);
            printf("OP2 = %08X \n",VM->reg[OP2]);


            // hace la funcion
            if (!((codOp <= 0x08) || (codOp >= 0x10 && codOp <= 0x1F))) {
                printf("[ERROR] Código de operación inválido: %02X\n", codOp);
                ejecutando = 0;  // fin por error
            } else if (Funciones[codOp] != NULL) {
                printf("[EXECUTE] Ejecutando opcode %02X...\n", codOp);
                Funciones[codOp](VM, instruc);
            } else {
                printf("[WARNING] Instrucción %02X no implementada\n", codOp);
            }

            // actualiza ip
            VM->reg[IP] += 1 + instruc.sizeA + instruc.sizeB;

            // Condición de parada por STOP
            if (VM->reg[IP] == -1) {
                printf("[STOP] Ejecución finalizada por instrucción STOP\n");
                ejecutando = 0;
            }

            // Debug de registros después de ejecutar
            printf("[DEBUG] Regs: EAX=%08X EBX=%08X ECX=%08X EDX=%08X AC=%08X CC=%08X IP=%08X\n",
                   VM->reg[EAX], VM->reg[EBX], VM->reg[ECX], VM->reg[EDX],
                   VM->reg[AC], VM->reg[CC], VM->reg[IP]);
            printf("\n");
        }
    }
}

void DefinoRegistro(int *CodReg, int Op){
  *CodReg = Op & 0x1F;
}// Devuelve codigo de registro


/*
MAR
0004
cantidad - fisica(base fiscia(DS) + offset(valor de op1))
LAR
logica(base del ds) - nuevo offsetr
MBR
valor a cargar(el del op2)
*/

void escribeMemoria(TVM * VM,int OP,int valor, int size) {
    
    int regact,csact,offreg,offop,offset;
    
    regact = (OP & 0x00FF0000) >> 16; //registro actual recibido a modificar
    csact = (VM->reg[regact] & 0xFFFF0000) >> 16; //codigo de segmento de op recibido
    offop = OP & 0x0000FFFF; //offset del op recibido
    offreg = (VM->reg[regact] & 0x0000FFFF); //offset del registro 
    offset = offreg + offop;
    VM->reg[LAR] = csact << 16 | offset;

    // 2. Traducir dirección lógica a física
    int dirFis = getDirfisica(VM, offset,csact, 4); //dir fiscia va a ser base segmento + offset

    // 3. Cargar MAR (parte alta: size, parte baja: dirección física)
    VM->reg[MAR] = (size << 16) | (dirFis & 0xFFFF);

    // 4. Cargar en MBR
    VM->reg[MBR] = valor;

    printf("LAR : %08X \n",VM->reg[LAR]);
    printf("MAR %08X \n",VM->reg[MAR]);
    printf("MBR %08X \n",VM->reg[MBR]);

    // 5. Escribir en memoria (big-endian: byte más significativo primero)
    for (int i = 0; i < size; i++) 
        VM->memory[dirFis + (size - 1 - i)] = (valor >> (8 * i)) & 0xFF;
}

int leerMemoria (TVM*VM, int OP,int size) {
    
    int regact,csact,offreg,offop,offset;

    regact = (OP & 0x00FF0000) >> 16; //registro actual recibido a modificar
    csact = (VM->reg[regact] & 0xFFFF0000) >> 16; //codigo de segmento de op recibido
    offop = OP & 0x0000FFFF; //offset del op recibido
    offreg = (VM->reg[regact] & 0x0000FFFF); //offset del registro 
    offset = offreg + offop;
    VM->reg[LAR] = csact << 16 | offset;

    int dirFis = getDirfisica(VM,offset,csact,4);

    VM->reg[MAR] = (size << 16) | (dirFis & 0xFFFF);

    int valorx = 0;
    for (int i = 0; i < size; i++) {
        valorx = (valorx << 8) | (VM->memory[dirFis + i] & 0xFF);
    }
    VM->reg[MBR] = valorx;

    printf("LAR  LECTURA: %08X \n",VM->reg[LAR]);
    printf("MAR LECTURA %08X \n",VM->reg[MAR]);
    printf("MBR LECTURA %08X \n",VM->reg[MBR]);
    return valorx;
}


void actualizaCC(TVM *VM, int resultado) {
    VM->reg[CC] = 0;
    if (resultado == 0) VM->reg[CC] |= 0x40000000; /* Z */
    if (resultado < 0)  VM->reg[CC] |= 0x80000000; /* N */
}


int guardaB(TVM *VM, Instruccion instruc) {
    int valorB = 0, codReg;

    printf("INTRUC B : %d",instruc.sizeB);
    printf("\n");

    switch (instruc.sizeB) {
        case 2: // inmediato
            valorB = instruc.valorB;
            break;
        case 1: // registro
            DefinoRegistro(&codReg, instruc.valorB);
            valorB = VM->reg[codReg];
            break;

        case 3: // memoria
            printf("YENDO A LEER A MEMORIA \n");
            valorB = leerMemoria(VM,VM->reg[OP2], 4);
            break;

        default:
            generaerror(ERROR_OPERANDO);
            break;
    }

    return valorB;
}


void MOV(TVM * VM,Instruccion instruc) {

  int valor,codReg;
  //MOV A,B;

  valor = guardaB(VM,instruc);

  printf("EJECUTANDO MOV \n");
  
  
  switch (instruc.sizeA) {
     case 1:
            DefinoRegistro(&codReg,instruc.valorA);
            VM->reg[codReg]=valor;
            printf("Valor : %08X CodReg : %d \n",valor,codReg);
            break;
     case 2:
            generaerror(ERROR_OPERANDO);
            break; //no puede ser inmediato
     case 3: 
            escribeMemoria(VM,VM->reg[OP1],valor,4); //valor es el valor de b a escribir en memoria
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
        case 2:
            generaerror(ERROR_OPERANDO);
            break; //no puede ser inmediato
        case 3: 
            valorA = leerMemoria(VM,VM->reg[OP1], 4);
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
            escribeMemoria(VM, VM->reg[OP1], resultado, 4);
            break;
    }
    actualizaCC(VM, resultado);
    printf("VALOR DE CC = %d \n",VM->reg[CC]);
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
        case 2:
            generaerror(ERROR_OPERANDO);
            break; //no puede ser inmediato
        case 3: // memoria
            valorA = leerMemoria(VM, VM->reg[OP1], 4);
            break;
    }

    resultado = valorA - valorB;

    actualizaCC(VM, resultado);
    printf("VALOR DE CC = %d \n",VM->reg[CC]);

    //guarda
    switch (instruc.sizeA) {
        case 1: // registro
            DefinoRegistro(&codReg, instruc.valorA);
            VM->reg[codReg] = resultado;
            break;

        case 3: // memoria
            escribeMemoria(VM,VM->reg[OP1], resultado, 4);
            break;
    }
}

void MUL(TVM *VM, Instruccion instruc) {
    int valorA = 0, valorB = 0, resultado = 0, codReg;

    valorB = guardaB(VM, instruc);

    printf("EJECUTANDO MUL \n");

    switch (instruc.sizeA) {
        case 1:
            DefinoRegistro(&codReg, instruc.valorA);
            valorA = VM->reg[codReg];
            break;
        case 2:
            generaerror(ERROR_OPERANDO);
            break; //no puede ser inmediato
        case 3:
            printf("YENDO A LEER A MEMORIA LO QUE HAY EN OP A \n");
            printf("INSTRUC VALOR A :%08X \n",instruc.valorA);
            valorA = leerMemoria(VM, VM->reg[OP1], 4);
            break;
    }

    resultado = valorA * valorB;

    actualizaCC(VM, resultado);
    printf("VALOR DE CC = %d \n",VM->reg[CC]);

    switch (instruc.sizeA) {
        case 1:
            DefinoRegistro(&codReg, instruc.valorA);
            VM->reg[codReg] = resultado;
            break;
        case 3:
            printf("YENDO A ESCRIBIR EN MEMORIA \n");
            escribeMemoria(VM,VM->reg[OP1], resultado, 4);
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
        case 2:
            generaerror(ERROR_OPERANDO);
            break; //no puede ser inmediato
        case 3:
            valorA = leerMemoria(VM, VM->reg[OP1], 4);
            break;
    }
    if (valorB!= 0) {
      cociente = valorA / valorB;
      resto = valorA % valorB;
    } 
    else
        generaerror(ERROR_DIVISION_POR_CERO);

    actualizaCC(VM, cociente);
    printf("VALOR DE CC = %08X \n",VM->reg[CC]);

    // Guardar cociente en A
    switch (instruc.sizeA) {
        case 1:
            DefinoRegistro(&codReg, instruc.valorA);
            VM->reg[codReg] = cociente;
            break;
        case 3:
            escribeMemoria(VM,VM->reg[OP1],cociente, 4);
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
            valorA = leerMemoria(VM, VM->reg[OP1], 4);
            break;
    }

    resultado = valorA - valorB;
    actualizaCC(VM, resultado);
    printf("VALOR DE CC = %d \n",VM->reg[CC]);
    
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
        case 2:
            generaerror(ERROR_OPERANDO);
            break; //no puede ser inmediato
        case 3:
            valorA = leerMemoria(VM, VM->reg[OP1], 4);
            break;
    }

    resultado = valorA << desplazamientos;

    actualizaCC(VM, resultado);
    printf("VALOR DE CC = %d \n",VM->reg[CC]);

    switch (instruc.sizeA) {
        case 1:
            VM->reg[codReg] = resultado;
            break;
        case 3:
            escribeMemoria(VM,VM->reg[OP1], resultado, 4);
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
        case 2:
            generaerror(ERROR_OPERANDO);
            break; //no puede ser inmediato 
        case 3:
            valorA = leerMemoria(VM, VM->reg[OP1], 4);
            break;
    }

    resultado = (unsigned int)valorA >> desplazamientos; // corrimiento lógico

    actualizaCC(VM, resultado);
    printf("VALOR DE CC = %d \n",VM->reg[CC]);

    switch (instruc.sizeA) {
        case 1:
            VM->reg[codReg] = resultado;
            break;
        case 3:
            escribeMemoria(VM,VM->reg[OP1], resultado, 4);
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
        case 2:
            generaerror(ERROR_OPERANDO);
            break; //no puede ser inmediato
        case 3:
            valorA = leerMemoria(VM, VM->reg[OP1], 4);
            break;
    }

    resultado = valorA >> desplazamientos; // corrimiento aritmético (mantiene signo en int)

    actualizaCC(VM, resultado);
    printf("VALOR DE CC = %d \n",VM->reg[CC]);

    switch (instruc.sizeA) {
        case 1:
            VM->reg[codReg] = resultado;
            break;
        case 3:
            escribeMemoria(VM,VM->reg[OP1], resultado, 4);
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
        case 2:
            generaerror(ERROR_OPERANDO);
            break; //no puede ser inmediato
        case 3: // memoria
            valorA = leerMemoria(VM, VM->reg[OP1], 4);
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
            escribeMemoria(VM,VM->reg[OP1], resultado, 4);
            break;
    }

    // 4. Actualizar CC
    actualizaCC(VM, resultado);
    printf("VALOR DE CC = %d \n",VM->reg[CC]);
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
        case 2:
            generaerror(ERROR_OPERANDO);
            break; //no puede ser inmediato
        case 3: // memoria
            valorA = leerMemoria(VM,  VM->reg[OP1], 4);
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
            escribeMemoria(VM,VM->reg[OP1], resultado, 4);
            break;
    }

    // 4. Actualizar CC
    actualizaCC(VM, resultado);
    printf("VALOR DE CC = %d \n",VM->reg[CC]);
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
        case 2:
            generaerror(ERROR_OPERANDO);
            break; //no puede ser inmediato
        case 3: // memoria
            valorA = leerMemoria(VM,  VM->reg[OP1], 4);
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
            escribeMemoria(VM,VM->reg[OP1], resultado, 4);
            break;
    }

    // 4. Actualizar CC
    actualizaCC(VM, resultado);
    printf("VALOR DE CC = %d \n",VM->reg[CC]);
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
        case 2:
            generaerror(ERROR_OPERANDO);
            break; //no puede ser inmediato
        case 3: // memoria
            valorA = leerMemoria(VM, VM->reg[OP1], 4);
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
        case 3: escribeMemoria(VM,VM->reg[OP1], valorA, 4); break;
    }

    // --- Guardar OpB ---
    switch (instruc.sizeB) {
        case 1: // registro
            DefinoRegistro(&codRegB, instruc.valorB);
            VM->reg[codRegB] = valorB;
            break;

        case 3: // memoria
            escribeMemoria(VM,VM->reg[OP2], valorB, 4);
            break;

        //case 2:  inmediato
            // no se puede swap con inmediato, hay que generar error, 
            // pero como no se pide, nose si hacerlo.
        //    return;
    }

    // --- Actualizar CC ---
    actualizaCC(VM, valorA);
    printf("VALOR DE CC = %d \n",VM->reg[CC]);
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
    printf("VALOR DE CC = %d \n",VM->reg[CC]);
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
    printf("VALOR DE CC = %d \n",VM->reg[CC]);
}

int random32() {
    return ((rand() & 0xFFFF) << 16) | (rand() & 0xFFFF);
}

void RND(TVM *VM, Instruccion instruc) {
    int codReg, resultado;

    // 1. Generar número aleatorio (32 bits)
    resultado = random32();

    // 2. Guardar en destino (OpA)
    switch (instruc.sizeA) {
        case 1: // Registro
            DefinoRegistro(&codReg, instruc.valorA);
            VM->reg[codReg] = resultado;
            break;

        case 3: // Memoria
            escribeMemoria(VM,VM->reg[OP1],resultado, 4);
            break;

        //case 2: // Inmediato no permitido
          //  generaerror(6); // Error: RND destino inmediato
            //return;
    }
}

int resolverSaltoSeguro(TVM *VM, Instruccion instruc) {
    int codReg, destino;

    //Resolver operando A
    switch (instruc.sizeA) {
        case 2: // inmediato
            destino = instruc.valorA;
            break;

        case 1: // registro
            DefinoRegistro(&codReg, instruc.valorA);
            destino = VM->reg[codReg];
            break;

        case 3: // memoria
            destino = leerMemoria(VM,  VM->reg[OP1], 4);
            break;
    }

    // Validar que no sale de CS
    int segIndex = VM->reg[CS] >> 16;
    int baseCS   = getBase(VM->segmentos[segIndex]);
    int tamCS    = getTam(VM->segmentos[segIndex]);
    int limiteCS = baseCS + tamCS;

    if (destino < baseCS || destino >= limiteCS) {
        generaerror(ERROR_SEGMENTO);
    }

    return destino;
}

void JMP(TVM *VM, Instruccion instruc) {
    int destino = resolverSaltoSeguro(VM, instruc);
    if (destino != -1) VM->reg[IP] = destino;
}

void JZ(TVM *VM, Instruccion instruc) {
    if (VM->reg[CC] & 0x40000000) { // Z=1 (bit 30)
        int destino = resolverSaltoSeguro(VM, instruc);
        if (destino != -1) VM->reg[IP] = destino;
    }
}

void JNZ(TVM *VM, Instruccion instruc) {
    if (!(VM->reg[CC] & 0x40000000)) { //pregunta por el primer bit de cc
        int destino = resolverSaltoSeguro(VM, instruc);
        if (destino != -1) VM->reg[IP] = destino;
    }
}

void JN(TVM *VM, Instruccion instruc) {
    if (VM->reg[CC] & 0x80000000) { // N=1 (bit 31)
        int destino = resolverSaltoSeguro(VM, instruc);
        if (destino != -1) VM->reg[IP] = destino;
    }
}

void JNN(TVM *VM, Instruccion instruc) {
    if (!(VM->reg[CC] & 0x80000000)) {
        int destino = resolverSaltoSeguro(VM, instruc);
        if (destino != -1) VM->reg[IP] = destino;
    }
}

void JP(TVM *VM, Instruccion instruc) {
    if (!(VM->reg[CC] & 0x80000000) && !(VM->reg[CC] & 0x40000000)) {
        int destino = resolverSaltoSeguro(VM, instruc);
        if (destino != -1) VM->reg[IP] = destino;
    }
}

void JNP(TVM *VM, Instruccion instruc) {
    if ((VM->reg[CC] & 0x80000000) || (VM->reg[CC] & 0x40000000)) {
        int destino = resolverSaltoSeguro(VM, instruc);
        if (destino != -1) VM->reg[IP] = destino;
    }
}

void NOT(TVM *VM, Instruccion instruc) {
    int codReg, valor, resultado;

    switch (instruc.sizeA) {
        case 1: // Registro
            DefinoRegistro(&codReg, instruc.valorA);
            valor = VM->reg[codReg];
            resultado = ~valor;
            VM->reg[codReg] = resultado;
            break;

        case 3: // Memoria
            valor = leerMemoria(VM,  VM->reg[OP1], 4);
            resultado = ~valor;
            escribeMemoria(VM,VM->reg[OP1], resultado, 4);
            break;
        case 2:
            generaerror(ERROR_OPERANDO);
            break; //no puede ser inmediato
    }

    // Actualizar banderas
    actualizaCC(VM,resultado);
    printf("VALOR DE CC = %d \n",VM->reg[CC]);
}

/*void SYS(TVM *VM, Instruccion instruc){
    int servicio, modo, posmemoria, ecx, size, count, logica,   segmento, offset,offset_inicial;

    switch (instruc.sizeA) {
        case 2: // inmediato
            servicio = instruc.valorA;
            break;
        case 1: { // registro
            int codReg;
            DefinoRegistro(&codReg, instruc.valorA);
            servicio = VM->reg[codReg];
            break;
        }
        case 3: // memoria
            servicio = leerMemoria(VM, VM->reg[OP1], 4);
            break;
    }

    modo = VM->reg[EAX];   
    ecx = VM->reg[ECX];
    size = (ecx >> 16) & 0xFFFF; // bytes por celda
     count = ecx & 0xFFFF;     
    logica = VM->reg[EDX]; // dirección lógica del buffer
    segmento = (logica >> 16) & 0xFFFF;
    offset = (logica & 0x0000FFFF);

   if (servicio == 1) {
        for (int i = 0; i < count; i++) {
            int offset = offset + i*size;
            int dirFis = getDirfisica(VM, offset, segmento, size);
            if (dirFis == -1) return; // error de acceso

            printf("[%04X] ", dirFis & 0xFFFF);

            int numero = 0;
            if (modo == 0x10) {
                numero = leer_binario_c2_32();
            } else if (modo == 0x08) {
                scanf("%X", &numero);
            } else if (modo == 0x04) {
                scanf("%o", &numero);
            } else if (modo == 0x02) {
                char c;
                scanf(" %c", &c);
                numero = (int)c;
            } else {
                scanf("%d", &numero); // decimal
            }

            // Guardar en memoria física
            if (size == 1) {
                VM->memory[dirFis] = numero & 0xFF;
            } else if (size == 2) {
                VM->memory[dirFis]     = (numero >> 8) & 0xFF;
                VM->memory[dirFis + 1] = numero & 0xFF;
            } else if (size == 4) {
                for (int j = 3; j >= 0; j--) {
                    VM->memory[dirFis + (3-j)] = (numero >> (8*j)) & 0xFF;
                }
            }
        }
    }


else if (servicio == 2) {
    offset_inicial = offset; // <--- ¡CORRECCIÓN AQUÍ!
    for (int i = 0; i < count; i++) {
        int offset_actual = offset_inicial + i*size;
        int dirFis = getDirfisica(VM, offset_actual, segmento, size);
        if (dirFis == -1) return;

        // Reconstruir valor desde memoria física
        int valor = 0;
        for (int j = 0; j < size; j++) {
            valor = (valor << 8) | (VM->memory[dirFis + j] & 0xFF);
        }

        // Salida en formato esperado
        if (modo == 0x02) { // caracter
            unsigned char ch = valor & 0xFF;
            printf("[%04X]: 0b", dirFis & 0xFFFF);
            for (int k = 7; k >= 0; k--) {
                printf("%d", (ch >> k) & 1);
            }
            printf(" %c\n", isprint(ch) ? ch : '.');
        } else if (modo == 0x08) { // hex
            printf("[%04X] 0x%X\n", dirFis & 0xFFFF, valor);
        } else if (modo == 0x04) { // octal
            printf("[%04X] 0%o\n", dirFis & 0xFFFF, valor);
        } else if (modo == 0x10) { // binario
            printf("[%04X] ", dirFis & 0xFFFF);
            imprimir_binario_32(valor);
        } else { // decimal
            printf("[%04X] %d\n", dirFis & 0xFFFF, valor);
        }
    }
}
}
*/

void SYS(TVM *VM, Instruccion instruc) {
    int servicio = 0;
    int modo, posmemoria, ecx, size, count, logica, segmento, offset, offset_inicial;

    // Determinar servicio (SYS <n>)
    switch (instruc.sizeA) {
        case 2: // inmediato
            servicio = instruc.valorA;
            break;
        case 1: { // registro
            int codReg;
            DefinoRegistro(&codReg, instruc.valorA);
            servicio = VM->reg[codReg];
            break;
        }
        case 3: // memoria
            servicio = leerMemoria(VM, VM->reg[OP1], 4);
            break;
    }

    // Configuración desde registros
    modo = VM->reg[EAX];      // máscara de modos
    ecx = VM->reg[ECX];       // cantidad y tamaño
    size = (ecx >> 16) & 0xFFFF;  // bytes por celda
    count = ecx & 0xFFFF;        // cantidad de celdas
    logica = VM->reg[EDX];       // dirección lógica
    segmento = (logica >> 16) & 0xFFFF;
    offset = (logica & 0xFFFF);

    // ---- SERVICIO 1: READ ----
    if (servicio == 1) {
        for (int i = 0; i < count; i++) {
            int offset_actual = offset + i * size;
            int dirFis = getDirfisica(VM, offset_actual, segmento, size);
            if (dirFis == -1) return; // error de acceso

            printf("[%04X]: ", dirFis & 0xFFFF);

            int numero = 0;

            if (modo & 0x10) { // binario
                numero = leer_binario_c2_32();
            } else if (modo & 0x08) { // hex
                scanf("%X", &numero);
            } else if (modo & 0x04) { // octal
                scanf("%o", &numero);
            } else if (modo & 0x02) { // char
                char c;
                scanf(" %c", &c);
                numero = (int)c;
            } else { // decimal por defecto
                scanf("%d", &numero);
            }

            // Guardar en memoria (LITTLE ENDIAN)
           /* if (size == 1) {
                VM->memory[dirFis] = numero & 0xFF;
            } else if (size == 2) {
                VM->memory[dirFis]     = numero & 0xFF;
                VM->memory[dirFis + 1] = (numero >> 8) & 0xFF;
            } else if (size == 4) {
                for (int j = 0; j < 4; j++) {
                    VM->memory[dirFis + j] = (numero >> (8 * j)) & 0xFF;
                }
            }
           */   
           
            //guarda en LITTLE ENDIAN SIEMPRE
           for (int j = 0; j < size; j++) {
              VM->memory[dirFis + j] = (numero >> (8 * (size - 1 - j))) & 0xFF;
           }
}  
    }
    // ---- SERVICIO 2: WRITE ----
    else if (servicio == 2) {
        offset_inicial = offset;
        for (int i = 0; i < count; i++) {
            int offset_actual = offset_inicial + i * size;
            int dirFis = getDirfisica(VM, offset_actual, segmento, size);
            if (dirFis == -1) return;
            int valor=0;

            // Reconstruir valor desde memoria física (big)
                // modo por defecto: big endian (MOV, SHL, OR)
                for (int j = 0; j < size; j++) {
                 valor = (valor << 8) | (VM->memory[dirFis + j] & 0xFF);
                }

            // Imprimir en los formatos activados
            printf("[%04X]: ", dirFis & 0xFFFF);

            if (modo & 0x10) { // binario
                imprimir_binario_nbits(valor, size * 8); // usa 8, 16 o 32 bits
                printf(" ");
            }
            if (modo & 0x08) { // hex
                printf("0x%X ", valor);
            }
            if (modo & 0x04) { // octal
                printf("0o%o ", valor);
            }
            if (modo & 0x02) { // caracter
                for (int j = size - 1; j >= 0; j--) {
                unsigned char ch = (valor >> (8 * j)) & 0xFF;
                printf("%c", isprint(ch) ? ch : '.');
             }
              printf(" ");
            }
            if (modo & 0x01) { // decimal
                printf("%d ", valor);
            }

            printf("\n");
        }
    }
}


int leer_binario_c2_32() {
    char buffer[33];
    int resultado = 0;
    int signo = 1;

    scanf("%32s", buffer);

    // Si empieza con '-', es negativo
    if (buffer[0] == '-') {
        signo = -1;
        memmove(buffer, buffer + 1, strlen(buffer));
    }

    for (int i = 0; buffer[i] != '\0'; i++) {
        resultado <<= 1;
        if (buffer[i] == '1') resultado |= 1;
        else if (buffer[i] != '0') break; // Ignora caracteres no binarios
    }

    if (signo == -1) resultado = -resultado;
    return resultado;
}

void imprimir_binario_32(int valor) {
    for (int i = 31; i >= 0; i--) {
        printf("%d", (valor >> i) & 1);
        if (i % 8 == 0 && i != 0) printf(" ");
    }
    printf("\n");
}

void imprimir_binario_nbits(int valor, int bits) {
    for (int i = bits - 1; i >= 0; i--) {
        printf("%d", (valor >> i) & 1);
    }
}


// -------------------- DISASSEMBLER ------------------------//
void LeoDissasembler(TVM * VM,char VecFunciones[CANTFUNC][5],char VecRegistros[CANTREG][4]) {

    unsigned char CodOp;
    int CantOp,baseCS,tamCS;
    Instruccion instruc;
    unsigned short int PosInicial,PosMemoria,PosFinal;

    baseCS = getBase(VM->segmentos[SEG_CS]);
    tamCS = getTam(VM->segmentos[SEG_CS]);

    PosMemoria = baseCS;
    PosFinal =  baseCS + tamCS;

    while (PosMemoria < PosFinal) {

        PosInicial=PosMemoria;
        ComponentesInstruccion(VM,PosMemoria,&instruc,&CantOp,&CodOp);
        SeteoValorOp(VM,PosMemoria,&instruc);

        PosMemoria += instruc.sizeA+instruc.sizeB+1; // Posicion de la Siguiente instruccion
        EscriboDissasembler(VM,VecFunciones,VecRegistros,CodOp,instruc,PosInicial,PosMemoria);
    }
}

void EscriboDissasembler(TVM *VM, char VecFunciones[CANTFUNC][5], char VecRegistros[CANTREG][4],
                         unsigned char CodOp, Instruccion instruc, int PosInicial, int PosFinal) {
    // 1. Dirección física
    printf("[%04X] ", PosInicial);

    // 2. Bytes de la instrucción
    for (int i = PosInicial; i < PosFinal; i++) {
        printf("%02X ", VM->memory[i]);
    }
    // Completar hasta 4 bytes para alinear (opcional)
    for (int i = PosFinal; i < PosInicial + 4; i++) {
        printf("   ");
    }

    printf("| ");

    // 3. Mnemónico
    printf("%s ", VecFunciones[CodOp]);

    // 4. Operando A
    if (instruc.sizeA) {
        if (instruc.sizeA == 1) { // Registro
            int codReg;
            DefinoRegistro(&codReg, instruc.valorA);
            printf("%s", VecRegistros[codReg]);
        } else if (instruc.sizeA == 2) { // Inmediato
            printf("%d", instruc.valorA);
        } else if (instruc.sizeA == 3) { // Memoria
            int seg = (instruc.valorA & 0xFF0000) >> 16;
            int off = instruc.valorA & 0xFFFF;
            printf("[%s+%d]", seg == SEG_DS ? "DS" : "CS", off);
        }
    }

    // 5. Operando B
    if (instruc.sizeB) {
        printf(", ");
        if (instruc.sizeB == 1) { // Registro
            int codReg;
            DefinoRegistro(&codReg, instruc.valorB);
            printf("%s", VecRegistros[codReg]);
        } else if (instruc.sizeB == 2) { // Inmediato
            printf("%d", instruc.valorB);
        } else if (instruc.sizeB == 3) { // Memoria
            int seg = (instruc.valorB & 0xFF0000) >> 16;
            int off = instruc.valorB & 0xFFFF;
            printf("[%s+%d]", seg == SEG_DS ? "DS" : "CS", off);
        }
    }

    printf("\n");
}




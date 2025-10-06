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
    Funciones[0x0F] = STOP;  // No implementada aún
    
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
    VM->reg[AC] = 0;
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
    char id[6];

    unsigned char leo;

    archb = fopen(nomarch,"rb");
    if(archb==NULL)
        printf("No se pudo abrir el archivo .asm");
    else {
        fread(&header.c1,sizeof(char),1,archb);
        fread(&header.c2,sizeof(char),1,archb);
        fread(&header.c3,sizeof(char),1,archb);
        fread(&header.c4,sizeof(char),1,archb);
        fread(&header.c5,sizeof(char),1,archb);
        fread(&header.version,sizeof(char),1,archb);
        sprintf(id, "%c%c%c%c%c", header.c1, header.c2, header.c3, header.c4, header.c5);


        fread(&leo,sizeof(char),1,archb);
         header.tam=leo;
         header.tam=header.tam<<8;
         fread(&leo,sizeof(char),1,archb);
         header.tam+=leo;


         printf("id=%s version=%d, tam=%d\n",id,header.version, header.tam);

        //AGREGAR VALIDACIONES DE HEADER
         if(strcmp(id, "VMX25") == 0 && header.version ==1) {
                cargaSegmentos(VM,header.tam);
           //    printf("Segmentos: CS=%08X, DS=%08X\n", VM->segmentos[CS], VM->segmentos[DS]);
                iniciaRegs(VM,header.tam);
            //   printf("Registros iniciales: CS=%08X, DS=%08X, IP=%08X\n", VM->reg[CS], VM->reg[DS], VM->reg[IP]);
                //carga memoria

                while(fread(&(VM->memory[i]),1,1,archb)==1) {
                     //lee de a 1 byte y carga la memoria con todo el codigo del .asm
                    i++;
                }
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

    if (segmento > CANTSEGMENTOS) { // ya no es fijo crear variable segm_count al leer archivo
        generaerror(ERROR_SEGMENTO);
    }
    else {
        base = (VM->segmentos[segmento] & 0xFFFF0000) >> 16; //base del segmento 
        tam = (VM->segmentos[segmento]&0x0000FFFF);
        int dirFisica = base+offset;
        if (dirFisica >= base && (dirFisica + size - 1) < (base + tam )) //+ inicial
            return dirFisica;
        else {
       //   printf("[ERROR] ACCEDIENDO A DIRECCION MENOR A LA BASE O MAYOR AL TAMANIO \n");
        //   printf("DIRFISICA %08X, BASE=%08X, TAMANIO= %08X \n",dirFisica, base, tam);
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

    
  //printf("[DEBUG] Inicio ejecución | segIndex=%d base=%d size=%d\n ac=%d", segIndex, base, size,VM->reg[AC]);
 //   printf("[SEGMENTOS] CODE SEGMENT %08X -- DATA SEGMENT %08X \n",VM->segmentos[SEG_CS],VM->segmentos[SEG_DS]);
  // printf("VALORES INICIALES CS=%08X DS=%08X IP=%08X \n",VM->reg[CS],VM->reg[DS],VM->reg[IP]);
    

    // Ciclo de ejecución
    while (ejecutando) {
        offset = VM->reg[IP] & 0xFFFF;

        // Validar que no nos pasamos del segmento de código
        if (offset >= size) {
       //     printf("[STOP] IP fuera de rango (offset=%d, size=%d)\n", offset, size);
            ejecutando = 0; 
        } else {
            // Dirección física = base + offset
            dirFisica = base + offset; //base del code segment y offset de ip
            unsigned char rawInstr = VM->memory[dirFisica];
            ComponentesInstruccion(VM, dirFisica, &instruc, &cantOp, &codOp);

        //    printf("[FETCH] IP=%08X | DirFisica=%04X | rawInstr=%02X | codOp=%02X | sizeA=%d sizeB=%d cantOp=%d AC=%d\n",
         //          VM->reg[IP], dirFisica, rawInstr, codOp, instruc.sizeA, instruc.sizeB, cantOp,VM->reg[AC]);

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

      //      printf("OPC = %08X \n",VM->reg[OPC]);
     //       printf("OP1 = %08X \n",VM->reg[OP1]);
      //      printf("OP2 = %08X \n",VM->reg[OP2]);


      //     printf("[DEBUG] codOp=%02X (%d)\n", codOp, codOp);

            int ip_anterior = VM->reg[IP];

             // actualiza ip
            if (VM->reg[IP] == ip_anterior ) {
            VM->reg[IP] += 1 + instruc.sizeA + instruc.sizeB;
            }

            // hace la funcion
            if (!((codOp <= 0x08) || (codOp >= 0x10 && codOp<= 0x1F))) {
               printf("[ERROR] Código de operación inválido: %02X\n", codOp);
                ejecutando = 0;  // fin por error
            } else if (Funciones[codOp] != NULL) {
        //       printf("[EXECUTE] Ejecutando opcode %02X...\n", codOp);
                Funciones[codOp](VM, instruc);
            } else {
        //        printf("[WARNING] Instrucción %02X no implementada\n", codOp);
            } 
            // Condición de parada por STOP
            if (VM->reg[IP] == -1) {
      //          printf("[STOP] Ejecución finalizada por instrucción STOP\n");
                ejecutando = 0;
            }

            // Debug de registros después de ejecutar
       //  printf("[DEBUG] Regs: EAX=%08X EBX=%08X ECX=%08X EDX=%08X EFX=%08X AC=%08X CC=%08X IP=%08X\n",
       //            VM->reg[EAX], VM->reg[EBX], VM->reg[ECX], VM->reg[EDX],VM->reg[EFX],
       //           VM->reg[AC], VM->reg[CC], VM->reg[IP]);
       //  printf("\n");
        }
    }
}

void DefinoRegistro(int *codReg, int *sector, int op) {
    *codReg = op & 0x1F;        
    *sector = (op >> 5) & 0x03; 
}

void LeerSectorRegistro(int *AuxR,TVM * VM,unsigned char Sec,int CodReg){ 
  int CorroSigno=0;
  if (Sec == 1){
        *AuxR = VM.reg[CodReg] & 0XFF;
        CorroSigno = 24;
    }
  else if (Sec == 2){
        *AuxR = (VM.reg[CodReg] & 0XFF00) >> 8;
        CorroSigno = 16;
    }
  else if (Sec == 3){
        *AuxR = VM.reg[CodReg] & 0XFFFF;
        CorroSigno = 16;
    }
  else
        *AuxR = VM.reg[CodReg];

  *AuxR = *AuxR << CorroSigno;
  *AuxR = *AuxR >> CorroSigno;
}

void escribirSectorRegistro(TVM *VM, int codReg, int sec, int nuevoValor) {
    switch (sec) {
        case 0: VM->reg[codReg] = nuevoValor; break;
        case 1: VM->reg[codReg] = (VM->reg[codReg] & 0xFFFFFF00) | (nuevoValor & 0xFF); break;
        case 2: VM->reg[codReg] = (VM->reg[codReg] & 0xFFFF00FF) | ((nuevoValor & 0xFF) << 8); break;
        case 3: VM->reg[codReg] = (VM->reg[codReg] & 0xFFFF0000) | (nuevoValor & 0xFFFF); break;
    }
}

void escribeMemoria(TVM * VM,int OP,int valor, int size) {
    
    int regact,csact,offreg,offop,offset;
    
    regact = (OP & 0x00FF0000) >> 16; //registro actual recibido a modificar
    csact = (VM->reg[regact] & 0xFFFF0000) >> 16; //codigo de segmento de op recibido
    offop = OP & 0x0000FFFF; //offset del op recibido
    offreg = (VM->reg[regact] & 0x0000FFFF); //offset del registro 

     // Si regact es DS o CS, y offreg==0, acceso directo
    if ((regact == DS || regact == CS) && offreg == 0) {
        offset = offop;
    } else {
        offset = offreg + offop;
    }
    

    VM->reg[LAR] = csact << 16 | offset;

    // 2. Traducir dirección lógica a física
    int dirFis = getDirfisica(VM, offset,csact, size); //dir fiscia va a ser base segmento + offset

    //error de segmento
        if (dirFis == -1) {
        generaerror(ERROR_SEGMENTO);
        return; // Retornar valor por defecto
    }
    
    // 3. Cargar MAR (parte alta: size, parte baja: dirección física)
    VM->reg[MAR] = (size << 16) | (dirFis & 0xFFFF);

    // 4. Cargar en MBR
    VM->reg[MBR] = valor;

    


   // printf("LAR : %08X \n",VM->reg[LAR]);
   // printf("MAR %08X \n",VM->reg[MAR]);
  //  printf("MBR %08X \n",VM->reg[MBR]);

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

    if ((regact == DS || regact == CS) && offreg == 0) {
        offset = offop;
    } else {
        offset = offreg + offop;
    }

    VM->reg[LAR] = csact << 16 | offset;

    int dirFis = getDirfisica(VM,offset,csact,size);

     //error de segmento
        if (dirFis == -1) {
        generaerror(ERROR_SEGMENTO);
        return 0; // Retornar valor por defecto
    }
    

    VM->reg[MAR] = (size << 16) | (dirFis & 0xFFFF);

    int valorx = 0;
    for (int i = 0; i < size; i++) {
        valorx = (valorx << 8) | (VM->memory[dirFis + i] & 0xFF);
    }
    VM->reg[MBR] = valorx;

   // printf("LAR  LECTURA: %08X \n",VM->reg[LAR]);
   // printf("MAR LECTURA %08X \n",VM->reg[MAR]);
   // printf("MBR LECTURA %08X \n",VM->reg[MBR]);
    return valorx;
}


void actualizaCC(TVM *VM, int resultado) {
    VM->reg[CC] = 0;
    if (resultado == 0) VM->reg[CC] |= 0x40000000; /* Z */
    if (resultado < 0)  VM->reg[CC] |= 0x80000000; /* N */
}


void guardaB(TVM *VM, Instruccion instruc, int *auxOpB) {
    int codReg;
    unsigned char SecB, CodOpB;

    switch (instruc.sizeB) {
        case 2: // inmediato
            *auxOpB = instruc.valorB;
            break;

        case 1: // registro
            DefinoRegistro(&CodOpB, &SecB, instruc.valorB);
            LeerSectorRegistro(auxOpB, VM, SecB, CodOpB);
            break;

        case 3: // memoria
            *auxOpB = leerMemoria(VM, VM->reg[OP2], 4);
            break;

        default:
            generaerror(ERROR_OPERANDO);
            break;
    }
}


void MOV(TVM *VM, Instruccion instruc) {
    unsigned char SecA, SecB;
    int CodOpA, CodOpB;
    int valorA = 0, valorB = 0;

    guardaB(VM, instruc, &valorB);

    if (instruc.sizeA == 1) { 
        DefinoRegistro(&CodOpA, &SecA, instruc.valorA);
        escribirSectorRegistro(VM, CodOpA, SecA, valorB);
    }
    else if (instruc.sizeA == 3) {
        escribeMemoria(VM, instruc.valorA, valorB, 4);
    }
    else generaerror(ERROR_OPERANDO);
}



void ADD(TVM *VM, Instruccion instruc) {
    unsigned char SecA, CodOpA;
    int valorA = 0, valorB = 0, resultado = 0;

    guardaB(VM, instruc, &valorB);

    if (instruc.sizeA == 1) {
        DefinoRegistro(&CodOpA,&SecA, instruc.valorA);
        LeerSectorRegistro(&valorA, VM, SecA, CodOpA);

        resultado = valorA + valorB;
        EscribirSectorRegistro(VM, SecA, CodOpA, resultado);
    }
    else if (instruc.sizeA == 3) { 
        // Memoria
        valorA = leerMemoria(VM, instruc.valorA, 4);
        resultado = valorA + valorB;
        escribeMemoria(VM, instruc.valorA, resultado, 4);
    }
    else generaerror(ERROR_OPERANDO);

    actualizaCC(VM, resultado);
}

void SUB(TVM *VM, Instruccion instruc) {
    unsigned char SecA, CodOpA;
    int valorA = 0, valorB = 0, resultado = 0;

    guardaB(VM, instruc, &valorB);

    if (instruc.sizeA == 1) {
        DefinoRegistro(&CodOpA,&SecA, instruc.valorA);
        LeerSectorRegistro(&valorA, VM, SecA, CodOpA);

        resultado = valorA - valorB;
        EscribirSectorRegistro(VM, SecA, CodOpA, resultado);
    }
    else if (instruc.sizeA == 3) { 
        valorA = leerMemoria(VM, instruc.valorA, 4);
        resultado = valorA - valorB;
        escribeMemoria(VM, instruc.valorA, resultado, 4);
    }
    else generaerror(ERROR_OPERANDO);

    actualizaCC(VM, resultado);
}

void MUL(TVM *VM, Instruccion instruc) {
    unsigned char SecA, CodOpA;
    int valorA = 0, valorB = 0, resultado = 0;

    guardaB(VM, instruc, &valorB);

    if (instruc.sizeA == 1) {
        DefinoRegistro(&CodOpA,&SecA, instruc.valorA);
        LeerSectorRegistro(&valorA, VM, SecA, CodOpA);

        resultado = valorA * valorB;
        EscribirSectorRegistro(VM, SecA, CodOpA, resultado);
    }
    else generaerror(ERROR_OPERANDO);

    actualizaCC(VM, resultado);
}

void DIV(TVM *VM, Instruccion instruc) {
    unsigned char SecA;
    int CodOpA;
    int valorA = 0, valorB = 0, resultado = 0, resto = 0;

    // Obtengo el operando B
    guardaB(VM, instruc, &valorB);

    // Verifico división por cero
    if (valorB == 0) {
        generaerror(ERROR_DIVISION_POR_CERO);
        return;
    }

    // Caso A = registro
    if (instruc.sizeA == 1) {
        DefinoRegistro(&CodOpA, &SecA, instruc.valorA);
        LeerSectorRegistro(&valorA, VM, SecA, CodOpA);

        resultado = valorA / valorB;
        resto = valorA % valorB;
        escribirSectorRegistro(VM, CodOpA, SecA, resultado);
    }

    // Caso A = memoria
    else if (instruc.sizeA == 3) {
        valorA = leerMemoria(VM, instruc.valorA, 4);
        resultado = valorA / valorB;
        resto = valorA % valorB;
        escribeMemoria(VM, instruc.valorA, resultado, 4);
    }

    // Operando inválido
    else generaerror(ERROR_OPERANDO);

    // Actualizo código de condición
    actualizaCC(VM, resultado);

    VM->reg[AC] = resto; // Guardo el resto en AC
}

void CMP(TVM *VM, Instruccion instruc) {
    unsigned char SecA;
    int CodOpA;
    int valorA = 0, valorB = 0, resultado = 0;

    guardaB(VM, instruc, &valorB);

    if (instruc.sizeA == 1) {
        DefinoRegistro(&CodOpA, &SecA, instruc.valorA);
        LeerSectorRegistro(&valorA, VM, SecA, CodOpA);
    }
    else if (instruc.sizeA == 3) {
        valorA = leerMemoria(VM, instruc.valorA, 4);
    }
    else generaerror(ERROR_OPERANDO);

    resultado = valorA - valorB;
    actualizaCC(VM, resultado);
}

void SHL(TVM *VM, Instruccion instruc) {
    unsigned char SecA;
    int CodOpA;
    int valorA = 0, valorB = 0, resultado = 0;

    guardaB(VM, instruc, &valorB); // cantidad de bits a desplazar

    if (instruc.sizeA == 1) {
        DefinoRegistro(&CodOpA, &SecA, instruc.valorA);
        LeerSectorRegistro(&valorA, VM, SecA, CodOpA);
        resultado = valorA << valorB;
        escribirSectorRegistro(VM, CodOpA, SecA, resultado);
    } 
    else if (instruc.sizeA == 3) {
        valorA = leerMemoria(VM, instruc.valorA, 4);
        resultado = valorA << valorB;
        escribeMemoria(VM, instruc.valorA, resultado, 4);
    } 
    else generaerror(ERROR_OPERANDO);

    actualizaCC(VM, resultado);
}


void SHR(TVM *VM, Instruccion instruc) {
    unsigned char SecA;
    int CodOpA;
    int valorA = 0, valorB = 0, resultado = 0;

    guardaB(VM, instruc, &valorB); // cantidad de bits a desplazar

    if (instruc.sizeA == 1) {
        DefinoRegistro(&CodOpA, &SecA, instruc.valorA);
        LeerSectorRegistro(&valorA, VM, SecA, CodOpA);
        resultado = (unsigned int)valorA >> valorB;
        escribirSectorRegistro(VM, CodOpA, SecA, resultado);
    } 
    else if (instruc.sizeA == 3) {
        valorA = leerMemoria(VM, instruc.valorA, 4);
        resultado = (unsigned int)valorA >> valorB;
        escribeMemoria(VM, instruc.valorA, resultado, 4);
    } 
    else generaerror(ERROR_OPERANDO);

    actualizaCC(VM, resultado);
}

void SAR(TVM *VM, Instruccion instruc) {
    unsigned char SecA;
    int CodOpA;
    int valorA = 0, valorB = 0, resultado = 0;

    guardaB(VM, instruc, &valorB); // cantidad de bits a desplazar

    if (instruc.sizeA == 1) {
        DefinoRegistro(&CodOpA, &SecA, instruc.valorA);
        LeerSectorRegistro(&valorA, VM, SecA, CodOpA);
        resultado = valorA >> valorB; // en C, el >> ya es aritmético para signed int
        escribirSectorRegistro(VM, CodOpA, SecA, resultado);
    }
    else if (instruc.sizeA == 3) {
        valorA = leerMemoria(VM, instruc.valorA, 4);
        resultado = valorA >> valorB;
        escribeMemoria(VM, instruc.valorA, resultado, 4);
    }
    else generaerror(ERROR_OPERANDO);

    actualizaCC(VM, resultado);
}

void AND(TVM *VM, Instruccion instruc) {
    unsigned char SecA;
    int CodOpA;
    int valorA = 0, valorB = 0, resultado = 0;

    guardaB(VM, instruc, &valorB);

    if (instruc.sizeA == 1) {
        DefinoRegistro(&CodOpA, &SecA, instruc.valorA);
        LeerSectorRegistro(&valorA, VM, SecA, CodOpA);
        resultado = valorA & valorB;
        escribirSectorRegistro(VM, CodOpA, SecA, resultado);
    }
    else if (instruc.sizeA == 3) {
        valorA = leerMemoria(VM, instruc.valorA, 4);
        resultado = valorA & valorB;
        escribeMemoria(VM, instruc.valorA, resultado, 4);
    }
    else generaerror(ERROR_OPERANDO);

    actualizaCC(VM, resultado);
}


void OR(TVM *VM, Instruccion instruc) {
    unsigned char SecA;
    int CodOpA;
    int valorA = 0, valorB = 0, resultado = 0;

    guardaB(VM, instruc, &valorB);

    if (instruc.sizeA == 1) {
        DefinoRegistro(&CodOpA, &SecA, instruc.valorA);
        LeerSectorRegistro(&valorA, VM, SecA, CodOpA);
        resultado = valorA | valorB;
        escribirSectorRegistro(VM, CodOpA, SecA, resultado);
    }
    else if (instruc.sizeA == 3) {
        valorA = leerMemoria(VM, instruc.valorA, 4);
        resultado = valorA | valorB;
        escribeMemoria(VM, instruc.valorA, resultado, 4);
    }
    else generaerror(ERROR_OPERANDO);

    actualizaCC(VM, resultado);
}

void XOR(TVM *VM, Instruccion instruc) {
    unsigned char SecA;
    int CodOpA;
    int valorA = 0, valorB = 0, resultado = 0;

    guardaB(VM, instruc, &valorB);

    if (instruc.sizeA == 1) {
        DefinoRegistro(&CodOpA, &SecA, instruc.valorA);
        LeerSectorRegistro(&valorA, VM, SecA, CodOpA);
        resultado = valorA ^ valorB;
        escribirSectorRegistro(VM, CodOpA, SecA, resultado);
    }
    else if (instruc.sizeA == 3) {
        valorA = leerMemoria(VM, instruc.valorA, 4);
        resultado = valorA ^ valorB;
        escribeMemoria(VM, instruc.valorA, resultado, 4);
    }
    else generaerror(ERROR_OPERANDO);

    actualizaCC(VM, resultado);
}


void SWAP(TVM *VM, Instruccion instruc) {
    unsigned char SecA, SecB;
    int CodOpA, CodOpB;
    int valorA = 0, valorB = 0;

    // Leer A y B
    if (instruc.sizeA == 1) {
        DefinoRegistro(&CodOpA, &SecA, instruc.valorA);
        LeerSectorRegistro(&valorA, VM, SecA, CodOpA);
    } 
    else if (instruc.sizeA == 3) {
        valorA = leerMemoria(VM, instruc.valorA, 4);
    } 
    else {
        generaerror(ERROR_OPERANDO);
        return;
    }

    guardaB(VM, instruc, &valorB);

    // Escribir intercambiados
    if (instruc.sizeA == 1) escribirSectorRegistro(VM, CodOpA, SecA, valorB);
    else if (instruc.sizeA == 3) escribeMemoria(VM, instruc.valorA, valorB, 4);

    if (instruc.sizeB == 1) {
        DefinoRegistro(&CodOpB, &SecB, instruc.valorB);
        escribirSectorRegistro(VM, CodOpB, SecB, valorA);
    } 
    else if (instruc.sizeB == 3) {
        escribeMemoria(VM, instruc.valorB, valorA, 4);
    }
}

void LDH(TVM *VM, Instruccion instruc) {
    unsigned char SecA;
    int CodOpA;
    int valorB = 0, valorA = 0, resultado = 0;

    guardaB(VM, instruc, &valorB);

    if (instruc.sizeA == 1) {
        DefinoRegistro(&CodOpA, &SecA, instruc.valorA);
        LeerSectorRegistro(&valorA, VM, SecA, CodOpA);
        resultado = (valorA & 0x0000FFFF) | ((valorB & 0xFFFF) << 16);
        escribirSectorRegistro(VM, CodOpA, SecA, resultado);
    } 
    else if (instruc.sizeA == 3) {
        valorA = leerMemoria(VM, instruc.valorA, 4);
        resultado = (valorA & 0x0000FFFF) | ((valorB & 0xFFFF) << 16);
        escribeMemoria(VM, instruc.valorA, resultado, 4);
    } 
    else generaerror(ERROR_OPERANDO);
}


void LDL(TVM *VM, Instruccion instruc) {
    unsigned char SecA;
    int CodOpA;
    int valorB = 0, valorA = 0, resultado = 0;

    guardaB(VM, instruc, &valorB);

    if (instruc.sizeA == 1) {
        DefinoRegistro(&CodOpA, &SecA, instruc.valorA);
        LeerSectorRegistro(&valorA, VM, SecA, CodOpA);
        resultado = (valorA & 0xFFFF0000) | (valorB & 0xFFFF);
        escribirSectorRegistro(VM, CodOpA, SecA, resultado);
    } 
    else if (instruc.sizeA == 3) {
        valorA = leerMemoria(VM, instruc.valorA, 4);
        resultado = (valorA & 0xFFFF0000) | (valorB & 0xFFFF);
        escribeMemoria(VM, instruc.valorA, resultado, 4);
    } 
    else generaerror(ERROR_OPERANDO);
}



int random32() {
    return ((rand() & 0xFFFF) << 16) | (rand() & 0xFFFF);
}

void RND(TVM *VM, Instruccion instruc) {
    unsigned char SecA;
    int CodOpA;
    int valorA = 0;
    int resultado;

    resultado = random32();  // genera número aleatorio de 32 bits

    if (instruc.sizeA == 1) {
        DefinoRegistro(&CodOpA, &SecA, instruc.valorA);
        LeerSectorRegistro(&valorA, VM, SecA, CodOpA);  // mantiene consistencia
        escribirSectorRegistro(VM, CodOpA, SecA, resultado);
    }
    else if (instruc.sizeA == 3) {
        escribeMemoria(VM, instruc.valorA, resultado, 4);
    }
    else generaerror(ERROR_OPERANDO);
}


int resolverSaltoSeguro(TVM *VM, Instruccion instruc) {
    int codReg, offset;

    // Resolver operando A (el destino del salto)
    switch (instruc.sizeA) {
        case 2: // inmediato
            offset = instruc.valorA;
            break;
        case 1: // registro
            DefinoRegistro(&codReg, instruc.valorA);
            offset = VM->reg[codReg];
            break;
        case 3: // memoria
            offset = leerMemoria(VM, VM->reg[OP1], 4);
            break;
        default:
            generaerror(ERROR_OPERANDO);
            return -1;
    }

    // Validar que el offset esté dentro del segmento de código
    int tamCS = getTam(VM->segmentos[SEG_CS]);
    if (offset < 0 || offset >= tamCS) {
        generaerror(ERROR_SEGMENTO);
        return -1;
    }

    return offset; // <-- Retorna offset lógico, NO dirección física
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
    unsigned char SecA;
    int CodOpA;
    int valorA = 0, resultado = 0;

    if (instruc.sizeA == 1) {
        DefinoRegistro(&CodOpA, &SecA, instruc.valorA);
        LeerSectorRegistro(&valorA, VM, SecA, CodOpA);
        resultado = ~valorA;
        escribirSectorRegistro(VM, CodOpA, SecA, resultado);
    }
    else if (instruc.sizeA == 3) {
        valorA = leerMemoria(VM, instruc.valorA, 4);
        resultado = ~valorA;
        escribeMemoria(VM, instruc.valorA, resultado, 4);
    }
    else generaerror(ERROR_OPERANDO);

    actualizaCC(VM, resultado);
}


void STOP(TVM * VM,Instruccion instruc) {
    exit (0);
}

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
            //deberia ser BIG ENDIAN????
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

   // int contador=0;

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

      //  contador++;
/*
            if (CodOp == 0x0F) {  // STOP
            printf("// Fin del código (STOP)\n");
            break;
            
        }
            */
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
            printf("[%s+%d]", VecRegistros[seg], off);
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
            printf("[%s+%d]", VecRegistros[seg],off);
        }
    }

    printf("\n");
}




#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "mv.h"

void inicializoVecFunciones(char VecFunciones[CANTFUNC][5]){
  
    strcpy(VecFunciones[0x00],"SYS");
    strcpy(VecFunciones[0x01],"JMP");
    strcpy(VecFunciones[0x02],"JZ");
    strcpy(VecFunciones[0x03],"JP");
    strcpy(VecFunciones[0x04],"JN");
    strcpy(VecFunciones[0x05],"JNZ");
    strcpy(VecFunciones[0x06],"JNP");
    strcpy(VecFunciones[0x07],"JNN");
    strcpy(VecFunciones[0x08],"NOT");
    
    strcpy(VecFunciones[0x0B],"PUSH");
    strcpy(VecFunciones[0x0C],"POP");
    strcpy(VecFunciones[0x0D],"CALL");
    strcpy(VecFunciones[0x0E],"RET");  
    strcpy(VecFunciones[0x0F],"STOP");
   
    strcpy(VecFunciones[0x10],"MOV");
    strcpy(VecFunciones[0x11],"ADD");
    strcpy(VecFunciones[0x12],"SUB");
    strcpy(VecFunciones[0x13],"MUL");
    strcpy(VecFunciones[0x14],"DIV");
    strcpy(VecFunciones[0x15],"CMP");
    strcpy(VecFunciones[0x16],"SHL");
    strcpy(VecFunciones[0x17],"SHR");
    strcpy(VecFunciones[0x18],"SAR");
    strcpy(VecFunciones[0x19],"AND");
    strcpy(VecFunciones[0x1A],"OR");
    strcpy(VecFunciones[0x1B],"XOR");
    strcpy(VecFunciones[0x1C],"SWAP");
    strcpy(VecFunciones[0x1D],"LDL");
    strcpy(VecFunciones[0x1E],"LDH");
    strcpy(VecFunciones[0x1F],"RND");
}

void inicializoVecRegistros(char VecRegistros[CANTREG][4]){
    strcpy(VecRegistros[LAR],"LAR");
    strcpy(VecRegistros[MAR],"MAR");
    strcpy(VecRegistros[MBR],"MBR");
    strcpy(VecRegistros[IP], "IP");
    strcpy(VecRegistros[OPC], "OPC");
    strcpy(VecRegistros[OP1], "OP1");
    strcpy(VecRegistros[OP2], "OP2");
    strcpy(VecRegistros[SP], "SP");  
    strcpy(VecRegistros[BP], "BP");  
    strcpy(VecRegistros[9], "");
    strcpy(VecRegistros[EAX], "EAX");
    strcpy(VecRegistros[EBX], "EBX");
    strcpy(VecRegistros[ECX], "ECX");
    strcpy(VecRegistros[EDX], "EDX");
    strcpy(VecRegistros[EEX], "EEX"); 
    strcpy(VecRegistros[EFX], "EFX"); 
    strcpy(VecRegistros[CC], "CC");   
    strcpy(VecRegistros[AC], "AC");   
    strcpy(VecRegistros[ES], "ES");   
    strcpy(VecRegistros[SS], "SS");   
    strcpy(VecRegistros[KS], "KS");   
    strcpy(VecRegistros[PS], "PS");   
    
    strcpy(VecRegistros[22], "");
    strcpy(VecRegistros[23], "");
    strcpy(VecRegistros[24], "");
    strcpy(VecRegistros[25], "");
    strcpy(VecRegistros[CS], "CS");
    strcpy(VecRegistros[DS], "DS");
    
    strcpy(VecRegistros[28], "");
    strcpy(VecRegistros[29], "");
    strcpy(VecRegistros[30], "");
    strcpy(VecRegistros[31], "");
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
    Funciones[0x0B] = PUSH;
    Funciones[0x0C] = POP;
    Funciones[0x0D] = CALL;
    Funciones[0x0E] = RET;
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
        case ERROR_MEMORIA_INSUFICIENTE:
            printf("[ERROR] Memoria insuficiente para alojar todos los segmentos\n");
            break;
        case STACK_OVERFLOW:
            printf("[ERROR] STACK OVERFLOW \n");
            break;
        case STACK_UNDERFLOW:
            printf("[ERROR] STACK UNDERFLOW \n");
            break;
    }

    exit(EXIT_FAILURE); // Aborta la ejecución
}


void push(TVM *VM, unsigned int valor) {



    int segIndex = (VM->reg[SS] >> 16) & 0xFFFF;
    int sp = VM->reg[SP] & 0xFFFF;
    int baseSS = VM->segmentos[segIndex].base;
    int tamSS  = VM->segmentos[segIndex].tam;

    if (VM->reg[SS] == 0xFFFFFFFF) {
    generaerror(STACK_OVERFLOW);
    return;
    }

    sp -= 4;

    if (sp < 0) {
    printf("[ERROR] Stack overflow (SP fuera de rango)\n");
    generaerror(STACK_OVERFLOW);
    return;
    }

    int dirFis = baseSS + sp;

    // escribe en memoria
    VM->memory[dirFis + 0] = (valor >> 24) & 0xFF;
    VM->memory[dirFis + 1] = (valor >> 16) & 0xFF;
    VM->memory[dirFis + 2] = (valor >> 8)  & 0xFF;
    VM->memory[dirFis + 3] = (valor >> 0)  & 0xFF;

    //actualiza sp
    VM->reg[SP] = (segIndex << 16) | (sp & 0xFFFF);
}

void pop(TVM *VM, unsigned int *valor) {
    int segIndex = (VM->reg[SS] >> 16) & 0xFFFF;
    int sp = VM->reg[SP] & 0xFFFF;
    int baseSS = VM->segmentos[segIndex].base;
    int tamSS  = VM->segmentos[segIndex].tam;

    // 1️⃣ Verificar underflow
    if  (sp >= tamSS) {
        printf("[ERROR] Stack underflow (SP fuera de rango)\n");
        generaerror(STACK_UNDERFLOW);
        return;
    }

    // 2️⃣ Calcular dirección física
    int dirFis = baseSS + sp;

    unsigned int val = 0;
    val |= (VM->memory[dirFis + 0] << 24);
    val |= (VM->memory[dirFis + 1] << 16);
    val |= (VM->memory[dirFis + 2] << 8);
    val |= (VM->memory[dirFis + 3]);

    *valor = val;

    sp += 4;
    VM->reg[SP] = (segIndex << 16) | (sp & 0xFFFF);
}


void iniciaRegs(TVM *VM, int entry_offset) {
    // Inicializar registros que no existen por defecto
    VM->reg[ES] = 0xFFFFFFFF;  // Por defecto no existe
    VM->reg[SS] = 0xFFFFFFFF;  // Se configura en cargaSegmentos si existe
    VM->reg[KS] = 0xFFFFFFFF;  // Se configura en cargaSegmentos si existe  
    VM->reg[PS] = 0xFFFFFFFF;  // Se configura en cargaSegmentos si existe
    
    // Inicializar registros de control
    VM->reg[CC] = 0;
    VM->reg[AC] = 0;
    VM->reg[SP] = 0xFFFFFFFF;  // Se configura en cargaSegmentos si hay stack
    VM->reg[BP] = 0xFFFFFFFF;  // No requiere inicialización especial

    // Validar que existe Code Segment
    if (VM->reg[CS] == 0xFFFFFFFF) {
        printf("Error: no hay Code Segment para iniciar IP.\n");
        return;
    }

    // Inicializar IP con entry point
    // 16 bits más significativos: selector de segmento CS
    // 16 bits menos significativos: entry_offset
    int csIndex = (int)(VM->reg[CS] >> 16);
    VM->reg[IP] = ((unsigned int)csIndex << 16) | entry_offset;

    // Configurar pila si existe Stack Segment
    if (VM->reg[SS] != 0xFFFFFFFF) {
        // Inicializar pila con valores por defecto
        push(VM, 0xFFFFFFFF);  // dirección de retorno
        push(VM, 0);           // argc dummy
        push(VM, 0xFFFFFFFF);  // argv dummy
        
        // Si hay Param Segment, reemplazar con valores reales
        if (VM->reg[PS] != 0xFFFFFFFF) {
            unsigned int valor;
            
            // Buscar argc al final del PS
            int ps_index = (VM->reg[PS] >> 16) & 0xFFFF;
            int tamPS = VM->segmentos[ps_index].tam;
            int basePS = VM->segmentos[ps_index].base;
            
            // Leer argc desde los últimos 4 bytes del PS
            int argc = 0;
            for (int i = 0; i < 4; i++) {
                argc = (argc << 8) | VM->memory[basePS + tamPS - 4 + i];
            }
            
            // Calcular dirección del array argv (justo antes de argc)
            int argv_offset = tamPS - 4 - (argc * 4);
            unsigned int argv_ptr = (ps_index << 16) | argv_offset;
            
            // Reemplazar valores dummy con reales
            pop(VM, &valor); // quitar argv dummy
            pop(VM, &valor); // quitar argc dummy
            pop(VM, &valor); // quitar retorno dummy
            
            push(VM, 0xFFFFFFFF);  // dirección de retorno
            push(VM, argc);        // argc real
            push(VM, argv_ptr);    // argv real
        }
        
        // Inicializar BP = SP (apunta al frame actual)
        VM->reg[BP] = VM->reg[SP];
    }
}

void cargaParametros(TVM *VM, int cant, char *params[]) {
    int offset = 0;
    int inicioString[50];
    
    // copia strings
    for (int i = 0; i < cant; i++) {
        inicioString[i] = offset; // guardar posición de cada string
        
        // Copiar caracteres del string
        for (int j = 0; params[i][j] != '\0'; j++) {
            VM->memory[offset++] = params[i][j];
        }
        VM->memory[offset++] = '\0'; // terminador nulo
    }
    
    // argv
    int inicioArgv = offset;
    for (int i = 0; i < cant; i++) {
        // Puntero = (segmento PS << 16) | offset_string
        unsigned int puntero = (0 << 16) | inicioString[i];
        
        // Almacenar en big-endian (4 bytes)
        VM->memory[offset++] = (puntero >> 24) & 0xFF;
        VM->memory[offset++] = (puntero >> 16) & 0xFF;
        VM->memory[offset++] = (puntero >> 8) & 0xFF;
        VM->memory[offset++] = puntero & 0xFF;
    }
    
    //argc
    VM->memory[offset++] = (cant >> 24) & 0xFF;
    VM->memory[offset++] = (cant >> 16) & 0xFF;
    VM->memory[offset++] = (cant >> 8) & 0xFF;
    VM->memory[offset++] = cant & 0xFF;
    
    VM->param_size = offset;
    
    printf("Param Segment creado: %d strings, argv en offset %d, argc=%d\n", 
           cant, inicioArgv, cant);
}

void cargaSegmentos(TVM *VM, THeader header) {
    int base = 0;
    int indice = 0;

    memset(VM->memory, 0, MEMORY_SIZE);

    // Limpio la tabla
    for (int i = 0; i < 8; i++) {
        VM->segmentos[i].base = 0;
        VM->segmentos[i].tam = 0;
    }

    // ✅ CORRECCIÓN: PS siempre en base 0
    if (VM->param_size > 0) {
        VM->segmentos[0].base = 0x00000000;  // ← FORZAR base 0
        VM->segmentos[0].tam = VM->param_size;
        VM->reg[PS] = (0 << 16) | 0;  // segmento 0, offset 0
        base = VM->param_size;  // siguiente segmento después de PS
        indice = 1;  // PS ocupa índice 0
    } else {
        VM->reg[PS] = 0xFFFFFFFF;
        base = 0;
        indice = 0;  // sin PS, otros segmentos desde índice 0
    }

    // === CONST SEGMENT ===
    if (header.const_size > 0) {
        VM->segmentos[indice].base = base;
        VM->segmentos[indice].tam  = header.const_size;
     //   VM->segmentos[indice].tam+=base;
        VM->reg[KS] = (indice << 16) | 0;
        base += header.const_size;
        indice++;
    } else VM->reg[KS] = 0xFFFFFFFF;

    // === CODE SEGMENT ===
    VM->segmentos[indice].base = base;
    VM->segmentos[indice].tam  = header.tam;
    //VM->segmentos[indice].tam+=base;
    VM->reg[CS] = (indice << 16) | 0;
    base += header.tam;
    indice++;

    // === DATA SEGMENT ===
    VM->segmentos[indice].base = base;
    VM->segmentos[indice].tam  = header.data_size > 0 ? header.data_size : 1024;
    //VM->segmentos[indice].tam+=base;
    VM->reg[DS] = (indice << 16) | 0;
    base += VM->segmentos[indice].tam;
    indice++;

    // === EXTRA SEGMENT ===
    VM->segmentos[indice].base = base;
    VM->segmentos[indice].tam  = header.extra_size > 0 ? header.extra_size : 1024;
    //VM->segmentos[indice].tam+=base;
    VM->reg[ES] = (indice << 16) | 0;
    base += VM->segmentos[indice].tam;
    indice++;

    // === STACK SEGMENT ===
    VM->segmentos[indice].base = base;
    VM->segmentos[indice].tam  = header.stack_size > 0 ? header.stack_size : 1024;
    //VM->segmentos[indice].tam+=base;
    VM->reg[SS] = (indice << 16) | 0;
    VM->reg[SP] = (indice << 16) | (VM->segmentos[indice].tam);
    base += VM->segmentos[indice].tam;
    indice++;

    // --- DEBUG ---
    printf("\nTabla de segmentos generada:\n");
    for (int i = 0; i < indice; i++) {
        printf("[%d] base=%04X tam=%04X\n", i, VM->segmentos[i].base, VM->segmentos[i].tam);
    }

    printf("Registros de segmento:\n");
    printf("PS=%08X KS=%08X CS=%08X DS=%08X ES=%08X SS=%08X\n",
           VM->reg[PS], VM->reg[KS], VM->reg[CS],
           VM->reg[DS], VM->reg[ES], VM->reg[SS]);
}



void leoVMI(TVM *VM, char nomarch[]) {
    FILE *archb;
    char id[6];
    unsigned char b1, b2;
    int mem_kib;
    int i;

    archb = fopen(nomarch, "rb");
    if (archb == NULL) {
        printf("No se pudo abrir el archivo .vmi\n");
        return;
    }

    // === Header ===
    fread(id, 1, 5, archb);
    id[5] = '\0';
    fread(&b1, 1, 1, archb); // versión
    fread(&b1, 1, 1, archb);
    fread(&b2, 1, 1, archb);
    mem_kib = (b1 << 8) | b2;

    if (strcmp(id, "VMI25") != 0) {
        printf("Error: archivo .vmi inválido\n");
        fclose(archb);
        return;
    }

    printf("Cargando imagen VMI25 (memoria=%d KiB)\n", mem_kib);

    // Carga los registrso
    //reconstruye leyendo de a 4 bytes

    for (i = 0; i < 32; i++) {
        unsigned int val = 0;
        fread(&b1, 1, 1, archb);
        val |= ((unsigned int)b1 << 24);
        fread(&b1, 1, 1, archb);
        val |= ((unsigned int)b1 << 16);
        fread(&b1, 1, 1, archb);
        val |= (b1 << 8);
        fread(&b1, 1, 1, archb);
        val |= (unsigned int)b1;
        VM->reg[i] = val;
    }

    // Carga la tabla de segmentos
    for (i = 0; i < 8; i++) {
        fread(&b1, 1, 1, archb);
        fread(&b2, 1, 1, archb);
        VM->segmentos[i].base = (b1 << 8) | b2;
        fread(&b1, 1, 1, archb);
        fread(&b2, 1, 1, archb);
        VM->segmentos[i].tam = (b1 << 8) | b2;
    }

    // Toda la memoria
    int totalBytes = mem_kib * 1024;
    if (totalBytes > MEMORY_SIZE) {
        printf("Error: imagen .vmi excede la memoria disponible.\n");
        fclose(archb);
        return;
    }
    fread(VM->memory, 1, totalBytes, archb);

    fclose(archb);

    printf("Imagen cargada correctamente. Reanudando ejecución...\n");
}

void leoArch(TVM *VM, char nomarch[], int cantParams, char *parametros[]) {
    FILE *archb;
    THeader header = {0};
    unsigned char b1, b2;
    char id[6];
    int i = 0;

    archb = fopen(nomarch, "rb");
    if (!archb) {
        printf("No se pudo abrir el archivo %s\n", nomarch);
        return;
    }

    fread(&header.c1, 1, 1, archb);
    fread(&header.c2, 1, 1, archb);
    fread(&header.c3, 1, 1, archb);
    fread(&header.c4, 1, 1, archb);
    fread(&header.c5, 1, 1, archb);
    fread(&header.version, 1, 1, archb);
    fread(&b1, 1, 1, archb); fread(&b2, 1, 1, archb);
    header.tam = (b1 << 8) | b2;

    sprintf(id, "%c%c%c%c%c", header.c1, header.c2, header.c3, header.c4, header.c5);

    // Intenta leer campos extendidos (si no hay bytes, se quedan en 0)
    fread(&b1, 1, 1, archb); fread(&b2, 1, 1, archb); header.data_size  = (b1 << 8) | b2;
    fread(&b1, 1, 1, archb); fread(&b2, 1, 1, archb); header.extra_size = (b1 << 8) | b2;
    fread(&b1, 1, 1, archb); fread(&b2, 1, 1, archb); header.stack_size = (b1 << 8) | b2;
    fread(&b1, 1, 1, archb); fread(&b2, 1, 1, archb); header.const_size = (b1 << 8) | b2;
    fread(&b1, 1, 1, archb); fread(&b2, 1, 1, archb); header.entry_offset = (b1 << 8) | b2;

    // Valores por defecto (si no se pudieron leer)
    if (!header.data_size)  header.data_size  = 1024;
    if (!header.extra_size) header.extra_size = 1024;
    if (!header.stack_size) header.stack_size = 1024;
    if (!header.const_size) header.const_size = 0;
    if (!header.entry_offset) header.entry_offset = 0;

    printf("id=%s version=%d tam=%d entry=%d\n",
           id, header.version, header.tam, header.entry_offset);
    printf("DATA=%d EXTRA=%d STACK=%d CONST=%d\n",
           header.data_size, header.extra_size,
           header.stack_size, header.const_size);

    if (strcmp(id, "VMX25") != 0) {
        printf("Archivo inválido.\n");
        fclose(archb);
        return;
    }

    if (cantParams > 0)
        cargaParametros(VM, cantParams, parametros);
    else
        VM->param_size = 0;

    cargaSegmentos(VM, header);
    iniciaRegs(VM, header.entry_offset);

    // Cargar código directamente en el segmento CODE
    int baseCode = getBase(VM->segmentos[(VM->reg[CS] >> 16)]);
    fread(&(VM->memory[baseCode]), 1, header.tam, archb);

    fclose(archb);
    printf("Archivo cargado correctamente.\n");
}


int getBase(TSegmento seg) {
    return seg.base;
}

int getTam(TSegmento seg) {
    return seg.tam;
}


int getDirfisica(TVM *VM, int offset, int segmento, int size) {
    int base, tam;

    if (segmento < 0 || segmento >= SEG_TABLE) {
        generaerror(ERROR_SEGMENTO);
        return -1;
    }

    base = VM->segmentos[segmento].base;
    tam = VM->segmentos[segmento].tam;

    // Validar que el acceso completo esté dentro del segmento
    if (offset < 0 || (offset + size - 1) >= tam) {
        generaerror(ERROR_SEGMENTO);
        return -1;
    }

    return base + offset;
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

int extenderSigno(int valor, int bytes) {
    int msb = 1 << ((bytes * 8) - 1);
    if (valor & msb)
        valor |= ~((1 << (bytes * 8)) - 1);
    return valor;
}

void SeteoValorOp(TVM * VM, int dirFisicaActual, Instruccion *instr) {
    instr->valorA = 0;
    instr->valorB = 0;

    // --- Operando B ---
    for (int i = 0; i < instr->sizeB; i++) {
        instr->valorB = (instr->valorB << 8) | VM->memory[++dirFisicaActual];
    }
    // Propagación de signo para inmediatos (sizeB == 2)
    if (instr->sizeB == 2) {
       instr->valorB = extenderSigno(instr->valorB,2);
    }

    // --- Operando A ---
    for (int i = 0; i < instr->sizeA; i++) {
        instr->valorA = (instr->valorA << 8) | VM->memory[++dirFisicaActual];
    }
    // Propagación de signo para inmediatos (sizeA == 2)
    if (instr->sizeA == 2) {
         instr->valorA = extenderSigno(instr->valorA, 2);
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

    int csIndex = (VM->reg[CS] >> 16) & 0xFFFF;
    if (VM->reg[CS] == 0xFFFFFFFF || csIndex >= SEG_TABLE) {
        printf("[ERROR] Code Segment no válido\n");
        return;
    }
    
    segIndex = (VM->reg[CS] >> 16) & 0xFFFF; //cs ya esta cargado y no es fijo
    base = VM->segmentos[segIndex].base;
    size = VM->segmentos[segIndex].tam;

    // Ciclo de ejecución
    while (ejecutando) {
        offset = VM->reg[IP] & 0xFFFF;

        // Validar que no nos pasamos del segmento de código
        if (offset >= size) {
       //   printf("[STOP] IP fuera de rango (offset=%d, size=%d)\n", offset, size);
            ejecutando = 0; 
        } else {
            // Dirección física = base + offset
            dirFisica = getDirfisica(VM,offset,segIndex,1);
            unsigned char rawInstr = VM->memory[dirFisica];
            ComponentesInstruccion(VM, dirFisica, &instruc, &cantOp, &codOp);

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

            int ip_anterior = VM->reg[IP];

             // actualiza ip
            if (VM->reg[IP] == ip_anterior ) {
             unsigned int ip = VM->reg[IP] & 0xFFFF;
             ip += 1 + instruc.sizeA + instruc.sizeB;
             VM->reg[IP] = (VM->reg[IP] & 0xFFFF0000) | (ip & 0xFFFF);
            }
            //hay que asegurar que solo se modifiquen los 16 bits bajos del IP.

            //(!((codOp <= 0x08) || (codOp >= 0x10 && codOp<= 0x1F)))
            // hace la funcion
             if (codOp < CANTFUNC && Funciones[codOp] != NULL)
               Funciones[codOp](VM, instruc);
             else {
             printf("[ERROR] Código de operación inválido: %02X\n", codOp);
             ejecutando = 0;
            }

            // Condición de parada por STOP
            if ((VM->reg[IP] & 0xFFFF) == 0xFFFF)
              ejecutando = 0;
        }
    }
}

void DefinoRegistro(int *codReg,int*sector, int op) {
    *codReg = op & 0x1F;        
    *sector = (op >> 5) & 0x03; 
}

void LeerSectorRegistro(int *AuxR,TVM * VM,unsigned char Sec,int CodReg){ 
  int CorroSigno=0;
  if (Sec == 1){
        *AuxR = VM->reg[CodReg] & 0XFF;
        CorroSigno = 24;
    }
  else if (Sec == 2){
        *AuxR = (VM->reg[CodReg] & 0XFF00) >> 8;
        CorroSigno = 16;
    }
  else if (Sec == 3){
        *AuxR = VM->reg[CodReg] & 0XFFFF;
        CorroSigno = 16;
    }
  else
        *AuxR = VM->reg[CodReg];

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

void escribeMemoria(TVM *VM, int OP, int valor) {
    int regact, segIndex, offreg, offop, offset, sizeBits;
    int size; // tamaño de la celda (1, 2 o 4 bytes)

    //  Extraer tamaño de celda desde los 2 bits más significativos
    sizeBits = (OP >> 30) & 0x3;  // bits 31-30

    switch (sizeBits) {
    case 0: size = 4; break; // long
    case 1: size = 2; break; // word  
    case 2: size = 1; break; // byte
     default: 
        generaerror(ERROR_OPERANDO);
        return;
    }

    // Extraer código de registro base (bits 24–28)
    regact = (OP >> 25) & 0x1F;

    // Extraer offset de 16 bits
    offop = OP & 0x0000FFFF;
    if (offop & 0x8000)  // signo extendido si es negativo
        offop |= 0xFFFF0000;

    // Obtener selector de segmento y offset actual del registro base
    int regVal = VM->reg[regact];
    segIndex = (regVal >> 16) & 0xFFFF;
    offreg   = regVal & 0xFFFF;

    // Validar existencia del segmento
    if (segIndex < 0 || segIndex >= SEG_TABLE || VM->segmentos[segIndex].base == -1) {
     generaerror(ERROR_SEGMENTO);
    return;
    }

    // No se permite escribir en PS o KS????
    int ps_index = (VM->reg[PS] >> 16) & 0xFFFF;
    int ks_index = (VM->reg[KS] >> 16) & 0xFFFF;
    if (segIndex == ps_index || segIndex == ks_index) {
        generaerror(ERROR_INSTRUCCION);
        return;
    }

    offset = offreg + offop;

    // Cargar LAR
    VM->reg[LAR] = (segIndex << 16) | (offset & 0xFFFF);

    // Obtener dirección física
    int dirFis = getDirfisica(VM, offset, segIndex, size);
    if (dirFis == -1) {
        generaerror(ERROR_SEGMENTO);
        return;
    }

    // Cargar registros de control
    VM->reg[MAR] = ((size & 0xFFFF) << 16) | (dirFis & 0xFFFF);
    VM->reg[MBR] = valor;

    // Escribir valor en memoria (big-endian)
    for (int i = 0; i < size; i++)
        VM->memory[dirFis + (size - 1 - i)] = (valor >> (8 * i)) & 0xFF;
}

int leerMemoria(TVM *VM, int OP) {
    int regact, segIndex, offreg, offop, offset, sizeBits;
    int size;
    int valor = 0;

    sizeBits = (OP >> 30) & 0x3;
    
    switch (sizeBits) {
        case 0: size = 4; break; // long
        case 1: size = 2; break; // word 
        case 2: size = 1; break; // byte
        default: 
            generaerror(ERROR_OPERANDO);
            return 0;
    }
    
   regact = (OP >> 25) & 0x1F;

    //Extraer offset de 16 bits (bits 0–15)
    offop = OP & 0x0000FFFF;
    if (offop & 0x8000)  // signo extendido si es negativo
        offop |= 0xFFFF0000;

    //  Obtener registro base: selector de segmento + desplazamiento
    uint32_t regVal = VM->reg[regact];
    segIndex = (regVal >> 16) & 0xFFFF;
    offreg   = regVal & 0xFFFF;

    if (segIndex < 0 || segIndex >= SEG_TABLE || VM->segmentos[segIndex].base == -1) {
     generaerror(ERROR_SEGMENTO);
    return 0;
    }


    offset = offreg + offop;

    // Cargar LAR
    VM->reg[LAR] = (segIndex << 16) | (offset & 0xFFFF);

    //  Obtener dirección física
    int dirFis = getDirfisica(VM, offset, segIndex, size);
    if (dirFis == -1) {
        generaerror(ERROR_SEGMENTO);
        return 0;
    }

    // Cargar MAR y leer contenido
    VM->reg[MAR] = ((size & 0xFFFF) << 16) | (dirFis & 0xFFFF);
    valor = 0;
    for (int i = 0; i < size; i++) {
        valor = (valor << 8) | (VM->memory[dirFis + i] & 0xFF);
    }

    // Cargar MBR
    VM->reg[MBR] = valor;

    return valor;
}


void actualizaCC(TVM *VM, int resultado) {
    VM->reg[CC] = 0;
    if (resultado == 0) VM->reg[CC] |= 0x40000000; /* Z */
    if (resultado < 0)  VM->reg[CC] |= 0x80000000; /* N */
}


void guardaB(TVM *VM, Instruccion instruc, int *auxOpB) {
    int SecB,CodOpB;

    switch (instruc.sizeB) {
        case 2: // inmediato
            *auxOpB = instruc.valorB;
            break;

        case 1: // registro
            DefinoRegistro(&CodOpB, &SecB, instruc.valorB);
            LeerSectorRegistro(auxOpB, VM, SecB, CodOpB);
            break;

        case 3: // memoria
            *auxOpB = leerMemoria(VM, VM->reg[OP2]);
            break;

        default:
            generaerror(ERROR_OPERANDO);
            break;
    }
}


void MOV(TVM *VM, Instruccion instruc) {
    int SecA, SecB;
    int CodOpA, CodOpB;
    int valorA = 0, valorB = 0;

    guardaB(VM, instruc, &valorB);

    if (instruc.sizeA == 1) { 
        DefinoRegistro(&CodOpA, &SecA, instruc.valorA);
        escribirSectorRegistro(VM, CodOpA, SecA, valorB);
    }
    else if (instruc.sizeA == 3) {
        escribeMemoria(VM, instruc.valorA, valorB);
    }
    else generaerror(ERROR_OPERANDO);
}



void ADD(TVM *VM, Instruccion instruc) {
    int SecA, CodOpA;
    int valorA = 0, valorB = 0, resultado = 0;

    guardaB(VM, instruc, &valorB);

    if (instruc.sizeA == 1) {
        DefinoRegistro(&CodOpA,&SecA, instruc.valorA);
        LeerSectorRegistro(&valorA, VM, SecA, CodOpA);

        resultado = valorA + valorB;
        escribirSectorRegistro(VM, SecA, CodOpA, resultado);
    }
    else if (instruc.sizeA == 3) { 
        // Memoria
        valorA = leerMemoria(VM, instruc.valorA);
        resultado = valorA + valorB;
        escribeMemoria(VM, instruc.valorA, resultado);
    }
    else generaerror(ERROR_OPERANDO);

    actualizaCC(VM, resultado);
}

void SUB(TVM *VM, Instruccion instruc) {
    int SecA, CodOpA;
    int valorA = 0, valorB = 0, resultado = 0;

    guardaB(VM, instruc, &valorB);

    if (instruc.sizeA == 1) {
        DefinoRegistro(&CodOpA,&SecA, instruc.valorA);
        LeerSectorRegistro(&valorA, VM, SecA, CodOpA);

        resultado = valorA - valorB;
        escribirSectorRegistro(VM, SecA, CodOpA, resultado);
    }
    else if (instruc.sizeA == 3) { 
        valorA = leerMemoria(VM, instruc.valorA);
        resultado = valorA - valorB;
        escribeMemoria(VM, instruc.valorA, resultado);
    }
    else generaerror(ERROR_OPERANDO);

    actualizaCC(VM, resultado);
}

void MUL(TVM *VM, Instruccion instruc) {
    int SecA, CodOpA;
    int valorA = 0, valorB = 0, resultado = 0;

    guardaB(VM, instruc, &valorB);

    if (instruc.sizeA == 1) {
        DefinoRegistro(&CodOpA,&SecA, instruc.valorA);
        LeerSectorRegistro(&valorA, VM, SecA, CodOpA);

        resultado = valorA * valorB;
        escribirSectorRegistro(VM, SecA, CodOpA, resultado);
    } else if (instruc.sizeA == 3) { 
        // Memoria
        valorA = leerMemoria(VM, instruc.valorA);
        resultado = valorA * valorB;
        escribeMemoria(VM, instruc.valorA, resultado);
    }
    else generaerror(ERROR_OPERANDO);

    actualizaCC(VM, resultado);
}

void DIV(TVM *VM, Instruccion instruc) {
    int CodOpA,SecA;
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
        valorA = leerMemoria(VM, instruc.valorA);
        resultado = valorA / valorB;
        resto = valorA % valorB;
        escribeMemoria(VM, instruc.valorA, resultado);
    }

    // Operando inválido
    else generaerror(ERROR_OPERANDO);

    // Actualizo código de condición
    actualizaCC(VM, resultado);

    VM->reg[AC] = resto; // Guardo el resto en AC
}

void CMP(TVM *VM, Instruccion instruc) {
    int SecA;
    int CodOpA;
    int valorA = 0, valorB = 0, resultado = 0;

    guardaB(VM, instruc, &valorB);

    if (instruc.sizeA == 1) {
        DefinoRegistro(&CodOpA, &SecA, instruc.valorA);
        LeerSectorRegistro(&valorA, VM, SecA, CodOpA);
    }
    else if (instruc.sizeA == 3) {
        valorA = leerMemoria(VM, instruc.valorA);
    }
    else generaerror(ERROR_OPERANDO);

    resultado = valorA - valorB;
    actualizaCC(VM, resultado);
}

void SHL(TVM *VM, Instruccion instruc) {
    int SecA;
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
        valorA = leerMemoria(VM, instruc.valorA);
        resultado = valorA << valorB;
        escribeMemoria(VM, instruc.valorA, resultado);
    } 
    else generaerror(ERROR_OPERANDO);

    actualizaCC(VM, resultado);
}


void SHR(TVM *VM, Instruccion instruc) {
    int SecA;
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
        valorA = leerMemoria(VM, instruc.valorA);
        resultado = (unsigned int)valorA >> valorB;
        escribeMemoria(VM, instruc.valorA, resultado);
    } 
    else generaerror(ERROR_OPERANDO);

    actualizaCC(VM, resultado);
}

void SAR(TVM *VM, Instruccion instruc) {
    int SecA;
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
        valorA = leerMemoria(VM, instruc.valorA);
        resultado = valorA >> valorB;
        escribeMemoria(VM, instruc.valorA, resultado);
    }
    else generaerror(ERROR_OPERANDO);

    actualizaCC(VM, resultado);
}

void AND(TVM *VM, Instruccion instruc) {
    int SecA;
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
        valorA = leerMemoria(VM, instruc.valorA);
        resultado = valorA & valorB;
        escribeMemoria(VM, instruc.valorA, resultado);
    }
    else generaerror(ERROR_OPERANDO);

    actualizaCC(VM, resultado);
}


void OR(TVM *VM, Instruccion instruc) {
    int SecA;
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
        valorA = leerMemoria(VM, instruc.valorA);
        resultado = valorA | valorB;
        escribeMemoria(VM, instruc.valorA, resultado);
    }
    else generaerror(ERROR_OPERANDO);

    actualizaCC(VM, resultado);
}

void XOR(TVM *VM, Instruccion instruc) {
    int SecA;
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
        valorA = leerMemoria(VM, instruc.valorA);
        resultado = valorA ^ valorB;
        escribeMemoria(VM, instruc.valorA, resultado);
    }
    else generaerror(ERROR_OPERANDO);

    actualizaCC(VM, resultado);
}


void SWAP(TVM *VM, Instruccion instruc) {
    int SecA, SecB;
    int CodOpA, CodOpB;
    int valorA = 0, valorB = 0;

    // Leer A y B
    if (instruc.sizeA == 1) {
        DefinoRegistro(&CodOpA, &SecA, instruc.valorA);
        LeerSectorRegistro(&valorA, VM, SecA, CodOpA);
    } 
    else if (instruc.sizeA == 3) {
        valorA = leerMemoria(VM, instruc.valorA);
    } 
    else {
        generaerror(ERROR_OPERANDO);
        return;
    }

    guardaB(VM, instruc, &valorB);

    // Escribir intercambiados
    if (instruc.sizeA == 1) escribirSectorRegistro(VM, CodOpA, SecA, valorB);
    else if (instruc.sizeA == 3) escribeMemoria(VM, instruc.valorA, valorB);

    if (instruc.sizeB == 1) {
        DefinoRegistro(&CodOpB, &SecB, instruc.valorB);
        escribirSectorRegistro(VM, CodOpB, SecB, valorA);
    } 
    else if (instruc.sizeB == 3) {
        escribeMemoria(VM, instruc.valorB, valorA);
    }
}

void LDH(TVM *VM, Instruccion instruc) {
    int SecA;
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
        valorA = leerMemoria(VM, instruc.valorA);
        resultado = (valorA & 0x0000FFFF) | ((valorB & 0xFFFF) << 16);
        escribeMemoria(VM, instruc.valorA, resultado);
    } 
    else generaerror(ERROR_OPERANDO);
}


void LDL(TVM *VM, Instruccion instruc) {
    int SecA;
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
        valorA = leerMemoria(VM, instruc.valorA);
        resultado = (valorA & 0xFFFF0000) | (valorB & 0xFFFF);
        escribeMemoria(VM, instruc.valorA, resultado);
    } 
    else generaerror(ERROR_OPERANDO);
}



int random32() {
    return ((rand() & 0xFFFF) << 16) | (rand() & 0xFFFF);
}

void RND(TVM *VM, Instruccion instruc) {
    int SecA;
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
        escribeMemoria(VM, instruc.valorA, resultado);
    }
    else generaerror(ERROR_OPERANDO);
}

void PUSH(TVM *VM, Instruccion instruc) {
    unsigned int valor = 0;

    switch (instruc.sizeA) {
        case 2: // inmediato
            valor = instruc.valorA;
            break;

        case 1: // registro
        {
            int codReg, sec;
            DefinoRegistro(&codReg, &sec, instruc.valorA);
            LeerSectorRegistro((int *)&valor, VM, sec, codReg);
            break;
        }

        case 3: // memoria
            valor = leerMemoria(VM, instruc.valorA);
            break;

        default:
            generaerror(ERROR_OPERANDO);
            return;
    }

    push(VM, valor);
}

void POP(TVM *VM, Instruccion instruc) {
    unsigned int val;
    pop(VM, &val);

    switch (instruc.sizeA) {
        case 1: { // registro
            int codReg, sec;
            DefinoRegistro(&codReg, &sec, instruc.valorA);
            
            // CORREGIR: Truncar valor según el sector
            if (sec == 1 || sec == 2) val &= 0xFF;        // byte
            else if (sec == 3) val &= 0xFFFF;             // word
            // sec == 0 mantiene 32 bits completos
            
            escribirSectorRegistro(VM, codReg, sec, val);
            break;
        }
        case 3: { // memoria
            escribeMemoria(VM, instruc.valorA, val);
            break;
        }
        default:
            generaerror(ERROR_OPERANDO);
    }
}

void CALL(TVM *VM, Instruccion instruc) {
    unsigned int retorno = VM->reg[IP] & 0xFFFF; // guardo solo offset
    push(VM, retorno);

    unsigned int destino = instruc.valorA & 0xFFFF;
    VM->reg[IP] = (VM->reg[IP] & 0xFFFF0000) | destino;
}

void RET(TVM *VM, Instruccion instruc) {
    unsigned int dirRet = 0;
    pop(VM, &dirRet);
    VM->reg[IP] = (VM->reg[IP] & 0xFFFF0000) | (dirRet & 0xFFFF);
}

int resolverSaltoSeguro(TVM *VM, Instruccion instruc) {
    int codReg, offset,sec;

    // Resolver operando A (el destino del salto)
    switch (instruc.sizeA) {
        case 2: // inmediato
            offset = instruc.valorA;
            break;
        case 1: // registro
            DefinoRegistro(&codReg,&sec, instruc.valorA);
            LeerSectorRegistro(&offset, VM, sec, codReg);
            break;
        case 3: // memoria
            offset = leerMemoria(VM, VM->reg[OP1]);
            //debieria ser offset = leerMemoria(VM, instruc.valorA); ??????
            break;
        default:
            generaerror(ERROR_OPERANDO);
            return -1;
    }

    // Validar que el offset esté dentro del segmento de código
    int csIndex = (VM->reg[CS] >> 16) & 0xFFFF; 
    int tamCS   = VM->segmentos[csIndex].tam;

    if (offset < 0 || offset >= tamCS) {
        generaerror(ERROR_SEGMENTO);
        return -1;
    }
  
    return offset; // <-- Retorna offset lógico, NO dirección física
}

void JMP(TVM *VM, Instruccion instruc) {
    int destino = resolverSaltoSeguro(VM, instruc);
    if (destino != -1) VM->reg[IP] = (VM->reg[IP] & 0xFFFF0000) | (destino & 0xFFFF);
}

void JZ(TVM *VM, Instruccion instruc) {
    if (VM->reg[CC] & 0x40000000) { // Z=1 (bit 30)
        int destino = resolverSaltoSeguro(VM, instruc);
        if (destino != -1) VM->reg[IP] = (VM->reg[IP] & 0xFFFF0000) | (destino & 0xFFFF);
    }
}

void JNZ(TVM *VM, Instruccion instruc) {
    if (!(VM->reg[CC] & 0x40000000)) { //pregunta por el primer bit de cc
        int destino = resolverSaltoSeguro(VM, instruc);
        if (destino != -1) VM->reg[IP] = (VM->reg[IP] & 0xFFFF0000) | (destino & 0xFFFF);
    }
}

void JN(TVM *VM, Instruccion instruc) {
    if (VM->reg[CC] & 0x80000000) { // N=1 (bit 31)
        int destino = resolverSaltoSeguro(VM, instruc);
        if (destino != -1) VM->reg[IP] = (VM->reg[IP] & 0xFFFF0000) | (destino & 0xFFFF);
    }
}

void JNN(TVM *VM, Instruccion instruc) {
    if (!(VM->reg[CC] & 0x80000000)) {
        int destino = resolverSaltoSeguro(VM, instruc);
        if (destino != -1) VM->reg[IP] = (VM->reg[IP] & 0xFFFF0000) | (destino & 0xFFFF);
    }
}

void JP(TVM *VM, Instruccion instruc) {
    if (!(VM->reg[CC] & 0x80000000) && !(VM->reg[CC] & 0x40000000)) {
        int destino = resolverSaltoSeguro(VM, instruc);
        if (destino != -1) VM->reg[IP] = (VM->reg[IP] & 0xFFFF0000) | (destino & 0xFFFF);
    }
}

void JNP(TVM *VM, Instruccion instruc) {
    if ((VM->reg[CC] & 0x80000000) || (VM->reg[CC] & 0x40000000)) {
        int destino = resolverSaltoSeguro(VM, instruc);
        if (destino != -1) VM->reg[IP] = (VM->reg[IP] & 0xFFFF0000) | (destino & 0xFFFF);
    }
}

void NOT(TVM *VM, Instruccion instruc) {
    int SecA;
    int CodOpA;
    int valorA = 0, resultado = 0;

    if (instruc.sizeA == 1) {
        DefinoRegistro(&CodOpA, &SecA, instruc.valorA);
        LeerSectorRegistro(&valorA, VM, SecA, CodOpA);
        resultado = ~valorA;
        escribirSectorRegistro(VM, CodOpA, SecA, resultado);
    }
    else if (instruc.sizeA == 3) {
        valorA = leerMemoria(VM, instruc.valorA);
        resultado = ~valorA;
        escribeMemoria(VM, instruc.valorA, resultado);
    }
    else generaerror(ERROR_OPERANDO);

    actualizaCC(VM, resultado);
}


void STOP(TVM *VM, Instruccion instruc) {
    VM->reg[IP] = -1; // Detiene ejecución
    printf("[STOP] Ejecución finalizada\n");
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
            int codReg, sec;
            DefinoRegistro(&codReg, &sec, instruc.valorA);
            LeerSectorRegistro(&servicio, VM, sec, codReg);
            break;
        }
        case 3: // memoria
            servicio = leerMemoria(VM, VM->reg[OP1]);
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

    switch(servicio) {
        case 1: { //read
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

           for (int j = 0; j < size; j++) {
              VM->memory[dirFis + j] = (numero >> (8 * (size - 1 - j))) & 0xFF;
           }
        }  
        break;
    }
    case 2:{
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
        break;
        }
        case 0x0F: {
            printf("\n=== Breakpoint alcanzado ===\n");
            guardarVMI(VM, "imagen.vmi");
            printf("Imagen guardada. Opciones:\n");
            printf("[g] continuar | [q] salir | [Enter] paso a paso\n");

            char opcion = getchar();
            if (opcion == 'q') {
                printf("Ejecución finalizada por usuario.\n");
                exit(0);
            } else if (opcion == 'g') {
                printf("Continuando ejecución...\n");
            } else {
                printf("Ejecutando paso a paso...\n");
                // modo debug futuro
            }
            break;
        }
        case 3: {
           int logica, segmento, offset, tamMax;

            // Obtener dirección lógica destino desde EDX
            logica   = VM->reg[EDX];
            segmento = (logica >> 16) & 0xFFFF;
            offset   = logica & 0xFFFF;

            // Obtener cantidad máxima de caracteres desde ECX (parte baja)
            tamMax = VM->reg[ECX] & 0xFFFF;
            if (tamMax == -1 || tamMax > 255)
                tamMax = 255;

            // 3️⃣ Leer string desde teclado
            char buffer[1024];
            printf("[SYS3 INPUT]: ");
            fflush(stdout);
            fgets(buffer, sizeof(buffer), stdin);

            // Eliminar salto de línea final si lo hay
            size_t len = strlen(buffer);
            if (len > 0 && buffer[len - 1] == '\n')
                buffer[len - 1] = '\0';

            // Limitar longitud según tamMax
            if ((int)len > tamMax)
                buffer[tamMax] = '\0';

            int realLen = strlen(buffer);

            // 4️⃣ Escribir en memoria (1 byte por carácter, orden natural)
            for (int i = 0; i < realLen; i++) {
                int dirFis = getDirfisica(VM, offset + i, segmento, 1);
                if (dirFis == -1) {
                    generaerror(ERROR_SEGMENTO);
                    return;
                }
                VM->memory[dirFis] = (unsigned char) buffer[i];
            }

            // 5️⃣ Agregar terminador nulo (0x00)
            int dirFis = getDirfisica(VM, offset + realLen, segmento, 1);
            if (dirFis != -1)
                VM->memory[dirFis] = 0x00;

            printf("[SYS3] %d caracteres leídos y almacenados en memoria.\n", realLen);
            break;
        }
        case 4: {
            // Obtener dirección lógica inicial desde EDX
            int logica   = VM->reg[EDX];
            int segmento = (logica >> 16) & 0xFFFF;
            int offset   = logica & 0xFFFF;

            // Validar segmento
            if (segmento >= SEG_TABLE || segmento == 0xFFFF) {
                generaerror(ERROR_SEGMENTO);
                return;
            }

            // Imprimir carácter a carácter hasta encontrar '\0'
            printf("[SYS4 OUTPUT]: ");
            while (1) {
                int dirFis = getDirfisica(VM, offset, segmento, 1);
                if (dirFis == -1) {
                    generaerror(ERROR_SEGMENTO);
                    return;
                }

                unsigned char ch = VM->memory[dirFis];
                if (ch == 0x00) break; // fin de cadena
                printf("%c", ch);

                offset++; // avanzar
            }

            printf("\n");
            break;
        }
        case 7: {
            #ifdef _WIN32
                system("cls");   // Windows
            #else
                system("clear"); // Linux / macOS
            #endif
            break;
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


void guardarVMI(TVM *VM, char nombre[]) {
    FILE *archb;
    unsigned char b1, b2;
    int i;

    archb = fopen(nombre, "wb");
    if (archb == NULL) {
        printf("No se pudo crear el archivo de imagen .vmi\n");
        return;
    }

    printf("Generando imagen %s ...\n", nombre);

    // "VMI25" + version + tamaño de memoria (en KiB)
    fwrite("VMI25", 1, 5, archb);
    char version = 1;
    fwrite(&version, 1, 1, archb);
    int mem_kib = (int)(MEMORY_SIZE / 1024);
    b1 = (mem_kib >> 8) & 0xFF;
    b2 = (mem_kib) & 0xFF;
    fwrite(&b1, 1, 1, archb);
    fwrite(&b2, 1, 1, archb);

    // registros
    for (i = 0; i < 32; i++) {
         int val = VM->reg[i];
        char bytes[4];
        bytes[0] = (val >> 24) & 0xFF;
        bytes[1] = (val >> 16) & 0xFF;
        bytes[2] = (val >> 8) & 0xFF;
        bytes[3] = (val) & 0xFF;
        fwrite(bytes, 1, 4, archb);
    }

    //tabla de segmentos
    for (i = 0; i < 8; i++) {
        int base = VM->segmentos[i].base;
        int tam  = VM->segmentos[i].tam;

        b1 = (base >> 8) & 0xFF;
        b2 = (base) & 0xFF;
        fwrite(&b1, 1, 1, archb);
        fwrite(&b2, 1, 1, archb);

        b1 = (tam >> 8) & 0xFF;
        b2 = (tam) & 0xFF;
        fwrite(&b1, 1, 1, archb);
        fwrite(&b2, 1, 1, archb);
    }

    // === MEMORIA PRINCIPAL ===
    fwrite(VM->memory, 1, MEMORY_SIZE, archb);

    fclose(archb);
}

// -------------------- DISASSEMBLER CORREGIDO ------------------------//

void LeoDissasembler(TVM * VM, char VecFunciones[CANTFUNC][5], char VecRegistros[CANTREG][4]) {

    unsigned char CodOp;
    int CantOp, baseCS, tamCS;
    Instruccion instruc;
    int PosInicial, PosMemoria, PosFinal;

    int entry = VM->reg[IP] & 0xFFFF;

    // === MOSTRAR CADENAS DEL SEGMENTO CONSTANTE (KS) ===
    if (VM->reg[KS] != 0xFFFFFFFF) {
        int ks_index = (VM->reg[KS] >> 16) & 0xFFFF;
        int baseKS = getBase(VM->segmentos[ks_index]);
        int tamKS  = getTam(VM->segmentos[ks_index]);
        
        printf("=== Segmento de Constantes ===\n");
        int i = 0;
        while (i < tamKS) {
            int inicio = i;
            // Buscar el final de la cadena (terminador nulo)
            while (i < tamKS && VM->memory[baseKS + i] != 0) i++;
            int fin = i;

            if (fin > inicio) { // Solo mostrar si hay contenido
                printf("[%04X] ", baseKS + inicio);

                // Mostrar hasta 7 bytes en hex, si supera mostrar ".."
                int k;
                for (k = 0; k <= fin - inicio && k < 7; k++)
                    printf("%02X ", VM->memory[baseKS + inicio + k]);
                if ((fin - inicio) >= 7) printf(".. ");
                else {
                    // Completar espacios para alinear
                    while (k < 4) {
                        printf("   ");
                        k++;
                    }
                }

                printf("| \"");
                for (k = inicio; k < fin; k++) {
                    unsigned char c = VM->memory[baseKS + k];
                    if (c >= 32 && c <= 126) printf("%c", c);
                    else printf(".");
                }
                printf("\"\n");
            }

            i = fin + 1; // Saltar el terminador nulo
        }
        printf("\n");
    }

    // === DISASSEMBLER DEL CÓDIGO ===
    int cs_index = (VM->reg[CS] >> 16) & 0xFFFF;
    baseCS = getBase(VM->segmentos[cs_index]);
    tamCS  = getTam(VM->segmentos[cs_index]);
    
    if (tamCS <= 0) {
        printf("[ERROR] Segmento de código vacío o no definido (CS=%d)\n", cs_index);
        return;
    }

    PosMemoria = baseCS;
    PosFinal   = baseCS + tamCS;

    while (PosMemoria < PosFinal) {
        PosInicial = PosMemoria;
        ComponentesInstruccion(VM, PosMemoria, &instruc, &CantOp, &CodOp);
        SeteoValorOp(VM, PosMemoria, &instruc);

        PosMemoria += instruc.sizeA + instruc.sizeB + 1;

        // Marcar entry point con ">"
        if ((PosInicial - baseCS) == entry)
            printf(">");

        EscriboDissasembler(VM, VecFunciones, VecRegistros, CodOp, instruc, PosInicial, PosMemoria);
    }
}

void EscriboDissasembler(TVM *VM, char VecFunciones[CANTFUNC][5], char VecRegistros[CANTREG][4],
                         unsigned char CodOp, Instruccion instruc, int PosInicial, int PosFinal) {

    // 1. Dirección física en formato [0000]
    printf("[%04X] ", PosInicial);

    // 2. Bytes de la instrucción en hexadecimal
    for (int i = PosInicial; i < PosFinal; i++)
        printf("%02X ", VM->memory[i]);
    
    // Completar hasta 4 posiciones para alinear
    for (int i = PosFinal; i < PosInicial + 4; i++)
        printf("   ");

    printf("| %-5s ", VecFunciones[CodOp]);

    // 3. Operando A
    if (instruc.sizeA) {
        if (instruc.sizeA == 1) { // Registro
            int codReg, sec;
            DefinoRegistro(&codReg, &sec, instruc.valorA);
            MostrarPseudonimo(codReg, sec, VecRegistros);
        } else if (instruc.sizeA == 2) { // Inmediato
            printf("%d", instruc.valorA);
        } else if (instruc.sizeA == 3) { // Memoria con modificadores
            MostrarOperandoMemoria(VM, instruc.valorA, VecRegistros);
        }
    }

    // 4. Operando B
    if (instruc.sizeB) {
        printf(", ");
        if (instruc.sizeB == 1) { // Registro
            int codReg, sec;
            DefinoRegistro(&codReg, &sec, instruc.valorB);
            MostrarPseudonimo(codReg, sec, VecRegistros);
        } else if (instruc.sizeB == 2) { // Inmediato
            printf("%d", instruc.valorB);
        } else if (instruc.sizeB == 3) { // Memoria con modificadores
            MostrarOperandoMemoria(VM, instruc.valorB, VecRegistros);
        }
    }

    printf("\n");
}

// -------------------- FUNCIONES AUXILIARES ------------------------//

void MostrarPseudonimo(int codReg, int sec, char VecRegistros[CANTREG][4]) {
    
    if (codReg >= EAX && codReg <= EFX) {
        switch (sec) {
            case 0: printf("%s", VecRegistros[codReg]); break;     // EAX (32 bits)
            case 1: printf("%cL", VecRegistros[codReg][1]); break; // AL (byte bajo)
            case 2: printf("%cH", VecRegistros[codReg][1]); break; // AH (byte alto)
            case 3: printf("%cX", VecRegistros[codReg][1]); break; // AX (16 bits)
        }
    } else {
        // Para otros registros, mostrar nombre completo
        printf("%s", VecRegistros[codReg]);
    }
}

void MostrarOperandoMemoria(TVM *VM, int operando, char VecRegistros[CANTREG][4]) {

    int sizeBits = (operando >> 30) & 0x3;
    int size = sizeBits; // 00=long(4), 01=word(2), 10=byte(1), 11=reservado
    
    // Extraer registro base (bits 29-25, no 28-24)
    int regBase = (operando >> 25) & 0x1F;
    
    // Offset sigue siendo bits 15-0
    int offset = operando & 0xFFFF;
    if (offset & 0x8000) 
        offset |= 0xFFFF0000;
    
    // Mostrar modificador correcto
    switch (sizeBits) {
        case 0: printf("["); break;      // long (4 bytes) - por defecto
        case 1: printf("w["); break;     // word (2 bytes)
        case 2: printf("b["); break;     // byte (1 byte)
        case 3: printf("?["); break;     // reservado
    }
    
    printf("%s", VecRegistros[regBase]);
    
    if (offset > 0) {
        printf("+%d", offset);
    } else if (offset < 0) {
        printf("%d", offset);
    }
    
    printf("]");
}
#include <stdio.h>
#include <stdint.h>

#define MEMORY_SIZE 16384           // 16 KiB
uint8_t memory[MEMORY_SIZE]; // Memoria principal

// ========== INSTRUCCIONES ==========
 typedef enum {
    OP_STOP  = 0x0F,  
    OP_SYS   = 0x00,    
    OP_JMP   = 0x01,    
    OP_JZ    = 0x02,    
    OP_JP    = 0x03,    
    OP_JN    = 0x04,    
    OP_JNZ   = 0x05,    
    OP_JNP   = 0x06,    
    OP_JNN   = 0x07,    
    OP_NOT   = 0x08,  
    OP_MOV   = 0x10,    
    OP_ADD   = 0x11,    
    OP_SUB   = 0x12,    
    OP_MUL   = 0x13,    
    OP_DIV   = 0x14,      
    OP_CMP   = 0x15,    
    OP_SHL   = 0x16,    
    OP_SHR   = 0x17,    
    OP_SAR   = 0x18,    
    OP_AND   = 0x19,    
    OP_OR    = 0x1A,    
    OP_XOR   = 0x1B,    
    OP_SWAP  = 0x1C,    
    OP_LDL   = 0x1D,    
    OP_LDH   = 0x1E,    
    OP_RND   = 0x1F,  
} CodigosOperacion;

// ========== REGISTROS ==========
typedef enum { 
    REG_LAR = 0,
    REG_MAR,        // = 1
    REG_MBR,        // = 2
    REG_IP,         // = 3
    REG_OPC,        // = 4
    REG_OP1,        // = 5
    REG_OP2,        // = 6
    // ...
    REG_EAX = 10,   
    REG_EBX,        // = 11
    REG_ECX,        // = 12
    REG_EDX,        // = 13
    REG_EEX,        // = 14
    REG_EFX,        // = 15
    REG_AC,         // = 16
    REG_CC,         // = 17
    // ...
    REG_CS = 26,    
    REG_DS,         // = 27
    REG_COUNT = 32  // Total de registros
} CodigosRegistros;
uint32_t reg[REG_COUNT]; // Almacenamiento de registros

typedef enum {
  OP_NONE = 0x00,
  OP_REGISTRO = 0x01,
  OP_INMEDIATO = 0x02,
  OP_MEMORIA = 0x03,
} TipoOperando;

typedef struct  {
  TipoOperando tipoOp;
  uint32_t valor; //2 bytes son cod op y el resto son el valor 
} Operando;

typedef struct {
  uint16_t base; //dirreccion base (2 bytes) // uint16_t 16 bits sin signo
  uint16_t tam; // tamanio d el segmento
} DescribeSegmentos; 

#define MASK_OP 0x00FFFFFF
#define MASK_CC_N 0x80000000
#define MASK_CC_Z 0x40000000

void MOV(Instruccion inst) 
{
  uint32_t valor;

  // Obtener valor del operando 2
  switch (TOp2) {
    case OP_REGISTRO: 
      valor = reg[Op2];
      break;
    case OP_INMEDIATO: 
      valor = Op2;
      break;
    case OP_MEMORIA: 
      // Cargar desde memoria
      break;
  }

  // Almacenar valor en el operando 1
  switch (TOp1) {
    case OP_REGISTRO: 
      reg[Op1] = valor;
      break;
    case OP_MEMORIA: 
      // Almacenar en memoria
      break;
  }
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
  */



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


int getvalue() {
  int value = op & MASK_OP;
  return value;
}

int main() {
  Instruccion inst;

  reg[REG_IP] = base; //base code segment leida en el header
  int running = 1;

  while (running) {
    //cargar regip

    getInstruction(leido); 
    //actualiza ip
    //memoria

    instruccion[instr.opc];


  }
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
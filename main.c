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
  OP_NONE = 0x00;
  OP_REGISTRO = 0x01;
  OP_INMEDIATO = 0x02;
  OP_MEMORIA = 0x03;
} TipoOperando;

typedef struct  {
  TipoOperando tipoOp;
  uint32_t valor; //2 bytes son cod op y el resto son el valor 
} Operando;

typedef struct {
  uint8_t opc;
  Operando op1;
  Opernado op2;
} Instruccion;

typedef struct {
  uint16_t base; //dirreccion base (2 bytes) // uint16_t 16 bits sin signo
  uint16_t tam; // tamanio d el segmento
} DescribeSegmentos; 

#define MASK_OP 0x00FFFFFF
#define MASK_CC_N 0x80000000
#define MASK_CC_Z 0x40000000

void MOV(uint8_t TOp2, uint8_t TOp1, uint32_t Op2, uint32_t Op1)
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

void decodifica() {
  Instruccion inst;
  int tipoOp1,tipoOp2,opc;

  if ( (leido>>4) & 0x01 == 0) { //preguntar por el bit 4
    tipoOp1 = getOp2(leido) // es de un solo opeando
    opc = 10+ getOpc();
  }
  else {
    tipoOp2 = getOp2(leido); //10110000 -> 00000010 y mascara con 00000011 -> me quedo con el valor del tipo d op 1
    tipoOp1 = getOp1(leido);
    opc = getOpc();
  }

  if (tipoOp1 == 3 || tipoOp2 == 3) {
    cargaMemoria();
  }
  

}


int getvalue(op) {
  int value = op & MASK_OP;
  return value;
}

int main() {
    printf("Prueba commit");
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
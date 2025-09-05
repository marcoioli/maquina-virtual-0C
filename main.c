#include <stdio.h>

#define MEMORY_SIZE 16384           // 16 KiB
#define CANT_REGISTROS 32

int memory[MEMORY_SIZE]; //uint32???? 

typedef enum {
    REG_LAR = 0,    // Logical Address Register
    REG_MAR = 1,    // Memory Address Register  
    REG_MBR = 2,    // Memory Buffer Register
    REG_IP  = 3,    // Instruction Pointer
    REG_OPC = 4,    // Codigo de operacion
    REG_OP1 = 5,    // Operando 1
    REG_OP2 = 6,    // Operando 2

    REG_EAX = 10,   // uso general
    REG_EBX = 11,   // uso general
    REG_ECX = 12,   // uso general
    REG_EDX = 13,   // uso general
    REG_EEX = 14,   // uso general
    REG_EFX = 15,   // uso general
    REG_AC  = 16,   // Acumulador
    REG_CC  = 17,   // Condition Code

    REG_CS  = 26,   // Code Segment
    REG_DS  = 27    // Data Segment

} codigosRegistros;

typedef enum {
   OP_SYS  = 0x00,  OP_JMP  = 0x01,  OP_JZ   = 0x02,  OP_JP   = 0x03,
    OP_JN   = 0x04,  OP_JNZ  = 0x05,  OP_JNP  = 0x06,  OP_JNN  = 0x07,
    OP_NOT  = 0x08,  OP_STOP = 0x0F,  OP_MOV  = 0x10,  OP_ADD  = 0x11,
    OP_SUB  = 0x12,  OP_MUL  = 0x13,  OP_DIV  = 0x14,  OP_CMP  = 0x15,
    OP_SHL  = 0x16,  OP_SHR  = 0x17,  OP_SAR  = 0x18,  OP_AND  = 0x19,
    OP_OR   = 0x1A,  OP_XOR  = 0x1B,  OP_SWAP = 0x1C,  OP_LDL  = 0x1D,
    OP_LDH  = 0x1E,  OP_RND  = 0x1F
} codigosOperacion;

typedef enum {
  OP_NONE = 0x00;
  OP_REGISTRO = 0x01;
  OP_INMEDIATO = 0x02;
  OP_MEMORIA = 0x03;
} tipoOperando;

struct operando {
  tipoOperando tipoOp;
  uint24_t valor; //2 bytes son cod op y el resto son el valor 
}

typedef struct {
  uint16_t base; //dirreccion base (2 bytes) // uint16_t 16 bits sin signo
  uint16_t tam; // tamanio d el segmento
} describeSegmentos; 


uint32_t registro[CANT_REGISTROS];

int getvalue(op) {
  int value = op & MASK;
  return value;
}

int main() {
    printf("Prueba commit");
    return 0;
}

#define MEMORY_SIZE 16384           // 16 KiB
#define CANTREG 32
#define SEG_TABLE 8
#define CANTFUNC 26
#define MASK_OP 0x00FFFFFF
#define MASK_CC_N 0x80000000
#define MASK_CC_Z 0x40000000
#define TAMID 5

//  REGISTROS
#define LAR 0 //
#define MAR 1 //
#define MBR 2 //
#define IP 3 //instruction pointer
#define OPC 4
#define OP1 5
#define OP2 6
//6 7 8 RESERVADOS
#define EAX 10 //uso general
#define EBX 11
#define ECX 12
#define EDX 13
#define EEX 14
#define EFX 15
#define AC 16 //acumulador
#define CC 17 //condition code
//18 - 28 RESERVADOS
#define CS 26 //code segment
#define DS 27 //data segment
//28 - 31 RESERVADOS

int reg[CANTREG]; // Almacenamiento de registros

typedef struct {
    char id[TAMID]; //caracter por caracter o string?
    char version;
    unsigned short int tam;
}THeader;

typedef struct  {
    int reg[CANTREG];
    int segmentos[SEG_TABLE]; //DescribeSegmentos segmentos[seg_table]; ????????????????
    unsigned char memory[MEMORY_SIZE];
    //errores
} TVM;

typedef struct {
  int base; //dirreccion base (2 bytes) // uint16_t 16 bits sin signo
  int tam; // tamanio d el segmento
} DescribeSegmentos;

typedef struct {
  int valorA;
  int valorB;
  int sizeA;
  int sizeB;
  unsigned char inst; //ver par que es bien esto
} Instruccion;

typedef void (*vFunciones[CANTFUNC])(TVM *VM,Instruccion inst); //Vector de punteros a funciones
/*cada funcion tiene una posiicion relacionada con el codigo de operacion y se ejecuta la funcion en se indice
cada funcion esta declarada en vm.c
*/


//funciones del programa
void declaraFunciones(vFunciones Funciones);
void iniciaRegs(TVM * VM,int tam);
void cargaSegmentos(TVM * VM,int tam);
void leoArch(TVM * VM);
int getBase(int valor);
int getTam(int valor);
int direccionamiento_logtofis(TVM * VM, int puntero);
void ComponentesInstruccion(TVM *VM, int DirFisica, Instruccion *instr, int *CantOp, unsigned char *CodOp);
void SeteoValorOp(TVM *VM,int DirFisicaActual,Instruccion *instr);
void leeIP(TVM * VM);

void MOV(TVM * VM, Instruccion instruc);
void DIV(TVM * VM, Instruccion instruc);
void ADD(TVM * VM, Instruccion instruc);
void MUL(TVM * VM, Instruccion instruc);
void SUB(TVM * VM, Instruccion instruc);
void CMP(TVM * VM, Instruccion instruc);
void SHR(TVM * VM, Instruccion instruc);
void SHL(TVM * VM, Instruccion instruc);
void SAR(TVM * VM, Instruccion instruc);
void AND(TVM *VM, Instruccion instruc);
void OR(TVM *VM, Instruccion instruc);
void XOR(TVM *VM, Instruccion instruc);
void SWAP(TVM *VM, Instruccion instruc);
void LDH(TVM *VM, Instruccion instruc);
void LDL(TVM *VM, Instruccion instruc);


void actualizaCC(TVM *VM, int resultado);
int guardaB(TVM *VM, Instruccion instruc);

int leerMemoria(TVM *VM, int dirLogica, int size) ;
void escribeMemoria(TVM * MV,int dirLogica, int valor, int size);
void DefinoRegistro(int *CodReg, int Op);
void DefinoAuxRegistro(int *AuxR,TVM VM, int CodReg);



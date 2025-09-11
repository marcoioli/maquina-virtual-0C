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

typedef struct THeader{
    char id[TAMID]; //caracter por caracter o string?
    char version;
    unsigned short int tam;
}THeader;

typedef struct TVM {
    int reg[CANTREG];
    int segmentos[SEG_TABLE]; //DescribeSegmentos segmentos[seg_table]; ????????????????
    /*tipo*/ memory[MEMORY_SIZE];
    //errores 
} TVM

typedef struct {
  uint16_t base; //dirreccion base (2 bytes) // uint16_t 16 bits sin signo
  uint16_t tam; // tamanio d el segmento
} DescribeSegmentos; 

typedef struct  Operando{
  TipoOperando tam;
  uint32_t valor; //2 bytes son cod op y el resto son el valor 
} Operando;

typedef struct {
  Operando op1;
  Operando op2;
  int sizeA;
  int sizeB;
  unsigned char inst; //ver par que es bien esto
} Instruccion;

typedef void (*vFunciones[CANTFUNC])(TMV *mv,Instruccion inst); //Vector de punteros a funciones
/*cada funcion tiene una posiicion relacionada con el codigo de operacion y se ejecuta la funcion en se indice
cada funcion esta declarada en vm.c
*/


//funciones del programa
void declaraFunciones(vFunciones Funciones);
void iniciaRegs(TVM * VM,int tam);
void cargaSegmentos(TVM * VM,int tam);
void leoArch(TVM * VM, int tam);
int getBase(int valor);
int getTam(int valor);
int direccionamiento_logtofis(TVM VM, int puntero);
void ComponentesInstruccion(TVM VM, int DirFisica, Instruccion *instr, int *CantOp, unsigned char *CodOp)
void SeteoValorOp(TVM VM,int DirFisicaActual,Instruccion *instr);
void leeIP(TVM * VM);

#define MEMORY_SIZE 16384           // 16 KiB
#define CANTREG 32
#define SEG_TABLE 8
#define CANTFUNC 32
#define MASK_OP 0x00FFFFFF
#define MASK_CC_N 0x80000000
#define MASK_CC_Z 0x40000000
#define TAMID 5
#define CANTSEGMENTOS 8

#define SEG_CS 0
#define SEG_DS 1

//  REGISTROS
#define LAR 0 //
#define MAR 1 //
#define MBR 2 //
#define IP 3 //instruction pointer
#define OPC 4
#define OP1 5
#define OP2 6
#define SP 7
#define BP 8
#define EAX 10
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
#define ES 28
#define SS 29
#define KS 30
#define PS 31

#define ERROR_INSTRUCCION 1
#define ERROR_DIVISION_POR_CERO 2
#define ERROR_SEGMENTO 3
#define ERROR_OPERANDO 4


typedef struct THeader{
    unsigned char c1,c2,c3,c4,c5;  //caracter por caracter o string?
    char version;
    unsigned short int tam;
    int code_size, data_size, extra_size, stack_size, const_size, entry_offset;
}THeader;

typedef struct  {
    int reg[CANTREG];
    TSegmento segmentos[SEG_TABLE]; //DescribeSegmentos segmentos[seg_table]; ????????????????
    unsigned char memory[MEMORY_SIZE];
    int param_size;
    //errores
} TVM;

typedef struct TSegmento {
  int base; //dirreccion base (2 bytes) // uint16_t 16 bits sin signo
  int tam; // tamanio d el segmento
} TSegmento;

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

void inicializoVecFunciones(char VecFunciones[CANTFUNC][5]);
void inicializoVecRegistros(char VecRegistros[CANTREG][4]);

//funciones del programa
void declaraFunciones(vFunciones Funciones);
void iniciaRegs(TVM *VM, int entry_offset)
void cargaSegmentos(TVM *VM, THeader header)
void cargaParametros(TVM *VM, int cant, char *params[])
void leoArch(TVM * VM, char nomarch[]);
void leoVMI(TVM *VM, char nomarch[]);
int getBase(int valor);
int getTam(int valor);
int getDirfisica(TVM *VM, int offset,int segmento, int size);
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
void RND(TVM *VM, Instruccion instruc);
void JMP(TVM *VM, Instruccion instruc);
void JZ(TVM *VM, Instruccion instruc);
void JN(TVM *VM, Instruccion instruc);
void JNZ(TVM *VM, Instruccion instruc);
void JNN(TVM *VM, Instruccion instruc);
void JP(TVM *VM, Instruccion instruc);
void JNP(TVM *VM, Instruccion instruc);
void NOT(TVM *VM, Instruccion instruc);
void SYS(TVM *VM, Instruccion instruc);
void STOP(TVM * VM,Instruccion instruc);
void PUS(TVM *VM, unsigned int valor);
void POP(TVM *VM, unsigned int *valor);
void CALL(TVM *VM, unsigned int destino);
void RET(TVM *VM);



int leer_binario_c2_32();
void imprimir_binario_32(int valor);
void imprimir_binario_nbits(int valor,int size);

int random32();
int resolverSaltoSeguro(TVM *VM, Instruccion instruc);
void actualizaCC(TVM *VM, int resultado);
int guardaB(TVM *VM, Instruccion instruc);
int leerMemoria(TVM *VM, int dirLogica, int size) ;
void escribeMemoria(TVM * MV,int dirLogica, int valor, int size);
void DefinoRegistro(int *CodReg, int Op);
void DefinoAuxRegistro(int *AuxR,TVM VM, int CodReg);

void guardarVMI(TVM *VM, char nombre[]);

void LeoDissasembler(TVM * VM,char VecFunciones[CANTFUNC][5],char VecRegistros[CANTREG][4]);
void EscriboDissasembler(TVM *VM, char VecFunciones[CANTFUNC][5], char VecRegistros[CANTREG][4], unsigned char CodOp, Instruccion instruc, int PosInicial, int PosFinal);
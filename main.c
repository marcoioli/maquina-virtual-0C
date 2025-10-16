#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mv.h"


int main(int argc, char *argv[]) {
    TVM VM = {0};
    char VecFunciones[CANTFUNC][5]; //5 Es la cantidad de caracteres que tiene como maximo el nombre de la funcion.
    char VecRegistros[CANTREG][4];
    char nomarch[50];
    THeader header;
    int i;
    char *vmx_filename = NULL;
    char *vmi_filename = NULL;
    bool disassembler_flag = false;
    int params_start_index = -1;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0) {
            params_start_index = i + 1;
            break; // -p siempre es el último flag
        } else if (strcmp(argv[i], "-d") == 0) {
            disassembler_flag = true;
        } else if (strstr(argv[i], ".vmx")) {
            vmx_filename = argv[i];
        } else if (strstr(argv[i], ".vmi")) {
            vmi_filename = argv[i];
        }
    }
    if (!vmx_filename && !vmi_filename) {
        fprintf(stderr, "Error: Se requiere un archivo de entrada (.vmx o .vmi).\n");
        return 1;
    }
    if (vmi_filename) {
        VM.vmi_filename = vmi_filename;
    }

    if (vmx_filename) {
        // --- Cargar y ejecutar un programa .vmx ---
        int param_count = 0;
        if (params_start_index != -1) {
            param_count = argc - params_start_index;
            cargaParametros(&VM, param_count, &argv[params_start_index]);
        } else {
            VM.param_size = 0;
        }
        leoArch(&VM, vmx_filename,param_count,&argv[params_start_index]);
        //si estra -d muestra el dissasembler
        if (disassembler_flag) {
            inicializoVecFunciones(VecFunciones);
            inicializoVecRegistros(VecRegistros);
            LeoDissasembler(&VM, VecFunciones, VecRegistros);
        }

        printf("Comienza a leer IP \n");
        leeIP(&VM);

    } else {
        // comienza a leer desde una imagen
        leoVMI(&VM, vmi_filename);
        printf("Imagen VMI cargada. Continuando ejecución...\n");
        leeIP(&VM);
    }
    return 0;
}



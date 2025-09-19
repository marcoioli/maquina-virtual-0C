; ===== Decimal a Binario (simplificado) =====
; Lee un número decimal e imprime sus bits de derecha a izquierda (LSB -> MSB)

inicio:   ; leer un número en decimal
          mov   eax, 0x01       ; modo lectura decimal
          mov   edx, DS
          add   edx, 0
          ldh   ecx, 0x04
          ldl   ecx, 0x01
          sys   0x1             ; READ → [DS+0]

          mov   eax, [0]        ; EAX = número ingresado

loop:     cmp   eax, 0
          jz    fin             ; si EAX == 0, termino

          mov   ebx, eax
          and   ebx, 1          ; aislar bit menos significativo

          cmp   ebx, 0
          jz    bit_cero

bit_uno:  mov   [DS+4], '1'
          jmp   imprimir

bit_cero: mov   [DS+4], '0'

imprimir: mov   edx, DS
          add   edx, 4          ; apuntar al char
          ldh   ecx, 0x01       ; tamaño 1 byte
          ldl   ecx, 0x01       ; 1 celda
          mov   eax, 0x02       ; salida en caracteres
          sys   0x2

          shr   eax, 1          ; desplazo el número a la derecha
          jmp   loop

fin:      stop




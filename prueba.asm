inicio:   ; Leer un número en decimal
          mov   eax, 0x01       ; modo lectura decimal
          mov   edx, DS
          add   edx, 0
          ldh   ecx, 0x04       ; tamaño de celda = 4 bytes
          ldl   ecx, 0x01       ; cantidad de celdas = 1
          sys   0x1             ; READ → guarda en [DS+0]

          ; Mostrar el número
          mov   eax, 0x01       ; modo escritura decimal
          mov   edx, DS
          add   edx, 0
          ldh   ecx, 0x04
          ldl   ecx, 0x01
          sys   0x2             ; WRITE → imprime [DS+0]

          stop
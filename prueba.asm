
limpiar:  mov   edx, DS        ; EDX = base de datos
          mov   eax, 0         ; valor 0 para escribir
loop:     cmp   ecx, 0
          jz    fin_limpia     ; si contador llegó a 0 → terminar
          mov   [edx], eax     ; escribir 0 en la celda
          add   edx, 4         ; avanzar a la siguiente celda
          add   ecx, -1        ; decrementar contador
          jmp   loop

fin_limpia: STOP

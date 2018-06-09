.486 
.model flat, stdcall 
option casemap :none 

include windows.inc ; always first 

include masm32.inc 
include gdi32.inc 
include user32.inc 
include kernel32.inc 

includelib masm32.lib 
includelib gdi32.lib 
includelib user32.lib 
includelib kernel32.lib 

.data 
Intro db "Enter String: ", 0 
buf db 300 dup(' ') 
szFormat db "Result = %d", 0 
cur dd 0 
curc byte 0 

save byte 0 
now dd 0 
save2 dd 0 
save1 dd 0  
 
before dd 0 
index dd 0 
count dd 0 
save3 dd 0 
Buffer1 byte 256 dup(0) 
.data? 
Buffer byte 65536 dup(?) 
.code 
MainProc proc 
    invoke StdOut, addr Intro 
    invoke StdIn, addr Buffer, 300 
    sub eax,2
    mov ecx, eax 
    PNT1 : 
    mov esi, cur 
    mov al, Buffer[esi] 
    cmp curc, al 
    mov save1, ecx 
    jl LS1 
    cmp al, ' ' 
    je PNT2 
    cmp al, 0 
    je PNT2 
    jmp PNT3 
    LS1 : 
    mov esi, count 
    mov Buffer1[esi], al 
    mov curc, al 
    inc cur 
    inc count 
    jmp PNT4 
    PNT2 : 
    mov save, cl 
    cmp curc,' ' 
    je PNT3 
    mov esi, count 
    mov Buffer1[esi], al 
    mov curc, al 
    inc cur 
    inc count 
    mov ecx, count 
    mov save2, esi 
    mov esi, now 
    
    
    cmp count, 1 
    jg GR2 
    mov curc, 0 
    mov ecx, save1 
    jmp PNT4 
    PNT3 : 
    mov save2, esi 
    
    mov ecx, count 
    cmp count, 1 
    jl LS2 
  
    WHL2 : 
    mov Buffer1[ecx - 1], 0 
    loop WHL2 
    LS2: 
    mov esi, save2 
    mov ecx, save1 
    inc cur 
    mov count, 0 
    mov curc, 127 
    PNT4 : 
    inc esi 
    dec cl 
    jnz PNT1 
    GR2 : 
    invoke StdOut, addr Buffer1 
    invoke StdIn, addr Buffer, 300 
    
    
    ret 

MainProc endp 


end MainProc
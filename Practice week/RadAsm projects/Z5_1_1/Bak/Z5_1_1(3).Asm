.486
.model flat, stdcall                    
option casemap :none

include windows.inc     ; always first

include masm32.inc
include gdi32.inc
include user32.inc
include kernel32.inc

includelib masm32.lib
includelib gdi32.lib
includelib user32.lib
includelib kernel32.lib

.data
    szEnterMatrix db  "Enter elements", 13, 10, 0
    buf                 db  300 dup(' ')
    Intro              db  'Input n', 13, 10, 0
    OutSpace            db  " ", 0
    OutNewLine           db  " ", 13,10,0
    ResStr              db  16 DUP(' ')
    Buffer              db  10 DUP (' ')
    szFormat            db  "%d:%d", 0
    count dw 0
.data?
    bufa             sbyte  ?
    mat              sbyte  65536 dup(?)
    n                 byte  ?
.code

MainProc  proc
            invoke  StdOut,addr Intro
            
            invoke  StdIn,addr Buffer, LengthOf Buffer
            invoke  StripLF,addr Buffer
            invoke  atol,addr Buffer
       
            ;xchg    al,bl
            ;movzx   eax,bl
            mov byte ptr n, al         
            mul n
            mov ecx, eax
            mov esi, 0
            pushad
            invoke StdOut, addr szEnterMatrix
            popad
            
            cmp ecx,0
            jz bad_end_
          
          
            FillMatrix:
                pushad
                invoke StdIn,addr Buffer, LengthOf Buffer
                invoke StripLF,addr Buffer
                invoke atol,addr Buffer
                mov bufa,al
                popad
                mov al,bufa
                mov [mat][esi], al
                inc esi
            loop FillMatrix
    
            mov esi,0
            mov ecx, dword ptr n
            Show1:
                push ecx
                mov ecx,dword ptr n
                    Show2:
                    pushad
                    mov al,byte ptr [mat][esi]
                    test al,         80h
                    jz Not_
                    not al
                    not eax
                    Not_:
                    invoke dwtoa, eax, addr ResStr
                    invoke StdOut, addr ResStr
                    invoke StdOut, addr OutSpace
                    popad
                    inc esi
                    loop Show2
                pushad
                invoke StdOut, addr OutNewLine
                popad
                pop ecx
            loop Show1
        
            mov esi,0
            mov ecx,dword ptr n
            
            
            Count1:
                mov eax,0;counter
                mov esi,dword ptr n
                sub esi,ecx
                push ecx
                mov ecx, dword ptr n
                    Count2:
                    cmp [mat][esi],0
                    jg Gr
                    add esi,dword ptr n
                    cmp ecx,1
                    jne Neql
                    add esi,dword ptr n
                    mov eax,0
                    jmp Skip
                    Count1_:
                    jmp Count1
                    Neql:
                    dec ecx ; ?
                        Count3:
                        cmp [mat][esi],0
                        jl Ls
                        inc eax
                        Ls:
                        add esi,dword ptr n
                        loop Count3
                    jmp Skip 
                    Gr:
                    add esi,dword ptr n
                    loop Count2
                Skip:
                pop ecx
                mov dword ptr count,eax
                pushad
                mov ebx,dword ptr n
                sub ebx,ecx
                inc ebx
                invoke  wsprintf,addr ResStr,addr szFormat,  ebx,eax
                invoke  StdOut,addr ResStr
                invoke  StdOut,addr OutSpace
                popad
            loop Count1_
            jmp     end_
        bad_end_:
            mov eax, 0
        end_:            
            invoke  StdIn,addr buf, 300
            invoke  ExitProcess, 0
MainProc    endp

end MainProc
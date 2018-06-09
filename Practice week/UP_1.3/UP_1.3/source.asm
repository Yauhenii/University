.386
PUBLIC @atohex@4
.model flat
.data?
str1 db 32 dup (?)
.data
hex db '0123456789ABCDEF'
.code
@atohex@4 proc
lea edi,str1
mov esi, ecx

mov eax, esi
xor ecx, ecx
_while1 :
shr eax, 4
inc ecx
cmp eax, 0
jnz _while1

mov ebx,'h'
mov [edi][ecx],ebx
dec ecx
mov eax, esi

_while2 :
mov ebx, 15
and ebx, eax
mov dl, hex[ebx]
mov [edi][ecx], dl
dec ecx
shr eax, 4
cmp eax, 0
jnz _while2
mov eax,edi
ret 
@atohex@4 endp

.model flat
PUBLIC _MemoryToHex
.code
_MemoryToHex proc
mov al, 0
mov edi, [esp + 12] ; buff
mov ecx, 9
beg_2:
mov  [edi], al
inc edi
loop beg_2

mov edi, [esp + 12]
lea ebx,hex
mov edx, [esp + 4]	; adr
mov ecx, [esp + 8]	; size 
beg_2_2:

mov al, [edx]
shr al, 4	
xlat
add [edi], al
inc edi

mov al, [edx]	
and al, 15
xlat
add [edi], al

inc edi
inc edx
loop beg_2_2
ret
_MemoryToHex endp

end
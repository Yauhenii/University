.386
PUBLIC _findCloseNum@12
.model flat
.code
_findCloseNum@12 proc
push ebp
mov ebp,esp
mov ecx,[ebp+12] 
mov edi,[ebp+8] 

mov eax,[edi]
sub eax,[ebp+16]
cmp eax,0
jnl SKIP_1
mov edx,-1
imul edx
SKIP_1:
mov ebx,eax ;diff
mov esi,[edi] ;cl
add edi,4

dec ecx 
beg_:
mov eax,[edi]
sub eax,[ebp+16]
cmp eax,0
jnl SKIP
mov edx,-1
imul edx
SKIP:
cmp ebx,eax
jl LESS
mov ebx,eax
mov esi,[edi]
LESS:
add edi,4
loop beg_

mov eax,esi
pop ebp
ret 12
_findCloseNum@12 endp

PUBLIC @atobinom@4
.model flat
.data
bin db 8 dup (?)
revbin db 9 dup (?)
.code
@atobinom@4 proc
lea esi,bin
mov eax,ecx
mov ecx,8
beg_2_:
cmp eax,0
je EXIT

cdq
mov ebx,2
idiv ebx
cmp edx,0
je EQL
mov ebx,'1'
mov [esi],ebx
jmp SKIP2
EQL:
mov ebx,'0'
mov [esi],ebx
SKIP2:
inc esi
dec ecx
jmp beg_2_
EXIT:

beg_3_:
mov ebx,'0'
mov [esi],ebx
inc esi
loop beg_3_

lea esi,bin
lea edi,revbin

add esi,7
mov ecx,8

beg_4_:
mov eax,[esi]
mov [edi],eax
dec esi
inc edi
loop beg_4_

mov eax,'\0'
mov [edi],eax

lea edi,revbin
mov eax,edi
ret 
@atobinom@4 endp

end
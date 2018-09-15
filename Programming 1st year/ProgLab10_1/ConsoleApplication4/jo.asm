.386

PUBLIC @sumFast@8
.model flat
.code
@sumFast@8 proc
mov eax,edx
add eax,ecx
ret
@sumFast@8 endp


PUBLIC _sumStd@12
.model flat
.code
_sumStd@12 proc
push ebp
mov ebp,esp
mov eax, dword ptr [ebp+16] 
mov ebx, dword ptr [ebp+12] 
mov ecx, dword ptr [ebp+8] 
imul ecx
add eax,ebx
pop ebp
ret 12
_sumStd@12 endp

PUBLIC _sumCd
.model flat
.code
_sumCd proc
push ebp
mov ebp,esp
mov eax, dword ptr [ebp+16] 
mov ebx, dword ptr [ebp+12] 
mov ecx, dword ptr [ebp+8] 
imul ecx
add eax,ebx
pop ebp
ret 
_sumCd endp
end
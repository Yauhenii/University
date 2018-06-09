.586
PUBLIC @strtoken@8 
.model flat
.code 
@strtoken@8 proc 
xor eax, eax
mov [edx], ecx
mov edi, ecx
mov ecx, 64
mov eax, '\'
cld
repne scasb
mov byte ptr [edi-1], 0
mov eax, edi
ret 
@strtoken@8 endp
end
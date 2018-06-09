.586
.model flat
PUBLIC _findsol
option casemap:none
.data
xcur dd 0
xlas dd 0
xnex dd 0
fcur dd 0
flas dd 0
delta dd 0
area dd 0
iterat dd 0
perem dd 0
aflo dd 0
bflo dd 0
cflo dd 0
.code
_findsol proc

	mov esi, [esp + 16] ;root
	mov eax, [esp + 4] ;a
	mov aflo, eax
	mov eax, [esp + 8] ;b
	mov bflo, eax
	mov eax, [esp + 12] ;c
	mov cflo, eax

	finit
	fld dword ptr [esp + 20]; set leftb
	fst dword ptr xlas
		fadd aflo
		fmul xlas
		fadd bflo
		fmul xlas
		fadd cflo
	fstp dword ptr flas

	fld dword ptr [esp + 24]; set rightb
	fstp dword ptr xnex

	mov ecx, 40
beginroo:
	fld dword ptr xlas
	fadd xnex
	mov perem, 2
	fidiv perem
	fst dword ptr xcur

	fadd aflo; f(mid)
	fmul xcur
	fadd bflo
	fmul xcur
	fadd cflo
	fst dword ptr fcur
	
	fmul flas; comparing
	fldz
	fcompp
	fstsw ax
	test ax, 0100h
	jnz moveleft
		fld dword ptr xcur
		fstp dword ptr xnex
loop beginroo
	jmp endrooting
	moveleft:
		fld dword ptr xcur
		fstp dword ptr xlas
loop beginroo
endrooting:
	fld dword ptr xcur
	fstp dword ptr [esi]
	
	fld dword ptr [esp + 24]
	fld dword ptr xcur
	fcomi st,st(1)
	jne NEQL
	fldz
	jmp SKIP
	NEQL:
	fld1
	SKIP:
	ret
_findsol endp
end
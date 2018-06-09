.586
PUBLIC _compln
PUBLIC @userln@4
.model flat
.data
sum dd 0.0
 k dd 0
 th dd 4.0
 tw dd 2.0
.code 
_compln proc 
finit
fldln2
ret 
_compln endp
@userln@4 proc 
beg_:
mov k,ecx
fild k ;st(0)=k
fld st(0) ;
fmulp st(1),st
fld th
fmulp st(1),st
fld1
fsubp st(1),st(0)
fild k ;st(1)=k
fmulp st(1),st
fld1
fdivrp st(1),st(0)
fld sum
faddp st(1),st(0)
fstp sum
loop beg_
fld tw
fld sum
fld1
faddp st(1),st(0)
fdivrp st(1),st(0)
fldz 
fstp sum
ret 
@userln@4 endp
end
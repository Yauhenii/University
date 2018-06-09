.586
PUBLIC _integral
.model flat
.data
d dd 0.01; delta 
two dd 2.0
.code 
_integral proc 
finit

fldz ;st(0)=0 - sum=0

fldpi
fldpi
faddp st(1),st(0) ; st(0)=2*pi

fld d; st(0)=d
BEG_WHL:
fcomi st,st(1) ; st(0)=x st(1)=2*pi st(2)=sum
ja END_WHL
fld st(0) ;push x
fcos ; cosx
fmul st(0),st(1) ;xcosx  st(0)=xcosx st(1)=x st(2)=2*pi st(3)=sum
fld st(1) ;push x
fld d; push d
fsubp st(1),st(0) ; x-d pop d
fld st(0) ; push x-d
fcos ; cos(x-d)
fmulp st(1),st(0);(x-d)cos(x-d) pop cos(x-d)  st(0)=(x-d)cos(x-d) st(1)=xcosx st(2)=x st(3)=2*pi st(4)=sum
faddp st(1),st(0); st(0)=xcosx+(x-d)cos(x-d)  st(1)=x st(2)=2*pi st(3)=sum
fld two
fdivp st(1),st(0); st(0)=(xcosx+(x-d)cos(x-d))/2
fld d
fmulp st(1),st(0); st(0)=(xcosx+(x-d)cos(x-d))/2 *d
faddp st(3),st(0); st(0)=x st(1)=2*pi st(2)=sum+=(xcosx+(x-d)cos(x-d))/2 *d
fld d
faddp st(1),st(0); st(0)+=d
jmp BEG_WHL
END_WHL:
fld st(2)
ret 
_integral endp
end
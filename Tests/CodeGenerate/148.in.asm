.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%f %f %f", 0
	float0 dd 3.000000
	float1 dd 2.300000
	var_$b dd 3 dup(0)
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_main:
	push ebp
	mov ebp, esp
	sub esp, 16
	mov tmp4, 3
	fild tmp4
	fstp tmp4
	push tmp4
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push float0
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	mov eax, dword ptr [eax + 0]
	mov tmp4, eax
	fld tmp4
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	mov eax, dword ptr [eax + 0]
	mov tmp4, eax
	fld tmp4
	fdivp
	fstp tmp4
	mov eax, tmp4
	mov tmp4, eax
	fld tmp4
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	mov eax, dword ptr [eax + 0]
	mov tmp4, eax
	fld tmp4
	fmulp
	fstp tmp4
	push tmp4
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push offset var_$b
	mov eax, ebp
	mov ebx, -16
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	mov ebx, dword ptr [eax + 0]
	mov eax, dword ptr [ebp + -16]
	mov dword ptr [eax + 0], ebx
	push float1
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	mov eax, dword ptr [eax + 0]
	mov tmp4, eax
	fld tmp4
	fld float1
	fdivp
	fstp tmp4
	mov eax, dword ptr [ebp + -16]
	mov ebx, 4
	add eax, ebx
	mov ebx, tmp4
	mov dword ptr [eax + 0], ebx
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	mov eax, dword ptr [eax + 0]
	mov tmp4, eax
	fld tmp4
	fchs
	fstp tmp4
	mov eax, dword ptr [ebp + -16]
	mov ebx, 8
	add eax, ebx
	mov ebx, tmp4
	mov dword ptr [eax + 0], ebx
	mov eax, offset var_$b
	mov ebx, 8
	add eax, ebx
	push dword ptr [eax + 0]
	pop tmp4
	fld tmp4
	fstp tmp8
	mov eax, offset tmp8
	push dword ptr [eax + 4]
	push dword ptr [eax + 0]
	mov eax, offset var_$b
	mov ebx, 4
	add eax, ebx
	push dword ptr [eax + 0]
	pop tmp4
	fld tmp4
	fstp tmp8
	mov eax, offset tmp8
	push dword ptr [eax + 4]
	push dword ptr [eax + 0]
	mov eax, offset var_$b
	push dword ptr [eax + 0]
	pop tmp4
	fld tmp4
	fstp tmp8
	mov eax, offset tmp8
	push dword ptr [eax + 4]
	push dword ptr [eax + 0]
	invoke crt_printf, addr str0
	add esp, 24
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
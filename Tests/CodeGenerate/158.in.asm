.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%f %f", 0dh, 0ah, 0
	str1 db "%f %f", 0dh, 0ah, 0
	float0 dd 3.000000
	var_a dd 1 dup(0)
	var_b dd 1 dup(0)
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_main:
	push ebp
	mov ebp, esp
	fld var_a
	push float0
	fld float0
	faddp
	fstp tmp4
	mov eax, offset var_a
	mov ebx, tmp4
	mov dword ptr [eax + 0], ebx
	mov tmp4, 1
	fild tmp4
	fstp tmp4
	fld var_b
	push tmp4
	mov tmp4, 1
	fild tmp4
	fstp tmp4
	mov eax, tmp4
	mov tmp4, eax
	fld tmp4
	fsubp
	fstp tmp4
	mov eax, offset var_b
	mov ebx, tmp4
	mov dword ptr [eax + 0], ebx
	push dword ptr [var_b + 0]
	pop tmp4
	fld tmp4
	fstp tmp8
	mov eax, offset tmp8
	push dword ptr [eax + 4]
	push dword ptr [eax + 0]
	push dword ptr [var_a + 0]
	pop tmp4
	fld tmp4
	fstp tmp8
	mov eax, offset tmp8
	push dword ptr [eax + 4]
	push dword ptr [eax + 0]
	invoke crt_printf, addr str0
	add esp, 16
	fld var_a
	fld var_b
	fdivp
	fstp tmp4
	mov eax, offset var_a
	mov ebx, tmp4
	mov dword ptr [eax + 0], ebx
	fld var_a
	fld var_a
	fmulp
	fstp tmp4
	fld var_b
	fld var_a
	fld var_a
	fmulp
	push dword ptr [var_b + 0]
	push dword ptr [var_a + 0]
	push tmp4
	fstp tmp4
	mov eax, tmp4
	mov tmp4, eax
	fld tmp4
	fmulp
	fstp tmp4
	mov eax, offset var_b
	mov ebx, tmp4
	mov dword ptr [eax + 0], ebx
	push dword ptr [var_a + 0]
	push dword ptr [var_b + 0]
	pop tmp4
	fld tmp4
	fstp tmp8
	mov eax, offset tmp8
	push dword ptr [eax + 4]
	push dword ptr [eax + 0]
	push dword ptr [var_a + 0]
	pop tmp4
	fld tmp4
	fstp tmp8
	mov eax, offset tmp8
	push dword ptr [eax + 4]
	push dword ptr [eax + 0]
	invoke crt_printf, addr str1
	add esp, 16
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
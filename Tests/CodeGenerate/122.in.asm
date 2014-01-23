.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%f %f %f", 0
	float0 dd 0.550000
	var_a dd 1 dup(0)
	var_b dd 1 dup(0)
	var_c dd 1 dup(0)
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_main:
	push ebp
	mov ebp, esp
	mov eax, offset var_c
	mov ebx, float0
	mov dword ptr [eax + 0], ebx
	mov eax, offset var_b
	mov dword ptr [eax + 0], ebx
	mov eax, offset var_a
	mov dword ptr [eax + 0], ebx
	push dword ptr [var_c + 0]
	pop tmp4
	fld tmp4
	fstp tmp8
	mov eax, offset tmp8
	push dword ptr [eax + 4]
	push dword ptr [eax + 0]
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
	add esp, 24
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
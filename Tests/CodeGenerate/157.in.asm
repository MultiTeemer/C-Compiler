.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d", 0
	str1 db "%d", 0
	str2 db "%d", 0
	str3 db "%d", 0
	str4 db "%d", 0
	str5 db "%d", 0
	float0 dd 0.300000
	float1 dd 0.001000
	float2 dd 1000.000000
	float3 dd 0.090000
	var_a dd 1 dup(0)
	var_b dd 1 dup(0)
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_main:
	push ebp
	mov ebp, esp
	mov eax, offset var_a
	mov ebx, float0
	mov dword ptr [eax + 0], ebx
	fld var_a
	push float1
	fld float1
	fmulp
	fstp tmp4
	mov eax, offset var_b
	mov ebx, tmp4
	mov dword ptr [eax + 0], ebx
	mov eax, dword ptr [var_a + 0]
	cmp eax, dword ptr [var_b + 0]
	mov eax, 0
	setl al
	push eax
	invoke crt_printf, addr str0
	add esp, 4
	mov eax, dword ptr [var_a + 0]
	cmp eax, dword ptr [var_b + 0]
	mov eax, 0
	setg al
	push eax
	invoke crt_printf, addr str1
	add esp, 4
	fld var_b
	push float2
	fld float2
	fmulp
	fstp tmp4
	mov eax, dword ptr [var_a + 0]
	cmp eax, tmp4
	mov eax, 0
	sete al
	push eax
	invoke crt_printf, addr str2
	add esp, 4
	mov eax, dword ptr [var_a + 0]
	cmp eax, dword ptr [var_b + 0]
	mov eax, 0
	setne al
	push eax
	invoke crt_printf, addr str3
	add esp, 4
	fld var_a
	fld var_a
	fmulp
	fstp tmp4
	mov eax, float3
	cmp eax, tmp4
	mov eax, 0
	setle al
	push dword ptr [var_a + 0]
	push eax
	invoke crt_printf, addr str4
	add esp, 4
	mov tmp4, 1
	fild tmp4
	fstp tmp4
	mov eax, tmp4
	cmp eax, dword ptr [var_b + 0]
	mov eax, 0
	setg al
	push eax
	invoke crt_printf, addr str5
	add esp, 4
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
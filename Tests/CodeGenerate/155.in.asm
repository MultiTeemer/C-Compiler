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
	float0 dd 0.100000
	float1 dd 10.000000
	float2 dd 100.000000
	float3 dd 0.000000
	float4 dd 1.100000
	float5 dd 1.100000
	float6 dd 1.100000
	float7 dd 0.200000
	float8 dd 10.000000
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
	f_main:
	push ebp
	mov ebp, esp
	mov tmp4, 1
	fild tmp4
	fstp tmp4
	mov eax, float0
	mov ebx, tmp4
	cmp eax, ebx
	mov eax, 0
	setl al
	push eax
	invoke crt_printf, addr str0
	add esp, 4
	mov eax, float1
	mov ebx, float2
	cmp eax, ebx
	mov eax, 0
	setg al
	push eax
	invoke crt_printf, addr str1
	add esp, 4
	mov tmp4, 0
	fild tmp4
	fstp tmp4
	push tmp4
	mov tmp4, 1
	fild tmp4
	fstp tmp4
	fld float3
	mov ebx, tmp4
	mov tmp4, 1
	fild tmp4
	fstp tmp4
	mov eax, tmp4
	mov tmp4, eax
	fld tmp4
	fsubp
	fstp tmp4
	mov eax, tmp4
	cmp eax, ebx
	mov eax, 0
	setle al
	push eax
	invoke crt_printf, addr str2
	add esp, 4
	mov tmp4, 0
	fild tmp4
	fstp tmp4
	mov eax, tmp4
	mov ebx, float4
	cmp eax, ebx
	mov eax, 0
	setge al
	push eax
	invoke crt_printf, addr str3
	add esp, 4
	mov eax, float5
	mov ebx, float6
	cmp eax, ebx
	mov eax, 0
	sete al
	push eax
	invoke crt_printf, addr str4
	add esp, 4
	mov tmp4, 2
	fild tmp4
	fstp tmp4
	mov eax, tmp4
	mov tmp4, eax
	fld tmp4
	push float8
	fld float8
	fdivp
	fstp tmp4
	mov eax, float7
	mov ebx, tmp4
	cmp eax, ebx
	mov eax, 0
	sete al
	push eax
	invoke crt_printf, addr str5
	add esp, 4
	f_main_end:
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d", 0dh, 0ah, 0
	str1 db "%d", 0dh, 0ah, 0
	str2 db "%d", 0dh, 0ah, 0
	str3 db "%d", 0dh, 0ah, 0
	str4 db "%d", 0dh, 0ah, 0
	var_a dd 1 dup(0)
	var_b dd 1 dup(0)
	var_c dd 1 dup(0)
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
	f_main:
	push ebp
	mov ebp, esp
	mov var_a, 1
	mov var_b, 2
	mov ebx, -3
	mov eax, offset var_c
	mov dword ptr [eax + 0], ebx
	mov eax, dword ptr [var_a + 0]
	mov ebx, dword ptr [var_b + 0]
	add eax, ebx
	mov ebx, dword ptr [var_c + 0]
	add eax, ebx
	push eax
	invoke crt_printf, addr str0
	add esp, 4
	mov eax, dword ptr [var_a + 0]
	mov ebx, dword ptr [var_b + 0]
	add eax, ebx
	mov ebx, dword ptr [var_c + 0]
	sub eax, ebx
	push eax
	invoke crt_printf, addr str1
	add esp, 4
	mov eax, dword ptr [var_b + 0]
	mov ebx, dword ptr [var_c + 0]
	cdq
	idiv ebx
	mov ebx, eax
	mov eax, dword ptr [var_a + 0]
	imul eax, ebx
	push eax
	invoke crt_printf, addr str2
	add esp, 4
	mov eax, dword ptr [var_a + 0]
	mov ebx, dword ptr [var_b + 0]
	add eax, ebx
	mov ebx, dword ptr [var_c + 0]
	imul eax, ebx
	push eax
	invoke crt_printf, addr str3
	add esp, 4
	mov ebx, dword ptr [var_b + 0]
	mov eax, dword ptr [var_b + 0]
	imul eax, ebx
	push eax
	push dword ptr [var_a + 0]
	mov ebx, dword ptr [var_a + 0]
	mov eax, dword ptr [var_a + 0]
	imul eax, ebx
	mov ebx, eax
	mov eax, dword ptr [var_c + 0]
	imul eax, ebx
	push eax
	mov eax, dword ptr [var_b + 0]
	mov ebx, dword ptr [var_a + 0]
	cdq
	idiv ebx
	mov ebx, dword ptr [var_c + 0]
	imul eax, ebx
	pop ebx
	sub eax, ebx
	pop ebx
	cdq
	idiv ebx
	pop ebx
	add eax, ebx
	push eax
	invoke crt_printf, addr str4
	add esp, 4
	f_main_end:
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d", 0dh, 0ah, 0
	str1 db "%d", 0dh, 0ah, 0
	str2 db "%d", 0dh, 0ah, 0
	str3 db "%d", 0dh, 0ah, 0
	var_a dd 1 dup(0)
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
	f_main:
	push ebp
	mov ebp, esp
	mov var_a, 1
	mov ebx, dword ptr [var_a + 0]
	mov eax, dword ptr [var_a + 0]
	and eax, ebx
	push eax
	invoke crt_printf, addr str0
	add esp, 4
	mov eax, 1
	mov ebx, 3
	mov ecx, ebx
	shl eax, cl
	mov ebx, eax
	mov eax, dword ptr [var_a + 0]
	or eax, ebx
	push eax
	invoke crt_printf, addr str1
	add esp, 4
	mov ebx, dword ptr [var_a + 0]
	mov eax, dword ptr [var_a + 0]
	xor eax, ebx
	push eax
	invoke crt_printf, addr str2
	add esp, 4
	mov eax, dword ptr [var_a + 0]
	not eax
	push eax
	invoke crt_printf, addr str3
	add esp, 4
	f_main_end:
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
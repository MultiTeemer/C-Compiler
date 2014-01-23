.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d %d %d", 0
	var_a dd 1 dup(0)
	var_b dd 10 dup(0)
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
	f_main:
	push ebp
	mov ebp, esp
	mov var_b, 1
	mov eax, 1
	mov ebx, 4
	imul eax, ebx
	mov ebx, offset var_b
	add eax, ebx
	mov ebx, 2
	mov dword ptr [eax + 0], ebx
	mov eax, 2
	mov ebx, 4
	imul eax, ebx
	mov ebx, offset var_b
	add eax, ebx
	mov ebx, 3
	mov dword ptr [eax + 0], ebx
	mov eax, 1
	mov ebx, 4
	imul eax, ebx
	mov ebx, offset var_b
	add eax, ebx
	mov ebx, eax
	mov eax, offset var_a
	mov dword ptr [eax + 0], ebx
	mov ebx, dword ptr [var_a + 0]
	mov eax, dword ptr [ebx + 0]
	mov ebx, dword ptr [var_a + 0]
	push eax
	mov eax, dword ptr [ebx + 0]
	mov ebx, dword ptr [var_a + 0]
	push eax
	mov eax, dword ptr [ebx + 0]
	push eax
	invoke crt_printf, addr str0
	add esp, 12
	f_main_end:
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
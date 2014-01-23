.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d %d", 0
	var_a dd 100 dup(0)
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
	f_main:
	push ebp
	mov ebp, esp
	sub esp, 4
	push dword ptr 5
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, -15
	push eax
	mov eax, dword ptr [ebp + -4]
	mov ebx, 4
	imul eax, ebx
	mov ebx, offset var_a
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, dword ptr [ebp + -4]
	mov ebx, 5
	cdq
	idiv ebx
	mov ebx, 3
	cdq
	idiv ebx
	mov ebx, edx
	mov eax, dword ptr [ebp + -4]
	imul eax, ebx
	mov ebx, eax
	mov eax, dword ptr [ebp + -4]
	imul eax, ebx
	mov ebx, 1
	cdq
	idiv ebx
	mov ebx, 1
	sub eax, ebx
	push eax
	mov eax, dword ptr [ebp + -4]
	mov ebx, 4
	imul eax, ebx
	mov ebx, offset var_a
	add eax, ebx
	pop ebx
	imul ebx, dword ptr [eax + 0]
	mov dword ptr [eax + 0], ebx
	mov eax, 99
	mov ebx, 4
	imul eax, ebx
	mov ebx, offset var_a
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, dword ptr [ebp + -4]
	mov ebx, 4
	imul eax, ebx
	mov ebx, offset var_a
	add eax, ebx
	push dword ptr [eax + 0]
	invoke crt_printf, addr str0
	add esp, 8
	f_main_end:
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
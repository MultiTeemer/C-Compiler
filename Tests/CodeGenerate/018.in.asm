.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d", 0
	var_a dd 50 dup(0)
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
	f_main:
	push ebp
	mov ebp, esp
	mov eax, 1
	mov ebx, 20
	imul eax, ebx
	mov ebx, offset var_a
	add eax, ebx
	push dword ptr 3
	push eax
	mov eax, 2
	mov ebx, 4
	imul eax, ebx
	pop ebx
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, 1
	mov ebx, 20
	imul eax, ebx
	mov ebx, offset var_a
	add eax, ebx
	push eax
	mov eax, 2
	mov ebx, 4
	imul eax, ebx
	pop ebx
	add eax, ebx
	push dword ptr [eax + 0]
	invoke crt_printf, addr str0
	add esp, 4
	f_main_end:
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d %d %d", 0
	var_a dd 10 dup(0)
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
	f_main:
	push ebp
	mov ebp, esp
	mov eax, 8
	mov ebx, offset var_a
	add eax, ebx
	mov ebx, 1
	mov dword ptr [eax + 0], ebx
	mov eax, 1
	push ebx
	mov ebx, 4
	imul eax, ebx
	mov ebx, offset var_a
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, 0
	push ebx
	mov ebx, 4
	imul eax, ebx
	mov ebx, offset var_a
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, 8
	mov ebx, offset var_a
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, 4
	mov ebx, offset var_a
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, offset var_a
	push dword ptr [eax + 0]
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
.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d", 0
	var_a dd 1 dup(0)
	var_b dd 1 dup(0)
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
	f_main:
	push ebp
	mov ebp, esp
	mov ebx, -1
	mov eax, offset var_b
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
	mov var_a, 1
	mov ebx, 1000
	mov eax, ebx
	push eax
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
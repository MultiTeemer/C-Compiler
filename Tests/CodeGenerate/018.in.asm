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
	mov eax, 20
	mov ebx, offset var_a
	add eax, ebx
	mov ebx, eax
	mov eax, 8
	add eax, ebx
	mov ebx, 3
	mov dword ptr [eax + 0], ebx
	mov eax, 20
	mov ebx, offset var_a
	add eax, ebx
	mov ebx, eax
	mov eax, 8
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
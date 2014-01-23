.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d", 0dh, 0ah, 0
	var_a dd 720600 dup(0)
	var_$b dd 7206 dup(0)
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_print:
	push ebp
	mov ebp, esp
	mov ebx, dword ptr [ebp + 8]
	push dword ptr [ebx + 0]
	invoke crt_printf, addr str0
	add esp, 4
	mov esp, ebp
	pop ebp
	ret 0
f_main:
	push ebp
	mov ebp, esp
	sub esp, 4
	mov eax, 1441200
	mov ebx, offset var_a
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	mov ebx, 14352
	add eax, ebx
	mov eax, eax
	mov ebx, 15
	mov dword ptr [eax + 0], ebx
	mov eax, 1441200
	mov ebx, offset var_a
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	mov ebx, 14352
	add eax, ebx
	push eax
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push dword ptr [ebp + -4]
	call f_print
	add esp, 4
	mov eax, dword ptr [ebp + -4]
	mov ebx, 4
	sub eax, ebx
	push eax
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	mov ebx, eax
	pop eax
	mov dword ptr [ebx + 0], eax
	push eax
	call f_print
	add esp, 4
	mov eax, dword ptr [ebp + -4]
	mov ebx, 4
	sub eax, ebx
	push eax
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	mov ebx, eax
	pop eax
	mov dword ptr [ebx + 0], eax
	push eax
	call f_print
	add esp, 4
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
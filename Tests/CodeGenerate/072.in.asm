.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d %d %d", 0
	var_$a dd 3 dup(0)
	var_$b dd 3 dup(0)
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_main:
	push ebp
	mov ebp, esp
	mov eax, offset var_$a
	mov ebx, 8
	add eax, ebx
	mov ebx, 0
	mov dword ptr [eax + 0], ebx
	mov eax, offset var_$a
	push ebx
	mov ebx, 4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, offset var_$a
	mov dword ptr [eax + 0], ebx
	mov eax, offset var_$b
	mov ebx, 8
	add eax, ebx
	mov ebx, -30
	mov dword ptr [eax + 0], ebx
	mov eax, offset var_$b
	push ebx
	mov ebx, 4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, offset var_$b
	mov dword ptr [eax + 0], ebx
	mov eax, offset var_$a
	mov ebx, dword ptr [var_$b + 0]
	mov dword ptr [eax + 0], ebx
	mov ebx, dword ptr [var_$b + 4]
	mov dword ptr [eax + 4], ebx
	mov ebx, dword ptr [var_$b + 8]
	mov dword ptr [eax + 8], ebx
	mov eax, offset var_$a
	mov ebx, 8
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, offset var_$a
	mov ebx, 4
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, offset var_$a
	push dword ptr [eax + 0]
	invoke crt_printf, addr str0
	add esp, 12
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
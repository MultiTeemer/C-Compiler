.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d %d %d %d %d", 0
	var_$a dd 13 dup(0)
	var_$b dd 13 dup(0)
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
	f_main:
	push ebp
	mov ebp, esp
	mov eax, -30
	push eax
	mov eax, offset var_$b
	mov ebx, 8
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
	push eax
	mov eax, offset var_$b
	mov ebx, 4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov ebx, ebx
	mov eax, offset var_$b
	mov dword ptr [eax + 0], ebx
	mov eax, -15
	push eax
	mov eax, offset var_$b
	mov ebx, 12
	add eax, ebx
	pop ebx
	mov eax, eax
	mov dword ptr [eax + 0], ebx
	mov eax, offset var_$b
	mov ebx, 12
	add eax, ebx
	push dword ptr 15
	push eax
	mov eax, 9
	mov ebx, 4
	imul eax, ebx
	pop ebx
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, offset var_$a
	mov ebx, dword ptr [var_$b + 0]
	mov dword ptr [eax + 0], ebx
	mov ebx, dword ptr [var_$b + 4]
	mov dword ptr [eax + 4], ebx
	mov ebx, dword ptr [var_$b + 8]
	mov dword ptr [eax + 8], ebx
	mov ebx, dword ptr [var_$b + 12]
	mov dword ptr [eax + 12], ebx
	mov ebx, dword ptr [var_$b + 16]
	mov dword ptr [eax + 16], ebx
	mov ebx, dword ptr [var_$b + 20]
	mov dword ptr [eax + 20], ebx
	mov ebx, dword ptr [var_$b + 24]
	mov dword ptr [eax + 24], ebx
	mov ebx, dword ptr [var_$b + 28]
	mov dword ptr [eax + 28], ebx
	mov ebx, dword ptr [var_$b + 32]
	mov dword ptr [eax + 32], ebx
	mov ebx, dword ptr [var_$b + 36]
	mov dword ptr [eax + 36], ebx
	mov ebx, dword ptr [var_$b + 40]
	mov dword ptr [eax + 40], ebx
	mov ebx, dword ptr [var_$b + 44]
	mov dword ptr [eax + 44], ebx
	mov ebx, dword ptr [var_$b + 48]
	mov dword ptr [eax + 48], ebx
	mov eax, offset var_$a
	mov ebx, 12
	add eax, ebx
	push eax
	mov eax, 9
	mov ebx, 4
	imul eax, ebx
	pop ebx
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, offset var_$a
	mov ebx, 12
	add eax, ebx
	mov eax, eax
	push dword ptr [eax + 0]
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
	add esp, 20
	f_main_end:
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
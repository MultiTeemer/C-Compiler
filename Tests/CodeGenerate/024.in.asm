.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d %d %d %d", 0
	var_$a dd 6 dup(0)
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
	f_main:
	push ebp
	mov ebp, esp
	mov var_$a, 3
	mov eax, offset var_$a
	mov ebx, 4
	add eax, ebx
	mov ebx, 15
	sub dword ptr [eax + 0], ebx
	mov ecx, eax
	mov eax, offset var_$a
	mov ebx, 4
	add eax, ebx
	mov ebx, dword ptr [eax + 0]
	mov eax, offset var_$a
	mov eax, dword ptr [eax + 0]
	sub eax, ebx
	push eax
	mov eax, offset var_$a
	mov ebx, 8
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, offset var_$a
	mov ebx, 4
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, offset var_$a
	mov ebx, 8
	add eax, ebx
	pop ebx
	mov eax, dword ptr [eax + 0]
	sub eax, ebx
	push eax
	mov eax, offset var_$a
	mov ebx, 8
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	pop ebx
	add dword ptr [eax + 0], ebx
	mov ecx, eax
	mov eax, offset var_$a
	mov ebx, 8
	add eax, ebx
	mov ebx, 4
	add eax, ebx
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
	add esp, 16
	f_main_end:
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
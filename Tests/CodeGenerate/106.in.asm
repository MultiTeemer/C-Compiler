.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "(%d, %d) -> (%d, %d)", 0dh, 0ah, 0
	str1 db "(%d, %d) -> (%d, %d)", 0dh, 0ah, 0
	var_$global dd 4 dup(0)
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
	f_main:
	push ebp
	mov ebp, esp
	sub esp, 24
	push offset var_$global
	mov eax, ebp
	mov ebx, -20
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebp
	mov ebx, -16
	add eax, ebx
	push eax
	mov eax, ebp
	mov ebx, -24
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov var_$global, -1
	mov eax, offset var_$global
	mov ebx, 4
	add eax, ebx
	mov ebx, 1
	mov dword ptr [eax + 0], ebx
	mov eax, offset var_$global
	mov ebx, 8
	add eax, ebx
	mov ebx, 1
	mov dword ptr [eax + 0], ebx
	mov eax, offset var_$global
	mov ebx, 8
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	mov ebx, -1
	mov dword ptr [eax + 0], ebx
	push dword ptr [var_$global + 12]
	push dword ptr [var_$global + 8]
	push dword ptr [var_$global + 4]
	push dword ptr [var_$global + 0]
	mov eax, ebp
	mov ebx, -16
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	pop ebx
	mov dword ptr [eax + 4], ebx
	pop ebx
	mov dword ptr [eax + 8], ebx
	pop ebx
	mov dword ptr [eax + 12], ebx
	mov eax, dword ptr [ebp + -20]
	mov ebx, 8
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, dword ptr [ebp + -20]
	mov ebx, 8
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, dword ptr [ebp + -20]
	mov ebx, 4
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, dword ptr [ebp + -20]
	push dword ptr [eax + 0]
	invoke crt_printf, addr str0
	add esp, 16
	mov eax, dword ptr [ebp + -24]
	mov ebx, 8
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, dword ptr [ebp + -24]
	mov ebx, 8
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, dword ptr [ebp + -24]
	mov ebx, 4
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, dword ptr [ebp + -24]
	push dword ptr [eax + 0]
	invoke crt_printf, addr str1
	add esp, 16
	f_main_end:
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d", 0
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
	f_main:
	push ebp
	mov ebp, esp
	sub esp, 8
	push dword ptr 0
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push dword ptr 0
	mov eax, ebp
	mov ebx, -8
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, dword ptr [ebp + -4]
	cmp eax, 0
	jne if41_true
	je if41_false
	if41_true:
	push dword ptr 3
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	pop ebx
	sub dword ptr [eax + 0], ebx
	mov ecx, eax
	jmp if41_end
	if41_false:
	mov eax, dword ptr [ebp + -8]
	cmp eax, 0
	jne if18467_true
	je if18467_false
	if18467_true:
	push dword ptr 7
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	pop ebx
	add dword ptr [eax + 0], ebx
	mov ecx, eax
	jmp if18467_end
	if18467_false:
	push dword ptr 6
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	pop ebx
	sub dword ptr [eax + 0], ebx
	mov ecx, eax
	if18467_end:
	if41_end:
	push dword ptr [ebp + -4]
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
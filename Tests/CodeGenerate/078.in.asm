.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d %d %d %d %d %d %d %d", 0dh, 0ah, 0
	var_a dd 1700 dup(0)
	var_$b dd 17 dup(0)
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
	f_main:
	push ebp
	mov ebp, esp
	sub esp, 4
	mov eax, offset var_$b
	mov ebx, 52
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	mov ebx, -1000
	mov dword ptr [eax + 0], ebx
	mov eax, offset var_$b
	push ebx
	mov ebx, 12
	add eax, ebx
	pop ebx
	mov eax, eax
	mov dword ptr [eax + 0], ebx
	mov eax, offset var_$b
	push ebx
	mov ebx, 8
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, offset var_$b
	push ebx
	mov ebx, 4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, offset var_$b
	mov dword ptr [eax + 0], ebx
	push dword ptr 0
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
	for41_start:
	for41_cond:
	push eax
	mov eax, dword ptr [ebp + -4]
	mov ebx, 100
	cmp eax, ebx
	mov eax, 0
	setl al
	cmp eax, 0
	je for41_end
	mov eax, dword ptr [ebp + -4]
	mov ebx, 68
	imul eax, ebx
	mov ebx, offset var_a
	add eax, ebx
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
	mov ebx, dword ptr [var_$b + 52]
	mov dword ptr [eax + 52], ebx
	mov ebx, dword ptr [var_$b + 56]
	mov dword ptr [eax + 56], ebx
	mov ebx, dword ptr [var_$b + 60]
	mov dword ptr [eax + 60], ebx
	mov ebx, dword ptr [var_$b + 64]
	mov dword ptr [eax + 64], ebx
	for41_inc:
	mov eax, dword ptr [ebp + -4]
	inc eax
	push eax
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	mov ebx, eax
	pop eax
	mov dword ptr [ebx + 0], eax
	jmp for41_cond
	for41_end:
	push eax
	push dword ptr 0
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
	for18467_start:
	for18467_cond:
	push eax
	mov eax, dword ptr [ebp + -4]
	mov ebx, 100
	cmp eax, ebx
	mov eax, 0
	setl al
	cmp eax, 0
	je for18467_end
	mov eax, dword ptr [ebp + -4]
	mov ebx, 68
	imul eax, ebx
	mov ebx, offset var_a
	add eax, ebx
	mov ebx, 52
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, dword ptr [ebp + -4]
	mov ebx, 68
	imul eax, ebx
	mov ebx, offset var_a
	add eax, ebx
	mov ebx, 52
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, dword ptr [ebp + -4]
	mov ebx, 68
	imul eax, ebx
	mov ebx, offset var_a
	add eax, ebx
	mov ebx, 12
	add eax, ebx
	mov ebx, 36
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, dword ptr [ebp + -4]
	mov ebx, 68
	imul eax, ebx
	mov ebx, offset var_a
	add eax, ebx
	mov ebx, 12
	add eax, ebx
	mov ebx, 20
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, dword ptr [ebp + -4]
	mov ebx, 68
	imul eax, ebx
	mov ebx, offset var_a
	add eax, ebx
	mov ebx, 12
	add eax, ebx
	mov eax, eax
	push dword ptr [eax + 0]
	mov eax, dword ptr [ebp + -4]
	mov ebx, 68
	imul eax, ebx
	mov ebx, offset var_a
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, dword ptr [ebp + -4]
	mov ebx, 68
	imul eax, ebx
	mov ebx, offset var_a
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, dword ptr [ebp + -4]
	mov ebx, 68
	imul eax, ebx
	mov ebx, offset var_a
	add eax, ebx
	push dword ptr [eax + 0]
	invoke crt_printf, addr str0
	add esp, 32
	for18467_inc:
	mov eax, dword ptr [ebp + -4]
	inc eax
	push eax
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	mov ebx, eax
	pop eax
	mov dword ptr [ebx + 0], eax
	jmp for18467_cond
	for18467_end:
	f_main_end:
	push eax
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
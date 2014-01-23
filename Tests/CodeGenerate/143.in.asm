.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%f", 0dh, 0ah, 0
	float0 dd 0.100000
	var_a dd 10 dup(0)
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_main:
	push ebp
	mov ebp, esp
	sub esp, 4
	push dword ptr 0
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
for41_cond:
	push eax
	mov eax, dword ptr [ebp + -4]
	mov ebx, 10
	cmp eax, ebx
	mov eax, 0
	setl al
	cmp eax, 0
	je for41_end
	mov eax, dword ptr [ebp + -4]
	mov tmp4, eax
	fild tmp4
	fstp tmp4
	mov eax, tmp4
	mov tmp4, eax
	fld tmp4
	push float0
	fld float0
	fmulp
	fstp tmp4
	mov eax, dword ptr [ebp + -4]
	mov ebx, 4
	imul eax, ebx
	mov ebx, offset var_a
	add eax, ebx
	mov ebx, tmp4
	mov dword ptr [eax + 0], ebx
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
for18467_cond:
	push eax
	mov eax, dword ptr [ebp + -4]
	mov ebx, 10
	cmp eax, ebx
	mov eax, 0
	setl al
	cmp eax, 0
	je for18467_end
	mov eax, dword ptr [ebp + -4]
	mov ebx, 4
	imul eax, ebx
	mov ebx, offset var_a
	add eax, ebx
	push dword ptr [eax + 0]
	pop tmp4
	fld tmp4
	fstp tmp8
	mov eax, offset tmp8
	push dword ptr [eax + 4]
	push dword ptr [eax + 0]
	invoke crt_printf, addr str0
	add esp, 8
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
	push eax
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
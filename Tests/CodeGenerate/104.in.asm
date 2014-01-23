.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d", 0
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_five:
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
	cmp eax, 10
	mov eax, 0
	setl al
	cmp eax, 0
	je for41_end
	sub esp, 4
	push dword ptr [ebp + -4]
	mov eax, ebp
	mov ebx, -8
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, dword ptr [ebp + -4]
	cmp eax, 5
	mov eax, 0
	setl al
	cmp eax, 0
	jne if18467_true
	je if18467_false
if18467_true:
	jmp for41_inc
	jmp if18467_end
if18467_false:
if18467_end:
	mov eax, dword ptr [ebp + -8]
	mov dword ptr [ebp + 8], eax
	jmp f_five_end
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
f_five_end:
	push eax
	mov esp, ebp
	pop ebp
	ret 0
f_main:
	push ebp
	mov ebp, esp
	sub esp, 4
	call f_five
	invoke crt_printf, addr str0
	add esp, 4
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
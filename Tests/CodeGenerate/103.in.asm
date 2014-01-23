.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d %d", 0dh, 0ah, 0
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
	mov ebx, 5
	cmp eax, ebx
	mov eax, 0
	setl al
	cmp eax, 0
	je for41_end
	sub esp, 4
	push dword ptr -1
	mov eax, ebp
	mov ebx, -8
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
prewhile18467_start:
prewhile18467_cond:
	mov ebx, dword ptr [ebp + -4]
	mov eax, dword ptr [ebp + -4]
	imul eax, ebx
	mov ebx, eax
	mov eax, dword ptr [ebp + -8]
	cmp eax, ebx
	mov eax, 0
	setl al
	cmp eax, 0
	je prewhile18467_end
	mov eax, dword ptr [ebp + -8]
	inc eax
	push eax
	mov eax, ebp
	mov ebx, -8
	add eax, ebx
	mov ebx, eax
	pop eax
	mov dword ptr [ebx + 0], eax
	mov ebx, dword ptr [ebp + -4]
	mov eax, dword ptr [ebp + -8]
	cmp eax, ebx
	mov eax, 0
	setl al
	cmp eax, 0
	jne if6334_true
	je if6334_false
if6334_true:
	jmp prewhile18467_start
	jmp if6334_end
if6334_false:
if6334_end:
	mov eax, dword ptr [ebp + -4]
	mov ebx, 1
	add eax, ebx
	mov ebx, eax
	mov eax, dword ptr [ebp + -8]
	cmp eax, ebx
	mov eax, 0
	setge al
	cmp eax, 0
	jne if26500_true
	je if26500_false
if26500_true:
	jmp prewhile18467_end
	jmp if26500_end
if26500_false:
if26500_end:
	push dword ptr [ebp + -4]
	push dword ptr [ebp + -8]
	invoke crt_printf, addr str0
	add esp, 8
	jmp prewhile18467_cond
prewhile18467_end:
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
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d", 0dh, 0ah, 0
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
postwhile41_start:
	push dword ptr [ebp + -4]
	invoke crt_printf, addr str0
	add esp, 4
	mov eax, dword ptr [ebp + -4]
	inc eax
	push dword ptr 10
	push eax
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	mov ebx, eax
	pop eax
	pop ebx
	mov dword ptr [ebx + 0], eax
	cmp eax, ebx
	mov eax, 0
	setl al
	cmp eax, 0
	jne if18467_true
	je if18467_false
if18467_true:
	jmp postwhile41_start
	jmp if18467_end
if18467_false:
if18467_end:
	mov eax, 0
	cmp eax, 0
	jne postwhile41_start
f_main_end:
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d is odd", 0dh, 0ah, 0
	str1 db "%d is even", 0dh, 0ah, 0
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
	push dword ptr 10
	mov eax, ebp
	mov ebx, -8
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
prewhile41_cond:
	mov ebx, dword ptr [ebp + -8]
	mov eax, dword ptr [ebp + -4]
	sub eax, ebx
	cmp eax, 0
	je prewhile41_end
	mov eax, dword ptr [ebp + -4]
	inc eax
	push eax
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	mov ebx, eax
	pop eax
	mov dword ptr [ebx + 0], eax
	mov eax, dword ptr [ebp + -4]
	mov ebx, 1
	and eax, ebx
	cmp eax, 0
	jne if18467_true
	je if18467_false
if18467_true:
	push dword ptr [ebp + -4]
	invoke crt_printf, addr str0
	add esp, 4
	jmp if18467_end
if18467_false:
	push dword ptr [ebp + -4]
	invoke crt_printf, addr str1
	add esp, 4
if18467_end:
	jmp prewhile41_cond
prewhile41_end:
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
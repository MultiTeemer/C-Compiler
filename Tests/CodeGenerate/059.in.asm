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
	postwhile41_start:
	mov eax, dword ptr [ebp + -4]
	inc eax
	push eax
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	mov ebx, eax
	pop eax
	mov dword ptr [ebx + 0], eax
	push eax
	invoke crt_printf, addr str0
	add esp, 4
	mov eax, dword ptr [ebp + -4]
	mov ebx, dword ptr [ebp + -8]
	sub eax, ebx
	cmp eax, 0
	jne postwhile41_start
	postwhile41_end:
	f_main_end:
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d %d", 0
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
	f_main:
	push ebp
	mov ebp, esp
	sub esp, 40
	push dword ptr 15
	mov eax, ebp
	mov ebx, -40
	add eax, ebx
	mov ebx, 12
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push dword ptr 17
	mov eax, ebp
	mov ebx, -40
	add eax, ebx
	mov ebx, 20
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebp
	mov ebx, -40
	add eax, ebx
	mov ebx, 20
	add eax, ebx
	mov ebx, eax
	push dword ptr [ebx + 0]
	mov eax, ebp
	mov ebx, -40
	add eax, ebx
	mov ebx, 12
	add eax, ebx
	mov ebx, eax
	push dword ptr [ebx + 0]
	invoke crt_printf, addr str0
	add esp, 8
	f_main_end:
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
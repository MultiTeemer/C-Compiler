.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d %d %d", 0
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
	f_main:
	push ebp
	mov ebp, esp
	sub esp, 44
	mov eax, ebp
	mov ebx, -40
	add eax, ebx
	push eax
	mov eax, ebp
	mov ebx, -44
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, 12
	mov ebx, dword ptr [ebp + -44]
	add eax, ebx
	mov ebx, 3
	mov dword ptr [eax + 0], ebx
	mov eax, 8
	mov ebx, dword ptr [ebp + -44]
	add eax, ebx
	mov ebx, -3
	mov dword ptr [eax + 0], ebx
	mov eax, 16
	mov ebx, dword ptr [ebp + -44]
	add eax, ebx
	mov ebx, 15
	mov dword ptr [eax + 0], ebx
	mov eax, 16
	mov ebx, dword ptr [ebp + -44]
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, 8
	mov ebx, dword ptr [ebp + -44]
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, 12
	mov ebx, dword ptr [ebp + -44]
	add eax, ebx
	push dword ptr [eax + 0]
	invoke crt_printf, addr str0
	add esp, 12
	f_main_end:
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
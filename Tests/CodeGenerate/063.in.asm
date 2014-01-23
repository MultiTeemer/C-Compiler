.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d", 0
	str1 db "%d", 0
	str2 db "%d", 0
	str3 db "%d", 0
	str4 db "%d", 0
	str5 db "%d", 0
	str6 db "%d", 0
	str7 db "%d", 0
	str8 db "%d", 0
	str9 db "%d", 0
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_main:
	push ebp
	mov ebp, esp
	sub esp, 4
	push dword ptr 1
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, dword ptr [ebp + -4]
	mov ebx, 1
	cmp eax, ebx
	mov eax, 0
	sete al
	push eax
	invoke crt_printf, addr str0
	add esp, 4
	mov eax, dword ptr [ebp + -4]
	mov ebx, 1
	cmp eax, ebx
	mov eax, 0
	setne al
	push eax
	invoke crt_printf, addr str1
	add esp, 4
	mov eax, dword ptr [ebp + -4]
	mov ebx, 2
	cmp eax, ebx
	mov eax, 0
	setl al
	push eax
	invoke crt_printf, addr str2
	add esp, 4
	mov eax, dword ptr [ebp + -4]
	mov ebx, 2
	cmp eax, ebx
	mov eax, 0
	setg al
	push eax
	invoke crt_printf, addr str3
	add esp, 4
	mov eax, dword ptr [ebp + -4]
	mov ebx, 2
	cmp eax, ebx
	mov eax, 0
	setle al
	push eax
	invoke crt_printf, addr str4
	add esp, 4
	mov eax, dword ptr [ebp + -4]
	mov ebx, 1
	cmp eax, ebx
	mov eax, 0
	setle al
	push eax
	invoke crt_printf, addr str5
	add esp, 4
	mov eax, dword ptr [ebp + -4]
	mov ebx, 1
	cmp eax, ebx
	mov eax, 0
	setge al
	push eax
	invoke crt_printf, addr str6
	add esp, 4
	mov eax, dword ptr [ebp + -4]
	mov ebx, 2
	cmp eax, ebx
	mov eax, 0
	setge al
	push eax
	invoke crt_printf, addr str7
	add esp, 4
	mov eax, dword ptr [ebp + -4]
	mov ebx, 3
	cmp eax, ebx
	mov eax, 0
	sete al
	push eax
	invoke crt_printf, addr str8
	add esp, 4
	mov eax, dword ptr [ebp + -4]
	mov ebx, 3
	cmp eax, ebx
	mov eax, 0
	setne al
	push eax
	invoke crt_printf, addr str9
	add esp, 4
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
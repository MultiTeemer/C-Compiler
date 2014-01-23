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
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
	f_main:
	push ebp
	mov ebp, esp
	sub esp, 12
	push dword ptr 1
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push dword ptr 1
	mov eax, ebp
	mov ebx, -8
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push dword ptr 0
	mov eax, ebp
	mov ebx, -8
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
	push eax
	push dword ptr 0
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
	pop ebx
	cmp eax, 0
	mov eax, 0
	setne al
	cmp ebx, 0
	mov ebx, 0
	setne bl
	imul eax, ebx
	cmp eax, 0
	mov eax, 0
	setne al
	push eax
	invoke crt_printf, addr str0
	add esp, 4
	mov eax, dword ptr [ebp + -8]
	mov ebx, 0
	cmp eax, ebx
	mov eax, 0
	sete al
	push eax
	mov eax, dword ptr [ebp + -4]
	mov ebx, 0
	cmp eax, ebx
	pop ebx
	mov eax, 0
	sete al
	cmp eax, 0
	mov eax, 0
	setne al
	cmp ebx, 0
	mov ebx, 0
	setne bl
	imul eax, ebx
	cmp eax, 0
	mov eax, 0
	setne al
	push eax
	invoke crt_printf, addr str1
	add esp, 4
	mov eax, dword ptr [ebp + -8]
	mov ebx, 5
	cmp eax, ebx
	mov eax, 0
	sete al
	push eax
	push dword ptr 1
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
	pop ebx
	cmp eax, 0
	mov eax, 0
	setne al
	cmp ebx, 0
	mov ebx, 0
	setne bl
	add eax, ebx
	cmp eax, 0
	mov eax, 0
	setne al
	push eax
	invoke crt_printf, addr str2
	add esp, 4
	push dword ptr 3
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
	push eax
	push dword ptr 1
	mov eax, ebp
	mov ebx, -8
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
	push eax
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
	pop ebx
	cmp eax, 0
	mov eax, 0
	setne al
	cmp ebx, 0
	mov ebx, 0
	setne bl
	imul eax, ebx
	cmp eax, 0
	mov eax, 0
	setne al
	push eax
	invoke crt_printf, addr str3
	add esp, 4
	mov eax, dword ptr [ebp + -8]
	mov ebx, dword ptr [ebp + -12]
	cmp eax, ebx
	mov eax, 0
	setne al
	push eax
	mov eax, dword ptr [ebp + -12]
	mov ebx, dword ptr [ebp + -4]
	cmp eax, ebx
	mov eax, 0
	setne al
	push eax
	mov eax, dword ptr [ebp + -4]
	mov ebx, dword ptr [ebp + -8]
	cmp eax, ebx
	pop ebx
	mov eax, 0
	setne al
	cmp eax, 0
	mov eax, 0
	setne al
	cmp ebx, 0
	mov ebx, 0
	setne bl
	imul eax, ebx
	pop ebx
	cmp eax, 0
	mov eax, 0
	setne al
	cmp eax, 0
	mov eax, 0
	setne al
	cmp ebx, 0
	mov ebx, 0
	setne bl
	imul eax, ebx
	cmp eax, 0
	mov eax, 0
	setne al
	push eax
	invoke crt_printf, addr str4
	add esp, 4
	mov eax, dword ptr [ebp + -4]
	mov ebx, 1
	cmp eax, ebx
	mov eax, 0
	sete al
	push eax
	mov eax, dword ptr [ebp + -4]
	mov ebx, 1
	cmp eax, ebx
	pop ebx
	mov eax, 0
	setne al
	cmp eax, 0
	mov eax, 0
	setne al
	cmp ebx, 0
	mov ebx, 0
	setne bl
	add eax, ebx
	cmp eax, 0
	mov eax, 0
	setne al
	push eax
	invoke crt_printf, addr str5
	add esp, 4
	mov eax, dword ptr [ebp + -8]
	mov ebx, 10
	cmp eax, ebx
	mov eax, 0
	setg al
	push eax
	mov eax, dword ptr [ebp + -4]
	mov ebx, 10
	cmp eax, ebx
	pop ebx
	mov eax, 0
	setl al
	cmp eax, 0
	mov eax, 0
	setne al
	cmp ebx, 0
	mov ebx, 0
	setne bl
	imul eax, ebx
	cmp eax, 0
	mov eax, 0
	setne al
	push eax
	invoke crt_printf, addr str6
	add esp, 4
	mov eax, dword ptr [ebp + -8]
	mov ebx, dword ptr [ebp + -12]
	imul eax, ebx
	mov ebx, 15
	sub eax, ebx
	mov ebx, dword ptr [ebp + -4]
	push eax
	mov eax, dword ptr [ebp + -4]
	imul eax, ebx
	mov ebx, eax
	mov eax, dword ptr [ebp + -4]
	imul eax, ebx
	pop ebx
	cmp eax, ebx
	mov eax, 0
	setne al
	push eax
	mov eax, dword ptr [ebp + -4]
	mov ebx, 15
	imul eax, ebx
	mov ebx, 15000
	cmp eax, ebx
	pop ebx
	mov eax, 0
	setl al
	cmp eax, 0
	mov eax, 0
	setne al
	cmp ebx, 0
	mov ebx, 0
	setne bl
	add eax, ebx
	cmp eax, 0
	mov eax, 0
	setne al
	push eax
	invoke crt_printf, addr str7
	add esp, 4
	mov eax, 1
	mov ebx, 2
	cmp eax, ebx
	mov ebx, 3
	mov eax, 0
	setl al
	cmp eax, ebx
	mov ebx, 10
	mov eax, 0
	setl al
	cmp eax, ebx
	mov ebx, 0
	mov eax, 0
	setl al
	cmp eax, ebx
	mov eax, 0
	setg al
	push eax
	invoke crt_printf, addr str8
	add esp, 4
	f_main_end:
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
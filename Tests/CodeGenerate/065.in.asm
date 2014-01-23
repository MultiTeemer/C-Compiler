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
	str10 db "%d", 0
	str11 db "%d", 0
	str12 db "%d", 0
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
	f_main:
	push ebp
	mov ebp, esp
	mov eax, 1
	mov ebx, 2
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
	mov eax, 1
	mov ebx, 0
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
	mov eax, 0
	mov ebx, 0
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
	invoke crt_printf, addr str2
	add esp, 4
	mov eax, 1
	mov ebx, 2
	cmp eax, 0
	mov eax, 0
	setne al
	cmp ebx, 0
	mov ebx, 0
	setne bl
	imul eax, ebx
	mov ebx, 3
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
	invoke crt_printf, addr str3
	add esp, 4
	mov eax, 1
	mov ebx, 0
	cmp eax, 0
	mov eax, 0
	setne al
	cmp ebx, 0
	mov ebx, 0
	setne bl
	imul eax, ebx
	mov ebx, 3
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
	mov eax, 1
	mov ebx, 0
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
	mov eax, 0
	mov ebx, 0
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
	invoke crt_printf, addr str6
	add esp, 4
	mov eax, 1
	mov ebx, 1
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
	cmp eax, 0
	mov eax, 0
	setne al
	cmp ebx, 0
	mov ebx, 0
	setne bl
	add eax, ebx
	mov ebx, 3
	cmp eax, 0
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
	invoke crt_printf, addr str8
	add esp, 4
	mov eax, 1
	mov ebx, 2
	cmp eax, 0
	mov eax, 0
	setne al
	cmp ebx, 0
	mov ebx, 0
	setne bl
	add eax, ebx
	mov ebx, 0
	cmp eax, 0
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
	invoke crt_printf, addr str9
	add esp, 4
	mov eax, 0
	mov ebx, 1
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
	mov eax, 1
	mov ebx, 0
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
	add eax, ebx
	cmp eax, 0
	mov eax, 0
	setne al
	push eax
	invoke crt_printf, addr str10
	add esp, 4
	mov eax, 0
	mov ebx, 1
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
	mov eax, 1
	mov ebx, 1
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
	add eax, ebx
	cmp eax, 0
	mov eax, 0
	setne al
	push eax
	invoke crt_printf, addr str11
	add esp, 4
	mov eax, 0
	mov ebx, 1
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
	mov eax, 1
	mov ebx, 0
	cmp eax, 0
	mov eax, 0
	setne al
	cmp ebx, 0
	mov ebx, 0
	setne bl
	add eax, ebx
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
	invoke crt_printf, addr str12
	add esp, 4
	f_main_end:
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
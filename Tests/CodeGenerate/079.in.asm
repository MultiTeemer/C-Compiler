.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d", 0dh, 0ah, 0
	str1 db "%d", 0dh, 0ah, 0
	str2 db "%d", 0dh, 0ah, 0
	str3 db "%d", 0dh, 0ah, 0
	str4 db "%d", 0dh, 0ah, 0
	str5 db "%d", 0dh, 0ah, 0
	str6 db "%d", 0dh, 0ah, 0
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
	f_loop:
	push ebp
	mov ebp, esp
	mov eax, dword ptr [ebp + 8]
	mov dword ptr [ebp + 12], eax
	f_loop_end:
	mov esp, ebp
	pop ebp
	ret 0
	f_equal:
	push ebp
	mov ebp, esp
	mov ebx, dword ptr [ebp + 12]
	mov eax, dword ptr [ebp + 8]
	cmp eax, ebx
	mov eax, 0
	sete al
	mov dword ptr [ebp + 16], eax
	f_equal_end:
	mov esp, ebp
	pop ebp
	ret 0
	f_inc:
	push ebp
	mov ebp, esp
	mov eax, dword ptr [ebp + 8]
	inc eax
	push eax
	mov eax, ebp
	mov ebx, 8
	add eax, ebx
	mov ebx, eax
	pop eax
	mov dword ptr [ebx + 0], eax
	mov dword ptr [ebp + 12], eax
	f_inc_end:
	mov esp, ebp
	pop ebp
	ret 0
	f_main:
	push ebp
	mov ebp, esp
	sub esp, 4
	push dword ptr 1
	call f_loop
	add esp, 4
	pop ebx
	mov eax, 1
	cmp eax, ebx
	mov eax, 0
	sete al
	push eax
	invoke crt_printf, addr str0
	add esp, 4
	sub esp, 4
	push dword ptr 10
	call f_loop
	add esp, 4
	pop ebx
	mov eax, -1
	cmp eax, ebx
	mov eax, 0
	sete al
	push eax
	invoke crt_printf, addr str1
	add esp, 4
	sub esp, 4
	push dword ptr 1
	push dword ptr 1
	call f_equal
	add esp, 8
	invoke crt_printf, addr str2
	add esp, 4
	sub esp, 4
	mov eax, -1
	push eax
	push dword ptr 1
	call f_equal
	add esp, 8
	invoke crt_printf, addr str3
	add esp, 4
	sub esp, 4
	sub esp, 4
	mov eax, -1
	push eax
	call f_inc
	add esp, 4
	push dword ptr 0
	call f_equal
	add esp, 8
	invoke crt_printf, addr str4
	add esp, 4
	sub esp, 4
	sub esp, 4
	push dword ptr 1
	call f_loop
	add esp, 4
	sub esp, 4
	push dword ptr 1
	call f_loop
	add esp, 4
	call f_equal
	add esp, 8
	invoke crt_printf, addr str5
	add esp, 4
	sub esp, 4
	push dword ptr 1
	sub esp, 4
	sub esp, 4
	push dword ptr 0
	call f_loop
	add esp, 4
	call f_inc
	add esp, 4
	call f_equal
	add esp, 8
	invoke crt_printf, addr str6
	add esp, 4
	f_main_end:
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
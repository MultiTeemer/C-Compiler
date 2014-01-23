.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d", 0dh, 0ah, 0
	str1 db "%d", 0dh, 0ah, 0
	str2 db "%d", 0dh, 0ah, 0
	str3 db "%d", 0dh, 0ah, 0
	float0 dd 0.003000
	float1 dd 1000.000000
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
	f_main:
	push ebp
	mov ebp, esp
	sub esp, 8
	push float0
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push float1
	mov eax, ebp
	mov ebx, -8
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, dword ptr [ebp + -4]
	mov tmp4, eax
	fld tmp4
	push dword ptr [ebp + -8]
	mov eax, dword ptr [ebp + -8]
	mov tmp4, eax
	fld tmp4
	fmulp
	fstp tmp4
	mov eax, tmp4
	mov tmp4, eax
	fld tmp4
	fistp tmp4
	push tmp4
	invoke crt_printf, addr str0
	add esp, 4
	mov eax, dword ptr [ebp + -8]
	mov tmp4, eax
	fld tmp4
	push dword ptr [ebp + -4]
	mov eax, dword ptr [ebp + -4]
	mov tmp4, eax
	fld tmp4
	fmulp
	fstp tmp4
	mov eax, tmp4
	mov tmp4, eax
	fld tmp4
	fistp tmp4
	push tmp4
	invoke crt_printf, addr str1
	add esp, 4
	mov eax, dword ptr [ebp + -4]
	mov tmp4, eax
	fld tmp4
	push dword ptr [ebp + -8]
	mov eax, dword ptr [ebp + -8]
	mov tmp4, eax
	fld tmp4
	fdivp
	fstp tmp4
	mov eax, tmp4
	mov tmp4, eax
	fld tmp4
	fistp tmp4
	push tmp4
	invoke crt_printf, addr str2
	add esp, 4
	mov eax, dword ptr [ebp + -8]
	mov tmp4, eax
	fld tmp4
	push dword ptr [ebp + -4]
	mov eax, dword ptr [ebp + -4]
	mov tmp4, eax
	fld tmp4
	fdivp
	fstp tmp4
	mov eax, tmp4
	mov tmp4, eax
	fld tmp4
	fistp tmp4
	push tmp4
	invoke crt_printf, addr str3
	add esp, 4
	f_main_end:
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
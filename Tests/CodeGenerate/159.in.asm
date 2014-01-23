.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%f %f", 0dh, 0ah, 0
	str1 db "%f %f", 0dh, 0ah, 0
	float0 dd 3.000000
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
	f_main:
	push ebp
	mov ebp, esp
	sub esp, 8
	mov eax, dword ptr [ebp + -4]
	mov tmp4, eax
	fld tmp4
	push float0
	fld float0
	faddp
	fstp tmp4
	push tmp4
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov tmp4, 1
	fild tmp4
	fstp tmp4
	mov eax, dword ptr [ebp + -8]
	push tmp4
	mov tmp4, eax
	fld tmp4
	mov tmp4, 1
	fild tmp4
	fstp tmp4
	mov eax, tmp4
	mov tmp4, eax
	fld tmp4
	fsubp
	fstp tmp4
	push tmp4
	mov eax, ebp
	mov ebx, -8
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push dword ptr [ebp + -8]
	pop tmp4
	fld tmp4
	fstp tmp8
	mov eax, offset tmp8
	push dword ptr [eax + 4]
	push dword ptr [eax + 0]
	push dword ptr [ebp + -4]
	pop tmp4
	fld tmp4
	fstp tmp8
	mov eax, offset tmp8
	push dword ptr [eax + 4]
	push dword ptr [eax + 0]
	invoke crt_printf, addr str0
	add esp, 16
	push dword ptr [ebp + -8]
	mov eax, dword ptr [ebp + -4]
	mov tmp4, eax
	fld tmp4
	mov eax, dword ptr [ebp + -8]
	mov tmp4, eax
	fld tmp4
	fdivp
	fstp tmp4
	push tmp4
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push dword ptr [ebp + -4]
	mov eax, dword ptr [ebp + -4]
	mov tmp4, eax
	fld tmp4
	mov eax, dword ptr [ebp + -4]
	mov tmp4, eax
	fld tmp4
	fmulp
	fstp tmp4
	mov eax, dword ptr [ebp + -8]
	push tmp4
	mov tmp4, eax
	fld tmp4
	push dword ptr [ebp + -4]
	mov eax, dword ptr [ebp + -4]
	mov tmp4, eax
	fld tmp4
	mov eax, dword ptr [ebp + -4]
	mov tmp4, eax
	fld tmp4
	fmulp
	fstp tmp4
	mov eax, tmp4
	mov tmp4, eax
	fld tmp4
	fmulp
	fstp tmp4
	push tmp4
	mov eax, ebp
	mov ebx, -8
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push dword ptr [ebp + -8]
	pop tmp4
	fld tmp4
	fstp tmp8
	mov eax, offset tmp8
	push dword ptr [eax + 4]
	push dword ptr [eax + 0]
	push dword ptr [ebp + -4]
	pop tmp4
	fld tmp4
	fstp tmp8
	mov eax, offset tmp8
	push dword ptr [eax + 4]
	push dword ptr [eax + 0]
	invoke crt_printf, addr str1
	add esp, 16
	f_main_end:
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
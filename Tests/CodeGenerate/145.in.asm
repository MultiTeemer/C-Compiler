.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%f %f", 0
	float0 dd 3.000000
	float1 dd 5.000000
	var_a dd 500 dup(0)
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_main:
	push ebp
	mov ebp, esp
	sub esp, 8
	push offset var_a
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov tmp4, 1
	fild tmp4
	fstp tmp4
	mov eax, dword ptr [ebp + -4]
	mov ebx, tmp4
	mov dword ptr [eax + 0], ebx
	push dword ptr [ebp + -4]
	mov eax, ebp
	mov ebx, -8
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, dword ptr [ebp + -4]
	mov ebx, 4
	add eax, ebx
	push eax
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	mov ebx, eax
	pop eax
	mov dword ptr [ebx + 0], eax
	mov eax, float1
	mov tmp4, eax
	fld tmp4
	fistp tmp4
	mov eax, tmp4
	mov tmp4, eax
	fild tmp4
	fstp tmp4
	fld float0
	mov eax, float1
	push tmp4
	mov tmp4, eax
	fld tmp4
	fistp tmp4
	mov eax, tmp4
	mov tmp4, eax
	fild tmp4
	fstp tmp4
	mov eax, tmp4
	mov tmp4, eax
	fld tmp4
	fmulp
	fstp tmp4
	mov eax, dword ptr [ebp + -4]
	mov ebx, tmp4
	mov dword ptr [eax + 0], ebx
	mov ebx, dword ptr [ebp + -8]
	push dword ptr [ebx + 0]
	pop tmp4
	fld tmp4
	fstp tmp8
	mov eax, offset tmp8
	mov ebx, dword ptr [ebp + -4]
	push dword ptr [eax + 4]
	push dword ptr [eax + 0]
	push dword ptr [ebx + 0]
	pop tmp4
	fld tmp4
	fstp tmp8
	mov eax, offset tmp8
	push dword ptr [eax + 4]
	push dword ptr [eax + 0]
	invoke crt_printf, addr str0
	add esp, 16
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%f %f %f %d %d %d", 0dh, 0ah, 0
	str1 db "%f", 0
	float0 dd 3.000000
	var_$b dd 12006 dup(0)
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
	f_main:
	push ebp
	mov ebp, esp
	mov eax, -1
	push float0
	push eax
	mov eax, offset var_$b
	mov ebx, 24020
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
	push eax
	mov eax, offset var_$b
	mov ebx, 24016
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
	push eax
	mov eax, offset var_$b
	mov ebx, 4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov tmp4, ebx
	fild tmp4
	fstp tmp4
	mov eax, tmp4
	mov tmp4, eax
	fld tmp4
	fld float0
	fmulp
	fstp tmp4
	mov eax, offset var_$b
	mov ebx, 24012
	add eax, ebx
	mov ebx, tmp4
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
	push eax
	mov eax, offset var_$b
	mov ebx, 8
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov ebx, ebx
	mov eax, offset var_$b
	mov dword ptr [eax + 0], ebx
	mov eax, offset var_$b
	mov ebx, 24020
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, offset var_$b
	mov ebx, 24016
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, offset var_$b
	mov ebx, 4
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, offset var_$b
	mov ebx, 24012
	add eax, ebx
	push dword ptr [eax + 0]
	pop tmp4
	fld tmp4
	fstp tmp8
	mov eax, offset tmp8
	push dword ptr [eax + 4]
	push dword ptr [eax + 0]
	mov eax, offset var_$b
	mov ebx, 8
	add eax, ebx
	push dword ptr [eax + 0]
	pop tmp4
	fld tmp4
	fstp tmp8
	mov eax, offset tmp8
	push dword ptr [eax + 4]
	push dword ptr [eax + 0]
	mov eax, offset var_$b
	push dword ptr [eax + 0]
	pop tmp4
	fld tmp4
	fstp tmp8
	mov eax, offset tmp8
	push dword ptr [eax + 4]
	push dword ptr [eax + 0]
	invoke crt_printf, addr str0
	add esp, 36
	mov tmp4, 5
	fild tmp4
	fstp tmp4
	mov eax, offset var_$b
	mov ebx, 12
	add eax, ebx
	push tmp4
	push eax
	mov eax, 30
	mov ebx, 800
	imul eax, ebx
	pop ebx
	add eax, ebx
	push eax
	mov eax, 40
	mov ebx, 20
	imul eax, ebx
	pop ebx
	add eax, ebx
	push eax
	mov eax, 3
	mov ebx, 4
	imul eax, ebx
	pop ebx
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, offset var_$b
	mov ebx, 12
	add eax, ebx
	push eax
	mov eax, 30
	mov ebx, 800
	imul eax, ebx
	pop ebx
	add eax, ebx
	push eax
	mov eax, 40
	mov ebx, 20
	imul eax, ebx
	pop ebx
	add eax, ebx
	push eax
	mov eax, 3
	mov ebx, 4
	imul eax, ebx
	pop ebx
	add eax, ebx
	mov ebx, eax
	mov eax, dword ptr [ebx + 0]
	mov tmp4, eax
	fld tmp4
	fstp tmp8
	mov eax, offset tmp8
	push dword ptr [eax + 4]
	push dword ptr [eax + 0]
	invoke crt_printf, addr str1
	add esp, 8
	f_main_end:
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
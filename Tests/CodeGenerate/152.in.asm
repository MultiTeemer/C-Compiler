.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%f", 0dh, 0ah, 0
	str1 db "%f %f %f", 0dh, 0ah, 0
	str2 db "%f", 0
	float0 dd 3.400000
	float1 dd 2.100000
	float2 dd 1.000000
	float3 dd 0.340000
	float4 dd 3.000000
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_toInt:
	push ebp
	mov ebp, esp
	mov eax, dword ptr [ebp + 8]
	mov tmp4, eax
	fld tmp4
	fistp tmp4
	mov eax, tmp4
	mov dword ptr [ebp + 12], eax
	mov esp, ebp
	pop ebp
	ret 0
f_inc:
	push ebp
	mov ebp, esp
	mov tmp4, 1
	fild tmp4
	fstp tmp4
	mov eax, dword ptr [ebp + 8]
	push tmp4
	mov tmp4, eax
	fld tmp4
	mov tmp4, 1
	fild tmp4
	fstp tmp4
	mov eax, tmp4
	mov tmp4, eax
	fld tmp4
	faddp
	fstp tmp4
	mov eax, tmp4
	mov dword ptr [ebp + 12], eax
	mov esp, ebp
	pop ebp
	ret 0
f_sum:
	push ebp
	mov ebp, esp
	mov tmp4, 3
	fild tmp4
	fstp tmp4
	push tmp4
	mov tmp4, 1
	fild tmp4
	fstp tmp4
	mov eax, dword ptr [ebp + 12]
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
	mov eax, dword ptr [ebp + 8]
	push tmp4
	mov tmp4, eax
	fld tmp4
	mov tmp4, 1
	fild tmp4
	fstp tmp4
	mov eax, dword ptr [ebp + 12]
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
	mov eax, tmp4
	mov tmp4, eax
	fld tmp4
	fmulp
	fstp tmp4
	mov eax, tmp4
	mov tmp4, eax
	fld tmp4
	mov tmp4, 3
	fild tmp4
	fstp tmp4
	mov eax, tmp4
	mov tmp4, eax
	fld tmp4
	faddp
	fstp tmp4
	mov eax, tmp4
	mov dword ptr [ebp + 16], eax
	mov esp, ebp
	pop ebp
	ret 0
f_main:
	push ebp
	mov ebp, esp
	sub esp, 4
	fld float0
	push float1
	fld float1
	fsubp
	fstp tmp4
	push tmp4
	call f_toInt
	add esp, 4
	pop eax
	mov tmp4, eax
	fild tmp4
	fstp tmp4
	push tmp4
	pop tmp4
	fld tmp4
	fstp tmp8
	mov eax, offset tmp8
	push dword ptr [eax + 4]
	push dword ptr [eax + 0]
	invoke crt_printf, addr str0
	add esp, 8
	sub esp, 4
	sub esp, 4
	sub esp, 4
	mov eax, float3
	mov tmp4, eax
	fld tmp4
	fchs
	fstp tmp4
	push tmp4
	call f_inc
	add esp, 4
	call f_toInt
	add esp, 4
	pop eax
	mov tmp4, eax
	fild tmp4
	fstp tmp4
	push tmp4
	call f_inc
	add esp, 4
	pop tmp4
	fld tmp4
	fstp tmp8
	mov eax, offset tmp8
	push dword ptr [eax + 4]
	push dword ptr [eax + 0]
	sub esp, 4
	mov tmp4, -1
	fild tmp4
	fstp tmp4
	push tmp4
	call f_inc
	add esp, 4
	pop tmp4
	fld tmp4
	fstp tmp8
	mov eax, offset tmp8
	push dword ptr [eax + 4]
	push dword ptr [eax + 0]
	sub esp, 4
	push float2
	call f_inc
	add esp, 4
	pop tmp4
	fld tmp4
	fstp tmp8
	mov eax, offset tmp8
	push dword ptr [eax + 4]
	push dword ptr [eax + 0]
	invoke crt_printf, addr str1
	add esp, 24
	sub esp, 4
	mov tmp4, 10
	fild tmp4
	fstp tmp4
	push tmp4
	push float4
	call f_sum
	add esp, 8
	pop tmp4
	fld tmp4
	fstp tmp8
	mov eax, offset tmp8
	push dword ptr [eax + 4]
	push dword ptr [eax + 0]
	invoke crt_printf, addr str2
	add esp, 8
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
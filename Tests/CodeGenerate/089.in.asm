.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "(%d, %d) -> (%d, %d)", 0dh, 0ah, 0
	str1 db "%d %d", 0dh, 0ah, 0
	str2 db "%d %d", 0dh, 0ah, 0
	str3 db "%d %d", 0dh, 0ah, 0
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
	f_print:
	push ebp
	mov ebp, esp
	mov eax, ebp
	mov ebx, 8
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, ebp
	mov ebx, 8
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, ebp
	mov ebx, 8
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, ebp
	mov ebx, 8
	add eax, ebx
	mov eax, eax
	push dword ptr [eax + 0]
	invoke crt_printf, addr str0
	add esp, 16
	f_print_end:
	mov esp, ebp
	pop ebp
	ret 0
	f_scale:
	push ebp
	mov ebp, esp
	push dword ptr [ebp + 24]
	mov eax, ebp
	mov ebx, 8
	add eax, ebx
	pop ebx
	mov eax, eax
	imul ebx, dword ptr [eax + 0]
	mov dword ptr [eax + 0], ebx
	push dword ptr [ebp + 24]
	mov eax, ebp
	mov ebx, 8
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	pop ebx
	imul ebx, dword ptr [eax + 0]
	mov dword ptr [eax + 0], ebx
	push dword ptr [ebp + 24]
	mov eax, ebp
	mov ebx, 8
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	pop ebx
	imul ebx, dword ptr [eax + 0]
	mov dword ptr [eax + 0], ebx
	push dword ptr [ebp + 24]
	mov eax, ebp
	mov ebx, 8
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	pop ebx
	imul ebx, dword ptr [eax + 0]
	mov dword ptr [eax + 0], ebx
	push dword ptr [ebp + 20]
	push dword ptr [ebp + 16]
	push dword ptr [ebp + 12]
	mov eax, dword ptr [ebp + 8]
	mov dword ptr [ebp + 28], eax
	pop eax
	mov dword ptr [ebp + 32], eax
	pop eax
	mov dword ptr [ebp + 36], eax
	pop eax
	mov dword ptr [ebp + 40], eax
	f_scale_end:
	mov esp, ebp
	pop ebp
	ret 0
	f_cross_product:
	push ebp
	mov ebp, esp
	sub esp, 16
	mov eax, ebp
	mov ebx, 8
	add eax, ebx
	mov eax, eax
	push dword ptr [eax + 0]
	mov eax, ebp
	mov ebx, 8
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	pop ebx
	mov eax, dword ptr [eax + 0]
	sub eax, ebx
	push eax
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebp
	mov ebx, 8
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, ebp
	mov ebx, 8
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	pop ebx
	mov eax, dword ptr [eax + 0]
	sub eax, ebx
	push eax
	mov eax, ebp
	mov ebx, -8
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebp
	mov ebx, 24
	add eax, ebx
	mov eax, eax
	push dword ptr [eax + 0]
	mov eax, ebp
	mov ebx, 24
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	pop ebx
	mov eax, dword ptr [eax + 0]
	sub eax, ebx
	push eax
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebp
	mov ebx, 24
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, ebp
	mov ebx, 24
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	pop ebx
	mov eax, dword ptr [eax + 0]
	sub eax, ebx
	push eax
	mov eax, ebp
	mov ebx, -16
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov ebx, dword ptr [ebp + -8]
	mov eax, dword ptr [ebp + -12]
	imul eax, ebx
	mov ebx, dword ptr [ebp + -16]
	push eax
	mov eax, dword ptr [ebp + -4]
	imul eax, ebx
	pop ebx
	sub eax, ebx
	mov dword ptr [ebp + 40], eax
	f_cross_product_end:
	mov esp, ebp
	pop ebp
	ret 0
	f_main:
	push ebp
	mov ebp, esp
	sub esp, 32
	push dword ptr 0
	mov eax, ebp
	mov ebx, -32
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push ebx
	mov eax, ebp
	mov ebx, -32
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push ebx
	mov eax, ebp
	mov ebx, -32
	add eax, ebx
	pop ebx
	mov eax, eax
	mov dword ptr [eax + 0], ebx
	push ebx
	mov eax, ebp
	mov ebx, -16
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push ebx
	mov eax, ebp
	mov ebx, -16
	add eax, ebx
	pop ebx
	mov eax, eax
	mov dword ptr [eax + 0], ebx
	push dword ptr 1
	mov eax, ebp
	mov ebx, -16
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push ebx
	mov eax, ebp
	mov ebx, -16
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push dword ptr 1
	mov eax, ebp
	mov ebx, -32
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push dword ptr [ebp + -4]
	push dword ptr [ebp + -8]
	push dword ptr [ebp + -12]
	push dword ptr [ebp + -16]
	call f_print
	add esp, 16
	push dword ptr [ebp + -20]
	push dword ptr [ebp + -24]
	push dword ptr [ebp + -28]
	push dword ptr [ebp + -32]
	call f_print
	add esp, 16
	sub esp, 16
	push dword ptr 5
	push dword ptr [ebp + -4]
	push dword ptr [ebp + -8]
	push dword ptr [ebp + -12]
	push dword ptr [ebp + -16]
	call f_scale
	add esp, 20
	call f_print
	add esp, 16
	sub esp, 16
	push dword ptr 3
	push dword ptr [ebp + -20]
	push dword ptr [ebp + -24]
	push dword ptr [ebp + -28]
	push dword ptr [ebp + -32]
	call f_scale
	add esp, 20
	call f_print
	add esp, 16
	sub esp, 16
	push dword ptr -1
	sub esp, 16
	push dword ptr 3
	push dword ptr [ebp + -4]
	push dword ptr [ebp + -8]
	push dword ptr [ebp + -12]
	push dword ptr [ebp + -16]
	call f_scale
	add esp, 20
	call f_scale
	add esp, 20
	call f_print
	add esp, 16
	sub esp, 4
	push dword ptr [ebp + -4]
	push dword ptr [ebp + -8]
	push dword ptr [ebp + -12]
	push dword ptr [ebp + -16]
	push dword ptr [ebp + -20]
	push dword ptr [ebp + -24]
	push dword ptr [ebp + -28]
	push dword ptr [ebp + -32]
	call f_cross_product
	add esp, 32
	sub esp, 4
	push dword ptr [ebp + -20]
	push dword ptr [ebp + -24]
	push dword ptr [ebp + -28]
	push dword ptr [ebp + -32]
	push dword ptr [ebp + -4]
	push dword ptr [ebp + -8]
	push dword ptr [ebp + -12]
	push dword ptr [ebp + -16]
	call f_cross_product
	add esp, 32
	invoke crt_printf, addr str1
	add esp, 8
	sub esp, 4
	push dword ptr [ebp + -4]
	push dword ptr [ebp + -8]
	push dword ptr [ebp + -12]
	push dword ptr [ebp + -16]
	sub esp, 16
	push dword ptr 3
	push dword ptr [ebp + -20]
	push dword ptr [ebp + -24]
	push dword ptr [ebp + -28]
	push dword ptr [ebp + -32]
	call f_scale
	add esp, 20
	call f_cross_product
	add esp, 32
	sub esp, 4
	push dword ptr [ebp + -20]
	push dword ptr [ebp + -24]
	push dword ptr [ebp + -28]
	push dword ptr [ebp + -32]
	sub esp, 16
	push dword ptr 5
	push dword ptr [ebp + -4]
	push dword ptr [ebp + -8]
	push dword ptr [ebp + -12]
	push dword ptr [ebp + -16]
	call f_scale
	add esp, 20
	call f_cross_product
	add esp, 32
	invoke crt_printf, addr str2
	add esp, 8
	sub esp, 4
	sub esp, 16
	push dword ptr 3
	push dword ptr [ebp + -4]
	push dword ptr [ebp + -8]
	push dword ptr [ebp + -12]
	push dword ptr [ebp + -16]
	call f_scale
	add esp, 20
	sub esp, 16
	push dword ptr 3
	push dword ptr [ebp + -20]
	push dword ptr [ebp + -24]
	push dword ptr [ebp + -28]
	push dword ptr [ebp + -32]
	call f_scale
	add esp, 20
	call f_cross_product
	add esp, 32
	sub esp, 4
	sub esp, 16
	push dword ptr -1
	push dword ptr [ebp + -20]
	push dword ptr [ebp + -24]
	push dword ptr [ebp + -28]
	push dword ptr [ebp + -32]
	call f_scale
	add esp, 20
	sub esp, 16
	push dword ptr -1
	push dword ptr [ebp + -4]
	push dword ptr [ebp + -8]
	push dword ptr [ebp + -12]
	push dword ptr [ebp + -16]
	call f_scale
	add esp, 20
	call f_cross_product
	add esp, 32
	invoke crt_printf, addr str3
	add esp, 8
	f_main_end:
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
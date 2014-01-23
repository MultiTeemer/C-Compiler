.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "do-while", 0dh, 0ah, 0
	str1 db "while-do", 0dh, 0ah, 0
	str2 db "for", 0
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
	f_main:
	push ebp
	mov ebp, esp
	pop eax
	for41_start:
	for41_cond:
	cmp eax, 0
	je for41_end
	prewhile18467_start:
	prewhile18467_cond:
	mov eax, 1
	cmp eax, 0
	je prewhile18467_end
	postwhile6334_start:
	invoke crt_printf, addr str0
	jmp postwhile6334_end
	mov eax, 1
	cmp eax, 0
	jne postwhile6334_start
	postwhile6334_end:
	invoke crt_printf, addr str1
	jmp prewhile18467_end
	jmp prewhile18467_cond
	prewhile18467_end:
	invoke crt_printf, addr str2
	jmp for41_end
	for41_inc:
	jmp for41_cond
	for41_end:
	f_main_end:
	mov esp, ebp
	pop ebp
	ret 0
start:
	call f_main
	ret 0
end start
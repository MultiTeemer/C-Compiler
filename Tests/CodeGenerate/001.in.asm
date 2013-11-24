.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc  
includelib c:\masm32\lib\msvcrt.lib
.data
	i dd 1
	f db "%d", 0
.code
main:
	invoke crt_printf, addr f, i
	ret 0
end main

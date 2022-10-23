.DATA
msg db 'filenam.bmp', 0FFH
.CODE

CpuIdAsm proc
	mov rax, 85
	mov rdi, offset msg
	mov rsi, 111111111b
	;syscall
	mov r8, rax ; save file descriptor
	
	mov rax, 5
	ret
CpuIdAsm endp

END
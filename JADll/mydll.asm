.DATA
;data: QWORD
.CODE

laplaceFilter proc
; Image width is stored in RCX
; Image height is stored in RDX
; Number of threads R8
; Position R9
	mov rax, 9		; rax contains 9
	mul rdx			; multiplication values stored in rax and rdx, result in rax
	mov rbx, rax	; moving result to rbx
	mov rax, 3		; rax contains 3
	mul rcx			; multiplication values stored in rax and rcx, result in rax
	add rax, rbx	; 
	;mov data, rax
	;mov rbx, QWORD PTR [rsp - data]
	ret
laplaceFilter endp
END
.DATA
data QWORD ?
modifiedData QWORD ?
maskB QWORD ?
five DD 5.0
.CODE

laplaceFilter proc
; Image width is stored in RCX
; Image height is stored in RDX
; Number of threads R8
; Position R9
	push    rbp
    mov     rbp, rsp
	mov rax, QWORD PTR [rbp]
	mov rax, QWORD PTR [rbp+8]
	mov rax, QWORD PTR [rbp+16]
	mov data, rax
	mov rax, QWORD PTR [rbp+24]
	mov rax, QWORD PTR [rbp+32]
	mov rax, QWORD PTR [rbp+40]
	mov rax, QWORD PTR [rbp+48]
	mov rax, QWORD PTR [rbp+56]
	mov rax, QWORD PTR [rbp+64]
	mov maskB, rax
	mov rax, QWORD PTR [rbp+72]
	mov rax, QWORD PTR [rbp+80]
	mov modifiedData, rax
	;movss xmm0, [five]
    ;movss [rbp+24], xmm0

	mov rax, qword ptr [rbp+24]
	movss xmm0, [five]
    movss dword ptr [rax], xmm0

	mov rax, 9		; rax contains 9
	mul rdx			; multiplication values stored in rax and rdx, product in rax
	mov rbx, rax	; moving result to rbx
	mov rax, 3		; rax contains 3
	mul rcx			; multiplication values stored in rax and rcx, product in rax
	add rax, rbx	; adds rax and rbx, 
	;mov rax, QWORD PTR [rbp+24]
	;mov data, rax
	;mov rbx, QWORD PTR [rsp - data]
	mov rsp, rbp
	pop rbp
	ret
laplaceFilter endp
END
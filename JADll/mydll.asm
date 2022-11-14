.DATA
imageHeight DD ?
imageWidth DD ?
noThreads DD ?
position DD ?
dataAddress DQ ?
modifiedDataAddress DQ ?
maskAddress DQ ?
const dd 0.6

.CODE

laplaceFilter proc
; Address of the data is stored in RCX
; Address of the modified data is stored in RDX
; Address of the mask is stored in R8
; Image width is stored in R9

	push    rbp
    mov     rbp, rsp
	mov dataAddress, rcx
	mov modifiedDataAddress, rdx
	mov maskAddress, r8
	mov imageWidth, r9d
	mov eax, DWORD PTR [rbp+48] ;	Image height is stored in eax
	mov imageHeight, eax
	mov eax, DWORD PTR [rbp+56] ;	Number of threads is stored in eax
	mov noThreads, eax
	mov eax, DWORD PTR [rbp+64] ;	Position is stored in eax
	mov position, eax

	mov rcx, modifiedDataAddress	; modifiedDataAddress is now stored in rcx
	movss xmm0, [const]
	movss DWORD PTR [rcx+64], xmm0

cpy:
	mov rdx, dataAddress
	mov rcx, modifiedDataAddress
	mov rbx, 0


	inc rbx
	;cmp rbx,
endA:
	mov ebx, eax
	xor rax, rax
	mov eax, ebx
	add rax, rcx
	movd xmm0, DWORD PTR[rsi+rax]		; save dword in xmm
	pmovzxbw xmm0, xmm0					; set bytes as words
	vpbroadcastq xmm0, xmm0				; copy first half into the second half
	
	;movss xmm0, 5
	;movss QWORD PTR [rdx], xmm0

	;mov modifiedData, rax
	;movss xmm0, [five]
    ;movss [rbp+24], xmm0

	;mov rax, qword ptr [rbp+24]
	;movss xmm0, [five]
    ;movss dword ptr [rax], xmm0

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
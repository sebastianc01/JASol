.DATA
imageHeight DD ?
imageWidth DD ?
noThreads DD ?
position DD ?
noRows DD ?
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

;Saving data
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
	mov eax, DWORD PTR [rbp+72] ; Number of rows is stored in eax
	mov noRows, eax
;Saving data

;Trying to copy an image
	;Setting registers
	mov rbx, 0						; counter of the data, it would be an address of the proper array element
	mov rcx, 0						; counter of the modified data, it would be an address of the proper array element
	mov rdx, dataAddress			; data address is stored in rdx anyway, just to emphasise it
	mov r8, modifiedDataAddress		; modified data address is stored in r8
	;Setting registers
	startL:


	incorrectData:
	inc rbx							; incrementing address of the data
	inc rcx							; incrementing address of the modified data
	;Checking values in rbx and rcx, when they are too big exiting loop
	mov rax, 0

	;Checking values in rbx and rcx, when they are too big exiting loop
	jmp startL
	endL:
;Trying to copy an image

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
.DATA
imageHeight DD ?
imageWidth DD ?
noThreads DD ?
position DD ?
noRows DD ?
dataAddress DQ ?
modifiedDataAddress DQ ?
paddingSize DD ?


.CODE

laplaceAsm proc
; Address of the data is stored in RCX
; Address of the modified data is stored in RDX
; Padding size is stored in R8
; Image width is stored in R9
	mov rax, 5
;Saving data
	push rbp
    mov     rbp, rsp
	mov dataAddress, rcx			;	Saving image data address
	mov modifiedDataAddress, rdx	;	Saving modified data address
	mov paddingSize, r8d			;   Saving padding size
	mov imageWidth, r9d				;	Saving image width
	mov eax, DWORD PTR [rbp+48]	
	mov imageHeight, eax			;	Saving image height
	mov eax, DWORD PTR [rbp+56]	
	mov noThreads, eax				;	Saving number of threads
	mov eax, DWORD PTR [rbp+64]
	mov position, eax				;	Saving position
	mov eax, DWORD PTR [rbp+72]
	mov noRows, eax					;	Saving number of rows to calculate in the thread
	push rax
	push rbx
	push r10
	push r11
	push r12						;
	push r13						;
	push r14						;
	push r15						;
;Saving data

;Set correct mask in xmm2
	mov r8, 0001000100010001h
	movq xmm2, r8						
	mov r8, 0001000100010001h
	pinsrq xmm2, r8, 1
;Set correct mask in xmm2



;Trying to copy an image
	;Setting registers
	mov rbx, 0						; counter of all rows which have to be modificated in this thread
	mov rcx, 0						; counter of all columns
	mov rdx, 0						; counter of colours of every pixel, so its value has to be always between 0 and 2
	mov r8, dataAddress				; data address is stored in r8
	mov r9, 0						; set r9 to 0
	mov r10d, position				; position is stored in r10
	;Setting registers
	mov r11, 0						; maximum row - 1
	mov r12d, imageWidth			; image width - 1 is stored in r12d
	dec r12d						; program will not calculate first and the last column
	mov r13, 0						; number of bytes per single row
	mov r14, 0
	mov r15, 0

	mov r11d, r10d
	add r11d, noRows				; maximum row
	dec r11d

	xor rax, rax					; set rax 0
	mov eax, imageWidth				; move image width to r13d
	add eax, paddingSize			; add padding size to image width
	mov r15, 3						
	mul r15							; multiply by 3, in result eax contains number of bytes per single row
	xor r15, r15					; set r15 to 0
	mov r13, rax					; r13 contains number of bytes per single row


	startL:
	jmp checkCurrentRow				; jump to checkCurrentRow

	sameRow:
	cmp edx, 3						; compare current colour with 3 (3 colours in every pixel)
	je nextColumn					; when all colours in the pixel are modified then modify next column
	; pixel is not ready yet
	mov rax, rbx					; move current number of row to rax
	imul rax, r13							; multiply current number of row by bytes per single row
	mov r15, rax					; move temporarily contents of the rax to r15
	mov rax, rcx					; move current column number to rax
	add rax, rcx
	add rax, rcx					; multiply current column by 3
	add rax, r15					; add calculated values and save the result in rax
	add rax, rdx					; add current colour to the result
	xor r15, r15					; set r15 to 0
	add rax, dataAddress			; add dataAddress to the result
	; 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
	; 161514131211109 8 7 6 5 4 3 2 1
	;   15  13  11  9   7   5   3   1
	mov r14, 0FFFFFFFFFFFFFFFFh		 ;set xmm0
	movq xmm0, r14						
	mov r14, 0FFFFFFFFFFFF0000h
	pinsrq xmm0, r14, 1				;set xmm0
	pinsrb xmm0, byte ptr [rax], 9	; load to xmm0 center of the square
	sub rax, 3						; substract 3 from rax, now it points element on the left
	pinsrb xmm0, byte ptr [rax], 7	; (1, 2)
	add rax, 6						; add 6 to rax, now it points element on the right
	pinsrb xmm0, byte ptr [rax], 11	; (3, 2)
	sub rax, r13					; substract number of bytes per row from rax
	pinsrb xmm0, byte ptr [rax], 5	; (3, 1)
	sub rax, 3						; substract 3 from rax
	pinsrb xmm0, byte ptr [rax], 3	; (2, 1)
	sub rax, 3						; substract 3 from rax
	pinsrb xmm0, byte ptr [rax], 1	; (1, 1)
	add rax, r13					; add number of byter per row to rax, now it points (1, 2)
	add rax, r13					; add number of byter per row to rax, now it points (1, 3)
	pinsrb xmm0, byte ptr [rax], 13	; (1, 3)
	add rax, 3						; add 3 to rax
	pinsrb xmm0, byte ptr [rax], 15	; (2, 3)
	add rax, 3						; add 3 to rax
	;pinsrb xmm0, byte ptr [rax], 9	; (3, 3)
	xor r14, r14					; set r14 to 0
	mov r14b, byte ptr [rax]		; save (3, 3) element in r14b
	imul r14, -1					; multiply last element by -1
	

	sub rax, 3						; (2, 3)
	sub rax, r13					; (2, 2)
	mov r15, rax					; currently calculated element

	pmaddubsw xmm0, xmm2			; multiply bytes in xmm0 by xmm2 and save the sum of the words in xmm0
	pextrd r14d, xmm0, 1
	
	sub r15, dataAddress
	add r15, modifiedDataAddress
	
	mov byte ptr [r15], cl ;r14b
	;test
	xor rax, rax					; set rax to 0
	xor r14, r14					; set r14 to 0
	;pextrb al, xmm0, 1
	;pmovmskb
	
	;test
	;pextrb byte ptr [r15], xmm0, 1
	xor r15, r15					; set r15 to 0
	inc edx							; modify next colour
	jmp sameRow						; jump to sameRow

	nextColumn:
	mov edx, 0						; calculating another pixel, reset colour counter
	cmp ecx, r12d					; compare current column with total column number - 1
	je nextRow						; when all columns are modified then modify next row
	inc ecx							; calculate next column
	jmp sameRow						; jump to sameRow

	nextRow:
	mov ecx, 1						; calculating another row, reset column counter
	inc ebx							; calculate next row
	jmp startL						; jump to startL

	checkCurrentRow:
	cmp ebx, 0						; compare current row with 0
	je nextRow						; current row is 0, jump to nextRow, program will not calculate first row
	cmp ebx, r11d					; compare current row with total rows - 1
	je endL							; current row is the last one, program will not calculate last row
	jmp sameRow						; jump to sameRow


	endL:
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop rbx
	pop rax
	mov rsp, rbp
	pop rbp
	ret
laplaceAsm endp

TestAsm proc

;Save mask values in xmm1
	mov r8, 01010101010101h
	movq xmm2, r8						
	;mov r8, 0101010101010101h
	pinsrq xmm2, r8, 1	
	;movdqu xmm2, 0101010101010101h
	;mov al, 'G'
	;mov byte ptr [dataAddress + 656], al
	;pinsrb xmm3, byte ptr [dataAddress + 656], 2
	;mov al, 'J'
	;mov byte ptr [dataAddress + 656], al
	pinsrb xmm3, byte ptr [dataAddress + 656], 11
	pinsrb xmm3, byte ptr [dataAddress + 656], 1
	pinsrb xmm3, byte ptr [dataAddress + 656], 3
	pinsrb xmm3, byte ptr [dataAddress + 656], 4
	pinsrb xmm3, byte ptr [dataAddress + 656], 5
	pinsrb xmm3, byte ptr [dataAddress + 656], 6
	pinsrb xmm3, byte ptr [dataAddress + 656], 7
	pinsrb xmm3, byte ptr [dataAddress + 656], 8
	pinsrb xmm3, byte ptr [dataAddress + 656], 10
	pinsrb xmm3, byte ptr [dataAddress + 656], 9
	pinsrb xmm3, byte ptr [dataAddress + 656], 12
	pinsrb xmm3, byte ptr [dataAddress + 656], 13
	pinsrb xmm3, byte ptr [dataAddress + 656], 14
	pinsrb xmm3, byte ptr [dataAddress + 656], 15
	;pinsrb xmm3, byte ptr [dataAddress + 656], 0
	;pinsrb xmm3, byte ptr [dataAddress + 656], 16
	;pmaddubsw xmm2, xmm3

	;pextrb byte ptr [dataAddress], xmm3, 2
;Save mask values in xmm1
;Saving data
	push    rbp
    mov     rbp, rsp
	mov dataAddress, rcx
	mov modifiedDataAddress, rdx
	mov paddingSize, r8d
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
	;mov eax, DWORD PTR [dataAddress]	; move first element of the array to the eax
	;cvtsi2ss xmm0, eax					; convert 32-bit integer to 32-bit float
	;vbroadcastss xmm1, DWORD PTR [dataAddress]
	pop rbp
	ret
TestAsm endp

END
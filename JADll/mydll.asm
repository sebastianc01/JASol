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
	mov rbx, 0						; counter of all rows which have to be modificated in this thread
	mov rcx, 0						; counter of all columns
	mov rdx, 0						; counter of colours of every pixel, so its value has to be always between 0 and 2
	mov r8, dataAddress				; data address is stored in r8
	mov r9, modifiedDataAddress		; modified data address is stored in r9
	;Setting registers
	;Calculating thr beginning position of the data ?
	mov rax, 0						; set eax to 0
	mov eax, position				; move number of rows to eax
	mul imageWidth					; multiply position by the image width
	mov r10, 3						; move 3 (number of the colours per pixel) to r10
	mul r10							; multiply the result by r10 (3 colours in all pixels)
	mov r10, rax					; move the result to r10, the beginning of the data will be stored there
	;Calculating thr beginning position of the data ?
	push r12						;r12 would contain height between -1 and 1, size of the mask
	push r13						;r13 would contain width between -1 and 1, size of the mask
	push r14						;
	push r15						;
	mov r12, 0
	mov r13, 0
	mov r14, 0
	mov r15, 0

	startL:
	; filter for one colour
	;Making sure that we are calculating data inside the array
	mov rax, r12					; move current height of the mask to rax
	add rax, rbx					; add current row number to the height of the mask
	cmp rax, 0						; compare rax with 0
	jl nextMaskRow					; when rax is lesser than 0 jump to nextMaskRow
	cmp eax, imageHeight			; compare eax with image height
	jge nextMaskRow					; when rax is greater or equal to image height jump to nextMaskRow

	mov rax, r13					; move current width of the mask to rax
	add rax, rcx					; add current column number to the width of the mask
	cmp rax, 0						; compare rax with 0
	jl nextMaskRow					; when rax is lesser than 0 jump to nextMaskColumn
	cmp eax, imageWidth				; compare eax with image width
	jge nextMaskColumn				; when rax is greater or equal to image width jump to nextMaskColumn
	;Making sure that we are calculating data inside the array
	;Calculating the correct address of elements
	mov rax, 3						; move 3 to rax
	mul imageWidth					; multiply rax by image width
	add rbx, r12					; temporarily add current height number of the mask
	mul rbx							; multiply rax by number of the current row increased by current height number of the mask, now rax contains 3*(H+h)*imageWidth
	sub rbx, r12					; substract rbx by current height of the mask, now in rbx is stored just current row
	mov r14, rax					; move rax to r14 to temporarily save it
	mov rax, 3						; move 3 to rax
	add rcx, r13					; temporarily add current width number of the mask
	mul rcx							; multiply rax by current number of the column increased by current width number of the mask, now rax contains 3*(W+w)
	sub rcx, r13					; substract rcx by the number of the current width of the mask, now in rcx is stored just current width
	add rax, rdx					; add colour number, now rax contains 3*(W+w) + 1
	add rax, r14					; now rax contains 3*(H+h)*imageWidth + 3*(W+w) + 1
	cmp r12, -1						; compare current width of the mask with -1
	je maskFirst					; jump to maskFirst, data correct
	cmp r12, 0						; compare current width of the mask with 0
	je maskSecond					; jump to maskSecond
	cmp r12, 1						; compare current width of the mask with 1
	je maskThird					; jump to maskThird
	jmp nextMaskColumn				; data incorrect, out of the array
	;Place data in the correct position in the XMM registers
	;mask's width equal to -1
	maskFirst:
	;											@todo: place data into xmm
	jmp nextMaskColumn
	;mask's width equal to -1
	;mask's width equal to 0
	maskSecond:
	;											@todo: place data into xmm
	jmp nextMaskColumn
	;mask's width equal to 0
	;mask's width equal to 1
	maskThird:
	;											@todo: place data into xmm
	jmp nextMaskRow
	;mask's width equal to 1
	;Place data in the correct position in the XMM registers

	;Calculating the correct address of elements

	;mask's width
	nextMaskColumn:
	inc r13							; increment current width of the mask
	cmp r13, 3						; mask's width has to be between 0 and 2
	je nextMaskRow					; the value is bigger than 2
	jmp startL						; jump to startL
	;mask's width

	;mask's height
	nextMaskRow:
	mov r13, 0						;reset mask's height
	inc r12							; increment current height of the mask
	cmp r12, 3						; mask's height has to be between 0 and 2
	je nextColour					; the value is bigger than 2
	jmp startL						; jump to startL
	;mask's height

	;Modifying all colours of the pixel
	nextColour:
	mov r12, 0						; reset mask's row
	mov r13, 0						; reset mask's height
	inc rdx							; modifying all colours of the pixel
	cmp rdx, 3						; its value has to be always between 0 and 2
	JE nextColumn					; all colours have been set, jump to nextColumn
	jmp startL						; some colours of the pixel have to be modified
	;Modifying all colours of the pixel

	;Checking values in rbx and rcx and when they are too big exiting the loop
	;Modifying all columns of the row
	nextColumn:
	mov rdx, 0						; modifying next pixel, so colours of the pixel have to be set to 0
	cmp imageWidth, ecx				; comparing total image width with a current width counter
	JE widthMaxValue				; jump to widthMaxValue when compared values are the same
	inc ecx							; values are not the same yet, increments the number of the column
	jmp startL						; jump to the start
	;Modifying all columns of the row

	;All columns have been already modified, incrementing the counter of the rows
	widthMaxValue:
	cmp noRows, ebx					; comparing number of Rows to modify in this thread with current row number
	JE endL							; jump to endL when compared values are the same (all data modified)
	inc ebx							; same rows are not modified yet, increment the value
	mov ecx, 0						; next row will be modified, counter of the columns is set to 0
	jmp startL						; jump to startL
	;All columns have been already modified, incrementing the counter of the rows

	;Checking values in rbx and rcx and when they are too big exiting the loop
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
;	movd xmm0, DWORD PTR[rsi+rax]		; save dword in xmm
;	pmovzxbw xmm0, xmm0					; set bytes as words
;	vpbroadcastq xmm0, xmm0				; copy first half into the second half
	
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

	pop r15
	pop r14
	pop r13
	pop r12
	mov rsp, rbp
	pop rbp
	ret
laplaceFilter endp
END
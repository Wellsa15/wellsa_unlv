;Andrew Wells CPE 301 DA1
; DA1T1.asm
;
; Created: 2/24/2016 9:26:24 PM
; Author : sirfe
;

;  Macro to create the stack
.macro SET_STACK	;Beginning of macro
	ldi		r16, LOW(RAMEND)	;Copy the lower 8 bits of the end of the RAM into R16
	out		spl, r16			;Copy the address in r16 to the lower portion of the stack pointer
	ldi		r16, HIGH(RAMEND)	;Copy the higher 8 bits of the end of hte RAM into R16
	out		sph, r16			;Copy the address in r16 to the higher portion of the stack pointer
.endmacro
	
.cseg
.org 0			
	rjmp start	;Skips over the interrupt portion of the memory

.org 0x20	; starting point for the program

start:
	SET_STACK	; runs macro to create the stack

	ldi		r19, 25	; Copy 25 into R19, keeps track of how many values have been obtained
	ldi		r20, 0	; make sure R20 is 0 to start
	ldi		r21, 0	; make sure R21 is 0 to start
	ldi		r23, 0	; make sure R23 is 0 to start
	ldi		r24, 0	; make sure R24 is 0 to start
    ldi		ZH, HIGH(RAMEND)	; Copy the High portion of the end of the RAM to ZH pointer/register
	ldi		ZL, LOW(RAMEND)		; Copy the Lower portion of the end of the RAM to the ZL pointer/register
	; Divide by 2, to get to the middle of the RAM
	lsr		ZH	; Shift the upper portion of Z to the right
	ror		ZL	; Rotate the lower portion of Z to the right, brings in the carry(if any) from the upper portion
	mov		XL, ZL ; Copy lower 8-bits of ram center address to the XL reg
	mov		XH, ZH ; Copy hupper 8-bits of ram center address to the XH reg
NEXT:
	mov		r17, ZL	;Copy the value of the lower portion of the address in Z to R17
	st		Z+, r17	;  Store back the value into the RAM and increment Z to the next location
	;mov		r16, r17	;  Copy the value in R17 to R16, to keep the value available for later calculations
	dec		r19	; Decrement R19 by 1
	cpi		r19, 0 ; Compare R19 with zero
	brne	NEXT	; If not zero return to beginning and get another value
	ldi		r19, 25
DIVINIT:
	LD		r17, X+ ;
	mov		r16, r17
DIV7:

	subi	r16, 7	; Subtract the Value in R16 by 7
	cpi		r16, 7	; Check to see if the value in r16 is 7
	brsh	DIV7	; If the value is greater than or equal to 7, jump to DIV7 and continue to subtract
	cp		r16, r0 ; Compare the value in R16 to 0
	brne	NEXTDIV	; If R16 is not 0 Skip to NEXTDIV and do not add value to total

	add		r20, r17	;  If value is divisable by 7 then add value to running total
	adc		r21, r21	;  Add R21 to itself, only increases if there is a carry from previous addition

NEXTDIV:
	mov		r16, r17	;  Copy the Value in R17 into R16
DIV3:
	subi	r16, 3	; Subtract the Value in R16 by 3
	cpi		r16, 3	; Check to see if the value in r16 is 3
	brsh	DIV3	; If the value is greater than or equal to 3, jump to DIV3 and continue to subtract
	cp		r16, r0	; Compare the value in R16 to 0
	brne	DIVDONE	; If R16 is not 0 Skip to DIVDONE and do not add value to total
		
	add		r23, r17	;  If value is divisable by 3 then add value to running total
	adc		r24, r24	;  Add R24 to itself, only increases if there is a carry from previous addition

DIVDONE:
	
	dec		r19		; Decrement R19 by 1
	cpi		r19, 0	; Compare R19 with 0
	brne	DIVINIT	; If not zero return to beginning and get another value

	cpi		r21,0	; Compare R21 with 0
	brne	SETREG	; If R21 is not 0 jump to SETREG
	cpi		r24,0	; Compare R24 with 0
	brne	SETREG	; If R24 is not 0 jump to SETREG

HOLD:
	rjmp HOLD	;Infinite Loop

SETREG:
	ldi		r16, 4	; Copy 4 into R16
	mov		r7, r16	;  Copy 4 into R7, setting bit 3 to high
	rjmp	HOLD	;Jump to infinite loop


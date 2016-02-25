;
; DA1T1.asm
;
; Created: 2/24/2016 9:26:24 PM
; Author : sirfe
;

.macro SET_STACK
	ldi		r16, LOW(RAMEND)
	out		spl, r16
	ldi		r16, HIGH(RAMEND)
	out		sph, r16
.endmacro
	
.cseg
.org 0
	rjmp start

.org 0x20
; Replace with your application code
start:
	SET_STACK

	ldi		r19, 25
	ldi		r20, 0
	ldi		r21, 0
	ldi		r23, 0
	ldi		r24, 0
    ldi		ZH, HIGH(RAMEND)
	ldi		ZL, LOW(RAMEND)
	lsr		ZH
	ror		ZL
NEXT:
	mov		r17, ZL
	st		Z+, r17
	mov		r16, r17
DIV7:
	subi	r16, 7
	cpi		r16, 7
	brsh	DIV7
	cp		r16, r0
	brne	NEXTDIV

	add		r20, r17
	adc		r21, r21

NEXTDIV:
	mov		r16, r17
DIV3:
	subi	r16, 3
	cpi		r16, 3
	brsh	DIV3
	cp		r16, r0
	brne	DIVDONE
		
	add		r23, r17
	adc		r24, r24

DIVDONE:
	
	dec		r19
	cpi		r19, 0
	brne	NEXT

	cpi		r21,0
	brne	SETREG
	cpi		r24,0
	brne	SETREG

HOLD:
	rjmp HOLD

SETREG:
	ldi		r16, 4
	mov		r7, r16
	rjmp	HOLD


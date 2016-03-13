;
; DA2T1.asm
;
; Created: 3/10/2016 9:57:30 AM
; Author : sirfe
;


; Replace with your application code

.macro MAKESTACK				;Start of Stack Macro
	ldi		r16, LOW(RAMEND)	;Copy the lower 8 bits of the end of the RAM into R16
	out		spl, r16			;Copy the address in r16 to the lower portion of the stack pointer
	ldi		r16, HIGH(RAMEND)	;Copy the higher 8 bits of the end of hte RAM into R16
	out		sph, r16			;Copy the address in r16 to the higher portion of the stack pointer
.endmacro


.cseg
.org 0x0
	jmp		START

.org 0x100		;Start of Program
START:
	
	SBI		DDRC, 0
	SBI		DDRC, 4
	SBI		DDRC, 5
	LDI		R21, 0
	LDI		R22, 0
	LDI		R16, 0xFF
	OUT		DDRB, R16
	MAKESTACK
COUNTING:
	call	DELAY
	CPI		R17, 0x01
	BRNE	COUNTING
	INC		R21
	INC		R22
	OUT		PORTB, R21
CHECK1:
	CPI		R22, 5
	BRNE	CHECK2
	LDI		R18, 0x20
	OR		R18, R17
	OUT		PORTC, R18
CHECK2:
	CPI		R22, 10
	BRNE	CHECK3
	LDI		R18, 0x10
	OR		R18, R17
	OUT		PORTC, R18
CHECK3:
	CPI		R22, 15
	BRNE	CHECK4
	LDI		R18, 0x30
	OR		R18, R17
	OUT		PORTC, R18
CHECK4:
	CPI		R22, 20
	BRNE	COUNTING
	LDI		R22, 0
	LDI		R18, 0x00
	OR		R18, R17
	OUT		PORTC, R18
	rjmp	COUNTING

HOLD:
	rjmp	HOLD			;Infinite Loop


DELAY:
	LDI		R16, 0x01
	LDI		R20, 0xF8	;OCR1A = 0xF862, 63586
	STS		TCNT1H, R20	;OCR1AH = 7
	LDI		R20, 0x62
	STS		TCNT1L, R20	;OCR1AL = 158
	LDI		R20, 0x00
	STS		TCCR1A, R20	; Normal Mode
	LDI		R20, 0x05
	STS		TCCR1B, R20 ;1024 prescaler
LOOP1:
	IN		R20, TIFR1
	SBRS	R20, TOV1	;skip next instruction if TOV1 = 1
	rjmp	LOOP1
	EOR		R17, R16
	CPI		R17, 0x00
	BRNE	NOTZERO
	CBI		PORTC, 0
	rjmp	ZERO
NOTZERO:
	SBI		PORTC, 0
ZERO:
	LDI		R19, 1<<TOV1
	OUT		TIFR1, R19	;clear OCF1A
ret

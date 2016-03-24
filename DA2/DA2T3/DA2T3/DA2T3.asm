;
; DA2T1.asm
;
; Created: 3/10/2016 9:57:30 AM
; Author : sirfe
;
.macro MAKESTACK				;Start of Stack Macro
	ldi		r16, LOW(RAMEND)	;Copy the lower 8 bits of the end of the RAM into R16
	out		spl, r16			;Copy the address in r16 to the lower portion of the stack pointer
	ldi		r16, HIGH(RAMEND)	;Copy the higher 8 bits of the end of hte RAM into R16
	out		sph, r16			;Copy the address in r16 to the higher portion of the stack pointer
.endmacro


.cseg
.org 0x0
	jmp		START	;Jump to program start

.org 0x16
	jmp		TIM1_COMPA	;Jump to Interrupt

.org 0x100		;Start of Program
START:
	
	SBI		DDRC, 0	;Enable DDRC.0
	SBI		DDRC, 4	;Enable DDRC.4
	SBI		DDRC, 5	;Enable DDRC.5
	LDI		R21, 0	;Set R21 to 0
	LDI		R22, 0	;Set R22 to 0
	LDI		R17, 0	;Set R17 to 0
	LDI		R16, 0xFF	;Load 0xFF into R16
	OUT		DDRB, R16	;Enable all pins of DDRB
	MAKESTACK	; Call MAKESTACK macro to create the stack

	LDI		R20, 0x02	;Set R20 to 2, 0000 0010
	STS		TIMSK1, R20	;Enable Timer1 compare interrupt
	SEI		;Global Interrupt Enable
	LDI		R20, 0x0F	;When using Chip on breadboard: TCNT1 = 0x079E, 1950
	STS		OCR1AH, R20	;When using Xplained Mini board: OCR1A = 0x0F42, 3906
	LDI		R20, 0x42	;When using Chip on breadboard: TCNT1H = 7, TCNT1L = 158
	STS		OCR1AL, R20	;When using Xplained Mini board: TCNT1H = 15, TCNT1L = 66
	LDI		R20, 0x00	;Set R20 to 0, 00000000
	STS		TCCR1A, R20	;CTC Mode
	LDI		R20, 0x0D	;Set R20 to 13, 00001101
	STS		TCCR1B, R20 ;1024 prescaler
HOLD:
	rjmp	HOLD			;Infinite Loop






TIM1_COMPA: ;OCF1A clears automatically when interrupt is run
	LDI		R16, 0x01	;Set R16 to 1
	EOR		R17, R16	;XOR R17 and R16 and Save into R17
	CPI		R17, 0x00	;Compare R17 with 0
	BRNE	NOTZERO		;Jump to NOTZERO if R17 is not zero
	CBI		PORTC, 0	;Set PORTC.0 to 0
	rjmp	COUNTING	;Jump to COUNTING
NOTZERO:
	SBI		PORTC, 0	;Set PORTC.0 to 1				
	INC		R21		;Increase R21 by 1
	INC		R22		;Increase R22 by 2
	OUT		PORTB, R21	;Copy value of R21 into PORTB
CHECK1:
	CPI		R22, 5	;Compare R22 with 5
	BRNE	CHECK2	;If not 5 skip to CHECK2
	LDI		R18, 0x20;Set R18 to 0x20, 0010 0000
	OR		R18, R17; OR R18 and R17, Keeps value in PORTC.0
	OUT		PORTC, R18;Copy R18 into PORTC
	jmp		COUNTING	;Jump to COUNTING, skip rest of checks
CHECK2:
	CPI		R22, 10	;Compare R22 with 10
	BRNE	CHECK3	;If not 10 skip to CHECK3
	LDI		R18, 0x10;Set R18 to 0x10, 0001 0000
	OR		R18, R17; OR R18 and R17, Keeps Value in PORTC.0
	OUT		PORTC, R18; Copy R18 into PORTC
	jmp		COUNTING	;Jump to COUNTING, skip rest of checks
CHECK3:
	CPI		R22, 15	;Compare R22 with 15
	BRNE	CHECK4	;If not 15 skip to CHECK4
	LDI		R18, 0x30; Set R18 to 0x30, 0011 0000
	OR		R18, R17; OR R18 and R17, Keeps value in PORTC.0
	OUT		PORTC, R18;Copy R18 into PORTC
	jmp		COUNTING	;Jump to COUNTING, skip rest of checks
CHECK4:
	CPI		R22, 20	;Compare R2 wtih 20
	BRNE	COUNTING; If not 20 Skip to COUNTING to exit interrupt
	LDI		R22, 0	; Reset R22 to 0
	LDI		R18, 0x00;Load 0 into R18
	OR		R18, R17; OR R18 and R17, Keeps Value in PORTC.0
	OUT		PORTC, R18; Copy R18 into PORTC
COUNTING:	
RETI
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
	
	SBI		DDRC, 0	;Enable DDRC.0
	SBI		DDRC, 4	;Enable DDRC.4
	SBI		DDRC, 5	;Enable DDRC.5
	LDI		R21, 0	;Set R21 to 0
	LDI		R22, 0	;Set R22 to 0
	LDI		R16, 0xFF	;Load 0xFF into R16
	OUT		DDRB, R16	;Enable all pins of DDRB
	MAKESTACK	; Call MAKESTACK macro to create the stack
COUNTING:
	call	DELAY	;Run DELAY function
	CPI		R17, 0x01	;Compare R17 with 1
	BRNE	COUNTING	;If R17 is 0 go back to COUNTING and run DELAY again
	INC		R21		;Increase R21 by 1
	INC		R22		;Increase R22 by 2
	OUT		PORTB, R21	;Copy value of R21 into PORTB
CHECK1:
	CPI		R22, 5	;Compare R22 with 5
	BRNE	CHECK2	;If not 5 skip to CHECK2
	LDI		R18, 0x20;Set R18 to 0x20, 0010 0000
	OR		R18, R17; OR R18 and R17, Keeps value in PORTC.0
	OUT		PORTC, R18;Copy R18 into PORTC
	rjmp	COUNTING ; Return to COUNTING to repeat loop
CHECK2:
	CPI		R22, 10	;Compare R22 with 10
	BRNE	CHECK3	;If not 10 skip to CHECK3
	LDI		R18, 0x10;Set R18 to 0x10, 0001 0000
	OR		R18, R17; OR R18 and R17, Keeps Value in PORTC.0
	OUT		PORTC, R18; Copy R18 into PORTC
	rjmp	COUNTING ;Return to COUNTING to repeat loop
CHECK3:
	CPI		R22, 15	;Compare R22 with 15
	BRNE	CHECK4	;If not 15 skip to CHECK4
	LDI		R18, 0x30; Set R18 to 0x30, 0011 0000
	OR		R18, R17; OR R18 and R17, Keeps value in PORTC.0
	OUT		PORTC, R18;Copy R18 into PORTC
	rjmp	COUNTING;Return to COUNTING to repeat loop
CHECK4:
	CPI		R22, 20	;Compare R2 wtih 20
	BRNE	COUNTING; If not 20 Return to COUNTING to repeat loop
	LDI		R22, 0	; Reset R22 to 0
	LDI		R18, 0x00;Load 0 into R18
	OR		R18, R17; OR R18 and R17, Keeps Value in PORTC.0
	OUT		PORTC, R18; Copy R18 into PORTC
	rjmp	COUNTING; Return to COUNTING to repeat loop

HOLD:
	rjmp	HOLD			;Infinite Loop


DELAY:
	LDI		R16, 0x01	;Set R16 to 1 
	LDI		R20, 0xF0	;When using Chip on breadboard: TCNT1 = 0xF862, 63586
	STS		TCNT1H, R20	;When using Xplained Mini board: TCNT1 = 0xF0BE, 61630
	LDI		R20, 0xBE	;When using Chip on breadboard: TCNT1H = 248, TCNT1L = 98
	STS		TCNT1L, R20	;When using Xplained Mini board: TCNT1H = 240, TCNT1L = 190
	LDI		R20, 0x00	;Set R20 to 0, 00000000
	STS		TCCR1A, R20	;Normal Mode
	LDI		R20, 0x05	;Set R20 to 5, 00000101
	STS		TCCR1B, R20 ;1024 prescaler
LOOP1:
	IN		R20, TIFR1	;Copy TIFR1 into R20
	SBRS	R20, TOV1	;skip next instruction if TOV1 = 1
	rjmp	LOOP1		;Jump back to LOOP1
	EOR		R17, R16	;XOR R17 and R16 and Save into R17
	CPI		R17, 0x00	;Compare R17 with 0
	BRNE	NOTZERO		;Jump to NOTZERO if R17 is not zero
	CBI		PORTC, 0	;Set PORTC.0 to 0
	rjmp	ZERO		;Jump to ZERO
NOTZERO:
	SBI		PORTC, 0	;Set PORTC.0 to 1
ZERO:
	LDI		R19, 1<<TOV1;Clear TOV1
	OUT		TIFR1, R19	;clear OCF1A
ret; Return to Main Program
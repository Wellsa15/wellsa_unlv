;  Andrew Wells CPE 301 Design Assignment 0
; DA0_T1.asm
;
; Created: 2/13/2016 1:01:54 PM
; Author : sirfe
;  

	ldi R21, 0x04 // Load 0x04 into R21 = 00000100
	out DDRB, R21 // Set bit 2 of PORTB as an Output port
	
start:
	out PORTB, R0  // Sets all the pins of PORTB to LOW

	ldi R16, 31  // Load 31 into R16
	ldi R17, 53  // Load 53 into R17
	ldi R18, 47  // Load 47 into R18
	ldi R19, 35  // Load 35 into R19
	ldi R20, 59  // Load 59 into R20

	add R16, R17  // R16 = R16 + R17
	add R16, R18  // R16 = R16 + R18
	add R16, R19  // R16 = R16 + R19
	add R16, R20  // R16 = R16 + R20
	brcs overflo  // Checks Carry bit to see if the sum produced an overflow, if yes branch to overflo.  If not go to next instruction.

	out PORTB, R0  // Sets all bits of PORTB to LOW
	rjmp start	    // Unconditional Jump back to start

overflo: // Label for Branch instruction
	out PORTB, R21  // Sets PORTB.2 pin to HIGH
    rjmp start  // Unconditional Jump back to start

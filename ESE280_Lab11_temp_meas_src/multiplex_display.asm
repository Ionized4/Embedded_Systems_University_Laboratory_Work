;***************************************************************************
;* 
;* "multiplex_display" - Multiplex the Four Digit LED Display
;*
;* Description: Updates a single digit of the display and increments the 
;*  digit_num to the value of the digit position to be displayed next.
;*
;* Author:
;* Version:
;* Last updated:
;* Target:						;ATmega4809 @ 3.3MHz
;* Number of words:				16
;* Number of cycles:			Variable
;* Low registers modified:	none
;* High registers modified:	r16, r17, r18, Z
;*
;* Parameters:
;* led_display: a four byte array that holds the segment values
;*  for each digit of the display. led_display[0] holds the segment pattern
;*  for digit 0 (the rightmost digit) and so on.
;* digit_num: a byte variable, the least significant two bits provide the
;* index of the next digit to be displayed.
;*
;* Returns: Outputs segment pattern and turns on digit driver for the next
;*  position in the display to be turned ON in the multiplexing sequence.
;*
;* Notes: Ports C and D must be configured as output ports.
;*
;***************************************************************************
.ifndef M_DISPLAY
.set M_DISPLAY = 0
.dseg
	digit_num: .byte 1
	led_display: .byte 4
.cseg

multiplex_display:
	; Load the address of led_display[0]
	ldi ZH, HIGH(led_display)
	ldi ZL, LOW(led_display)
	lds r16, digit_num
	andi r16, 0x03		; Mask the important bits of digit_num
	ldi r17, 0
	add ZL, r16			; Add digit_num to SP
	adc ZH, r17			; Address carry if one occurred
	ld r17, Z			; Load led_display[digit_num]
	ldi r18, 0xF7		; Load bit field for driving transistors
	cpi r16, 0			; Check if r16 is 0
shift_PORTC_driver:
	breq output			; If r16 == 0, drive PORTC
	lsr r18				; Shift r18 right to clear the correct pin
	dec r16				; Decrement r16 and set 0 flag if r16 == 0
	rjmp shift_PORTC_driver
output:
	swap r18
	out VPORTD_OUT, r17 ; Output 7 segment byte
	out VPORTC_OUT, r18	; Output driving nibble
	lds r16, digit_num
	inc r16
	andi r16, 0x03
	sts digit_num, r16
	ret
.endif
; temperature conversion
; This program uses the ADC to read the analog voltage of the MCP9700A.
; When the conversion is complete, the voltage is translated to a
; temperature with a precision of 0.1 degrees celcius.  TCA0 triggers
; an interrupt that multiplexes a seven segment display to display the
; most recent temperature reading.
.nolist
.include "m4809def.inc"
.list

initialization:
	jmp start

.org TCA0_OVF_vect
	jmp TCA0_tick

.org ADC0_RESRDY_vect
	jmp ADC_input_processing

start:
	; Configure port directions
	ldi r16, 0xFF
	out VPORTD_DIR, r16 ; Port D - Output
	ldi r16, 0xF0
	out VPORTC_DIR, r16	; Port C[7:4] - Output
	call config_and_enable_ADC	; Configure the ADC
	call enable_RESRDY_int
	call ADC_start_conversion
    call init_TCA0				; Initialize the counter
	sei							; Set global interrupts
	call post_display			; POST
loop:
	rjmp loop

TCA0_tick:
	cli	; Disable global interrupts

	; Save the values of the modified
	; registers on the stack
	push r19
	push r18
	push r17
	push r16
	in r16, CPU_SREG
	push r16

	; Multiplex the display
	call multiplex_display

	; Disable the timer overflow flag
	ldi r16, TCA_SINGLE_OVF_bm
	sts TCA0_SINGLE_INTFLAGS, r16

	; Restore values of the modified registers
	pop r16
	out CPU_SREG, r16
	pop r16
	pop r17
	pop r18
	pop r19

	sei		; Reenable global interrupts
	reti	; Return from interrupt

ADC_input_processing:
	cli

	; Save all volatile register contents on the stack
	push ZH
	push ZL
	push r23
	push r22
	push r21
	push r20
	push r19
	push r18
	push r17
	push r16
	push r15
	push r14
	push r13
	in r16, CPU_SREG
	push r16

	lds r16, ADC0_RESL	; Read the result
	lds r17, ADC0_RESH

	; Load constant for temp conversion
	ldi r18, LOW(2500)
	ldi r19, HIGH(2500)

	; Temperature conversion
	call mpy16u		; 16 bit multiplication
	lsr r20			; Divide r20 by 2
	ror r19			; Divide r19 by 2 (with carry in)
	lsr r20			; Divide r20 by 2
	ror r19			; Divide r19 by 2 (with carry in)
	mov ZH, r20		; Load the value into a 16 bit register pair
	mov ZL, r19
	; Loop to subtract 500 from the conversion result
	ldi r16, 10
subtraction_loop:
	sbiw Z, 50
	dec r16
	brne subtraction_loop

	; BCD conversion on temperature value
	mov r16, ZL
	mov r17, ZH
	call short_bin16_to_BCD

	; Compute 7seg encodings and
	; store the results
	mov r18, r22
	call hex_to_7seg
	ori r18, 0x80
	sts led_display, r18
	mov r18, r22
	swap r18
	call hex_to_7seg
	sts led_display + 1, r18
	mov r18, r23
	call hex_to_7seg
	ori r18, 0x80
	sts led_display + 2, r18
	mov r18, r23
	swap r18
	call hex_to_7seg
	ori r18, 0x80
	sts led_display + 3, r18

	call ADC_start_conversion

	; Restore registers
	pop r16
	out CPU_SREG, r16
	pop r13
	pop r14
	pop r15
	pop r16
	pop r17
	pop r18
	pop r19
	pop r20
	pop r21
	pop r22
	pop r23
	pop ZL
	pop ZH

	sei
	reti

.nolist
.include "ADC.asm"
.include "post_display.asm"
.include "multiplex_display.asm"
.include "init_TCA0.asm"
.include "hex_to_7seg.asm"
.include "avr204.asm"
.include "avr200.asm"
.include "ShortBin2BCD.asm"
.list

.ifndef POST_DISPLAY_MACRO
.set POST_DISPLAY_MACRO = 1
.nolist
.include "multiplex_display.asm"
.include "var_delay.asm"
.list
; post_display
; A one second software delay that sets led_display to 0.
; Modifies r16 - r19
post_display:				; Save r20 on stack
	ldi r17, 0				; Load 0 into r20
	sts led_display, r17		; Instantiate all the led_display
	sts led_display + 1, r17	; with 0.  This will turn on all
	sts led_display + 2, r17	; segments of each digit when the
	sts led_display + 3, r17	; digit is turned on.
	ldi r18, 40				; Load outer loop counter
post_display_loop:
	ldi r19, 250			; Load variable delay parameter
	call var_delay			; Call the 25ms delay
	dec r18					; Decrement the loop counter
	brne post_display_loop	; Loop if second is not completed
	ret						; Return
.endif

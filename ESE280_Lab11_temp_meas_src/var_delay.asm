.ifndef VARDELAY
.set VARDELAY = 0
; Variable software delay for the ATmega4809 at 3.333MHz
; Approximately 0.1ms * r19
; Uses r16 and r19
var_delay:
outer_loop:
	ldi r16, 110
inner_loop:
	dec r16
	brne inner_loop
	dec r19
	brne outer_loop
	ret
.endif
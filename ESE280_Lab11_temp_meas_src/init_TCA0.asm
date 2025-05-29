.ifndef TCA0_INIT_MACRO
.set TCA0_INIT_MACRO = 1

.equ PERIOD_EXAMPLE_VALUE = 60

; init_TCA0
; Configures TCA0 to trigger an overflow interrupt at a
; frequency proportional to PERIOD_EXAMPLE_VALUE
init_TCA0:
	; Since the counter is not generating a waveform
	; set the waveform mode to normal
	ldi r16, TCA_SINGLE_WGMODE_NORMAL_gc
	sts TCA0_SINGLE_CTRLB, r16

	; An interrupt is being triggered on the overflow
	; of the counter, so the overflow bit of the
	; interrupt control register needs to be set.
	ldi r16, TCA_SINGLE_OVF_bm
	sts TCA0_SINGLE_INTCTRL, r16

	; Loads the period value into the period register.
	; The period example value is defined at the top of
	; this file.
	ldi r16, LOW(PERIOD_EXAMPLE_VALUE)
	sts TCA0_SINGLE_PER, r16
	ldi r16, HIGH(PERIOD_EXAMPLE_VALUE)
	sts TCA0_SINGLE_PER + 1, r16

	; Configure the control A register.  Combines the bitmasks
	; for the 256 clock period division encoding and the enable
	; bit for TCA0 and stores them in the control register.
    ldi r16, TCA_SINGLE_CLKSEL_DIV256_gc | TCA_SINGLE_ENABLE_bm
	sts TCA0_SINGLE_CTRLA, r16

	ret

.endif
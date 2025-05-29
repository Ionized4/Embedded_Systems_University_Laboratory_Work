.ifndef DANR_ADC_FUNCTIONS
.set DANR_ADC_FUNCTIONS = 1

.nolist
.include "m4809def.inc"
.list

; config_and_enable_ADC
; Configures the ADC to read analog
; voltages from PE1
config_and_enable_ADC:
	; Set the internal reference
	ldi r16, VREF_ADC0REFSEL_2V5_gc
	sts VREF_CTRLA, r16

	; Disable digital input on PE1
	ldi r16, 0x04	; Input disable bm
	sts PORTE_PIN1CTRL, r16

	; Configure ADC_CTRLC
	; Combines the bitmasks for use of the internal
	; voltage reference and a clock prescaler of 64
	ldi r16, ADC_REFSEL_INTREF_gc | ADC_PRESC_DIV64_gc
	sts ADC0_CTRLC, r16

	; Select the multiplexer position
	ldi r16, ADC_MUXPOS_AIN9_gc
	sts ADC0_MUXPOS, r16

	; Enable the ADC
	ldi r16, ADC_ENABLE_bm
	sts ADC0_CTRLA, r16
	ret

; Configure ADC0 to operate in freerun mode
config_enable_ADC_freerun:
	lds r16, ADC0_CTRLA
	ori r16, ADC_FREERUN_bm
	sts ADC0_CTRLA, r16
	ldi r16, ADC_STCONV_bm
	sts ADC0_COMMAND, r16
	ret

enable_RESRDY_INT:
	ldi r16, 1
	sts ADC0_INTCTRL, r16
	ret

ADC_start_conversion:
	ldi r16, ADC_STCONV_bm
	sts ADC0_COMMAND, r16
	ret

.endif
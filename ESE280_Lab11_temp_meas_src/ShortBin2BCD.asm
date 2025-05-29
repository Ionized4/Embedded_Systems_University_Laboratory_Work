;***************************************************************************
;* 
;* "bin16_to_BCD" - 16-bit Binary to BCD Conversion
;*
;* Description: Converts a 16-bit unsigned binary number to a five digit
;* packed BCD number. Uses subroutine div16u from Atmel application note AVR200
;*
;* Author:					Ken Short
;* Version:					0.0
;* Last updated:			111320
;* Target:					ATmega4809
;* Number of words:
;* Number of cycles:
;* Low registers modified:	r14, r15
;* High registers modified: r16, r17, r18, r19, r20, r22, r23, r24
;*
;* Parameters: r17:r16 16-bit unsigned right justified number to be converted.
;* Returns:		r24:r23:r22 five digit packed BCD result.
;*
;* Notes: 
;* Subroutine uses repeated division by 10 to perform conversion.
;***************************************************************************
short_bin16_to_BCD:
	ldi r19, 0			;high byte of divisor for div16u
	ldi r18, 10			;low byte of the divisor for div16u

	rcall div16u		;divide original binary number by 10
	mov r22, r14		;result is BCD digit 0 (least significant digit)
	rcall div16u		;divide result from first division by 10, gives digit 1 
	swap r14			;swap digit 1 for packing
	or r22, r14			;pack

	rcall div16u		;divide result from second division by 10, gives digit 2
	mov r23, r14		;place in r23
	rcall div16u		;divide result from third division by 10, gives digit 3 
	swap r14			;swap digit 3 for packing
	or r23, r14			;pack

	rcall div16u		;divide result from fourth division by 10, gives digit 4
	mov r24, r14		;place in r24

	ret
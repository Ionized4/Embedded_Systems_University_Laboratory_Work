;***************************************************************************
;* 
;* "hex_to_7seg" - Hexadecimal to Seven Segment Conversion
;*
;* Description: Converts a right justified hexadecimal digit to the seven
;* segment pattern required to display it. Pattern is right justified a
;* through g. Pattern uses 0s to turn segments on ON.
;*
;* Author:						Ken Short
;* Version:						1.0						
;* Last updated:				101620
;* Target:						ATmega4809
;* Number of words:				8
;* Number of cycles:			13
;* Low registers modified:		none		
;* High registers modified:		r18, r19, XL, XH
;*
;* Parameters: r18: right justified hex digit, high nibble 0
;* Returns: r18: segment values a through g right justified
;*
;* Notes: 
;*
;***************************************************************************
.ifndef HEX_7SEG
.set HEX_7SEG = 0

hex_to_7seg:
	push ZH
	push ZL
	andi r18, 0x0F				;clear ms nibble
    ldi ZH, HIGH(hextable << 1) ;set Z to point to start of table
    ldi ZL, LOW(hextable << 1)
    ldi r19, $00                ;add offset to Z pointer
    add ZL, r18
    adc ZH, r19
    lpm r18, Z                  ;load byte from table pointed to by Z
	pop ZL
	pop ZH
	ret

hextable: .db $01, $4F, $12, $06, $4C, $24, $20, $0F, $00, $04, $08, $60, $31, $42, $30, $38
.endif
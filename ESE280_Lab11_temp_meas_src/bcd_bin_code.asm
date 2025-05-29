.ifndef BRYANTS_BCD_CODE
.set BRYANTS_BCD_CODE = 1
;***************************************************************************
;*
;* "bin2BCD16" - 16-bit Binary to BCD conversion
;*
;* This subroutine converts a 16-bit number (fbinH:fbinL) to a 5-digit
;* packed BCD number represented by 3 bytes (tBCD2:tBCD1:tBCD0).
;* MSD of the 5-digit number is placed in the lowermost nibble of tBCD2.
;*
;* Number of words	:25
;* Number of cycles	:751/768 (Min/Max)
;* Low registers used	:3 (tBCD0,tBCD1,tBCD2)
;* High registers used  :4(fbinL,fbinH,cnt16a,tmp16a)	
;* Pointers used	:Z
;*
;***************************************************************************

;***** Subroutine Register Variables

.dseg
tBCD0: .byte 1  // BCD digits 1:0
tBCD1: .byte 1  // BCD digits 3:2
tBCD2: .byte 1  // BCD digits 4

.cseg
.def	tBCD0_reg = r13		;BCD value digits 1 and 0
.def	tBCD1_reg = r14		;BCD value digits 3 and 2
.def	tBCD2_reg = r15		;BCD value digit 4

.def	fbinL = r16		;binary value Low byte
.def	fbinH = r17		;binary value High byte

.def	cnt16a	=r18		;loop counter
.def	tmp16a	=r19		;temporary value

;***** Code

bin2BCD16:
    push fbinL
    push fbinH
    push cnt16a
    push tmp16a


	ldi	cnt16a, 16	;Init loop counter	
    ldi r20, 0x00
    sts tBCD0, r20 ;clear result (3 bytes)
    sts tBCD1, r20
    sts tBCD2, r20
bBCDx_1:
    // load values from memory
    lds tBCD0_reg, tBCD0
    lds tBCD1_reg, tBCD1
    lds tBCD2_reg, tBCD2

    lsl	fbinL		;shift input value
	rol	fbinH		;through all bytes
	rol	tBCD0_reg		;
	rol	tBCD1_reg
	rol	tBCD2_reg

    sts tBCD0, tBCD0_reg
    sts tBCD1, tBCD1_reg
    sts tBCD2, tBCD2_reg

	dec	cnt16a		;decrement loop counter
	brne bBCDx_2		;if counter not zero

    pop tmp16a
    pop cnt16a
    pop fbinH
    pop fbinL
ret			; return
    bBCDx_2:
    // Z Points tBCD2 + 1, MSB of BCD result + 1
    ldi ZL, LOW(tBCD2 + 1)
    ldi ZH, HIGH(tBCD2 + 1)
    bBCDx_3:
	    ld tmp16a, -Z	    ;get (Z) with pre-decrement
	    subi tmp16a, -$03	;add 0x03

	    sbrc tmp16a, 3      ;if bit 3 not clear
	    st Z, tmp16a	    ;store back

	    ld tmp16a, Z	;get (Z)
	    subi tmp16a, -$30	;add 0x30

	    sbrc tmp16a, 7	;if bit 7 not clear
        st Z, tmp16a	;	store back

	    cpi	ZL, LOW(tBCD0)	;done all three?
    brne bBCDx_3
        cpi	ZH, HIGH(tBCD0)	;done all three?
    brne bBCDx_3
rjmp bBCDx_1		



;***************************************************************************
;*
;* "BCD2bin16" - BCD to 16-Bit Binary Conversion
;*
;* This subroutine converts a 5-digit packed BCD number represented by
;* 3 bytes (fBCD2:fBCD1:fBCD0) to a 16-bit number (tbinH:tbinL).
;* MSD of the 5-digit number must be placed in the lowermost nibble of fBCD2.
;*
;* Let "abcde" denote the 5-digit number. The conversion is done by
;* computing the formula: 10(10(10(10a+b)+c)+d)+e.
;* The subroutine "mul10a"/"mul10b" does the multiply-and-add operation
;* which is repeated four times during the computation.
;*
;* Number of words	:30
;* Number of cycles	:108
;* Low registers used	:4 (copyL,copyH,mp10L/tbinL,mp10H/tbinH)
;* High registers used  :4 (fBCD0,fBCD1,fBCD2,adder)	
;*
;***************************************************************************

;***** "mul10a"/"mul10b" Subroutine Register Variables

.def	copyL	=r12		;temporary register
.def	copyH	=r13		;temporary register
.def	mp10L	=r14		;Low byte of number to be multiplied by 10
.def	mp10H	=r15		;High byte of number to be multiplied by 10
.def	adder	=r19		;value to add after multiplication	

;***** Code

mul10a:	;***** multiplies "mp10H:mp10L" with 10 and adds "adder" high nibble
	swap	adder
mul10b:	;***** multiplies "mp10H:mp10L" with 10 and adds "adder" low nibble
	mov	copyL,mp10L	;make copy
	mov	copyH,mp10H
	lsl	mp10L		;multiply original by 2
	rol	mp10H
	lsl	copyL		;multiply copy by 2
	rol	copyH		
	lsl	copyL		;multiply copy by 2 (4)
	rol	copyH		
	lsl	copyL		;multiply copy by 2 (8)
	rol	copyH		
	add	mp10L,copyL	;add copy to original
	adc	mp10H,copyH	
	andi	adder,0x0f	;mask away upper nibble of adder
	add	mp10L,adder	;add lower nibble of adder
    brcc	m10_1		;if carry not cleared
	    inc	mp10H		;	inc high byte
    m10_1:
ret	

;***** Main Routine Register Variables

.def	tbinL	=r14		;Low byte of binary result (same as mp10L)
.def	tbinH	=r15		;High byte of binary result (same as mp10H)
.def	fBCD0	=r16		;BCD value digits 1 and 0
.def	fBCD1	=r17		;BCD value digits 2 and 3
.def	fBCD2	=r18		;BCD value digit 5

;***** Code

BCD2bin16:
	andi	fBCD2,0x0f	;mask away upper nibble of fBCD2
	clr	mp10H		
	mov	mp10L,fBCD2	;mp10H:mp10L = a
	mov	adder,fBCD1
	rcall	mul10a		;mp10H:mp10L = 10a+b
	mov	adder,fBCD1
	rcall	mul10b		;mp10H:mp10L = 10(10a+b)+c
	mov	adder,fBCD0		
	rcall	mul10a		;mp10H:mp10L = 10(10(10a+b)+c)+d
	mov	adder,fBCD0
	rcall	mul10b		;mp10H:mp10L = 10(10(10(10a+b)+c)+d)+e
ret
.endif
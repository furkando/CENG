LIST    P=18F8722

#INCLUDE <p18f8722.inc> 
    
CONFIG OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

UDATA_ACS
  t1	res 1
  t2	res 1
  t3	res 1

v1 udata 0x20
i
 
v2 udata 0x21
j

v3 udata 0x22
l
 
v4 udata 0x23
buttonReleased 
	
v5 udata 0x24
isStart 
 
v6 udata 0x25
isOn
 
v7 udata 0x26
isOff
 
v8 udata 0x27
checked 
 
v9 udata 0x28
atWait
 
ORG     0x00
goto    main

init
bsf isStart,0
bsf isOff,0
movlw b'00010000'
movwf TRISA
movlw b'00001111'
movwf LATA
 
movlw b'00000000'
movwf TRISB
movlw b'00001111'
movwf LATB
 
movlw b'00000000'
movwf TRISC
movlw b'00001111'
movwf LATC 
 
movlw b'00000000'
movwf TRISD
movlw b'00001111'
movwf LATD

call delay
call delay
call delay
call delay
 
clrf LATA
clrf LATB
clrf LATC
clrf LATD
  
call delay  
call delay

movlw b'11111111'
movwf TRISE
clrf LATE
 
 
clrf PORTA
clrf PORTE

movlw 0
movwf j

bsf buttonReleased,0
bcf checked,0
return
 
lighton:
    bcf		atWait,0
    btfss	isStart,0
    goto	initLight
    btfss	isOff,0
    goto	fromOff
    movlw	4
    movwf	i
    movlw	0
    movwf	j
    goto	cont1
    initLight:
	btfsc isOn,0
	goto cont1
    fromOff:
	movf	j,0
	sublw	4
	movwf	j
	movf	i,0
	sublw	4
	movwf	i
    cont1:
	btfss	LATA,3
	goto	forloopa
	goto	checkLightOn1
    forloopa:
        bsf	isOn,0
	bcf	isOff,0
	bcf	isStart,0
	btfsc buttonReleased,1 
	goto inloopa
	btfss buttonReleased,0
	goto buttonReleased1
	btfss checked,0
	goto buttonCheck
	bcf checked,0
	inloopa:
	    bcf	buttonReleased,1
	    incf	j,1
	    movff	j,l
	    dcfsnz	l,1
	    bsf		LATA,0
	    dcfsnz	l,1
	    bsf		LATA,1
	    dcfsnz	l,1
	    bsf		LATA,2
	    dcfsnz	l,1
	    bsf		LATA,3
	    call delay
	    decf i,1
	    btfss buttonReleased,0
	    goto buttonReleased2
	    innerloopa:
		movlw	0
		cpfseq	i
		goto	forloopa
		bsf	isStart,0
	
    initLight1:
	btfss	isStart,0
	btfss	LATB,3
	goto	checkLightOn1
	goto	checkLightOn2
    checkLightOn1:
	btfsc	LATB,3
	goto	checkLightOn2
	btfss	isStart,0
	goto	forloopb
	movlw	0
	movwf	j
	movlw	4
	movwf	i
    forloopb:
	bsf isOn,0
	bcf	isOff,0
	bcf	isStart,0
	btfsc buttonReleased,1 
	goto inloopb
	btfss buttonReleased,0
	goto buttonReleased1
	btfss checked,0
	goto buttonCheck
	bcf checked,0
	inloopb:
	    bcf buttonReleased,1
	    incf	j,1
	    movff	j,l
	    dcfsnz	l,1
	    bsf		LATB,0
	    dcfsnz	l,1
	    bsf		LATB,1
	    dcfsnz	l,1
	    bsf		LATB,2
	    dcfsnz	l,1
	    bsf		LATB,3
	    call delay
	    decf i,1
	    btfss buttonReleased,0
	    goto buttonReleased2
	    innerloopb:
		movlw	0
		cpfseq	i
		goto	forloopb
		bsf	isStart,0


    initLight2:
	btfss	isStart,0
	btfss	LATC,3
	goto	checkLightOn2
	goto	checkLightOn3
    checkLightOn2:
	btfsc	LATC,3
	goto	checkLightOn3
	btfss	isStart,0
	goto	forloopc
	movlw	0
	movwf	j
	movlw	4
	movwf	i
    forloopc:
        bsf isOn,0
	bcf isOff,0
	bcf isStart,0
	btfsc buttonReleased,1 
	goto inloopc
	btfss buttonReleased,0
	goto buttonReleased1
	btfss checked,0
	goto buttonCheck
	bcf checked,0
	inloopc:
	    bcf	buttonReleased,1
	    incf	j,1
	    movff	j,l
	    dcfsnz	l,1
	    bsf		LATC,0
	    dcfsnz	l,1
	    bsf		LATC,1
	    dcfsnz	l,1
	    bsf		LATC,2
	    dcfsnz	l,1
	    bsf		LATC,3
	    call delay
	    decf i,1
	    btfss buttonReleased,0
	    goto buttonReleased2
	    innerloopc:
		movlw	0
		cpfseq	i
		goto	forloopc
		bsf	isStart,0

    initLight3:
	btfss	isStart,0
	btfss	LATD,3
	goto	checkLightOn3
	goto	waiting
    checkLightOn3:
	btfsc	LATD,3
	goto	waiting
	btfss	isStart,0
	goto	forloopd
	movlw	0
	movwf	j
	movlw	4
	movwf	i
    forloopd:
	bsf	isOn,0
	bcf	isOff,0
	bcf	isStart,0
	btfsc buttonReleased,1 
	goto inloopd
	btfss buttonReleased,0
	goto buttonReleased1
	btfss checked,0
	goto buttonCheck
	bcf checked,0
	inloopd:
	    bcf	buttonReleased,1
	    incf	j,1
	    movff	j,l
	    dcfsnz	l,1
	    bsf		LATD,0
	    dcfsnz	l,1
	    bsf		LATD,1
	    dcfsnz	l,1
	    bsf		LATD,2
	    dcfsnz	l,1
	    bsf		LATD,3
	    call delay
	    decf i,1
	    btfss buttonReleased,0
	    goto buttonReleased2
	    innerloopd:
		movlw	0
		cpfseq	i
		goto	forloopd
		bsf	isStart,0
		goto waiting
    
lightoff:
    bcf		atWait,0
    btfss	isStart,0
    goto	initLightoff
    btfss	isOn,0
    goto	fromOn
    movlw	4
    movwf	i
    movlw	0
    movwf	j
    goto	cont
    initLightoff:
	btfsc isOff,0
	goto cont
    fromOn:
	movf	j,0
	sublw	4
	movwf	j
	movf	i,0
	sublw	4
	movwf	i
    cont:
	btfss	LATD,0
	goto	checkLightOff1
    forloopdoff:
	bcf isOn,0
	bsf isOff,0
	bcf isStart,0
	btfsc buttonReleased,1 
	goto inloopoffd
	btfss buttonReleased,0
	goto buttonReleased1
	btfss checked,0
	goto buttonCheck
	bcf checked,0
	inloopoffd:
	    bcf	buttonReleased,1
	    incf	j,1
	    movff	j,l
	    dcfsnz	l,1
	    bcf		LATD,3
	    dcfsnz	l,1
	    bcf		LATD,2
	    dcfsnz	l,1
	    bcf		LATD,1
	    dcfsnz	l,1
	    bcf		LATD,0
	    call delay
	    decf i,1
	    btfss buttonReleased,0
	    goto buttonReleased2
	    innerloopoffd:
		movlw	0
		cpfseq	i
		goto	forloopdoff
		bsf	isStart,0
    initLightoff1:
	btfss	isStart,0
	btfsc	LATC,0
	goto	checkLightOff1
	goto	checkLightOff2
    checkLightOff1:
	btfss	LATC,0
	goto	checkLightOff2
	btfss	isStart,0
	goto	forloopcoff
	movlw	0
	movwf	j
	movlw	4
	movwf	i
    forloopcoff:
	bcf	isOn,0
	bsf	isOff,0
	bcf	isStart,0
	btfsc buttonReleased,1 
	goto inloopoffc
	btfss buttonReleased,0
	goto buttonReleased1
	btfss checked,0
	goto buttonCheck
	bcf checked,0
	inloopoffc:
	    bcf	buttonReleased,1
	    incf	j,1
	    movff	j,l
	    dcfsnz	l,1
	    bcf		LATC,3
	    dcfsnz	l,1
	    bcf		LATC,2
	    dcfsnz	l,1
	    bcf		LATC,1
	    dcfsnz	l,1
	    bcf		LATC,0
	    call delay 
	    decf i,1
	    btfss buttonReleased,0
	    goto buttonReleased2
	    innerloopoffc:
		movlw	0
		cpfseq	i
		goto	forloopcoff
		bsf	isStart,0
    initLightoff2:
	btfss	isStart,0
	btfsc	LATB,0
	goto	checkLightOff2
	goto	checkLightOff3
    checkLightOff2:
	btfss	LATB,0
	goto	checkLightOff3
	btfss	isStart,0
	goto	forloopboff
	movlw	0
	movwf	j
	movlw	4
	movwf	i
    forloopboff:
	bcf	isOn,0
	bsf	isOff,0
	bcf	isStart,0
	btfsc buttonReleased,1 
	goto inloopoffb
	btfss buttonReleased,0
	goto buttonReleased1
	btfss checked,0
	goto buttonCheck
	bcf checked,0
	inloopoffb:
	    bcf	buttonReleased,1
	    incf	j,1
	    movff	j,l
	    dcfsnz	l,1
	    bcf		LATB,3
	    dcfsnz	l,1
	    bcf		LATB,2
	    dcfsnz	l,1
	    bcf		LATB,1
	    dcfsnz	l,1
	    bcf		LATB,0
	    call delay	    
	    decf i,1
	    btfss buttonReleased,0
	    goto buttonReleased2
	    innerloopoffb:
		movlw	0
		cpfseq	i
		goto	forloopboff
		bsf	isStart,0
    initLightoff3:
	btfss	isStart,0
	btfsc	LATA,0
	goto	checkLightOff3
	goto	waiting
    checkLightOff3:
	btfss	LATA,0
	goto	waiting
	btfss	isStart,0
	goto	forloopaoff
	movlw	0
	movwf	j
	movlw	4
	movwf	i	 
    forloopaoff:
	bcf	isOn,0
	bsf	isOff,0
	bcf	isStart,0
	btfsc buttonReleased,1 
	goto inloopoffa
	btfss buttonReleased,0
	goto buttonReleased1
	btfss checked,0
	goto buttonCheck
	bcf checked,0
	inloopoffa:
	    bcf	buttonReleased,1
	    incf	j,1
	    movff	j,l
	    dcfsnz	l,1
	    bcf		LATA,3
	    dcfsnz	l,1
	    bcf		LATA,2
	    dcfsnz	l,1
	    bcf		LATA,1
	    dcfsnz	l,1
	    bcf		LATA,0
	    call delay
	    decf i,1
	    btfss buttonReleased,0
	    goto buttonReleased2
	    innerloopoffa:
		movlw	0
		cpfseq	i
		goto	forloopaoff
		bsf	isStart,0
		goto	waiting

waiting:
    bsf atWait,0
    bsf  isStart,0
    waitingRoom:
	btfss buttonReleased,0
	goto buttonReleased1
	goto buttonCheck
	goto waitingRoom

delay
movlw 0X6d
movwf t1
movlw 0X5e
movwf t2
movlw 0X1a
movwf t3
loop:
    decfsz t1, 1
    goto loop
    decfsz t2, 1
    goto loop
    decfsz t3, 1
    goto loop
    nop
    return
    
finder:
    btfsc atWait,0
    goto waitingRoom
    btfss isOn,0
    goto lightoff
    goto lighton
    
buttonCheck:
    bsf checked,0
    btfss PORTA,4
    goto finder
    bcf buttonReleased,0
    goto buttonReleasedin1
    
buttonReleased1:
    bsf buttonReleased,1
    buttonReleasedin1:
	btfsc PORTA,4
	goto finder
	bsf buttonReleased,0
	goto buttonCheck1
	
buttonReleased2:
    movlw	0
    cpfseq	i
    goto buttonReleasedin2
    bsf		isStart,0
    buttonReleasedin2:
	btfsc PORTA,4
	goto finder
	decfsz i,0
	bsf isStart,0
	bsf buttonReleased,0
	goto buttonCheck1

buttonCheck1:
	btfss PORTE,3
	goto buttonCheck2
	buttonReleased3:
	btfss PORTE,3
	goto lighton
	goto buttonReleased3
	
	
buttonCheck2:
	btfss PORTE,4
	goto buttonCheck1
	buttonReleased4:
	btfss PORTE,4
	goto lightoff
	goto buttonReleased4
	
main: 
    call init
    mainLoop:
	goto lighton
	goto mainLoop
end
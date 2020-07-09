;Furkan Dogan/2098937
;Sinan Talha Kosar/2099190
list P=18F8722

#include <p18f8722.inc>
config OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

state   udata 0x21
state
   
moduloReg   udata 0x22
moduloReg
   
tlow udata 0x23
tlow
 
thigh udata 0x24
thigh

astReg udata 0x25
astReg
 
i udata 0x26
i
 
copyReg udata 0x27
copyReg
 
counterl udata 0x28
counterl
 
counterh udata 0x29
counterh 

astCounter udata 0x2A
astCounter
 
j udata 0x2B
j
 
ast0 udata 0x2C
ast0
 
ast1 udata 0x2D
ast1
 
ast2 udata 0x2E
ast2
 
ast3 udata 0x2F
ast3
 
ast4 udata 0x30
ast4
 
ast5 udata 0x31
ast5

fire0 udata 0x32
fire0
 
fire1 udata 0x33
fire1
 
fire2 udata 0x34
fire2
 
fire3 udata 0x35
fire3
 
fire4 udata 0x36
fire4
 
fire5 udata 0x37
fire5 

res0 udata 0x38
res0
 
res1 udata 0x39
res1
 
res2 udata 0x3A
res2
 
res3 udata 0x3B
res3
 
res4 udata 0x3C
res4
 
res5 udata 0x3D
res5  
 
game_over   udata   0x3E
game_over
   
ship   udata   0x3F
ship 
   
w_temp  udata 0x40
w_temp

status_temp udata 0x41
status_temp

pclath_temp udata 0x42
pclath_temp
 
astronom udata 0x43
astronom
 
modCount udata 0x44
modCount
 
score1 udata 0x45
score1
 
score2 udata 0x46
score2 

  
score3 udata 0x47
score3
 
fir0 udata 0x48
fir0
 
fir1 udata 0x49
fir1
 
fir2 udata 0x4A
fir2
 
fir3 udata 0x4B
fir3
 
fir4 udata 0x4C
fir4
 
fir5 udata 0x4D
fir5 
 
dcounter1 EQU 0X0C
dcounter2 EQU 0X0D
dcounter3 EQU 0X0E

org     0x00
goto    init

org     0x08
goto    isr             ;go to interrupt service routine
 
isr:
    call    score_show
    call    save_registers1
    bcf	    INTCON,2
    movlw   b'01111000'
    movwf   TMR0L
    infsnz  counterl
    incf    counterh
    movlw   0x32
    cpfslt  astCounter
    goto    fourth
    movlw   0x1E
    cpfslt  astCounter
    goto    third
    movlw   0x0A
    cpfslt  astCounter
    goto    second
    goto    first
    

init:
    clrf    ast0
    clrf    ast1
    clrf    ast2
    clrf    ast3
    clrf    ast4
    clrf    ast5
    clrf    fire0
    clrf    fire1
    clrf    fire2
    clrf    fire3
    clrf    fire4
    clrf    fire5
    clrf    modCount
    clrf    astCounter
    clrf    game_over
    clrf    state
    clrf    astronom
    clrf    i
    clrf    j
    clrf    game_over
    clrf    ship
    
    clrf    TMR1H
    clrf    TMR1L
    
    ;Disable interrupts
    clrf    INTCON
    clrf    INTCON2

    ;Configure Input - Output Ports
    clrf    LATA 
    clrf    TRISA 
    clrf    LATB
    clrf    TRISB
    clrf    LATC
    clrf    TRISC
    clrf    LATD
    clrf    TRISD
    clrf    LATE
    clrf    TRISE
    clrf    LATF
    clrf    TRISF
    clrf    TRISH
    clrf    TRISJ
    clrf    LATG ; clear LATG
    movlw   b'00001000' ; w_reg =  b'00001100'
    movwf   LATA   ; TRISG = =w_reg = b'00001100' 
    
    ;Configure Input/Interrupt Ports
    movlw   b'00001111' ; w_reg =  b'00001100'
    movwf   TRISG   ; TRISG = =w_reg = b'00001100' 
    movlw   0x03
    movwf   state
    
    ;Initialize Timer0 and Timer1
    movlw   b'01000101' ;Disable Timer0 by setting TMR0ON to 0 (for now)
    movwf   T0CON
    movlw   b'01010000'
    movwf   T1CON
    
    ;Enable interrupts
    movlw   b'11100000' ;Enable Global, peripheral, Timer0 and RB interrupts by setting GIE, PEIE, TMR0IE and RBIE bits to 1
    movwf   INTCON
    movlw   b'01111000'
    movwf   TMR0L
    bsf	    T1CON, 0
    
main:
    call    score_show
    btfsc   PORTG,0
    goto    preparer
    goto    main

preparer:
    clrf    LATH
    clrf    LATJ
    clrf    score1
    clrf    score2
    clrf    score3
    bsf     T0CON, 7    ;Enable Timer0 by setting TMR0ON to 1
    call    first_astroid
    
start:
    btfsc   game_over,0
    goto    init
    btfsc   PORTG,2
    call    down_handler
    btfsc   PORTG,3
    call    up_handler
    btfss   PORTG,1
    goto    start
    call    fire_table
    goto    start

score_show
    
    bcf	    LATH,3
    movff   score3,i
    movf    score3,0
    sublw   0x00
    btfsc   STATUS,2
    movlw   b'00111111'
    dcfsnz  i 
    movlw   b'00000110'
    dcfsnz  i 
    movlw   b'01011011'
    dcfsnz  i 
    movlw   b'01001111'
    dcfsnz  i 
    movlw   b'01100110'
    dcfsnz  i 
    movlw   b'01101101'
    dcfsnz  i 
    movlw   b'01111101'
    dcfsnz  i 
    movlw   b'00000111'
    dcfsnz  i 
    movlw   b'01111111'
    dcfsnz  i 
    movlw   b'01100111'
    
   
    
    
    movwf   LATJ
    bsf	    LATH,1
    
    call    delay2
    call    delay2
    
    bcf	    LATH,1
    movff   score2,i
    movf    score2,0
    sublw   0x00
    btfsc   STATUS,2
    movlw   b'00111111'
    dcfsnz  i 
    movlw   b'00000110'
    dcfsnz  i 
    movlw   b'01011011'
    dcfsnz  i 
    movlw   b'01001111'
    dcfsnz  i 
    movlw   b'01100110'
    dcfsnz  i 
    movlw   b'01101101'
    dcfsnz  i 
    movlw   b'01111101'
    dcfsnz  i 
    movlw   b'00000111'
    dcfsnz  i 
    movlw   b'01111111'
    dcfsnz  i 
    movlw   b'01100111'
    
 
    
    
    movwf   LATJ
    bsf	    LATH,2
    
    call    delay2
    call    delay2
    
    bcf	    LATH,2
    
    
    movff   score1,i
    movf    score1,0
    sublw   0x00
    btfsc   STATUS,2
    movlw   b'00111111'
    dcfsnz  i 
    movlw   b'00000110'
    dcfsnz  i 
    movlw   b'01011011'
    dcfsnz  i 
    movlw   b'01001111'
    dcfsnz  i 
    movlw   b'01100110'
    dcfsnz  i 
    movlw   b'01101101'
    dcfsnz  i 
    movlw   b'01111101'
    dcfsnz  i 
    movlw   b'00000111'
    dcfsnz  i 
    movlw   b'01111111'
    dcfsnz  i 
    movlw   b'01100111'
    
    
    movwf   LATJ
    bsf	    LATH,3
    
    call    delay2
    
    return

    
fire_table
    call    delay
    movff   state,i
    movf    state,0
    sublw   0x00
    btfsc   STATUS,2
    bsf	    fire0,5
    dcfsnz  i 
    bsf	    fire1,5
    dcfsnz  i 
    bsf	    fire2,5
    dcfsnz  i 
    bsf	    fire3,5
    dcfsnz  i 
    bsf	    fire4,5
    dcfsnz  i 
    bsf	    fire5,5
    return

    
down_handler
    call    delay
    movff   state,i
    movlw   0x05
    cpfseq  state
    incf    state
    call    up_table
    call    down_table
    return

up_handler
    call    delay
    movff   state,i
    clrf    WREG
    cpfseq  state
    decf    state
    call    up_table
    call    down_table
    return    

down_table
    movff   state,i
    movf    i,0
    sublw   0x00
    btfsc   STATUS,2
    bsf	    LATA,0
    dcfsnz  i 
    bsf	    LATA,1
    dcfsnz  i 
    bsf	    LATA,2
    dcfsnz  i 
    bsf	    LATA,3
    dcfsnz  i 
    bsf	    LATA,4
    dcfsnz  i 
    bsf	    LATA,5
    return

up_table
    movf    i,0
    sublw   0x00
    btfsc   STATUS,2
    bcf	    LATA,0
    dcfsnz  i 
    bcf	    LATA,1
    dcfsnz  i 
    bcf	    LATA,2
    dcfsnz  i 
    bcf	    LATA,3
    dcfsnz  i 
    bcf	    LATA,4
    dcfsnz  i 
    bcf	    LATA,5
    return
    
delay
    movlw 0xb5
    movwf dcounter1
    movlw 0xaf
    movwf dcounter2
    movlw 0x0d
    movwf dcounter3
    loop:
	decfsz dcounter1, 1
	goto loop
	decfsz dcounter2, 1
	goto loop
	decfsz dcounter3, 1
	goto loop
	nop
    return
    
delay2
    movlw 0x51
    movwf dcounter1
    loop2:
	decfsz dcounter1, 1
	goto loop2
	nop
    return
    
save_registers1
    call    save_registers
    return
    
astroid_handler:
    incf    astCounter
    movlw   b'00000111'; Son 3 biti almak icin WREGe atiyor
    andwf   tlow,0 ; tlow ile andlediginde WREGde tlow un son 3 biti WREGde
    movwf   moduloReg ; WREGde modulo isleminin sonucu var
    btfss   WREG,2 ; eger son 3 bitlik 4, 5, 6 veya 7 ise
    goto    ast_exit ; degilse ayni deger modulo sonucu
    btfss   WREG,1 ; eger son 3 bitlik 6 veya 7 ise
    goto    ast_exit ; degilse ayni deger modulo sonucu
    andlw   b'00000001' ;6 veya 7 ise 3 luk bitin son degeri modulo sonucu
    movwf   moduloReg ; modulo sonucu ayarlandi
    goto    ast_exit ;right shift ve led yakmak icin gidiyor
    
first_astroid:
    incf    astCounter
    movff   TMR1L, tlow ;TMR1 in 8lik sondaki
    movff   TMR1H, thigh ;TMR1 in 8lik bastaki
    bcf	    T1CON,0 ; TMR1 in degeri savelendikten sonra ihtiyac yok
    movlw   b'00000111'; Son 3 biti almak icin WREGe atiyor
    andwf   tlow,0 ; tlow ile andlediginde WREGde tlow un son 3 biti WREGde
    movwf   moduloReg ; WREGde modulo isleminin sonucu var
    btfss   WREG,2 ; eger son 3 bitlik 4, 5, 6 veya 7 ise
    goto    ast_exit ; degilse ayni deger modulo sonucu
    btfss   WREG,1 ; eger son 3 bitlik 6 veya 7 ise
    goto    ast_exit ; degilse ayni deger modulo sonucu
    andlw   b'00000001' ;6 veya 7 ise 3 luk bitin son degeri modulo sonucu
    movwf   moduloReg ; modulo sonucu ayarlandi
    goto    ast_exit ;right shift ve led yakmak icin gidiyor
    
ast_exit
    movlw   0x0A
    cpfslt  modCount
    call    complementor
    movf    moduloReg,0 ;moduloReg WREG de
    sublw   0x0 ;0 - WREG yapiyor
    btfsc   STATUS,2 ;eger 0- WREG = 0 ise Z=1 oluyor
    bsf	    LATF,0 ; ozaman F0 i yakiyor
    movf    moduloReg,0 ;0-WREG =! 0 ise WREG e atmak laz?m
    movwf   copyReg
    call    astroid_table
    dcfsnz  copyReg ; asagisi modulo sonucuna gore led yakiyor (moduloReg=0x1 ise F=0000 0010 olmasi gerektiginden)
    bsf	    LATF,1
    dcfsnz  copyReg
    bsf	    LATF,2
    dcfsnz  copyReg
    bsf	    LATF,3
    dcfsnz  copyReg
    bsf	    LATF,4
    dcfsnz  copyReg
    bsf	    LATF,5
    movf    thigh,0 ; thighi WREG e at, cunku thigh rightshiflendiginde degisecek
    rrncf   thigh,1 ;thighi right shifle
    bsf	    thigh,7
    btfss   tlow,0 ;16 bitlik rightshift olmasi gerektiginden tlowun 0.biti thighin 7 biti olmal?
    bcf	    thigh,7
    rrncf   tlow,1 ;tlow u right shiftle
    bsf	    tlow,7
    btfss   copyReg,0 ;eski thighin(WREG deki deger) 0.biti, tlowun 7.bitine gelmesi lazim
    bcf	    tlow,7
    incf    modCount
    return
    
complementor
    comf    tlow
    comf    thigh
    clrf    modCount
    return
    
astroid_table
    movwf   astronom
    sublw   0x00
    btfsc   STATUS,2
    bsf	    ast0,0
    dcfsnz  astronom 
    bsf	    ast1,0
    dcfsnz  astronom 
    bsf	    ast2,0
    dcfsnz  astronom 
    bsf	    ast3,0
    dcfsnz  astronom 
    bsf	    ast4,0
    dcfsnz  astronom 
    bsf	    ast5,0
    return
    

    
first:
    movlw   0xF1
    cpfseq  counterl
    retfie
    movlw   0x01
    cpfseq  counterh
    retfie
    clrf    counterh
    clrf    counterl
    call    movement
    retfie

second:
    movlw   0x8E
    cpfseq  counterl
    retfie
    movlw   0x01
    cpfseq  counterh
    retfie
    clrf    counterh
    clrf    counterl
    call    movement
    retfie
    
third:
    movlw   0x2A
    cpfseq  counterl
    retfie
    movlw   0x01
    cpfseq  counterh
    retfie
    clrf    counterh
    clrf    counterl
    call    movement
    retfie
    
fourth:
    movlw   0xC7
    cpfseq  counterl
    retfie
    movlw   0x00
    cpfseq  counterh
    retfie
    movlw   0x3F
    movwf   astCounter
    clrf    counterh
    clrf    counterl
    call    movement
    retfie
    
astroid_handler_jump
    call    astroid_handler
    return
    
movement:
    rlncf   ast0
    bcf	    ast0,6
    rlncf   ast1
    bcf	    ast1,6
    rlncf   ast2
    bcf	    ast2,6
    rlncf   ast3
    bcf	    ast3,6
    rlncf   ast4
    bcf	    ast4,6
    rlncf   ast5
    bcf	    ast5,6
    
    
    rrncf   fire0
    bcf	    fire0,7
    rrncf   fire1
    bcf	    fire1,7
    rrncf   fire2
    bcf	    fire2,7
    rrncf   fire3
    bcf	    fire3,7
    rrncf   fire4
    bcf	    fire4,7
    rrncf   fire5
    bcf	    fire5,7
    
    
    movf    ast0,0
    movff   fire0,fir0
    rlncf   fir0
    xorwf   fir0,0
    movwf   res0
    movf    ast0,0
    addwf   fir0,0
    cpfseq  res0
    movff   fir0,fire0
    cpfseq  res0
    call    for0
    
    movf    ast0,0
    xorwf   fire0,0
    movwf   res0
    movf    ast0,0
    addwf   fire0,0
    cpfseq  res0
    call    for0
    
    movf    ast1,0
    movff   fire1,fir1
    rlncf   fir1
    xorwf   fir1,0
    movwf   res1
    movf    ast1,0
    addwf   fir1,0
    cpfseq  res1
    movff   fir1,fire1
    cpfseq  res1
    call    for1
    
    movf    ast1,0
    xorwf   fire1,0
    movwf   res1
    movf    ast1,0
    addwf   fire1,0
    cpfseq  res1
    call    for1
    
    movf    ast2,0
    movff   fire2,fir2
    rlncf   fir2
    xorwf   fir2,0
    movwf   res2
    movf    ast2,0
    addwf   fir2,0
    cpfseq  res2
    movff   fir2,fire2
    cpfseq  res2
    call    for2
    
    movf    ast2,0
    xorwf   fire2,0
    movwf   res2
    movf    ast2,0
    addwf   fire2,0
    cpfseq  res2
    call    for2
    
    movf    ast3,0
    movff   fire3,fir3
    rlncf   fir3
    xorwf   fir3,0
    movwf   res3
    movf    ast3,0
    addwf   fir3,0
    cpfseq  res3
    movff   fir3,fire3
    cpfseq  res3
    call    for3
    
    movf    ast3,0
    xorwf   fire3,0
    movwf   res3
    movf    ast3,0
    addwf   fire3,0
    cpfseq  res3
    call    for3
    
    movf    ast4,0
    movff   fire4,fir4
    rlncf   fir4
    xorwf   fir4,0
    movwf   res4
    movf    ast4,0
    addwf   fir4,0
    cpfseq  res4
    movff   fir4,fire4
    cpfseq  res4
    call    for4
    
    movf    ast4,0
    xorwf   fire4,0
    movwf   res4
    movf    ast4,0
    addwf   fire4,0
    cpfseq  res4
    call    for4
    
    movf    ast5,0
    movff   fire5,fir5
    rlncf   fir5
    xorwf   fir5,0
    movwf   res5
    movf    ast5,0
    addwf   fir5,0
    cpfseq  res5
    movff   fir5,fire5
    cpfseq  res5
    call    for5
    
    movf    ast5,0
    xorwf   fire5,0
    movwf   res5
    movf    ast5,0
    addwf   fire5,0
    cpfseq  res5
    call    for5
    
    call    over_table
    btfsc   game_over,0
    goto    restore_registers
    
    clrf    LATF
    clrf    LATE
    clrf    LATD
    clrf    LATC
    clrf    LATB
    clrf    LATA
    
    movff   state,ship
    movf    ship,0
    sublw   0x00
    btfsc   STATUS,2
    bsf	    LATA,0
    dcfsnz  ship 
    bsf	    LATA,1
    dcfsnz  ship 
    bsf	    LATA,2
    dcfsnz  ship 
    bsf	    LATA,3
    dcfsnz  ship 
    bsf	    LATA,4
    dcfsnz  ship 
    bsf	    LATA,5	    
    
    btfsc   res0,0
    bsf	    LATF,0
    btfsc   res1,0
    bsf	    LATF,1
    btfsc   res2,0
    bsf	    LATF,2
    btfsc   res3,0
    bsf	    LATF,3
    btfsc   res4,0
    bsf	    LATF,4
    btfsc   res5,0
    bsf	    LATF,5
    
    btfsc   res0,1
    bsf	    LATE,0
    btfsc   res1,1
    bsf	    LATE,1
    btfsc   res2,1
    bsf	    LATE,2
    btfsc   res3,1
    bsf	    LATE,3
    btfsc   res4,1
    bsf	    LATE,4
    btfsc   res5,1
    bsf	    LATE,5
    
    btfsc   res0,2
    bsf	    LATD,0
    btfsc   res1,2
    bsf	    LATD,1
    btfsc   res2,2
    bsf	    LATD,2
    btfsc   res3,2
    bsf	    LATD,3
    btfsc   res4,2
    bsf	    LATD,4
    btfsc   res5,2
    bsf	    LATD,5
    
    btfsc   res0,3
    bsf	    LATC,0
    btfsc   res1,3
    bsf	    LATC,1
    btfsc   res2,3
    bsf	    LATC,2
    btfsc   res3,3
    bsf	    LATC,3
    btfsc   res4,3
    bsf	    LATC,4
    btfsc   res5,3
    bsf	    LATC,5
    
    
    btfsc   res0,4
    bsf	    LATB,0
    btfsc   res1,4
    bsf	    LATB,1
    btfsc   res2,4
    bsf	    LATB,2
    btfsc   res3,4
    bsf	    LATB,3
    btfsc   res4,4
    bsf	    LATB,4
    btfsc   res5,4
    bsf	    LATB,5
    
    btfsc   res0,5
    bsf	    LATA,0
    btfsc   res1,5
    bsf	    LATA,1
    btfsc   res2,5
    bsf	    LATA,2
    btfsc   res3,5
    bsf	    LATA,3
    btfsc   res4,5
    bsf	    LATA,4
    btfsc   res5,5
    bsf	    LATA,5
    call    astroid_handler_jump
    goto    restore_registers

for0
    call    score_table
    movff   fire0,j
    movf    fire0,0
    iorwf   res0,0
    xorwf   ast0,0
    movwf   fire0,0
    movf    ast0,0
    iorwf   res0,0
    xorwf   j,0
    movwf   ast0,0
    return
    
for1
    call    score_table
    movff   fire1,j
    movf    fire1,0
    iorwf   res1,0
    xorwf   ast1,0
    movwf   fire1,0
    movf    ast1,0
    iorwf   res1,0
    xorwf   j,0
    movwf   ast1,0
    return
    
for2
    call    score_table
    movff   fire2,j
    movf    fire2,0
    iorwf   res2,0
    xorwf   ast2,0
    movwf   fire2,0
    movf    ast2,0
    iorwf   res2,0
    xorwf   j,0
    movwf   ast2,0
    return
    
for3
    call    score_table
    movff   fire3,j
    movf    fire3,0
    iorwf   res3,0
    xorwf   ast3,0
    movwf   fire3,0
    movf    ast3,0
    iorwf   res3,0
    xorwf   j,0
    movwf   ast3,0
    return
    
for4
    call    score_table
    movff   fire4,j
    movf    fire4,0
    iorwf   res4,0
    xorwf   ast4,0
    movwf   fire4,0
    movf    ast4,0
    iorwf   res4,0
    xorwf   j,0
    movwf   ast4,0
    return

for5
    call    score_table
    movff   fire5,j
    movf    fire5,0
    iorwf   res5,0
    xorwf   ast5,0
    movwf   fire5,0
    movf    ast5,0
    iorwf   res5,0
    xorwf   j,0
    movwf   ast5,0
    return
    
score_table
    incf    score1
    movlw   0x0A
    cpfseq  score1
    return
    clrf    score1
    incf    score2
    movlw   0x0A
    cpfseq  score2
    return
    clrf    score2
    incf    score3
    return

over_table
    movff   state,i
    movf    i,0
    sublw   0x00
    btfsc   STATUS,2
    call    foro0   
    dcfsnz  i 
    call    foro1
    dcfsnz  i 
    call    foro2
    dcfsnz  i 
    call    foro3
    dcfsnz  i 
    call    foro4
    dcfsnz  i 
    call    foro5
    return

foro0
    btfsc   ast0,5
    bsf	    game_over,0
    return
foro1
    btfsc   ast1,5
    bsf	    game_over,0
    return
foro2
    btfsc   ast2,5
    bsf	    game_over,0
    return
foro3
    btfsc   ast3,5
    bsf	    game_over,0
    return
foro4
    btfsc   ast4,5
    bsf	    game_over,0
    return
foro5
    btfsc   ast5,5
    bsf	    game_over,0
    return

    
save_registers
    movwf 	w_temp          ;Copy W to TEMP register
    swapf 	STATUS, 0       ;Swap status to be saved into W
    clrf 	STATUS          ;bank 0, regardless of current bank, Clears IRP,RP1,RP0
    movwf 	status_temp     ;Save status to bank zero STATUS_TEMP register
    movf 	PCLATH, 0       ;Only required if using pages 1, 2 and/or 3
    movwf 	pclath_temp     ;Save PCLATH into W
    clrf 	PCLATH          ;Page zero, regardless of current page
    return

restore_registers
    movf 	pclath_temp, 0  ;Restore PCLATH
    movwf 	PCLATH          ;Move W into PCLATH
    swapf 	status_temp, 0  ;Swap STATUS_TEMP register into W
    movwf 	STATUS          ;Move W into STATUS register
    swapf 	w_temp	        ;Swap W_TEMP
    swapf 	w_temp, 0       ;Swap W_TEMP into W
    retfie
    
end

;========================================================================
;
; File Name     : rfft_brc.asm
; 
; Originator    : Advanced Embeeded Control 
;                 Texas Instruments 
; 
; Description   : This file contains source code to suffle the 2N-point real sequence
;                 as N-point complex sequence in bit reversed order
;
; Date          : 26/2/2002
;======================================================================
; 
; Routine Type  : C Callable
; 
; Usage :
; void rfft_brc(int *src, int *dst, ushort n)
; This function shuffles 2N-point in-order array and stores it as N-point complex
; array in bit-reversed order. This is helpful for Real FFT computation on the 2N-point
; real sequence using N-point complex FFT module
;=====================================================================
; SOURCE ARRAY
;         |  X(0)   |
;         |  X(1)   |
;         |  X(2)   |
;         |  X(3)   |
;            :
;            :
;         | X(2N-1) |
;=====================================================================


               .def _RFFT32_brev
 
_RFFT32_brev:  
                
            MOVL    XAR0,XAR4       ; XAR0->src
            NOP     *,ARP5
            
            MOV     AH,AL           ; AH=size
            LSR     AH,#1           ; AH=size/2
            MOVZ    AR7,AH          ; ZAR7=size/2
            SUBB    XAR7,#1         ; XAR7=(size/2)-1

            CMPR    0               ; TC=1, if src==dst
            BF      off_place,NTC   ; If not equal, jump to off place bit rev

            
next_data1:    
            MOVL    XT,*XAR4        ; XT=*src
            MOVL    P,*XAR5         ; P=*dst
            MOVL    XAR0,XAR4       ; XAR0=src
            CMPR    2               ; check src<dst
            BF      noswap,TC       ; if src<dst, do not swap *src & *dst
            
            MOVL    *XAR4,P         ; *src=*dst
            MOVL    *XAR5,XT        ; *dst=*src 
            MOVL    XT,*+XAR4[2]    ; XT=*(src+1)
            MOVL    P,*+XAR5[2]     ; P=*(dst+1)
            MOVL    *+XAR4[2],P     ; *(src+1)=*(dst+1)
            MOVL    *+XAR5[2],XT    ; *(dst+1)=*(src+1) 
           
noswap:     
            ADDB    XAR4,#4         ; XAR4=src+2
            MOVZ    AR0,AL          ; AR0=2*size
            NOP     *BR0++          ; ; dst=br(dst+2*size)
            BANZ    next_data1,AR7-- 
            LRETR

off_place:   
            MOVZ    AR0,AL          ; AR0=2*size
            
next_data2:
            MOVL    XT,*XAR4++      ; XT=*src
            MOVL    P,*XAR4++       ; P=*(src+1)
            MOVL    *XAR5,XT        ; *dst=*src
            MOVL    *+XAR5[2],P     ; *(dst+1)=*(src+1)
            NOP     *BR0++   
            BANZ    next_data2,AR7--               
            LRETR

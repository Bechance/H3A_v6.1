;============================================================================
;
; File Name     : rfft_winc.asm
; 
; Originator    : Advanced Embeeded Control 
;                 Texas Instruments 
; 
; Description   : This file contains source code to window the input data sequence in the case of 
;                 Real FFT modules
;               
; Date          : 26/4/2001 (dd/mm/yyyy)
;===========================================================================    
; Routine Type  : C Callable        
;
; Description   :
; void FFTR_win(FFTxxxR_handle) 
; This function windows the 2N point real valued data samples stored as N point complex 
; sequence in bit reversed order.( It is used with real FFT modules)
; 
;====================================================================== 
; REAL FFT MODULES
;----------------------------------------------------------------------
;typedef struct {   
;       long *ipcbptr;
;       long *tfptr;               
;       int size;
;       int nrstage;             
;       long *magptr;
;       long *winptr; 
;       long peakmag;
;       int peakfrq;
;       int ratio;     
;       void (*init)(void);
;       void (*calc)(void *);
;       void (*split)(void *);
;       void (*mag)(void *);
;       void (*win)(void *);
;       }FFTxxxR;
;======================================================================

                .def    _RFFT32_win       
                
_RFFT32_win:
         
            MOVL    XAR5,*XAR4++    ; XAR5=ipcbptr
            MOVL    XAR6,*+XAR4[6]  ; XAR6=winptr
            MOV     ACC,*+XAR4[2]<<15 ; AH=size/2
            MOVH    AR0,ACC<<2      ; AR0=2*size
            MOVZ    AR7,AH          ; AR7=(size/4)                                  
            SUBB    XAR7,#1         ; AR7=(size/4)-1
            MOVL    XAR4,XAR7       ; AR4=(size/4)-1
            
nextsamp1:  
            MOVL    XT,*XAR6++      
            QMPYL   ACC,XT,*XAR5
            LSL     ACC,#1
            MOVL    *XAR5,ACC       ; *(ipcbptr)=(*ipcbptr)*(*winptr)

            MOVL    XT,*XAR6++      
            QMPYL   ACC,XT,*+XAR5[2]
            LSL     ACC,#1
            MOVL    *+XAR5[2],ACC   ; *(ipcbptr+1)=(*(ipcbptr+1)) X (*(winptr+1))
            NOP    *BR0++       
            BANZ    nextsamp1,AR7--
            
nextsamp2:  
            MOVL    XT,*--XAR6      
            QMPYL   ACC,XT,*XAR5
            LSL     ACC,#1
            MOVL    *XAR5,ACC       ; *(ipcbptr)=(*ipcbptr)*(*winptr)

            MOVL    XT,*--XAR6      
            QMPYL   ACC,XT,*+XAR5[2]
            LSL     ACC,#1
            MOVL    *+XAR5[2],ACC   ; *(ipcbptr+1)=(*(ipcbptr+1)) X (*(winptr+1))
            NOP    *BR0++   
            BANZ    nextsamp2,AR4--
            LRETR


              

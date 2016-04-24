      .sect	"ramfuncs"
;*********************************************************************************************************
; 宏定义
;*********************************************************************************************************
.POPALL .macro
        POP     DP:ST1
        NASP
        POP     XT
     	MOV32   R6H,*--SP
     	MOV32   R5H,*--SP
     	MOV32   R4H,*--SP
     	MOV32   R3H,*--SP
     	MOV32   R2H,*--SP
     	MOV32   R1H,*--SP
     	MOV32   R0H,*--SP
     	MOV32   STF,*--SP
     	
        POP     XAR7
        POP     XAR6
        POP     XAR5
        POP     XAR4
        POP     XAR3
        POP     XAR2   
        POP     AR1H:AR0H 
        POP     RPC
        .endm

.PUSHALL .macro
        PUSH    RPC
        PUSH    AR1H:AR0H   
        PUSH    XAR2
        PUSH    XAR3
        PUSH    XAR4
        PUSH    XAR5
        PUSH    XAR6
        PUSH    XAR7
        MOV32   *SP++,STF
        MOV32   *SP++,R0H
        MOV32   *SP++,R1H
        MOV32   *SP++,R2H
        MOV32   *SP++,R3H
        MOV32   *SP++,R4H
        MOV32   *SP++,R5H       
        MOV32   *SP++,R6H  
        
      	PUSH    XT
        ASP
        PUSH    DP:ST1
        SETFLG  RNDF32 =1,RNDF64=1
        .endm

;*********************************************************************************************************
; 引用的外部函数及变量声明
;*********************************************************************************************************
        .ref _OSTCBHighRdy
        .ref _OSInitHookBegin
        .ref _OSIntEnter
        .ref _OSIntExit
        .ref _OSTimeTick

        .ref _OSTCBCur
        .ref _OSPrioHighRdy
        .ref _OSPrioCur
        .ref _OSRunning
        .ref _OSIntNesting



;*********************************************************************************************************
; OSTickISR
; 系统时钟嘀嗒函数
;*********************************************************************************************************
        .global _OSTickISR

_OSTickISR:
    	.ref _PieCtrlRegs
        .PUSHALL
        CLRC    PAGE0,OVM
        CLRC    AMODE


		EALLOW
    	MOVW      DP,#_PieCtrlRegs+1
    	MOV       @_PieCtrlRegs+1,#1


		EDIS

        MOVW    DP,#_OSIntNesting
        INC     @_OSIntNesting
        
      	LCR _OSTimeTick    
    	LCR _OSIntExit
        .POPALL
        IRET        

;*********************************************************************************************************
; OSIntCtxSw
; 中断级任务切换函数
;*********************************************************************************************************
        .global _OSIntCtxSw
_OSIntCtxSw:

	.ref _OSTCBHighRdy
	.ref _OSTCBCur
	.ref _OSPrioHighRdy
	.ref _OSPrioCur	


     SUBB     SP,#4	 
     MOVW     DP,#_OSTCBCur	 	;  OSTCBCur->OSTCBStkPtr = SP
     MOVL     XAR4,@_OSTCBCur  	;  |274| 
     MOV      AL,SP     
     MOV      *+XAR4[0],AL     	;  |274| 


;=================================================================================	
     MOVW DP,#_OSTCBHighRdy	 	;  OSTCBCur = OSTCBHighRdy
     MOVL ACC,@_OSTCBHighRdy    
     MOVW DP,#_OSTCBCur
     MOVL @_OSTCBCur,ACC        
;=================================================================================	
	
     MOVW DP,#_OSPrioHighRdy    ;  OSPrioCur = OSPrioHighRdy
     MOV  AL,@_OSPrioHighRdy    
     MOVW DP,#_OSPrioCur
     MOV  @_OSPrioCur,AL 
;=================================================================================    
     MOVW    DP,#_OSTCBHighRdy	 ;  SP = OSTCBhighRdy->OSTCBStkPtr	
     MOVL    XAR4,@_OSTCBHighRdy   
     MOV     AL,*+XAR4[0]                    
     MOV     @SP,AL
     
        .POPALL
     IRET
;*********************************************************************************************************
; OSStartHighRdy
; 启动第一个任务
;*********************************************************************************************************
        .global _OSStartHighRdy
_OSStartHighRdy:
	.ref   _OSTCBHighRdy
	
	    MOVW    DP,#_OSRunning
        MOV     @_OSRunning,#0x0001						                               	        	
	       
     	MOVW    DP,#_OSTCBHighRdy	 ;SP = OSTCBhighRdy->OSTCBStkPtr	
     	MOVL    XAR4,@_OSTCBHighRdy   
     	MOV     AL,*+XAR4[0]              
     	MOV     @SP,AL
       .POPALL
        IRET

;*********************************************************************************************************
; OSCtxSw
; 任务级任务切换函数
;*********************************************************************************************************
        .global _OSCtxSw
_OSCtxSw:
        .PUSHALL

;=================================================================================	     

     MOVW     	DP,#_OSTCBCur	 	; OSTCBCur->OSTCBStkPtr = SP
     MOVL     	XAR4,@_OSTCBCur   ;  |274| 
     MOV      	AL,SP     
     MOV      	*+XAR4[0],AL     	;  |274|  

;=================================================================================	
     MOVW 		DP,#_OSTCBHighRdy	 	; OSTCBCur = OSTCBHighRdy
     MOVL 		ACC,@_OSTCBHighRdy    
     MOVW 		DP,#_OSTCBCur
     MOVL 		@_OSTCBCur,ACC  
	
     MOVW 		DP,#_OSPrioHighRdy    ;OSPrioCur = OSPrioHighRdy
     MOV  		AL,@_OSPrioHighRdy    
     MOVW 		DP,#_OSPrioCur
     MOV  		@_OSPrioCur,AL        
;================================================================================= 
	
     MOVW    	DP,#_OSTCBHighRdy	 ;SP = OSTCBhighRdy->OSTCBStkPtr	
     MOVL    	XAR4,@_OSTCBHighRdy   
     MOV     	AL,*+XAR4[0]                    
     MOV     	@SP,AL
;=================================================================================     
     
     .POPALL
     IRET

.end
;*********************************************************************************************************
; No more
;*********************************************************************************************************

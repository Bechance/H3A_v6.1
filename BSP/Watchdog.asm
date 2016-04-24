      .sect	"ramfuncs"

     .global _WDog_disable

;¿´ÃÅ¹·¹Ø 	  
_WDog_disable:
    SETC OBJMODE        ;Set OBJMODE for 28x object code
    EALLOW              ;Enable EALLOW protected register access
    MOVZ DP, #7029h>>6  ;Set data page for WDCR register
    MOV @7029h, #0068h  ;Set WDDIS bit in WDCR to disable WD
    EDIS                ;Disable EALLOW protected register access
   
	lretr
 	   

 	 

/*         
         Contiguous SARAM memory blocks can be combined 
         if required to create a larger memory block. 
*/


MEMORY
{
PAGE 0 :
   /* BEGIN is used for the "boot to SARAM" bootloader mode     */
   /* BOOT_RSVD is used by the boot ROM for stack.              */
   /* This section is only reserved to keep the BOOT ROM from   */
   /* corrupting this area during the debug process             */
   BEGIN      : origin = 0x120400, length = 0x000002     /* Boot to M0 will go here                      */
   BOOT_RSVD  : origin = 0x000002, length = 0x00004E     /* Part of M0, BOOT rom will use this for stack */               
   RAMM0      : origin = 0x000050, length = 0x0003B0     /**/
   RAML	      : origin = 0x008000, length = 0x002000     /*芯片内存区*/
   program	  : origin = 0x120402, length = 0x1F000      /*程序空间 仿真时放在外部内存后128K*/
 
 
   mod_list_head  : origin = 0x120000, length = 0x10
   mod_list       : origin = 0x120010, length = 0x190
   DR_list_head   : origin = 0x120200, length = 0x10
   DR_list        : origin = 0x120210, length = 0x190

   CSM_RSVD   : origin = 0x33FF80, length = 0x000076     /* Part of FLASHA.  Program with all 0x0000 when CSM is in use. */
   CSM_PWL    : origin = 0x33FFF8, length = 0x000008     /* Part of FLASHA.  CSM password locations in FLASHA            */
   ADC_CAL    : origin = 0x380080, length = 0x000009
   RESET      : origin = 0x3FFFC0, length = 0x000002
   IQTABLES   : origin = 0x3FE000, length = 0x000b50
   IQTABLES2  : origin = 0x3FEB50, length = 0x00008c
   FPUTABLES  : origin = 0x3FEBDC, length = 0x0006A0
   BOOTROM    : origin = 0x3FF27C, length = 0x000D44               
         
PAGE 1 : 
   RAMM       : origin = 0x000400,length = 0x000400     /* on-chip RAM block M1 */
   SRAM	      : origin = 0x100000,length = 0x20000      /*外部ram 384k*/
   MY_FAST_RAM: origin = 0x00A000,length = 0x3000       /*高速变量区域*/
   FFT_RAM    : origin = 0x00D000,length = 0x00C44      /*FFT*/
   MY_STACK	  : origin = 0x00e000,length = 0x2000       /*系统堆栈区域*/
   ZONE7B     : origin = 0x20FC00,length = 0x000400     /* XINTF zone 7 - data space */
}
 
 
SECTIONS
{
   /* Setup for "boot to SARAM" mode: 
      The codestart section (found in DSP28_CodeStartBranch.asm)
      re-directs execution to the start of user code.  */
   codestart        : > BEGIN,     PAGE = 0
   ramfuncs         : LOAD = program, 
                     RUN = RAML, 
                     LOAD_START(_RamfuncsLoadStart),
                     LOAD_END(_RamfuncsLoadEnd),
                     RUN_START(_RamfuncsRunStart),
                     PAGE = 0
   .text            : > program,      PAGE = 0
   .cinit           : > RAML,      PAGE = 0
   .pinit           : > RAML,      PAGE = 0
   .switch          : > RAML,      PAGE = 0
   
   .stack           : > RAMM,      PAGE = 1
   .bss             : >MY_FAST_RAM      PAGE =1
   .ebss            : > SRAM,      PAGE = 1
   .econst          : > SRAM,      PAGE = 1      
   .esysmem         : > RAMM,      PAGE = 1

   IQmath           : > RAML,      PAGE = 0
   IQmathTables     : > IQTABLES,  PAGE = 0, TYPE = NOLOAD 
   IQmathTables2    : > IQTABLES2, PAGE = 0, TYPE = NOLOAD 
   FPUmathTables    : > FPUTABLES, PAGE = 0, TYPE = NOLOAD 

   ZONE7DATA        : > ZONE7B,    PAGE = 1  

   .reset           : > RESET,     PAGE = 0, TYPE = DSECT /* not used                    */
   csm_rsvd         : > CSM_RSVD   PAGE = 0, TYPE = DSECT /* not used for SARAM examples */
   csmpasswds       : > CSM_PWL    PAGE = 0, TYPE = DSECT /* not used for SARAM examples */
   
   /* Allocate ADC_cal function (pre-programmed by factory into TI reserved memory) */
   .adc_cal     :  load = ADC_CAL,   	PAGE = 0, TYPE = NOLOAD
    MY_MEM  	:  {} > MY_STACK  		PAGE 1     
    FAST_RAM  	:  {} > MY_FAST_RAM  	PAGE 1  
    FAST_PROGRAM:  {} > RAML  			PAGE 0 
    FFTipcb             :>FFT_RAM PAGE=1 
    FFTmag              :>FFT_RAM  PAGE=1 
    MOD_LIST_ADDR	    :>mod_list           PAGE =0
    MOD_LIST_HEAD	    :>mod_list_head      PAGE =0
    DR_LIST_ADDR	    :>DR_list            PAGE =0
    DR_LIST_HEAD	    :>DR_list_head       PAGE =0
}

/*
//===========================================================================
// End of file.
//===========================================================================
*/
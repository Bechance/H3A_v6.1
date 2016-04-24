
#define	 SetDACS1	0x01
#define  ClrDACS1	0xFE
#define	 SetDACS2	0x02
#define	 ClrDACS2	0xFD
#define	 SetDAOUT	0x04
#define	 ClrDAOUT	0xFB
#define	 SetDACLK	0x08
#define  ClrDACLK	0xF7

#define   OutReg	(*((volatile  Uint16 *)0x40FF))
#define	  LedReg	(*((volatile  Uint16 *)0x41FF))
#define   SysReg    (*((volatile  Uint16 *)0x42FF))
#define   SDReg		(*((volatile  Uint16 *)0x43FF))
#define   KeyReg    (*((volatile  Uint16 *)0x44FF))
#define   InlReg	(*((volatile  Uint16 *)0x45FF))
#define   UsbIntReg (*((volatile  Uint16 *)0x46FF))
#define   IntReg    (*((volatile  Uint16 *)0x47FF))





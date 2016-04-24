///////////////////////////////////
//#include "RBCCMD.H"
///////////////////////////////////
Uint16 EnumMassDev( void );
Uint16 SPC_Inquiry( void );
Uint16 SPC_RequestSense( void );
Uint16 SPC_TestUnit( void );
Uint16 SPC_LockMedia( void );
Uint16 RBC_ReadCapacity( void );
Uint16 RBC_Read( Uint32 lba, Uint16 len, Uint16 *pBuffer );
Uint16 RBC_Write( Uint32 lba, Uint16 len, Uint16 *pBuffer );
void play_Udisc();

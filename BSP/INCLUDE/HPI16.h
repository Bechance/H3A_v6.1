/////////////////hpi.h////////////////////////////////
Uint16 List16( void );
Uint16 OpenFile16( char *pBuffer );
Uint16 ReadFile16( Uint32 readLength, Uint16 *pBuffer );
Uint16 SetFilePointer16( Uint32 pointer );

Uint16 CreateFile16( char *str, Uint16 attr );
Uint16 WriteFile16( Uint32 writeLength, Uint16 *pBuffer );
Uint16 RemoveFile16( char *str );
Uint32 GetCapacity16( void );

Uint16 CreateDir16( char *str );
Uint16 DownDir16( char *pBuffer );
Uint16 UpDir16( void );
Uint16 UpRootDir16( void );


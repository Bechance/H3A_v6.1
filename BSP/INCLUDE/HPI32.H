/////////////////hpi32.h////////////////////////////////
#define MaxLFNum 20

typedef struct ShowFileName_D
{
	Uint16 LongName[256];	//long file name,end by 00 00
	Uint16 item[32];		//short file name & dir
} ShowFileName_Def;

Uint16 List32( void );
Uint16 OpenFile32( char *str );


Uint16 ReadFile32( Uint32 readLength, Uint16 *pBuffer );
Uint16 SetFilePointer32( Uint32 pointer );
Uint16 CreateFile32( char *str, Uint16 attr );
Uint16 WriteFile32( Uint32 writeLength, Uint16 *pBuffer );
Uint16 RemoveFile32( char *str );
Uint32 GetCapacity32( void );

Uint16 CreateDir32( char *str );
Uint16 DownDir32( char *pBuffer );
Uint16 UpDir32( void );
Uint16 UpRootDir32( void );
Uint16 ListDirve( void );

//====================no more==================================

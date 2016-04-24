Uint16  List( void );
Uint16	OpenFile( char *str );
Uint16  ReadFile( Uint32 readLength, Uint16 *pBuffer );
Uint16 	SetFilePointer( Uint32 pointer );
Uint16  CreateFile( char *str, Uint16 attr );
Uint16  WriteFile( Uint32 writeLength, Uint16 *pBuffer );
Uint16  RemoveFile( char *str );
Uint32 	GetCapacity();
Uint16  DetectDisk();
Uint16  CreateDir( char *str );
Uint16  DownDir( char *str );
Uint16  UpDir();
Uint16  UpRootDir();



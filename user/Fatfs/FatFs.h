#ifndef  _PEC_FATFS_INCLUDE
#define  _PEC_FATFS_INCLUDE

#include "src\diskio.h"
#include "src\ff.h"
#include "src\integer.h"

FRESULT 		fatfs_write_bytes( FIL *, const void *, UINT , UINT * );
FRESULT 		fatfs_read_bytes( FIL *, const void *, UINT , UINT * );
uint8_t 		fatfs_FIobj_put( FIL* );
FIL*  			fatfs_FIobj_obtain( void );
FIL*			fatfs_open( const TCHAR *, BYTE );
void 			fatfs_close( FIL* fp );
BOOL 			fatfs_file_ISexist( const char*path );
BOOL 			fatfs_file_create( const char*path, void*data, int size );
BOOL 			fatfs_readARG( const char*path, void*data, int size );
BOOL 			fatfs_writeARG( const char*path, void*data, int size );
void 			fatfs_init( void );
extern void GUI_bmp_display( int x0, int y0, void*pDATA );
void Fileview_sendMSG( uint8_t key );
void Fileview_path_obtain( char*path );
char Fileview_type_obtain( void );
void Fileview_open( char*path, int posRST );
void Fileview_setARG(int lsize,int hsize,const void*pf);
void Fileview_close( void );
BOOL Fileview_ISexit( void );

void File_txtView(char*path,char*title);

#endif

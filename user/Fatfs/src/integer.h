/*-------------------------------------------*/
/* Integer type definitions for FatFs module */
/*-------------------------------------------*/

#ifndef _INTEGER
#define _INTEGER

#ifdef _WIN32	/* FatFs development platform */

#include <windows.h>
#include <tchar.h>

#else			/* Embedded platform */

/* These types must be 16-bit, 32-bit or larger integer */
typedef int				INT;
typedef unsigned int		UINT;

/* These types must be 8-bit integer */
typedef char			CHAR;
typedef unsigned char	UCHAR;
typedef unsigned char	BYTE;

/* These types must be 16-bit integer */
typedef short			SHORT;
typedef unsigned short	USHORT;
typedef unsigned short	WORD;
typedef unsigned short	WCHAR;

/* These types must be 32-bit integer */
typedef long			LONG;
typedef unsigned long	ULONG;
typedef unsigned long	DWORD;

#endif
/* Boolean type */
#ifndef     FALSE
#define    FALSE     0
#endif

#ifndef     TRUE
#define    TRUE     1
#endif



#ifndef        TYPE_EXPEND
#define         TYPE_EXPEND
#define  		room_t      		const
typedef  	unsigned short    		Bsize_t;
typedef 		unsigned char    		BOOL;
typedef 		unsigned int			pint;      /*指针数据整形长度*/
enum
{
	FAIL = 0,
	TURE = 1,
	IDLE = 0,
	BUSY = 1,
	PNULL = 0,
	ASCII_NULL = '\0'
};
#endif
#endif

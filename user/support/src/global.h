#ifndef   GLOBAL_H_2010
#define   GLOBAL_H_2010
#include 	 	<stdlib.h>
#include 	  	<stdio.h>
#include 	  	<string.h>
#include 		"../app_support.h"
#define 		MY_STRING_SUPPORT_EN  1

typedef unsigned char( *debounce_conditon_function_t )( unsigned char *shadow_buf , unsigned char *ture_buf );

typedef struct
{
	uint8_t shadow_buf[4];
	uint8_t shadow_buf_size;
	uint8_t debounce_times_level;
	uint8_t current_debounce_times;
	debounce_conditon_function_t debounce_conditon;
}debounce_t;

enum
{
	IS_CHAR = 0,
	IS_UNSIGN_CHAR,
	IS_INT,
	IS_UNSIGN_INT,
	IS_LONG,
	IS_UNSIGN_LONG,
	IS_FLOAT
};
void 			global_memcpy( void * pur_add, void*src_ptr, Bsize_t lenth );

void 			global_memset( void * pur_add, uint8_t pad_data, Bsize_t lenth );

BOOL      		global_memcmp( void * pur_add, void*src_ptr, Bsize_t lenth );

Bsize_t 		global_strlen( char*psrc );

char* 			global_strcpy( char*pdest, char*psrc );

void 			global_strcat( char*pstr1, char*pstr2 );

char* 			global_string_locate_first( char*psrc, char*psearch );

char* 			global_string_locate_end( char*phead, char*pend, char*psearch );

Bsize_t 		global_string_locate( char*phead, char*pend, char*psearch, uint32_t*paddr );

void 			global_string_reversal( char *psrc );

Bsize_t 		global_itoa( int isrc, char *pdes );

Bsize_t 		global_ltoa( long isrc, char *pdes );

long 			global_atoi( char *pdes );

float  			global_atof( char *pdes );

Bsize_t 		global_ftoa( float fsrc , char*pdes, Bsize_t point_bit );

void 			global_string_reversal( char *psrc );

void 			global_debounce_times_mod( debounce_t *debounce_ptr, unsigned char  times );

BOOL 			global_debounce_support( uint8_t *in_data, debounce_t *debounce_ptr );

Bsize_t  		global_sum_array( void*Phead, Bsize_t size, Bsize_t data_type );

void 			global_udelay( Bsize_t us );

char 			global_NtoC( uint8_t num );

uint8_t 		global_CtoN( char num );

BOOL  			global_decide_num( char asc );

Bsize_t			global_decide_letter( char asc );

uint8_t 		global_switch_letter_module( char asc );

uint8_t 		global_adjust_letter_module( char asc, uint8_t type );

void 			global_float_putoff_point( float fsrc, long *plong, float*ppoint );

float  			global_round_off_float( float src );

void  			global_string_filter( char*psrc, char ch );

#define                GLOBAL_MEMCLR(psrc,lenth) 	(global_memset((void*)(psrc),0,(lenth)))
#define                GLOBAL_STRSET(psrc,data) 	(global_memset((void*)(psrc),data,(global_strlen(psrc))))

#define                GLOBAL_PI                            3.1415926
#define                GLOBAL_SW_RADIAN(angle)   (( angle ) * GLOBAL_PI / 180 )
#define                GLOBAL_SW_ANGLE(radian)    ((radian)*180/GLOBAL_PI)

#define                IS_NO_NCHAR      	0XFF
#define                IS_BIG_LETTER      	 2
#define                IS_LITTLE_LETTER      1
#define                IS_NO_LETTER      	 0



/*以下是常用的宏定义，本人收集*/
typedef  unsigned char     byte;         /* Unsigned 8  bit value type. */
typedef  uint16_t 		    word;         /* Unsinged 16 bit value type. */
typedef  uint32_t 		    dword;         /* Unsinged 16 bit value type. */


#define   SIZE_TO_NBLK(size,block_size) (( size + block_size - 1 )/ block_size)
#define   GET_STRING_END(str) 		    ((char*)(str)+global_strlen(str)-1)
#define   GLOBAL_ABS(val)   	        (((val)>0)?((val)):(-(val)))
#define   MEM_B( x )  			        (*((byte *)(x)))	/*得到指定地址上的一个字节*/
#define   MEM_W( x )  			        (*((word *)(x)))	/*得到指定地址上的一个字*/

/*得到一个field在结构体(struct)中的偏移量*/
#define FPOS( type, field ) \
	/*lint -e545 */ ( (dword) &(( type *) 0)-> field ) /*lint +e545 */

/*得到一个结构体中field所占用的字节数*/
#define FSIZ( type, field ) sizeof( ((type *) 0)->field )


/*按照LSB格式把两个字节转化为一个Word*/
#define  FLIPW( ray ) ( (((word) (ray)[0]) * 256) + (ray)[1] )

/*按照LSB格式把一个Word转化为两个字节*/
#define  FLOPW( ray, val ) \
	(ray)[0] = ((val) / 256); \
	(ray)[1] = ((val) & 0xFF)

/*得到一个变量的地址（word宽度）*/
#define  B_PTR( var )  ( (byte *) (void *) &(var) )
#define  W_PTR( var )  ( (word *) (void *) &(var) )

/*得到一个字的高位和低位字节*/
#define  WORD_LO(val)  ((byte) ((word)(val) & 255))
#define  WORD_HI(val)  ((byte) ((word)(val) >> 8))

/*返回一个比X大的最接近的8的倍数*/
#define RND8( x )       ((((x) + 7) / 8 ) * 8 )

/*将一个字母转换为大写*/
#define  UPCASE( c ) ( ((c) >= 'a' && (c) <= 'z') ? ((c) - 0x20) : (c) )
/*将一个字母转换为小写*/
#define  DOWNCASE( c ) ( ((c) >= 'A' && (c) <= 'Z') ? ((c) + 0x20) : (c) )


/*判断字符是不是10进值的数字*/
#define  DECCHK( c ) ((c) >= '0' && (c) <= '9')

/*判断字符是不是字母*/
#define  LETTERCHK( c ) (((c) >= 'a' && (c) <= 'z')||((c) >= 'A' && (c) <= 'Z'))


/*判断字符是不是16进值的数字*/
#define  HEXCHK( c ) ( ((c) >= '0' && (c) <= '9') ||\
					   ((c) >= 'A' && (c) <= 'F') ||\
					   ((c) >= 'a' && (c) <= 'f') )

/*自加防止溢出的一个方法*/
#define  INC_SAT( val )  (val = ((val)+1 > (val)) ? (val)+1 : (val))

#define  INC_DECIDE(val,MAX)  (val=((val<MAX) ?( (val)+1 ):(val=val)))
#define  DEC_DECIDE(val)  	 (val=((val>0) ?( (val)-1):(val=val)))


#define  INC_DECIDE_LOOP(val,MAX)    (val=((val<MAX) ?( (val)+1 ):(val=0)))
#define  DEC_DECIDE_LOOP(val,MAX)  	 (val=((val>0) ?( (val)-1):(val=MAX)))



/*返回一个无符号数n尾的值MOD_BY_POWER_OF_TWO(X,n)=X%(2^n)*/
#define MOD_BY_POWER_OF_TWO( val, mod_by ) \
	( (dword)(val) & (dword)((mod_by)-1) )

/*对于IO空间映射在存储空间的结构，输入输出处理*/
#define INPUT_B(port)         		(*((volatile byte *) (port)))
#define INPUT_W(port)        		(*((volatile word *) (port)))
#define INPUT_DW(port)       		(*((volatile dword *)(port)))
#define OUTPUT_B(port, val)   	(*((volatile byte *) (port)) = ((byte) (val)))
#define OUTPUT_W(port, val)  	(*((volatile word *) (port)) = ((word) (val)))
#define OUTPUT_DW(port, val) 	(*((volatile dword *) (port)) = ((dword) (val)))





#ifndef GLOBAL_NTOC
#define  GLOBAL_NTOC(num) 	((num>9)?(IS_NO_NCHAR):( '0' + num ))
#endif
#ifndef GLOBAL_CTON
#define  GLOBAL_CTON(char) 	((( '0' <= char ) && ( char <= '9' ))?( char -'0' ):('0'))
#endif

#ifndef MIN
#	define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif // #ifndef MIN

#ifndef MAX
#	define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif // #ifndef MAX

#ifndef SIZE_OF_ARRAY
#	define SIZE_OF_ARRAY( array ) (sizeof(array)/sizeof(array[0]))
#endif

#ifndef PTR_OFFSET_BYTES
#	define PTR_OFFSET_BYTES( ptr, offset ) ( (char *)(ptr) + (offset) )
#endif

#ifndef PTR_OFFSET_BYTES_FORWARD
#	define PTR_OFFSET_BYTES_FORWARD PTR_OFFSET_BYTES
#endif

#ifndef PTR_OFFSET_BYTES_BACKWARD
#	define PTR_OFFSET_BYTES_BACKWARD( ptr, offset ) PTR_OFFSET_BYTES( ptr, - (offset) )
#endif

#ifndef PTR_OFFSET_BETWEEN
#	define PTR_OFFSET_BETWEEN( from, to ) (((char*)to) - ((char*)from))
#endif

#ifndef PTR_OFFSET_UNITS
#	define PTR_OFFSET_UNITS( ptr, offset ) ( (ptr) + (offset) )
#endif

#ifndef PTR_OFFSET_UNITS_FORWARD
#	define PTR_OFFSET_UNITS_FORWARD PTR_OFFSET_UNITS
#endif

#ifndef PTR_OFFSET_UNITS_BACKWARD
#	define PTR_OFFSET_UNITS_BACKWARD( ptr, offset ) PTR_OFFSET_UNITS( ptr, - (offset) )
#endif

#define IF_TRUE_DO( condition, jobs ) if( condition ) { jobs; }
#define IF_FALSE_DO( condition, jobs ) if( !(condition) ) { jobs; }
#define IF_TRUE_RETURN( condition, result ) IF_TRUE_DO( condition, return result )
#define IF_FALSE_RETURN( condition, result ) IF_FALSE_DO( condition, return result )
#define IF_TRUE_GOTO( condition, label ) IF_TRUE_DO( (condition), goto label )
#define IF_FALSE_GOTO( condition, label ) IF_FALSE_DO( !(condition), goto label )
#define IF_FAILED_GOTO IF_FALSE_GOTO
#define IF_SUCCESSED_GOTO IF_TRUE_GOTO
#define IF_AFTER_EXECUTED_CONDITION_TRUE_GOTO( label, code_section, condition ) { { code_section ;}; if( condition ) goto label; };
#endif

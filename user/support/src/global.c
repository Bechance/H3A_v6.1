/*********************************************************************************************
*            Öéº£¿ÆÜöµçÆ÷ÓÐÏÞ¹«Ë¾
*            http://www.zhkh.com
* Ä£¿éÃû³Æ:Ï°¹ßÍ¨ÓÃÐÔº¯ÊýÖ§³Ö
* ´´½¨Ê±¼ä:2010-06-23
* ËùÊô¹«Ë¾ :¿ÆÜö
* ÎÄ¼þÃû³Æ:global.c
* ´´½¨ÈË :½Ò³É
* ¹¦ÄÜÃèÊö:³£ÓÃº¯Êý¼¯ºÏ¡£
*----------------------------------------------------------------
ÐÞ¸Ä¼ÇÂ¼:

ÐÞ¸ÄÈÕÆÚ:

ÐÞ¸ÄÈË:

**********************************************************************************************************/
#include 	   "global.h"

#if  MY_STRING_SUPPORT_EN >0
/********************************************************************
 *@´´½¨ÈË:½Ò³É
 *@¹¦ÄÜ:ÄÚ´æ¸´ÖÆ
 *
 *@ÊäÈë:  pur_add  :Ö¸ÏòÄ¿±êµØÖ·
 * 			pad_data :Ö¸ÏòÔ´µØÖ·
 *      		lenth:Êý¾Ý³¤¶È
 *@Êä³ö:	NONE
********************************************************************/
void global_memcpy( void * pur_add, void*src_ptr, Bsize_t lenth )
{
	uint8_t index;
	char*p_pur = ( char* )pur_add;
	char*p_src = ( char* )src_ptr;
	for ( index = 0;index < lenth;index++ )
	{
		p_pur[index] = p_src[index];
	}
	return ;
}
/********************************************************************
 *@´´½¨ÈË:½Ò³É
 *@¹¦ÄÜ:ÄÚ´æÌî³ä
 *
 *@ÊäÈë: 	pur_add  :Ö¸Ïò´ýÌî³äÄÚ´æ¿ªÊ¼µØÖ·
 *       		pad_data :Ìî³äÊý¾Ý
 *      		lenth:Êý¾Ý³¤¶È
 *@Êä³ö:	NONE
********************************************************************/
void global_memset( void*pur_add, uint8_t pad_data, Bsize_t lenth )
{
	int index;
	uint8_t*p_pur = ( uint8_t* )pur_add;
	for ( index = 0;index < lenth;index++ )
	{
		p_pur[index] = pad_data;
	}
	return ;
}
/********************************************************************
 *@´´½¨ÈË:½Ò³É
 *@¹¦ÄÜ:ÄÚ´æ±È½Ï
 *
 *@ÊäÈë: 	pur_add  :Ö¸ÏòÄ¿µÄµØÖ·
 *       		src_ptr :Ö¸ÏòÔ´µØÖ·
 *      		lenth:Êý¾Ý³¤¶È
 *@Êä³ö:	NONE
********************************************************************/
BOOL  global_memcmp( void * pur_add, void*src_ptr, Bsize_t lenth )
{
	uint8_t index;
	char*p_pur = ( char* )pur_add;
	char*p_src = ( char* )src_ptr;
	for ( index = 0;index < lenth;index++ )
	{
		if ( p_pur[index] != p_src[index] )
		{
			return FAIL;
		}
	}
	return TURE;
}
/*********************************************************************
*@º¯ÊýÃû³Æ:²âÊÔ×Ö·û³¤¶È
*@´´½¨ÈË:½Ò³É
*@ÊäÈë: psrc  :Òª²âÊÔµÄ×Ö·û´®
*@ output :×Ö·û³¤¶È
*********************************************************************/
Bsize_t global_strlen( char*psrc )
{
	Bsize_t lenth = 0;
	while (( *psrc ) != ASCII_NULL )
	{
		lenth++;
		psrc++;
	}
	return lenth;
}
/*********************************************************************
*@º¯ÊýÃû³Æ:½«×Ö·û´®pstr2ºÏ²¢µ½pstr1
*@´´½¨ÈË:½Ò³É
*@ÊäÈë: pstr1  pstr2
*@ output :NONE
*±ØÐëÈ·±£pstr1 ÓÐ×ã¹»µÄ¿Õ¼ä
*********************************************************************/
void global_strcat( char*pstr1, char*pstr2 )
{
	char*pch;
	pch = ( char* )pstr1 + global_strlen( pstr1 );
	global_memcpy(( uint8_t* )pch,
				  ( uint8_t* )pstr2,
				  ( global_strlen( pstr2 ) + 1 ) );
	return;
}
/*********************************************************************
*   @º¯ÊýÃû³Æ:¸´ÖÆ×Ö·û´®
*   @´´½¨ÈË:½Ò³É
*   @ÊäÈë: psrc  :Òª¸´ÖÆ×Ö·û´®
*
*    @ output :×Ö·û³¤¶È
*
*********************************************************************/
char* global_strcpy( char*pdest, char*psrc )
{
	Bsize_t len = 0;
	char*pdest_shadow =  pdest ;
	while ( *psrc != ASCII_NULL )
	{
		*pdest++ = *psrc++;
		len++;
	}
	*pdest = ASCII_NULL;
	return ( pdest_shadow );
}
#else
void global_memcpy( void * pur_add, void*src_ptr, Bsize_t lenth )
{
	memcpy(( void* )pur_add, ( void* )src_ptr, lenth );
	return;
}
void global_memset( void*pur_add, uint8_t pad_data, Bsize_t lenth )
{
	memset(( void * ) pur_add, ( int )pad_data, ( Bsize_t )lenth );
	return;
}
BOOL global_memcmp( void * pur_add, void*src_ptr, Bsize_t lenth )
{
	int ret;
	ret = memcmp(( const void* )pur_add, ( const void* )src_ptr, ( Bsize_t )lenth );
	return( ret == 0 ) ? ( TURE ) : ( FAIL );
}
Bsize_t global_strlen( char*psrc )
{
	return ( Bsize_t )strlen(( const char* )psrc );
}

void global_strcat( char*pstr1, char*pstr2 )
{
	strcat(( char * )pstr1, ( const char * )pstr2 );
	return;
}
char* global_strcpy( char*pdest, char*psrc )
{
	return  strcpy( pdest, ( const char* )psrc );
}
#endif
/********************************************************************
 * @´´½¨ÈË :½Ò³É
 * @¹¦ÄÜ :ÔÚ×Ö·û´®ÖÐ¶¨Î»Ò»¸ö×Ö·û´®
 * @ÊäÈë :psrc ×Ö·û´®  pserch ´ý²éÑ¯µÄ×Ö·û´®
 *
 *@Êä³ö :²éÑ¯µ½×Ö·ûµÄÎ»ÖÃ
********************************************************************/
char* global_string_locate_first( char*psrc, char*psearch )
{
	Bsize_t offset;
	Bsize_t psearch_lenth = global_strlen( psearch );
	Bsize_t psrc_lenth = global_strlen( psrc );
	for ( offset = 0;offset < psrc_lenth;offset++ )
	{
		if ( global_memcmp( &psrc[offset],
							psearch,
							psearch_lenth ) )
		{
			return &( psrc[offset] );
		}
	}
	return PNULL;
}
/********************************************************************
 * @´´½¨ÈË :½Ò³É
 * @¹¦ÄÜ :ÔÚ×Ö·û´®ÖÐ¶¨Î»Ò»¸ö×Ö·û´®£¬
 *			·µ»Ø¶¨Î»µ½µÄ¸öÊý
 * @ÊäÈë :phead ×Ö·û´®Í·£¬
 *			pend  ×Ö·û´®Î²
 *			paddr ´æ·Å¶¨Î»µ½µÄ×Ö·ûµØÖ·
 *
 *@Êä³ö :²éÑ¯µ½×Ö·ûµÄÎ»ÖÃ
********************************************************************/
Bsize_t global_string_locate( char*phead, char*pend, char*psearch,uint32_t*paddr )
{
	char*pch;
	Bsize_t num = 0;
	while ( 1 )
	{
		pch = global_string_locate_first( phead, psearch );
		if (( pch < pend ) && ( pch != PNULL ) )
		{
			paddr[num] = ( pint )pch;
			phead = pch + 1;
			num++;
		}
		else
		{
			break;
		}
	}
	return num;
}
/********************************************************************
 * @´´½¨ÈË :½Ò³É
 * @¹¦ÄÜ :ÔÚ×Ö·û´®ÖÐ¶¨Î»Ò»¸ö×Ö·û´®£¬
 *			·µ»Ø×îºóÒ»¸ö¶¨Î»µ½µÄ
 * @ÊäÈë :phead ×Ö·û´®Í·£¬
 *			pend  ×Ö·û´®Î²
 *			pserch ´ý²éÑ¯µÄ×Ö·û´®
 *
 *@Êä³ö :²éÑ¯µ½×Ö·ûµÄÎ»ÖÃ
********************************************************************/
char* global_string_locate_end( char*phead, char*pend, char*psearch )
{
	char*pch;
	char*pret = PNULL;
	while ( 1 )
	{
		pch = global_string_locate_first( phead, psearch );
		if (( pch < pend ) && ( pch != PNULL ) )
		{
			( pret < pch ) ? ( pret = pch ) : ( pret = pret );
			phead = pch + 1;
		}
		else
		{
			break;
		}
	}
	return pret;
}
/*********************************************************************
*@º¯ÊýÃû³Æ:×Ö·û´®·´×ª
*@´´½¨ÈË:½Ò³É
*@ÊäÈë:
*@Êä³ö: NONE
*********************************************************************/
void global_string_reversal( char *psrc )
{
	char *pshadow_1 = psrc;
	char *pshadow_2;
	char temp;
	pshadow_2 = ( char* )psrc + ( global_strlen( psrc ) - 1 );
	while ( pshadow_1 < pshadow_2 )
	{
		if ( *pshadow_1 != *pshadow_2 )
		{
			temp = *pshadow_1;
			*pshadow_1 = *pshadow_2;
			*pshadow_2 = temp;
		}
		++pshadow_1;
		--pshadow_2;
	}
}
/*********************************************************************
*@º¯ÊýÃû³Æ:int ×ª»»³É×Ö·û´®
*@´´½¨ÈË:½Ò³É
*@ÊäÈë:
*@Êä³ö: NONE
*********************************************************************/
Bsize_t global_itoa( int isrc, char *pdes )
{
	int itemp = isrc;
	int offset = 0;
	if ( itemp < 0 )
	{
		isrc = -isrc;
	}
	do
	{
		pdes[offset++] = isrc % 10 + '0';
		isrc = isrc / 10;
	}
	while ( isrc > 0 );
	if ( itemp < 0 )
	{
		pdes[offset++] = '-';
	}
	pdes[offset] = ASCII_NULL;
	global_string_reversal( pdes );
	return ( global_strlen( pdes ) );
}
/*********************************************************************
*@º¯ÊýÃû³Æ:int ×ª»»³É×Ö·û´®
*@´´½¨ÈË:½Ò³É
*@ÊäÈë:
*@Êä³ö: NONE
*********************************************************************/
Bsize_t global_ltoa( long isrc, char *pdes )
{
	long  itemp = isrc;
	int offset = 0;
	if ( itemp < 0 )
	{
		isrc = isrc*(-1);
	}
	do
	{
		pdes[offset++] = isrc % 10 + '0';
		isrc = isrc / 10;
	}
	while ( isrc > 0 );
	if ( itemp < 0 )
	{
		pdes[offset++] = '-';
	}
	pdes[offset] = ASCII_NULL;
	global_string_reversal( pdes );
	return ( global_strlen( pdes ) );
}
/*********************************************************************
*@º¯ýÃû:float ×ª»»³É×Ö·û´®
*@´´½¨ÈË:½Ò³É
*@ÊäÈë:
*@ output : NONE
*FAIL :È¥¶¶¹ý³ÌÖÐ
*********************************************************************/
Bsize_t global_ftoa( float fsrc , char*pdes, Bsize_t point_bit )
{
	long ltemp, lmul;
	float fpoint;
	char point[15]="";
	global_float_putoff_point( fsrc, &ltemp, &fpoint );
	if ( ltemp == 0 && fsrc < 0 )
	{
		pdes[0] = '-';
		pdes[1] = '0';
		pdes[2] = ASCII_NULL;
	}
	else global_ltoa( ltemp, pdes );
	fpoint = GLOBAL_ABS( fpoint );
	if ( fpoint > 0 )
	{
		( point_bit > 6 ) ? ( point_bit = 6 ) : ( point_bit = point_bit );
		lmul = 1;
		while ( point_bit > 0 )
		{
			lmul = 10 * lmul;
			point_bit--;
		}
		ltemp = ( long )( fpoint * lmul );
		ltemp = GLOBAL_ABS( ltemp );
		global_ltoa( ltemp, point );
		while ( (*GET_STRING_END( point ) == '0')&&(global_strlen(point)>0))
		{
			*GET_STRING_END( point ) = ASCII_NULL;
		}
		global_strcat( pdes, "." );
		lmul /= 10;
		while ( ltemp < lmul )
		{
			global_strcat( pdes, "0" );
			lmul /= 10;
		}
		global_strcat( pdes, point );
		while ( *GET_STRING_END( pdes ) == '.' )
		{
			global_strcat( pdes, "0" );
		}
	}
	return global_strlen( pdes );
}
/*********************************************************************
*@º¯ÊýÃû³Æ:×Ö·û×ª»»³ÉINT
*@´´½¨ÈË:½Ò³É
*@ÊäÈë:
*@Êä³ö: NONE
*********************************************************************/
long global_atoi( char *pdes )
{
	return atoi( pdes );
}
/*********************************************************************
*@º¯ÊýÃû³Æ:×Ö·û×ª»»³ÉINT
*@´´½¨ÈË:½Ò³É
*@ÊäÈë:
*@Êä³ö: NONE
*********************************************************************/
float  global_atof( char *pdes )
{
	float  itemp = 0.0;
	Bsize_t offset;
	float   Mdata = 10;
	Bsize_t string_size = global_strlen( pdes );
	for ( offset = (( pdes[0] == '-' ) ? 1 : 0 );offset < string_size;offset++ )
	{
		if ( pdes[offset] == '.' )
		{
			Mdata = 0.1;
			continue;
		}
		if ( Mdata == 10 )
		{
			itemp = itemp * 10 + GLOBAL_CTON( pdes[offset] );
		}
		else
		{
			itemp =  itemp + GLOBAL_CTON( pdes[offset] ) * Mdata ;
			Mdata *= ( 0.1 );
		}
	}
	if ( pdes[0] == '-' )
	{
		itemp = -itemp;
	}
	return itemp;
}
/*********************************************************************
*@º¯ÊýÃû³Æ:ÉèÖÃÈ¥¶¶´ÎÊý
*@´´½¨ÈË:½Ò³É
*@ÊäÈë: times  :´ýÉèÖÃµÄÈ¥¶¶´ÎÊý
*      	    debounce_ptr :È¥¶¶²ÎÊý¿ØÖÆÓòÖ¸Õë
*@Êä³ö: NONE
*********************************************************************/
void global_debounce_times_mod( debounce_t *debounce_ptr, unsigned char  times )
{
	debounce_ptr->debounce_times_level = times;
	return ;
}
/*********************************************************************
*@º¯ÊýÃû³Æ:·Ç×èÈûÈ¥¶¶
*@´´½¨ÈË:½Ò³É
*@ÊäÈë:indata  :²ÉÑùÊý¾Ý£¬ÊµÊ±¸üÐÂ
*       	   debounce_ptr :È¥¶¶²ÎÊý¿ØÖÆÓòÖ¸Õë
*@Êä³ö: TURE or FAIL
*********************************************************************/
BOOL  global_debounce_support( uint8_t* in_data, debounce_t *debounce_ptr )
{
	if ( debounce_ptr->current_debounce_times == 0 )
	{
		global_memcpy( debounce_ptr->shadow_buf , in_data, debounce_ptr->shadow_buf_size );
		debounce_ptr->current_debounce_times++;
		if (( debounce_ptr->current_debounce_times ) > ( debounce_ptr->debounce_times_level ) )
		{
			debounce_ptr->current_debounce_times = 0;
			return TURE;
		}
		return FAIL;
	}
	debounce_ptr->current_debounce_times++;
	if ( debounce_ptr->debounce_conditon( debounce_ptr->shadow_buf, in_data ) )
	{
		if (( debounce_ptr->current_debounce_times ) > ( debounce_ptr->debounce_times_level ) )
		{
			debounce_ptr->current_debounce_times = 0;
			return TURE;
		}
	}
	else
	{
		debounce_ptr->current_debounce_times = 0;
	}
	return FAIL;
}

/********************************************************************
 *@´´½¨ÈË:½Ò³É
 *@¹¦ÄÜ:ÇóÒ»¶ÓÁÐÀÛ¼ÓºÍ
 *
 *@ÊäÈë:Phead Êý×éÍ·  size Êý×é³¤¶È data_type  Êý×éÀàÐÍ
 *
 *@Êä³ö:ËùÇóµÄºÍ
********************************************************************/
Bsize_t  global_sum_array( void*Phead, Bsize_t size, Bsize_t data_type )
{
	Bsize_t  sum = 0;
	float     fsum = 0;
	Bsize_t index;
	for ( index = 0;index < size;index++ )
	{
		if ( data_type == IS_CHAR || data_type == IS_UNSIGN_CHAR )
		{
			sum += ( *(( uint8_t* )Phead + index ) );
		}
		else if ( data_type == IS_INT || data_type == IS_UNSIGN_INT )
		{
			sum += ( *(( int* )Phead + index ) );
		}
		else if ( data_type == IS_FLOAT )
		{
			fsum += ( *(( float* )Phead + index ) );
		}
	}
	if ( data_type == IS_FLOAT ) return fsum;
	return sum;
}
/********************************************************************
 *@´´½¨ÈË:½Ò³É
 *@¹¦ÄÜ:US ¼¶ÑÓÊ±º¯Êý
 *
 *@ÊäÈë:NONE
 *
 *@Êä³ö:NONE
********************************************************************/
#define   US_MIP  50
void global_udelay( Bsize_t us )
{
	volatile int t_index, h_index;
	for ( t_index = 0;t_index < us;t_index++ )
	{
		for ( h_index = 0;h_index < US_MIP;h_index++ )
		{
			asm( "	nop" );
		}
	}
	return;
}
/********************************************************************
 * @´´½¨ÈË :½Ò³É
 * @¹¦ÄÜ:Êý×ÖÀàÐÍ×ª»»£¬2Î»ÊýÒÔÄÚ
 *
 * @ÊäÈë:NONE
 *
 *@Êä³ö:NONE
********************************************************************/
void  hex_convert_bcd( uint8_t src, uint8_t *bcd_src )
{
	uint8_t tmp = 0;
	while ( src >= 10 )
	{
		src -= 10;
		tmp += 16;
	}
	tmp += src;
	*bcd_src = tmp;
	return ;
}
void  bcd_convert_hex( uint8_t bcd_src, uint8_t *pur_ptr )
{
	uint8_t tmp = 0;
	while ( bcd_src >= 16 )
	{
		bcd_src -= 16;
		tmp += 10;
	}
	tmp += bcd_src;
	*pur_ptr = tmp;
	return ;
}
/********************************************************************
 *@´´½¨ÈË :½Ò³É
 *@¹¦ÄÜ :½«Êý×Ö0-9×ª»»³É×Ö·û
 *@ÊäÈë :num  ÊäÈëµÄÊý×Ö 0 -9£»
 *
 *@Êä³ö : '0'-'9'
********************************************************************/
char global_NtoC( uint8_t num )
{
	if ( num > 9 )
	{
		return IS_NO_NCHAR;
	}
	return ( '0' + num );
}
/********************************************************************
 *@´´½¨ÈË :½Ò³É
 *@¹¦ÄÜ :½«Êý×Ö0-9×ª»»³É×Ö·û
 *@ÊäÈë :num  ÊäÈëµÄÊý×Ö 0 -9£»
 *
 *@Êä³ö : '0'-'9'
********************************************************************/
uint8_t global_CtoN( char num )
{
	if (( '0' <= num ) && ( num <= '9' ) )
	{
		return ( num -'0' );
	}
	return IS_NO_NCHAR;
}
/********************************************************************
 *@´´½¨ÈË :½Ò³É
 *@¹¦ÄÜ :ÅÐ¶Ï×Ö·ûÊÇ²»ÊÇÊý×Ö×Ö·û
 *@ÊäÈë :asc Âë
 *
 *@Êä³ö : '0'-'9'
********************************************************************/
BOOL  global_decide_num( char asc )
{
	return DECCHK( asc );
}
/********************************************************************
 *@´´½¨ÈË :½Ò³É
 *@¹¦ÄÜ :ÅÐ¶Ï×Ö·ûÊÇ²»ÊÇ×ÖÄ¸
 *@ÊäÈë :asc Âë
 *
 *@Êä³ö : ×ÖÄ¸µÄÀàÐÍ
                 IS_BIG_LETTER : 	´óÐ´×ÖÄ¸
                 IS_LITTLE_LETTER : Ð¡Ð´×ÖÄ¸
                 IS_NO_LETTER: ²»ÊÇ×ÖÄ¸Êý¾Ý
********************************************************************/
Bsize_t global_decide_letter( char asc )
{
	Bsize_t ret = FAIL;
	if (( 65 <= asc ) && ( asc <= 90 ) )
	{
		ret = IS_BIG_LETTER;
	}
	else  if (( 97 <= asc ) && ( asc <= 122 ) )
	{
		ret = IS_LITTLE_LETTER;
	}
	else
	{
		ret = IS_NO_LETTER;
	}
	return ret;
}
/********************************************************************
 * @´´½¨ÈË :½Ò³É
 * @¹¦ÄÜ :´óÐ¡Ð´ÇÐ»»
 * @ÊäÈë :asc Âë
 *
 *@Êä³ö : ×ÖÄ¸Êä³ö£¬ÊÇ´óÐ´µÄÊä³öÐ¡Ð´£¬
 *			ÊÇÐ¡Ð´µÄÊä³ö´óÐ´¡£
********************************************************************/
uint8_t global_switch_letter_module( char asc )
{
	Bsize_t  type;
	uint8_t gap = (( uint8_t )'a' -( uint8_t )'A' );
	type = global_decide_letter( asc ) ;
	if ( type == IS_BIG_LETTER )
	{
		return ( gap + asc );
	}
	else if ( type == IS_LITTLE_LETTER )
	{
		return ( asc -gap );
	}
	return asc;
}
/********************************************************************
 * @´´½¨ÈË :½Ò³É
 * @¹¦ÄÜ :½«×Ö·ûASC µ÷ÕûÎªTYPE ÀàÐÍ(´óÐ´ »ò Ð¡Ð´)
 * @ÊäÈë :asc Âë
 *
 *@Êä³ö : '0'-'9'
********************************************************************/
uint8_t global_adjust_letter_module( char asc, uint8_t type )
{
	if ( global_decide_letter( asc ) == IS_NO_LETTER )
	{
		return asc;
	}
	if (( global_decide_letter( asc ) == type ) )
	{
		return asc;
	}
	return global_switch_letter_module( asc );
}

/********************************************************************
 * @´´½¨ÈË :½Ò³É
 * @¹¦ÄÜ :¸¡µãÊý·ÖÀë³öÕûÊýºÍÐ¡Êý
 * @ÊäÈë :src Òª´¦ÀíµÄ¸¡µãÊý£¬plong ÕûÐÎ»º´æ£¬ppoint Ð¡Êý»º´æ
 *
 *@Êä³ö :NONE
********************************************************************/
void global_float_putoff_point( float fsrc, long *plong, float*ppoint )
{
	*plong = ( long )fsrc;
	*ppoint = fsrc - *plong;
	return;
}
/********************************************************************
 * @´´½¨ÈË :½Ò³É
 * @¹¦ÄÜ :¸¡µãÊýËÄÉáÎåÈë
 * @ÊäÈë :src Òª´¦ÀíµÄ¸¡µãÊý
 *
 *@Êä³ö :´¦ÀíºóµÄ¸¡µãÊý
********************************************************************/
float  global_round_off_float( float src )
{
	long ltemp;
	float fpoint;
	global_float_putoff_point( GLOBAL_ABS( src ), &ltemp, &fpoint );
	( fpoint > 0.4 ) ? ( fpoint = 1 ) : ( fpoint = 0 );
	return( src > 0 ) ? (( float )ltemp + fpoint ) : ( -(( float )ltemp + fpoint ) );
}
/********************************************************************
 * @´´½¨ÈË :½Ò³É
 * @¹¦ÄÜ :È¥µô×Ö·û´®ÖÐÖ¸¶¨µÄËùÓÐ×Ö·û
 * @ÊäÈë :psrc Ô­×Ö·û´®£¬ch Ö¸¶¨×Ö·û
 *
 *@Êä³ö :NONE
********************************************************************/
void  global_string_filter( char*psrc, char ch )
{
	Bsize_t offset, shoffset;
	Bsize_t strlen = global_strlen( psrc );
	char *pshadow = app_malloc( strlen + 2 );
	if ( !pshadow ) return;
	GLOBAL_MEMCLR( pshadow, ( strlen + 2 ) );
	shoffset = 0;
	for ( offset = 0;offset < strlen;offset++ )
	{
		if ( psrc[offset] != ch )
		{
			pshadow[shoffset] = psrc[offset];
			shoffset++;
		}
	}
	global_memcpy( psrc, pshadow, strlen );
	app_free( pshadow );
	return;
}

/********************************************************************
 * @´´½¨ÈË :½Ò³É
 * @¹¦ÄÜ :²éÑ¯×éºÏ×Ö·û´®ÖÐ×îºóÒ»¸öºÏ·¨×Ö·û£¬±ÜÃâÖÐÎÄÂÒÂë
 * @ÊäÈë :
 *
 *@Êä³ö :NONE
********************************************************************/
char* global_ASC_code_search_end( char*src, Bsize_t size )
{
	Bsize_t len;
	Bsize_t offset = 0;
	char*pch;
	len = global_strlen( src );
	if ( len > size )
	{
		while ( 1 )
		{
			if (( src[offset]&0xff ) >= 0x81 )
			{
				if (( offset + 2 ) > ( size - 1 ) ) break;
				offset += 2;
			}
			else
			{
				if (( offset + 1 ) > ( size - 1 ) ) break;
				offset += 1;
			}
		}
		pch = src + offset;
	}
	else
	{
		pch = GET_STRING_END( src );
	}
	return pch;
}

/********************************************************************
 * @´´½¨ÈË :½Ò³É
 * @¹¦ÄÜ :ÔÚpbufºóÃæÌí¼Ó×Ö·û
 * @ÊäÈë :pbuf,buf_len³¤¶È,str×Ö·û´®
 *
 *Èç
 *		char buf[20];
 *		global_sprintf(buf,20,"is %d %s",10,"abc");
 *		"is 10 abc"
 *
 *@Êä³ö :NONE
********************************************************************/
#include <stdarg.h>
int global_sprintf( char* pbuf, Bsize_t buf_len, char* str, ... )
{
	va_list arp;
	char c, d, s[16], *p;
	Bsize_t offset;
	Bsize_t len;
	offset = 0;
	buf_len -= 1;
	va_start( arp, str );
	GLOBAL_MEMCLR( pbuf, buf_len );
	while ( 1 )
	{
		c = *str++;
		if ( c == 0 ) break;			
		if ( c != '%' )  				
		{
			pbuf[offset++] = c;
			continue;
		}
		c = *str++;
		if ( !c ) break;
		d = UPCASE( c );
		switch ( d )  				
		{
		case 'S' :					
			p = va_arg( arp, char* );
			len = global_strlen( p );
			if (( len + offset ) < buf_len )
			{
				global_strcpy( &pbuf[offset], p );
				offset += len;
			}
			continue;
		case 'C' :					
			len = 1;
			if (( len + offset ) < buf_len )
			{
				pbuf[offset++] = ( char )va_arg( arp, char );
			}
			continue;
                case 'F':
			len = global_ftoa(( float )va_arg( arp, float ), s,3);
			if (( len + offset ) < buf_len )
			{
				global_strcat( pbuf, s );
				offset += len;
			}		
			continue;
		case 'L' :
			len = global_ltoa(( long )va_arg( arp, long ), s );
			if (( len + offset ) < buf_len )
			{
				global_strcat( pbuf, s );
				offset += len;
			}
			continue;
		case 'D' :					
		case 'U' :					
			len = global_itoa(( int )va_arg( arp, int ), s );
			if (( len + offset ) < buf_len )
			{
				global_strcat( pbuf, s );
				offset += len;
			}
			continue;
		default:					
			continue;
		}
	}
	va_end( arp );
	return offset;
}


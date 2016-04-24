/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:GUI内存操作
* 创建时间:2012-07-22
* 所属公司 :科荟
* 文件名称: GUI_stc.c
* 创建人 :	揭成
* 功能描述: 内存操作函数
*----------------------------------------------------------------*/
#include	"../GUI.h"
/********************************************************************
*@创建人:揭成
*@功能:内存填充
*@输入: pur_add  :指向待填充内存开始地址
*       pad_data :填充数据
*      	lenth:数据长度
*@输出:	NONE
********************************************************************/
void GUI_memset( void*pur_add, uint8_t pad_data, uint16_t lenth )
{
	int index;
	uint8_t*p_pur = ( uint8_t* )pur_add;
	for ( index = 0;index < lenth;index++ )
	{
		p_pur[index] = pad_data;
	}
	return ;
}
/*********************************************************************
*@函数名称:测试字符长度
*@创建人	  :揭成
*@输入	  :psrc  :要测试的字符串
*@output  :字符长度
*********************************************************************/
uint16_t GUI_strlen( char*psrc )
{
	uint16_t lenth = 0;
	while (( *psrc ) != PNULL )
	{
		lenth++;
		psrc++;
	}
	return lenth;
}
/********************************************************************
 *@创建人:揭成
 *@功能:内存比较
 *@输入:pur_add :指向目的地址
 *      src_ptr :指向源地址
 *      lenth	:数据长度
 *@输出:NONE
********************************************************************/
BOOL  GUI_memcmp( void * pur_add, void*src_ptr, uint16_t lenth )
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
/********************************************************************
 *@创建人:揭成
 *@功能:内存复制
 *@输入:pur_add  :指向目标地址
 * 		pad_data :指向源地址
 *      lenth	 :数据长度
 *@输出:NONE
********************************************************************/
void GUI_memcpy( void * pur_add, void*src_ptr, uint16_t lenth )
{
	int index;
	char*p_pur = ( char* )pur_add;
	char*p_src = ( char* )src_ptr;
	for ( index = 0;index < lenth;index++ )
	{
		p_pur[index] = p_src[index];
	}
	return ;
}
/*********************************************************************
*@函数名称:将字符串pstr2合并到pstr1
*@创建人:揭成
*@输入: pstr1  pstr2
*@ output :NONE
*必须确保pstr1 有足够的空间
*********************************************************************/
void GUI_strcat( char*pstr1, char*pstr2 )
{
	char*pch;
	pch = pstr1 + GUI_strlen( pstr1 );
	GUI_memcpy( pch, pstr2, ( GUI_strlen( pstr2 ) + 1 ) );
	return;
}
/*********************************************************************
*@函数名称:字符串反转
*@创建人:揭成
*@输入:
*@输出: NONE
*********************************************************************/
void GUI_string_reversal( char *psrc )
{
	char *pstr1 = psrc;
	char *pstr2;
	char ch;
	pstr2 = ( char* )psrc + ( GUI_strlen( psrc ) - 1 );
	while ( pstr1 < pstr2 )
	{
		if ( *pstr1 != *pstr2 )
		{
			ch 	   = *pstr1;
			*pstr1 = *pstr2;
			*pstr2 = ch;
		}
		++pstr1;
		--pstr2;
	}
}
/*********************************************************************
*@函数名称:int 转换成字符串
*@创建人:揭成
*@输入:
*@输出: NONE
*********************************************************************/
int GUI_ltoa( long isrc, char *pdes )
{
	long  itemp = isrc;
	int offset = 0;
	if ( itemp < 0 )
	{
		isrc = isrc * ( -1 );
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
	pdes[offset] = PNULL;
	GUI_string_reversal( pdes );
	return ( GUI_strlen( pdes ) );
}
/*********************************************************************
*   @函数名称:复制字符串
*   @创建人:揭成
*   @输入: psrc  :要复制字符串
*
*    @ output :字符长度
*
*********************************************************************/
char* GUI_strcpy( char*pdest, char*psrc )
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
/********************************************************************
 * @创建人 :揭成
 * @功能 :浮点数分离出整数和小数
 * @输入 :src 要处理的浮点数，plong 整形缓存，ppoint 小数缓存
 *
 * @输出 :NONE
********************************************************************/
void GUI_float_putoff_point( float fsrc, long *plong, float*ppoint )
{
	*plong  = ( long )fsrc;
	*ppoint = fsrc - *plong;
	return;
}
/*********************************************************************
*@函数名:float 转换成字符串
*@创建人	:揭成
*@输入	:
*@output:NONE
*FAIL 	:去抖过程中
*********************************************************************/
int GUI_ftoa( float fsrc , char*pdes, int point_bit )
{
	long ltemp, lmul;
	float fpoint;
	char point[8] = "";
	GUI_float_putoff_point( fsrc, &ltemp, &fpoint );
	if ( ltemp == 0 && fsrc < 0 )
	{
		pdes[0] = '-';
		pdes[1] = '0';
		pdes[2] = PNULL;
	}
	else GUI_ltoa( ltemp, pdes );
	fpoint = GUI_ABS( fpoint );
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
		ltemp = GUI_ABS( ltemp );
		GUI_ltoa( ltemp, point );
		while (( *GUI_GET_STRING_END( point ) == '0' ) && ( GUI_strlen( point ) > 0 ) )
		{
			*GUI_GET_STRING_END( point ) = PNULL;
		}
		GUI_strcat( pdes, "." );
		lmul /= 10;
		while ( ltemp < lmul )
		{
			GUI_strcat( pdes, "0" );
			lmul /= 10;
		}
		GUI_strcat( pdes, point );
		while ( *GUI_GET_STRING_END( pdes ) == '.' )
		{
			GUI_strcat( pdes, "0" );
		}
	}
	return GUI_strlen( pdes );
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :在字符串中定位一个字符串
 * @输入 :psrc 字符串  pserch 待查询的字符串
 *
 *@输出 :查询到字符的位置
********************************************************************/
char* GUI_string_locate( char*psrc, char*psearch )
{
	uint16_t offset;
	uint16_t psearch_lenth = GUI_strlen( psearch );
	uint16_t psrc_lenth = GUI_strlen( psrc );
	for ( offset = 0;offset < psrc_lenth;offset++ )
	{
		if ( GUI_memcmp( &psrc[offset],
						 psearch,
						 psearch_lenth ) )
		{
			return &( psrc[offset] );
		}
	}
	return PNULL;
}

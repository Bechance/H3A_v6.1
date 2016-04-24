#include    "../os_connect.h"
/********************************************************************
 *@创建人:揭成
 *@功能:内存复制
 *@输入:pur_add  :指向目标地址
 * 		pad_data :指向源地址
 *      lenth    :数据长度
 *@输出:NONE
********************************************************************/
void os_memcpy( void * pur_add, void*src_ptr, Bsize_t lenth )
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
 *@创建人:揭成
 *@功能:内存填充
 *
 *@输入: 	pur_add  :指向待填充内存开始地址
 *       	pad_data :填充数据
 *      	lenth	 :数据长度
 *@输出:	NONE
********************************************************************/
void os_memset( void*pur_add, uint8_t pad_data, Bsize_t lenth )
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
 *@创建人:揭成
 *@功能:内存比较
 *@输入:pur_add :指向目的地址
		src_ptr :指向源地址
		lenth   :数据长度
 *@输出:NONE
********************************************************************/
BOOL  os_memcmp( void * pur_add, void*src_ptr, Bsize_t lenth )
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
*@函数名称:测试字符长度
*@创建人:揭成
*@输入: psrc  :要测试的字符串
*@ output :字符长度
*********************************************************************/
Bsize_t os_strlen( char*psrc )
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
*@函数名称:将字符串pstr2合并到pstr1
*@创建人:揭成
*@输入: pstr1  pstr2
*@ output :NONE
*必须确保pstr1 有足够的空间
*********************************************************************/
void os_strcat( char*pstr1, char*pstr2 )
{
	char*pch;
	pch = ( char* )pstr1 + os_strlen( pstr1 );
	os_memcpy(( uint8_t* )pch,
				  ( uint8_t* )pstr2,
				  ( os_strlen( pstr2 ) + 1 ) );
	return;
}
/*********************************************************************
*   @函数名称:复制字符串
*   @创建人:揭成
*   @输入: psrc  :要复制字符串
*
*    @ output :字符长度
*
*********************************************************************/
char* os_strcpy( char*pdest, char*psrc )
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
 * @功能 :在字符串中定位一个字符串
 * @输入 :psrc 字符串  pserch 待查询的字符串
 *
 *@输出 :查询到字符的位置
********************************************************************/
char* os_string_locate_first( char*psrc, char*psearch )
{
	Bsize_t offset;
	Bsize_t psearch_lenth = os_strlen( psearch );
	Bsize_t psrc_lenth = os_strlen( psrc );
	for ( offset = 0;offset < psrc_lenth;offset++ )
	{
		if ( os_memcmp( &psrc[offset],
							psearch,
							psearch_lenth ) )
		{
			return &( psrc[offset] );
		}
	}
	return PNULL;
}

#include    "../os_connect.h"
/********************************************************************
 *@������:�ҳ�
 *@����:�ڴ渴��
 *@����:pur_add  :ָ��Ŀ���ַ
 * 		pad_data :ָ��Դ��ַ
 *      lenth    :���ݳ���
 *@���:NONE
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
 *@������:�ҳ�
 *@����:�ڴ����
 *
 *@����: 	pur_add  :ָ�������ڴ濪ʼ��ַ
 *       	pad_data :�������
 *      	lenth	 :���ݳ���
 *@���:	NONE
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
 *@������:�ҳ�
 *@����:�ڴ�Ƚ�
 *@����:pur_add :ָ��Ŀ�ĵ�ַ
		src_ptr :ָ��Դ��ַ
		lenth   :���ݳ���
 *@���:NONE
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
*@��������:�����ַ�����
*@������:�ҳ�
*@����: psrc  :Ҫ���Ե��ַ���
*@ output :�ַ�����
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
*@��������:���ַ���pstr2�ϲ���pstr1
*@������:�ҳ�
*@����: pstr1  pstr2
*@ output :NONE
*����ȷ��pstr1 ���㹻�Ŀռ�
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
*   @��������:�����ַ���
*   @������:�ҳ�
*   @����: psrc  :Ҫ�����ַ���
*
*    @ output :�ַ�����
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
 * @������ :�ҳ�
 * @���� :���ַ����ж�λһ���ַ���
 * @���� :psrc �ַ���  pserch ����ѯ���ַ���
 *
 *@��� :��ѯ���ַ���λ��
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

/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:GUI�ڴ����
* ����ʱ��:2012-07-22
* ������˾ :����
* �ļ�����: GUI_stc.c
* ������ :	�ҳ�
* ��������: �ڴ��������
*----------------------------------------------------------------*/
#include	"../GUI.h"
/********************************************************************
*@������:�ҳ�
*@����:�ڴ����
*@����: pur_add  :ָ�������ڴ濪ʼ��ַ
*       pad_data :�������
*      	lenth:���ݳ���
*@���:	NONE
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
*@��������:�����ַ�����
*@������	  :�ҳ�
*@����	  :psrc  :Ҫ���Ե��ַ���
*@output  :�ַ�����
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
 *@������:�ҳ�
 *@����:�ڴ�Ƚ�
 *@����:pur_add :ָ��Ŀ�ĵ�ַ
 *      src_ptr :ָ��Դ��ַ
 *      lenth	:���ݳ���
 *@���:NONE
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
 *@������:�ҳ�
 *@����:�ڴ渴��
 *@����:pur_add  :ָ��Ŀ���ַ
 * 		pad_data :ָ��Դ��ַ
 *      lenth	 :���ݳ���
 *@���:NONE
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
*@��������:���ַ���pstr2�ϲ���pstr1
*@������:�ҳ�
*@����: pstr1  pstr2
*@ output :NONE
*����ȷ��pstr1 ���㹻�Ŀռ�
*********************************************************************/
void GUI_strcat( char*pstr1, char*pstr2 )
{
	char*pch;
	pch = pstr1 + GUI_strlen( pstr1 );
	GUI_memcpy( pch, pstr2, ( GUI_strlen( pstr2 ) + 1 ) );
	return;
}
/*********************************************************************
*@��������:�ַ�����ת
*@������:�ҳ�
*@����:
*@���: NONE
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
*@��������:int ת�����ַ���
*@������:�ҳ�
*@����:
*@���: NONE
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
*   @��������:�����ַ���
*   @������:�ҳ�
*   @����: psrc  :Ҫ�����ַ���
*
*    @ output :�ַ�����
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
 * @������ :�ҳ�
 * @���� :�����������������С��
 * @���� :src Ҫ����ĸ�������plong ���λ��棬ppoint С������
 *
 * @��� :NONE
********************************************************************/
void GUI_float_putoff_point( float fsrc, long *plong, float*ppoint )
{
	*plong  = ( long )fsrc;
	*ppoint = fsrc - *plong;
	return;
}
/*********************************************************************
*@������:float ת�����ַ���
*@������	:�ҳ�
*@����	:
*@output:NONE
*FAIL 	:ȥ��������
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
 * @������ :�ҳ�
 * @���� :���ַ����ж�λһ���ַ���
 * @���� :psrc �ַ���  pserch ����ѯ���ַ���
 *
 *@��� :��ѯ���ַ���λ��
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

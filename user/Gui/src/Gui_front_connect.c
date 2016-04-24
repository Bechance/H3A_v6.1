/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:GUI光标
* 创建时间:2010-12-18
* 所属公司 :科荟
* 文件名称:GUI_front_connect.c
* 创建人 :揭成
* 功能描述:GUI字体管理
*----------------------------------------------------------------
修改记录:
修改日期:
修改人:
********************************************************************/
#include "../Gui.h"
#if GUI_LIBCONNECT_EN>0
uint16_t		m_GUI_CHwide;
uint16_t		m_GUI_CHhigh;
GUI_front_t     m_GUI_front;
BOOL GUI_Extlib_data_obtain( GUI_front_t*pfront, uint16_t ASCcode, uint8_t size );
void GUI_SetFront( const GUI_FONT*pNEW );
void GUI_extlib_init( void );
void GUI_local_ASC_data_obtain( GUI_front_t*pfront, uint16_t ASCcode );

const GUI_FONT* GUI_GET_DEFGRONT()
{
	return ( &GUI_HZK24_EXTASCII );
}
void GUI_SET_DEFFRONT( void )
{
	GUI_SetFront( GUI_GET_DEFGRONT() );
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :设置字体
 * @输入 :
 * @输出 :
********************************************************************/
void GUI_SetFront( const GUI_FONT*pNEW )
{
	GUI_conText.CUR_Front  = ( GUI_FONT* )pNEW;
	m_GUI_CHwide = pNEW->CHwide;
	m_GUI_CHhigh = pNEW->high;
	return;
}
void GUI_SetLineHigh( Bsize_t lineH)
{
	GUI_conText.CUR_Lhigh= lineH;
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :设置字体
 * @输入 :
 * @输出 :
********************************************************************/
const GUI_FONT*GUI_GetFront( void )
{
	return ( const GUI_FONT* )GUI_conText.CUR_Front;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :获取当前字符数据数据
 * @输入 :
 * @输出 :
********************************************************************/
static void GUI_front_data_obtain( GUI_front_t*pfront, uint16_t ASCcode, uint8_t size )
{
	if ( !GUI_conText.CUR_Front->ASC_obtain )
	{
		GUI_local_ASC_data_obtain( pfront, ASCcode );
	}
	else
	{
		GUI_conText.CUR_Front->ASC_obtain( pfront, ASCcode, size );
	}
	return;
}



void GUI_VASCcode_display(
	uint16_t x, uint16_t y, Bsize_t wide,
	Bsize_t high, Bsize_t real_wide, void*pbuf
)
{
	Bsize_t Hoffset = 0;
	Bsize_t Woffset = 0;
	Bsize_t Boffset = 0;
	char  cdata;
	GUI_fill_Rect(x,y,real_wide,high);
	for ( Hoffset = 0;Hoffset < wide;Hoffset++ )
	{
		if ( Hoffset > real_wide )continue;
		for ( Woffset = 0;Woffset < ( high / 8 );Woffset++ )
		{
			cdata = (( char* )pbuf )[Hoffset*( high/8 )+Woffset];
			for ( Boffset = 0;Boffset < 8;Boffset++ )
			{
				if ( ( cdata&( 0x80 >> Boffset ) ) )
					GUI_draw_dot( x + Hoffset, y + 8*Woffset + Boffset );
			}
		}
	}
	return;
}

void GUI_HASCcode_display(
	uint16_t x, uint16_t y, Bsize_t wide,
	Bsize_t high, Bsize_t real_wide, void*pbuf
)
{
	Bsize_t Hoffset = 0;
	Bsize_t Woffset = 0;
	Bsize_t Boffset = 0;
	char  cdata;
	GUI_fill_Rect(x,y,real_wide,high);
	for (Hoffset=0;Hoffset<high;Hoffset++)
	{
		for ( Woffset = 0;Woffset < ( wide / 8 );Woffset++ )
		{
			cdata = (( char* )pbuf )[Hoffset*( wide/8 )+Woffset];
			for ( Boffset = 0;Boffset < 8;Boffset++ )
			{
				if (( Boffset + Woffset*8 ) > real_wide )continue;
				if (( cdata&( 0x80 >> Boffset ) ) )
					GUI_draw_dot( x + 8*Woffset + Boffset, y + Hoffset );
			}
		}
	}
	return;
}
/********************************************************************
 * @创建人 : 揭成
 * @功能   : LCD显示字符串
 * @输入   :
 * @输出   :
********************************************************************/
void GUI_print_string( int x, int y, int xs, int ys, char*str )
{
	uint16_t  ASCcode;
	Bsize_t   offset;
	Bsize_t   str_len;
	uint16_t  asc_code_h;
	uint16_t  asc_code_l;
	uint16_t  x_start = x ;
	uint8_t	  size;
	str_len = GUI_strlen( str );
	offset  = 0;
	while ( offset < str_len )
	{
		if (( str[offset]&0xff ) >= 0x81 )  /*判断是不是汉字*/
		{
			asc_code_h = (( uint8_t )( str[offset] ) * 0x100 );
			asc_code_l = ( uint8_t )str[offset+1];
			ASCcode    = asc_code_h + asc_code_l;
			size       = 2;
			offset     = 2 + offset;
		}
		else if (( str[offset]&0xff ) >= 0x20 && ( str[offset]&0xff ) <= 0x7E )
		{
			ASCcode  = str[offset];
			size = 1;
			offset++;
		}
		else break;
		GUI_front_data_obtain( &m_GUI_front, ASCcode, size );
		if (( xs - x ) < m_GUI_front.CHwide )
		{
			x = x_start;
			if ( y < ys )y += m_GUI_front.high+GUI_conText.CUR_Lhigh;
		}
		if ( m_GUI_front.way == 'V' || m_GUI_front.way == 'v' )
			GUI_VASCcode_display( x, y, m_GUI_front.wide, m_GUI_front.high, m_GUI_front.CHwide, m_GUI_front.Dbuf );
		else
			GUI_HASCcode_display( x, y, m_GUI_front.wide, m_GUI_front.high, m_GUI_front.CHwide, m_GUI_front.Dbuf );
		x += m_GUI_front.CHwide;
	}
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能:系统字库初始化
 *
 * @输入:NONE
 * @输出:NONE
********************************************************************/
void GUI_lib_init( void )
{
	GUI_extlib_init();
	GUI_SET_DEFFRONT();
	return;
}
#endif

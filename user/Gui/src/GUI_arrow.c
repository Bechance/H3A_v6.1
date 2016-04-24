#if  GUI_ARROW_EN >0
#include "../Gui.h"
#include "GUI_lcd_driver.h"
static int       m_GUI_ARROW_CURposx;
static int       m_GUI_ARROW_CURposy;
static uint16_t* m_GUI_ARROW_CURPTR;
static int       m_GUI_ARROW_First = 0;

//箭头提示
const unsigned char m_GUI_ARROW[60+2] =
{
	15, 15,//24,20,
	0x40, 0x00, 0x00,
	0x50, 0x00, 0x00,
	0x64, 0x00, 0x00,
	0x69, 0x00, 0x00,
	0x6A, 0x40, 0x00,
	0x6A, 0x90, 0x00,
	0x6A, 0xA4, 0x00,
	0x6A, 0xA9, 0x00,
	0x6A, 0xAA, 0x40,
	0x6A, 0xAA, 0x90,
	0x6A, 0xAA, 0xA4,
	0x6A, 0xA9, 0x55,
	0x69, 0xA9, 0x00,
	0x64, 0x6A, 0x40,
	0x50, 0x6A, 0x40,
	0x40, 0x1A, 0x90,
	0x00, 0x1A, 0x90,
	0x00, 0x06, 0xA4,
	0x00, 0x06, 0xA4,
	0x00, 0x01, 0x50
};
/********************************************************************
 * @功能 :LCD显示任意大小字模
********************************************************************/
static  void GUI_arrow_draw( uint16_t x, uint16_t y )
{
	GUI_set_Fgcolor( C_GUI_WHITE );
	GUI_draw_line( x, y + 5, x + 11, y + 5 );
	GUI_draw_line( x + 5, y, x + 5, y + 11 );
	return;
}

static void GUI_arrow_display( uint16_t x, uint16_t y )
{
	Bsize_t wide = m_GUI_ARROW[0];
	Bsize_t high = m_GUI_ARROW[1];
	GUI_write_block(
		m_GUI_ARROW_CURposx,
		m_GUI_ARROW_CURposy,
		m_GUI_ARROW_CURposx + wide + 2,
		m_GUI_ARROW_CURposy + high + 2, m_GUI_ARROW_CURPTR, 0 );
	m_GUI_ARROW_CURposx = x;
	m_GUI_ARROW_CURposy = y;
	GUI_read_block(
		m_GUI_ARROW_CURposx,
		m_GUI_ARROW_CURposy,
		m_GUI_ARROW_CURposx + wide + 2,
		m_GUI_ARROW_CURposy + high + 2,
		m_GUI_ARROW_CURPTR );
	GUI_arrow_draw( x, y );
	return;
}

void GUI_Set_arrow_pos( uint16_t x, uint16_t y )
{
	Bsize_t wide = m_GUI_ARROW[0] + 2;
	Bsize_t high = m_GUI_ARROW[1] + 2;
	if ( m_GUI_ARROW_First == 0 )
	{
		m_GUI_ARROW_First   = 1;
		m_GUI_ARROW_CURposx = x;
		m_GUI_ARROW_CURposy = y;
		GUI_read_block(
			m_GUI_ARROW_CURposx,
			m_GUI_ARROW_CURposy,
			m_GUI_ARROW_CURposx + wide + 2,
			m_GUI_ARROW_CURposy + high + 2,
			m_GUI_ARROW_CURPTR );
		GUI_arrow_draw( m_GUI_ARROW_CURposx, m_GUI_ARROW_CURposy );
	}
	else if ( x != m_GUI_ARROW_CURposx
			  || y != m_GUI_ARROW_CURposy )
	{
		GUI_arrow_display( x, y );
	}
	return;
}


void GUI_arrow_init( void )
{
	Bsize_t wide = m_GUI_ARROW[0] + 2;
	Bsize_t high = m_GUI_ARROW[1] + 2;
	m_GUI_ARROW_First = 0;
	m_GUI_ARROW_CURPTR = GUI_MALLOC( wide * high );
	return;
}
#else
void GUI_arrow_init( void )
{
	;
}

#endif


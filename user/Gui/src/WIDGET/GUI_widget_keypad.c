/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:手写管理
* 创建时间:2011-10-04
* 所属公司:科荟
* 文件名称:GUI_keypad.c
* 作   者 :揭成
* 功能描述:系统手写管理相关函数
*----------------------------------------------------------------
修改记录:

修改日期:

修改人:
********************************************************************/
#include "../../Gui.h"
#if GUI_TOUCH_EN >0
#define     KEY_PAD_FONT         &GUI_HZK24_EXTASCII
#define     PAD_FG_COR           C_GUI_RED
#define     PAD_BG_COR           LCD_Color2Index_565(0xc0c0c0)
#define     KEYPAD_WIDE          450
#define     KEYPAD_HIGH          200
static int  keypad_x 			 = 10;
static int  keypad_y 			 = LCD_HIGH - KEYPAD_HIGH - 10;
static      hwin_t*   			 m_keyPAD          = 0;
const 		GUI_FONT*			 m_keyPAD_Font = KEY_PAD_FONT;
static char m_Rbuf[10];
static int  m_Rsize				 =	0;

void  	    GUI_handPen_creat( hwin_t * pWIN );
Bsize_t 	GUI_handPen_GB_obtain( char*hzbuf );
void  		GUI_handPen_HZBOX_handle( void );
void 		GUI_set_Bgcolor( uint16_t color );
uint16_t LCD_Color2Index_565( uint32_t Color );

#define      _KEY_ENTER      KEY_ENTER
#define      _KEY_DEL        KEY_DEL
#define      _KEY_OK         KEY_0
#define      SEND_PADMSG     GUI_dummy_press_Ekey

BLKmem_t m_keyPADPmem;

const    char*m_ABCkey_tab[] =
{
	"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
	"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P",
	"A", "S", "D", "F", "G", "H", "J", "K", "L",
	"Z", "X", "C", "V", "B", "N", "M", "."
};
const    char*m_abckey_tab[] =
{
	"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
	"q", "w", "e", "r", "t", "y", "u", "i", "o", "p",
	"a", "s", "d", "f", "g", "h", "j", "k", "l",
	"z", "x", "c", "v", "b", "n", "m", "."
};

const    char*m_func_tab[] =
{
	"手写", "大写", "小写", "删除", "确认"
};

static void GUI_key_creat(hwin_t * pWIN,const char**phead)
{	
   hwin_t * pOBJ;
   int      xgap,ygap,x0,y0,Bwide,Bhigh,pos,n;
   uint8_t  id;
   GUI_FONT*pF = (GUI_FONT*)m_keyPAD_Font;
   pWIN->type = WIDEGE_PAD_KEY;
   Bwide =  pF->CHwide*3+5;
   Bhigh =  pF->high+10;
   xgap  = (pWIN->win_wide-Bwide*10)/11;
   ygap  = (pWIN->win_high-Bhigh*5)/6;
   GUI_window_hwin_req(pWIN);
   GUI_SetFront(pF);
   for(pos = 0;pos< 20 ;pos++)
   {
        id   = phead[pos][0];
		x0   = xgap+(pos%10)*(Bwide+xgap);
		y0   = ygap+(pos/10)*(Bhigh+ygap);
   		pOBJ = Gui_button_creat(x0,y0,Bwide,Bhigh,WIN_MSG_EN|BUTTON_PAINT|id,pWIN);
   		GUI_button_title_display(pOBJ,(char*)phead[pos]);
   }
   xgap  = (pWIN->win_wide-Bwide*9)/10;
   for(pos = 0;pos<9 ;pos++)
   {
        id     = (uint8_t)phead[pos+20][0];
		x0     = xgap+(pos%9)*(Bwide+xgap);
		y0     = ygap+2*(Bhigh+ygap);
   		pOBJ   = Gui_button_creat(x0,y0,Bwide,Bhigh,WIN_MSG_EN|BUTTON_PAINT|id,pWIN);
   		GUI_button_title_display(pOBJ,(char*)phead[pos+20]);
   }
   xgap  = (pWIN->win_wide-Bwide*8)/9;
   for(pos = 0;pos<8;pos++)
   {
        id     = (uint8_t)phead[pos+29][0];
		x0     = xgap+(pos%8)*(Bwide+xgap);
		y0     = ygap+3*(Bhigh+ygap);
   		pOBJ   = Gui_button_creat(x0,y0,Bwide,Bhigh,WIN_MSG_EN|BUTTON_PAINT|id,pWIN);
   		GUI_button_title_display(pOBJ,(char*)phead[pos+29]);
   }
    n = SIZE_OF_ARRAY(m_func_tab);
   
    Bwide =  pF->CHwide*n+10;
    xgap  = (pWIN->win_wide-Bwide*n)/(n+1);
    Bhigh =  pF->high+10;
	y0     = ygap+4*(Bhigh+ygap);
    GUI_SetFront(&GUI_HZK16_EXTASCII);
    for(pos=0;pos<n;pos++)
    {
        id     = 200+pos;
		x0     = xgap+(pos%n)*(Bwide+xgap);
		pOBJ = Gui_button_creat(x0,y0,Bwide,Bhigh,WIN_MSG_EN|BUTTON_PAINT|id,pWIN);
		Gui_button_dye(pOBJ,C_GUI_ORANGE,C_GUI_WHITE);
		GUI_button_title_display(pOBJ,(char*)m_func_tab[pos]);
    }
	GUI_window_hwin_release(pWIN);
	return;
}



static void GUI_keypad_area_create( void )
{
	GUI_LOCK();
	GUI_set_Bgcolor( PAD_BG_COR );
	m_keyPAD = GUI_window_creat( keypad_x, keypad_y, KEYPAD_WIDE, KEYPAD_HIGH, WIN_FILL | WIN_3D );
	GUI_MsgFIFO_create( m_keyPAD, 30 );
	GUI_TopWIN_Set( m_keyPAD );
	GUI_UNLOCK();
	return;
}

static void GUI_keypad_area_delete( void )
{
	GUI_LOCK();
	if ( m_keyPAD )
	{
		GUI_TopWIN_cancel( m_keyPAD );
		GUI_window_destory( m_keyPAD );
	}
	GUI_UNLOCK();
	return;
}


static void GUI_keypad_type_select( int type )
{
	if ( type == 0 )
	{
		GUI_keypad_area_delete();
		GUI_keypad_area_create();
		GUI_handPen_creat( m_keyPAD );
	}
	else if ( 1 == type )
	{
		GUI_keypad_area_delete();
		GUI_keypad_area_create();
		GUI_key_creat( m_keyPAD, m_ABCkey_tab );
	}
	else if ( 2 == type )
	{
		GUI_keypad_area_delete();
		GUI_keypad_area_create();
		GUI_key_creat( m_keyPAD, m_abckey_tab );
	}
	return;
}


void GUI_keypad_read(void)
{
    uint8_t key;
	if(!m_keyPAD) return;
    if(m_keyPAD->type == WIDEGE_PAD_KEY)
    {
       if(GUI_MsgFIFO_read(m_keyPAD,&key,1)==1)
       {
           if(key<200)
           {
               m_Rbuf[0] = key;
			   m_Rsize   = 1;
			   SEND_PADMSG(_KEY_OK);
		   }
		   else if(key<203)
		   {
               GUI_keypad_type_select(key-200);
		   }
		   else if(key==203)SEND_PADMSG(_KEY_DEL);
		   else if(key==204)SEND_PADMSG(_KEY_ENTER);
	   }
	}
	else if(m_keyPAD->type == WIDEGE_PAD_HAND)
	{
		GUI_handPen_HZBOX_handle();
		m_Rsize=GUI_handPen_GB_obtain(m_Rbuf);
	}
    return ;
}





static void GUI_LCDmem_read( void )
{
	int x1, y1, x2, y2;
	uint32_t size;
	x1 = keypad_x - 2;
	y1 = keypad_y - 2;
	x2 = x1 + KEYPAD_WIDE + 4;
	y2 = y1 + KEYPAD_HIGH + 4;
	size = (( uint32_t )x2 - ( uint32_t )x1 ) * (( uint32_t )y2 - ( uint32_t )y1 );
	size = size;
	m_keyPADPmem = GUI_read_block_Extp( x1, y1, x2, y2 );
	return;
}

static void GUI_LCDmem_wirte( void )
{
	int x1, y1, x2, y2;
	x1 = keypad_x - 2;
	y1 = keypad_y - 2;
	x2 = x1 + KEYPAD_WIDE + 4;
	y2 = y1 + KEYPAD_HIGH + 4;
	GUI_write_block_Extp( x1, y1, x2, y2, m_keyPADPmem );
	return;
}


void GUI_keypad_open( void )
{
	GUI_LCDmem_read();
	GUI_keypad_area_create();
	GUI_key_creat( m_keyPAD, m_ABCkey_tab );
	return;
}


void GUI_keypad_close( void )
{
	if ( m_keyPAD )
	{
		GUI_keypad_area_delete();
		GUI_LCDmem_wirte();
		m_keyPAD = 0;
	}
	return;
}


BOOL 	 GUI_keypad_is_exist( void )
{
	return ( m_keyPAD == 0 ) ? ( 0 ) : ( 1 );
}



Bsize_t  GUI_keypad_GB_obtain( char*hzbuf )
{
	int n;
	if ( m_Rsize > 0 )
	{
		n = m_Rsize;
		GUI_memcpy( hzbuf, m_Rbuf, m_Rsize );
		m_Rsize = 0;
		return n;
	}
	return 0;
}


void GUI_keypad_Setpos( int x, int y )
{
	keypad_x = x;
	keypad_y = y;
	return;
}


void GUI_keypad_init( void )
{
	m_keyPAD = 0;
	return;
}
#endif

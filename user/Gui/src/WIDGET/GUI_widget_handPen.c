/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:手写管理
* 创建时间:2013-04-01
* 所属公司:科荟
* 文件名称:GUI_widget_handpen.c
* 作   者 :揭成
* 功能描述:系统手写管理相关函数
*----------------------------------------------------------------
修改记录:

修改日期:

修改人:
********************************************************************/
#if GUI_TOUCH_EN >0
#include "../../Gui.h"

extern hwin_t*	m_keyPAD 		    ;          
hwin_t*			hand_area_write 	    = PNULL;
hwin_t*			hand_HZ_box 			= PNULL;
BOOL      		hand_HZ_box_dis_enable  = FAIL;
BOOL      		hand_HZ_box_start_input = FAIL;
char 	  		m_hand_input_HZbuf[30];
uint32_t  		m_GUI_keypad_cnt  =0;
app_touch_t 	m_last_touch_data;
BOOL            m_HZFIFO_full           = FAIL;


#define         HAND_HZ_WIDE     		28
#define         HAND_FONT           	((GUI_FONT*)&GUI_HZK24_EXTASCII)

/********************************************************************
 * @作者 :揭成
 * @功能 :触摸按键匹配
 *
 * @输入 :hp  BUTTON 句柄，data 获取到的触摸数据
 *
 *@输出  :NONE
***********************************************************/
static BOOL GUI_window_touch_match( hwin_t*hp, app_touch_t*data )
{
	int xmax = hp->win_x + hp->win_wide -2;
	int xmin  = hp->win_x + 2;
	int ymax = hp->win_y + hp->win_high - 2;
	int ymin  = hp->win_y+2;
	( xmax > LCD_WIDE ) ? ( xmax = LCD_WIDE ) : ( xmax = xmax );
	( xmin < 0 ) ? ( xmin = 0 ) : ( xmin = xmin );
	( ymax > LCD_HIGH ) ? ( ymax = LCD_HIGH ) : ( ymax = ymax );
	( ymin < 0 ) ? ( ymin = 0 ) : ( ymin = ymin );
	if ( data->x < xmin ) return FAIL;
	if ( data->x > xmax ) return FAIL;
	if ( data->y < ymin ) return FAIL;
	if ( data->y > ymax ) return FAIL;
	return TURE;
}
/********************************************************************
 * @作者 :揭成
 * @功能 :手写处理
 * @输入 :
 * @输出 :NONE
***********************************************************/
static void GUI_handPen_touch_handle(app_touch_t*ptouch)
{
   if(GUI_window_touch_match(hand_area_write,ptouch))
   {
   	    GUI_set_Fgcolor(C_GUI_RED);
		hand_HZ_box_start_input = TURE;
		m_GUI_keypad_cnt =OSTimeGet(); 
		if(m_last_touch_data.x==0
		&&m_last_touch_data.y==0)
	    GUI_draw_dot(ptouch->x,ptouch->y);
		else
     	GUI_draw_line(ptouch->x,ptouch->y,
     	m_last_touch_data.x,m_last_touch_data.y);
        GUI_memcpy(&m_last_touch_data,ptouch,sizeof(app_touch_t));
		return;
   }
   return ;
}



static void GUI_handPen_HZBOX_auto_clean(BOOL flag)
{
	if(flag||(OSTimeGet()-m_GUI_keypad_cnt)>(APP_100_MS*20))
	{
		GUI_window_hwin_req(hand_area_write);
	    GUI_set_Bgcolor(C_GUI_BlACK);
		GUI_clean_window();
		m_GUI_keypad_cnt =OSTimeGet(); 
		GUI_window_hwin_release(hand_area_write);
	}
   return;
}
/********************************************************************
 * @作者 :揭成
 * @功能 :手写输入后汉字显示
 *
 * @输入 :
 *
 * @输出  :NONE
***********************************************************/
void  GUI_handPen_HZBOX_handle(void)
{
	BOOL flag;
	char icon[3]="";
	Bsize_t offset;
	hwin_t*pOBJ;
	int     xgap,ygap,x0,y0,Bwide,Bhigh;
	uint8_t id;
	
	GUI_FONT*pF = (GUI_FONT*)&GUI_HZK16_EXTASCII;
	os_driver_ioctl(m_touch_dev_id,TOUCH_CMD_GET_HZ_FLAG,&flag);
	if(flag&&hand_HZ_box_start_input)
	{
		hand_HZ_box_dis_enable = TURE;
		hand_HZ_box_start_input = FAIL;
		os_driver_ioctl(m_touch_dev_id,TOUCH_CMD_GET_HZ_BOX,m_hand_input_HZbuf);
        if(hand_HZ_box)GUI_window_destory(hand_HZ_box);
        GUI_window_hwin_req(m_keyPAD);
	    GUI_set_Bgcolor(C_GUI_BlACK);
		hand_HZ_box=Gui_txtbox_creat(m_keyPAD->win_wide/2+5,5,m_keyPAD->win_wide/2-10,m_keyPAD->win_high/2-5,WIN_FILL|WIN_3D,m_keyPAD);
        GUI_window_hwin_release(m_keyPAD);
		
		GUI_window_hwin_req(hand_HZ_box);
	    GUI_set_Bgcolor(C_GUI_BlACK);
		for(offset=0;offset<10;offset++)
		{
			GUI_memcpy(icon,m_hand_input_HZbuf+offset*2,2);
			Bwide =  pF->CHwide*3+5;
			Bhigh =  pF->high+10;
			xgap  = (hand_HZ_box->win_wide-Bwide*5)/6;
			ygap  = (hand_HZ_box->win_high-Bhigh*3)/4;
			id    = offset;
			x0    = xgap+(offset%5)*(Bwide+xgap);
			y0    = ygap+(offset/5)*(Bhigh+ygap);
			pOBJ  = Gui_button_creat(x0,y0,Bwide,Bhigh,WIN_MSG_EN|BUTTON_PAINT|id,hand_HZ_box);
			GUI_button_title_display(pOBJ,(char*)icon);
		}
		GUI_window_hwin_release(hand_HZ_box);
	}
	GUI_handPen_HZBOX_auto_clean(flag);
	return;
}

void GUI_handPen_check(app_touch_t*pdata)
{ 
	if(pdata->state==0)
	{
		m_last_touch_data.x =0;
		m_last_touch_data.y =0;
		return;
	}
    if(!m_keyPAD)return;
	if(WIDEGE_PAD_HAND!=m_keyPAD->type)return;
	GUI_window_hwin_req(m_keyPAD);
	GUI_handPen_touch_handle(pdata);
	GUI_window_hwin_release(m_keyPAD);
	return ;
} 


Bsize_t GUI_handPen_GB_obtain(char*hzbuf )
{ 
	uint8_t key;
	if(GUI_MsgFIFO_read(m_keyPAD,&key,1)==1)
	{
	   if(key<200)
	   {
		   GUI_memcpy(hzbuf,m_hand_input_HZbuf+key*2,2);
		   SEND_PADMSG(_KEY_OK);
		   return 2;

	   }
	   else if(key<203)
	   {
	       GUI_keypad_type_select(key-200);
	   }
	   else if(key==203)SEND_PADMSG(_KEY_DEL);
	   else if(key==204)SEND_PADMSG(_KEY_ENTER);
	}
	return 0;
}

extern const    char*m_func_tab[];

void  GUI_handPen_creat(hwin_t * pWIN)
{	
	hwin_t * pOBJ;
	uint16_t x0;
	uint16_t y0;
	uint8_t  id;
    int      Bwide,Bhigh,n;

	GUI_FONT*pF = (GUI_FONT*)m_keyPAD_Font;
	pWIN->type = WIDEGE_PAD_HAND;
	GUI_window_hwin_req(pWIN);
	GUI_set_Bgcolor(0);
	hand_area_write=Gui_txtbox_creat(5,5,pWIN->win_wide/2-2,pWIN->win_high-10,WIN_FILL|WIN_3D, pWIN);
	hand_HZ_box    =Gui_txtbox_creat(pWIN->win_wide/2+5,5,pWIN->win_wide/2-10,pWIN->win_high/2-5,WIN_FILL|WIN_3D, pWIN);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
    x0 = pWIN->win_wide/2+30;
	y0 = pWIN->win_high/2+10;
	id = 201;
    n = SIZE_OF_ARRAY(m_func_tab);
    Bwide =  pF->CHwide*n+10;
    Bhigh =  pF->high+10;
	
	pOBJ = Gui_button_creat(x0+100*0,y0,Bwide,Bhigh,WIN_MSG_EN|BUTTON_PAINT|id,pWIN);
	Gui_button_dye(pOBJ,C_GUI_ORANGE,C_GUI_WHITE);
	GUI_button_title_display(pOBJ,(char*)m_func_tab[id-200]);
	id++;
	pOBJ = Gui_button_creat(x0+100*1,y0,Bwide,Bhigh,WIN_MSG_EN|BUTTON_PAINT|id,pWIN);
	Gui_button_dye(pOBJ,C_GUI_ORANGE,C_GUI_WHITE);
	GUI_button_title_display(pOBJ,(char*)m_func_tab[id-200]);
	id++;
    x0 = pWIN->win_wide/2+30;
	y0 = pWIN->win_high/2+60;
	pOBJ = Gui_button_creat(x0+100*0,y0,Bwide,Bhigh,WIN_MSG_EN|BUTTON_PAINT|id,pWIN);
	Gui_button_dye(pOBJ,C_GUI_ORANGE,C_GUI_WHITE);
	GUI_button_title_display(pOBJ,(char*)m_func_tab[id-200]);
	id++;
	pOBJ = Gui_button_creat(x0+100*1,y0,Bwide,Bhigh,WIN_MSG_EN|BUTTON_PAINT|id,pWIN);
	Gui_button_dye(pOBJ,C_GUI_ORANGE,C_GUI_WHITE);
	GUI_button_title_display(pOBJ,(char*)m_func_tab[id-200]);
	GUI_window_hwin_release(pWIN);
    return ;
}
#endif

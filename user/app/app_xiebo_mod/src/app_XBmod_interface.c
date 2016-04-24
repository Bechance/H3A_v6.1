/********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:系统设置模块界面函数集合
* 创建时间:2010-11-19
* 所属公司 :科荟
* 文件名称:app_XBmod_interface.c
* 创建人:揭成
* 功能描述:提供系统设置界面画屏函数API
******************************************************************************************/
#include"app_XBmod_inside.h"
hwin_t*	m_XBmod_win_Obj   = PNULL;
/*****************************************************************
 * @创建人 :揭成
 * @功能 :图形显示
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
#define       XBBLACK   0x10A3
void app_XBmod_graph_display(int len,int n,xb_data_t*pfdata,hwin_t*hp)
{
#define GRAPH_WIDE  31
#define GRAPH_LBR   8
#define GRAPH_GAP   35 
	int x,y;
	int mid;
	//int ltemp;
	float fuge;
	char temp[10]= "";
	fuge = pfdata->Usage;
    if(n<19)
    {
	    x = GRAPH_GAP*(n-2)+GRAPH_LBR;
		y = hp->win_high-30;
    }
    else if(n<36)
    {
	    x = GRAPH_GAP*(n-19)+GRAPH_LBR;
		y = hp->win_high-30;
    }
    else 
    {
	    x = GRAPH_GAP*(n-36)+GRAPH_LBR;
		y = hp->win_high-30;
    }
	GUI_window_hwin_req(hp);	
	GUI_set_Bgcolor(XBBLACK);
    GUI_fill_window_area(x,y-115,x+GRAPH_WIDE,y);
	GUI_set_Bgcolor(C_GUI_GREEN);
	if(len>0)
    	GUI_fill_window_area(x,y-len,x+GRAPH_WIDE,y);
	GUI_set_Fgcolor(C_GUI_WHITE);
	GUI_set_Bgcolor(XBBLACK);
	global_itoa(pfdata->offset,temp);
	GUI_SetFront(&GUI_F8X11_ASCII);
	mid =(GRAPH_WIDE-global_strlen(temp)*6)/2;
	GUI_string_display_At(x+mid,y+2,temp);
	GUI_string_display(" ");
	GUI_set_Fgcolor(C_GUI_WHITE);
	if(fuge == 0)global_strcpy(temp,"0");
	else if(fuge<0.01)global_strcpy(temp,"0.01");
	else if(fuge>10) global_ltoa((long)fuge,temp);
  	else       global_ftoa(fuge,temp,2);
	mid =(GRAPH_WIDE-global_strlen(temp)*6)/2;
	GUI_string_display_At(x+mid,y-len-13,temp);
	GUI_SET_DEFFRONT();
	GUI_window_hwin_release(hp);
	return;
}

void app_XBmod_ruler_clean(void)
{
	int x,y;
	hwin_t*hp = GET_DOBJ(0);
	y = hp->win_high-30+1;
	x = 9;
	GUI_window_hwin_req(hp);	
	GUI_set_Bgcolor(XBBLACK);
    GUI_fill_window_area(x,y,x+hp->win_wide-40,y+15);
	GUI_window_hwin_release(hp);
	hp = GET_DOBJ(1);
	y = hp->win_high-30+1;
	x = 9;
	GUI_window_hwin_req(hp);	
	GUI_set_Bgcolor(XBBLACK);
    GUI_fill_window_area(x,y,x+hp->win_wide-40,y+15);
	GUI_window_hwin_release(hp);
	return;
}

void app_XBmod_THDdisplay(float fuage,hwin_t*hp)
{
	GUI_window_hwin_req(hp);	
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_set_Bgcolor(XBBLACK);
	GUI_set_Fgcolor(C_GUI_YELLOW);
	if(hp== GET_DOBJ(0))
	{
    	GUI_string_display_At(hp->win_wide-20,5,"U");
    	GUI_string_display_At(hp->win_wide/2-50,5,"电压THD(%) = ");
	}
    else
    {
        GUI_string_display_At(hp->win_wide-20,5,"I");
		GUI_string_display_At(hp->win_wide/2-50,5,"电流THD(%) = ");
    }

	GUI_float_display(fuage,3);
	GUI_string_display("          ");
	GUI_window_hwin_release(hp);
    return;
}

static void _GRcallblack(hwin_t*hp)
{
    Bsize_t wide,high;
	GUI_window_hwin_req(hp);
	GUI_set_Fgcolor(C_GUI_WHITE);
	GUI_set_Bgcolor(XBBLACK);
	GUI_clean_window();
	wide  = hp->win_wide;
	high  = hp->win_high;
	GUI_window_angle_line_display(5,high-30,90,high-60,1,"");
	GUI_window_angle_line_display(5,high-30,0 ,wide-5,1,"");
	GUI_set_Fgcolor(C_GUI_WHITE);
    GUI_string_display_At(wide-30+10,high-27, "N");
    GUI_string_display_At(12,25,"(%)");
	GUI_window_hwin_release(hp);	
    return;
}



static void _CTLwin_callback(hwin_t*hp)
{
#define LBR_Y   2
	GUI_window_hwin_req(hp);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_set_Bgcolor(GUI_GetwinBgcor());
	GUI_set_Fgcolor(C_GUI_DLA_COLOR);
	GUI_radio_memeber_add(85+100*0,LBR_Y,"A相",0);
	GUI_set_Fgcolor(C_GUI_DLB_COLOR);
	GUI_radio_memeber_add(85+100*1,LBR_Y,"B相",1);
	GUI_set_Fgcolor(C_GUI_DLC_COLOR);
	GUI_radio_memeber_add(85+100*2+5,LBR_Y,"C相",2);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
    GUI_checkBox_create("排序",XBmod_FG_COLOR,82+100*3+5,0,WIN_TOUCH_EN|GUI_KEY_SELECT_WAY|WIN_MSG,hp);
	GUI_set_Fgcolor(C_GUI_WHITE);
    GUI_string_display_At(85+100*4+5,LBR_Y,"模式切换");
	GUI_window_hwin_release(hp);	
    return;
}
const dialogBox_t  app_XBmod_dialogbox[]=
{
	{WIDGET_TEXTBOX,0,"",5,2  ,630,200, 0,0,0,_GRcallblack},
	{WIDGET_TEXTBOX,1,"",5,207,630,200, 1,0,0,_GRcallblack},
	{WIDGET_RADIO,2,  "",0,412,630,38,  GUI_KEY_SELECT_PHASE|WIN_MSG,0,0,_CTLwin_callback},
};
/*****************************************************************
 * @创建人 :揭成
 * @功能 :图形模式窗口创建
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_XBmod_window_create(void)
{
	GUI_set_Bgcolor(XBmod_WIN_BG_COLOR );
	m_XBmod_win_Obj = GUI_dialogBox_create("谐波测量(图形模式,按F5切换到数字模式)",0,0,LCD_WIDE-2,LCD_HIGH,
	WIN_ALL,app_XBmod_dialogbox,
	SIZE_OF_ARRAY(app_XBmod_dialogbox));
	GUI_MsgFIFO_create(m_XBmod_win_Obj,2);
    return;
}
/*****************************************************************
 * @创建人 :揭成
 * @功能 :图形模式窗口删除
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_XBmod_window_destory(void)
{
	GUI_window_destory( m_XBmod_win_Obj );
    return;
}

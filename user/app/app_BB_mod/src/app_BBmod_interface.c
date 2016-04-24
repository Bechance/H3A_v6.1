/*************************************************************************
*
* 模块名称:变比模块界面函数集合
* 创建时间:
* 所属公司 :科荟
* 文件名称:app_BBmod_interface.c
* 作者 :
* 功能描述:提供变比模块界面画屏函数API
***************************************************************************/
#include "app_BBmod_inside.h"
hwin_t *m_bianbi_win = PNULL;
hwin_t *m_BB_edit = PNULL;

hwin_t* m_BB_droplist_1 = PNULL;
hwin_t* m_BB_droplist_2 = PNULL;

const char *m_bianbi_title[2] = {"CT变比测量", ""};
const char*m_BB_first_choose[4] = {"100A", "500A", "1000A","1500A"};
const char*m_BB_second_choose[3] = {"5A", "20A", "100A "};


/********************************************************************
 * @作者 :揭成
 * @功能 :	编辑框内输出浮点数
 *
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
static void app_BBmod_DL_float_display( Bsize_t id, float data, Bsize_t bit )
{
	char pst[20];
	GUI_edit_select( id );
	GUI_edit_clean();
	if ( GLOBAL_ABS( data ) < 100000 && data != 0 )
	{
		global_ftoa( data, pst, 4 );
		if ( global_strlen( pst ) > 6 )
		{
			pst[6] = ASCII_NULL;
		}
		GUI_edit_string_display( pst );
	}
	return;
}
/**************************************************************************
 * @作者 : 揭成
 * @功能 :变比模块参数显示
 * @输入 :NONE
 * @输出  :NONE
*************************************************************************/
void app_BBmod_DLarg_display( measure_param_t*pDLarg )
{
	float fBB=0;
	GUI_window_hwin_req( m_BB_edit );
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
	GUI_set_Fgcolor( C_GUI_YELLOW );
	app_BBmod_DL_float_display( FIREST_CURRENT, pDLarg->phase_a.f_I, 2 );
	app_BBmod_DL_float_display( SECOND_CURRENT, pDLarg->phase_c.f_I, 2 );
	if(pDLarg->phase_c.f_I!=0)
	{
		fBB = pDLarg->phase_a.f_I / pDLarg->phase_c.f_I;
	}
	if(fBB==0)
	{
		app_BBmod_DL_float_display( XIANGJIAO,0, 2 );
		app_BBmod_DL_float_display( BIANBI,0, 2 );
	}
       else
	{
		app_BBmod_DL_float_display( XIANGJIAO, pDLarg->f_JIac, 2 );
		app_BBmod_DL_float_display( BIANBI, fBB, 2 );
	}
	GUI_window_hwin_release( m_BB_edit );
	return;
}


#define 	BB_GET_YLINE(LINE) 	(40*LINE)

void app_BBmod_state_display(BOOL state)
{
	GUI_window_hwin_req(m_bianbi_win);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
    if(state == 0)
    {
		GUI_set_Bgcolor(BBmod_WIN_BG_COLOR );
		GUI_set_Fgcolor(C_GUI_RED);
		GUI_string_display_At(50,410,"测量已经停止.   ");
    }
    else  
    {
		GUI_set_Bgcolor(BBmod_WIN_BG_COLOR );
		GUI_set_Fgcolor(C_GUI_GREEN);
		GUI_string_display_At(50,410,"正在测量中.....");
	}
	GUI_window_hwin_release(m_bianbi_win);
}


void app_BBmod_DLmember_add(hwin_t*hp)
{
	GUI_window_hwin_req(hp);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_edit_memeber_add(40,   BB_GET_YLINE(1),8, "一次电流", FIREST_CURRENT | MEMBER_3D );
	GUI_edit_memeber_add(40,   BB_GET_YLINE(2),8, "二次电流", SECOND_CURRENT | MEMBER_3D );
	GUI_edit_memeber_add(340,  BB_GET_YLINE(1),8, "    变比", BIANBI | MEMBER_3D );
	GUI_edit_memeber_add(340,  BB_GET_YLINE(2),8, "  相位差", XIANGJIAO | MEMBER_3D );
/*	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_string_display_At(40, BB_GET_YLINE(1)+5, "一次电流");
    GUI_string_display_At(40, BB_GET_YLINE(2)+5, "二次电流");
    GUI_string_display_At(340,BB_GET_YLINE(1)+5, "    变比");
    GUI_string_display_At(340,BB_GET_YLINE(2)+5, "  相位差");*/
	GUI_set_Fgcolor(BBmod_FG_COLOR);
	GUI_WINDIS_VLINE(hp->win_wide/2,2,hp->win_high);
	GUI_window_hwin_release(hp);
	return;
}

const dialogBox_t  app_BBmod_dialogbox[]=
{
    {WIDGET_DROP_LIST,1,"",159,10,0  ,0  ,KEY_F2,m_BB_first_choose,SIZE_OF_ARRAY(m_BB_first_choose),0},
    {WIDGET_DROP_LIST,2,"",409,10,0  ,0  ,KEY_F3,m_BB_second_choose,SIZE_OF_ARRAY(m_BB_second_choose),0},
    {WIDGET_EDIT     ,3,"",2  ,80,LCD_WIDE-4,150,WIN_PAINT|WIN_3D,0,0,app_BBmod_DLmember_add},
};
/**************************************************************************
 * @作者 :
 * @功能 :变比主窗口
 * @输入 :NONE
 * @输出  :NONE
*************************************************************************/
void app_BBmod_window_creat( void )
{
    GUI_window_hwin_req(GET_GUIWIN);
   	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_set_Bgcolor( BBmod_BG_COLOR );
	GUI_clean_screen();
	GUI_set_Bgcolor( BBmod_WIN_BG_COLOR );
	GUI_set_Fgcolor( BBmod_FG_COLOR );
	m_bianbi_win = GUI_dialogBox_create(m_bianbi_title[g_language],0,0,LCD_WIDE-2,LCD_HIGH,
	WIN_3D|WIN_FILL,
	app_BBmod_dialogbox,
	SIZE_OF_ARRAY(app_BBmod_dialogbox));

	GUI_window_hwin_req( m_bianbi_win );
	GUI_set_Fgcolor( BBmod_FG_COLOR );
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_string_display_At(76-17,12, "一次钳表" );
	GUI_string_display_At(326-17,12, "二次钳表" );
	GUI_set_Fgcolor( C_GUI_RED);
	GUI_string_display_At(50,52, "请将一次钳表接入仪器的A相,二次钳表接入仪器的C相." );
	GUI_set_Fgcolor( C_GUI_GREEN);
	GUI_string_display_At(50,245, "键盘操作提示 - ->" );
	GUI_set_Fgcolor( C_GUI_YELLOW);
	GUI_string_display_At(50,245+38*1, "按<F2>选择一次钳表." );
	GUI_string_display_At(50,245+38*2, "按<F3>选择二次钳表." );
	GUI_string_display_At(50,245+38*3, "按<确认>开始测量变比,按<取消>返回主界面.");
	GUI_set_Fgcolor( BBmod_FG_COLOR );
	GUI_WINDIS_HLINE(2,m_bianbi_win->win_wide-2,395);
	GUI_window_hwin_release( m_bianbi_win );
	m_BB_edit = GET_DOBJ(3);
	app_BBmod_state_display(0);
    GUI_window_hwin_release(GET_GUIWIN);
	return;
}


void app_BBmod_window_destroy( void )
{
	GUI_window_destory( m_bianbi_win );
	return;
}
/**************************************************************************
 * @作者 :显示用户信息
 * @功能 :范文斌
 * @输入 :NONE
 * @输出 :NONE
*************************************************************************/
void app_BBmod_user_info_diplay(DL_app_user_all_info_t *pdata)
{
	GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
	GUI_set_Fgcolor( BBmod_FG_COLOR );	
	GUI_window_hwin_req(GET_DOBJ(4));
	GUI_edit_string_display_At(pdata->DL_app_user_info.wrok_list, 1);
	GUI_edit_string_display_At(pdata->DL_app_user_info.user_id,   2);
	GUI_edit_string_display_At(pdata->DL_app_user_info.TQ_id,     3);
	GUI_edit_string_display_At(pdata->DL_app_user_info.time_buf,  4);  
	GUI_window_hwin_release(GET_DOBJ(4));
	return;
}



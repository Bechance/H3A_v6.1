/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:系统设置模块主体程序
* 创建时间:2010-08-02
* 所属公司 :科荟
* 文件名称:app_XBmod_fun.c
* 创建人 :揭成
* 功能描述:实现系统设置功能
******************************************************************************************/
#include	"app_XBmod_inside.h"
#include	<stdlib.h> 
static      int         _CURpage;
static  	a_sem_t*	_sem     = INVALID_SEM;

/*****************************************************************
 * @创建人 :揭成
 * @功能 :图形模式窗口创建
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void _WINpaint(void)
{
    int  xGAP;
	hwin_t*pOBJ;
	GUI_set_Bgcolor(XBmod_WIN_BG_COLOR );
	pOBJ = Gui_3D_window_creat(0,0,LCD_WIDE-2,LCD_HIGH,"谐波测量(数字模式,按F5切换到图形模式)",WIN_ALL,0);
    GUI_window_hwin_req(pOBJ);
	GUI_set_Bgcolor(0x0313);
    GUI_fill_window_area(5,2,pOBJ->win_wide,30);
	xGAP = (pOBJ->win_wide-10)/7;
	GUI_set_Fgcolor(XBmod_FG_COLOR);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
    GUI_string_display_At(5+xGAP*0,10,"谐波次数");
    GUI_string_display_At(5+xGAP*1,10,"Ua(%)");
    GUI_string_display_At(5+xGAP*2,10,"Ia(%)");
    GUI_string_display_At(5+xGAP*3,10,"Ub(%)");
    GUI_string_display_At(5+xGAP*4,10,"Ib(%)");
    GUI_string_display_At(5+xGAP*5,10,"Uc(%)");
    GUI_string_display_At(5+xGAP*6,10,"Ic(%)");
	GUI_WINDIS_VLINE(5+xGAP*1-5,30,pOBJ->win_high);
	GUI_WINDIS_VLINE(5+xGAP*2-5,30,pOBJ->win_high);
	GUI_WINDIS_VLINE(5+xGAP*3-5,30,pOBJ->win_high);
	GUI_WINDIS_VLINE(5+xGAP*4-5,30,pOBJ->win_high);
	GUI_WINDIS_VLINE(5+xGAP*5-5,30,pOBJ->win_high);
	GUI_WINDIS_VLINE(5+xGAP*6-5,30,pOBJ->win_high);
    GUI_window_hwin_release(pOBJ);
	m_XBmod_win_Obj=pOBJ;
	GUI_MsgFIFO_create(m_XBmod_win_Obj,2);
	return;
}

/********************************************************************
 * @创建人 :揭成
 * @功能 :根据需要排列数据
 * @输入 :
 * @输出 :NONE
********************************************************************/
static void _sort_data(xb_data_t*pdata,float*fdata)
{
	Bsize_t   xpos;
	for(xpos=0;xpos<55;xpos++)
	{
		pdata[xpos].offset = xpos;
		if(xpos<52)
		pdata[xpos].Usage  = fdata[xpos];
		else
		pdata[xpos].Usage  = 0;
		if(GLOBAL_ABS(pdata[xpos].Usage)>100)
		pdata[xpos].Usage  = 0;
	}
	return;
}
#define LINE_H  23
static void _XBtimes_view(int start_n,int end_n)
{
    int         pos;
	int         line=2;
	GUI_window_hwin_req(m_XBmod_win_Obj);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_set_Fgcolor(XBmod_FG_COLOR);
	GUI_set_Bgcolor(XBmod_WIN_BG_COLOR);
	for(pos=start_n;pos<end_n;pos++)
	{
	    GUI_int_display_At(5,10+LINE_H*line++,pos);	
		GUI_string_display("  ");
	}
	GUI_window_hwin_release(m_XBmod_win_Obj);

}

static void _XBview(int start_n,int end_n,float pXB[6][52])
{
	xb_data_t   UIxb_data[55];
    int         pos;
    int  		xGAP;
	int         line=2;
	int         phase = 0;
	xGAP = (m_XBmod_win_Obj->win_wide-10)/7;
	GUI_window_hwin_req(m_XBmod_win_Obj);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_set_Fgcolor(XBmod_FG_COLOR);
	GUI_set_Bgcolor(XBmod_WIN_BG_COLOR);
	GUI_string_display_At(5,10+LINE_H*1,"THD%");	
	for(phase=0;phase<6;phase++)
	{
		_sort_data(UIxb_data,pXB[phase]);
		if(GLOBAL_ABS(UIxb_data[0].Usage)<100)
			GUI_float_display_At(5+xGAP*(phase+1),10+LINE_H,UIxb_data[0].Usage,2);
		line=2;
		for(pos=start_n;pos<end_n;pos++)
		{
			if(GLOBAL_ABS(UIxb_data[pos].Usage)<100)
		    	GUI_float_display_At(5+xGAP*(phase+1),10+LINE_H*line,UIxb_data[pos].Usage,2);
			line++;
		}
	}
	GUI_window_hwin_release(m_XBmod_win_Obj);
    return;
}






static void _display_task( void*arg )
{
	Bsize_t 	page = 0;
	int     	start_n = 2,end_n= 19;
	DL_flag_t 	dl_flag;
	float 		XBdata[6][52];
	GLOBAL_MEMCLR(XBdata,sizeof(float)*6*52);
	_WINpaint();
	while(1)
	{
		if ( os_task_delete_req( SELF_PRO ) == OS_TASK_DEL_REQ )
		{
			os_task_delete( SELF_PRO );   			
		}
		if(os_sem_pend(_sem,APP_100_MS*5) == OS_NO_ERR )
		{
		    page       = _GR_CURpage;
			start_n    = 2+17*page;
			end_n      = start_n+17; 
			_XBtimes_view(start_n,end_n);
			os_driver_ioctl(g_DL_dev,MEASURE_CMD_XBDATA_GET,XBdata);
			_XBview(start_n,end_n,XBdata);
		}
		app_global_DLflag_obtain(&dl_flag);
		if(dl_flag.DL_XB_is_new)
		{
			os_driver_ioctl(g_DL_dev,MEASURE_CMD_XBDATA_GET,XBdata);
			_XBview(start_n,end_n,XBdata);
		}
	}
}

void app_XBmod_Numtask( void*arg )
{
	uint8_t key_buf;
	uint8_t id;
	_GR_CURpage = 0;
	id= os_task_assign_stk_build( _display_task,10);
	os_sem_creat(&_sem,1);
	msleep(APP_100_MS);
	while(1)
	{
		msleep(TYPE_DELAY);
		if ( os_task_delete_req( SELF_PRO ) == OS_TASK_DEL_REQ )
		{
			OS_DELETE_REQ( id );
		    app_XBmod_window_destory();
			os_task_delete( SELF_PRO );   			
		}
		if ( GUI_key_read(m_XBmod_win_Obj,&key_buf, 1 ) == 1 ) 
		{
			if(key_buf==KEY_RIGHT)
			{
			    _GR_CURpage = (_GR_CURpage==2)?(0):(_GR_CURpage+1);
 				os_sem_post(_sem);
			}	
			else if(key_buf==KEY_LEFT)
			{
			    _GR_CURpage = (_GR_CURpage==0)?(2):(_GR_CURpage-1);
 				os_sem_post(_sem);
			}	
		}
	}
}

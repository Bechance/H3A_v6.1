/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:变比模块主体程序
* 创建时间:
* 所属公司 :科荟
* 文件名称:app_FILmod_fun.c
* 创建人 :
* 功能描述:实现变比功能
******************************************************************************************/
#include "app_FIL_inside.h"
static  uint8_t   _task_id=INVALID_PRO;
char    m_CUR_flop[50];


static void _txtView(char*path)
{
    hwin_t* pwin;
	char  * pbuf ;
	FIL   * fil;
	char  *p;
	uint8_t key_buf;
	int   y = 5;
	pbuf = app_malloc(1000);
	GUI_window_hwin_req( GET_GUIWIN );
	GUI_set_Bgcolor( 0 );
	GUI_clean_screen();
	GUI_set_Bgcolor( 0 );
	GUI_set_Fgcolor( C_GUI_WHITE);
	pwin = Gui_3D_window_creat( 0, 0, LCD_WIDE - 2, LCD_HIGH, "文本浏览器", WIN_3D | WIN_FILL, 0 );
	GUI_window_hwin_release( GET_GUIWIN ); 
	fil=fatfs_open(path,FA_OPEN_EXISTING|FA_READ|FA_WRITE);
	while(1)
	{
	    p = f_gets(pbuf,500,fil);
		if(!p)
		{
		    while(1)
			{
			    msleep(1);
				if ( app_global_key_obtain( &key_buf, 1 ) == 1 ) 
				{
					 if(key_buf==KEY_EXIT)
					 {
					    fatfs_close(fil);
						app_free(pbuf);
						GUI_window_destory(pwin);
						return;
					 }
				}
		    }
		}
		GUI_window_hwin_req( pwin );
		GUI_set_Bgcolor( 0 );
		GUI_set_Fgcolor( C_GUI_WHITE);
		GUI_SetFront(&GUI_HZK16_EXTASCII);
		GUI_string_display_At(2,y,pbuf);
		GUI_window_hwin_release( pwin );
		y+=GUI_conText.CUR_Front->high;
		if((y+GUI_conText.CUR_Front->high)>pwin->win_high)
		{
		    while(1)
		    {
		        msleep(1);
				if ( app_global_key_obtain( &key_buf, 1 ) == 1 ) 
				{
				     if(key_buf==KEY_DOWN)
				     {
				     	 GUI_window_hwin_req( pwin );
						 GUI_set_Bgcolor( 0 );
						 GUI_set_Fgcolor( C_GUI_WHITE);
					     GUI_clean_window();
			             y = 5;
						 GUI_window_hwin_release( pwin );
						 break;
				     }
					 else if(key_buf==KEY_EXIT)
					 {
						app_free(pbuf);
						GUI_window_destory(pwin);
						return;
					 }
				}
		    }
		}
	}
}
/********************************************************************
* @创建人 :揭成
* @功能 :selfCOR模块主任务
* @输入 :NONE
* @输出 :NONE
********************************************************************/
static void _open_Fview(char*s,int rst)
{
    Fileview_open(s,rst);
	Fileview_setARG(4,4,&GUI_HZK16_EXTASCII);
	return;
}

static void _flop_path_sub(void)
{
    char*p;
	if(m_CUR_flop[0]==0) return;
    p=global_string_locate_end(m_CUR_flop,m_CUR_flop+global_strlen(m_CUR_flop),"/");
    if(p)
    {
		*p = 0;
    }
    return;
}


static void _task(void*arg)
{
    uint8_t key_buf;
	GLOBAL_MEMCLR(m_CUR_flop,50);
	_open_Fview(m_CUR_flop,1);
	while(1)
	{
	    msleep(TYPE_DELAY);
		if ( os_task_delete_req(SELF_PRO) == OS_TASK_DEL_REQ)
		{
			os_task_delete( SELF_PRO );   			
		}
		if ( app_global_key_obtain( &key_buf, 1 ) == 1 ) 
		{
               if(key_buf==KEY_ENTER)
               {
				   if(Fileview_type_obtain()=='d')
				   {
                       Fileview_close();
					   Fileview_path_obtain(m_CUR_flop);
					   _open_Fview(m_CUR_flop,1);
				   }
				   else if(Fileview_type_obtain()=='t')
				   {
                       Fileview_close();
					   Fileview_path_obtain(m_CUR_flop);
					   _txtView(m_CUR_flop);
					   _flop_path_sub();
					   _open_Fview(m_CUR_flop,1);	   
				   }
			   }
			   else if(key_buf==KEY_EXIT)
			   {
			   	  if(m_CUR_flop[0]){
                   Fileview_close();
				   _flop_path_sub();
				   	_open_Fview(m_CUR_flop,1);}
				  else
				  {
					  app_module_switch_last_mod();
				  }
			   }
			   else Fileview_sendMSG(key_buf);
		}
	}
}
/********************************************************************
* @创建人 :揭成
* @功能 :selfCOR模块主功能初始化
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void app_FILmod_fun_entry()
{
	_task_id = os_task_assign_stk_build( _task,8);
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :selfCOR模块主功能释放资源
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_FILmod_fun_exit()
{
	if ( _task_id != INVALID_PRO )
	{
		OS_DELETE_REQ( _task_id );
		_task_id = INVALID_PRO;
	}
	return;
}

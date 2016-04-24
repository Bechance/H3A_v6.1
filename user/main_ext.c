/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:主函数功能
* 创建时间:2010-06-29
* 所属公司:科荟
* 文件名称:main_init.c
* 创建人    :揭成
* 功能描述: 全局资源
*----------------------------------------------------------------
*********************************************************************************************************/
#include "includes.h"

/********************************************************************
 * @创建人:揭成
 * @功能:模块切换逻辑,将该部分放在此是为了更好的
 * 是模块能够编译成库
 *
 * @输入:NONE
 *
 *@输出:NONE
 * 模块移植用户自己编写
********************************************************************/
Bsize_t    	    		g_mod_branch 	= 0;   /*模块分支变量，控制进入模块内不同分支*/
Bsize_t	    			g_select_branch	= 0;  //分支数据选择，=1主测量，=2查线，=3谐波,=4变比，=5读表
uint16_t   	    		g_Fg_color 	    = C_GUI_WHITE;
uint16_t	    		g_Bg_color      = C_GUI_HIGH_BLUE;
uint16_t          		g_win_Bg_color  = 799;
uint16_t	    		g_color_pos     = 1;
dev_id_t          		g_gpio_dev    	= INVALID_DEV_ID;
dev_id_t				g_piccom_dev  	= INVALID_DEV_ID;
dev_id_t				g_UIout_dev   	= INVALID_DEV_ID;
uint8_t					g_language	  	= 0;
dev_id_t 		 		g_spi_dev_id  	= INVALID_DEV_ID;
DL_app_user_all_info_t  g_user_arg 		= {0, };
measure_param_t  		g_DL_param 		= {0, };
dev_id_t            	g_DL_dev   		= INVALID_DEV_ID;
uint8_t 				m_protect 	  	= IDLE;
/********************************************************************
 * @创建人:揭成
 * @功能:界面颜色选择
 * @输入:NONE
 * @输出:NONE
********************************************************************/
void GUI_scrb_SetDeuCor(uint16_t Bg,uint16_t Fg);
void app_module_color_select(uint8_t flags)
{
	if(flags==1)
	{
		g_Bg_color = C_GUI_HIGH_BLUE;
		g_Fg_color = C_GUI_WHITE;
		g_win_Bg_color = 799;
		Gui_button_set_color(BUTTON_TXT_COLOR,BUTTON_BG_COLOR);
		Gui_3D_window_set_color(C_GUI_WHITE,C_GUI_LOW_BLUE,C_GUI_WHITE);	
		Gui_drop_list_set_color(C_GUI_BlACK, 1151);
	}
	else if(flags==2)
	{
		g_Bg_color     = 0;
		g_Fg_color     = C_GUI_WHITE;
		g_win_Bg_color = 0x0313;
		Gui_button_set_color(C_GUI_WHITE,g_win_Bg_color);
		Gui_3D_window_set_color(C_GUI_WHITE,0X022E,C_GUI_WHITE);	
		Gui_drop_list_set_color(0x0313, g_win_Bg_color);
		GUI_edit_member_set_color(C_GUI_WHITE,0x10A3);
		GUI_3D_widow_setFornt(&GUI_HZK24_EXTASCII);
		GUI_scrb_SetDeuCor(C_GUI_WHITE,g_win_Bg_color);
	}
	else
	{
		g_Bg_color = C_GUI_BlACK;
		g_Fg_color = C_GUI_BlACK;
		g_win_Bg_color = LCD_Color2Index_565(0xc0c0c0);	
		Gui_button_set_color(C_GUI_LOW_BLUE,LCD_Color2Index_565(0xd3d3d3));
		Gui_3D_window_set_color(C_GUI_WHITE,LCD_Color2Index_565(0x808080),C_GUI_RED);	
		Gui_drop_list_set_color(0, LCD_Color2Index_565(0xc0c0c0));
	}	
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能   :连击事件
 * @输入   :NONE
 * @输出   :NONE
********************************************************************/
BOOL app_continuous_event(int32_t *jiffies,int32_t*times)
{
    if(++(*times)==2)
    { 
        if((OSTimeGet()-(*jiffies))<50)
        {
	        *times    = 0;
	        *jiffies  = OSTimeGet();
			return TURE;
        }
		else
		{
		    *times    = 1;
	        *jiffies  = OSTimeGet();
		}
    }
	else
	{
		    *times   = 1;
	        *jiffies = OSTimeGet();
	}
	return FAIL;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :主模块从驱动获取测量参数
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_global_DLarg_obtain( void*pDLarg )
{
	os_driver_ioctl(g_DL_dev,MEASURE_CMD_DL_PARAM_GET, pDLarg);
	return;
}
void app_global_path1_DLerr_obtain( void*pDLarg )
{
	os_driver_ioctl(g_DL_dev,MEASURE_CMD_PATH1_ERR_GET, pDLarg);
	return;
}
void app_global_path2_DLerr_obtain( void*pDLarg )
{
	os_driver_ioctl(g_DL_dev,MEASURE_CMD_PATH2_ERR_GET, pDLarg);
	return;
}
void app_global_path3_DLerr_obtain( void*pDLarg )
{
	os_driver_ioctl(g_DL_dev,MEASURE_CMD_PATH3_ERR_GET, pDLarg);
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :读取BUF标志，获取电力参数并显示
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_global_DLflag_obtain( void*pDL_flag)
{
	os_driver_ioctl(g_DL_dev,MEASURE_CMD_DLFLAG_GET,pDL_flag);
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :读取BUF标志,获取电力参数并显示
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_global_set_user_dl_arg( void*arg)
{
    os_driver_ioctl(g_DL_dev,MEASURE_CMD_USER_DL_ARG_SET,arg);
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :选择钳表
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_global_QB_select( uint8_t A_dlsr,uint8_t B_dlsr,uint8_t C_dlsr )
{
   uint8_t QB[3];
   QB[0] = A_dlsr;
   QB[1] = B_dlsr;
   QB[2] = C_dlsr;
   os_driver_ioctl(g_DL_dev,MEASURE_CMD_USER_QB_SELECT,QB);
   return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :获取通道一误差圈数
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
Bsize_t app_global_path1_circle_obtain( void )
{
	Bsize_t circle;
	os_driver_ioctl( g_DL_dev, MEASURE_CMD_PATH1_ERR_CIRCLE_GET, &circle );
	return circle;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :获取通道二误差圈数
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
Bsize_t app_global_path2_circle_obtain( void )
{
	Bsize_t circle;
	os_driver_ioctl( g_DL_dev, MEASURE_CMD_PATH2_ERR_CIRCLE_GET, &circle );
	return circle;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :获取通道三误差圈数
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
Bsize_t app_global_path3_circle_obtain( void )
{
	Bsize_t circle;
	os_driver_ioctl( g_DL_dev, MEASURE_CMD_PATH3_ERR_CIRCLE_GET, &circle );
	return circle;
}

void app_global_DL_measure_entery( void )
{
    BUS_WRITE(0,0X4402);//选择FPGA内部时钟为DA频率。
	os_driver_ioctl( g_DL_dev, MEASURE_CMD_START, 0 );
	return;
}

void app_global_DL_measure_exit( void )
{
	os_driver_ioctl( g_DL_dev, MEASURE_CMD_STOP, 0 );
	return;
}

/********************************************************************
 * @创建人 :揭成
 * @功能 :加载测量功能
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void app_global_DL_driver_add( void )
{
	GLOBAL_MEMCLR( &g_DL_param, sizeof( measure_param_t ) );
	GLOBAL_MEMCLR( &g_user_arg, sizeof( DL_app_user_all_info_t ) );	
	g_DL_dev = measure_driver_register(); 			
	if ( g_DL_dev != INVALID_DEV_ID )
	{
		os_driver_init( g_DL_dev );      			
	}
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :退出测量功能
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void app_global_DL_driver_delete( void )
{
	if ( g_DL_dev != INVALID_DEV_ID )
	{
		os_driver_uninstall( g_DL_dev );
		g_DL_dev = INVALID_DEV_ID;
	}
	return;
}

/********************************************************************
 * @创建人:揭成
 * @功能:申请公共资源
 *
 * @输入:arg
 *
 *@输出:NONE
********************************************************************/
void app_global_resouce_req( void*arg )
{
	if ( m_protect == IDLE )
	{
		m_protect = BUSY;
		GUI_resource_req();
	}
	return;
}


/************************************************************/
//存储
//*************************************************************/
const  char*m_XCERRmod_MSG_25[2]={"工单号 ", ""};
const  char*m_XCERRmod_MSG_251[2]={"用户号 ", ""};
const  char*m_XCERRmod_MSG_26[2]={"用户名 ", ""};
const  char*m_XCERRmod_MSG_27[2]={"电表号1", ""};
const  char*m_XCERRmod_MSG_28[2]={"电表号2", ""};
const  char*m_XCERRmod_MSG_29[2]={"校验员1", ""};
const  char*m_XCERRmod_MSG_30[2]={"  时间 ", ""};
const  char*m_XCERRmod_MSG_31[2]={"确认", ""};
const  char*m_XCERRmod_MSG_32[2]={"退出", ""};
const  char*m_XCERRmod_MSG_36[2]={"用户设置", ""};

const  char*m_PECD_MSG_1[2]={"正在保存请稍后...", ""};
const  char*m_PECD_MSG_2[2]={"数据已经成功保存.", ""};
const  char*m_PECD_MSG_3[2]={"数据保存失败.", ""};



static       hwin_t*	 _Save_edit;
static void _save_WINpaint(hwin_t*  pOBJ)
{
#define M_GET_YLINE(LINE) 	(10 + 40*LINE)
	hwin_t*edit,*p;
	edit = GUI_edit_creat(5,5,pOBJ->win_wide-10,pOBJ->win_high-10,WIN_TOUCH_EN,pOBJ);
	GUI_window_hwin_req(edit);
	GUI_set_Bgcolor(0xce79);
	GUI_set_Fgcolor(0);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_edit_memeber_add( 10, M_GET_YLINE( 0 ),30,(char*)m_XCERRmod_MSG_25[g_language], MEMBER_3D | 0 );
	GUI_edit_memeber_add( 10, M_GET_YLINE( 1 ),30,(char*)m_XCERRmod_MSG_251[g_language], MEMBER_3D | 1 );
	GUI_edit_memeber_add( 10, M_GET_YLINE( 2 ),30,(char*)m_XCERRmod_MSG_26[g_language], MEMBER_3D | 2 );
	GUI_edit_memeber_add( 10, M_GET_YLINE( 3 ),30,(char*)m_XCERRmod_MSG_27[g_language], MEMBER_3D | 3 );
	GUI_edit_memeber_add( 10, M_GET_YLINE( 4 ),30,(char*)m_XCERRmod_MSG_28[g_language], MEMBER_3D | 4 );
	GUI_edit_memeber_add( 10, M_GET_YLINE( 5 ),30,(char*)m_XCERRmod_MSG_29[g_language], MEMBER_3D | 5 );
	GUI_edit_memeber_add( 10, M_GET_YLINE( 6 ),30,(char*)m_XCERRmod_MSG_30[g_language], MEMBER_3D | 6 );
	GUI_SetFront(&GUI_HZK16_EXTASCII);
    p=Gui_button_creat((pOBJ->win_wide-160)/3,pOBJ->win_high-35,70,25,WIN_MSG_EN|BUTTON_PAINT|KEY_ENTER,pOBJ);
	GUI_button_title_display(p,(char*)m_XCERRmod_MSG_31[g_language]);
    p=Gui_button_creat(2*(pOBJ->win_wide-160)/3+80,pOBJ->win_high-35,70,25,WIN_MSG_EN|BUTTON_PAINT|KEY_EXIT,pOBJ);
	GUI_button_title_display(p,(char*)m_XCERRmod_MSG_32[g_language]);
	GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
	GUI_set_Fgcolor( C_GUI_WHITE);		
	GUI_edit_string_display_At(g_user_arg.DL_app_user_info.time_buf,6);
	GUI_window_hwin_release(edit);
	_Save_edit = edit;
	return;
}

static void _WINview(DL_app_user_all_info_t*pU)
{
	GUI_window_hwin_req(_Save_edit);
	GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
	GUI_set_Fgcolor( C_GUI_WHITE);	
	GUI_edit_string_display_At(pU->DL_app_user_info.wrok_list,0);
	GUI_edit_string_display_At(pU->DL_app_user_info.user_id,1);
	GUI_edit_string_display_At(pU->DL_app_user_info.user_name,2);
	GUI_edit_string_display_At(pU->DL_app_user_info.meter_id1,3);
	GUI_edit_string_display_At(pU->DL_app_user_info.meter_id2,4);
	GUI_edit_string_display_At(pU->DL_app_user_info.worker,5);
	GUI_window_hwin_release(_Save_edit);
	return;
}

extern Bsize_t USB_HID_data_obtain(void*pdata);
static Bsize_t _get_content_from_USB_scanner(char *pshadow,Bsize_t edit_length)
{
	uint16_t data_len = 0;
	uint16_t real_len = 0;
	uint16_t copy_size = 0;
	char *pmalloc  = app_malloc(256);
	if(pmalloc == PNULL) return FAIL;
	GLOBAL_MEMCLR(pmalloc, 256);
	data_len = USB_HID_data_obtain(pmalloc);
	if(data_len != 0)
	{
		global_string_filter(pmalloc, '\n');
		global_string_filter(pmalloc, '\r');
		real_len = global_strlen(pmalloc);
		copy_size = edit_length < real_len?edit_length:real_len;
		global_memcpy(pshadow, pmalloc, copy_size);
		pshadow[copy_size]=ASCII_NULL;
		app_free(pmalloc);
		return copy_size;
	}
	app_free(pmalloc);
	return 0;
}

extern hwin_t*         		m_cur_hwin;
extern USB_dev_state_u*		pUSBstate;

static void _fill_edit_by_USB_scanner(hwin_t *hp,DL_app_user_all_info_t*pU)
{
	Bsize_t  member=0;
	
	if(!hp) return;
	if(!pU) return;
	
	GUI_window_hwin_req( hp );
	GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
	GUI_set_Fgcolor( C_GUI_WHITE);
	
	member = GUI_edit_id_obtain();
	if(member==0)
	{	
		if(_get_content_from_USB_scanner(pU->DL_app_user_info.wrok_list,m_cur_hwin->subord_cur->eidt_lenth) != 0)
			GUI_edit_string_display_At( pU->DL_app_user_info.wrok_list, GUI_edit_id_obtain() );
	}
	else if(member==1)
	{
		if( _get_content_from_USB_scanner(pU->DL_app_user_info.user_id,m_cur_hwin->subord_cur->eidt_lenth) != 0)
			GUI_edit_string_display_At( pU->DL_app_user_info.user_id, GUI_edit_id_obtain() );
	}
	else if(member==2)
	{	
		if( _get_content_from_USB_scanner(pU->DL_app_user_info.user_name,m_cur_hwin->subord_cur->eidt_lenth)!= 0)		
			GUI_edit_string_display_At( pU->DL_app_user_info.user_name, GUI_edit_id_obtain() );
	}
	else if(member==3)
	{
		if( _get_content_from_USB_scanner(pU->DL_app_user_info.meter_id1,m_cur_hwin->subord_cur->eidt_lenth) != 0)
			GUI_edit_string_display_At( pU->DL_app_user_info.meter_id1, GUI_edit_id_obtain() );
	}
	else if(member==4)
	{	
		if( _get_content_from_USB_scanner(pU->DL_app_user_info.meter_id2,m_cur_hwin->subord_cur->eidt_lenth)!= 0)
			GUI_edit_string_display_At( pU->DL_app_user_info.meter_id2, GUI_edit_id_obtain() );
	}
	else if(member==5)
	{
		if( _get_content_from_USB_scanner(pU->DL_app_user_info.worker,m_cur_hwin->subord_cur->eidt_lenth)!= 0)
			GUI_edit_string_display_At( pU->DL_app_user_info.worker, GUI_edit_id_obtain() );
	}
	GUI_window_hwin_release( hp );
}

static uint8_t _Save_setting(hwin_t*hp)
{
	uint8_t  key;
	Bsize_t  member;
	DL_app_user_all_info_t*pU;
	hand_wirte_input_box_pos_set(100,380);
    pU = app_malloc(sizeof(DL_app_user_all_info_t));
	global_memcpy(pU,&g_user_arg,sizeof(DL_app_user_all_info_t));
	while(1)
	{
	    msleep(1);
		
		_fill_edit_by_USB_scanner(hp,pU);
		
		if ( app_global_key_obtain( &key, 1 ) != 1 )
		{
			continue;
		}
		GUI_window_hwin_req( hp );
		GUI_edit_run_hook( key );
		GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
		GUI_set_Fgcolor( C_GUI_WHITE);		
		if ( app_global_decide_inputkey( key ) != FAIL )
		{
			member = GUI_edit_id_obtain();	
            if(member==0)key_method_input_shadow(key,pU->DL_app_user_info.wrok_list);
			else if(member==1)key_method_input_shadow(key,pU->DL_app_user_info.user_id);
			else if(member==2)key_method_input_shadow(key,pU->DL_app_user_info.user_name);			
            else if(member==3)key_method_input_shadow(key,pU->DL_app_user_info.meter_id1);
            else if(member==4)key_method_input_shadow(key,pU->DL_app_user_info.meter_id2);
            else if(member==5)key_method_input_shadow(key,pU->DL_app_user_info.worker);
		}
		else if(key == KEY_DOWN)GUI_edit_shift( EDIT_NEXT);	
		else if(key == KEY_UP)  GUI_edit_shift( EDIT_LAST );	
		GUI_window_hwin_release( hp );
	    if (key == KEY_EXIT)
		{
			break;
		}
	    else if(key == KEY_ENTER)
		{
			global_memcpy(&g_user_arg,pU,sizeof(DL_app_user_all_info_t));
			break;
		}		
		else if(key == KEY_F3)
		{
			GUI_window_hwin_req( hp );
			member = GUI_edit_id_obtain();	
            if(member==0)GLOBAL_MEMCLR(pU->DL_app_user_info.wrok_list,UER_DSIZE);
			else if(member==1)GLOBAL_MEMCLR(pU->DL_app_user_info.user_id,UER_DSIZE);
			else if(member==2)GLOBAL_MEMCLR(pU->DL_app_user_info.user_name,UER_DSIZE);			
            else if(member==3)GLOBAL_MEMCLR(pU->DL_app_user_info.meter_id1,UER_DSIZE);
            else if(member==4)GLOBAL_MEMCLR(pU->DL_app_user_info.meter_id2,UER_DSIZE);
            else if(member==5)GLOBAL_MEMCLR(pU->DL_app_user_info.worker,UER_DSIZE);
			GUI_edit_clean();
			GUI_cursor_reload( m_edit_csr );
			GUI_edit_select(member);
			GUI_window_hwin_release( hp );
		}
	}
	app_free(pU);
    return key;
}

uint8_t app_USERinfo_Save(void)
{
    int x1,y1,x2,y2;
	Bsize_t  sec;
	hwin_t*  pOBJ ;
	uint8_t  key;
	x1 = 130;y1 =2;x2 = x1+370;y2 = y1+350;
	sec=GUI_read_block_Extp(x1,y1,x2, y2);
	GUI_window_hwin_req(GET_GUIWIN);
	GUI_3D_widow_setFornt(&GUI_HZK16_EXTASCII);
	Gui_button_set_color(C_GUI_LOW_BLUE,0xce79);
	Gui_3D_window_set_color(C_GUI_WHITE,C_GUI_LOW_BLUE,C_GUI_RED);
	GUI_set_Bgcolor(0xce79);
    pOBJ = Gui_3D_window_creat(x1+1,y1+1,(x2-x1-8),(y2-y1-5),(char*)m_XCERRmod_MSG_36[g_language],WIN_MSG_EN|WIN_3D|WIN_FILL,0);
    _save_WINpaint(pOBJ);
	GUI_TopWIN_Set(pOBJ);
	GUI_window_hwin_release(GET_GUIWIN);
	_WINview(&g_user_arg);
	key = GUI_edit_start(_Save_edit,_Save_setting);
	_WINview(&g_user_arg);
	Gui_button_set_color(BUTTON_TXT_COLOR,BUTTON_BG_COLOR);
	Gui_3D_window_set_color(C_GUI_WHITE,C_GUI_LOW_BLUE,C_GUI_WHITE);
	GUI_3D_widow_setFornt(&GUI_HZK24_EXTASCII);
	GUI_window_destory(pOBJ);
	GUI_write_block_Extp(x1,y1,x2, y2, sec);
    return key;
}

/*******************************************************************
 * @作者 : 
 * @功能 : 获取日期与日期
 * @输入 : NONE
 * @输出 : NONE
 * @时间格式: 2014-01-04 15:32:11(备注:日期与时间之间只能有1个空格，不然
              存储溢出了)
********************************************************************/
void app_Systime_obtain( void)
{
	dev_id_t 	  dev;
	time_string_t ctime;
    char*pch  = g_user_arg.DL_app_user_info.time_buf;
	dev = os_driver_open( "TIME" );
	os_driver_ioctl( dev, TMR_CMD_TIME_STRING_GET, &ctime );
    GLOBAL_MEMCLR(pch,20);
    global_strcat(pch,"20");	
    global_strcat(pch,ctime.year_buf);	
    global_strcat(pch,"-");
    global_strcat(pch,ctime.month_buf);
    global_strcat(pch,"-");
    global_strcat(pch,ctime.day_buf);
	global_strcat(pch, " " );
    global_strcat(pch,ctime.hour_buf);
	global_strcat(pch, ":" );
	global_strcat(pch, ctime.min_buf );
	global_strcat(pch, ":" );
	global_strcat(pch, ctime.sec_buf );
	return;
}

/***************************************************************/
// SAVE
/***************************************************************/
static char _mIDbuf[40]="";
static void _create_DIRtime(char*path)
{
	time_string_t ctime;
	dev_id_t dev;
	dev = os_driver_open( "TIME" );
	os_driver_ioctl( dev, TMR_CMD_TIME_STRING_GET,&ctime);
    global_strcat(path,ctime.year_buf);	
    global_strcat(path,ctime.month_buf);
    global_strcat(path,ctime.day_buf);
	return;
}

static BOOL _create_listDIR(FIL*fOBJ,char*mID,char*path)
{
	Slist_t   	 list;
	char         CHn[10]  = "";
	char         name[40] = "";
	UINT         wr; 
	FRESULT 	 ret;
	FIL          *fp;
	global_strcpy(path,path);
	if(!fOBJ)return FAIL;
	while(1)
	{
		ret=f_read(fOBJ,&list,sizeof(Slist_t),&wr);
		if(ret!=FR_OK) return FAIL;
		if(wr==0)break;
		if(global_memcmp(mID,list.mID,global_strlen(mID)))
		{
		    global_ltoa(list.n, CHn);
			global_strcat(path,"/");
			global_strcat(path,CHn);
			global_strcpy(name, path);
			global_strcat(name,"/");
			global_strcat(name,"info");		
			fp=fatfs_open(name,FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
		    f_write(fp,&g_user_arg.DL_app_user_info,sizeof(DL_app_user_info_t),&wr);
			fatfs_close(fp);	
			return TURE;
		}
	}
	GLOBAL_MEMCLR(&list,sizeof(Slist_t));
	if(fOBJ->fsize)
	{
		f_lseek(fOBJ, fOBJ->fsize-sizeof(Slist_t));
		ret=f_read(fOBJ,&list,sizeof(Slist_t),&wr);
		list.n++;GLOBAL_MEMCLR(list.mID,30);
	}
	global_strcpy(list.mID,mID);
	f_lseek(fOBJ, fOBJ->fsize);
	f_write(fOBJ,&list,sizeof(Slist_t),&wr);
    global_ltoa(list.n, CHn);
	global_strcat(path,"/");
	global_strcat(path,CHn);
    if(f_mkdir(path)!=FR_OK) return FAIL;
	global_strcpy(name, path);
	global_strcat(name,"/");
	global_strcat(name,"info");
    fp=fatfs_open(name,FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
    f_write(fp,&g_user_arg.DL_app_user_info,sizeof(DL_app_user_info_t),&wr);
	fatfs_close(fp);
	return TURE;
}

static FIL* app_create_DIRFIL(char*mID,char*File,int*pstate)
{
    char name[30]="";
	char path[30]="PECD/";
	FIL* fOBJ;
	FRESULT ret;
	DIR    dir;
	BOOL   state;
	*pstate  = 0;
   	ret = f_opendir(&dir,"PECD");
    if(ret==FR_NO_PATH)
    {
        if(f_mkdir("PECD")!=FR_OK)
		   return FAIL;
	}
	_create_DIRtime(path);
	if(File[0]==0)return 0;
	if(path[0]==0)return 0;
   	ret = f_opendir(&dir,path);
    if(ret==FR_NO_PATH)
    {
        if(f_mkdir(path)!=FR_OK)
		   return FAIL;
	}
	global_strcpy(name,(char*)path);
	global_strcat(name,"/");
	global_strcat(name,(char*)"list");
    fOBJ=fatfs_open(name,FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
	if(fOBJ)
	{
        state=_create_listDIR(fOBJ,mID,path);
        fatfs_close(fOBJ);
		if(!state) return 0;
	    global_strcat(path,"/");
		global_strcat(path,File);
		if(fatfs_file_ISexist(path))
		{
			GUI_waitWIN_destory();
			if ( MsgBox( 200, 200, 250, 120, 
				( char* )"提示", 
				( char* )"今天已经记录过该终端数据,是否继续存储.",
				APP_HALF_SEC*100 ) == GUIE_KEY_ENTER)
			{
				GUI_waitWIN_create((char*)m_PECD_MSG_1[g_language]);
				*pstate = 1;
	        	return 0;
			}
			GUI_waitWIN_create((char*)m_PECD_MSG_1[g_language]);
			return 0;
		}
		fOBJ=fatfs_open(path,FA_CREATE_ALWAYS|FA_WRITE|FA_READ);
		return fOBJ;
	}
	return 0;
}

void app_save_PECD(const char*file,const void*p,int size)
{
	UINT nbyte;
	FIL*fp;
	FRESULT    ret = FR_DISK_ERR;
    int        state;
	global_strcpy(_mIDbuf,g_user_arg.DL_app_user_info.meter_id1);
	GUI_waitWIN_create((char*)m_PECD_MSG_1[g_language]);
	fp=app_create_DIRFIL(_mIDbuf,(char*)file,&state);
    if(!fp) 
    {
        if(state==0)
        {
	        GUI_waitWIN_CHmod((char*)m_PECD_MSG_3[g_language]);
			msleep(APP_100_MS*10);
		    GUI_waitWIN_destory();
			return;
        }
		else
		{
		    app_Systime_obtain();
			global_strcat(_mIDbuf,"_");
			global_strcat(_mIDbuf,&g_user_arg.DL_app_user_info.time_buf[14]);
		    fp=app_create_DIRFIL(_mIDbuf,(char*)file,&state);
			if(!fp)
			{
		        GUI_waitWIN_CHmod((char*)m_PECD_MSG_3[g_language]);
				msleep(APP_100_MS*10);
			    GUI_waitWIN_destory();
				return;
			}
		}
    }
	ret = f_write(fp,p,size,&nbyte);
	fatfs_close(fp);
    if(ret==FR_OK)
    {
		GUI_waitWIN_CHmod((char*)m_PECD_MSG_2[g_language]);
    }
	else
	{
		GUI_waitWIN_CHmod((char*)m_PECD_MSG_3[g_language]);
	}
	msleep(APP_100_MS*10);
	GUI_waitWIN_destory();
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能:查看帮助文件
 * @输入:arg
 * @输出:NONE
********************************************************************/
void app_help_view(void)
{
    char *name = app_malloc(20);
    char *path = app_malloc(40);
	GLOBAL_MEMCLR(name,20);
	GLOBAL_MEMCLR(path,40);
    app_module_CURmod_OB(name);
	global_strcat(path,"help/");
	global_strcat(path,name);
	global_strcat(path,".TXT");
	if(fatfs_file_ISexist((const char*)path))
	{
		File_txtView(path,"帮助");
	}
	app_free(name);
	app_free(path);
	return;
}

/********************************************
//导入外部程序
*********************************************/
static		uint16_t 	_Bsector;
static		hwin_t*  	_ExpWIN;
static		hwin_t*  	_Expprogress_bar;

#define  	EXPWIN_WIDE   110
#define  	EXPWIN_HIGH   40
#define  	EXPWIN_BAR    80

static void _Infoview( char*notice )
{
	GUI_window_hwin_req( _ExpWIN );
	GUI_set_Bgcolor( C_GUI_DARK_GRAY );
	GUI_set_Fgcolor( C_GUI_RED );
	GUI_SetFront( &GUI_HZK16_EXTASCII );
	GUI_string_display_At( 5, _Expprogress_bar->win_high + 5, notice );
	GUI_window_hwin_release( _ExpWIN );
	return;
}

static void _progressBar_view( FIL*fp )
{
	uint16_t  vlue;
	uint16_t  Mvlue;
	Mvlue 	= EXPWIN_BAR;
	vlue 	= Mvlue * fp->fptr / fp->fsize;
	Gui_progress_bar_setValue( _Expprogress_bar, vlue );
	return;
}

static void _WIN_create( void )
{
	int x1, y1, x2, y2;
	x1 = LCD_WIDE / 2 - 50;
	y1 = LCD_HIGH / 2 - 30;
	x2 = x1 + EXPWIN_WIDE;
	y2 = y1 + EXPWIN_HIGH;

	_Bsector=_Bsector;
	_Bsector = GUI_read_block_Extp( x1, y1, x2, y2 );
	GUI_window_hwin_req( GET_GUIWIN );
	GUI_set_Bgcolor( C_GUI_DARK_GRAY );
	_ExpWIN = GUI_window_creat( x1 + 1, y1 + 1, EXPWIN_WIDE - 2, EXPWIN_HIGH - 2, WIN_3D | WIN_FILL );
	GUI_TopWIN_Set( _ExpWIN );
	GUI_window_hwin_req( _ExpWIN );
	_Expprogress_bar = Gui_progress_bar_creat( 5, 2, _ExpWIN->win_wide - 10, 15, EXPWIN_BAR, _ExpWIN );
	GUI_window_hwin_release( _ExpWIN );
	GUI_window_hwin_release( GET_GUIWIN );
	return;
}

static void _read_EXE( FIL *fp, const void *buff, UINT btw, UINT *bw )
{
	char *pst;
	char *pMem;
	Bsize_t offset;
	uint16_t itemp;
	pMem = app_malloc( btw * 2 );
	if ( !pMem ) return ;
	pst = pMem;
	f_read( fp, pst, btw*2, bw );
	( *bw ) /= 2;
	for ( offset = 0;offset < ( *bw );offset++ )
	{
		itemp = ( *pst ) | (( *( pst + 1 ) ) << 8 );
		pst += 2;
		(( char* )buff )[offset] = itemp;
	}
	app_free( pMem );
	return;
}

void  app_load_eEXE( const char*file )
{
	uint16_t*phead = ( uint16_t* )0x140000;
	UINT    w;
	FIL*fp;
	char    name[20] = "";
	global_strcpy( name, "EXP/" );
	global_strcat( name, ( char* )file );
	fp = fatfs_open( name, FA_OPEN_EXISTING | FA_READ );
	if (fp)
	{
		_WIN_create();
		_Infoview( "正在打开.." );
		while ( 1 )
		{
			if (( fp->fsize - fp->fptr ) > 512 )
			{
				_read_EXE( fp, phead, 512, &w );
				_progressBar_view( fp );
				phead += 512;
			}
			else
			{
				fatfs_read_bytes( fp, phead, ( fp->fsize - fp->fptr ), &w );
				_progressBar_view( fp );
				fatfs_close( fp );
				asm( "  lb 0x140000" );
			}
		}
	}
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能:释放公共资源
 *
 * @输入:arg
 *
 *@输出:NONE
********************************************************************/
void app_global_resouce_release( void*arg )
{
	m_protect=IDLE;
	GUI_resource_release();
	app_release_all_mem();
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能:初始化共享试?
 *
 * @输入:NONE
 *
 *@输出:NONE
********************************************************************/
void app_global_resouce_init( void )
{
	g_gpio_dev   = os_driver_open("GPIO");
	g_spi_dev_id = os_driver_open("SPI");
	g_piccom_dev = os_driver_open("EXTINT"); 
    app_global_buz_entery();
	key_input_method_init();
	fatfs_init();
	FS_req();
	app_global_DL_driver_add();
	app_global_resouce_req( 0 );
	g_UIout_dev  = os_driver_open("UIout");
	//开机将输出常数恢复成36000
	fatfs_readARG("USERDATA",&g_user_arg,sizeof(DL_app_user_all_info_t));
	global_strcpy(g_user_arg.DL_app_user_info.data_order,"36000");
	fatfs_writeARG("USERDATA",&g_user_arg,sizeof(DL_app_user_all_info_t));	
	return;
}

/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:系统设置模块主体程序
* 创建时间:2012-04-01
* 所属公司 :科荟
* 文件名称:app_ERRmod_Ucorrect.c
* 作者 :揭成
* 功能描述:时钟误差修正功能
******************************************************************************************/
#include	"app_ERRmod_inside.h"
#define TMR_QS       1
#define TMR_CS       2

extern  dev_id_t	 g_piccom_dev;
static  char         m_TMR1_const[10]="1.0";
static  float        m_TMR1_err[4];
static  float        m_ERRmod_TMR_K;
/********************************************************************
 * @作者 :揭成
 * @功能 :误差圈数显示
 * @输入 :
 * @输出 :NONE
********************************************************************/
void app_ERRmod_TMRcircle_display(Bsize_t circle,hwin_t*hp)
{
    char ctemp[10]="";
	if(!hp)return;
	GUI_window_hwin_req(hp);
    GUI_set_Bgcolor(C_GUI_BlACK);
	global_itoa(circle,ctemp);
	global_strcat(ctemp,"  ");
    GUI_SetFront(&GUI_F16X24_ASCII);
    GUI_set_Fgcolor(C_GUI_RED);
	GUI_string_display_At(10,30,ctemp);
	GUI_window_hwin_release(hp);
	return;
}
/********************************************************************
 * @作者 :揭成
 * @功能 :误差显示
 * @输入 :
 * @输出 :NONE
********************************************************************/
static app_ERRmod_Err_ftoa(float err,char*str)
{
   if(GLOBAL_ABS(err)<1)global_ftoa(err,str,5);
   else if(GLOBAL_ABS(err)<100)global_ftoa(err,str,2);
   else global_strcpy(str,"100.0");
   return;
}
void app_ERRmod_TMRErr_display(float*perr,hwin_t*hp)
{
    char ctemp[10]="";
	Bsize_t len;
	if(!hp)return;
	GUI_window_hwin_req(hp);
    GUI_set_Bgcolor(C_GUI_BlACK);
	GUI_set_Fgcolor(C_GUI_GREEN);
	app_ERRmod_Err_ftoa(perr[0],ctemp);
	global_strcat(ctemp,"%");
    len=global_strlen(ctemp);
	if(len<=9){len=9-len;
	while(len>0){global_strcat(ctemp," ");len --;}}
    GUI_SetFront(&GUI_F16X32_ASCII);
    GUI_set_Fgcolor(C_GUI_GREEN);
	GUI_string_display_At(2,60,ctemp);	
	GUI_set_Fgcolor(C_GUI_YELLOW);
    GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_fill_window_area(78,2,78+9*8,42+15);
	app_ERRmod_Err_ftoa(perr[3],ctemp);
    GUI_string_display_At(80,2,ctemp);
	app_ERRmod_Err_ftoa(perr[2],ctemp);
    GUI_string_display_At(80,22,ctemp);
	app_ERRmod_Err_ftoa(perr[1],ctemp);
    GUI_string_display_At(80,42,ctemp);
	GUI_window_hwin_release(hp);
	return;
}


static float app_ERRmod_TMR_K_obtain(void)
{
    FIL*fp;
	float TMR_K;
	UINT r;
	FRESULT ret;
	fp = fatfs_open("TMRk",FA_OPEN_EXISTING|FA_READ);
    if(fp)
    {
        ret=fatfs_read_bytes(fp,&TMR_K,sizeof(float),&r);
        fatfs_close(fp);
		if(ret==FR_OK)return TMR_K;
		return 1;
	}
    return 1;
}

static void app_ERRmod_TMR_K_del(void)
{
	f_unlink("TMRk");
	return;
}

static int app_ERRmod_TMR_K_save(float TMR_K)
{
    FIL*fp;
	UINT r;
	FRESULT ret;
	fp = fatfs_open("TMRk",FA_OPEN_ALWAYS|FA_WRITE);
    if(fp)
    {
        ret=fatfs_write_bytes(fp,&TMR_K,sizeof(float),&r);
        fatfs_close(fp);
		if(ret==FR_OK)return 1;
		return 0;
	}
    return 0;
}


static void app_ERRmod_TMR_hanlde(int*circle,int*last_circle,hwin_t**err_win)
{
#define  GET_TMR_FLAG     0
#define  GET_TMR_ERR      1
    BOOL flag[3];
    uint32_t TMR_count;
	float    real=0.0,meter=0.0;
	float    TMR_const,err;
	int      plus_N;
	circle[0] = app_global_path1_circle_obtain();
	if(circle[0]!=last_circle[0])
	{
		last_circle[0] = circle[0];
		app_ERRmod_TMRcircle_display(circle[0],err_win[0]);
	}
    os_driver_ioctl(g_piccom_dev,GET_TMR_FLAG,flag);
	if(flag[0])
	{
    	os_driver_ioctl(g_piccom_dev,GET_TMR_ERR,&TMR_count);
		TMR_const = global_atof(m_TMR1_const);
		plus_N    = global_atoi(g_user_arg.DL_app_user_set.qs);
		meter     = TMR_const*plus_N*48000000*m_ERRmod_TMR_K;
		real      = TMR_count;
		err       = (meter-real)/real*100.0;
		m_TMR1_err[3] = m_TMR1_err[2];
		m_TMR1_err[2] = m_TMR1_err[1];
		m_TMR1_err[1] = m_TMR1_err[0];
		m_TMR1_err[0] = err;
		buz_response( APP_100_MS*2);
		app_ERRmod_TMRErr_display(m_TMR1_err,err_win[0]);
	}
    return;
}


void app_ERRmod_TMR_member_add(hwin_t*hobj,hwin_t**edit,hwin_t**err)
{
    hwin_t*edit_win;
    hwin_t*err_win;
    const GUI_FONT*pFONT;
    GUI_window_hwin_req(hobj);
	GUI_set_Bgcolor(ERRmod_WIN_BG_COLOR);
    edit_win = GUI_edit_creat(120,10,hobj->win_wide,300,WIN_TOUCH_EN, hobj);
    GUI_window_hwin_req(edit_win);
    GUI_SetFront(&GUI_HZK24_EXTASCII);
	pFONT = GUI_GetFront();
	GUI_set_Bgcolor(ERRmod_WIN_BG_COLOR);
	GUI_set_Fgcolor(ERRmod_FG_COLOR);
    GUI_edit_memeber_add(100,10+(pFONT->high+15)*0,8,"时间常数(s)",MEMBER_3D|TMR_CS);
    GUI_edit_memeber_add(100,10+(pFONT->high+15)*1,8,"设置圈数(n)",MEMBER_3D|TMR_QS);
    err_win = Gui_txtbox_creat(100,90,170,100,WIN_3D,edit_win);
    GUI_window_hwin_req(err_win);
    GUI_set_Bgcolor(C_GUI_BlACK);
	GUI_clean_window();
    GUI_SetFront(&GUI_F16X24_ASCII);
    GUI_set_Fgcolor(C_GUI_WHITE);
	GUI_string_display_At(2,2," Err:");
    GUI_window_hwin_release(err_win);
    GUI_window_hwin_release(edit_win); 
    GUI_window_hwin_release(hobj); 
    *err  = err_win;
	*edit = edit_win;
	return;
}
/********************************************************************
 * @作者 :揭成
 * @功能 :电力测量电表设置相关参数显示
 * @输入 :
 * @输出 :NONE
********************************************************************/
void app_ERRmod_meterARG_view(hwin_t*hp)
{
	DL_app_user_set_t* pARG = &g_user_arg.DL_app_user_set;;
	GUI_window_hwin_req( hp );
	GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
	GUI_set_Fgcolor( ERRmod_FG_COLOR );	
	GUI_edit_string_display_At(pARG->qs,TMR_QS);
	GUI_edit_string_display_At(m_TMR1_const,TMR_CS);
	GUI_window_hwin_release( hp );
	return;
}
/********************************************************************
 * @作者 :揭成
 * @功能 :电力测量电表参数设置回调
 * @输入 :
 * @输出 :NONE
********************************************************************/
uint8_t app_ERRmod_TMR_setting(hwin_t*hp)
{
	uint8_t  key;
	Bsize_t  member;
	DL_app_user_set_t plocal;
    global_memcpy(&plocal,&g_user_arg.DL_app_user_set,sizeof(DL_app_user_set_t));
	hand_wirte_input_box_pos_set(200,250);
	key_input_method_filter(KEY_INPUT_123);
	while(1)
	{
		msleep( TYPE_DELAY );
		
		if ( app_global_key_force_obtain( &key, 1 ) != 1 )/*判断驱动是否收到按键响应*/
		{
			continue;
		}
		GUI_window_hwin_req( hp );
		GUI_edit_run_hook( key );
		GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
		GUI_set_Fgcolor( ERRmod_FG_COLOR );		
		if ( app_global_decide_inputkey( key ) != FAIL )
		{
			member = GUI_edit_id_obtain();
			if ( member == TMR_QS) key_method_input_shadow( key,plocal.qs);
			else if ( member == TMR_CS) key_method_input_shadow( key,m_TMR1_const);			
		}
		else if(key == KEY_DOWN)GUI_edit_shift(EDIT_NEXT);	
		else if(key == KEY_UP)  GUI_edit_shift(EDIT_LAST);	
		GUI_window_hwin_release( hp );
		if ( key == GUI_KEY_ENTER|| key == KEY_ENTER)
		{
    		global_memcpy(&g_user_arg.DL_app_user_set,&plocal,sizeof(DL_app_user_set_t));
			fatfs_writeARG("USERDATA",&g_user_arg,sizeof(DL_app_user_all_info_t));
			app_global_set_user_dl_arg(&g_user_arg);
			break;
		}
		else if ( key == GUI_KEY_EXIT|| key == KEY_EXIT)
		{
			break;
		}
	}
    return 0;
}
/********************************************************************
 * @作者 :揭成
 * @功能 :电压修正
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void app_ERRmod_TMRcorrect_task(void*arg)
{
	uint8_t  key_buf;
	hwin_t	 *edit,*err_win;
	int      circle      = 0;
	int 	 last_circle = 0;
	app_ERRmod_TMRwindow_creat();
	fatfs_readARG("USERDATA",&g_user_arg,sizeof(DL_app_user_all_info_t));
	app_global_set_user_dl_arg(&g_user_arg);
	app_ERRmod_TMR_member_add(m_errmod_TMRcorrect_win,&edit,&err_win);
	m_ERRmod_TMR_K =   app_ERRmod_TMR_K_obtain();
	app_ERRmod_meterARG_view(edit);
	while ( 1 )
	{
		msleep( TYPE_DELAY );
		app_ERRmod_TMR_hanlde(&circle,&last_circle,&err_win);
		if ( app_global_key_obtain( &key_buf, 1 ) != 1 )
		{
			continue;
		}
		if(key_buf==KEY_EXIT || key_buf==GUI_KEY_EXIT)
		{
           app_ERRmod_TMRwindow_destroy();
		   break;
		}
		else if(key_buf==KEY_SETTING || key_buf==GUI_KEY_SET)
		{
			GUI_edit_start(edit,app_ERRmod_TMR_setting);
			app_ERRmod_meterARG_view(edit);
		}
		else if(key_buf==KEY_F5)
		{
			if(MsgBox(250,100,300,150, "提示","确认删除时钟误差的修正数据",APP_HALF_SEC*100)==GUIE_KEY_ENTER)
			{
				GUI_set_Fgcolor(ERRmod_FG_COLOR);
				GUI_3D_window_title_mod( m_errmod_TMRcorrect_win, "正在删除,请稍后...." );
                app_ERRmod_TMR_K_del();
				msleep( APP_HALF_SEC );
				GUI_3D_window_title_mod( m_errmod_TMRcorrect_win, "成功删除修正数据!" );
				msleep( APP_HALF_SEC );
				GUI_3D_window_title_mod( m_errmod_TMRcorrect_win, "时钟误差修正" );
				
			}
		}
		else if(key_buf==KEY_ENTER || key_buf==GUI_KEY_ENTER)
		{
			GUI_set_Fgcolor(ERRmod_FG_COLOR);
			GUI_3D_window_title_mod( m_errmod_TMRcorrect_win, "开始修正....." );
            m_ERRmod_TMR_K = m_ERRmod_TMR_K-m_TMR1_err[0]/100;
            app_ERRmod_TMR_K_save(m_ERRmod_TMR_K);
			msleep( APP_HALF_SEC );
			GUI_3D_window_title_mod( m_errmod_TMRcorrect_win, "本次修正完成" );
			msleep( APP_HALF_SEC );
			GUI_3D_window_title_mod( m_errmod_TMRcorrect_win, "时钟误差修正" );
		}
	}
}

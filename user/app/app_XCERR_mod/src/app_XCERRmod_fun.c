/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:变比模块主体程序
* 创建时间:
* 所属公司:科荟
* 文件名称:app_XCerrmod_fun.c
* 创建人    :
* 功能描述:实现变比功能
******************************************************************************************/
#include    "app_XCerrmod_inside.h"
static       uint8_t     _task_id	= INVALID_PRO;
dev_id_t	m_XCERRmod_print_dev_id = INVALID_PRO;

void 	app_power_switch_req( void );
uint8_t app_USERinfo_Save(void);   
/********************************************************************
* @作者 :周炳权
* @功能 :电源切换处理
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void app_power_switch_handle( void )
{
	//提示:判断有无外部电源然后再切换
	if ( (g_power_state == PS_BATTERY || g_power_state == PS_PA_BATTERY) && (g_cur_power_up == 0) )
	{
		MsgBox(200,130,250,200,(char*)m_XCERRmod_MSG_17[g_language],(char*)m_XCERRmod_MSG_18[g_language],APP_HALF_SEC*20);
		return;
	}
	else if ( (g_power_state == PS_BATTERY) && (g_cur_power_up == 1) )
	{
		if ( (g_DL_param.phase_a.f_U > 57) || (g_DL_param.phase_b.f_U > 57) || (g_DL_param.phase_c.f_U > 57))
		{}//在没有适配器插入的时候,通过判断端子是否有电压来确定是否切换到开关电源供电
		else
		{
			MsgBox(200,130,250,200,(char*)m_XCERRmod_MSG_17[g_language],(char*)m_XCERRmod_MSG_19[g_language],APP_HALF_SEC*20);
			return;
		}
	}
	else if ( (g_power_state == PS_PA_BATTERY) && (g_cur_power_up == 1) )
	{
		MsgBox(200,130,250,200,(char*)m_XCERRmod_MSG_17[g_language],(char*)m_XCERRmod_MSG_20[g_language],APP_HALF_SEC*20);
		return;
	}
	else if ( (g_power_state == PS_ADAPTER || g_power_state == PS_ADAPTER_EXTRAL ) 
				&& (g_cur_power_up == 0) )
	{
		MsgBox(200,130,250,200,(char*)m_XCERRmod_MSG_17[g_language],(char*)m_XCERRmod_MSG_21[g_language],APP_HALF_SEC*20);
		return;
	}
	else if ( (g_power_state == PS_EXTRAL || g_power_state == PS_PA_EXTRAL ) 
				&& (g_cur_power_up == 0) )
	{
		MsgBox(200,130,250,200,(char*)m_XCERRmod_MSG_17[g_language],(char*)m_XCERRmod_MSG_22[g_language],APP_HALF_SEC*20);
		return;
	}

	if ( (g_cur_power_up == 0) && 
	(MsgBox(200,130,250,200,(char*)m_XCERRmod_MSG_17[g_language],(char*)m_XCERRmod_MSG_23[g_language],APP_HALF_SEC*20)==GUIE_KEY_ENTER))
	{
		app_power_switch_req();
	}
	if ( (g_cur_power_up == 1) && 
	(MsgBox(200,130,250,200,(char*)m_XCERRmod_MSG_17[g_language],(char*)m_XCERRmod_MSG_24[g_language],APP_HALF_SEC*20)==GUIE_KEY_ENTER))
	{	
		app_power_switch_req();
	}
	return;
}




static void _SaveData_load(Errsave_t *p)
{
	measure_param_t*pDL;
	DL_app_param_str_t*pDES;
	char pSTR[20] = "";
	char chSTR[2]="";
	float*pfdata;
	char*pch;
	Bsize_t offset;
	dev_id_t     sht1;
	float        fdata[3];
	sht1 = os_driver_open( "SHT11" );
	GLOBAL_MEMCLR(p, sizeof(Errsave_t));
	os_driver_ioctl(sht1,0,&fdata);
	global_ftoa(fdata[1], g_user_arg.DL_app_user_info.temp_buf, 2);
	global_ftoa(fdata[0], g_user_arg.DL_app_user_info.hnm_buf, 2);
	global_memcpy(&p->DL_app_user_set, &g_user_arg.DL_app_user_set,sizeof(DL_app_user_set_t));
	global_memcpy(&p->DL_app_user_info,&g_user_arg.DL_app_user_info,sizeof(DL_app_user_info_t));
	pDL =&g_DL_param;
	pDES = &p->DLarg;
	for(offset=0;offset <(sizeof(measure_param_t)/sizeof(float)-1);offset++ )
	{
		pfdata =&(pDL->phase_a.f_U );
		pch    =(pDES->phase_a.c_U);
		pfdata = pfdata+offset;
		pch += ((DL_DSIZE)* offset );
		GLOBAL_MEMCLR(pSTR, 10);
		global_ftoa(( *pfdata ),pSTR,4);
		strncpy( pch, pSTR, DL_DSIZE);
		if ( pch[DL_DSIZE-2] == '.' )
		{
			pch[DL_DSIZE-2] = ASCII_NULL;
		}
		pch[DL_DSIZE-1] = ASCII_NULL;
	}
	GLOBAL_MEMCLR(pDES->c_Cos, DL_DSIZE);
	global_ftoa(pDL->f_Cos,pDES->c_Cos,2);
	chSTR[0] = pDL->cos_LC;
	chSTR[1] = ASCII_NULL;
	global_strcat(pDES->c_Cos,chSTR);
	return;
}

static  void _Save_file(void)
{
	Errsave_t *p;
	p = app_malloc(sizeof(Errsave_t));
	if(p == PNULL) return;
	GLOBAL_MEMCLR(p, sizeof(Errsave_t));
    _SaveData_load(p);
	app_save_PECD("XCERR",p,sizeof(Errsave_t));
	app_free(p);
    return;
}

/***************************************************
//模块切换
***************************************************/
static void _key_SWmod_Exc( uint8_t key_buf )
{
	if ( key_buf == KEY_1 )
	{
		app_module_assignExt( "BB" );
	}
	else if ( key_buf == KEY_2 )
	{
		app_module_assignExt( "YDWIR" );
	}
	else if ( key_buf == KEY_3 )
	{
		app_module_assignExt( "XB" );
	}
	else if ( key_buf == KEY_4 )
	{
		app_module_assignExt( "ZOUZI" );
	}
	else if ( key_buf == KEY_5 )
	{
///		app_module_assignExt( "PTCT" ); //文伟要求屏蔽； 
	}
	else if ( key_buf == KEY_8 )
	{
		app_module_assignExt( "WAVE" );
	}
	else if(key_buf == KEY_SYS)
	{
		app_module_assignExt( "MENU" );
	}
	else if(key_buf == KEY_ADD_SUB)
	{
		app_module_assignExt( "RMETER" );
	}
	else if ( key_buf == KEY_SELF )
	{
	    if(MsgPasswordBox( 200,150,"1234"))
	    {
			app_load_eEXE( "SELF" );
		}
	}
	else if ( key_buf == KEY_SEARCH)
	{
		app_module_assignExt( "SEARCH" );
	}
	else if ( key_buf == KEY_6 )
	{
		app_load_eEXE( "USB" );
	}
	else if(key_buf == KEY_9)
	{
		app_load_eEXE( "COMMCTL" );
	}
	return;
}
/********************************************************************
* @创建人 :
* @功能 :主模块按键任务
* @输入 :NONE
* @输出 :NONE
********************************************************************/
static void _task(void*arg)
{
	uint8_t  key_buf;
	uint8_t  DLtask,Errtask;    
	int      cnt = 28;
	fatfs_readARG("USERDATA",&g_user_arg,sizeof(DL_app_user_all_info_t));
	if(g_user_arg.DL_app_user_info.data_order[0]==0)
	{ 
	   global_strcpy(g_user_arg.DL_app_user_info.data_order,"36000");
	}
    app_global_set_user_dl_arg(&g_user_arg);
    Errtask=os_task_assign_stk_build(app_ERR_task,3);
    DLtask =os_task_assign_stk_build(app_DLview_task,3);
	while(1)
	{   
	    msleep(TYPE_DELAY);
		if ( os_task_delete_req( SELF_PRO ) == OS_TASK_DEL_REQ )
		{
			OS_DELETE_REQ( DLtask );
			OS_DELETE_REQ( Errtask );
			os_task_delete( SELF_PRO );   			
		}
		
		if(++cnt%20==0) {app_XCERR_external_display();app_XCERR_charge_display();}
		if(++cnt%30==0)	{app_Systime_obtain();}
		if(++cnt%55==0) {app_XCERR_battery_display();}

		app_XCERR_adapter_protect_display();
		
		if ( app_global_key_obtain( &key_buf, 1 ) == 1 ) 
		{
			  if(key_buf==KEY_SAVE)
			  {
                if(app_USERinfo_Save()==KEY_ENTER)
                {
					_Save_file();
					fatfs_writeARG("USERDATA",&g_user_arg,sizeof(DL_app_user_all_info_t));
                }
			  }	
			  else if(key_buf==KEY_USER)
			  {
                if(app_USERinfo_Save()==KEY_ENTER)
                {
					fatfs_writeARG("USERDATA",&g_user_arg,sizeof(DL_app_user_all_info_t));
                }
			  }
			  else if(key_buf==KEY_SETTING)
			  {
                GUI_keyMSG_post(m_XCERR_ERR_win,KEY_CONST_SETTING);
			  }
			  else if(key_buf==KEY_F1)
			  {
                GUI_keyMSG_post(m_XCERR_ERR_win,KEY_I_IN);
			  }
			  else if(key_buf==KEY_F2)
			  {
                GUI_keyMSG_post(m_XCERR_ERR_win,KEY_QS_SELECT);
			  }
			  else if(key_buf==KEY_F3)
			  {
                GUI_keyMSG_post(m_XCERR_ERR_win,KEY_CONST_SELECT);
			  }
			  else if(key_buf==KEY_0)
			  {
                GUI_keyMSG_post(m_XCERR_ERR_win,KEY_CS_CHECK);
			  }
			  else if (key_buf==KEY_F4)
			  {
			  	app_power_switch_handle();
			  }
			  else if(key_buf==KEY_F5)
			  {
                GUI_keyMSG_post(m_XCERR_ERR_win,KEY_PAUSE);
			  }
			  else if(key_buf==KEY_RIGHT||key_buf==KEY_LEFT||key_buf==KEY_ENTER)
			  {
                GUI_keyMSG_post(m_XCERR_ERR_win,key_buf);
			  }
			  else if(key_buf == KEY_7)
			  {
			  	 app_XCERRmod_Stime_entery();
			  }
			  _key_SWmod_Exc(key_buf);
		}
	}		
}
/********************************************************************
 * @创建人 :背光设置
 * @功能   :进入时间设置
 * @输入   :NONE
 * @输出   :NONE
********************************************************************/
static void _lcd_BLK(void)
{
#define CMD_LCD_BLK_B         0x408F
#define CMD_LCD_BLK_L         0x4090
	int   lcdBLK_ALL = 469;
	int   lcdBLK_B ;
	int   lcdBLK_L ;
	int   max 		 = 20;
	float fuage;
	int   uage       = 10;
	fatfs_readARG("LCDblk",&uage,sizeof(int));
	fuage = (float)uage/(float)max;
    lcdBLK_B = (int)((float)lcdBLK_ALL*(1.0-fuage));
    lcdBLK_L = (int)((float)lcdBLK_ALL*(fuage));
	BUS_WRITE(lcdBLK_L,CMD_LCD_BLK_L);
	BUS_WRITE(lcdBLK_B,CMD_LCD_BLK_B);
    return;
}
/********************************************************************
* @创建人 :
* @功能 :模块主功能初始化
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void app_XCerrmod_fun_entry()
{
	uint32_t arg;
	
	app_XCerrmod_window_creat();
	app_global_DL_measure_entery();
	
	m_XCERRmod_print_dev_id = os_driver_open(XCERRMOD_UART_DEV_NAME);
	arg = XCERRMOD_BAUD;
	os_driver_ioctl(m_XCERRmod_print_dev_id, SET_SERIAL_BAUD, &arg);
	arg = XCERRMOD_UART_NO_CHECHK;
	os_driver_ioctl(m_XCERRmod_print_dev_id, SET_SERIAL_CHECK,&arg);
	os_driver_ioctl(m_XCERRmod_print_dev_id, SET_EMPTY_FIFO, 0);
	
	_lcd_BLK();
	_task_id = os_task_assign_stk_build( _task,4);
	return;
}
/********************************************************************
 * @创建人 :
 * @功能 :test模块主功能释放资源
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_XCerrmod_fun_exit()
{
	if ( _task_id != INVALID_PRO )
	{
		OS_DELETE_REQ( _task_id );
		_task_id = INVALID_PRO;
	}
	app_global_DL_measure_exit();
	return;
}

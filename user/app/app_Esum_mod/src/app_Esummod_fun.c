/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:电能累积对外连接层
* 创建时间:2013-05-11
* 所属公司:科荟
* 文件名称:app_Esummod_connect.c
* 创建人  :揭成
* 功能描述:
*-------------------------------------------------------------------*/
#include		    "app_Esummod_inside.h"
static uint8_t      _EstaskID = INVALID_PRO;
static hwin_t*	 	_Save_edit;

static void _save_WINpaint(hwin_t*  pOBJ)
{
#define M_GET_YLINE(LINE) 	(10 + 40*LINE)
	hwin_t*edit,*p;
	edit = GUI_edit_creat(5,5,pOBJ->win_wide-10,pOBJ->win_high-10,WIN_TOUCH_EN,pOBJ);
	GUI_window_hwin_req(edit);
	GUI_set_Bgcolor(0xce79);
	GUI_set_Fgcolor(0);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_edit_memeber_add( 10, M_GET_YLINE( 0 ),30, "工单号 ", MEMBER_3D | 0 );
	GUI_edit_memeber_add( 10, M_GET_YLINE( 1 ),30, "用户名 ", MEMBER_3D | 1 );
	GUI_edit_memeber_add( 10, M_GET_YLINE( 2 ),30, "电表号1", MEMBER_3D | 2 );
	GUI_edit_memeber_add( 10, M_GET_YLINE( 2 ),30, "电表号2", MEMBER_3D | 3 );
	GUI_edit_memeber_add( 10, M_GET_YLINE( 4 ),30, "校验员1", MEMBER_3D | 4 );
	GUI_edit_memeber_add( 10, M_GET_YLINE( 5 ),30, "  时间",  MEMBER_3D | 5 );
	GUI_SetFront(&GUI_HZK16_EXTASCII);
    p=Gui_button_creat((pOBJ->win_wide-160)/3,pOBJ->win_high-40,70,25,WIN_MSG_EN|BUTTON_PAINT|KEY_ENTER,pOBJ);
	GUI_button_title_display(p,"确认");
    p=Gui_button_creat(2*(pOBJ->win_wide-160)/3+80,pOBJ->win_high-40,70,25,WIN_MSG_EN|BUTTON_PAINT|KEY_EXIT,pOBJ);
	GUI_button_title_display(p,"退出");
	GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
	GUI_set_Fgcolor( C_GUI_WHITE);		
	GUI_edit_string_display_At(g_user_arg.DL_app_user_info.time_buf,5);
	GUI_window_hwin_release(edit);
	_Save_edit = edit;
	return;
}

static void _WINview(DL_app_user_all_info_t*pU)
{
	GUI_window_hwin_req(_Save_edit);
	GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
	GUI_set_Fgcolor( C_GUI_WHITE);	
	GUI_edit_string_display_At(pU->DL_app_user_info.user_id,  0);
	GUI_edit_string_display_At(pU->DL_app_user_info.user_name,1);
	GUI_edit_string_display_At(pU->DL_app_user_info.meter_id1,2);
	GUI_edit_string_display_At(pU->DL_app_user_info.meter_id2,3);
	GUI_edit_string_display_At(pU->DL_app_user_info.worker,4);
	GUI_window_hwin_release(_Save_edit);
	return;
}

static void _SaveData_load(Esum_save_arg_t *p)
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
	
	GLOBAL_MEMCLR(p, sizeof(Esum_save_arg_t));
	sht1 = os_driver_open( "SHT11" );
	GLOBAL_MEMCLR(p, sizeof(Errsave_t));
	os_driver_ioctl(sht1,0,&fdata);
	global_ftoa(fdata[1], g_user_arg.DL_app_user_info.temp_buf, 2);
	global_ftoa(fdata[0], g_user_arg.DL_app_user_info.hnm_buf, 2);
	
	global_memcpy(&p->Esum_arg,&m_Esum_arg,sizeof(Esum_arg_t));
	global_memcpy(&p->DL_app_user_info,&g_user_arg.DL_app_user_info,sizeof(DL_app_user_info_t));
	pDL =&g_DL_param;
	pDES = &p->DLarg;
	for(offset=0;offset <(sizeof(measure_param_t)/sizeof(float)-1);offset++ )
	{
		pfdata =&(pDL->phase_a.f_U );
		pch    =(pDES->phase_a.c_U);
		pfdata = pfdata+offset;
		pch += ((DL_DSIZE)* offset );
		GLOBAL_MEMCLR(pSTR, 20);
		global_ftoa((*pfdata ),pSTR,4);
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
	UINT nbyte;
	FIL*fp;
	Esum_save_arg_t *p;
	FRESULT    ret = FR_DISK_ERR;
	p = app_malloc(sizeof(Esum_save_arg_t));
	fp=fatfs_open("ESUM",FA_OPEN_ALWAYS|FA_WRITE);
	if(fp)
	{
        _SaveData_load(p);
		f_lseek(fp,fp->fsize);
        ret = f_write(fp,p,sizeof(Esum_save_arg_t),&nbyte);
		fatfs_close(fp);
	}
	app_free(p);
    if(ret==FR_OK)
    {
		MsgBox(150,100,240,100,"提示","数据已经成功保存.",APP_HALF_SEC*2);
    }
	else
	{
		MsgBox(150,100,240,100,"提示","数据保存失败.",APP_HALF_SEC*2);
	}
    return;
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
            if(member==0)key_method_input_shadow(key,pU->DL_app_user_info.user_id);
            else if(member==1)key_method_input_shadow(key,pU->DL_app_user_info.user_name);			
            else if(member==2)key_method_input_shadow(key,pU->DL_app_user_info.meter_id1);
            else if(member==3)key_method_input_shadow(key,pU->DL_app_user_info.meter_id2);
            else if(member==4)key_method_input_shadow(key,pU->DL_app_user_info.worker);
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
	}
    return key;
}

static uint8_t _Save(void)
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
    pOBJ = Gui_3D_window_creat(x1+1,y1+1,(x2-x1-8),(y2-y1-5),"用户设置",WIN_MSG_EN|WIN_3D|WIN_FILL,0);
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
	app_module_color_select(2);
    return key;
}


static uint8_t _BL_setting(hwin_t*hp)
{
	uint8_t  key;
	Bsize_t  member;
	char     pU[10];
	hand_wirte_input_box_pos_set(100,380);
	global_memcpy(pU,m_Esum_arg.BL,10);
	while(1)
	{
	    msleep(1);
		if ( app_global_key_obtain( &key, 1 ) != 1 )
		{
			continue;
		}
		GUI_window_hwin_req( hp );
		GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
		GUI_set_Fgcolor( C_GUI_WHITE);		
		if ( app_global_decide_inputkey( key ) != FAIL )
		{
			member = GUI_edit_id_obtain();	
            if(member==12)key_method_input_shadow(key,pU);
		}	
		GUI_window_hwin_release( hp );
	    if (key == KEY_EXIT)
		{
			break;
		}
	    else if(key == KEY_ENTER)
		{
			global_memcpy(m_Esum_arg.BL,pU,10);
			break;
		}		
	}
    return key;
}


static  void _clean_arg(void)
{
    char ch[10] = "";
   	global_memcpy(ch,m_Esum_arg.BL,10);
	GLOBAL_MEMCLR(&m_Esum_arg,sizeof(Esum_arg_t));
   	global_memcpy(m_Esum_arg.BL,ch,10);
	app_Esumarg_display(&m_Esum_arg);
    return;
}

static void _DLstr_to_DLf(DL_app_param_str_t*pSrc,measure_param_t*pDes)
{
	float*  pfdata;
	char*   pch;
	Bsize_t offset;
	GLOBAL_MEMCLR(pDes, sizeof(measure_param_t));
	for(offset=0;offset <(sizeof(measure_param_t)/sizeof(float)-1);offset++ )
	{
		pfdata =&(pDes->phase_a.f_U );
		pch    = (pSrc->phase_a.c_U);
		pfdata =  pfdata+offset;
		pch   += ((DL_DSIZE)*offset);
		global_string_filter(pch,' ');
	   *pfdata = global_atof(pch);
	}
	return;
}

void app_Esummod_record_display(void*data)
{
    uint8_t key;
	Esum_save_arg_t*p = (Esum_save_arg_t*)data;
	app_Esummod_window_creat();
	app_Esumarg_display(&p->Esum_arg);
	_DLstr_to_DLf(&p->DLarg,&g_DL_param);
	app_Esummod_DLarg_display(&g_DL_param);
    while(1)
    {
        msleep(1);
		if ( app_global_key_obtain( &key, 1 )== 1 )
		{
			if(key==KEY_EXIT)break;
		}
	}
    app_Esummod_window_destroy();
    return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :WIFI发送结果
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
static void _Esum_task( void*arg )
{
	uint8_t  key_buf;
	uint8_t  DLtask;
	uint8_t  ESUMtask = INVALID_PRO;
	dev_id_t dev;
	int      cnt = 28;
	app_Esummod_window_creat();
	fatfs_readARG("USERDATA",&g_user_arg,sizeof(DL_app_user_all_info_t));
    app_global_set_user_dl_arg(&g_user_arg);
    DLtask =os_task_assign_stk_build(app_Esummod_DLview_task,3);
	dev = os_driver_open( "TIME" );
	global_strcpy(m_Esum_arg.BL,"50");
	_clean_arg();
	while ( 1 )
	{
	    msleep(TYPE_DELAY);
		if ( os_task_delete_req( SELF_PRO ) == OS_TASK_DEL_REQ )
		{
			OS_DELETE_REQ( DLtask );
			if(ESUMtask!=INVALID_PRO)
			{
					OS_DELETE_REQ( ESUMtask );
					ESUMtask = INVALID_PRO;
			}
			os_task_delete( SELF_PRO );   			
		}
		if(++cnt%30==0)	{app_Systime_obtain();}
		if ( app_global_key_obtain( &key_buf,1)== 1) 
		{
			if(key_buf==KEY_SAVE)
			{
				if(_Save()==KEY_ENTER)
				{
					_Save_file();
					fatfs_writeARG("USERDATA",&g_user_arg,sizeof(DL_app_user_all_info_t));
				}
			}
			else if(key_buf==KEY_SETTING)
			{
			    if(ESUMtask==INVALID_PRO)
			    {
					GUI_edit_start(m_Esummod_edit,_BL_setting);
			    }
			}
			else if(key_buf==KEY_F1)
			{
			    if(ESUMtask==INVALID_PRO)
			   	{
			   		_clean_arg();
               		ESUMtask = os_task_assign_stk_build(app_Esummod_SUM_task,3); 
			   	}
			}
			else if(key_buf==KEY_F2)
			{
			   if(ESUMtask!=INVALID_PRO)
			   {
               		OS_DELETE_REQ( ESUMtask );
			   		ESUMtask = INVALID_PRO;
			   }
			}		
			else if(key_buf==KEY_F3)
			{
               if(ESUMtask==INVALID_PRO)
               {
					_clean_arg();
			   }
			}
			else if(key_buf==KEY_F4)
			{
				app_Esummod_window_destroy();
				OSTaskSuspend(DLtask);
                app_upload_main();
				app_Esummod_window_creat();
				_clean_arg();
				OSTaskResume(DLtask);				
			}
			else if(key_buf==KEY_EXIT)
			{
                app_module_assignExt("START");
			}
			else if(key_buf==KEY_SEARCH)
			{
				if(ESUMtask==INVALID_PRO)
				{
					app_Esummod_window_destroy();
					OSTaskSuspend(DLtask);
					app_Esummod_search(0);
					app_Esummod_window_creat();
					_clean_arg();
					OSTaskResume(DLtask);
				}
			}
		}
	}
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :主模块主功能初始化
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void app_Esummod_fun_entry( void )
{
	_EstaskID=os_task_assign_stk_build( _Esum_task, 6);
	app_global_DL_measure_entery();
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :主模块主功能释放资源
 *
 * @输入 :NONE
 *
 * @输出 :NONE
********************************************************************/
void app_Esummod_fun_exit( void )
{
	app_global_DL_measure_exit();
	if ( _EstaskID != INVALID_PRO )
	{
		OS_DELETE_REQ( _EstaskID );
		_EstaskID = INVALID_PRO;
	}
	return;
}

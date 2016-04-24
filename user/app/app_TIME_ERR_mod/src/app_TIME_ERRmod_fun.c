/*********************************************************************************************
*            Öéº£¿ÆÜöµçÆ÷ÓÐÏÞ¹«Ë¾
*            http://www.zhkh.com
* Ä£¿éÃû³Æ:±ä±ÈÄ£¿éÖ÷Ìå³ÌÐò
* ´´½¨Ê±¼ä:
* ËùÊô¹«Ë¾:¿ÆÜö
* ÎÄ¼þÃû³Æ:app_XCerrmod_fun.c
* ´´½¨ÈË  :
* ¹¦ÄÜÃèÊö:ÊµÏÖ±ä±È¹¦ÄÜ
******************************************************************************************/
#include    "app_TIME_errmod_inside.h"
static       uint8_t     _task_id	= INVALID_PRO;
   

static void _SaveData_load(Errsave_t *p)
{
	measure_param_t*pDL;
	DL_app_param_str_t*pDES;
	char pSTR[20] = "";
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
	global_ftoa(m_day_timer_error, p->DL_app_user_set.qs3, 5);
	g_DL_param.f_path1_err[0]= m_TMR_err[0];
	g_DL_param.f_path1_err[1]= m_TMR_err[1];
	g_DL_param.f_path1_err[2]= m_TMR_err[2];
	g_DL_param.f_path1_err[3]= m_TMR_err[3];
	pDL =&g_DL_param;
	pDES = &p->DLarg;
	for(offset=0;offset <(sizeof(measure_param_t)/sizeof(float));offset++ )
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
	}
	pDES->cos_LC = pDL->cos_LC;
	p->DL_app_user_set.ERRtype = 3;//Ê±ÖÓÐ£Ñé
	return;
}

static  void _Save_file(void)
{
	Errsave_t *p;
	p = app_malloc(sizeof(Errsave_t));
	if(p == PNULL) return;
	GLOBAL_MEMCLR(p, sizeof(Errsave_t));
    _SaveData_load(p);
	app_save_PECD("TMRERR",p,sizeof(Errsave_t));
	app_free(p);
    return;
}
/********************************************************************
* @´´½¨ÈË :
* @¹¦ÄÜ :Ö÷Ä£¿é°´¼üÈÎÎñ
* @ÊäÈë :NONE
* @Êä³ö :NONE
********************************************************************/
static void _task(void*arg)
{
	uint8_t  key_buf;
	uint8_t  DLtask,Errtask;    
	int      cnt = 0;
	fatfs_readARG("USERDATA",&g_user_arg,sizeof(DL_app_user_all_info_t));
	g_user_arg.DL_app_user_set.meter_1OR3 = 0;	//µ¥±í
    g_user_arg.DL_app_user_set.handOR_GD  = 0;	//¹âµç
    fatfs_writeARG("USERDATA",&g_user_arg,sizeof(DL_app_user_all_info_t));
    app_global_set_user_dl_arg(&g_user_arg);
    Errtask= os_task_assign_stk_build(app_TIME_ERR_task,3);
    DLtask = os_task_assign_stk_build(app_TIME_DLview_task,3);
	while(1)
	{   
	    msleep(TYPE_DELAY);
		
		if ( os_task_delete_req( SELF_PRO ) == OS_TASK_DEL_REQ )
		{
			OS_DELETE_REQ( Errtask );
			OS_DELETE_REQ( DLtask );
			os_task_delete( SELF_PRO );   			
		}
		if(++cnt%30==0)app_Systime_obtain();
		if ( app_global_key_obtain( &key_buf, 1 ) == 1 ) 
		{
              if(key_buf==KEY_RETURN||key_buf==KEY_EXIT)
              {
		       	app_module_assignExt("START");
              }
			  else if(key_buf==KEY_SAVE)
			  {
                   if(app_USERinfo_Save()==KEY_ENTER)
	               {
						_Save_file();
				   		fatfs_writeARG("USERDATA",&g_user_arg,sizeof(DL_app_user_all_info_t));
	               }
			  }
			  else if(key_buf==KEY_SEARCH)
			  {
					app_module_assignExt( "SEARCH" );
			  }
			  else if(key_buf==KEY_UP)
			  {
                 GUI_WINshift(1);
			  }
			  else if(key_buf==KEY_DOWN)
			  {
                 GUI_WINshift(0);
			  }			 
			  else if(key_buf==KEY_ENTER)
			  {
                 GUI_WINdo();
			  }		
			  else if(key_buf==KEY_SETTING)
			  {
                 GUI_keyMSG_post(m_TIME_ERR_main_win,KEY_CONST_SETTING);
			  }
			  else if(key_buf==KEY_F1)
			  {
                GUI_keyMSG_post(m_TIME_ERR_main_win,KEY_PAUSE);
			  }
		}
	}		
}
/********************************************************************
* @´´½¨ÈË :
* @¹¦ÄÜ :Ä£¿éÖ÷¹¦ÄÜ³õÊ¼»¯
* @ÊäÈë :NONE
* @Êä³ö :NONE
********************************************************************/
void app_TIME_errmod_fun_entry()
{
	app_TIME_errmod_window_creat();
	app_global_DL_measure_entery();
	_task_id = os_task_assign_stk_build( _task,4);
	return;
}
/********************************************************************
 * @´´½¨ÈË :
 * @¹¦ÄÜ :testÄ£¿éÖ÷¹¦ÄÜÊÍ·Å×Ê´
 * @ÊäÈë :NONE
 * @Êä³ö :NONE
********************************************************************/
void app_TIME_errmod_fun_exit()
{
	if ( _task_id != INVALID_PRO )
	{
		OS_DELETE_REQ( _task_id );
		_task_id = INVALID_PRO;
	}
	app_global_DL_measure_exit();
	return;
}

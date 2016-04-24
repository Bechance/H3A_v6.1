/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:变比模块主体程序
* 创建时间:
* 所属公司 :科荟
* 文件名称:app_PTCTmod_fun.c
* 创建人 :
* 功能描述:实现变比功能
******************************************************************************************/
#include "app_PTCTmod_inside.h"
static  uint8_t   _task_id=INVALID_PRO;
static  hwin_t    *_edit = 0;

static void app_float_display(float data,Bsize_t id,Bsize_t bit)
{
	char pst[20]="";
	GUI_edit_select(id);
	GUI_edit_clean();	
	if ( GLOBAL_ABS( data )<9999999 && data != 0 )
	{
        if((data<0)&&GLOBAL_ABS( data )>999999)
        {
			return;
		}
		global_ftoa( data, pst, 4 );
		if ( global_strlen( pst ) > (bit-1))
		{
			pst[bit] = ASCII_NULL;
		}
		if(pst[bit-1]=='.')
		{
			pst[bit-1] = ASCII_NULL;
		}
		GUI_edit_string_display( pst );
	}
	return;
}

static void _calculateDD_DN( float fU, float fI, float fJ, float *fDD, float *fDN )
{
	float fX=1;
	if ( fU < 0.0000001 )
	{
		fX = 0;
	}
	else
	{
		fX = fI / fU;
	}
	*fDD =fX*cos( fJ * GLOBAL_PI / 180.0 )*1000;
	*fDN =-1*fX*sin( fJ * GLOBAL_PI / 180.0 )*1000;
	return;
}
/********************************************************************
* @创建人 :范文斌
* @功能 :PTCT数据显示
* @输入 :数据
* @输出 :NONE
********************************************************************/
static void _PTCT_data_display(measure_param_t *pmem)
{
	float szgl[4]={0,};
	float DD[3]={0,};
	float DN[3]={0,};
	GUI_window_hwin_req( _edit );
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_DLA_COLOR);
	app_float_display(pmem->phase_a.f_U, EDIT_UA,6);
	app_float_display(pmem->phase_a.f_I, EDIT_IA,6);
	app_float_display(pmem->phase_a.f_J, EDIT_JA,6);
	app_float_display(pmem->phase_a.f_P, EDIT_PA,6);
	app_float_display(pmem->phase_a.f_Q, EDIT_QA,6);
	GUI_set_Fgcolor(C_GUI_DLB_COLOR);
	app_float_display(pmem->phase_b.f_U, EDIT_UB,6);
	app_float_display(pmem->phase_b.f_I, EDIT_IB,6);
	app_float_display(pmem->phase_b.f_J, EDIT_JB,6);
	app_float_display(pmem->phase_b.f_P, EDIT_PB,6);
	app_float_display(pmem->phase_b.f_Q, EDIT_QB,6);	
	GUI_set_Fgcolor(C_GUI_DLC_COLOR);
	app_float_display(pmem->phase_c.f_U, EDIT_UC,6);
	app_float_display(pmem->phase_c.f_I, EDIT_IC,6);
	app_float_display(pmem->phase_c.f_J, EDIT_JC,6);
	app_float_display(pmem->phase_c.f_P, EDIT_PC,6);
	app_float_display(pmem->phase_c.f_Q, EDIT_QC,6);	
	GUI_set_Fgcolor(C_GUI_WHITE);
	_calculateDD_DN(pmem->phase_a.f_U,pmem->phase_a.f_I,pmem->phase_a.f_J,&DD[0],&DN[0]);
	_calculateDD_DN(pmem->phase_a.f_U,pmem->phase_b.f_I,pmem->phase_b.f_J,&DD[1],&DN[1]);
	_calculateDD_DN(pmem->phase_a.f_U,pmem->phase_c.f_I,pmem->phase_c.f_J,&DD[2],&DN[2]);
	GUI_set_Fgcolor(C_GUI_DLA_COLOR);
	app_float_display(DD[0], EDIT_DDA,8);
	app_float_display(DN[0], EDIT_DNA,8);
	GUI_set_Fgcolor(C_GUI_DLB_COLOR);
	app_float_display(DD[1], EDIT_DDB,8);
	app_float_display(DN[1], EDIT_DNB,8);	
	GUI_set_Fgcolor(C_GUI_DLC_COLOR);
	app_float_display(DD[2], EDIT_DDC,8);
	app_float_display(DN[2], EDIT_DNC,8);
	szgl[0]=pmem->phase_a.f_U*pmem->phase_a.f_I;
	szgl[1]=pmem->phase_b.f_U*pmem->phase_b.f_I;
	szgl[2]=pmem->phase_c.f_U*pmem->phase_c.f_I;
	szgl[3]=szgl[0]+szgl[1]+szgl[2];	
	GUI_set_Fgcolor(C_GUI_DLA_COLOR);
	app_float_display(szgl[0], EDIT_SZ_A,8);
	GUI_set_Fgcolor(C_GUI_DLB_COLOR);
	app_float_display(szgl[1], EDIT_SZ_B,8);
	GUI_set_Fgcolor(C_GUI_DLC_COLOR);
	app_float_display(szgl[2], EDIT_SZ_C,8);
	GUI_set_Fgcolor(C_GUI_WHITE);
	app_float_display(pmem->f_Feq, EDIT_FREQ,6);
	app_float_display(szgl[3], EDIT_SZ_ALL,8);
	GUI_window_hwin_release(_edit);
	return;
}
/********************************************************************
* @创建人 :范文斌
* @功能 :PTCT模块测量任务
* @输入 :NONE
* @输出 :NONE
********************************************************************/
static void _display_task(void *arg)
{
	measure_param_t *pmem;
	pmem = &g_DL_param;
	app_PTCTmod_window_main_creat(&_edit);
	while ( 1 )
	{
		msleep( APP_100_MS*20);
		app_global_DLarg_obtain( pmem); 
		_PTCT_data_display( pmem );
	}
}
/********************************************************************
* @创建人 :范文斌
* @功能 :PTCT模块主任务
* @输入 :NONE
* @输出 :NONE
********************************************************************/
static void _task(void*arg)
{
	uint8_t key;
	uint8_t id;
	app_global_DL_measure_entery();
	id = os_task_assign_stk_build(_display_task,3);
	while(1)
	{
	    msleep(TYPE_DELAY);
		if ( os_task_delete_req(SELF_PRO) == OS_TASK_DEL_REQ)
		{
			os_task_delete( id );
			app_PTCTmod_window_main_destory();
			os_task_delete( SELF_PRO );   			
		}
		if ( app_global_key_obtain( &key, 1 ) == 1 )
		{	
		    if(key ==KEY_EXIT)
			{
		       	//app_module_assignExt("START");
				app_module_switch_last_mod();
			}
			else if(key == KEY_F2)
			{
			    g_user_arg.DL_app_user_set.dlsr=GUI_drop_list_unfold(m_PTdrop);
				fatfs_readARG("USERDATA",&g_user_arg,sizeof(DL_app_user_all_info_t));
			    app_global_set_user_dl_arg(&g_user_arg);
			}
		}
	}
}
/********************************************************************
* @创建人 :范文斌
* @功能 :PTCT模块主功能初始化
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void app_PTCTmod_fun_entry()
{
	_task_id = os_task_assign_stk_build( _task,3);
	return;
}
/********************************************************************
 * @创建人 :范文斌
 * @功能 :PTCT模块主功能释放资源
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_PTCTmod_fun_exit()
{
	if ( _task_id != INVALID_PRO )
	{
		OS_DELETE_REQ( _task_id );
		_task_id = INVALID_PRO;
	}
	app_global_DL_measure_exit();
	return;
}

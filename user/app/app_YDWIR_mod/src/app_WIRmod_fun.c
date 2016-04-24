/********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:系统设置模块界面函数集合
* 创建时间:2010-12-19
* 所属公司 :科荟
* 文件名称:app_WIRmod_DLcreate.c
* 创建人:揭成
* 功能描述:提供系统设置界面画屏函数API
******************************************************************************************/
#include"app_WIRmod_inside.h"
static   	uint8_t _task_id;
static      float  	  _angle_FIFO[6];
static      Bsize_t   _timecnt;
#define     ANGLE_ERR          			2


static void _SaveData_load(WIR_save_arg_t *p)
{
	GLOBAL_MEMCLR(p, sizeof(WIR_save_arg_t));
	global_memcpy(p->m_WIR_ALG,m_alg_presult,sizeof(WIR_ALG_t)*4);
	global_memcpy(&p->m_redeem_arg,&m_WIRmod_redeem_arg,sizeof(redeem_arg_t));
	p->m_offset = GUI_radio_curmember_obtain(m_WIRresjult_win);
	return;
}


static  void _Save_file(void)
{
	WIR_save_arg_t *p;
	p = app_malloc(sizeof(WIR_save_arg_t));
	if(p == PNULL) return;
	GLOBAL_MEMCLR(p, sizeof(WIR_save_arg_t));
    _SaveData_load(p);
	app_save_PECD("WIRDATA",p,sizeof(WIR_save_arg_t));
	app_free(p); 
    return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :分析角度是否改变
 * @输入 :
 * @输出 : NONE
********************************************************************/
static BOOL _angle_sys( measure_param_t*Pdl )
{
	Bsize_t offset = 0;
	if ( ++_timecnt % 10 == 0 )
		return TURE;
	if ( GLOBAL_ABS( _angle_FIFO[offset] - Pdl->phase_a.f_J ) > ANGLE_ERR )
	{
		_angle_FIFO[offset] = Pdl->phase_a.f_J;
		return TURE;
	}
	offset++;
	if ( GLOBAL_ABS( _angle_FIFO[offset] - Pdl->phase_b.f_J ) > ANGLE_ERR )
	{
		_angle_FIFO[offset] = Pdl->phase_b.f_J;
		return TURE;
	}
	offset++;
	if ( GLOBAL_ABS( _angle_FIFO[offset] - Pdl->phase_c.f_J ) > ANGLE_ERR )
	{
		_angle_FIFO[offset] = Pdl->phase_c.f_J;
		return TURE;
	}
	offset++;
	if ( GLOBAL_ABS( _angle_FIFO[offset] - Pdl->f_JUab ) > ANGLE_ERR )
	{
		_angle_FIFO[offset] = Pdl->f_JUab;
		return TURE;
	}
	offset++;
	if ( GLOBAL_ABS( _angle_FIFO[offset] - Pdl->f_JUac ) > ANGLE_ERR )
	{
		_angle_FIFO[offset] = Pdl->f_JUac;
		return TURE;
	}
	offset++;
	if ( GLOBAL_ABS( _angle_FIFO[offset] - Pdl->f_JUbc ) > ANGLE_ERR )
	{
		_angle_FIFO[offset] = Pdl->f_JUbc;
		return TURE;
	}
	return  FAIL;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :DL任务
 * @输入 :NONE
 * @输出  :NONE
********************************************************************/
static void _WIRtask( void*arg )
{
	uint8_t key_buf;
	Bsize_t cnt = 50;
	DL_flag_t DL_flag;
	Bsize_t   ID;
	_angle_FIFO[1] = 1;
	_angle_FIFO[0] = 1;
	_timecnt   = 95;
	GLOBAL_MEMCLR(&m_WIRmod_redeem_arg,sizeof(redeem_arg_t));
	app_WIRmod_window_creat(0);
	app_WIRmod_YDWIR_entry();
	while ( 1 )
	{
		msleep( TYPE_DELAY );
		if ( os_task_delete_req( SELF_PRO ) == OS_TASK_DEL_REQ )
		{
			app_WIRmod_YDWIR_exit();
			os_task_delete( SELF_PRO );   			
		}
		if ( app_global_key_obtain( &key_buf, 1 ) == 1 ) 	/*读取按键驱动值*/
		{
			if ( key_buf == KEY_EXIT )
			{
				app_module_assignExt("START");
			}
			else if(key_buf == KEY_DOWN)
			{
			    ID=GUI_radio_curmember_obtain(m_WIRresjult_win);
				if(ID<4)
				{
				    ID++;
					GUI_radio_select(m_WIRresjult_win,ID);
					os_sem_post( m_WIRmod_Resem ); 
				}
			}
			else if(key_buf == KEY_UP)
			{
			    ID=GUI_radio_curmember_obtain(m_WIRresjult_win);
				if(ID>0)
				{
				    ID--;
					GUI_radio_select(m_WIRresjult_win,ID);
					os_sem_post( m_WIRmod_Resem ); 
				}
			}
			else if(key_buf==KEY_F4)
			{
				app_WIRmod_redeem_function();
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
		}
		if(++cnt%66==0)
		{
			app_global_DLarg_obtain(&g_DL_param);
		    app_global_DLflag_obtain(&DL_flag);
			if (_angle_sys(&g_DL_param))
			{
				os_sem_post(m_WIRmod_Resem); 
			}
		}
	}
}

void app_WIRmod_fun_entry(void)
{
	_task_id= os_task_assign_stk_build(_WIRtask, 3);
    return;
}

void app_WIRmod_fun_exit(void)
{
	if ( _task_id != INVALID_PRO )
	{
		OS_DELETE_REQ( _task_id );
		_task_id = INVALID_PRO;
	}
	return;
}

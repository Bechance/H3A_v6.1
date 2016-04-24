/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:GUI核心
* 创建时间:2013-03-11
* 所属公司 :科荟
* 文件名称:Gui_lock.c
* 创建人 :揭成
* 功能描述:多任务下资源锁的产生
*----------------------------------------------------------------
修改记录:
修改日期:
修改人:
********************************************************************/
#include "../Gui.h"
a_lock_t* m_GUI_lock 	 = INVALID_LOCK;
uint8_t	  m_Gui_task_pro = INVALID_PRO;

#define   GUI_MAXTASK   10

typedef struct
{
	uint32_t  TaskID;
	hwin_t  * CURWIN;
	GUI_const_t Text;
	int       occupy;
} _Save_t;

_Save_t  m_task_save[GUI_MAXTASK];
static int 		m_CUR_taskID 	= -1;
int 		    m_enter_depth   =  0;
static uint32_t m_taskIDlock 	=  0;

static void  GUI_OS_lock( void )
{
	os_lock( m_GUI_lock );
	return;
}


static void  GUI_OS_unlock( void )
{
	os_unlock( m_GUI_lock );
	return;
}

static uint32_t  GUI_OS_taskID_obtain( void )
{
	return (( uint32_t )( OSTCBCur->OSTCBPrio ) );
}

void GUI_lcdDEV_Reg( void );

static int GUI_taskNO_obtain( void )
{
	int offset;
	uint32_t taskID;
	for ( offset = 0; offset < GUI_MAXTASK; offset++ )
	{
		taskID = GUI_OS_taskID_obtain();
		if ( m_task_save[offset].TaskID == taskID )
			return offset;
		if ( m_task_save[offset].TaskID == 0 )
		{
			m_task_save[offset].TaskID = taskID;
			return offset;
		}
	}
	return 0;
}

static void GUI_lock_SW( int TaskNo )
{
	m_task_save[m_CUR_taskID].Text = GUI_conText;
	m_task_save[m_CUR_taskID].CURWIN = m_cur_hwin;
	if ( m_task_save[TaskNo].occupy != 0 )
	{
		GUI_conText  =  m_task_save[TaskNo].Text;
		m_cur_hwin   =  m_task_save[TaskNo].CURWIN;
	}
	else
	{
		m_task_save[TaskNo].occupy = 1;
		GUI_lcdDEV_Reg();
		GUI_conText.CUR_Lhigh 	   =  0;
		m_task_save[TaskNo].Text   =  GUI_conText;
		m_task_save[TaskNo].CURWIN =  m_cur_hwin;
	}
	return;
}

void GUI_UNLOCK( void )
{
	if ( --m_enter_depth == 0 )
	{
		GUI_OS_unlock();
	}
	return;
}

void GUI_msleep( uint32_t jiffies )
{
	msleep( jiffies );
	return;
}

BOOL GUI_LOCK_state_obtain( void )
{
	if ( m_enter_depth == 0 ) return TURE;
	return FAIL;
}

void GUI_LOCK( void )
{
	int taskNO;
	if ( m_enter_depth == 0 )
	{
		GUI_OS_lock();
		taskNO = GUI_taskNO_obtain();
//		m_task_save[taskNO].Text.Bcolor =  GUI_conText.Bcolor;
//		m_task_save[taskNO].Text.Fcolor =  GUI_conText.Fcolor;
//		m_task_save[taskNO].CURWIN      =  m_cur_hwin;
		m_taskIDlock = GUI_OS_taskID_obtain();
	}
	else
	{
		if ( m_taskIDlock != GUI_OS_taskID_obtain() )
		{
			GUI_OS_lock();
			taskNO = GUI_taskNO_obtain();
//			m_task_save[taskNO].Text.Bcolor =  GUI_conText.Bcolor;
//			m_task_save[taskNO].Text.Fcolor =  GUI_conText.Fcolor;
//			m_task_save[taskNO].CURWIN      =  m_cur_hwin;
			m_taskIDlock = GUI_OS_taskID_obtain();
		}
	}
	if ( ++m_enter_depth == 1 )
	{
		taskNO = GUI_taskNO_obtain();
		if ( taskNO != m_CUR_taskID )
		{
			if ( m_CUR_taskID >= 0 )
			{
				GUI_lock_SW( taskNO );
			}
			m_CUR_taskID = taskNO;
		}
	}
}

uint32_t GUI_os_jiffies_obtain( void )
{
	return OSTimeGet();
}
void  GUI_task( void*arg );
void GUI_os_init( void )
{
	int offset;
	m_enter_depth = 0;
	m_CUR_taskID  = -1;
	m_taskIDlock  = 0;
	os_lock_creat( &m_GUI_lock );
	for ( offset = 0; offset < GUI_MAXTASK; offset++ )
	{
		m_task_save[offset].TaskID = 0;
		m_task_save[offset].occupy = 0;
		m_task_save[offset].Text.CUR_Lhigh = 0;
	}
	m_Gui_task_pro = os_task_assign_stk_build( GUI_task, 3 );
	return;
}
void GUI_os_exit( void )
{
	os_task_delete( m_Gui_task_pro );
	m_Gui_task_pro = INVALID_PRO;
}


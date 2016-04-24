/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:ϵͳ����ģ���������
* ����ʱ��:2010-08-02
* ������˾ :����
* �ļ�����:app_ERRmod_fun.c
* ���� :�ҳ�
* ��������:ʵ��ϵͳ���ù���
******************************************************************************************/
#include				"app_ERRmod_inside.h"
uint8_t  			   	m_ERRmod_task_pro = INVALID_PRO;
correct_option_t   		m_correct_option;
/********************************************************************
 * @���� :�ҳ�
 * @���� :������������
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void  app_ERRmod_correct_step_send( uint8_t main_step,uint8_t sub_step,uint8_t phase,uint8_t pos)
{
	DL_correct_step_t correct_setp;
	correct_setp.main_step 	= main_step;
	correct_setp.sub_step   = sub_step;
	correct_setp.base_U 	= global_atof( m_correct_option.c_base_U );
	correct_setp.pos 		= pos;
	correct_setp.phase 		= phase;
	os_driver_ioctl( g_DL_dev, MEASURE_CMD_CORRECT_STEP_SET, &correct_setp );/*������������*/
	return;
}
/********************************************************************
 * @���� :�ҳ�
 * @���� :ERRģ��������
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void app_ERRmod_main_task( void*arg )
{
	uint8_t key_buf;
	GLOBAL_MEMCLR(&m_correct_option,sizeof(correct_option_t));
	global_strcpy(m_correct_option.c_base_U,"220");
	app_ERRmod_window_creat();
    if(!MsgPasswordBox( 200,150,"1234"))
    {
       MOD_SWITCH_STARTMOD();
	}
	while ( 1 )
	{
		msleep( TYPE_DELAY );
		if ( app_global_key_obtain( &key_buf, 1 ) != 1 )
		{
			continue;
		}
		if(key_buf==KEY_1)
		{
		   app_ERRmod_window_destroy();
           app_ERRmod_Ucorrect_task(0);
		   app_ERRmod_window_creat();
		}
		else if(key_buf==KEY_2)
		{
		   app_ERRmod_window_destroy();
           app_ERRmod_Icorrect_task(0);
		   app_ERRmod_window_creat();
		}
		else if(key_buf==KEY_3)
		{
		   app_ERRmod_window_destroy();
           app_ERRmod_TMRcorrect_task(0);
		   app_ERRmod_window_creat();
		}
		else if(key_buf==KEY_4||key_buf==KEY_EXIT)
		{
			app_ERRmod_window_destroy();
            MOD_SWITCH_STARTMOD();
		}
	}
}
/********************************************************************
 * @���� :�ҳ�
 * @���� :��ģ�������ܳ�ʼ��
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_ERRmod_fun_entry( void )
{
	m_ERRmod_task_pro
	= os_task_assign_stk_build( app_ERRmod_main_task,11);
	os_task_name_set( m_ERRmod_task_pro, "ERRmod--Main task" );
    app_global_DL_measure_entery();
	os_driver_ioctl( g_DL_dev, MEASURE_CMD_IAUTO_LEVEL_STOP,0);
	return;
}
/********************************************************************
 * @���� :�ҳ�
 * @���� :��ģ���������ͷ���Դ
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void app_ERRmod_fun_exit( void )
{
	if ( m_ERRmod_task_pro != INVALID_PRO )
	{
		os_task_delete( m_ERRmod_task_pro );
		m_ERRmod_task_pro = INVALID_PRO;
	}
	app_global_DL_measure_exit();
	return;
}

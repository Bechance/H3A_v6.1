/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:ϵͳ����ģ���������
* ����ʱ��:2010-08-02
* ������˾ :����
* �ļ�����:app_USBmod_fun.c
* ������ :�ҳ�
* ��������:ʵ��ϵͳ���ù���
******************************************************************************************/
#include"app_USBmod_inside.h"
uint8_t 		m_usb_main_task_pro = INVALID_PRO;
extern dev_id_t g_DL_dev;
/********************************************************************
 * @������ :�ܱ�Ȩ
 * @���� :�������ݱ���
 * @���� :
 * @��� :NONE
********************************************************************/
static void app_USBmod_correct_data_backup(void)
{
	uint8_t flag;
	correct_save_data_t*psave;
	os_driver_ioctl(g_DL_dev,MEASURE_CMD_GET_CORRECT_FILE,&psave);
	flag = app_correct_data_backup(psave,sizeof(correct_save_data_t));
	if ( flag == 1 )
	{
		if(MsgBox(220,185,350,200, "��ʾ",
				  "�����������ݳɹ�",APP_HALF_SEC*10)==KEY_ENTER)
		{}
	}
	else
	{
		if(MsgBox(220,185,350,200, "��ʾ",
				  "������������ʧ��",APP_HALF_SEC*10)==KEY_ENTER)
		{}
	}
    return;    
}
/********************************************************************
 * @������ :�ܱ�Ȩ
 * @���� :��ȡ��������
 * @���� :
 * @��� :NONE
********************************************************************/
static void app_USBmod_correct_data_read(void)
{
	uint8_t flag;
	correct_save_data_t*psave;
	os_driver_ioctl(g_DL_dev,MEASURE_CMD_GET_CORRECT_FILE,&psave);
	//GLOBAL_MEMCLR(psave, sizeof(correct_save_data_t));
	flag = app_correct_data_read(psave,sizeof(correct_save_data_t));
	if ( flag == 1 )
	{
		if(MsgBox(220,185,350,200, "��ʾ",
				  "��ȡ�������ݳɹ�",APP_HALF_SEC*10)==KEY_ENTER)
		{}
	}
	else
	{
		if(MsgBox(220,185,350,200, "��ʾ",
				  "��ȡ��������ʧ�ܣ������ļ��Ƿ����!",APP_HALF_SEC*10)==KEY_ENTER)
		{}
	}
    return;
}

/********************************************************************
 * @������ :�ҳ�
 * @���� :USBģ��������
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void app_USBmod_main_task( void*arg )
{
	uint8_t key_buf;
	hwin_t *hp;
	hp = hp;
	if ( !app_USBmod_search_window_creat() )
	{
		app_module_assignExt("START");
	}
	app_USBmod_window_creat();
	while ( 1 )
	{
		if ( app_global_key_obtain( &key_buf, 1 ) == 1 ) 	/*��ȡ��������ֵ*/
		{
			if ( key_buf == KEY_EXIT||key_buf==GUI_KEY_3D_WINDOW_CLOSE)
			{
				//app_module_assignExt("START");
				asm( "  lb 0x300000" );
			}
			else if ( key_buf == KEY_0 || key_buf == KEY_1
					  || key_buf == KEY_2 || key_buf == KEY_3
					  || key_buf == KEY_5||key_buf == KEY_8
					  ||key_buf == KEY_9 ||key_buf == KEY_F5)
			{
				app_USBmod_window_destroy();
				app_hzlib_updata_entery( key_buf );
				app_USBmod_window_creat();
			}
			else if ( key_buf == KEY_4 )
			{
			}
			else if ( key_buf == KEY_6 )
			{
				if(MsgBox(220,185,350,200, "��ʾ",
				  "ȷ�ϱ�����������?",APP_HALF_SEC*100)==KEY_ENTER)
				{
					app_USBmod_correct_data_backup();
				}
			}
			else if ( key_buf == KEY_7 )
			{
				if(MsgBox(220,185,350,200, "��ʾ",
				  "ȷ�϶�ȡ������������?",APP_HALF_SEC*100)==KEY_ENTER)
				{
					app_USBmod_correct_data_read();
				}
			}
		}
		OS_DELETE_REQ_SELF();
		msleep( TYPE_DELAY );
	}
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :��ģ�������ܳ�ʼ��
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void app_USBmod_fun_entry( void )
{
	m_usb_main_task_pro
	= os_task_assign_stk_build( app_USBmod_main_task, 6 );
	os_task_name_set( m_usb_main_task_pro, "UT" );
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :��ģ���������ͷ���Դ
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void app_USBmod_fun_exit( void )
{
	if ( m_usb_main_task_pro != INVALID_PRO )
	{
		os_task_delete( m_usb_main_task_pro );
		m_usb_main_task_pro = INVALID_PRO;
	}
	return;
}

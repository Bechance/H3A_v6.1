/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:����������
* ����ʱ��:2010-06-29
* ������˾ :����
* �ļ�����:main.c
* ������ :�ҳ�
* ��������:�����ܣ�������ڣ�������ϵͳ
* ����һ�������״̬�¡�
*----------------------------------------------------------------
�޸ļ�¼:����user_main_task
Ŀ��:���ܱ�֤����ʼ���жϲŷ����Ļ�������
����Ҫ�����񣬽�ʡһ����ջ�ռ䣬Ϊ�˰�ȫ
���Ǳ����������ɸ�����ͳһ��ʼ���û�Ӧ��
ģ�顣
�޸�����:7��7��
�޸���: �ҳ� 
*********************************************************************************************************/
#include "includes.h"
/********************************************************************
 * @������:�ҳ�
 *
 * @����:ϵͳ�������
 * @����:NONE
 *
 *@���: ��ϵͳ�����쳣ʱ���û����Ե��øú���
********************************************************************/
void mod_err_inlet( void )
{
	return;
}
/********************************************************************
 * @������:�ҳ�
 * @����:��ʼ��ϵͳ��Ҫʹ�õ��ж�
 *
 * @����:NONE
 *
 *@���:NONE
********************************************************************/
void os_isr_init( void )
{
	InitCpuTimers();
	EALLOW;						   			 /*This is needed to write to EALLOW protected registers*/
	PieVectTable.USER12 = &OSCtxSw;			 /*���û��ж�12��Ϊ���������л����*/
	IER|=M_INT14;
	PieVectTable.TINT2  = &OSTickISR;		 /*����ʱ��0��Ϊϵͳ�����ж�*/
	ConfigCpuTimer( &CpuTimer2,150,10000);	 /*ϵͳ�����ж�ʱ��Ϊ10MS*/
	StartCpuTimer2();                		 /*������ʱ��2*/
	ERTM;
	EINT;
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @����:��ʼ��֧��ģ��,��������ѡ���compile_cfg.h��
 * @����:NONE
 * @���:NONE
********************************************************************/
void  support_mod_init( void )
{
#if   TASK_MONITOR_EN>0
	thread_monitor_insmod();/*���������ʼ��*/
#endif
#if  (LINK_LIST_MANEGER_EN >0)
	link_list_maneger_init();/*��̬��������ʼ��*/
#endif
#if TIMER_ARRAY_EN>0
	timer_array_mod_init();/*��ʱ�����������ʼ��*/
#endif
#if   DEBUG_LOG_EN    >0
	log_debug_insmod(); /*ϵͳ���Կڳ�ʼ��*/
#endif
#if   UC_SHELL_EN    >0
	ucshell_entery(); 	/*ϵͳ���Կڳ�ʼ��*/
#endif
#if   MEM_MANEGER_EN    >0
	mem_maneger_init();
#endif
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @����:��ʼ��ϵͳ����ģ�飬�Ը���ģ���ע��
 * @����:NONE
 * @���:NONE
********************************************************************/
#pragma DATA_SECTION (m_mod_list_head,"MOD_LIST_HEAD")
#pragma DATA_SECTION (m_DR_list_head, "DR_LIST_HEAD")
static char m_mod_list_head;
static char m_DR_list_head;
/**********************************************************************/
void app_mod_mount( void )
{
    Bsize_t offset;
	Bsize_t id;
	mod_member_t*plist;
	DR_member_t *pDR;
	char*phead = (char*)&m_mod_list_head;
    plist=(mod_member_t*)(phead+0x10);
	for(offset=0;offset<25;offset++)
	{
		if(plist[offset].crc==0x5a5a)
		{
            id=plist[offset].os_mod_add();
			app_module_set_name(id,plist[offset].name);
		}
	}
	phead = (char*)&m_DR_list_head;
    pDR=(DR_member_t*)(phead+0x10);
	for(offset=0;offset<25;offset++)
	{
		if(pDR[offset].crc==0x5a5a)
		{
            pDR[offset].driver_register();
		}
	}
	return;
}
/********************************************************************
 * @������:�ҳ�
 * @����:�õ���һ�����е�ģ��ID
 * @����:NONE
 * @���:NONE
********************************************************************/
Bsize_t app_get_1th_mod_id( void )
{
	return app_module_ID_obtain((char*)"START");
}
/********************************************************************
 * @������ :�ҳ�
 * @����: ������һ����Ҫ���е�ģ��׼������ģ�����
 *
 * @����:NONE
 *
 *@���:NONE
 ����⵽û������ģ�����ʱ��ϵͳ��������
********************************************************************/
void app_start_module( void )
{
	Bsize_t mod_id = app_get_1th_mod_id();
	if ( !mod_id )return;
	buz_response( 15);
	app_module_send_hmsg( APP_MOD_START, mod_id, 0 ); 
	return;
}
/********************************************************************
 * @������:�ҳ�
 * @����:�û��������ɸ����񴴽���Ӧ��
 * ģ�����񣬲�����ģ��Ĺ���
 * @����:p_arg �������
 * @���:NONE
********************************************************************/
void user_main_task( void *p_arg )
{
	os_isr_init();       	 			/*ϵͳ�ж� ��ʼ���������жϺ������л��ж�*/
//	OSStatInit();                       /*��ʼ��ͳ������*/
	while ( 1 )
	{
		app_mod_mount();				/*����Ӧ��ģ���ʼ��*/
		support_mod_init();				/*��ʼ��֧��ģ��*/
		app_global_resouce_init();		/*ȫ�ֹ�����Դ��ʼ��*/
		app_start_module();				/*������һ��Ӧ��ģ��*/
		app_module_state_handle(0);		/*��ʼ����ģ��״̬*/
		msleep( TYPE_DELAY );
	}
}
/********************************************************************
 * @������ :�ҳ�
 * @����:Ӧ��ģ���ʼ��
 *
 * @����:NONE
 *
 *@���:NONE
********************************************************************/
void app_init( void )
{
	os_connect_init();											 /*��ʼ��ϵͳ���Ӳ�*/

	os_task_assign_pro_build( user_main_task, MAIN_TASK_PRO, 5 );/*����ϵͳ������*/

	os_task_name_set( MAIN_TASK_PRO, "Global--Module manege" );  /*Ϊ��������������*/
	return;
}
/********************************************************************
* @������:�ҳ�
* @����:����������ucos,��ʼ��Ӧ��ģ��
*
* @����:NONE
*
* @���:NONE
********************************************************************/
void  main( void )
{
	bsp_init();   			/*��ʼ��Ӳ������*/
	OSInit();			 	/*��ʼ��ϵͳ*/
	app_init();   			/*��ʼ��Ӧ��ģ��*/
	OSStart();             	/*��ʼ����*/
	return ;
}

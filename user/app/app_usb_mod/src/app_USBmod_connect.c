/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������: USB ����ģ��������Ӳ�
* ����ʱ��:2010-09-19
* ������˾ :����
* �ļ�����:app_USBmod_connect.c
* ������ :�ҳ�
* ��������:ʵ�ָ�ģ��Ķ�������
*-------------------------------------------------------------------*/
#include"app_USBmod_inside.h"
#include "../../../support\app_support.h"

/********************************************************************
 * @������ :�ҳ�
 * @���� : ϵͳ����ģ����Դ��ʼ��
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void app_USBmod_resource_init( void )
{
	app_module_color_select(2);
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� : ģ��������
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void app_USBmod_insmod( void )
{
	app_global_resouce_req( 0 );  /*����ռ�ù�����Դ*/
	app_USBmod_resource_init();/*��ʼ����ģ����Դ*/
	app_USBmod_fun_entry();    /*��ģ�����������*/
	os_driver_ioctl(g_UIout_dev,UIOUT_CLOSE,0);
	DBGMSG( "USBmod....\n", LOG_NP, LOG_NP );
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� : ģ��ж�����
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void app_USBmod_exit( void )
{
	app_global_resouce_release( 0 );          /*�ͷ�ռ�ù�����Դ*/
	app_USBmod_fun_exit();/*�˳���ģ��������*/
	DBGMSG( "USBmod=>", LOG_NP, LOG_NP );
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� : ��ģ��ӵ������б�
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
Bsize_t app_USBmod_add( void )
{
	app_mod_inlet_t temp;
	Bsize_t mod_id;
	temp.insmod_fun = app_USBmod_insmod;/*ģ�����*/
	temp.exitmod_fun = app_USBmod_exit;/*ģ�����*/
	mod_id = app_module_add( &temp );
	return mod_id;
}
/*module AUTO Register*/
#pragma DATA_SECTION (app_USBmod_list,"MOD_LIST_ADDR")
const mod_member_t app_USBmod_list[]=
{
	app_USBmod_add,
	"USB",
	0x5a5a
};

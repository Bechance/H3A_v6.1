/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������: ģ��������Ӳ�
* ����ʱ��:2013-04-26
* ������˾ :����
* �ļ�����:app_RMmod_connect.c
* ���� :�ܱ�Ȩ
* ��������:ʵ��ģ��Ķ�������
*-------------------------------------------------------------------*/
#include"app_RM_inside.h"
#include "../../../support\app_support.h"
/********************************************************************
 * @���� :�ҳ�
 * @���� :ϵͳ����ģ����Դ��ʼ��
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_RMmod_resource_init( void )
{
	app_module_color_select(2);
	return;
}
/********************************************************************
 * @���� :�ҳ�
 * @���� :ģ��������
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_RMmod_insmod( void )
{
	app_global_resouce_req( 0 );  	/*����ռ�ù�����Դ*/
	app_RMmod_resource_init();/*��ʼ����ģ����Դ*/
	app_RMmod_fun_entry();    /*��ģ�����������*/
	return;
}
/********************************************************************
 * @���� :�ҳ�
 * @���� :ģ��ж�����
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_RMmod_exit( void )
{
	app_RMmod_fun_exit();/*�˳���ģ��������*/
	app_global_resouce_release(0);
	return;
}
/********************************************************************
 * @���� :�ҳ�
 * @���� :��ģ��ӵ������б�
 * @���� :NONE
 * @��� :NONE
********************************************************************/
Bsize_t app_RMmod_add( void )
{
	app_mod_inlet_t temp;
	Bsize_t mod_id;
	temp.insmod_fun = app_RMmod_insmod;/*ģ�����*/
	temp.exitmod_fun = app_RMmod_exit;/*ģ�����*/
	mod_id = app_module_add( &temp );
	return mod_id;
}
/*module AUTO Register*/
#pragma DATA_SECTION (app_RMmod_list,"MOD_LIST_ADDR")
const mod_member_t app_RMmod_list[] =
{
	app_RMmod_add,
	"RMETER",
	0x5a5a
};



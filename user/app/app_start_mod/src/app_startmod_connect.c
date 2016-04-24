/********************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������: testģ��������Ӳ�
* ����ʱ��:
* ������˾ :����
* �ļ�����:app_startmod_connect.c
* ������ :�ҳ�
* ��������:ʵ�ָ�ģ��Ķ�������
*----------------------------------------------------------------*/
#include "app_startmod_inside.h"
#include "../../../support\app_support.h"
/********************************************************************
 * @������ :
 * @���� :startģ����Դ��ʼ��
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_startmod_resource_init( void )
{
	app_module_color_select( 2 );
	fatfs_readARG( "USERDATA", &g_user_arg, sizeof( DL_app_user_all_info_t ) );
	return;
}
/********************************************************************
 * @������ :
 * @���� : startģ��������
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void app_startmod_insmod( void )
{
	app_global_resouce_req( 0 );  	/*����ռ�ù�����Դ*/
	app_startmod_resource_init();	/*��ʼ����ģ����Դ*/
	app_startmod_fun_entry();    	/*��ģ�����������*/
	DBGMSG( "startmod....\n", LOG_NP, LOG_NP );
	return;
}
/********************************************************************
 * @������ :
 * @���� : startģ��ж�����
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void app_startmod_exit( void )
{
	app_startmod_fun_exit();					/*�˳���ģ��������*/
	app_global_resouce_release( 0 );          	/*�ͷ�ռ�ù�����Դ*/
	DBGMSG( "startmod=>", LOG_NP, LOG_NP );
	return;
}
/********************************************************************
 * @������ :
 * @���� : ��ģ��ӵ������б�
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
Bsize_t app_startmod_add( void )
{
	app_mod_inlet_t temp;
	Bsize_t mod_id;
	temp.insmod_fun = app_startmod_insmod;/*ģ�����*/
	temp.exitmod_fun = app_startmod_exit;/*ģ�����*/
	mod_id = app_module_add( &temp );
	return mod_id;
}
/*module AUTO Register*/
#pragma DATA_SECTION (app_startmod_list,"MOD_LIST_ADDR")
const mod_member_t app_startmod_list[] =
{
	app_startmod_add,
	"MENU",
	0x5a5a
};

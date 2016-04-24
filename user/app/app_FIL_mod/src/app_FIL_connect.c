/********************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������: selfCORģ��������Ӳ�
* ����ʱ��:
* ������˾:����
* �ļ�����:app_FILmod_connect.c
* ������  :�ҳ�
* ��������:ʵ�ָ�ģ��Ķ�������
*----------------------------------------------------------------*/
#include "app_FIL_inside.h"
#include "../../../support\app_support.h"
/********************************************************************
 * @������ :�ҳ�
 * @���� : selfCORģ����Դ��ʼ��
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void app_FILmod_resource_init( void )
{
	app_module_color_select(2);
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� : selfCORģ��������
 * @���� :NONE
 * @���  :NONE
********************************************************************/
void app_FILmod_insmod( void )
{
	app_global_resouce_req( 0 );  			/*����ռ�ù�����Դ*/
	app_FILmod_resource_init();				/*��ʼ����ģ����Դ*/
	app_FILmod_fun_entry();    		       	/*��ģ�����������*/
	DBGMSG( "FILmod....\n", LOG_NP, LOG_NP);
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :selfCORģ��ж�����
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_FILmod_exit( void )
{
	app_global_resouce_release( 0 );          	/*�ͷ�ռ�ù�����Դ*/
	app_FILmod_fun_exit();				    	/*�˳���ģ��������*/
	DBGMSG( "FILmod=>", LOG_NP, LOG_NP);
	return;
}
/********************************************************************
 * @������ :
 * @����   :��ģ��ӵ������б�
 *
 * @����   :NONE
 *
 * @���   :NONE
********************************************************************/
Bsize_t app_FILmod_add( void )
{
	app_mod_inlet_t temp;
	Bsize_t mod_id;
	temp.insmod_fun  = app_FILmod_insmod;/*ģ�����*/
	temp.exitmod_fun = app_FILmod_exit;/*ģ�����*/
	mod_id = app_module_add( &temp );
	return mod_id;
}

#pragma DATA_SECTION (app_FILmod_list,"MOD_LIST_ADDR")
const mod_member_t app_FILmod_list[]=
{
	app_FILmod_add,
	"FIL",
	0x5a5a
};

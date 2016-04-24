/********************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������: ���ģ��������Ӳ�
* ����ʱ��:
* ������˾ :����
* �ļ�����:app_PCmod_connect.c
* ������ :�ҳ�
* ��������:ʵ�ָ�ģ��Ķ�������
*----------------------------------------------------------------*/
#include "app_BBmod_inside.h"
#include "../../../support\app_support.h"
/********************************************************************
 * @������ :
 * @���� : ���ģ����Դ��ʼ��
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void app_BBmod_resource_init( void )
{
	app_module_color_select(2);
	return;
}
/********************************************************************
 * @������ :
 * @���� : ���ģ��������
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void app_BBmod_insmod( void )
{
	app_global_resouce_req( 0 );  	/*����ռ�ù�����Դ*/
	app_BBmod_resource_init();		/*��ʼ����ģ����Դ*/
	app_BBmod_fun_entry();    		/*��ģ�����������*/
	DBGMSG( "BBmod....\n", LOG_NP, LOG_NP );
	return;
}
/********************************************************************
 * @������ :
 * @���� : ���ģ��ж�����
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void app_BBmod_exit( void )
{
	app_global_resouce_release( 0 );          	/*�ͷ�ռ�ù�����Դ*/
	app_BBmod_fun_exit();					  /*�˳���ģ��������*/
	DBGMSG( "BBmod=>", LOG_NP, LOG_NP );
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
Bsize_t app_BBmod_add( void )
{
	app_mod_inlet_t temp;
	Bsize_t mod_id;
	temp.insmod_fun = app_BBmod_insmod;/*ģ�����*/
	temp.exitmod_fun = app_BBmod_exit;/*ģ�����*/
	mod_id = app_module_add( &temp );
	return mod_id;
}

#pragma DATA_SECTION (app_BBmod_list,"MOD_LIST_ADDR")
const mod_member_t app_BBmod_list[]=
{
	app_BBmod_add,
	"BB",
	0x5a5a
};

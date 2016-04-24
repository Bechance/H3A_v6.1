/********************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������: PTCTģ��������Ӳ�
* ����ʱ��:
* ������˾ :����
* �ļ�����:app_PTCTmod_connect.c
* ������ :�ҳ�
* ��������:ʵ�ָ�ģ��Ķ�������
*----------------------------------------------------------------*/
#include "app_PTCTmod_inside.h"
#include "../../../support\app_support.h"
/********************************************************************
 * @������ :���ı�
 * @���� : PTCTģ����Դ��ʼ��
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void app_PTCTmod_resource_init( void )
{
	app_module_color_select(2);
	return;
}
/********************************************************************
 * @������ :���ı�
 * @���� : PTCTģ��������
 * @���� :NONE
 * @���  :NONE
********************************************************************/
void app_PTCTmod_insmod( void )
{
	app_global_resouce_req( 0 );  	/*����ռ�ù�����Դ*/
	app_PTCTmod_resource_init();		/*��ʼ����ģ����Դ*/
	app_PTCTmod_fun_entry();    		/*��ģ�����������*/
	DBGMSG( "PTCTmod....\n", LOG_NP, LOG_NP );
	return;
}
/********************************************************************
 * @������ :���ı�
 * @���� :PTCTģ��ж�����
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_PTCTmod_exit( void )
{
	app_global_resouce_release( 0 );          	/*�ͷ�ռ�ù�����Դ*/
	app_PTCTmod_fun_exit();					  /*�˳���ģ��������*/
	DBGMSG( "PTCTmod=>", LOG_NP, LOG_NP );
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
Bsize_t app_PTCTmod_add( void )
{
	app_mod_inlet_t temp;
	Bsize_t mod_id;
	temp.insmod_fun = app_PTCTmod_insmod;/*ģ�����*/
	temp.exitmod_fun = app_PTCTmod_exit;/*ģ�����*/
	mod_id = app_module_add( &temp );
	return mod_id;
}

#pragma DATA_SECTION (app_PTCTmod_list,"MOD_LIST_ADDR")
const mod_member_t app_PTCTmod_list[]=
{
	app_PTCTmod_add,
	"PTCT",
	0x5a5a
};

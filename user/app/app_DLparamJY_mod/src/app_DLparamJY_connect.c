/********************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������: �����У��ģ��������Ӳ�
* ����ʱ��: 2013��1��7��
* ������˾:����
* �ļ�����:app_DLparamJY_connect.c
* ���� :�ܱ�Ȩ
* ��������:ʵ�ָ�ģ��Ķ�������
*----------------------------------------------------------------*/
#include "app_DLparamJY_inside.h"
#include "../../../support\app_support.h"
/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :ģ����Դ��ʼ��
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_DLparamJYmod_resource_init( void )
{
	app_module_color_select(2);
	return;
}
/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :ģ��������
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_DLparamJYmod_insmod( void )
{
	app_global_resouce_req( 0 );  		/*����ռ�ù�����Դ*/
	app_DLparamJYmod_resource_init();	/*��ʼ����ģ����Դ*/
	app_DLparamJYmod_fun_entry();    	/*��ģ�����������*/
	DBGMSG("DLparamJYmod....\n",LOG_NP,LOG_NP);
	return;
}
/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :ģ��ж�����
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_DLparamJYmod_exit( void )
{
	app_global_resouce_release( 0 );          	/*�ͷ�ռ�ù�����Դ*/
	app_DLparamJYmod_fun_exit();					/*�˳���ģ��������*/
	DBGMSG( "DLparamJYmod=>", LOG_NP, LOG_NP );
	return;
}
/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :��ģ��ӵ������б�
 * @���� :NONE
 * @��� :NONE
********************************************************************/
Bsize_t app_DLparamJYmod_add( void )
{
	app_mod_inlet_t temp;
	Bsize_t mod_id;
	temp.insmod_fun = app_DLparamJYmod_insmod;/*ģ�����*/
	temp.exitmod_fun = app_DLparamJYmod_exit;/*ģ�����*/
	mod_id = app_module_add( &temp );
	return mod_id;
}

#pragma DATA_SECTION (app_DLparamJYmod_list,"MOD_LIST_ADDR")
const mod_member_t app_DLparamJYmod_list[]=
{
	app_DLparamJYmod_add,
	"DLPARAM",
	0x5a5a
};



/********************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������: testģ��������Ӳ�
* ����ʱ��:
* ������˾ :����
* �ļ�����:app_XCerrmod_connect.c
* ������ :�ҳ�
* ��������:ʵ�ָ�ģ��Ķ�������
*----------------------------------------------------------------*/
#include "app_TIME_errmod_inside.h"
#include "../../../support\app_support.h"
/********************************************************************
 * @������ :
 * @���� :startģ����Դ��ʼ��
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_TIME_errmod_resource_init( void )
{
	app_module_color_select(2);
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
void app_TIME_errmod_insmod( void )
{
	app_global_resouce_req( 0 );  	/*����ռ�ù�����Դ*/
	app_TIME_errmod_resource_init();	/*��ʼ����ģ����Դ*/
	app_TIME_errmod_fun_entry();    	/*��ģ�����������*/
	DBGMSG("Terrmod....\n",LOG_NP,LOG_NP);
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
void app_TIME_errmod_exit( void )
{
	app_TIME_errmod_fun_exit();					/*�˳���ģ��������*/
	app_global_resouce_release( 0 );          	/*�ͷ�ռ�ù�����Դ*/
	DBGMSG( "Terrmod=>", LOG_NP, LOG_NP );
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
Bsize_t app_Time_errmod_add( void )
{
	app_mod_inlet_t temp;
	Bsize_t mod_id;
	temp.insmod_fun = app_TIME_errmod_insmod;/*ģ�����*/
	temp.exitmod_fun = app_TIME_errmod_exit;/*ģ�����*/
	mod_id = app_module_add( &temp );
	return mod_id;
}
/*module AUTO Register*/
#pragma DATA_SECTION (app_Time_errmod_list,"MOD_LIST_ADDR")
const mod_member_t app_Time_errmod_list[]=
{
	app_Time_errmod_add,
	"TERR",
	0x5a5a
};


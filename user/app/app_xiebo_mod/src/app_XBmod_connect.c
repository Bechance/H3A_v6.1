/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������: wave ����ģ��������Ӳ�
* ����ʱ��:2010-09-19
* ������˾ :����
* �ļ�����:app_XBmod_connect.c
* ������ :�ҳ�
* ��������:ʵ�ָ�ģ��Ķ�������
*-------------------------------------------------------------------*/
#include"app_XBmod_inside.h"
#include "../../../support\app_support.h"

uint16_t LCD_Color2Index_565(uint32_t Color );

/********************************************************************
 * @������ :�ҳ�
 * @���� : ϵͳ����ģ����Դ��ʼ��
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void app_XBmod_resource_init( void )
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
void app_XBmod_insmod( void )
{
	app_global_resouce_req( 0 );  /*����ռ�ù�����Դ*/
	app_XBmod_resource_init();/*��ʼ����ģ����Դ*/
	app_XBmod_fun_entry();    /*��ģ�����������*/
	DBGMSG( "XBmod....\n", LOG_NP, LOG_NP );
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
void app_XBmod_exit( void )
{
	app_global_resouce_release( 0 );          /*�ͷ�ռ�ù�����Դ*/
	app_XBmod_fun_exit();/*�˳���ģ��������*/
	DBGMSG( "XBmod=>", LOG_NP, LOG_NP );
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
Bsize_t app_XBmod_add( void )
{
	app_mod_inlet_t temp;
	Bsize_t mod_id;
	temp.insmod_fun  = app_XBmod_insmod;
	temp.exitmod_fun = app_XBmod_exit;
	mod_id           = app_module_add( &temp );
	return mod_id;
}
/*module AUTO Register*/
#pragma DATA_SECTION (app_XBmod_list,"MOD_LIST_ADDR")
const mod_member_t app_XBmod_list[]=
{
	app_XBmod_add,
	"XB",
	0x5a5a
};

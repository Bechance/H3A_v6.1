/********************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������: saveģ��������Ӳ�
* ����ʱ��:
* ������˾ :����
* �ļ�����:app_Searchmod_connect.c
* ���� :�ҳ�
* ��������:ʵ�ָ�ģ��Ķ�������
*----------------------------------------------------------------*/
#include "app_Searchmod_inside.h"
#include "../../../support\app_support.h"
/********************************************************************
 * @���� :
 * @���� : PCģ����Դ��ʼ��
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void app_Searchmod_resource_init( void )
{
	app_module_color_select(2);
	return;
}
/********************************************************************
 * @���� :
 * @���� : ģ��������
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void app_Searchmod_insmod( void )
{
	app_global_resouce_req( 0 );  /*����ռ�ù�����Դ*/
	app_Searchmod_resource_init();/*��ʼ����ģ����Դ*/
	app_Searchmod_fun_entry();    /*��ģ�����������*/
	DBGMSG( "Searchmod....\n", LOG_NP, LOG_NP );
	return;
}
/********************************************************************
 * @���� :
 * @���� : ģ��ж�����
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void app_Searchmod_exit( void )
{
	app_global_resouce_release( 0 );          /*�ͷ�ռ�ù�����Դ*/
	app_Searchmod_fun_exit();			/*�˳���ģ��������*/
	DBGMSG( "Searchmod=>", LOG_NP, LOG_NP );
	return;
}
/********************************************************************
 * @���� :
 * @���� : ��ģ��ӵ������б�
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
Bsize_t app_Searchmod_add( void )
{
	app_mod_inlet_t temp;
	Bsize_t mod_id;
	temp.insmod_fun = app_Searchmod_insmod;/*ģ�����*/
	temp.exitmod_fun = app_Searchmod_exit;/*ģ�����*/
	mod_id = app_module_add( &temp );
	return mod_id;
}
/*module AUTO Register*/
#pragma DATA_SECTION (app_searchmod_list,"MOD_LIST_ADDR")
const mod_member_t app_searchmod_list[] =
{
	app_Searchmod_add,
	"SEARCH",
	0x5a5a
};

/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:�����ۻ�ģ��������Ӳ�
* ����ʱ��:2013-05-11
* ������˾:����
* �ļ�����:app_Esummod_connect.c
* ������  :�ҳ�
* ��������:ʵ��ģ��Ķ�������
*-------------------------------------------------------------------*/
#include"app_Esummod_inside.h"
/********************************************************************
 * @������ :�ҳ�
 * @���� : ϵͳ����ģ����Դ��ʼ��
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void app_Esummod_resource_init( void )
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
void app_Esummod_insmod( void )
{
	app_global_resouce_req( 0 );  	/*����ռ�ù�����Դ*/
	app_Esummod_resource_init();		/*��ʼ����ģ����Դ*/
	app_Esummod_fun_entry();    		/*��ģ�����������*/
	DBGMSG( "Esummod....\n", LOG_NP, LOG_NP );
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
void app_Esummod_exit( void )
{
	app_global_resouce_release( 0 );/*�ͷ�ռ�ù�����Դ*/
	app_Esummod_fun_exit();			/*�˳���ģ��������*/
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
Bsize_t app_Esummod_add( void )
{
	app_mod_inlet_t temp;
	Bsize_t mod_id;
	temp.insmod_fun 	= app_Esummod_insmod;/*ģ�����*/
	temp.exitmod_fun 	= app_Esummod_exit;/*ģ�����*/
	mod_id = app_module_add( &temp );
	return mod_id;
}

#pragma DATA_SECTION (app_Esummod_list,"MOD_LIST_ADDR")
const mod_member_t app_Esummod_list[]=
{
	app_Esummod_add,
	"ZOUZI",
	0x5a5a
};

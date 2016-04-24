/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:����ģ��������Ӳ�
* ����ʱ��:2012-07-10
* ������˾:����
* �ļ�����:app_zzmod_connect.c
* ������ :�ܱ�Ȩ
* ��������:ʵ�ָ�ģ��Ķ�������
**********************************************************************************************/
#include "app_ZZmod_inside.h"
#include "../../../support\app_support.h"

/********************************************************************
 * @������ :�ܱ�Ȩ
 * @���� :ģ����Դ��ʼ��
 *
 * @���� :NONE
 *
 * @��� :NONE
********************************************************************/
void 	app_zzmod_resource_init( void )
{
	app_module_color_select(2);
	return;
}

/********************************************************************
 * @������ :�ܱ�Ȩ
 * @���� : ģ��������
 *
 * @���� :NONE
 *
 * @��� :NONE
********************************************************************/
void	app_zzmod_insmod( void )
{
	app_global_resouce_req( 0 );  /*����ռ�ù�����Դ*/
	app_zzmod_resource_init();	  /*��ʼ����ģ����Դ*/
	app_zzmod_fun_entry();       /*��ģ�����������*/
	DBGMSG( "zzmod....\n", LOG_NP, LOG_NP );
	return;
}
/********************************************************************
 * @������ :�ܱ�Ȩ
 * @���� :ģ��ж�����
 *
 * @���� :NONE
 *
 * @��� :NONE
********************************************************************/
void	app_zzmod_exit( void )
{
	app_global_resouce_release( 0 );   /*�ͷ�ռ�ù�����Դ*/
	app_zzmod_fun_exit();			   /*�˳���ģ��������*/
	DBGMSG( "zzmod=>", LOG_NP, LOG_NP );
	return;
}
/********************************************************************
 * @������ :�ܱ�Ȩ
 * @���� : ��ģ��ӵ������б�
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
Bsize_t app_zzmod_add( void )
{
	app_mod_inlet_t	 temp;
	Bsize_t 	mod_id;
	temp.insmod_fun 	= app_zzmod_insmod;/*ģ�����*/
	temp.exitmod_fun 	= app_zzmod_exit;/*ģ�����*/
	mod_id = app_module_add( &temp );
	return mod_id;
}

/*module AUTO Register*/
#pragma DATA_SECTION (app_zzmod_list,"MOD_LIST_ADDR")
const mod_member_t app_zzmod_list[]=
{
	app_zzmod_add,
	"ZOUZI",
	0x5a5a
};


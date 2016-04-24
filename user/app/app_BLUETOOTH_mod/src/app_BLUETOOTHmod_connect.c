/********************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������: UARTģ��������Ӳ�
* ����ʱ��:
* ������˾ :����
* �ļ�����:app_UARTmod_connect.c
* ������ : ������
* ��������:ʵ�ָ�ģ��Ķ�������
*----------------------------------------------------------------*/
#include "app_BLUETOOTHmod_inside.h"
#include "../../../support\app_support.h"
/********************************************************************
 * @������ :������
 * @���� :UARTģ����Դ��ʼ��
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_BLUETOOTHmod_resource_init( void )
{
	g_Bg_color = C_GUI_HIGH_BLUE;
	g_Fg_color = C_GUI_WHITE;
	g_win_Bg_color = 799;
	Gui_button_set_color(BUTTON_TXT_COLOR,BUTTON_BG_COLOR);
	Gui_3D_window_set_color(C_GUI_WHITE,C_GUI_LOW_BLUE,C_GUI_WHITE);	
	Gui_drop_list_set_color(C_GUI_BlACK, 1151);
	return;
}
/********************************************************************
 * @������ :������
 * @���� : UARTģ��������
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void app_BLUETOOTHmod_insmod( void )
{
	app_global_resouce_req( 0 );  	/*����ռ�ù�����Դ*/
	app_BLUETOOTHmod_resource_init();	/*��ʼ����ģ����Դ*/
	app_BLUETOOTHmod_fun_entry();    	/*��ģ�����������*/
	DBGMSG("BLUETOOTHmod....\n",LOG_NP,LOG_NP);
	return;
}
/********************************************************************
 * @������ :������
 * @���� : UARTģ��ж�����
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void app_BLUETOOTHmod_exit( void )
{
	app_BLUETOOTHmod_fun_exit();					/*�˳���ģ��������*/
	app_global_resouce_release( 0 );          	/*�ͷ�ռ�ù�����Դ*/
	DBGMSG( "BLUETOOTHmod=>", LOG_NP, LOG_NP );
	return;
}
/********************************************************************
 * @������ :������
 * @���� : ��UARTģ��ӵ������б�
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
Bsize_t app_BLUETOOTHmod_add( void )
{
	app_mod_inlet_t temp;
	Bsize_t mod_id;
	temp.insmod_fun = app_BLUETOOTHmod_insmod;/*ģ�����*/
	temp.exitmod_fun = app_BLUETOOTHmod_exit;/*ģ�����*/
	mod_id = app_module_add( &temp );
	return mod_id;
}


#pragma DATA_SECTION (app_BLUETOOTHmod_list,"MOD_LIST_ADDR")
const mod_member_t app_BLUETOOTHmod_list[]=
{
	app_BLUETOOTHmod_add,
	"BLUETOOTH",
	0x5a5a
};



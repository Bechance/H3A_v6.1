/********************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称: UART模块对外连接层
* 创建时间:
* 所属公司 :科荟
* 文件名称:app_UARTmod_connect.c
* 创建人 : 阳永锋
* 功能描述:实现该模块的对外连接
*----------------------------------------------------------------*/
#include "app_BLUETOOTHmod_inside.h"
#include "../../../support\app_support.h"
/********************************************************************
 * @创建人 :阳永锋
 * @功能 :UART模块资源初始化
 * @输入 :NONE
 * @输出 :NONE
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
 * @创建人 :阳永锋
 * @功能 : UART模块加载入口
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void app_BLUETOOTHmod_insmod( void )
{
	app_global_resouce_req( 0 );  	/*申请占用公共资源*/
	app_BLUETOOTHmod_resource_init();	/*初始化本模块资源*/
	app_BLUETOOTHmod_fun_entry();    	/*本模块主功能入口*/
	DBGMSG("BLUETOOTHmod....\n",LOG_NP,LOG_NP);
	return;
}
/********************************************************************
 * @创建人 :阳永锋
 * @功能 : UART模块卸载入口
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void app_BLUETOOTHmod_exit( void )
{
	app_BLUETOOTHmod_fun_exit();					/*退出本模块主功能*/
	app_global_resouce_release( 0 );          	/*释放占用公共资源*/
	DBGMSG( "BLUETOOTHmod=>", LOG_NP, LOG_NP );
	return;
}
/********************************************************************
 * @创建人 :阳永锋
 * @功能 : 把UART模块加到管理列表
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
Bsize_t app_BLUETOOTHmod_add( void )
{
	app_mod_inlet_t temp;
	Bsize_t mod_id;
	temp.insmod_fun = app_BLUETOOTHmod_insmod;/*模块入口*/
	temp.exitmod_fun = app_BLUETOOTHmod_exit;/*模块出口*/
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



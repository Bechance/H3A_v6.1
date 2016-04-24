/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称: 模块对外连接层
* 创建时间:2013-04-26
* 所属公司 :科荟
* 文件名称:app_RMmod_connect.c
* 作者 :周炳权
* 功能描述:实现模块的对外连接
*-------------------------------------------------------------------*/
#include"app_RM_inside.h"
#include "../../../support\app_support.h"
/********************************************************************
 * @作者 :揭成
 * @功能 :系统设置模块资源初始化
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_RMmod_resource_init( void )
{
	app_module_color_select(2);
	return;
}
/********************************************************************
 * @作者 :揭成
 * @功能 :模块加载入口
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_RMmod_insmod( void )
{
	app_global_resouce_req( 0 );  	/*申请占用公共资源*/
	app_RMmod_resource_init();/*初始化本模块资源*/
	app_RMmod_fun_entry();    /*本模块主功能入口*/
	return;
}
/********************************************************************
 * @作者 :揭成
 * @功能 :模块卸载入口
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_RMmod_exit( void )
{
	app_RMmod_fun_exit();/*退出本模块主功能*/
	app_global_resouce_release(0);
	return;
}
/********************************************************************
 * @作者 :揭成
 * @功能 :把模块加到管理列表
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
Bsize_t app_RMmod_add( void )
{
	app_mod_inlet_t temp;
	Bsize_t mod_id;
	temp.insmod_fun = app_RMmod_insmod;/*模块入口*/
	temp.exitmod_fun = app_RMmod_exit;/*模块出口*/
	mod_id = app_module_add( &temp );
	return mod_id;
}
/*module AUTO Register*/
#pragma DATA_SECTION (app_RMmod_list,"MOD_LIST_ADDR")
const mod_member_t app_RMmod_list[] =
{
	app_RMmod_add,
	"RMETER",
	0x5a5a
};



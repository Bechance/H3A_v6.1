/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称: ERR 管理模块对外连接层
* 创建时间:2010-09-19
* 所属公司 :科荟
* 文件名称:app_ERRmod_connect.c
* 作者 :揭成
* 功能描述:实现改模块的对外连接
*-------------------------------------------------------------------*/
#include"app_ERRmod_inside.h"
#include "../../../support\app_support.h"

/********************************************************************
 * @作者 :揭成
 * @功能 : 系统设置模块资源初始化
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void app_ERRmod_resource_init( void )
{
	app_module_color_select(2);
	return;
}
/********************************************************************
 * @作者 :揭成
 * @功能 : 模块加载入口
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void app_ERRmod_insmod( void )
{
	app_global_resouce_req( 0 );  /*申请占用公共资源*/
	app_ERRmod_resource_init();/*初始化本模块资源*/
	app_ERRmod_fun_entry();    /*本模块主功能入口*/
	DBGMSG( "ERRmod....\n", LOG_NP, LOG_NP );
	return;
}
/********************************************************************
 * @作者 :揭成
 * @功能 : 模块卸载入口
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void app_ERRmod_exit( void )
{
	app_global_resouce_release( 0 );          /*释放占用公共资源*/
	app_ERRmod_fun_exit();/*退出本模块主功能*/
	DBGMSG( "ERRmod=>", LOG_NP, LOG_NP );
	return;
}
/********************************************************************
 * @作者 :揭成
 * @功能 : 把模块加到管理列表
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
Bsize_t app_ERRmod_add( void )
{
	app_mod_inlet_t temp;
	Bsize_t mod_id;
	temp.insmod_fun = app_ERRmod_insmod;/*模块入口*/
	temp.exitmod_fun = app_ERRmod_exit;/*模块出口*/
	mod_id = app_module_add( &temp );
	return mod_id;
}
#pragma DATA_SECTION (app_ERRmod_list,"MOD_LIST_ADDR")
const mod_member_t app_ERRmod_list[]=
{
	app_ERRmod_add,
	"CORRECT",
	0x5a5a
};

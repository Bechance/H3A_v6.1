/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称: 模块对外连接层
* 创建时间:2013-04-26
* 所属公司 :科荟
* 文件名称:app_VMmod_connect.c
* 作者 :周炳权
* 功能描述:实现改模块的对外连接
*-------------------------------------------------------------------*/
#include"app_VM_inside.h"
#include "../../../support\app_support.h"


/********************************************************************
 * @作者 :周炳权
 * @功能 :虚拟电表设置参数保存
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_VMmod_set_data_save( VMMeter_dl_info_t *p_arg, char *m_name )
{
	FIL*fp;
	UINT w;
	fp = fatfs_open(m_name,FA_READ|FA_WRITE |FA_OPEN_EXISTING);
	if(fp)
	{ 
		//f_lseek(fp,fp->fsize);
		fatfs_write_bytes(fp,p_arg,sizeof(VMMeter_dl_info_t),&w);
		fatfs_close(fp);
	}
	else 
	{
		fp=fatfs_open(m_name,FA_READ | FA_WRITE |FA_CREATE_ALWAYS);
		fatfs_write_bytes(fp,p_arg,sizeof(VMMeter_dl_info_t),&w);
		fatfs_close(fp);
	}
	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :虚拟电表设置参数读取
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_VMmod_set_data_read( VMMeter_dl_info_t *p_arg, char *m_name )
{
	FIL*fp;
	UINT w;
	fp=fatfs_open(m_name,FA_READ | FA_WRITE |FA_OPEN_EXISTING);
    if(!fp)
    {
		fp=fatfs_open(m_name,FA_READ | FA_WRITE |FA_CREATE_ALWAYS);
		GLOBAL_MEMCLR(p_arg, sizeof(VMMeter_dl_info_t));
		if(fp)
		{
			fatfs_write_bytes(fp,p_arg,sizeof(VMMeter_dl_info_t),&w);
			fatfs_close(fp);
		}
		return;
	}
	fatfs_read_bytes(fp,p_arg,sizeof(VMMeter_dl_info_t),&w);
	fatfs_close(fp);
	return;
}

/********************************************************************
 * @作者 :揭成
 * @功能 :系统设置模块资源初始化
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_VMmod_resource_init( void )
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
void app_VMmod_insmod( void )
{
	app_global_resouce_req( 0 );  	/*申请占用公共资源*/
	app_VMmod_resource_init();/*初始化本模块资源*/
	app_VMmod_fun_entry();    /*本模块主功能入口*/
	return;
}
/********************************************************************
 * @作者 :揭成
 * @功能 :模块卸载入口
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_VMmod_exit( void )
{
	app_VMmod_fun_exit();/*退出本模块主功能*/
	GUI_resource_release();
	return;
}
/********************************************************************
 * @作者 :揭成
 * @功能 :把模块加到管理列表
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
Bsize_t app_VMmod_add( void )
{
	app_mod_inlet_t temp;
	Bsize_t mod_id;
	temp.insmod_fun = app_VMmod_insmod;/*模块入口*/
	temp.exitmod_fun = app_VMmod_exit;/*模块出口*/
	mod_id = app_module_add( &temp );
	return mod_id;
}

#pragma DATA_SECTION (app_VMmod_list,"MOD_LIST_ADDR")
const mod_member_t app_VMmod_list[]=
{
	app_VMmod_add,
	"VMETER",
	0x5a5a
};


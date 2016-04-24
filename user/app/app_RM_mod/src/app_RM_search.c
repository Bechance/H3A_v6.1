/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:查询模块主体程序
* 创建时间:
* 所属公司 :科荟
* 文件名称:app_Searchmod_view.c
* 作者 :
* 功能描述:实现查询数据显示
******************************************************************************************/
#include 		"app_RM_inside.h"

#define     		_DATA_F          	"RMDATA"

static void  RMdata_obtain(Slist_t*p,RM_save_data_t*pD)
{
    FIL*fp;
    char name[40] = "";
	char nCH[10]  = "";
	UINT w;
	Fileview_path_obtain(name);
	global_strcat( name, "/");
	global_ltoa(p->n, nCH);
	global_strcat( name, nCH);
	global_strcat( name, "/");
	global_strcat( name, _DATA_F);
	fp = fatfs_open(name, FA_OPEN_EXISTING | FA_READ );
	if(!fp) {GLOBAL_MEMCLR(pD,sizeof(RM_save_data_t));return ;}
	f_read(fp,pD,sizeof(RM_save_data_t),&w);
	fatfs_close(fp);
	return;
}
/***********************************************/
//信息详情
/************************************************/
static void RM_record_detail(Slist_t*plist)
{
    uint8_t key;
	RM_save_data_t*p = app_malloc(sizeof(RM_save_data_t));
	RMdata_obtain(plist,p);
	app_RMmod_window_create(1);
	app_RMmod_data_all_display(&p->m_RM_data,0);
	app_global_key_control_power_req();
    while(1)
    {
        msleep(2);
		if ( app_global_key_force_obtain( &key, 1 )== 1 )
		{
			if(key==KEY_EXIT)
			{
				break;
			}
		}
	}
	app_free(p);
    app_RMmod_window_destory();
	app_global_key_control_power_release();
    return;
}

const search_fun_t  m_RM_fun=
{	
   RM_record_detail,
   _DATA_F	
};

/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:查线模块数据查询
* 创建时间:
* 所属公司 :科荟
* 文件名称:app_WIRmod_search.c
* 作者 :
* 功能描述:实现查询数据显示
******************************************************************************************/
#include		    "app_WIRmod_inside.h"

#define     _DATA_F     	"WIRDATA"


void  WIR_record_obtain(Slist_t*p,void*ppD)
{
    FIL*fp;
    char name[40] = "";
	char nCH[10]  = "";
	UINT w;
    WIR_save_arg_t*pD = (WIR_save_arg_t*)ppD;
	Fileview_path_obtain(name);
	global_strcat( name, "/");
	global_ltoa(p->n, nCH);
	global_strcat( name, nCH);
	global_strcat( name, "/");
	global_strcat( name, _DATA_F);
	fp = fatfs_open(name, FA_OPEN_EXISTING | FA_READ );
	if(!fp) {GLOBAL_MEMCLR(pD,sizeof(WIR_save_arg_t));return ;}
	f_read(fp,pD,sizeof(WIR_save_arg_t),&w);
	fatfs_close(fp);
	return;
}

void WIR_record_detail(Slist_t*plist)
{
	uint8_t key;
	WIR_save_arg_t*p = app_malloc(sizeof(WIR_save_arg_t));
	WIR_record_obtain(plist,p);
	app_global_key_control_power_req();
	GUI_window_hwin_req( GET_GUIWIN );
	app_WIRmod_window_creat(1);
	GUI_3D_window_title_mod(m_WIR_win,"信息详情");
	m_alg_presult    = app_malloc( sizeof( WIR_ALG_t ) * 4 );
	GUI_radio_select(m_WIRresjult_win,p->m_offset);
	GUI_window_hwin_release( GET_GUIWIN );
	global_memcpy(m_alg_presult, &p->m_WIR_ALG, sizeof(WIR_ALG_t)*4);
	app_WIRmod_WIRresult_display(p->m_WIR_ALG);
	if ( app_WIRmod_resulst_sys( &m_WIRresult ) )
	{
		app_WIRmod_simpleness_alg(&m_WIRresult);
		app_WIRmod_draw_creat( p->m_WIR_ALG->jxmod );
		app_WIRmod_result_display( &m_WIRresult );
		app_WIRmod_componet_auto_link( &m_WIRresult );
	}
    while(1)
    {
        msleep(1);
		if ( app_global_key_force_obtain( &key, 1 )== 1 )
		{
			if(key==KEY_EXIT)break;
			if(key==KEY_F4)app_WIRmod_redeem_search(&p->m_redeem_arg);
		}
	}
	app_free(p);
    app_WIRmod_window_destroy();
	app_global_key_control_power_release();
	app_free(m_alg_presult);
    return;
}

const search_fun_t  m_WIR_fun=
{	
   WIR_record_detail,
   _DATA_F	
};

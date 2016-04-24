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
#include    "app_XCerrmod_inside.h"

#define     _DATA_F          	"XCERR"


static void  XCERR_obtain(Slist_t*p,Errsave_t*ppD)
{
    FIL*fp;
    char name[40] = "";
	char nCH[10]  = "";
	UINT w;
    Errsave_t*pD = (Errsave_t*)ppD;
	Fileview_path_obtain(name);
	global_strcat( name, "/");
	global_ltoa(p->n, nCH);
	global_strcat( name, nCH);
	global_strcat( name, "/");
	global_strcat( name, _DATA_F);
	fp = fatfs_open(name, FA_OPEN_EXISTING | FA_READ );
	if(!fp) {GLOBAL_MEMCLR(pD,sizeof(Errsave_t));return ;}
	f_read(fp,pD,sizeof(Errsave_t),&w);
	fatfs_close(fp);
	return;
}

/***********************************************/
//信息详情
/************************************************/
static void _DLstr_to_DLf(DL_app_param_str_t*pSrc,measure_param_t*pDes)
{	
	float*  pfdata;
	char*   pch;
	Bsize_t offset;
	GLOBAL_MEMCLR(pDes, sizeof(measure_param_t));
	for(offset=0;offset <(sizeof(measure_param_t)/sizeof(float)-2);offset++ )
	{
		pfdata =&(pDes->phase_a.f_U );
		pch    = (pSrc->phase_a.c_U);
		pfdata =  pfdata+offset;
		pch   += ((DL_DSIZE)*offset);
		global_string_filter(pch,' ');
	   *pfdata = global_atof(pch);
	}
	pch = pSrc->c_Cos;
	if( global_string_locate_first(pch, "L") != PNULL)
		pDes->cos_LC = 'L';
	else if ( global_string_locate_first(pch, "C") != PNULL)
		pDes->cos_LC = 'C';
	else
		pDes->cos_LC = ASCII_NULL;
	pfdata =&pDes->f_Cos;
	pch    = pSrc->c_Cos;
	global_string_filter(pch,' ');
	global_string_filter(pch,'L');
	global_string_filter(pch,'C');
	*pfdata = global_atof(pch);
	return;
}

static void XCERR_record_detail(Slist_t*plist)
{
    uint8_t key;
	Errsave_t*pD = app_malloc(sizeof(Errsave_t));
	XCERR_obtain(plist,pD);
    _DLstr_to_DLf(&pD->DLarg,&g_DL_param);
	global_memcpy(&g_user_arg.DL_app_user_set,&pD->DL_app_user_set, sizeof(DL_app_user_set_t));
	global_memcpy(&g_user_arg.DL_app_user_info,&pD->DL_app_user_info,sizeof(DL_app_user_info_t));
	app_module_color_select(2);
	app_XCerrmod_record_detail_win_creat();
	GUI_3D_window_title_mod(m_XCERR_3DWIN,"信息详情");
    app_XCERR_errPAINT();
	app_XCERR_DLpaint();
	app_global_key_control_power_req();
    while(1)
    {
        msleep(1);
		if ( app_global_key_force_obtain( &key, 1 )== 1 )
		{
			if(key==KEY_EXIT)break;
			else if(key==KEY_F5)
			{
				app_global_key_control_power_release();
				if ( MsgBox(100,100,350,150,(char*)m_XCERRmod_MSG_48[g_language],(char*)m_XCERRmod_MSG_49[g_language],APP_HALF_SEC*100) == GUIE_KEY_ENTER )
				{
					app_XCERRmod_record_print(pD);
				}
				app_global_key_control_power_req();
			}
		}
	}
	app_free(pD);
	app_XCerrmod_window_destory();
	app_global_key_control_power_release();
    return;
}

const search_fun_t  m_XCERR_fun=
{	
   XCERR_record_detail,
   _DATA_F	
};

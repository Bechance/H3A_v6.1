/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:��ѯģ���������
* ����ʱ��:
* ������˾ :����
* �ļ�����:app_Searchmod_view.c
* ���� :
* ��������:ʵ�ֲ�ѯ������ʾ
******************************************************************************************/
#include    "app_TIME_errmod_inside.h"

#define     _DATA_F          	"TMRERR"


static void  TIME_XXdata_obtain(Slist_t*p,Errsave_t*pD)
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
	if(!fp) {GLOBAL_MEMCLR(pD,sizeof(Errsave_t));return ;}
	f_read(fp,pD,sizeof(Errsave_t),&w);
	fatfs_close(fp);
	return;
}
/***********************************************/
/***********************************************/
//��Ϣ����
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
	/*��ȡcos_LC*/
	pDes->cos_LC = pSrc->cos_LC;
	/* ��ȡf_cosֵ */
	pfdata =&pDes->f_Cos;
	pch    = pSrc->c_Cos;
	global_string_filter(pch,' ');
	global_string_filter(pch,'L');
	global_string_filter(pch,'C');
	*pfdata = global_atof(pch);
	return;
}

static void TIME_record_detail(Slist_t*p)
{
    uint8_t key;
	Errsave_t*pD = app_malloc(sizeof(Errsave_t));
	TIME_XXdata_obtain(p,pD);
    _DLstr_to_DLf(&pD->DLarg,&g_DL_param);
	global_memcpy(&g_user_arg.DL_app_user_set,&pD->DL_app_user_set, sizeof(DL_app_user_set_t));
	global_memcpy(&g_user_arg.DL_app_user_info,&pD->DL_app_user_info,sizeof(DL_app_user_info_t));
	app_module_color_select(2);
	app_TIME_errmod_window_creat();
	GUI_3D_window_title_mod(m_TIME_ERR_3DWIN,"��Ϣ����");
    app_TMR_err_rePAINT();
	app_TMR_DLpaint();
	app_global_key_control_power_req();
    while(1)
    {
        msleep(1);
		if ( app_global_key_force_obtain( &key, 1 )== 1 )
		{
			if(key==KEY_EXIT)break;
		}
	}
	app_free(pD);
	app_TIME_errmod_window_destroy();
	app_global_key_control_power_release();
    return;
}


const search_fun_t  m_TIME_fun=
{	
   TIME_record_detail,
   _DATA_F	
};
/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������: ģ��������Ӳ�
* ����ʱ��:2013-04-26
* ������˾ :����
* �ļ�����:app_VMmod_connect.c
* ���� :�ܱ�Ȩ
* ��������:ʵ�ָ�ģ��Ķ�������
*-------------------------------------------------------------------*/
#include"app_VM_inside.h"
#include "../../../support\app_support.h"


/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :���������ò�������
 * @���� :NONE
 * @��� :NONE
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
 * @���� :�ܱ�Ȩ
 * @���� :���������ò�����ȡ
 * @���� :NONE
 * @��� :NONE
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
 * @���� :�ҳ�
 * @���� :ϵͳ����ģ����Դ��ʼ��
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_VMmod_resource_init( void )
{
	app_module_color_select(2);
	return;
}
/********************************************************************
 * @���� :�ҳ�
 * @���� :ģ��������
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_VMmod_insmod( void )
{
	app_global_resouce_req( 0 );  	/*����ռ�ù�����Դ*/
	app_VMmod_resource_init();/*��ʼ����ģ����Դ*/
	app_VMmod_fun_entry();    /*��ģ�����������*/
	return;
}
/********************************************************************
 * @���� :�ҳ�
 * @���� :ģ��ж�����
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_VMmod_exit( void )
{
	app_VMmod_fun_exit();/*�˳���ģ��������*/
	GUI_resource_release();
	return;
}
/********************************************************************
 * @���� :�ҳ�
 * @���� :��ģ��ӵ������б�
 * @���� :NONE
 * @��� :NONE
********************************************************************/
Bsize_t app_VMmod_add( void )
{
	app_mod_inlet_t temp;
	Bsize_t mod_id;
	temp.insmod_fun = app_VMmod_insmod;/*ģ�����*/
	temp.exitmod_fun = app_VMmod_exit;/*ģ�����*/
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


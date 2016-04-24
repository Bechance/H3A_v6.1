/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:走字上传程序
* 创建时间:2013-11-21
* 所属公司 :科荟
* 文件名称:upload.c
* 作者 :周炳权
* 功能描述:实现走字测量数据上传
******************************************************************************************/
#include		    "app_Esummod_inside.h"


hwin_t* m_upload_WIN  = PNULL;
hwin_t* m_upload_edit = PNULL;
FIL	   *m_upload_flie = PNULL;

static uint8_t _uploadtask_id = INVALID_PRO;
uint32_t g_upload_all_size = 0;
uint32_t g_upload_cur_num = 0;
uint8_t  g_upload_start = 0;
dev_id_t g_upload_dev_id = INVALID_DEV_ID;

#define			UPLOAD_FILE			"ESUM"
#define 		ALL_SIZE_DI			41
/* 手动校验记录上传信息前缀*/
const char *upload_symbol_tbl[] =
{
	"RECORD=","WORKID=","UID=   ","UNAME= ","BH=    ",		
	"WORKER=","TIME=  ","STIME= ","ETIME= ","BL=    ","EG1=   ","EG2=   ","RSEC=   ",
	"TEMP=  ","HNM=   ","UA=    ","UB=    ","UC=    ",		
	"IA=    ","IB=    ","IC=    ","JA=    ","JB=    ",		
	"JC=    ","PA=    ","PB=    ","PC=    ","QA=    ",		
	"QB=    ","QC=    ","JIAC=  ","JIBC=  ","JIAB=  ",	
	"JUAC=  ","JUBC=  ","JUAB=  ","COS=   ","PABC=  ",	
	"QABC=  ","FEQ=   ","COSLC= ",	
};
/********************************************************************
 * @作者 :周炳权
 * @功能 :创建窗口
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_upload_window_create(void) 
{ 
	GUI_window_hwin_req(GET_GUIWIN);
  	GUI_set_Bgcolor(799);
	GUI_set_Fgcolor(C_GUI_WHITE);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
    GUI_clean_screen();
	m_upload_WIN=Gui_3D_window_creat(0,2,636,475,"走字测量数据上传",WIN_3D | WIN_FILL,GET_GUIWIN);
    GUI_window_hwin_req(m_upload_WIN);
	m_upload_edit
		= GUI_edit_creat(70,90,485,230,WIN_3D | WIN_TOUCH_EN,m_upload_WIN);
	GUI_window_hwin_req( m_upload_edit );
	GUI_set_Bgcolor(799);
	GUI_set_Fgcolor(C_GUI_WHITE);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_string_display_At(30,20, "总存储数据条数:" );
	GUI_string_display_At(30,70, "已上传数据条数:" );
	GUI_string_display_At(80,190, "按<退出>返回上一级." );
	GUI_window_hwin_release(m_upload_edit);	
	GUI_window_hwin_release(m_upload_WIN);		
	GUI_window_hwin_release(GET_GUIWIN);
	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :界面销毁
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_upload_window_destory(void)
{
	GUI_window_destory(m_upload_edit);
    GUI_window_destory(m_upload_WIN);
	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :提示
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
static void app_upload_tips(char *m_temp)
{
	GUI_window_hwin_req( m_upload_edit );
	GUI_set_Bgcolor(799);
	GUI_set_Fgcolor(C_GUI_GREEN);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_string_display_At(30,120, m_temp );
	GUI_window_hwin_release(m_upload_edit);
	return;
}

static void app_upload_allsize(uint32_t m_temp)
{
	GUI_window_hwin_req( m_upload_edit );
	GUI_set_Bgcolor(799);
	GUI_set_Fgcolor(C_GUI_GREEN);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_int_display_At(230, 20, (long)m_temp);
	GUI_window_hwin_release(m_upload_edit);
	return;
}

static void app_upload_cursize(uint32_t m_temp)
{
	GUI_window_hwin_req( m_upload_edit );
	GUI_set_Bgcolor(799);
	GUI_set_Fgcolor(C_GUI_GREEN);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_int_display_At(230, 70, (long)m_temp);
	GUI_window_hwin_release(m_upload_edit);
	return;
}

/****************************************************
* @作者 :周炳权
* @功能 :读取一条数据；
* @输入 :pdata:当前数据偏移数;
* @输出 :0:无数据;1:有数据；
****************************************************/
static uint8_t cur_one_record_obtain(Esum_save_arg_t *pdata,uint32_t m_offset )
{
	uint16_t w;
	uint32_t fleek;	
    if(f_open(m_upload_flie,UPLOAD_FILE,FA_OPEN_EXISTING|FA_READ)!=FR_OK)
    { 
		return 0;
   	}
	fleek = m_offset*sizeof(Esum_save_arg_t);
	f_lseek(m_upload_flie,fleek);
    f_read(m_upload_flie,pdata,sizeof(Esum_save_arg_t),&w);
    f_close(m_upload_flie);
	return (w/sizeof(Esum_save_arg_t));
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :获取总数据条数
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
uint32_t data_allsize_obtain( void )
{
	uint32_t m_num = 0;
	uint32_t fsize;
	uint32_t fboxsize;	
    if(f_open(m_upload_flie,UPLOAD_FILE,FA_OPEN_EXISTING|FA_READ)!=FR_OK)
    { 
		return 0;
   	}
    fsize = m_upload_flie->fsize;
	fboxsize = sizeof(Esum_save_arg_t);	
    m_num=(fsize+fboxsize-1)/fboxsize;
	f_close(m_upload_flie);
	return (m_num);
}

/********************************************************************
 * @作者:周炳权
 * @功能:串口接收协议分析
 * @输入:NONE
 * @输出:成功返回1，失败返回0
********************************************************************/
uint8_t Usart_read_analysis( void )
{
	uint8_t frame_head;
	uint8_t m_buf[20] = {0,};
	if ( os_driver_read( g_upload_dev_id,(uint8_t *)&frame_head, 1,0 ) == 1 )
	{	
		if (  frame_head == 0x68 )
		{	
			msleep( 3 );				//等待串口接收完成
			if ( os_driver_read( g_upload_dev_id, (uint8_t *)m_buf, 8,0 ) != 8 )
				return 0;
		}
		else return 0;
		if ( m_buf[7] == 0x11 )
		{
			if ( os_driver_read( g_upload_dev_id, (uint8_t *)&m_buf[8], 6,0 ) != 6 )
				return 0;
		}
		if ( m_buf[10] == 0x02 && m_buf[11] == 0x01 && m_buf[13] == 0x16 )
		{
			return 1;
		}
	}
	return 0;
}
/********************************************************************
 * @作者 :周炳权
 * @功能 :发送数据
 * @输入 :m_buf:数据缓存;m_size:数据长度;
 * @输出 :crc
********************************************************************/
static uint8_t send_data_content( void* m_sbuf )
{
	uint32_t m_crc = 0;
	uint8_t m_offset = 0;
	uint8_t m_temp_buf[45] = { 0, };
	Esum_save_arg_t *m_pdata = m_sbuf;

	GLOBAL_MEMCLR(m_temp_buf, 45);
	
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_ltoa(g_upload_cur_num, (char *)&m_temp_buf[8]);
	global_memcpy((char *)&m_temp_buf[13], "/",1);
	global_ltoa(g_upload_all_size, (char *)&m_temp_buf[15]);
	global_memcpy((char *)&m_temp_buf[21], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],22, 0 );
	m_crc = global_sum_array((char*)&m_temp_buf[0],22,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DL_app_user_info.wrok_list,30);
	global_memcpy((char *)&m_temp_buf[39], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],40, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],40,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DL_app_user_info.user_id,30);
	global_memcpy((char *)&m_temp_buf[39], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],40, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],40,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DL_app_user_info.user_name,30);
	global_memcpy((char *)&m_temp_buf[39], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],40, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],40,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DL_app_user_info.meter_id1,30);
	global_memcpy((char *)&m_temp_buf[39], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],40, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],40,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DL_app_user_info.worker,30);
	global_memcpy((char *)&m_temp_buf[39], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],40, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],40,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DL_app_user_info.time_buf,20);
	global_memcpy((char *)&m_temp_buf[29], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],30, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],30,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->Esum_arg.start_time,20);
	global_memcpy((char *)&m_temp_buf[29], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],30, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],30,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->Esum_arg.end_time,20);
	global_memcpy((char *)&m_temp_buf[29], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],30, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],30,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->Esum_arg.BL,10);
	global_memcpy((char *)&m_temp_buf[19], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],20, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],20,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->Esum_arg.Eg_1,20);
	global_memcpy((char *)&m_temp_buf[29], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],30, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],30,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->Esum_arg.Eg_2,20);
	global_memcpy((char *)&m_temp_buf[29], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],30, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],30,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->Esum_arg.second,10);
	global_memcpy((char *)&m_temp_buf[19], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],20, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],20,IS_CHAR);
	m_offset++;
	if ( m_pdata->DL_app_user_info.temp_buf[0] < 0x30 )
		m_pdata->DL_app_user_info.temp_buf[0] = 0;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DL_app_user_info.temp_buf,10);
	global_memcpy((char *)&m_temp_buf[19], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],20, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],20,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DL_app_user_info.hnm_buf,10);
	global_memcpy((char *)&m_temp_buf[19], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],20, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],20,IS_CHAR);
	m_offset++;

	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DLarg.phase_a.c_U,10);
	global_memcpy((char *)&m_temp_buf[19], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],20, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],20,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DLarg.phase_b.c_U,10);
	global_memcpy((char *)&m_temp_buf[19], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],20, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],20,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DLarg.phase_c.c_U,10);
	global_memcpy((char *)&m_temp_buf[19], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],20, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],20,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DLarg.phase_a.c_I,10);
	global_memcpy((char *)&m_temp_buf[19], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],20, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],20,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DLarg.phase_b.c_I,10);
	global_memcpy((char *)&m_temp_buf[19], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],20, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],20,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DLarg.phase_c.c_I,10);
	global_memcpy((char *)&m_temp_buf[19], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],20, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],20,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DLarg.phase_a.c_J,10);
	global_memcpy((char *)&m_temp_buf[19], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],20, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],20,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DLarg.phase_b.c_J,10);
	global_memcpy((char *)&m_temp_buf[19], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],20, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],20,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DLarg.phase_c.c_J,10);
	global_memcpy((char *)&m_temp_buf[19], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],20, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],20,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DLarg.phase_a.c_P,10);
	global_memcpy((char *)&m_temp_buf[19], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],20, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],20,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DLarg.phase_b.c_P,10);
	global_memcpy((char *)&m_temp_buf[19], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],20, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],20,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DLarg.phase_c.c_P,10);
	global_memcpy((char *)&m_temp_buf[19], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],20, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],20,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DLarg.phase_a.c_Q,10);
	global_memcpy((char *)&m_temp_buf[19], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],20, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],20,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DLarg.phase_b.c_Q,10);
	global_memcpy((char *)&m_temp_buf[19], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],20, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],20,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DLarg.phase_c.c_Q,10);
	global_memcpy((char *)&m_temp_buf[19], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],20, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],20,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DLarg.c_JIac,10);
	global_memcpy((char *)&m_temp_buf[19], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],20, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],20,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DLarg.c_JIbc,10);
	global_memcpy((char *)&m_temp_buf[19], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],20, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],20,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DLarg.c_JIab,10);
	global_memcpy((char *)&m_temp_buf[19], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],20, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],20,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DLarg.c_JUac,10);
	global_memcpy((char *)&m_temp_buf[19], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],20, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],20,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DLarg.c_JUbc,10);
	global_memcpy((char *)&m_temp_buf[19], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],20, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],20,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DLarg.c_JUab,10);
	global_memcpy((char *)&m_temp_buf[19], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],20, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],20,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DLarg.c_Cos,10);
	global_memcpy((char *)&m_temp_buf[19], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],20, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],20,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DLarg.c_Pabc,10);
	global_memcpy((char *)&m_temp_buf[19], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],20, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],20,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DLarg.c_Qabc,10);
	global_memcpy((char *)&m_temp_buf[19], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],20, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],20,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], m_pdata->DLarg.c_Feq,10);
	global_memcpy((char *)&m_temp_buf[19], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],20, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],20,IS_CHAR);
	m_offset++;
	global_memcpy((void *)&m_temp_buf[0],(void *)upload_symbol_tbl[m_offset],7);
	global_memcpy((char *)&m_temp_buf[8], (char *)&m_pdata->DLarg.cos_LC,1);
	global_memcpy((char *)&m_temp_buf[19], ",",1);
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_temp_buf[0],20, 0 );
	m_crc += global_sum_array((char*)&m_temp_buf[0],20,IS_CHAR);
	m_offset++;
	os_driver_write( g_upload_dev_id, (void*)";\n",3, 0 );
	
	return (uint8_t)(m_crc & 0xff);
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :发送数据
 * @输入 :m_buf:数据缓存;m_size:数据长度;
 * @输出 :NONE
********************************************************************/
void app_upload_send(void* m_sbuf, uint16_t m_size )
{
	uint8_t m_crc = 0;
	uint8_t m_temp = 0;
	char m_buf[20] = { 0, };
	uint16_t m_size2 = 977;
	
	m_buf[0]  = 0x68;
	m_buf[1]  = 0x99;
	m_buf[2]  = 0x99;
	m_buf[3]  = 0x99;
	m_buf[4]  = 0x99;
	m_buf[5]  = 0x99;
	m_buf[6]  = 0x99;
	m_buf[7]  = 0x68;
	if ( (g_upload_cur_num +1) == g_upload_all_size )
		 m_buf[8]  = 0xb1;
	else m_buf[8]  = 0x91;
	m_buf[9]  = (uint8_t)(m_size2 >> 8);
	m_buf[10] = (uint8_t)(m_size2 & 0xff);
	m_buf[11] = 0x02;
	m_buf[12] = 0x01;
	m_temp = global_sum_array((char*)&m_buf[0],13,IS_CHAR);
	
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_buf[0],13, 0 );
	m_crc = send_data_content(m_sbuf);
	m_crc += m_temp;
	m_buf[18] = (uint8_t)(m_crc & 0xff);
	m_buf[19] = 0x16;
	os_driver_write( g_upload_dev_id, (uint8_t*)&m_buf[18],2, 0 );
	return;
}

/********************************************************************
* @作者 :周炳权
* @功能 :数据上传任务
* @输入 :NONE
* @输出 :NONE
********************************************************************/
static void _upload_task(void*arg)
{
	uint8_t m_flag = 0;
	Esum_save_arg_t *m_pdata = app_malloc(sizeof(Esum_save_arg_t));
	while(1)
	{
		msleep( TYPE_DELAY );
		if ( os_task_delete_req( SELF_PRO ) == OS_TASK_DEL_REQ )
		{
			app_free(m_pdata);
			os_task_delete( SELF_PRO );
		}
		if ( g_upload_start == 1 )
		{
			if ( (g_upload_cur_num == g_upload_all_size) && (g_upload_all_size != 0))
			{
				app_upload_tips("全部数据已上传完成...");
				m_flag = 0;
				g_upload_start = 0;
				continue;
			}
			if ( m_flag == 0 )
			{
				m_flag = 1;
				g_upload_all_size = data_allsize_obtain();
				if ( g_upload_all_size == 0 )
				{
					app_upload_tips("无数据可以上传...");
					m_flag = 0;
					continue;
				}
				app_upload_allsize(g_upload_all_size);
				app_upload_cursize(g_upload_cur_num);
				app_upload_tips("正在上传数据...");
			}
			cur_one_record_obtain(m_pdata,g_upload_cur_num);
			app_upload_send(m_pdata,sizeof(Esum_save_arg_t));
			g_upload_cur_num++;
			app_upload_cursize(g_upload_cur_num);
			msleep(10);
			if ( g_upload_cur_num == g_upload_all_size )
			{
				app_upload_tips("全部数据已上传完成...");
				m_flag = 0;
				g_upload_start = 0;
			}
		}
	}
}
/********************************************************************
 * @作者 :周炳权
 * @功能 :主任务
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_upload_main( void )
{
	uint8_t key_buf;
	uint8_t m_CRC = 0;//无校验
	uint32_t m_baud = 38400;//波特率
	_uploadtask_id = os_task_assign_stk_build( _upload_task,3);
	
	g_upload_dev_id = os_driver_open( "D232" );
	os_driver_ioctl(g_upload_dev_id, SET_SERIAL_CHECK, &m_CRC);
	os_driver_ioctl(g_upload_dev_id, SET_SERIAL_BAUD, &m_baud);
	app_upload_window_create();
	app_upload_tips("等待上传数据...");
	g_upload_all_size = 0;
	g_upload_cur_num = 0;
	g_upload_start = 0;
	while (1)
	{
		msleep( TYPE_DELAY );
		if ( (Usart_read_analysis() == 1) && (g_upload_start == 0) )
		{
			g_upload_start = 1;
			g_upload_all_size = 0;
			g_upload_cur_num = 0;
		}
		if ( app_global_key_obtain( &key_buf, 1 ) != 1 )  //从驱动读取按键值
		{
			continue;
		}
		if ( key_buf == KEY_EXIT)
		{
			app_upload_window_destory();
			if ( _uploadtask_id != INVALID_PRO )
			{
				OS_DELETE_REQ( _uploadtask_id );
				_uploadtask_id = INVALID_PRO;
			}
			break;
		}
	}
	return;
}




/********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:系统设置模块界面函数集合
* 创建时间:2013-04-26
* 所属公司 :科荟
* 文件名称:app_VMmod_interface.c
* 创建人:揭成
* 功能描述:提供界面画屏函数API
******************************************************************************************/
#include"app_VM_inside.h"

hwin_t *m_VMwin_pWIN;
hwin_t *m_VMwin_edit;
hwin_t* m_VM_droplist_guiyue = PNULL;
hwin_t* m_VM_droplist_baud 	 = PNULL;
hwin_t* m_VM_droplist_crc 	 = PNULL;

const char *m_VMagreement_tip[]=
{
	" 国标DL/T 645-1997  ",
	" 国标DL/T 645-2007  ",
};
const char*m_VMbaud_tip[]	={"  300 ","  600 "," 1200 "," 2400 "," 4800 "," 9600 ","19200 "};
const char*m_VMcrc_tip[]	={" None "," Odd "," Even "};

#define X_VMSTAR 40
#define X_VMGAP  (85+10)
#define Y_VMGAP  26

/********************************************************************
* @作者 :周炳权
* @功能 :规约显示
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void app_VMmod_guiyue_display(uint8_t m_offset)
{
	GUI_drop_list_member_assign(m_VM_droplist_guiyue,m_offset);
	return;
}
/********************************************************************
* @作者 :周炳权
* @功能 :波特率显示
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void app_VMmod_baud_display(uint8_t m_offset)
{
	GUI_drop_list_member_assign(m_VM_droplist_baud,m_offset);
	return;
}
/********************************************************************
* @作者 :周炳权
* @功能 :校验方式显示
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void app_VMmod_CRC_display(uint8_t m_offset)
{
	GUI_drop_list_member_assign(m_VM_droplist_crc,m_offset);
	return;
}

/********************************************************************
* @作者 :周炳权
* @功能 :表地址显示
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void app_VMmod_meter_addr_display(void *pdata)
{
	GUI_window_hwin_req( m_VMwin_edit );
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_YELLOW);
	GUI_edit_string_display_At(pdata,EDIT_M_ADDR);
	GUI_window_hwin_release( m_VMwin_edit );
	return;
}

/********************************************************************
* @作者 :周炳权
* @功能 :主编辑窗口参数显示
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void app_VMmod_main_edit_display( VMMeter_dl_info_t * m_data_buf )
{
	if ( m_VMoffset_guiyue == 0 )
		global_memcpy(m_VMmeter_addr,VMmeter_data_buf.meter_Addr, 13);
	else if (  m_VMoffset_guiyue == 1 )
		global_memcpy(m_VMmeter07_addr,VMmeter_data_buf.meter_Addr, 13);
	GUI_window_hwin_req( m_VMwin_edit );
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_YELLOW);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	
	GUI_drop_list_member_assign(m_VM_droplist_baud,m_data_buf->baud);
	GUI_drop_list_member_assign(m_VM_droplist_crc,m_data_buf->crc);
	GUI_edit_string_display_At(m_data_buf->meter_Addr,EDIT_M_ADDR);
	
	GUI_edit_string_display_At(m_data_buf->zy_power_ALL,EDIT_Z_1);
	GUI_edit_string_display_At(m_data_buf->zy_power_J,EDIT_J_1);
	GUI_edit_string_display_At(m_data_buf->zy_power_F,EDIT_F_1);
	GUI_edit_string_display_At(m_data_buf->zy_power_P,EDIT_P_1);
	GUI_edit_string_display_At(m_data_buf->zy_power_G,EDIT_G_1);

	GUI_edit_string_display_At(m_data_buf->zw_power_ALL,EDIT_Z_2);
	GUI_edit_string_display_At(m_data_buf->zw_power_J,EDIT_J_2);
	GUI_edit_string_display_At(m_data_buf->zw_power_F,EDIT_F_2);
	GUI_edit_string_display_At(m_data_buf->zw_power_P,EDIT_P_2);
	GUI_edit_string_display_At(m_data_buf->zw_power_G,EDIT_G_2);

	GUI_edit_string_display_At(m_data_buf->fy_power_ALL,EDIT_Z_3);
	GUI_edit_string_display_At(m_data_buf->fy_power_J,EDIT_J_3);
	GUI_edit_string_display_At(m_data_buf->fy_power_F,EDIT_F_3);
	GUI_edit_string_display_At(m_data_buf->fy_power_P,EDIT_P_3);
	GUI_edit_string_display_At(m_data_buf->fy_power_G,EDIT_G_3);

	GUI_edit_string_display_At(m_data_buf->fw_power_ALL,EDIT_Z_4);
	GUI_edit_string_display_At(m_data_buf->fw_power_J,EDIT_J_4);
	GUI_edit_string_display_At(m_data_buf->fw_power_F,EDIT_F_4);
	GUI_edit_string_display_At(m_data_buf->fw_power_P,EDIT_P_4);
	GUI_edit_string_display_At(m_data_buf->fw_power_G,EDIT_G_4);

	GUI_edit_string_display_At(m_data_buf->voltage_A,EDIT_A_U);
	GUI_edit_string_display_At(m_data_buf->voltage_B,EDIT_B_U);
	GUI_edit_string_display_At(m_data_buf->voltage_C,EDIT_C_U);
	GUI_edit_string_display_At(m_data_buf->current_A,EDIT_A_I);
	GUI_edit_string_display_At(m_data_buf->current_B,EDIT_B_I);
	GUI_edit_string_display_At(m_data_buf->current_C,EDIT_C_I);

	GUI_edit_string_display_At(m_data_buf->meter_time_year,EDIT_YEAR);
	GUI_edit_string_display_At(m_data_buf->meter_time_month,EDIT_MONTH);
	GUI_edit_string_display_At(m_data_buf->meter_time_day,EDIT_DAY);
	GUI_edit_string_display_At(m_data_buf->meter_time_hour,EDIT_HOUR);
	GUI_edit_string_display_At(m_data_buf->meter_time_minute,EDIT_MINUTE);
	GUI_edit_string_display_At(m_data_buf->meter_time_second,EDIT_SECOND);
	
	GUI_window_hwin_release( m_VMwin_edit );
	return;
}
/********************************************************************
* @作者 :周炳权
* @功能 :初始化设置显示
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void app_VMmod_set_init_display(void)
{
	if ( VMmeter_data_buf.m_crc_flag == 0x5a )
	{
		app_VMmod_guiyue_display(VMmeter_data_buf.guiyue);
		app_VMmod_baud_display(VMmeter_data_buf.baud);
		app_VMmod_CRC_display(VMmeter_data_buf.crc);
		app_VMmod_meter_addr_display(VMmeter_data_buf.meter_Addr);
		app_VMmod_main_edit_display(&VMmeter_data_buf);
		app_VMmod_baud_select(VMmeter_data_buf.baud); 	//设置波特率
		app_VMmod_CRC_select(VMmeter_data_buf.crc);	//设置校验方式
		m_VMoffset_guiyue 	= VMmeter_data_buf.guiyue;
		m_VMoffset_baud 	= VMmeter_data_buf.baud;
		m_VMoffset_crc 		= VMmeter_data_buf.crc;
		if ( m_VMoffset_guiyue == 0 )
			global_memcpy(m_VMmeter_addr,VMmeter_data_buf.meter_Addr, 13);
		else if (  m_VMoffset_guiyue == 1 )
			global_memcpy(m_VMmeter07_addr,VMmeter_data_buf.meter_Addr, 13);
	}
	else
	{
		app_VMmod_guiyue_display(0);
		app_VMmod_baud_display(0);
		app_VMmod_CRC_display(0);
		app_VMmod_meter_addr_display(m_VMmeter_addr);
	}
	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :创建窗口
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_VMmod_window_create(void) 
{ 
	uint8_t m_offset;
	const char *m_title[2] 		= {"虚拟电能表",""};
	const char *m_guiye[2] 		= {"虚拟表规约:",""};
	const char *m_bps[2] 		= {"通讯波特率: ",""};
	const char *m_crc[2] 		= {"校验方式:",""};
	const char *m_addr[2] 		= {"虚拟表地址:",""};
	const char *m_zong[2] 		= {"总",""};
	const char *m_jian[2] 		= {"尖",""};
	const char *m_feng[2] 		= {"峰",""};
	const char *m_ping[2] 		= {"平",""};
	const char *m_gu[2] 		= {"谷",""};
	const char *m_virtual[2] 	= {"当前虚拟电量:",""};
	const char *m_zy[2] 		= {"正向有功:",""};
	const char *m_zw[2] 		= {"正向无功:",""};
	const char *m_fy[2] 		= {"反向有功:",""};
	const char *m_fw[2] 		= {"反向无功:",""};
	const char *m_VU[2] 		= {"虚拟电压:",""};
	const char *m_VI[2] 		= {"虚拟电流:",""};
	const char *m_AP[2] 		= {"A相",""};
	const char *m_BP[2] 		= {"B相",""};
	const char *m_CP[2] 		= {"C相",""};
	const char *m_Mtime[2] 		= {"表时间:",""};
	const char *m_year[2] 		= {"年",""};
	const char *m_month[2] 		= {"月",""};
	const char *m_day[2] 		= {"日",""};
	const char *m_hour[2] 		= {"时",""};
	const char *m_min[2] 		= {"分",""};
	const char *m_sec[2] 		= {"秒",""};
	const char *m_unit[2] 		= {"单位:",""};
	const char *m_YGunit[2] 	= {"有功(kWh)",""};
	const char *m_WGunit[2] 	= {"无功(kvarh).",""};
	const char *m_Uunit[2] 		= {"电 压(V).",""};
	const char *m_Iunit[2] 		= {"电 流(A)",""};
	
	GUI_window_hwin_req(GET_GUIWIN);
	GUI_set_Bgcolor(VM_BG_COLOR);
	GUI_clean_screen();
	GUI_set_Bgcolor(VM_WIN_BG_COLOR);    
	GUI_set_Fgcolor(VM_FG_COLOR); 

    m_VMwin_pWIN=Gui_3D_window_creat(0,2,636,476,(char *)m_title[g_language],WIN_3D | WIN_FILL,GET_GUIWIN);
    GUI_window_hwin_req(m_VMwin_pWIN);
	m_VMwin_edit
		= GUI_edit_creat(5,10,600,420,WIN_TOUCH_EN,m_VMwin_pWIN);
	GUI_window_hwin_req( m_VMwin_edit );
	GUI_set_Bgcolor(VM_WIN_BG_COLOR);    
	GUI_set_Fgcolor(VM_FG_COLOR);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	
	GUI_string_display_At(30, 	Y_VMGAP*0, (char *)m_guiye[g_language]);
	GUI_string_display_At(30, 	(Y_VMGAP+7)*1+10, (char *)m_bps[g_language]);
	GUI_string_display_At(295, 	(Y_VMGAP+7)*1+10, (char *)m_crc[g_language]);

	m_VM_droplist_guiyue =
		Gui_drop_list_creat(165,Y_VMGAP*0,m_VMagreement_tip,SIZE_OF_ARRAY(m_VMagreement_tip),GUI_KEY_GUIYUE_SELECT,m_VMwin_edit);
	m_VM_droplist_baud =
		Gui_drop_list_creat(165,(Y_VMGAP+7)*1+9,m_VMbaud_tip,SIZE_OF_ARRAY(m_VMbaud_tip),GUI_KEY_BAUD_SELECT,m_VMwin_edit);
	m_VM_droplist_crc =
		Gui_drop_list_creat(405,(Y_VMGAP+7)*1+9,m_VMcrc_tip,SIZE_OF_ARRAY(m_VMcrc_tip),GUI_KEY_CRC_SELECT,m_VMwin_edit);	

    GUI_WINDIS_HLINE(30,600, (Y_VMGAP+7)*2+20);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_edit_memeber_add(30, (Y_VMGAP+7)*2+25, 12,(char *)m_addr[g_language], MEMBER_3D|EDIT_M_ADDR);

	GUI_string_display_At(X_VMSTAR+X_VMGAP*0+102, Y_VMGAP*5+15,(char *)m_zong[g_language]);
	GUI_string_display_At(X_VMSTAR+X_VMGAP*1+102, Y_VMGAP*5+15,(char *)m_jian[g_language]);
	GUI_string_display_At(X_VMSTAR+X_VMGAP*2+102, Y_VMGAP*5+15,(char *)m_feng[g_language]);
	GUI_string_display_At(X_VMSTAR+X_VMGAP*3+102, Y_VMGAP*5+15,(char *)m_ping[g_language]);
	GUI_string_display_At(X_VMSTAR+X_VMGAP*4+102, Y_VMGAP*5+15,(char *)m_gu[g_language]);
	
	GUI_set_Fgcolor(C_GUI_GREEN);
	GUI_string_display_At(35, Y_VMGAP*4+18,(char *)m_virtual[g_language]);
	GUI_set_Fgcolor(VM_FG_COLOR);
	
	GUI_string_display_At(X_VMSTAR, Y_VMGAP*6+10,(char *)m_zy[g_language]);
	GUI_string_display_At(X_VMSTAR, Y_VMGAP*7+10,(char *)m_zw[g_language]);
	GUI_string_display_At(X_VMSTAR, Y_VMGAP*8+10,(char *)m_fy[g_language]);
	GUI_string_display_At(X_VMSTAR, Y_VMGAP*9+10,(char *)m_fw[g_language]);
	for(m_offset=0;m_offset<5;m_offset++)
	{
		GUI_edit_memeber_add(X_VMSTAR+X_VMGAP*m_offset+70,Y_VMGAP*6+10,8,"", MEMBER_3D|(5+4*m_offset));
		GUI_edit_memeber_add(X_VMSTAR+X_VMGAP*m_offset+70,Y_VMGAP*7+10,8,"", MEMBER_3D|(6+4*m_offset));
		GUI_edit_memeber_add(X_VMSTAR+X_VMGAP*m_offset+70,Y_VMGAP*8+10,8,"", MEMBER_3D|(7+4*m_offset));
		GUI_edit_memeber_add(X_VMSTAR+X_VMGAP*m_offset+70,Y_VMGAP*9+10,8,"", MEMBER_3D|(8+4*m_offset));
	}
	GUI_string_display_At(X_VMSTAR, Y_VMGAP*11+10,(char *)m_VU[g_language]);
	GUI_string_display_At(X_VMSTAR, Y_VMGAP*12+10,(char *)m_VI[g_language]);
	GUI_set_Fgcolor(C_GUI_YELLOW);
	GUI_string_display_At(X_VMSTAR+X_VMGAP*0+94, Y_VMGAP*10+13,(char *)m_AP[g_language]);
	GUI_set_Fgcolor(C_GUI_RED);
	GUI_string_display_At(X_VMSTAR+X_VMGAP*1+94, Y_VMGAP*10+13,(char *)m_BP[g_language]);
	GUI_set_Fgcolor(C_GUI_GREEN);
	GUI_string_display_At(X_VMSTAR+X_VMGAP*2+94, Y_VMGAP*10+13,(char *)m_CP[g_language]);
	GUI_set_Fgcolor(VM_FG_COLOR);
	for(m_offset=0;m_offset<3;m_offset++)
	{
		GUI_edit_memeber_add(X_VMSTAR+X_VMGAP*m_offset+70,Y_VMGAP*11+10, 8, "", MEMBER_3D|25+3*m_offset);
		GUI_edit_memeber_add(X_VMSTAR+X_VMGAP*m_offset+70,Y_VMGAP*12+10, 8, "", MEMBER_3D|26+3*m_offset);
	}
	GUI_string_display_At(X_VMSTAR+16,Y_VMGAP*13+20,(char *)m_Mtime[g_language]);
	GUI_edit_memeber_add(10+100,Y_VMGAP*13+20,4,"", MEMBER_3D|EDIT_YEAR);
	GUI_string_display_At(10+148,Y_VMGAP*13+22,(char *)m_year[g_language]);
	GUI_edit_memeber_add(10+173,Y_VMGAP*13+20,2,"", MEMBER_3D|EDIT_MONTH);
	GUI_string_display_At(10+210,Y_VMGAP*13+22,(char *)m_month[g_language]);
	GUI_edit_memeber_add(10+173+54*1,Y_VMGAP*13+20,2,"", MEMBER_3D|EDIT_DAY);
	GUI_string_display_At(10+210+54*1,Y_VMGAP*13+22,(char *)m_day[g_language]);
	GUI_edit_memeber_add(10+173+54*2,Y_VMGAP*13+20,2,"", MEMBER_3D|EDIT_HOUR);
	GUI_string_display_At(10+210+54*2,Y_VMGAP*13+22,(char *)m_hour[g_language]);
	GUI_edit_memeber_add(10+173+54*3,Y_VMGAP*13+20,2,"", MEMBER_3D|EDIT_MINUTE);
	GUI_string_display_At(10+210+54*3,Y_VMGAP*13+22,(char *)m_min[g_language]);
	GUI_edit_memeber_add(10+173+54*4,Y_VMGAP*13+20,2,"", MEMBER_3D|EDIT_SECOND);
	GUI_string_display_At(10+210+54*4,Y_VMGAP*13+22,(char *)m_sec[g_language]);
	GUI_set_Fgcolor(C_GUI_ORANGE);
	GUI_string_display_At(X_VMSTAR+X_VMGAP*3+75+20, Y_VMGAP*10+23,	(char *)m_unit[g_language]);
	GUI_string_display_At(X_VMSTAR+X_VMGAP*3+80+60,Y_VMGAP*10+23,	(char *)m_YGunit[g_language]);
	GUI_string_display_At(X_VMSTAR+X_VMGAP*3+80+60, Y_VMGAP*11+20,	(char *)m_WGunit[g_language]);
	GUI_string_display_At(X_VMSTAR+X_VMGAP*3+80+30, Y_VMGAP*12+20,	(char *)m_Uunit[g_language]);
	GUI_string_display_At(X_VMSTAR+X_VMGAP*4+70+30, Y_VMGAP*12+20,	(char *)m_Iunit[g_language]);

	GUI_set_Fgcolor(C_GUI_GREEN);
	GUI_string_display_At(10, Y_VMGAP*15+20, "提示:按<设置>键进行虚拟电能表相关参数设置,确定后按<存储>键保存相关参数.");
	
	GUI_window_hwin_release(m_VMwin_edit);   
    GUI_window_hwin_release(m_VMwin_pWIN);
	GUI_window_hwin_release(GET_GUIWIN);
    return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :界面销毁
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_VMmod_window_destory(void)
{
	GUI_window_destory(m_VMwin_edit);
    GUI_window_destory(m_VMwin_pWIN);
	return;
}




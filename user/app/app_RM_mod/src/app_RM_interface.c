/********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:系统设置模块界面函数集合
* 创建时间:2013-04-26
* 所属公司 :科荟
* 文件名称:app_RMmod_interface.c
* 创建人:周炳权
* 功能描述:提供界面画屏函数API
******************************************************************************************/
#include "app_RM_inside.h"

hwin_t* m_RMwin_MpWIN = PNULL;
hwin_t* m_RMwin_DLWIN = PNULL;
hwin_t* m_RMwin_pWIN = PNULL;
hwin_t* m_RMwin_edit = PNULL;
hwin_t* m_Time_edit  = PNULL;
hwin_t* m_meter_droplist_guiyue = PNULL;
hwin_t* m_meter_droplist_baud = PNULL;
hwin_t* m_meter_droplist_crc = PNULL;

const char*m_agreement_tip[]=
{
	" 国标DL/T 645-1997 规约表 ",
	" 国标DL/T 645-2007 规约表 ",
	" DLMS规约表(兰吉尔电表)   ",
};
const char*m_baud_tip[]	={" 自适应","  300","  600"," 1200"," 2400"," 4800"," 9600","19200"};
const char*m_crc_tip[]	={" 自适应"," None"," Odd "," Even"};

const char*m_RM_title[2] = {"多功能电能表校验",  ""};

const char*m_RM_option[][2] =
{
	{"1.读表校时与通讯检测",       		""},
	{"2.费率时段读取与设置",       		""},
	{"3. 需量寄存器及其他 ",       		""},
	{"4. 需量示值误差试验 ",       		""},
};

#define X_STAR 40
#define X_GAP (85+10)
#define Y_GAP 25

/********************************************************************
 * @创建人 :周炳权
 * @功能 :读表数据查询显示
 * @输入 :m_pd:当前数据指针
 * @输出 :NONE
********************************************************************/
void app_RMmod_data_all_display(RMeter_dl_info_t *m_pd,uint8_t m_flag)
{
	app_RMmod_guiyue_display(m_pd->guiyue);
	app_RMmod_baud_display(m_pd->baud);
	app_RMmod_CRC_display(m_pd->crc);
	GUI_window_hwin_req( m_RMwin_edit );
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_YELLOW);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_edit_string_display_At(m_pd->meter_Addr,EDIT_M_ADDR);
	GUI_edit_string_display_At(m_pd->meter_password,EDIT_M_PASSWORD);
	
	GUI_edit_string_display_At(m_pd->zy_power_ALL,EDIT_Z_1);
	GUI_edit_string_display_At(m_pd->zy_power_J,EDIT_J_1);
	GUI_edit_string_display_At(m_pd->zy_power_F,EDIT_F_1);
	GUI_edit_string_display_At(m_pd->zy_power_P,EDIT_P_1);
	GUI_edit_string_display_At(m_pd->zy_power_G,EDIT_G_1);
	GUI_edit_string_display_At(m_pd->zy_power_JSALL,EDIT_ALL_1);

	GUI_edit_string_display_At(m_pd->zw_power_ALL,EDIT_Z_2);
	GUI_edit_string_display_At(m_pd->zw_power_J,EDIT_J_2);
	GUI_edit_string_display_At(m_pd->zw_power_F,EDIT_F_2);
	GUI_edit_string_display_At(m_pd->zw_power_P,EDIT_P_2);
	GUI_edit_string_display_At(m_pd->zw_power_G,EDIT_G_2);
	GUI_edit_string_display_At(m_pd->zw_power_JSALL,EDIT_ALL_2);

	GUI_edit_string_display_At(m_pd->fy_power_ALL,EDIT_Z_3);
	GUI_edit_string_display_At(m_pd->fy_power_J,EDIT_J_3);
	GUI_edit_string_display_At(m_pd->fy_power_F,EDIT_F_3);
	GUI_edit_string_display_At(m_pd->fy_power_P,EDIT_P_3);
	GUI_edit_string_display_At(m_pd->fy_power_G,EDIT_G_3);
	GUI_edit_string_display_At(m_pd->fy_power_JSALL,EDIT_ALL_3);

	GUI_edit_string_display_At(m_pd->fw_power_ALL,EDIT_Z_4);
	GUI_edit_string_display_At(m_pd->fw_power_J,EDIT_J_4);
	GUI_edit_string_display_At(m_pd->fw_power_F,EDIT_F_4);
	GUI_edit_string_display_At(m_pd->fw_power_P,EDIT_P_4);
	GUI_edit_string_display_At(m_pd->fw_power_G,EDIT_G_4);
	GUI_edit_string_display_At(m_pd->fw_power_JSALL,EDIT_ALL_4);

	GUI_edit_string_display_At(m_pd->voltage_A,EDIT_A_U);//a
	GUI_edit_string_display_At(m_pd->voltage_B,EDIT_B_U);//b
	GUI_edit_string_display_At(m_pd->voltage_C,EDIT_C_U);

	GUI_edit_string_display_At(m_pd->current_A,EDIT_A_I);//a
	GUI_edit_string_display_At(m_pd->current_B,EDIT_B_I);//b
	GUI_edit_string_display_At(m_pd->current_C,EDIT_C_I);

	GUI_edit_string_display_At(m_pd->lost_ALL,EDIT_SY_ALL);//总
	GUI_edit_string_display_At(m_pd->lost_A,EDIT_SY_A);//a
	GUI_edit_string_display_At(m_pd->lost_B,EDIT_SY_B);//b
	GUI_edit_string_display_At(m_pd->lost_C,EDIT_SY_C);//c

	GUI_edit_string_display_At(m_pd->meter_time,EDIT_M_TIME);
	if ( m_flag == 0 )
	{
		GUI_set_Bgcolor(RM_BG_COLOR);
		GUI_set_Fgcolor(C_GUI_WHITE);
		GUI_string_display_At(80,m_RMwin_edit->win_high-30,"温度:");
		GUI_string_display(m_pd->m_temp);
		GUI_SetFront(&GUI_HZK16_EXTASCII);
		GUI_string_display_At(X_STAR+X_GAP*1+62,m_RMwin_edit->win_high-30+6,"℃");
		GUI_SetFront(&GUI_HZK24_EXTASCII);
		GUI_string_display_At(300,m_RMwin_edit->win_high-30,"湿度:");
		GUI_string_display(m_pd->m_hum);
		GUI_string_display("%RH");
	}

	GUI_window_hwin_release( m_RMwin_edit );

	return;
}

/********************************************************************
 * @创建人 :揭成
 * @功能 :电参数显示
 * @输入 :
 * @输出 :NONE
********************************************************************/
static void mod_FDL_view(float fdata,int id)
{
    if(fdata==0){GUI_edit_string_display_At("0.000",id);return;}
	GUI_edit_float_display_at(fdata,id,6);
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :电参数显示
 * @输入 :
 * @输出 :NONE
********************************************************************/
void app_RM_DLarg_display(measure_param_t*pDL)
{
	char temp[10]="";
	char ch[2]="";
	GUI_window_hwin_req(m_RMwin_DLWIN);
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_DLA_COLOR);
	mod_FDL_view(pDL->phase_a.f_U,EDIT_U_A);
	mod_FDL_view(pDL->phase_a.f_I,EDIT_I_A);
	mod_FDL_view(pDL->phase_a.f_J,EDIT_J_A);
	mod_FDL_view(pDL->phase_a.f_P,EDIT_P_A);
	mod_FDL_view(pDL->phase_a.f_Q,EDIT_Q_A);

	GUI_set_Fgcolor(C_GUI_DLB_COLOR);
	mod_FDL_view(pDL->phase_b.f_U,EDIT_U_B);
	mod_FDL_view(pDL->phase_b.f_I,EDIT_I_B);
	mod_FDL_view(pDL->phase_b.f_J,EDIT_J_B);
	mod_FDL_view(pDL->phase_b.f_P,EDIT_P_B);
	mod_FDL_view(pDL->phase_b.f_Q,EDIT_Q_B);
	GUI_set_Fgcolor(C_GUI_DLC_COLOR);
	mod_FDL_view(pDL->phase_c.f_U,EDIT_U_C);
	mod_FDL_view(pDL->phase_c.f_I,EDIT_I_C);
	mod_FDL_view(pDL->phase_c.f_J,EDIT_J_C);
	mod_FDL_view(pDL->phase_c.f_P,EDIT_P_C);
	mod_FDL_view(pDL->phase_c.f_Q,EDIT_Q_C);

	GUI_set_Fgcolor(C_GUI_YELLOW);
	mod_FDL_view(pDL->f_Pabc,EDIT_PABC);
	mod_FDL_view(pDL->f_Qabc,EDIT_QABC);
	mod_FDL_view(pDL->f_JUab,EDIT_JUAB);
	mod_FDL_view(pDL->f_JUbc,EDIT_JUBC);
	mod_FDL_view(pDL->f_JUac,EDIT_JUAC);
	//mod_FDL_view(pDL->f_JIab,EDIT_JIAB);
	mod_FDL_view(pDL->f_JIac,EDIT_JIAC);
	//mod_FDL_view(pDL->f_JIbc,EDIT_JIBC);
	mod_FDL_view(pDL->f_Feq, EDIT_FEQ);
	if(pDL->f_Cos==1)
	{
		GUI_edit_string_display_At("1.00",EDIT_COS);
	}
	else
	{
		global_ftoa(pDL->f_Cos,temp,2);
		ch[0] = pDL->cos_LC;
		global_strcat(temp,ch);
		GUI_edit_string_display_At(temp,EDIT_COS);
	}
	GUI_window_hwin_release(m_RMwin_DLWIN);
	return;
}

/********************************************************************
* @作者 :周炳权
* @功能 :规约显示
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void app_RMmod_guiyue_display(uint8_t m_offset)
{
	GUI_drop_list_member_assign(m_meter_droplist_guiyue,m_offset);
	return;
}
/********************************************************************
* @作者 :周炳权
* @功能 :波特率显示
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void app_RMmod_baud_display(uint8_t m_offset)
{
	GUI_drop_list_member_assign(m_meter_droplist_baud,m_offset);
	return;
}
/********************************************************************
* @作者 :周炳权
* @功能 :校验方式显示
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void app_RMmod_CRC_display(uint8_t m_offset)
{
	GUI_drop_list_member_assign(m_meter_droplist_crc,m_offset);
	return;
}

/********************************************************************
* @作者 :周炳权
* @功能 :表地址显示
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void app_RMmod_meter_addr_display(void *pdata)
{
	GUI_window_hwin_req( m_RMwin_edit );
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_YELLOW);
	GUI_edit_string_display_At(pdata,EDIT_M_ADDR);
	GUI_edit_string_display_At(m_meter_Password,EDIT_M_PASSWORD);
	GUI_window_hwin_release( m_RMwin_edit );
	return;
}
/********************************************************************
* @作者 :周炳权
* @功能 :表逻辑地址显示
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void app_RMmod_logic_addr_display( uint8_t m_logic_addr)
{
	GUI_window_hwin_req( m_RMwin_edit );
	GUI_set_Bgcolor(RM_WIN_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_GREEN);
	GUI_string_display_At(290, (Y_GAP+7)*3+8, "表逻辑地址:");
	GUI_int_display(m_logic_addr);
	GUI_window_hwin_release( m_RMwin_edit );
	return;
}

void app_RMmod_logic_addr_NOTdisplay( void )
{
	GUI_window_hwin_req( m_RMwin_edit );
	GUI_set_Bgcolor(RM_WIN_BG_COLOR);
	GUI_set_Fgcolor(RM_WIN_BG_COLOR);
	GUI_string_display_At(290, (Y_GAP+7)*3+8, "                ");
	GUI_window_hwin_release( m_RMwin_edit );
	return;
}

/********************************************************************
* @作者 :周炳权
* @功能 :使用协议方式显示
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void app_RMmod_protocol_tip_display( void )
{
	GUI_3D_window_title_mod(m_RMwin_pWIN,"初始规约为IEC1107规约,读取数据时切换为DLMS规约.");
	return;
}

void app_RMmod_protocol_tip_NOTdisplay( void )
{
	GUI_3D_window_title_mod(m_RMwin_pWIN,"抄读电能表");
	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :时间显示函数
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_RMmod_notice_window_display(void)
{
    time_string_t ctime;
	float        fdata[3];
	os_driver_ioctl(g_sht11_dev_id,0,&fdata);
	os_driver_ioctl(g_timer_dev_id,TMR_CMD_TIME_STRING_GET,&ctime);
	GUI_window_hwin_req(m_Time_edit);
	GUI_set_Bgcolor(RM_WIN_BG_COLOR);
	GUI_set_Fgcolor(RM_FG_COLOR);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_string_display_At(5,7,"日期: 20");
    GUI_string_display(ctime.year_buf);
    GUI_string_display("年");
    GUI_string_display(ctime.month_buf);
	GUI_string_display("月");
	GUI_string_display(ctime.day_buf);
	GUI_string_display("日");
	GUI_string_display_At(5,28,"时间: ");
	GUI_string_display(ctime.hour_buf);
    GUI_string_display(":");
    GUI_string_display(ctime.min_buf);
	GUI_set_Fgcolor(C_GUI_GREEN);
	GUI_string_display_At(20,49,"                   ");
	GUI_float_display_At(20,49,fdata[1],2);
	GUI_set_Fgcolor(RM_FG_COLOR);
	GUI_string_display("℃  ");
	GUI_set_Fgcolor(C_GUI_GREEN);
	GUI_float_display(fdata[0], 2);
	GUI_set_Fgcolor(RM_FG_COLOR);
	GUI_string_display("%RH");
	GUI_window_hwin_release(m_Time_edit);
	g_pic_wd = fdata[1];
	g_pic_sd = fdata[0];
	return;
}

/********************************************************************
* @作者 :周炳权
* @功能 :初始化设置显示
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void app_RMmod_set_info_display(void)
{
	app_RMmod_guiyue_display(0);
	app_RMmod_baud_display(0);
	app_RMmod_CRC_display(0);
	app_RMmod_meter_addr_display(m_meter_addr);
	return;
}
/********************************************************************
* @作者 :周炳权
* @功能 :数据转换成字符兵显示
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void app_RMmod_rev_data_ftoa_and_display(float fdata,char *pdata,Bsize_t id)
{
	GUI_window_hwin_req( m_RMwin_edit );
	global_ftoa(fdata,pdata, 3);
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_YELLOW);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_edit_string_display_At(pdata,id);
	GUI_window_hwin_release( m_RMwin_edit );
	return;
}
/********************************************************************
* @作者 :范文斌
* @功能 :数据显示
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void app_RMmod_rev_data_display(char *pdata,Bsize_t id)
{
	GUI_window_hwin_req( m_RMwin_edit );
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_YELLOW);
	GUI_edit_string_display_At(pdata,id);
	GUI_window_hwin_release( m_RMwin_edit );
	return;
}
/********************************************************************
* @作者 :周炳权
* @功能 :显示通讯提示
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void app_RMmod_rev_tip_display(void *pdata)
{
	GUI_window_hwin_req( m_RMwin_edit );
	GUI_set_Bgcolor(RM_WIN_BG_COLOR);	
	GUI_set_Fgcolor(C_GUI_YELLOW);
	if ( GUI_edit_select( EDIT_TIP ) )
	{
		GUI_edit_clean();
		GUI_edit_string_display( pdata );
	}
	GUI_window_hwin_release( m_RMwin_edit );
	return;
}

void app_RMmod_rev_tip_clean(void)
{
	GUI_window_hwin_req( m_RMwin_edit );
	GUI_set_Bgcolor(RM_WIN_BG_COLOR);	
	GUI_set_Fgcolor(C_GUI_YELLOW);
	GUI_edit_select(EDIT_TIP);
	GUI_edit_clean();
	GUI_window_hwin_release( m_RMwin_edit );
	return;
}

/********************************************************************
* @作者 :范文斌
* @功能 :显示通讯提示
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void app_RMmod_rev_tip2_display(uint8_t offset)
{
	GUI_window_hwin_req( m_RMwin_edit );
	GUI_set_Bgcolor( RM_WIN_BG_COLOR );
	GUI_set_Fgcolor(C_GUI_GREEN);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	if ( GUI_edit_select( EDIT_TIP ) )
	{
		GUI_edit_clean();
		if(offset==0)
			GUI_edit_string_display("正在读表...");
		else if(offset==1)
			GUI_edit_string_display("读表完成.");
		else if(offset==2)
		{
			GUI_set_Fgcolor(C_GUI_RED);
			GUI_edit_string_display("读表失败.");
		}
		else if(offset==3)
			GUI_edit_string_display("命令校时完成.");
		else if(offset==4)
			GUI_edit_string_display("广播校时完成.");
		else if(offset==5)
		{
			GUI_set_Fgcolor(C_GUI_RED);
			GUI_edit_string_display("未能通讯上.");
		}
	}
	GUI_window_hwin_release( m_RMwin_edit );
	return;
}
/********************************************************************
* @作者 :范文斌
* @功能 :清除模块窗口数据
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void app_RMmod_clean_display(void)
{
	Bsize_t m_offset;
	GUI_window_hwin_req( m_RMwin_edit );
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_YELLOW);
	for(m_offset=5;m_offset<=35;m_offset++)
		GUI_edit_string_display_At("",m_offset);
	GUI_window_hwin_release( m_RMwin_edit );
	app_RMmod_logic_addr_NOTdisplay();
	app_RMmod_protocol_tip_NOTdisplay();
	app_RMmod_rev_tip_clean();
	return;
}
/********************************************************************
 * @作者 :周炳权
 * @功能 :创建窗口
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_RMmod_window_create(int flag) 
{ 
#define bn_wide 80
#define bn_higt 40
	uint8_t m_offset;
	hwin_t *hp = PNULL;
	GUI_window_hwin_req(GET_GUIWIN);
  	GUI_set_Bgcolor(RM_BG_COLOR);
	GUI_clean_screen();
	GUI_set_Bgcolor(RM_WIN_BG_COLOR);    
	GUI_set_Fgcolor(RM_FG_COLOR); 
	if ( flag == 0 )
    	m_RMwin_pWIN=Gui_3D_window_creat(0,2,636,476,"读表校时与通讯检测",WIN_3D | WIN_FILL,GET_GUIWIN);
	else 
		m_RMwin_pWIN=Gui_3D_window_creat(0,2,636,476,"读表数据查询详情",WIN_3D | WIN_FILL,GET_GUIWIN);
    GUI_window_hwin_req(m_RMwin_pWIN);
	m_RMwin_edit
		= GUI_edit_creat(5,10,600,420,WIN_TOUCH_EN,m_RMwin_pWIN);
	GUI_window_hwin_req( m_RMwin_edit );
	GUI_set_Bgcolor(RM_WIN_BG_COLOR);    
	GUI_set_Fgcolor(RM_FG_COLOR);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_string_display_At(45,Y_GAP*0+3, "规  约:");
	GUI_string_display_At(45,(Y_GAP+7)*1+4, "波特率:");
	GUI_string_display_At(260,(Y_GAP+7)*1+4,"校验方式:");
	m_meter_droplist_guiyue =
		Gui_drop_list_creat(130,Y_GAP*0,m_agreement_tip,SIZE_OF_ARRAY(m_agreement_tip),GUI_KEY_GUIYUE_SELECT,m_RMwin_edit);
	m_meter_droplist_baud =
		Gui_drop_list_creat(130,(Y_GAP+7)*1+4,m_baud_tip,SIZE_OF_ARRAY(m_baud_tip),GUI_KEY_BAUD_SELECT,m_RMwin_edit);
	m_meter_droplist_crc =
		Gui_drop_list_creat(370,(Y_GAP+7)*1+4,m_crc_tip,SIZE_OF_ARRAY(m_crc_tip),GUI_KEY_CRC_SELECT,m_RMwin_edit);	

	GUI_edit_memeber_add(40, 	(Y_GAP+7)*2+8, 12,"表地址:", MEMBER_3D|EDIT_M_ADDR);
	GUI_edit_memeber_add(285, 	(Y_GAP+7)*2+8, 8,"表权限密码:", MEMBER_3D|EDIT_M_PASSWORD);
	
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_edit_memeber_add(40, 	Y_GAP*4+10, 40,"", 0|EDIT_TIP);
	
	GUI_string_display_At(X_STAR+X_GAP*0+102, Y_GAP*5+15,"总");
	GUI_string_display_At(X_STAR+X_GAP*1+102, Y_GAP*5+15,"尖");
	GUI_string_display_At(X_STAR+X_GAP*2+102, Y_GAP*5+15,"峰");
	GUI_string_display_At(X_STAR+X_GAP*3+102, Y_GAP*5+15,"平");
	GUI_string_display_At(X_STAR+X_GAP*4+102, Y_GAP*5+15,"谷");	
	
	GUI_string_display_At(40, Y_GAP*6+10,"正向有功:");
	GUI_string_display_At(40, Y_GAP*7+10,"正向无功:");
	GUI_string_display_At(40, Y_GAP*8+10,"反向有功:");
	GUI_string_display_At(40, Y_GAP*9+10,"反向无功:");
	for(m_offset=0;m_offset<5;m_offset++)
	{
		GUI_edit_memeber_add(X_STAR+X_GAP*m_offset+70,Y_GAP*6+10,8,"", MEMBER_3D|(5+4*m_offset));
		GUI_edit_memeber_add(X_STAR+X_GAP*m_offset+70,Y_GAP*7+10,8,"", MEMBER_3D|(6+4*m_offset));
		GUI_edit_memeber_add(X_STAR+X_GAP*m_offset+70,Y_GAP*8+10,8,"", MEMBER_3D|(7+4*m_offset));
		GUI_edit_memeber_add(X_STAR+X_GAP*m_offset+70,Y_GAP*9+10,8,"", MEMBER_3D|(8+4*m_offset));
	}
	GUI_string_display_At(40, Y_GAP*11+10, "电  压:");
	GUI_string_display_At(40, Y_GAP*12+10,"电  流:");
	GUI_string_display_At(40, Y_GAP*13+10,"失  压:");
	GUI_set_Fgcolor(C_GUI_YELLOW);
	GUI_string_display_At(X_STAR+X_GAP*0+94, Y_GAP*10+13,"A相");
	GUI_set_Fgcolor(C_GUI_RED);
	GUI_string_display_At(X_STAR+X_GAP*1+94, Y_GAP*10+13,"B相");
	GUI_set_Fgcolor(C_GUI_GREEN);
	GUI_string_display_At(X_STAR+X_GAP*2+94, Y_GAP*10+13,"C相");
	GUI_set_Fgcolor(RM_FG_COLOR);
	for(m_offset=0;m_offset<3;m_offset++)
	{
		GUI_edit_memeber_add(X_STAR+X_GAP*m_offset+70,Y_GAP*11+10, 8, "", MEMBER_3D|25+3*m_offset);
		GUI_edit_memeber_add(X_STAR+X_GAP*m_offset+70,Y_GAP*12+10, 8, "", MEMBER_3D|26+3*m_offset);
		GUI_edit_memeber_add(X_STAR+X_GAP*m_offset+70,Y_GAP*13+10, 8, "", MEMBER_3D|27+3*m_offset);
	}
	GUI_string_display_At(40, Y_GAP*14+10,"总 失 压:");
	GUI_edit_memeber_add(X_STAR+X_GAP*0+70,Y_GAP*14+10,8,"", MEMBER_3D|EDIT_SY_ALL);
	GUI_edit_memeber_add(X_STAR+X_GAP*1+70,Y_GAP*14+10,19,"表时间:", MEMBER_3D|EDIT_M_TIME);
	GUI_set_Fgcolor(C_GUI_ORANGE);
	GUI_string_display_At(X_STAR+X_GAP*3+75, 	Y_GAP*10+13,	"单位:");
	GUI_string_display_At(X_STAR+X_GAP*3+80, 	Y_GAP*11+10,	"无功(kvarh).");
	GUI_string_display_At(X_STAR+X_GAP*3+80, 	Y_GAP*12+10,	"有功(kWh)");
	GUI_string_display_At(X_STAR+X_GAP*4+80+8, 	Y_GAP*12+10,	"失压(Min).");	
	GUI_string_display_At(X_STAR+X_GAP*3+80, 	Y_GAP*13+10,	"电 压(V).");
	GUI_string_display_At(X_STAR+X_GAP*4+70+8, 	Y_GAP*13+10,	"电 流(A) .");

	if ( flag == 0 )
	{
		hp = Gui_button_creat( 50,m_RMwin_pWIN->win_high-bn_higt,bn_wide+10,bn_higt-5,BUTTON_PAINT|GUI_KEY_READ_M|KEY_F1, m_RMwin_pWIN );
		GUI_button_title_display( hp, "读  表" );
		hp = Gui_button_creat( 50+110,m_RMwin_pWIN->win_high-bn_higt,bn_wide+10,bn_higt-5,BUTTON_PAINT|GUI_KEY_METER_BROADCAST_JIAOSHI|KEY_F2, m_RMwin_pWIN );
		GUI_button_title_display( hp, "广播校时" );
		hp = Gui_button_creat( 50+220,m_RMwin_pWIN->win_high-bn_higt,bn_wide+10,bn_higt-5,BUTTON_PAINT|GUI_KEY_METER_MOON_JIAOSHI|KEY_F3, m_RMwin_pWIN );
		GUI_button_title_display( hp, "卫星校时" );
		
		m_Time_edit = GUI_edit_creat(m_RMwin_pWIN->win_wide-190,m_RMwin_pWIN->win_high-76,190,75,WIN_3D,m_RMwin_pWIN);
	}
	GUI_window_hwin_release(m_RMwin_edit);
    GUI_window_hwin_release(m_RMwin_pWIN);
	GUI_window_hwin_release(GET_GUIWIN);
    return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :界面销毁
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_RMmod_window_destory(void)
{
	GUI_window_destory(m_RMwin_edit);
    GUI_window_destory(m_RMwin_pWIN);
	return;
}

/********************************************************************
 * @创建人 :揭成
 * @功能 :电参数显示窗口初始化
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_RM_DLmember_window_init(hwin_t*hp)
{
#define   DL_XLAB 15
#define   DL_GAP  90
#define   DL_VGAP 32
	
	GUI_window_hwin_req(hp);
	
	GUI_set_Bgcolor(m_GUI_3D_window_Bg_cor);
    GUI_fill_window_area(4,2,hp->win_wide,DL_VGAP);
    GUI_set_Fgcolor(RM_FG_COLOR);
	
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_string_display_At(24+DL_XLAB+DL_GAP*0, 2, "U(V)" );
	GUI_string_display_At(24+DL_XLAB+DL_GAP*1, 2, "I(A)" );
	GUI_string_display_At(12+DL_XLAB+DL_GAP*2, 2, "Φ(°)" );
	GUI_string_display_At(24+DL_XLAB+DL_GAP*3, 2, "P(W)" );
	GUI_string_display_At(12+DL_XLAB+DL_GAP*4, 2, "Q(var)" );
	GUI_string_display_At(30+DL_XLAB+DL_GAP*5, 2, "ΦABC(°)" );
	
	GUI_set_Bgcolor(RM_WIN_BG_COLOR);
    GUI_set_Fgcolor(RM_FG_COLOR);
	GUI_set_Fgcolor( C_GUI_YELLOW );
	GUI_string_display_At(2,DL_VGAP*1+2, "A" );
	GUI_set_Fgcolor( C_GUI_GREEN );
	GUI_string_display_At(2,DL_VGAP*2+2, "B" );
	GUI_set_Fgcolor( C_GUI_RED );
	GUI_string_display_At(2,DL_VGAP*3+2, "C" );

	GUI_edit_memeber_add(DL_XLAB+DL_GAP*0,DL_VGAP*1, 7, "", MEMBER_3D | EDIT_U_A |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*0,DL_VGAP*2, 7, "", MEMBER_3D | EDIT_U_B |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*0,DL_VGAP*3, 7, "", MEMBER_3D | EDIT_U_C |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*1,DL_VGAP*1, 7, "", MEMBER_3D | EDIT_I_A |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*1,DL_VGAP*2, 7, "", MEMBER_3D | EDIT_I_B |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*1,DL_VGAP*3, 7, "", MEMBER_3D | EDIT_I_C |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*2,DL_VGAP*1, 7, "", MEMBER_3D | EDIT_J_A |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*2,DL_VGAP*2, 7, "", MEMBER_3D | EDIT_J_B |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*2,DL_VGAP*3, 7, "", MEMBER_3D | EDIT_J_C |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*3,DL_VGAP*1, 7, "", MEMBER_3D | EDIT_P_A |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*3,DL_VGAP*2, 7, "", MEMBER_3D | EDIT_P_B |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*3,DL_VGAP*3, 7, "", MEMBER_3D | EDIT_P_C |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*4,DL_VGAP*1, 7, "", MEMBER_3D | EDIT_Q_A |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*4,DL_VGAP*2, 7, "", MEMBER_3D | EDIT_Q_B |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*4,DL_VGAP*3, 7, "", MEMBER_3D | EDIT_Q_C |EDIT_DIS);
	GUI_set_Fgcolor(RM_FG_COLOR);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*3,DL_VGAP*4,7, "",  MEMBER_3D | EDIT_PABC |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*4,DL_VGAP*4,7, "",  MEMBER_3D | EDIT_QABC |EDIT_DIS);
	GUI_edit_memeber_add(2,DL_VGAP*4,7,   "cosΦ", MEMBER_3D | EDIT_COS |EDIT_DIS);
	GUI_edit_memeber_add(159,DL_VGAP*4,6, "F", MEMBER_3D | EDIT_FEQ |EDIT_DIS);

	GUI_edit_memeber_add(5+DL_XLAB+DL_GAP*5,DL_VGAP*1,7, "ΦUab", MEMBER_3D | EDIT_JUAB );
	GUI_edit_memeber_add(5+DL_XLAB+DL_GAP*5,DL_VGAP*2,7, "ΦUac", MEMBER_3D | EDIT_JUAC);
	GUI_edit_memeber_add(5+DL_XLAB+DL_GAP*5,DL_VGAP*3,7, "ΦUbc", MEMBER_3D | EDIT_JUBC);
	GUI_edit_memeber_add(5+DL_XLAB+DL_GAP*5,DL_VGAP*4,7, "ΦIac", MEMBER_3D | EDIT_JIAC);
	
	GUI_SetFront(&GUI_HZK16_EXTASCII);
    GUI_string_display_At(DL_XLAB+DL_GAP*3-16,DL_VGAP*4+4, "∑");

	GUI_window_hwin_release(hp);
    return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :创建窗口
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_RMmod_main_window_create(void) 
{ 
	hwin_t *hp = PNULL;
	GUI_window_hwin_req(GET_GUIWIN);
  	GUI_set_Bgcolor(RM_WIN_BG_COLOR);
	GUI_set_Fgcolor(RM_FG_COLOR);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
    GUI_clean_screen();
	m_RMwin_MpWIN=Gui_3D_window_creat(2,2,636,475,( char* )m_RM_title[g_language],WIN_3D | WIN_FILL,GET_GUIWIN);
	m_RMwin_DLWIN = GUI_edit_creat(2,265,630,170,WIN_3D,m_RMwin_MpWIN );
    app_RM_DLmember_window_init(m_RMwin_DLWIN);
	/*m_RMwin_edit = GUI_edit_creat(2,0,630,228,WIN_3D,m_RMwin_MpWIN );
	GUI_window_hwin_req(m_RMwin_edit);
	GUI_set_Bgcolor(RM_WIN_BG_COLOR);
	GUI_set_Fgcolor(RM_FG_COLOR);
	GUI_SetFront(&GUI_HZK24_EXTASCII);		
	GUI_edit_memeber_add(90,10+35*0,21,"走字起始时间:", MEMBER_3D | 10);
	GUI_edit_memeber_add(90,10+35*1,21,"走字结束时间:", MEMBER_3D | 11);
	GUI_edit_memeber_add(90,10+35*2,6, "        倍率:", MEMBER_3D | 12);
	GUI_edit_memeber_add(90,10+35*3,8, "二次走字电能:", MEMBER_3D | 13);
	GUI_edit_memeber_add(90,10+35*4,8, "一次走字电能:", MEMBER_3D | 14);
	GUI_edit_memeber_add(90,10+35*5,8, "测量运行时间:", MEMBER_3D | 15);
	GUI_string_display_At(353,25+32*5, "秒" );
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_string_display_At(353,30+32*3, "kW·h" );
	GUI_string_display_At(353,30+32*4, "kW·h" );
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	hp = Gui_button_creat( 50,m_RMwin_MpWIN->win_high-35,80+10,35-5,BUTTON_PAINT|KEY_F1, m_RMwin_MpWIN );
	GUI_button_title_display( hp, "开始" );
	hp = Gui_button_creat( 50+110,m_RMwin_MpWIN->win_high-35,80+10,35-5,BUTTON_PAINT|KEY_F2, m_RMwin_MpWIN );
	GUI_button_title_display( hp, "结束" );
	hp = Gui_button_creat( 50+220,m_RMwin_MpWIN->win_high-35,80+10,35-5,BUTTON_PAINT|KEY_F3, m_RMwin_MpWIN );
	GUI_button_title_display( hp, "清零" );
	GUI_window_hwin_release(m_RMwin_edit);
	*/
	GUI_window_hwin_req(m_RMwin_MpWIN);
	GUI_set_Bgcolor(RM_WIN_BG_COLOR);
	GUI_set_Fgcolor(RM_FG_COLOR);
	GUI_SetFront(&GUI_HZK24_EXTASCII);	
	hp = Gui_button_creat(185,20+0*60,270,35,app_global_Ntokey(0+1) | BUTTON_PAINT,m_RMwin_MpWIN);
	GUI_button_title_display(hp,(char*)m_RM_option[0][g_language] );
	hp = Gui_button_creat(185,20+1*60,270,35,app_global_Ntokey(1+1) | BUTTON_PAINT,m_RMwin_MpWIN);
	GUI_button_title_display(hp,(char*)m_RM_option[1][g_language] );
	hp = Gui_button_creat(185,20+2*60,270,35,app_global_Ntokey(2+1) | BUTTON_PAINT,m_RMwin_MpWIN);
	GUI_button_title_display(hp,(char*)m_RM_option[2][g_language] );
	hp = Gui_button_creat(185,20+3*60,270,35,app_global_Ntokey(3+1) | BUTTON_PAINT,m_RMwin_MpWIN);
	GUI_button_title_display(hp,(char*)m_RM_option[3][g_language] );
	GUI_window_hwin_release(m_RMwin_MpWIN);
	GUI_window_hwin_release(GET_GUIWIN);
	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :界面销毁
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_RMmod_main_window_destory(void)
{
	GUI_window_destory(m_RMwin_DLWIN);
	GUI_window_destory(m_RMwin_MpWIN);
	return;
}


void app_TEST_init(void)
{
	GUI_window_hwin_req(m_RMwin_edit);
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_DLA_COLOR);
	GUI_edit_string_display_At("2013-11-19 10:20:30",10);
	GUI_edit_string_display_At("2013-11-19 11:10:30",11);
	GUI_edit_string_display_At("10",12);
	GUI_edit_string_display_At("1.5",13);
	GUI_edit_string_display_At("15",14);
	GUI_set_Fgcolor(C_GUI_GREEN);
	GUI_edit_string_display_At("3000",15);
	GUI_window_hwin_release(m_RMwin_edit);
	GUI_window_hwin_req(m_RMwin_DLWIN);
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_DLA_COLOR);
	mod_FDL_view(220.01,EDIT_U_A);
	mod_FDL_view(1.999,EDIT_I_A);
	mod_FDL_view(0.01,EDIT_J_A);
	mod_FDL_view(440,EDIT_P_A);
	mod_FDL_view(0.09,EDIT_Q_A);

	GUI_set_Fgcolor(C_GUI_DLB_COLOR);
	mod_FDL_view(220.01,EDIT_U_B);
	mod_FDL_view(1.999,EDIT_I_B);
	mod_FDL_view(0.01,EDIT_J_B);
	mod_FDL_view(440,EDIT_P_B);
	mod_FDL_view(0.09,EDIT_Q_B);
	GUI_set_Fgcolor(C_GUI_DLC_COLOR);
	mod_FDL_view(220.01,EDIT_U_C);
	mod_FDL_view(1.999,EDIT_I_C);
	mod_FDL_view(0.01,EDIT_J_C);
	mod_FDL_view(440,EDIT_P_C);
	mod_FDL_view(0.09,EDIT_Q_C);
	GUI_set_Fgcolor(C_GUI_YELLOW);
	mod_FDL_view(1320,EDIT_PABC);
	mod_FDL_view(0.27,EDIT_QABC);
	mod_FDL_view(120,EDIT_JUAB);
	mod_FDL_view(120,EDIT_JUBC);
	mod_FDL_view(-120,EDIT_JUAC);
	mod_FDL_view(-120,EDIT_JIAC);
	mod_FDL_view(50, EDIT_FEQ);
	GUI_edit_string_display_At("1.00",EDIT_COS);

	GUI_window_hwin_release(m_RMwin_DLWIN);
	return;
}


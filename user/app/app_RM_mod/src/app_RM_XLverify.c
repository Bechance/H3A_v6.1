/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:����Уʱ����
* ����ʱ��:2013-09-25
* ������˾ :����
* �ļ�����:app_RM_XLverify.c
* ���� :�ܱ�Ȩ
* ��������:ʵ�ֶ��������Ĵ����Ķ�ȡ�����ú���������
******************************************************************************************/
#include	"app_RM_inside.h"

hwin_t* m_RMwin_XLWIN  = PNULL;
hwin_t* m_RMwin_XLedit = PNULL;
hwin_t* m_XL_edit_Wmeter = PNULL;
hwin_t* m_XL_edit_Rmeter = PNULL;
hwin_t* m_XL_drop_way	 = PNULL;

static uint8_t  _XLtask_id = INVALID_PRO;
static uint8_t  _XLtask_recid = INVALID_PRO;

/*1.�������״̬��1;2.��̴���;3.���һ�α��ʱ��(07:��1�α�̼�¼);
4.��ع���ʱ��;5.�����������;6.����ʱ��;7.(07):ʱ�ӵ�ص�ѹ;8.(07):ÿ�µ�1������;
9.(07):ÿ�µ�2������;10.(07):ÿ�µ�3������;
*/
const uint8_t m_XLcmd_97_DI1[] = {0xc0,0xb2,0xb2,0xb2,0xc1,0xc1,};
const uint8_t m_XLcmd_97_DI0[] = {0x20,0x12,0x10,0x14,0x11,0x12,};

const uint8_t m_XLcmd_07_DI3[] = {0x04,0x03,0x03,0x02,0x04,0x04,0x02,0x04,0x04,0x04,};
const uint8_t m_XLcmd_07_DI2[] = {0x00,0x30,0x30,0x80,0x00,0x00,0x80,0x00,0x00,0x00,};
const uint8_t m_XLcmd_07_DI1[] = {0x05,0x00,0x00,0x00,0x01,0x01,0x00,0x0b,0x0b,0x0b,};
const uint8_t m_XLcmd_07_DI0[] = {0x01,0x00,0x01,0x0a,0x03,0x04,0x08,0x01,0x02,0x03,};

const char *m_XL_calc_way[]={"      ","����ʽ","����ʽ"};
/*д��ʱ��ʾ�û��򿪱�̿���(���б�̿���),�������*/

uint8_t g_XL_rec_start_flag = 0;	//��ʼ�������ݱ�־

XL_other_read_t XL_other_read_buf = { 0, };
XL_account_t    XL_account_buf    = { 0, };
XL_reg_t		XL_reg_read_buf   = { 0, };
XL_reg_t		XL_reg_write_buf  = { 0, };
uint8_t 		g_XL_calc_way	  = 0;
uint8_t			g_xl_MRstate_word1	  	= 0;//�������״̬��1	
uint8_t 		g_xl_read_or_write	  	= 0;//���Ͷ���д�������־,0:��;1:д;
uint8_t 		g_xl_send_read_offset 	= 0;//���Ͷ�ȡ�������ƫ��
uint8_t 		g_Xl_send_write_offset 	= 0;//����д�������ƫ��
uint8_t 		g_xl_send_read_flag  	= 0;//���Ͷ��������־,1:�����״̬�������ռ�����;
											//2:�������Ĵ���;
uint8_t 		g_xl_send_write_flag  	= 0;//����д�������־,1:д������;2:д�����Ĵ���;
uint8_t			g_xl_read_error_flag 	= 0;//��������־

#define  XL_Y_OFFSET	30
/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :�������Ĵ�����ʼ������
 * @���� :NONE
 * @��� :NONE
********************************************************************/
static void read_XL_window( hwin_t *m_hp )
{
	hwin_t *hp = PNULL;
	GUI_window_hwin_req(m_hp);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_set_Bgcolor(RM_BG_COLOR);
    GUI_set_Fgcolor(RM_FG_COLOR);
	GUI_WINDIS_HLINE(0,317,35);
	GUI_string_display_At(50, XL_Y_OFFSET*0+5, "��������Ĵ�����Ϣ");
	GUI_string_display_At(20, XL_Y_OFFSET*1+10, "�������������ʽ:");
	GUI_string_display_At(20, XL_Y_OFFSET*2+14, "�����������:");
	GUI_string_display_At(20, XL_Y_OFFSET*3+15, "�� �� ʱ �� :");
	GUI_string_display_At(220, XL_Y_OFFSET*2+14, "��");
	GUI_string_display_At(220, XL_Y_OFFSET*3+15, "��");
	hp = Gui_button_creat( 100,m_hp->win_high-40,110,35,BUTTON_PAINT|GUI_KEY_XL_READ|KEY_F4,m_hp );
	GUI_button_title_display( hp, "F4_����" );
	GUI_window_hwin_release(m_hp);

	return;
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :д�����Ĵ�����ʼ������
 * @���� :NONE
 * @��� :NONE
********************************************************************/
static void write_XL_window( hwin_t *m_hp )
{
	hwin_t *hp = PNULL;
	GUI_window_hwin_req(m_hp);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_set_Bgcolor(RM_BG_COLOR);
    GUI_set_Fgcolor(RM_FG_COLOR);
	GUI_WINDIS_HLINE(0,317,35);
	GUI_string_display_At(50, XL_Y_OFFSET*0+5, "��������Ĵ�������");
	GUI_WINDIS_HLINE(0,317,XL_Y_OFFSET*2+10);
	GUI_set_Fgcolor(C_GUI_GREEN);
	GUI_string_display_At(10, XL_Y_OFFSET*1+10,  "��������Ҫ����Ȩ������.");
	GUI_set_Fgcolor(RM_FG_COLOR);
	//GUI_string_display_At(30, XL_Y_OFFSET*1+15,  "�������������ʽ:");
	//m_XL_drop_way 
		//= Gui_drop_list_creat(240,XL_Y_OFFSET*1+15,m_XL_calc_way,SIZE_OF_ARRAY(m_XL_calc_way),GUI_KEY_XL_WAY,m_hp);
	GUI_edit_memeber_add(20, XL_Y_OFFSET*2+15, 2,"�����������:", MEMBER_3D|EDIT_XL_PERIOD);
	GUI_edit_memeber_add(20, XL_Y_OFFSET*3+15, 2,"�� �� ʱ �� :", MEMBER_3D|EDIT_XL_HC_TIME);
	GUI_string_display_At(213, XL_Y_OFFSET*2+15, "��");
	GUI_string_display_At(213, XL_Y_OFFSET*3+15, "��");
	hp = Gui_button_creat( 180,m_hp->win_high-40,110,33,BUTTON_PAINT|GUI_KEY_XL_WRITE_SET|KEY_F3,m_hp );
	GUI_button_title_display( hp, "F3_����" );
	hp = Gui_button_creat( 30,m_hp->win_high-40,110,35,BUTTON_PAINT|GUI_KEY_XL_WRITE,m_hp );
	GUI_button_title_display( hp, "F2_д��" );
	GUI_window_hwin_release(m_hp);

	return;
}	

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :��������
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_RMmod_XL_window_create(void) 
{ 
	hwin_t *hp = PNULL;
	GUI_window_hwin_req(GET_GUIWIN);
  	GUI_set_Bgcolor(RM_BG_COLOR);
	GUI_set_Fgcolor(RM_FG_COLOR);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
    GUI_clean_screen();
	m_RMwin_XLWIN=Gui_3D_window_creat(2,2,636,475,"�����Ĵ����������Ĵ������",WIN_3D | WIN_FILL,GET_GUIWIN);
    GUI_window_hwin_req(m_RMwin_XLWIN);
	m_RMwin_XLedit
		= GUI_edit_creat(2,2,635,450,WIN_TOUCH_EN,m_RMwin_XLWIN);
	GUI_window_hwin_req( m_RMwin_XLedit );
	GUI_set_Bgcolor(RM_BG_COLOR);
	GUI_set_Fgcolor(RM_FG_COLOR);
	GUI_SetFront(&GUI_HZK24_EXTASCII);

	GUI_edit_memeber_add(255, XL_Y_OFFSET*0+10, 8,"�ڲ���ع���ʱ��:", MEMBER_3D|EDIT_XL_BAT_TIME|EDIT_DIS);
	GUI_string_display_At(570, XL_Y_OFFSET*0+13, "��");
	GUI_edit_memeber_add(5, XL_Y_OFFSET*0+10, 6,"�ڲ����״̬:", MEMBER_3D|EDIT_XL_BAT_STATE|EDIT_DIS);
	GUI_edit_memeber_add(255, XL_Y_OFFSET*1+20, 6, "ʱ�ӵ�ص�ѹ:", MEMBER_3D|EDIT_XL_BAT_VOL|EDIT_DIS);
	GUI_string_display_At(500, XL_Y_OFFSET*1+20, "V (07��Լ)");
	GUI_edit_memeber_add(5, XL_Y_OFFSET*1+20, 9,"��̴���: ", MEMBER_3D|EDIT_XL_PNUM|EDIT_DIS);
	GUI_edit_memeber_add(5, XL_Y_OFFSET*2+30, 20,"��һ�α��ʱ��:", MEMBER_3D|EDIT_XL_PTIME|EDIT_DIS);
	GUI_string_display_At(5, XL_Y_OFFSET*4+10, "ÿ�½�����(DL/T645-2007��Լ):");
	GUI_edit_memeber_add(355, XL_Y_OFFSET*4+10, 8,"д��Ȩ������:", MEMBER_3D|EDIT_XL_PASSWORD);
	GUI_edit_memeber_add(10, XL_Y_OFFSET*5+15, 2,"��1: ", MEMBER_3D|EDIT_XL_ACCOUNT_DAY1);
	GUI_edit_memeber_add(100, XL_Y_OFFSET*5+15, 2,"�� ", MEMBER_3D|EDIT_XL_ACCOUNT_HOUR1);
	GUI_string_display_At(172, XL_Y_OFFSET*5+15, "ʱ;");
	GUI_edit_memeber_add(212, XL_Y_OFFSET*5+15, 2,"��2: ", MEMBER_3D|EDIT_XL_ACCOUNT_DAY2);
	GUI_edit_memeber_add(212+90, XL_Y_OFFSET*5+15, 2,"�� ", MEMBER_3D|EDIT_XL_ACCOUNT_HOUR2);
	GUI_string_display_At(212+90+72, XL_Y_OFFSET*5+15, "ʱ;");
	GUI_edit_memeber_add(212+90+72+37, XL_Y_OFFSET*5+15, 2,"��3: ", MEMBER_3D|EDIT_XL_ACCOUNT_DAY3);
	GUI_edit_memeber_add(212+93*2+72+35, XL_Y_OFFSET*5+15, 2,"�� ", MEMBER_3D|EDIT_XL_ACCOUNT_HOUR3);
	GUI_string_display_At(212+93*2+72*2+37, XL_Y_OFFSET*5+15, "ʱ;");

	hp = Gui_button_creat( 80,XL_Y_OFFSET*6+25,180,35,BUTTON_PAINT|GUI_KEY_XL_ABP_READ|KEY_F1,m_RMwin_XLWIN );
	GUI_button_title_display( hp, "F1_������Ϣ" );
	//hp = Gui_button_creat( 325,XL_Y_OFFSET*6+40,145,35,BUTTON_PAINT|GUI_KEY_XL_ACC_SET|KEY_SETTING,m_RMwin_XLWIN );
	//GUI_button_title_display( hp, " �� �� " );
	hp = Gui_button_creat( 350,XL_Y_OFFSET*6+25,180,35,BUTTON_PAINT|GUI_KEY_XL_ACC_WRITE|KEY_F5,m_RMwin_XLWIN );
	GUI_button_title_display( hp, "F5_д�������" );
	
	m_XL_edit_Wmeter = GUI_edit_creat(0,255,317,m_RMwin_XLWIN->win_high-255,WIN_3D|WIN_TOUCH_EN,m_RMwin_XLWIN);
	write_XL_window(m_XL_edit_Wmeter);
	m_XL_edit_Rmeter = GUI_edit_creat(317,255,317,m_RMwin_XLWIN->win_high-255,WIN_3D|WIN_TOUCH_EN,m_RMwin_XLWIN);
	read_XL_window(m_XL_edit_Rmeter);

	GUI_window_hwin_release(m_RMwin_XLedit);	
	GUI_window_hwin_release(m_RMwin_XLWIN);		
	GUI_window_hwin_release(GET_GUIWIN);
	return;
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :��������
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_RMmod_XL_window_destory(void)
{
	GUI_window_destory(m_RMwin_XLedit);
    GUI_window_destory(m_RMwin_XLWIN);
	return;
}

/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :���ַ��㴦��
* @���� :NONE
* @��� :NONE
********************************************************************/
static void data_mend_zero_handle( uint8_t *m_buf )
{
	if ( m_buf[0] == 0 && m_buf[1] == 0 )
	{
		m_buf[0] = '0';
		m_buf[1] = '0';
		m_buf[2] = '\0';
	}
	else if ( m_buf[0] == 0 )
	{
		m_buf[0] = '0';
		m_buf[2] = '\0';
	}
	else if ( m_buf[1] == 0 )
	{
		m_buf[1] = m_buf[0];
		m_buf[0] = '0';
		m_buf[2] = '\0';
	}
	return;
}

/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :���롢���״̬�������պͱ�������ʾ
* @���� :NONE
* @��� :NONE
********************************************************************/
static void XL_ABP_display( void )
{
	GUI_window_hwin_req( m_RMwin_XLedit );
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_YELLOW);
	GUI_edit_string_display_At(m_meter_Password,EDIT_XL_PASSWORD);
	if ( XL_other_read_buf.m_battery_sate == 1 )
		 GUI_edit_string_display_At("����",EDIT_XL_BAT_STATE);
	else if ( XL_other_read_buf.m_battery_sate == 2 )
		 GUI_edit_string_display_At("Ƿѹ",EDIT_XL_BAT_STATE);
	GUI_edit_string_display_At(	(char *)XL_other_read_buf.m_battery_RunT,EDIT_XL_BAT_TIME);
	GUI_edit_float_display_at(XL_other_read_buf.m_battery_V, EDIT_XL_BAT_VOL, 5);
	GUI_edit_string_display_At(	(char *)XL_other_read_buf.m_P_num,EDIT_XL_PNUM);
	GUI_edit_string_display_At(	(char *)XL_other_read_buf.m_P_time,EDIT_XL_PTIME);
	GUI_edit_string_display_At(	(char *)XL_account_buf.m_At1_day,EDIT_XL_ACCOUNT_DAY1);
	GUI_edit_string_display_At(	(char *)XL_account_buf.m_At1_hour,EDIT_XL_ACCOUNT_HOUR1);
	GUI_edit_string_display_At(	(char *)XL_account_buf.m_At2_day,EDIT_XL_ACCOUNT_DAY2);
	GUI_edit_string_display_At(	(char *)XL_account_buf.m_At2_hour,EDIT_XL_ACCOUNT_HOUR2);
	GUI_edit_string_display_At(	(char *)XL_account_buf.m_At3_day,EDIT_XL_ACCOUNT_DAY3);
	GUI_edit_string_display_At(	(char *)XL_account_buf.m_At3_hour,EDIT_XL_ACCOUNT_HOUR3);
	GUI_window_hwin_release( m_RMwin_XLedit );
	return;
}

/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :д�����Ĵ�����ʾ
* @���� :NONE
* @��� :NONE
********************************************************************/
static void XL_reg_write_display( void )
{
	GUI_window_hwin_req( m_XL_edit_Wmeter );
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_YELLOW);
	GUI_edit_string_display_At((char *)XL_reg_write_buf.m_XL_period,EDIT_XL_PERIOD);
	GUI_edit_string_display_At((char *)XL_reg_write_buf.m_HC_time,EDIT_XL_HC_TIME);
	GUI_window_hwin_release( m_XL_edit_Wmeter );
	return;
}

/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :�������Ĵ�����ʾ
* @���� :NONE
* @��� :NONE
********************************************************************/
static void XL_reg_read_display( void )
{
	GUI_window_hwin_req( m_XL_edit_Rmeter );
	GUI_set_Bgcolor(RM_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_YELLOW);
	if ( XL_reg_read_buf.m_calc_way == 1 )//����ʽ
		GUI_string_display_At(230,FL_Y_OFFSET*1+12,"����ʽ");
	else if ( XL_reg_read_buf.m_calc_way == 2 )//����ʽ
		GUI_string_display_At(230,FL_Y_OFFSET*1+12,"����ʽ");
	GUI_string_display_At(190,FL_Y_OFFSET*2+14,(char *)XL_reg_read_buf.m_XL_period);
	GUI_string_display_At(190,FL_Y_OFFSET*3+15,(char *)XL_reg_read_buf.m_HC_time);
	GUI_window_hwin_release( m_XL_edit_Rmeter );
	return;
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :(�༭�����ûص�����)
 * @���� :NONE
 * @��� :NONE
********************************************************************/
uint8_t app_RMmod_XLACC_setting( hwin_t*hp )
{
	uint8_t key;
	Bsize_t member;
	
	GUI_window_hwin_req( hp );
	hand_wirte_input_box_pos_set(380,315);
	key_input_method_filter( KEY_INPUT_123 );
	GUI_edit_select(EDIT_XL_PASSWORD);
	GUI_window_hwin_release( hp );
	app_global_key_control_power_req();

	while ( 1 )
	{
		msleep( TYPE_DELAY );
		if ( app_global_key_force_obtain( &key, 1 ) != 1 )/*�ж������Ƿ��յ�������Ӧ*/
		{
			continue;
		}
		GUI_window_hwin_req( hp );
		GUI_edit_run_hook( key );
		GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
		GUI_set_Fgcolor( RM_FG_COLOR );
		if (( app_global_decide_inputkey( key ) ) != FAIL )/*�ж��Ƿ������뷨��ذ�������*/
		{
			member = GUI_edit_id_obtain();     /*��ȡ��ǰ�༭��ԱID*/
			if ( member == EDIT_XL_PASSWORD)
			{
				key_method_input_shadow( key, m_meter_Password);
			}
			else if ( member == EDIT_XL_ACCOUNT_DAY1)
			{
				key_method_input_shadow( key, XL_account_buf.m_At1_day);
			}
			else if ( member == EDIT_XL_ACCOUNT_HOUR1)
			{
				key_method_input_shadow( key, XL_account_buf.m_At1_hour);
			}
			else if ( member == EDIT_XL_ACCOUNT_DAY2)
			{
				key_method_input_shadow( key, XL_account_buf.m_At2_day);
			}
			else if ( member == EDIT_XL_ACCOUNT_HOUR2)
			{
				key_method_input_shadow( key, XL_account_buf.m_At2_hour);
			}
			else if ( member == EDIT_XL_ACCOUNT_DAY3)
			{
				key_method_input_shadow( key, XL_account_buf.m_At3_day);
			}
			else if ( member == EDIT_XL_ACCOUNT_HOUR3)
			{
				key_method_input_shadow( key, XL_account_buf.m_At3_hour);
			}
		}
		else if(key == KEY_DOWN)GUI_edit_shift( EDIT_NEXT);	
		else if(key == KEY_UP)	GUI_edit_shift( EDIT_LAST );
		GUI_window_hwin_release( hp );
		if ( key == KEY_ENTER || key == GUI_KEY_ENTER )
		{
			m_meter_Password[8] = ASCII_NULL;
			break;
		}
		else if ( key == KEY_EXIT)
		{
			m_meter_Password[8] = ASCII_NULL;
			break;
		}
	}
	app_global_key_control_power_release();
	return key;
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :���������÷���
 * @���� :NONE
 * @��� :NONE
********************************************************************/
static void XLACC_set_analysis( void )
{
	uint8_t m_day1 	= 0;
	uint8_t m_hour1 = 0;
	uint8_t m_day2 	= 0;
	uint8_t m_hour2 = 0;
	uint8_t m_day3 	= 0;
	uint8_t m_hour3 = 0;

	m_day1 = global_atoi(XL_account_buf.m_At1_day);
	m_hour1 = global_atoi(XL_account_buf.m_At1_hour);
	m_day2 = global_atoi(XL_account_buf.m_At2_day);
	m_hour2 = global_atoi(XL_account_buf.m_At2_hour);
	m_day3 = global_atoi(XL_account_buf.m_At3_day);
	m_hour3 = global_atoi(XL_account_buf.m_At3_hour);

	if ( (m_day1 == 0) || (m_day1 > 31)||(m_day2 == 0) || (m_day2 > 31)||
		 (m_day3 == 0) || (m_day3 > 31))
	{
		MsgBox(300,300,200,200,"��ʾ!","������:������������Χ1��31,�������Ϊ��ǰ�����һ��.",APP_HALF_SEC*10);
		if ((m_day1 == 0) || (m_day1 > 31))
		{
			m_day1 = 1;
			global_itoa(m_day1, XL_account_buf.m_At1_day);
		}
		if ((m_day2 == 0) || (m_day2 > 31))
		{
			m_day2 = 1;
			global_itoa(m_day2, XL_account_buf.m_At2_day);
		}
		if ((m_day3 == 0) || (m_day3 > 31))
		{
			m_day3 = 1;
			global_itoa(m_day3, XL_account_buf.m_At3_day);
		}
	}
	
	if ( (m_hour1 > 23)|| (m_hour2 > 23)|| (m_hour3 > 23))
	{
		MsgBox(300,300,200,200,"��ʾ!","������:ʱ����������Χ0��23,�����������.",APP_HALF_SEC*10);
		if ( m_hour1 > 23 )
		{
			m_hour1 = 0;
			global_itoa(m_hour1, XL_account_buf.m_At1_hour);
		}
		if ( m_hour2 > 23 )
		{
			m_hour2 = 0;
			global_itoa(m_hour2, XL_account_buf.m_At2_hour);
		}
		if ( m_hour3 > 23 )
		{
			m_hour3 = 0;
			global_itoa(m_hour3, XL_account_buf.m_At3_hour);
		}
	}
	
	data_mend_zero_handle((uint8_t *)XL_account_buf.m_At1_day);
	data_mend_zero_handle((uint8_t *)XL_account_buf.m_At1_hour);
	data_mend_zero_handle((uint8_t *)XL_account_buf.m_At2_day);
	data_mend_zero_handle((uint8_t *)XL_account_buf.m_At2_hour);
	data_mend_zero_handle((uint8_t *)XL_account_buf.m_At3_day);
	data_mend_zero_handle((uint8_t *)XL_account_buf.m_At3_hour);
	return;
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :(�༭�����ûص�����)
 * @���� :NONE
 * @��� :NONE
********************************************************************/
uint8_t app_RMmod_XL_reg_setting( hwin_t*hp )
{
	uint8_t key;
	Bsize_t member;
	
	GUI_window_hwin_req( hp );
	hand_wirte_input_box_pos_set(380,315);
	key_input_method_filter( KEY_INPUT_123 );
	GUI_window_hwin_release( hp );
	app_global_key_control_power_req();

	while ( 1 )
	{
		msleep( TYPE_DELAY );
		if ( app_global_key_force_obtain( &key, 1 ) != 1 )/*�ж������Ƿ��յ�������Ӧ*/
		{
			continue;
		}
		GUI_window_hwin_req( hp );
		GUI_edit_run_hook( key );
		GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
		GUI_set_Fgcolor( RM_FG_COLOR );
		if (( app_global_decide_inputkey( key ) ) != FAIL )/*�ж��Ƿ������뷨��ذ�������*/
		{
			member = GUI_edit_id_obtain();     /*��ȡ��ǰ�༭��ԱID*/
			if ( member == EDIT_XL_PERIOD)
			{
				key_method_input_shadow( key, XL_reg_write_buf.m_XL_period);
			}
			else if ( member == EDIT_XL_HC_TIME)
			{
				key_method_input_shadow( key, XL_reg_write_buf.m_HC_time);
			}
		}
		else if(key == KEY_DOWN)GUI_edit_shift( EDIT_NEXT);	
		else if(key == KEY_UP)	GUI_edit_shift( EDIT_LAST );
		GUI_window_hwin_release( hp );
		if ( key == KEY_ENTER || key == GUI_KEY_ENTER )
		{
			break;
		}
		else if ( key == KEY_EXIT)
		{
			break;
		}
	}
	app_global_key_control_power_release();
	return key;
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :�����Ĵ������÷���
 * @���� :NONE
 * @��� :NONE
********************************************************************/
static void XL_reg_set_analysis( void )
{
	uint8_t m_int = 0; 
	
	m_int = global_atoi(XL_reg_write_buf.m_XL_period);
	if ( m_int < 5 || m_int > 60 )
	{
		MsgBox(300,200,200,200,"��ʾ!","��������������÷�ΧΪ5��60����,����ʱ�����÷�ΧΪ1��12����.",APP_HALF_SEC*10);
		m_int = 15;
		global_itoa(m_int, XL_reg_write_buf.m_XL_period);
	}
	m_int = global_atoi(XL_reg_write_buf.m_HC_time);
	if ( m_int > 12 || m_int == 0 )
	//if ( m_int > 12 )
	{
		MsgBox(300,200,200,200,"��ʾ!","��������������÷�ΧΪ5��60����,����ʱ�����÷�ΧΪ1��12����.",APP_HALF_SEC*10);
		m_int = 1;
		global_itoa(m_int, XL_reg_write_buf.m_HC_time);
	}

	data_mend_zero_handle((uint8_t *)XL_reg_write_buf.m_XL_period);
	data_mend_zero_handle((uint8_t *)XL_reg_write_buf.m_HC_time);
	
	return;
}

/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :97��Լ���ݷ���
* @���� :*data
* @��� :NONE
********************************************************************/
void app_RMmod_XLhandle_guiyue_97(uint8_t*data)
{
	uint8_t offset1		= 0;
	uint8_t s_data 		= 0;
	uint8_t hex_data 	= 0;
	uint8_t m_temp[4] 	= {0,};
	long    m_P_num 	= 0;
	uint8_t biaoshi1	= 0;
	uint8_t biaoshi2	= 0;

	biaoshi1=(data[11]-0x33) & 0xff;
	biaoshi2=(data[10]-0x33) & 0xff;

	if ( (biaoshi1 == m_XLcmd_97_DI1[0]) && (biaoshi2 == m_XLcmd_97_DI0[0]) )
	{
		s_data = data[12] - 0x33;
		if ( (s_data & 0xff) == 0xff )	s_data = 0;
		g_xl_MRstate_word1 = s_data;
		if ( (s_data & 0x04) == 0 )
			 XL_other_read_buf.m_battery_sate = 1;//����
		else XL_other_read_buf.m_battery_sate = 2;//Ƿѹ
		if ( (s_data & 0x02) == 0 )
			 XL_reg_read_buf.m_calc_way = 1;//����ʽ
		else XL_reg_read_buf.m_calc_way = 2;//����ʽ
	}
	else if ( (biaoshi1 == m_XLcmd_97_DI1[1]) && (biaoshi2 == m_XLcmd_97_DI0[1]) )
	{
		for ( offset1= 0;offset1 <2;offset1++ )
	 	{
	 		if(	data[12+offset1] < 0x33 ) s_data = 0;
			else
			{
				s_data = data[12+offset1] - 0x33;
				if ( (s_data & 0xff) == 0xff )	s_data = 0;
			}
			bcd_convert_hex(s_data,&hex_data);
			m_temp[offset1] = hex_data;
		}
		m_P_num = m_temp[0]+(long)(m_temp[1]*100);
		global_ltoa(m_P_num, (char *)XL_other_read_buf.m_P_num);
	}
	else if ( (biaoshi1 == m_XLcmd_97_DI1[2]) && (biaoshi2 == m_XLcmd_97_DI0[2]) )
	{
		for ( offset1= 0;offset1 < 4;offset1++ )
		{
			s_data = data[12+offset1] - 0x33;
			if ( (s_data & 0xff) == 0xff )	s_data = 0;
			m_temp[offset1] = s_data;
		}
		XL_other_read_buf.m_P_time[11] = '\0';
		XL_other_read_buf.m_P_time[10] = (m_temp[0] & 0x0f ) + 0x30;//��
		XL_other_read_buf.m_P_time[9] = (m_temp[0] >> 4 ) + 0x30;
		XL_other_read_buf.m_P_time[8] = ':';
		XL_other_read_buf.m_P_time[7] = (m_temp[1] & 0x0f ) + 0x30;//ʱ
		XL_other_read_buf.m_P_time[6] = (m_temp[1] >> 4 ) + 0x30;
		XL_other_read_buf.m_P_time[5] = ' ';
		XL_other_read_buf.m_P_time[4] = (m_temp[2] & 0x0f ) + 0x30;//��
		XL_other_read_buf.m_P_time[3] = (m_temp[2] >> 4 ) + 0x30;
		XL_other_read_buf.m_P_time[2] = '-';
		XL_other_read_buf.m_P_time[1] = (m_temp[3] & 0x0f ) + 0x30;//��
		XL_other_read_buf.m_P_time[0] = (m_temp[3] >> 4 ) + 0x30;
	}
	else if ( (biaoshi1 == m_XLcmd_97_DI1[3]) && (biaoshi2 == m_XLcmd_97_DI0[3]) )
	{
		for ( offset1= 0;offset1 < 3;offset1++ )
		{
			if(	data[12+offset1] < 0x33 ) s_data = 0;
			else
			{
				s_data = data[12+offset1] - 0x33;
				if ( (s_data & 0xff) == 0xff )	s_data = 0;
			}
			bcd_convert_hex(s_data,&hex_data);
			m_temp[offset1] = hex_data;
		}
		m_P_num = m_temp[0]+((long)m_temp[1]*100)+((long)m_temp[2]*10000);
		global_ltoa(m_P_num, (char *)XL_other_read_buf.m_battery_RunT);
		
	}
	else if ( (biaoshi1 == m_XLcmd_97_DI1[4]) && (biaoshi2 == m_XLcmd_97_DI0[4]) )
	{
		if(	data[12] < 0x33 ) s_data = 0;
		else
		{
			s_data = data[12] - 0x33;
			if ( (s_data & 0xff) == 0xff )	s_data = 0;
		}
		bcd_convert_hex(s_data,&m_temp[0]);
		global_itoa(m_temp[0], (char *)XL_reg_read_buf.m_XL_period);
		
	}
	else if ( (biaoshi1 == m_XLcmd_97_DI1[5]) && (biaoshi2 == m_XLcmd_97_DI0[5]) )
	{
		if(	data[12] < 0x33 ) s_data = 0;
		else
		{
			s_data = data[12] - 0x33;
			if ( (s_data & 0xff) == 0xff )	s_data = 0;
		}
		bcd_convert_hex(s_data,&m_temp[0]);
		global_itoa(m_temp[0], (char *)XL_reg_read_buf.m_HC_time);
	}
	return;
}
/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :07��Լ���ݷ���
* @���� :*data
* @��� :NONE
********************************************************************/
void app_RMmod_XLhandle_guiyue_07(uint8_t *data)
{
	uint8_t s_data 		= 0;
	uint8_t m_temp[6] 	= {0,};
	long	m_P_num 	= 0;
	uint8_t hex_data 	= 0;
	float   f_temp 		= 0;
	uint8_t offset1		= 0;
	uint8_t biaoshi1=0,biaoshi2=0,biaoshi3=0,biaoshi4=0;

	biaoshi1=(data[13]-0x33) & 0xff;
	biaoshi2=(data[12]-0x33) & 0xff;
	biaoshi3=(data[11]-0x33) & 0xff;	
	biaoshi4=(data[10]-0x33) & 0xff;

	if ( (biaoshi1 == m_XLcmd_07_DI3[0]) && (biaoshi2 == m_XLcmd_07_DI2[0]) &&
		 (biaoshi3 == m_XLcmd_07_DI1[0]) && (biaoshi4 == m_XLcmd_07_DI0[0]))
	{
		s_data = data[14] - 0x33;
		if ( (s_data & 0xff) == 0xff )	s_data = 0;
		g_xl_MRstate_word1 = s_data;
		if ( (s_data & 0x04) == 0 )
			 XL_other_read_buf.m_battery_sate = 1;//����
		else XL_other_read_buf.m_battery_sate = 2;//Ƿѹ
		if ( (s_data & 0x02) == 0 )
			 XL_reg_read_buf.m_calc_way = 1;//����ʽ
		else XL_reg_read_buf.m_calc_way = 2;//����ʽ
		s_data = data[15] - 0x33;
	}
	else if ( (biaoshi1 == m_XLcmd_07_DI3[1]) && (biaoshi2 == m_XLcmd_07_DI2[1]) &&
		 (biaoshi3 == m_XLcmd_07_DI1[1]) && (biaoshi4 == m_XLcmd_07_DI0[1]))
	{
		for ( offset1= 0;offset1 <2;offset1++ )
	 	{
	 		if(	data[14+offset1] < 0x33 ) s_data = 0;
			else
			{
				s_data = data[14+offset1] - 0x33;
				if ( (s_data & 0xff) == 0xff )	s_data = 0;
			}
			bcd_convert_hex(s_data,&hex_data);
			m_temp[offset1] = hex_data;
		}
		m_P_num = m_temp[0]+(uint32_t)(m_temp[1]*100);
		global_itoa(m_P_num, (char *)XL_other_read_buf.m_P_num);
	}
	else if ( (biaoshi1 == m_XLcmd_07_DI3[2]) && (biaoshi2 == m_XLcmd_07_DI2[2]) &&
		 (biaoshi3 == m_XLcmd_07_DI1[2]) && (biaoshi4 == m_XLcmd_07_DI0[2]))
	{
		for ( offset1= 0;offset1 < 6;offset1++ )
		{
			s_data = data[14+offset1] - 0x33;
			if ( (s_data & 0xff) == 0xff )	s_data = 0;
			m_temp[offset1] = s_data;
		}
		
		XL_other_read_buf.m_P_time[17] = '\0';
		XL_other_read_buf.m_P_time[16] = (m_temp[0] & 0x0f ) + 0x30;//��
		XL_other_read_buf.m_P_time[15] = (m_temp[0] >> 4 ) + 0x30;
		XL_other_read_buf.m_P_time[14] = ':';
		XL_other_read_buf.m_P_time[13] = (m_temp[1] & 0x0f ) + 0x30;//��
		XL_other_read_buf.m_P_time[12] = (m_temp[1] >> 4 ) + 0x30;
		XL_other_read_buf.m_P_time[11] = ':';
		XL_other_read_buf.m_P_time[10] = (m_temp[2] & 0x0f ) + 0x30;//ʱ
		XL_other_read_buf.m_P_time[9] = (m_temp[2] >> 4 ) + 0x30;
		XL_other_read_buf.m_P_time[8] = ' ';
		XL_other_read_buf.m_P_time[7] = (m_temp[3] & 0x0f ) + 0x30;//��
		XL_other_read_buf.m_P_time[6] = (m_temp[3] >> 4 ) + 0x30;
		XL_other_read_buf.m_P_time[5] = '-';
		XL_other_read_buf.m_P_time[4] = (m_temp[4] & 0x0f ) + 0x30;//��
		XL_other_read_buf.m_P_time[3] = (m_temp[4] >> 4 ) + 0x30;
		XL_other_read_buf.m_P_time[2] = '-';
		XL_other_read_buf.m_P_time[1] = (m_temp[5] & 0x0f ) + 0x30;//��
		XL_other_read_buf.m_P_time[0] = (m_temp[5] >> 4 ) + 0x30;

	}
	else if ( (biaoshi1 == m_XLcmd_07_DI3[3]) && (biaoshi2 == m_XLcmd_07_DI2[3]) &&
		 (biaoshi3 == m_XLcmd_07_DI1[3]) && (biaoshi4 == m_XLcmd_07_DI0[3]))
	{
		for ( offset1= 0;offset1 < 4;offset1++ )
		{
			if(	data[14+offset1] < 0x33 ) s_data = 0;
			else
			{
				s_data = data[14+offset1] - 0x33;
				if ( (s_data & 0xff) == 0xff )	s_data = 0;
			}
			bcd_convert_hex(s_data,&hex_data);
			m_temp[offset1] = hex_data;
		}
		m_P_num = m_temp[0]+((long)m_temp[1]*100)+((long)m_temp[2]*10000)+
					((long)m_temp[3]*1000000);
		global_ltoa(m_P_num, (char *)XL_other_read_buf.m_battery_RunT);
		
	}
	else if ( (biaoshi1 == m_XLcmd_07_DI3[4]) && (biaoshi2 == m_XLcmd_07_DI2[4]) &&
		 (biaoshi3 == m_XLcmd_07_DI1[4]) && (biaoshi4 == m_XLcmd_07_DI0[4]))
	{
		if(	data[14] < 0x33 ) s_data = 0;
		else
		{
			s_data = data[14] - 0x33;
			if ( (s_data & 0xff) == 0xff )	s_data = 0;
		}
		bcd_convert_hex(s_data,&m_temp[0]);
		global_itoa(m_temp[0], (char *)XL_reg_read_buf.m_XL_period);
		
	}
	else if ( (biaoshi1 == m_XLcmd_07_DI3[5]) && (biaoshi2 == m_XLcmd_07_DI2[5]) &&
		 (biaoshi3 == m_XLcmd_07_DI1[5]) && (biaoshi4 == m_XLcmd_07_DI0[5]))
	{
		if(	data[14] < 0x33 ) s_data = 0;
		else
		{
			s_data = data[14] - 0x33;
			if ( (s_data & 0xff) == 0xff )	s_data = 0;
		}
		bcd_convert_hex(s_data,&m_temp[0]);
		global_itoa(m_temp[0], (char *)XL_reg_read_buf.m_HC_time);
	}
	else if ( (biaoshi1 == m_XLcmd_07_DI3[6]) && (biaoshi2 == m_XLcmd_07_DI2[6]) &&
		 (biaoshi3 == m_XLcmd_07_DI1[6]) && (biaoshi4 == m_XLcmd_07_DI0[6]))
	{
		for ( offset1= 0;offset1 < 2;offset1++ )
		{
			if(	data[14+offset1] < 0x33 ) s_data = 0;
			else
			{
				s_data = data[14+offset1] - 0x33;
				if ( (s_data & 0xff) == 0xff )	s_data = 0;
			}
			bcd_convert_hex(s_data,&hex_data);
			m_temp[offset1] = hex_data;
		}
		f_temp = (float)(m_temp[0] * 0.01)+(float)m_temp[1];
		XL_other_read_buf.m_battery_V = f_temp;
	}
	else if ( (biaoshi1 == m_XLcmd_07_DI3[7]) && (biaoshi2 == m_XLcmd_07_DI2[7]) &&
		 (biaoshi3 == m_XLcmd_07_DI1[7]))
	{
		for ( offset1= 0;offset1 < 2;offset1++ )
		{
			if(	data[14+offset1] < 0x33 ) s_data = 0;
			else
			{
				s_data = data[14+offset1] - 0x33;
				if ( (s_data & 0xff) == 0xff )	s_data = 0;
			}
			bcd_convert_hex(s_data,&hex_data);
			m_temp[offset1] = hex_data;
		}
		if (biaoshi4 == m_XLcmd_07_DI0[7])
		{
			global_itoa(m_temp[0], (char *)XL_account_buf.m_At1_hour);
			global_itoa(m_temp[1], (char *)XL_account_buf.m_At1_day);
			data_mend_zero_handle((uint8_t *)XL_account_buf.m_At1_day);
			data_mend_zero_handle((uint8_t *)XL_account_buf.m_At1_hour);
		}
		else if (biaoshi4 == m_XLcmd_07_DI0[8])
		{
			global_itoa(m_temp[0], (char *)XL_account_buf.m_At2_hour);
			global_itoa(m_temp[1], (char *)XL_account_buf.m_At2_day);
			data_mend_zero_handle((uint8_t *)XL_account_buf.m_At2_day);
			data_mend_zero_handle((uint8_t *)XL_account_buf.m_At2_hour);
		}
		else if (biaoshi4 == m_XLcmd_07_DI0[9])
		{
			global_itoa(m_temp[0], (char *)XL_account_buf.m_At3_hour);
			global_itoa(m_temp[1], (char *)XL_account_buf.m_At3_day);
			data_mend_zero_handle((uint8_t *)XL_account_buf.m_At3_day);
			data_mend_zero_handle((uint8_t *)XL_account_buf.m_At3_hour);
		}
	}
	
	return;
}

/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :97��Լд�����Ĵ���
* @���� :m_offset:����ƫ��
* @��� :NONE
********************************************************************/
static void write_XLreg_to_meter_97( uint8_t m_offset )
{
	uint8_t m_cmd[2] = { 0,};
	uint8_t m_data_buf[4] = { 0,};
	uint8_t m_temp = 0;
	
	m_cmd[1] = m_XLcmd_97_DI1[m_offset];
	m_cmd[0] = m_XLcmd_97_DI0[m_offset];
	if ( m_offset == 0 )
	{
		if (XL_reg_write_buf.m_calc_way == 1 )
			m_data_buf[0] = (g_xl_MRstate_word1 & 0xFB);
		else if (XL_reg_write_buf.m_calc_way == 2 )
			m_data_buf[0] = (g_xl_MRstate_word1 | 0x04);
		else 
			m_data_buf[0] = g_xl_MRstate_word1;
	}
	else if ( m_offset == 4 )
	{
		m_temp = global_atoi(XL_reg_write_buf.m_XL_period);
		hex_convert_bcd(m_temp, &m_data_buf[0]);
	}
	else if ( m_offset == 5 )
	{
		m_temp = global_atoi(XL_reg_write_buf.m_HC_time);
		hex_convert_bcd(m_temp, &m_data_buf[0]);
	}
	app_RMmod_97_set_meter(m_cmd,(void *)m_data_buf,1);

	return;
}

/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :07��Լд�����Ĵ���
* @���� :m_offset:����ƫ��
* @��� :NONE
********************************************************************/
static void write_XLreg_to_meter_07( uint8_t m_offset )
{
	uint8_t m_cmd[4] = { 0,};
	uint8_t m_data_buf[4] = { 0,};
	uint8_t m_temp = 0;

	m_cmd[3] = m_XLcmd_07_DI3[m_offset];
	m_cmd[2] = m_XLcmd_07_DI2[m_offset];
	m_cmd[1] = m_XLcmd_07_DI1[m_offset];
	m_cmd[0] = m_XLcmd_07_DI0[m_offset];

	if ( m_offset == 4 )
	{
		m_temp = global_atoi(XL_reg_write_buf.m_XL_period);
		hex_convert_bcd(m_temp, &m_data_buf[0]);
	}
	else if ( m_offset == 5 )
	{
		m_temp = global_atoi(XL_reg_write_buf.m_HC_time);
		hex_convert_bcd(m_temp, &m_data_buf[0]);
	}
	
	app_RMmod_07_set_meter(m_cmd,(void *)m_data_buf,1);
	
	return;
}
/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :07��Լд������
* @���� :m_offset:����ƫ��
* @��� :NONE
********************************************************************/
static void write_AccDay_to_meter_07( uint8_t m_offset )
{
	uint8_t m_cmd[4] = { 0,};
	uint8_t m_data_buf[4] = { 0,};
	uint8_t m_temp = 0;

	m_cmd[3] = m_XLcmd_07_DI3[m_offset];
	m_cmd[2] = m_XLcmd_07_DI2[m_offset];
	m_cmd[1] = m_XLcmd_07_DI1[m_offset];
	m_cmd[0] = m_XLcmd_07_DI0[m_offset];

	if ( m_offset == 7 )
	{
		m_temp = global_atoi(XL_account_buf.m_At1_hour);
		hex_convert_bcd(m_temp, &m_data_buf[0]);
		m_temp = global_atoi(XL_account_buf.m_At1_day);
		hex_convert_bcd(m_temp, &m_data_buf[1]);
	}
	else if ( m_offset == 8 )
	{
		m_temp = global_atoi(XL_account_buf.m_At2_hour);
		hex_convert_bcd(m_temp, &m_data_buf[0]);
		m_temp = global_atoi(XL_account_buf.m_At2_day);
		hex_convert_bcd(m_temp, &m_data_buf[1]);
	}
	else if ( m_offset == 9 )
	{
		m_temp = global_atoi(XL_account_buf.m_At3_hour);
		hex_convert_bcd(m_temp, &m_data_buf[0]);
		m_temp = global_atoi(XL_account_buf.m_At3_day);
		hex_convert_bcd(m_temp, &m_data_buf[1]);
	}
	
	app_RMmod_07_set_meter(m_cmd,(void *)m_data_buf,2);
	
	return;
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :����д��ָ��
 * @���� :NONE
 * @��� :NONE
********************************************************************/
static void XL_send_write_meter_cmd(void)
{
	if(m_meter_data.guiyue==0)
	{
		write_XLreg_to_meter_97(g_Xl_send_write_offset);
		if ( g_Xl_send_write_offset == 0 )
			 g_Xl_send_write_offset = 4;
		else g_Xl_send_write_offset++;
		if ( (g_Xl_send_write_offset > 5) || (g_XL_rec_start_flag == 0))
		{
			g_Xl_send_write_offset = 0;
			g_XL_rec_start_flag = 0;
		}
	}
	else if(m_meter_data.guiyue==1)
	{
		if ( g_xl_send_write_flag == 2 )
		{
			write_XLreg_to_meter_07(g_Xl_send_write_offset);
			g_Xl_send_write_offset++;
			if ( (g_Xl_send_write_offset > 5) || (g_XL_rec_start_flag == 0))
			{
				g_Xl_send_write_offset = 0;
				g_XL_rec_start_flag = 0;
			}
		}
		else if ( g_xl_send_write_flag == 1 )
		{
			write_AccDay_to_meter_07(g_Xl_send_write_offset);
			g_Xl_send_write_offset++;
			if ( (g_Xl_send_write_offset > 9) || (g_XL_rec_start_flag == 0))
			{
				g_Xl_send_write_offset = 0;
				g_XL_rec_start_flag = 0;
			}
		}
	}
	return;
}

/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :ģ�鷢�Ͷ�97Э����������
* @���� :NONE
* @��� :NONE
********************************************************************/
static void XL_send_read_meter_cmd_97(uint8_t offset)
{
	send_data_97_t send_cmd_97={0,};
	GLOBAL_MEMCLR(&send_cmd_97, sizeof(send_data_97_t));
	send_cmd_97.head[0]=0xFE;
	send_cmd_97.head[1]=0xFE;
	send_cmd_97.head[2]=0xFE;
	send_cmd_97.head1=0x68;	
	send_cmd_97.addr[0] = (( CharToHex(&m_meter_addr[10]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[11]) & 0x0F );//0x99;
	send_cmd_97.addr[1] = (( CharToHex(&m_meter_addr[8]) & 0x0F ) << 4 ) + (CharToHex(& m_meter_addr[9]) & 0x0F );//0x99;
	send_cmd_97.addr[2] = (( CharToHex(&m_meter_addr[6]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[7]) & 0x0F );//0x99;
	send_cmd_97.addr[3] = (( CharToHex(&m_meter_addr[4]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[5]) & 0x0F );//0x99;
	send_cmd_97.addr[4] = (( CharToHex(&m_meter_addr[2]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[3]) & 0x0F );//0x99;
	send_cmd_97.addr[5] = (( CharToHex(&m_meter_addr[0]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[1]) & 0x0F );//0x99;
	send_cmd_97.head2=0x68;
	send_cmd_97.cmd=0x01;
	send_cmd_97.len=0x02;	
	send_cmd_97.biaoshi_L=m_XLcmd_97_DI0[offset]+0x33;
	send_cmd_97.biaoshi_H=m_XLcmd_97_DI1[offset]+0x33;
	send_cmd_97.crc=global_sum_array((char*)&send_cmd_97.head1,sizeof(send_data_97_t)-5,IS_CHAR);
	send_cmd_97.end=0x16;
	app_RMmod_Usart3_send((char*)&send_cmd_97,sizeof(send_data_97_t));
	
	return;
}
/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :ģ�鷢�Ͷ�07Э����������
* @���� :NONE
* @��� :NONE
********************************************************************/
static void XL_send_read_meter_cmd_07(uint8_t offset)
{
	send_data_07_t send_cmd_07={0,};
	GLOBAL_MEMCLR(&send_cmd_07, sizeof(send_data_07_t));
	send_cmd_07.head[0]=0xFE;
	send_cmd_07.head[1]=0xFE;
	send_cmd_07.head[2]=0xFE;
	send_cmd_07.head1=0x68;	
	send_cmd_07.addr[0] = (( CharToHex(&m_meter07_addr[10]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[11]) & 0x0F );//0x99;
	send_cmd_07.addr[1] = (( CharToHex(&m_meter07_addr[8]) & 0x0F ) << 4 ) + (CharToHex(& m_meter07_addr[9]) & 0x0F );//0x99;
	send_cmd_07.addr[2] = (( CharToHex(&m_meter07_addr[6]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[7]) & 0x0F );//0x99;
	send_cmd_07.addr[3] = (( CharToHex(&m_meter07_addr[4]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[5]) & 0x0F );//0x99;
	send_cmd_07.addr[4] = (( CharToHex(&m_meter07_addr[2]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[3]) & 0x0F );//0x99;
	send_cmd_07.addr[5] = (( CharToHex(&m_meter07_addr[0]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[1]) & 0x0F );//0x99;
	send_cmd_07.head2=0x68;
	send_cmd_07.cmd=0x11;
	send_cmd_07.len=0x04;	
	send_cmd_07.biaoshi[0]=m_XLcmd_07_DI0[offset]+0x33;
	send_cmd_07.biaoshi[1]=m_XLcmd_07_DI1[offset]+0x33;
	send_cmd_07.biaoshi[2]=m_XLcmd_07_DI2[offset]+0x33;
	send_cmd_07.biaoshi[3]=m_XLcmd_07_DI3[offset]+0x33;
	send_cmd_07.crc=global_sum_array((char*)&send_cmd_07.head1,sizeof(send_data_07_t)-5,IS_CHAR);
	send_cmd_07.end=0x16;
	app_RMmod_Usart3_send((char*)&send_cmd_07,sizeof(send_data_07_t));
	
	return;
}
/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :���Ͷ���ָ��
 * @���� :NONE
 * @��� :NONE
********************************************************************/
static void XL_send_read_meter_cmd(void)
{
	if(m_meter_data.guiyue==0)
	{
		XL_send_read_meter_cmd_97(g_xl_send_read_offset);
	}
	else if(m_meter_data.guiyue==1)
	{
		XL_send_read_meter_cmd_07(g_xl_send_read_offset);
	}
	return;
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :���Ͷ���ָ��ƫ�ƴ���
 * @���� :NONE
 * @��� :NONE
********************************************************************/
static void XL_97read_meter_cmd_handle(void)
{
	if ( g_xl_send_read_flag == 1 )
	{
		g_xl_send_read_offset++;
		if ( g_xl_send_read_offset > 3 )
		{
			g_XL_rec_start_flag = 0;
			g_xl_send_read_offset = 0;
			if ( g_xl_read_error_flag == 1 )
				 GUI_3D_window_title_mod(m_RMwin_XLWIN,"��ȡ���ܱ����,�޲�����������!");
			else GUI_3D_window_title_mod(m_RMwin_XLWIN,"��ȡ���ܱ����!");
		}
	}
	else if ( g_xl_send_read_flag == 2 )
	{
		if ( g_xl_send_read_offset == 4 )
		{
			g_xl_send_read_offset = 5;
		}
		else if ( g_xl_send_read_offset == 5 )
		{
			g_XL_rec_start_flag = 0;
			g_xl_send_read_offset = 0;
			if ( g_xl_read_error_flag == 1 )
				 GUI_3D_window_title_mod(m_RMwin_XLWIN,"��ȡ�����Ĵ������,�޲�����������!");
			else GUI_3D_window_title_mod(m_RMwin_XLWIN,"��������Ĵ�����ȡ���!");
		}
		else if ( g_xl_send_read_offset == 0 )
			g_xl_send_read_offset = 4;
	}

	return;
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :���Ͷ���ָ��ƫ�ƴ���
 * @���� :NONE
 * @��� :NONE
********************************************************************/
static void XL_07read_meter_cmd_handle(void)
{
	if ( g_xl_send_read_flag == 1 )
	{
		g_xl_send_read_offset++;
		if ( g_xl_send_read_offset > 9 )
		{
			g_XL_rec_start_flag = 0;
			g_xl_send_read_offset = 0;
			if ( g_xl_read_error_flag == 1 )
				 GUI_3D_window_title_mod(m_RMwin_XLWIN,"��ȡ���ܱ����,�޲�����������!");
			else GUI_3D_window_title_mod(m_RMwin_XLWIN,"��ȡ���ܱ����!");
		}
		else if ( g_xl_send_read_offset == 4 )
			g_xl_send_read_offset = 6;
	}
	else if ( g_xl_send_read_flag == 2 )
	{
		if ( g_xl_send_read_offset == 4 )
		{
			g_xl_send_read_offset = 5;
		}
		else if ( g_xl_send_read_offset == 5 )
		{
			g_XL_rec_start_flag = 0;
			g_xl_send_read_offset = 0;
			if ( g_xl_read_error_flag == 1 )
				 GUI_3D_window_title_mod(m_RMwin_XLWIN,"��ȡ�����Ĵ������,�޲�����������!");
			else GUI_3D_window_title_mod(m_RMwin_XLWIN,"��������Ĵ�����ȡ���!");
		}
		else if ( g_xl_send_read_offset == 0 )
			g_xl_send_read_offset = 4;
	}
	
	return;
}
/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :����ͨѶ���ݷ�������
* @���� :NONE
* @��� :NONE
********************************************************************/
static void _XL_rectask(void*arg)
{
	Bsize_t size;
	Bsize_t Fsize = 5;
	uint8_t m_error = 0;
	uint8_t m_cnt = 0;
	uint8_t rev_data[100];
	GLOBAL_MEMCLR(rev_data, 100);
	
	while(1)
	{
		msleep( TYPE_DELAY );
		
		if ( g_XL_rec_start_flag == 1 )
		{
			os_driver_ioctl(g_usart3_dev_id, SET_EMPTY_FIFO, 0);
			if ( g_xl_read_or_write == 0 )
				XL_send_read_meter_cmd();
			else if ( g_xl_read_or_write == 1 )
				XL_send_write_meter_cmd();
			msleep(10);
			while ( Fsize )
			{
				size = os_driver_read( g_usart3_dev_id, &rev_data[0], 1, 0 );
				Fsize--;
				if( (size == 1) && (rev_data[0] == 0x68) )
				{
					Fsize = 5;
					break;
				}
				else msleep(5);
			}
			if ( (Fsize == 0) && ((rev_data[0] != 0x68) || (size != 1)))
			{
				Fsize = 5;
				GUI_3D_window_title_mod(m_RMwin_XLWIN,"����ܱ�ͨѶ����!");
				m_cnt++;
				if ( m_cnt > 5 )
				{
					g_XL_rec_start_flag = 0;
					m_cnt = 0;
				}
				continue;
			}
			
			if ( m_offset_baud == 0 )	  msleep(80);	//����Ӧ
			else if ( m_offset_baud == 1 )msleep(90);	//300
			else if ( m_offset_baud == 2 )msleep(55);	//600
			else if ( m_offset_baud == 3 )msleep(40);	//1200
			else if ( m_offset_baud == 4 )msleep(20);	//2400
			else if ( m_offset_baud == 5 )msleep(15);	//4800
			else if ( m_offset_baud == 6 )msleep(10);	//9600
			else if ( m_offset_baud == 7 )msleep(6);	//19200
			if ( (m_meter_data.guiyue==1) && (g_xl_send_read_offset == 2) )
			{
				if ( m_offset_baud == 1 )		msleep(100);//300
				else if ( m_offset_baud == 2 )	msleep(60);	//600
				else if ( m_offset_baud == 3 )	msleep(30);	//1200
				else if ( m_offset_baud == 4 )	msleep(20);	//2400
				else if ( m_offset_baud == 5 )	msleep(15);	//4800
				else if ( m_offset_baud == 6 )	msleep(10);	//9600
				else if ( m_offset_baud == 7 )	msleep(5);	//19200
			}
			size = os_driver_read( g_usart3_dev_id, &rev_data[1], 7, 0 );
			if ( size != 7 || ( rev_data[7] != 0x68 ) )  //�յڶ���68H��
			{
				continue;
			}
			size = os_driver_read( g_usart3_dev_id, &rev_data[8], 2, 0 );
			if ( size != 2 )	  						//�տ�����ͳ���,8Ϊ������,9����;
			{
				continue;
			}
			size = os_driver_read( g_usart3_dev_id, &rev_data[10], rev_data[9], 0 );		
			size = os_driver_read( g_usart3_dev_id, &rev_data[10+rev_data[9]], 2, 0 );		
			if(size==2&&rev_data[10+rev_data[9]+1]==0x16)
			{
				if ( rev_data[9] == 0 )//��վ����Ӧ��
				{
					if (g_XL_rec_start_flag == 0)
					GUI_3D_window_title_mod(m_RMwin_XLWIN,"�������!");
					continue;
				}
				else if ( rev_data[9] == 1 )//��վ�쳣Ӧ��
				{
					m_error = rev_data[10] - 0x33; //������Ϣ��
					if ( rev_data[8] == 0xD4 || rev_data[8] == 0xC4 )//д���վ�쳣Ӧ��
					{
						if ( (m_error & 0x04) == 0x04 )
							 GUI_3D_window_title_mod(m_RMwin_XLWIN,"������δ��Ȩ,д��ʧ��!");
						else 
							 GUI_3D_window_title_mod(m_RMwin_XLWIN,"�쳣Ӧ����������,д��ʧ��!");
						g_XL_rec_start_flag = 0;
						continue;
					}
					else if ( rev_data[8] == 0xD1 || rev_data[8] == 0xC1 )//�����վ�쳣Ӧ��
					{
						if ( (m_error & 0x02) == 0x02 )
						{
							g_xl_read_error_flag = 1;
						}
						if(m_meter_data.guiyue==0) //97
							XL_97read_meter_cmd_handle();
						else if(m_meter_data.guiyue==1)//07
							XL_07read_meter_cmd_handle();
						continue;
					}
				}
			}
			if(m_meter_data.guiyue==0) //97
			{
				app_RMmod_XLhandle_guiyue_97(rev_data);
				XL_97read_meter_cmd_handle();
				if ( g_xl_send_read_flag == 2 ) 
					 XL_reg_read_display();
				else XL_ABP_display();
			}
			else if(m_meter_data.guiyue==1)//07
			{
				app_RMmod_XLhandle_guiyue_07(rev_data);
				XL_07read_meter_cmd_handle();
				if ( g_xl_send_read_flag == 2 ) 
					 XL_reg_read_display();
				else XL_ABP_display();
			}
		}
	}
}

/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :�����Ĵ�������ء������պͱ�̴���ʱ��������������
* @���� :NONE
* @��� :NONE
********************************************************************/
static void _XLtask(void*arg)
{
	uint8_t* key_buf = 0;
	uint8_t  err;

	while(1)
	{
		msleep( TYPE_DELAY );
		
		key_buf = (uint8_t*)os_q_pend(
					  m_RMmod_keymsg_qevent,
					  APP_100_MS, &err );       
		if ( err != OS_NO_ERR )
		continue;
		if ( *key_buf == KEY_ENTER )
		{

		}
		/*else if ( *key_buf == GUI_KEY_EDIT_START )
		{
			if ( Gui_start_editOBJ_obtain() == m_RMwin_XLedit )
	   	 	{
				GUI_edit_start( m_RMwin_XLedit,app_RMmod_XLACC_setting );
				XLACC_set_analysis();
				XL_ABP_display();
			}
			else if ( Gui_start_editOBJ_obtain() == m_XL_edit_Wmeter )
			{
				GUI_edit_start( m_XL_edit_Wmeter,app_RMmod_XL_reg_setting );
				XL_reg_set_analysis();
				XL_reg_write_display();
			}
		}*/
		else if ( *key_buf == GUI_KEY_XL_ACC_SET || *key_buf == KEY_SETTING )
		{
			GUI_edit_start( m_RMwin_XLedit,app_RMmod_XLACC_setting );
			XLACC_set_analysis();
			XL_ABP_display();
		}
		else if ( *key_buf == GUI_KEY_XL_WRITE_SET || *key_buf == KEY_F3)
		{
			GUI_edit_start( m_XL_edit_Wmeter,app_RMmod_XL_reg_setting );
			XL_reg_set_analysis();
			XL_reg_write_display();
		}
		/*else if ( *key_buf == GUI_KEY_XL_WAY )
		{
			g_XL_calc_way = GUI_drop_list_unfold(m_XL_drop_way);
			XL_reg_write_buf.m_calc_way = g_XL_calc_way;
		}*/
		else if ( *key_buf == GUI_KEY_XL_READ || *key_buf == KEY_F4)
		{
			if (MsgBox(200,200,200,200,"��ʾ!","��ȷ��ͨ��ͨѶ������ʹ�ñ�����,�����޷�����ܱ�ͨѶ,��<ȷ��>����",APP_HALF_SEC*10)
				== GUIE_KEY_ENTER )
			{
				g_xl_read_or_write  = 0;
				g_XL_rec_start_flag = 1;
				g_xl_send_read_flag = 2;
				g_xl_send_read_offset = 0;//ƫ��
				GUI_3D_window_title_mod(m_RMwin_XLWIN,"���ڶ�ȡ��������Ĵ���");
				g_xl_read_error_flag = 0;
			}
		}
		else if ( *key_buf == GUI_KEY_XL_WRITE || *key_buf == KEY_F2)
		{
			if (MsgBox(200,200,200,200,"��ʾ!","ȷ��ͨѶ������,���б�̿���,��򿪵��ܱ��̿���,�򿪺�<ȷ��>д��",APP_HALF_SEC*10)
				== GUIE_KEY_ENTER )
			{
				g_xl_send_write_flag = 2;
				g_xl_read_or_write   = 1;
				g_XL_rec_start_flag  = 1;
				g_Xl_send_write_offset = 4;//д����ƫ��
				GUI_3D_window_title_mod(m_RMwin_XLWIN,"����������������Ĵ���");
			}
		}
		else if ( *key_buf == GUI_KEY_XL_ABP_READ || *key_buf == KEY_F1)
		{
			if (MsgBox(200,200,200,200,"��ʾ!","��ȷ��ͨ��ͨѶ������ʹ�ñ�����,�����޷�����ܱ�ͨѶ,��<ȷ��>����",APP_HALF_SEC*10)
				== GUIE_KEY_ENTER )
			{
				g_xl_read_or_write  = 0;
				g_XL_rec_start_flag = 1;
				g_xl_send_read_flag = 1;
				g_xl_send_read_offset = 0;//ƫ��
				GUI_3D_window_title_mod(m_RMwin_XLWIN,"���ڶ�ȡ����������");
				g_xl_read_error_flag = 0;
			}
		}
		else if ( *key_buf == GUI_KEY_XL_ACC_WRITE || *key_buf == KEY_F5)
		{
			if ( m_meter_data.guiyue == 1 )
			{
				if (MsgBox(200,200,200,200,"��ʾ!","ȷ��ͨѶ������,���б�̿���,��򿪵��ܱ��̿���,�򿪺�<ȷ��>д��",APP_HALF_SEC*10)
				== GUIE_KEY_ENTER )
				{
					g_xl_send_write_flag = 1;
					g_xl_read_or_write 	 = 1;
					g_XL_rec_start_flag  = 1;
					g_Xl_send_write_offset = 7;//д������ƫ��
					GUI_3D_window_title_mod(m_RMwin_XLWIN,"�������ý�����");
				}
			}
			else 
			MsgBox(200,200,200,200,"��ʾ!","����������ֻ֧��DL/T 645-2007��Լ��!",APP_HALF_SEC*10);
		}
	}
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :����У�������Ĵ������������
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_RM_verify_funtion3( void )
{
	uint8_t key_buf;

	_XLtask_id 
		= os_task_assign_stk_build( _XLtask,3);
	_XLtask_recid
		= os_task_assign_stk_build( _XL_rectask,5);
	
	app_RMmod_XL_window_create();
	GLOBAL_MEMCLR(&XL_other_read_buf, sizeof(XL_other_read_t));
	GLOBAL_MEMCLR(&XL_account_buf, sizeof(XL_account_t));
	GLOBAL_MEMCLR(&XL_reg_read_buf, sizeof(XL_reg_t));
	GLOBAL_MEMCLR(&XL_reg_write_buf, sizeof(XL_reg_t));
	XL_ABP_display();
	while (1)
	{
		msleep( TYPE_DELAY );
		if ( app_global_key_obtain( &key_buf, 1 ) != 1 )  //��������ȡ����ֵ
		{
			continue;
		}
		if ( key_buf == KEY_EXIT || key_buf == GUI_KEY_EXIT)
		{
			app_RMmod_XL_window_destory();
			
			if ( _XLtask_id != INVALID_PRO )
			{
				os_task_delete( _XLtask_id );
				_XLtask_id = INVALID_PRO;
			}
			if ( _XLtask_recid != INVALID_PRO )
			{
				os_task_delete( _XLtask_recid );
				_XLtask_recid = INVALID_PRO;
			}
			break;
		}
		else 
		{
			os_q_post(m_RMmod_keymsg_qevent,(void *)&key_buf);
		}
	}
	return;
}



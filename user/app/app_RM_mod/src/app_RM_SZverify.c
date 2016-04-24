/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:����ʾֵ���У��
* ����ʱ��:2013-10-25
* ������˾ :����
* �ļ�����:app_RM_SZverify.c
* ���� :�ܱ�Ȩ
* ��������:ʵ�ֶ�������ʾֵ���У�鹦��
******************************************************************************************/
#include	"app_RM_inside.h"

hwin_t* m_RMwin_SZWIN  = PNULL;
hwin_t* m_RMwin_SZedit = PNULL;
hwin_t* m_RMwin_TTedit = PNULL;
hwin_t* m_RMwin_TMedit = PNULL;

static  uint8_t   _SZtask_id 	= INVALID_PRO;
static  uint8_t   _SZRECtask_id = INVALID_PRO;

// 1.�����������;2.����ʱ��;3.��ǰ�й�����kW(07);��ǰ�����й����������(97);
// 4.��ǰ�޹�����kvar(07);��ǰ�����޹����������(97);5.��ǰ��������kVA(07);

const uint8_t m_SZcmd_97_DI1[] = {0xc1,0xc1,0xA0,0xA1,};
const uint8_t m_SZcmd_97_DI0[] = {0x11,0x12,0x10,0x10,};

const uint8_t m_SZcmd_07_DI3[] = {0x04,0x04,0x02,0x02,0x02,};
const uint8_t m_SZcmd_07_DI2[] = {0x00,0x00,0x80,0x80,0x80,};
const uint8_t m_SZcmd_07_DI1[] = {0x01,0x01,0x00,0x00,0x00,};
const uint8_t m_SZcmd_07_DI0[] = {0x03,0x04,0x04,0x05,0x06,};

uint8_t g_SZ_rec_start_flag 		= 0;//��ʼ�������ݱ�־
uint8_t g_sz_read_or_write 			= 0;//���Ͷ���д�������־,0:��;1:д;
uint8_t g_sz_send_read_offset 		= 0;//���Ͷ�ȡ�������ƫ��
uint8_t g_sz_send_read_flag  		= 0;//���Ͷ��������־,1:�������Ĵ���;
										//2:����ǰ����(�й�);
uint8_t	g_sz_read_error_flag 		= 0;//��������־
uint8_t g_sz_testPt_offset			= 0;//���Ե�ƫ��
uint8_t g_sz_test_start_flag		= 0;//У�鿪ʼ��־,0:ֹͣ;1:��ʼ;
uint8_t g_sz_test_hc_num			= 0;//���������ڻ������;
uint8_t g_sz_after_hc_num			= 0;//�������ڹ��󻬴�����;
uint8_t g_sz_test_step_flag			= 0;//���Բ����־;
uint8_t g_sz_test_XL_zq_finish		= 0;//����������ɱ�־;1Ϊ���;
uint8_t g_sz_after_hc_finish_flag	= 0;//�������ں󻬴���ɱ�־;1Ϊ���;
uint8_t g_sz_test_time_start		= 0;//����ʱ�俪ʼ��־;
uint8_t g_sz_step1_start			= 0;//����1��ʼ��־;

unsigned long g_sz_time_start  		= 0;//��ʱ��ʼ����,1��Ӧ10ms
unsigned long g_sz_time_test 		= 0;//���������С,1��Ӧ10ms(�����������)
unsigned long g_sz_time_hc	 		= 0;//���������С,1��Ӧ10ms(����ʱ��)

sz_TT_t		  g_TT_buf   	= { 0, };
sz_result_t	  g_result_buf 	= { 0, };

float		  g_cur_xl_buf 			= 0;
uint32_t	  g_energy_hc_buf[15] 	= { 0, };
uint32_t	  g_energy_after_buf[10]= { 0, };

static const float SZ_const_energy  = 72000.0;		//���ܳ���
uint32_t	SZ_energy_count_start 	= 0;
uint32_t	SZ_energy_count_end 	= 0;
float		SZ_energy_count_real 	= 0.00000;

#define   	SZ_COUNTER_ADDR			0x42b8
#define     SZ_Pulse_Get(param)		{param = READ32BIT_DATA(SZ_COUNTER_ADDR);}

#define  SZ_Y_OFFSET	30
#define  ERROR_STANDARD	0.5
static void SZ_result_display( void );

/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :״̬����ʼ��
* @���� :NONE
* @��� :NONE
********************************************************************/
static void sz_state_flag_init( void )
{
	g_sz_test_start_flag 		= 0;
	g_SZ_rec_start_flag 		= 0;
	g_sz_send_read_flag 		= 0;
	g_sz_test_hc_num			= 0;
	g_sz_after_hc_num			= 0;
	g_sz_test_step_flag			= 0;
	g_sz_test_XL_zq_finish		= 0;
	g_sz_after_hc_finish_flag	= 0;
	g_sz_read_error_flag		= 0;
	g_sz_test_time_start 		= 0;
	g_sz_step1_start			= 0;
	return;
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :�������Ե��ʼ������
 * @���� :NONE
 * @��� :NONE
********************************************************************/
static void XL_Test_Time_window( hwin_t *m_hp )
{
	hwin_t *hp = PNULL;
	GUI_window_hwin_req(m_hp);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_set_Bgcolor(RM_WIN_BG_COLOR);
    GUI_set_Fgcolor(RM_FG_COLOR);
	GUI_WINDIS_HLINE(0,317,35);
	GUI_string_display_At(85, SZ_Y_OFFSET*0+5, "����ʱ�����");
	
	GUI_edit_memeber_add(5, SZ_Y_OFFSET*3+37, 2,"�������ڹ���:", MEMBER_3D|EDIT_SZ_HC_NUM);
	GUI_string_display_At(198, SZ_Y_OFFSET*3+37, "�λ���");
	
	GUI_edit_memeber_add(5, SZ_Y_OFFSET*1+15, 2,"�����������:", MEMBER_3D|EDIT_SZ_PERIOD|EDIT_DIS);
	GUI_edit_memeber_add(5, SZ_Y_OFFSET*2+27, 2,"�� �� ʱ �� :", MEMBER_3D|EDIT_SZ_HC_TIME|EDIT_DIS);
	GUI_string_display_At(198, SZ_Y_OFFSET*1+15, "��");
	GUI_string_display_At(198, SZ_Y_OFFSET*2+27, "��");
	
	GUI_edit_memeber_add(5, SZ_Y_OFFSET*5+12, 8,"��������ʱ��:", MEMBER_3D|EDIT_DIS|EDIT_SZ_RUN_TIME);
	GUI_string_display_At(270, SZ_Y_OFFSET*5+12, "��");

	GUI_string_display_At(8, SZ_Y_OFFSET*6+20, "��ǰ����״̬:");
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	hp = Gui_button_creat(225,SZ_Y_OFFSET*1+10,80,33,BUTTON_PAINT|GUI_KEY_SZ_TT_SET|KEY_F1,m_hp );
	GUI_button_title_display( hp, "F1_����" );
	hp = Gui_button_creat(225,SZ_Y_OFFSET*2+25,80,33,BUTTON_PAINT|GUI_KEY_SZ_TT_READ|KEY_F2,m_hp );
	GUI_button_title_display( hp, "F2_����" );

	GUI_window_hwin_release(m_hp);

	return;
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :�������Ե��ʼ������
 * @���� :NONE
 * @��� :NONE
********************************************************************/
static void XL_Test_Main_window( hwin_t *m_hp )
{
	hwin_t *hp = PNULL;
	GUI_window_hwin_req(m_hp);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_set_Bgcolor(RM_WIN_BG_COLOR);
    GUI_set_Fgcolor(RM_FG_COLOR);
	GUI_WINDIS_HLINE(0,317,35);
	GUI_string_display_At(85, SZ_Y_OFFSET*0+5, "����������");
	
	GUI_edit_memeber_add(5, SZ_Y_OFFSET*2+10, 8,"д��Ȩ������:", MEMBER_3D|EDIT_SZ_PASSWORD);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	//hp = Gui_button_creat(30,SZ_Y_OFFSET*1+10,60,26,BUTTON_PAINT|GUI_KEY_SZ_TM_SET|KEY_SETTING,m_hp );
	//GUI_button_title_display( hp, "����" );
	hp = Gui_button_creat(90,SZ_Y_OFFSET*1+10,150,26,BUTTON_PAINT|GUI_KEY_SZ_TM_XLCLEAR|KEY_F3,m_hp );
	GUI_button_title_display( hp, "F3_�����������" );
	GUI_SetFront(&GUI_HZK24_EXTASCII);

	GUI_edit_memeber_add(5,SZ_Y_OFFSET*(3)+10,8,"����������:",MEMBER_3D|EDIT_DIS|(EDIT_SZ_TM_MXL1));
	GUI_edit_memeber_add(5,SZ_Y_OFFSET*(4)+10,8,"ʵ���������:",MEMBER_3D|EDIT_DIS|(EDIT_SZ_TM_RXL1));
	GUI_edit_memeber_add(5,SZ_Y_OFFSET*(5)+10,8,"����ʾֵ���:",MEMBER_3D|EDIT_DIS|(EDIT_SZ_TM_SZE1));
	GUI_edit_memeber_add(5,SZ_Y_OFFSET*(6)+10,7,"���",MEMBER_3D|EDIT_DIS|(EDIT_SZ_TM_ERR1));
	//GUI_edit_memeber_add(5+170,SZ_Y_OFFSET*(6)+10,6,"����",MEMBER_3D|EDIT_DIS|(EDIT_SZ_TM_RES1));
	GUI_string_display_At(5+265, SZ_Y_OFFSET*(3)+13, "kW");
	GUI_string_display_At(5+265, SZ_Y_OFFSET*(4)+13, "kW");
	GUI_string_display_At(5+265, SZ_Y_OFFSET*(5)+13, "kW");
	GUI_string_display_At(5+145, SZ_Y_OFFSET*(6)+13, "%");
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	hp = Gui_button_creat(20,m_hp->win_high-35,120,30,BUTTON_PAINT|GUI_KEY_SZ_TM_START|KEY_F5,m_hp );
	GUI_button_title_display( hp, "F4_��ʼ����" );
	hp = Gui_button_creat(170,m_hp->win_high-35,120,30,BUTTON_PAINT|GUI_KEY_SZ_TM_STOP|KEY_F5,m_hp );
	GUI_button_title_display( hp, "F5_ֹͣ����" );
	
	GUI_window_hwin_release(m_hp);

	return;
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :��������
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_RMmod_SZ_window_create(void) 
{ 
	GUI_window_hwin_req(GET_GUIWIN);
  	GUI_set_Bgcolor(RM_WIN_BG_COLOR);
	GUI_set_Fgcolor(RM_FG_COLOR);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
    GUI_clean_screen();
	m_RMwin_SZWIN=Gui_3D_window_creat(0,2,636,475,"�������ʾֵ�������",WIN_3D | WIN_FILL,GET_GUIWIN);
    GUI_window_hwin_req(m_RMwin_SZWIN);
	m_RMwin_SZedit
		= GUI_edit_creat(2,2,635,450,WIN_TOUCH_EN,m_RMwin_SZWIN);
	m_RMwin_DLWIN = GUI_edit_creat(2,265,630,170,WIN_3D,m_RMwin_SZWIN );
    app_RM_DLmember_window_init(m_RMwin_DLWIN);
	GUI_window_hwin_req( m_RMwin_SZedit );
	GUI_set_Bgcolor(RM_WIN_BG_COLOR);
	GUI_set_Fgcolor(RM_FG_COLOR);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	
	m_RMwin_TTedit = GUI_edit_creat(0,0,317,263,WIN_3D|WIN_TOUCH_EN,m_RMwin_SZWIN);
	XL_Test_Time_window(m_RMwin_TTedit);
	m_RMwin_TMedit = GUI_edit_creat(317,0,317,263,WIN_3D|WIN_TOUCH_EN,m_RMwin_SZWIN);
	XL_Test_Main_window(m_RMwin_TMedit);

	GUI_window_hwin_release(m_RMwin_SZedit);	
	GUI_window_hwin_release(m_RMwin_SZWIN);		
	GUI_window_hwin_release(GET_GUIWIN);
	return;
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :��������
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_RMmod_SZ_window_destory(void)
{
	GUI_window_destory(m_RMwin_SZedit);
    GUI_window_destory(m_RMwin_SZWIN);
	return;
}

/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :����ǰ״̬��ʾ
* @���� :NONE
* @��� :NONE
********************************************************************/
static void cur_program_state( char *m_content )
{
	GUI_window_hwin_req( m_RMwin_TTedit );
	GUI_set_Bgcolor(RM_WIN_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_GREEN);
	GUI_string_display_At(170, FL_Y_OFFSET*6+20, (char *)m_content);
	GUI_window_hwin_release( m_RMwin_TTedit );
	return;
}

static void cur_program_hc_num( uint8_t m_num )
{
	GUI_window_hwin_req( m_RMwin_TTedit );
	GUI_set_Bgcolor(RM_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_GREEN);
	GUI_int_display_At(324, FL_Y_OFFSET*6+20, (long)m_num);
	GUI_window_hwin_release( m_RMwin_TTedit );
	return;
}

/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :��������ʱ����ʾ
* @���� :NONE
* @��� :NONE
********************************************************************/
static void cur_program_run_time( void )
{
	unsigned long m_cur_time = OSTimeGet();
	unsigned long m_cur_second = 0;
	
	m_cur_second = (unsigned long)((m_cur_time - g_sz_time_start) / 100);
	GUI_window_hwin_req( m_RMwin_TTedit );
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_GREEN);
	GUI_edit_int_display_At(m_cur_second,EDIT_SZ_RUN_TIME);
	GUI_window_hwin_release( m_RMwin_TTedit );
	return;
}

/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :���ַ��㴦��
* @���� :NONE
* @��� :NONE
********************************************************************/
static void data_mend_zero_handle2( uint8_t *m_buf )
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
* @���� :��ǰ���Ե�����������
* @���� :NONE
* @��� :NONE
********************************************************************/
static void cur_TP_result_handle( void )
{
	float m_Mxl = 0;
	float m_Rxl = 0;
	float m_sz_err = 0;
	float m_error = 0;
	float m_max_xl = 0;
	uint8_t m_multi = 0;
	uint8_t m_offset = 0;
	uint8_t m_period = global_atoi(g_TT_buf.m_XL_period);
	uint8_t m_hc_num = global_atoi(g_TT_buf.m_HC_num);
		
	if ( g_sz_after_hc_num == 0 )
	{	//�޻��������������
		m_Mxl = GLOBAL_ABS(g_cur_xl_buf);
		global_ftoa(m_Mxl, g_result_buf.m_Mxl[g_sz_testPt_offset], 4);
		m_Rxl = global_atof(g_result_buf.m_Rxl[g_sz_testPt_offset]);
	}
	else //�л��������������
	{
		//��������������
		m_Mxl = GLOBAL_ABS(g_cur_xl_buf);
		global_ftoa(m_Mxl, g_result_buf.m_Mxl[g_sz_testPt_offset], 4);

		//���������������
		m_max_xl = global_atof(g_result_buf.m_Rxl[g_sz_testPt_offset]);
		m_multi = (uint8_t)(60 / m_period);
		for ( m_offset = 0;m_offset < g_sz_after_hc_num;m_offset++)
		{
			if ( g_energy_after_buf[m_offset] > g_energy_hc_buf[m_offset] )
			{
				m_Rxl = (float)(g_energy_after_buf[m_offset] - g_energy_hc_buf[m_offset]);
				m_Rxl = m_Rxl / SZ_const_energy;
			}
			else if ( g_energy_after_buf[m_offset] < g_energy_hc_buf[m_offset] )
			{
				m_Rxl = (float)(0xffffffff - g_energy_hc_buf[m_offset] + g_energy_after_buf[m_offset]);
				m_Rxl = m_Rxl / SZ_const_energy;
			}
			m_Rxl = m_Rxl * m_multi;
			if ( m_Rxl > m_max_xl )
				m_max_xl = m_Rxl;
		}
		m_Rxl = m_max_xl;
		global_ftoa(m_Rxl,g_result_buf.m_Rxl[g_sz_testPt_offset],4);
	}
	m_sz_err = m_Mxl - m_Rxl;
	m_error  = m_sz_err / m_Rxl * 100;
	if(GLOBAL_ABS(m_error)>100) m_error = 100; 
	if(m_error>0&&m_error<0.001)
	{
		m_error=0.001;
	}
	if(m_error<0&&m_error>-0.001)
	{
		m_error=-0.001;
	}
	
	global_ftoa(m_sz_err, g_result_buf.m_XL_SZR[g_sz_testPt_offset], 4);
	global_ftoa(m_error, g_result_buf.m_XL_ERR[g_sz_testPt_offset], 4);
	if ( GLOBAL_ABS(m_error) <= ERROR_STANDARD )
	{
		global_memcpy(g_result_buf.m_XL_RES[g_sz_testPt_offset], "�ϸ�", 6);
	}
	else 
		global_memcpy(g_result_buf.m_XL_RES[g_sz_testPt_offset], "���ϸ�", 6);
	
	SZ_result_display();
	
	return;
}

/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :���鲽��1
* @���� :NONE
* @��� :NONE
********************************************************************/
static void sz_test_step1( void )
{
	static uint8_t m_cnt1= 0;
	
	if ( g_sz_step1_start == 1 )
	{
		if(MsgBox(200,200,300,200,"��ʾ!","�����򿪱�̿��ػ��߰����ܱ��ϵ��������㰴�����������������,���򽫵��²��ɿ���������,��<ȷ��>��ʼ",APP_HALF_SEC*10)
				== GUIE_KEY_ENTER )
		{
			//�����������
			g_sz_read_or_write = 1;
			g_SZ_rec_start_flag = 1;
			g_sz_read_error_flag = 0;
			//�����������
			GUI_3D_window_title_mod(m_RMwin_SZWIN,"���ڽ������������������!");
			cur_program_state("���ڲ��Ե�ǰ����    ");
			g_sz_step1_start = 0;
		}
	}
	else if ( (++m_cnt1) > 30 )
	{
		m_cnt1 = 0;
		sz_state_flag_init();
		GUI_3D_window_title_mod(m_RMwin_SZWIN,"�����������ʧ��,������ܱ��Ƿ�֧�����������������!");
		cur_program_state("�����Ѿ�ֹͣ        ");
	}
	return;
}

/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :���鲽��2
* @���� :NONE
* @��� :NONE
********************************************************************/
static void sz_test_step2( void )
{
	unsigned long m_cur_time = 0;
	unsigned long m_hc_time  = 0;
	
	m_cur_time = OSTimeGet();
	m_cur_time = m_cur_time - g_sz_time_start;
	if ( m_cur_time >= g_sz_time_test )	//���һ����������
	{
		g_sz_test_hc_num++;
		g_sz_read_or_write  = 0;
		g_SZ_rec_start_flag = 1;
		g_sz_send_read_flag = 2;		//����ǰ�й�������־
		g_sz_send_read_offset = 2;  	//����ǰ�й�����
		g_sz_test_XL_zq_finish = 1;
		cur_program_state("��ǰ���������������");
		g_sz_after_hc_num = 0;
	}
	else
	{
		m_hc_time  = g_sz_time_hc * (g_sz_test_hc_num+1);
		if ( m_cur_time > m_hc_time )
		{
			g_sz_test_hc_num++;
			g_sz_read_or_write  = 0;
			g_SZ_rec_start_flag = 1;
			g_sz_send_read_flag = 2;	//����ǰ�й�������־
			g_sz_send_read_offset = 2;  //����ǰ�й�����
		}
	}

	return;
}

/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :���鲽��3
* @���� :NONE
* @��� :NONE
********************************************************************/
static void sz_test_step3( void )
{
	uint8_t  m_hc_num = 0;
	static   uint8_t m_hc_flag = 0;
	unsigned long m_cur_time = 0;
	unsigned long m_hc_time  = 0;
	
	m_hc_num = global_atoi(g_TT_buf.m_HC_num);
	if ( m_hc_num > 0 )
	{
		if ( g_sz_after_hc_finish_flag == 0 )
		{
			m_cur_time = OSTimeGet();
			m_cur_time = m_cur_time-g_sz_time_start-g_sz_time_test;//���һ���������ں󻬴�
			m_hc_time  = g_sz_time_hc * (g_sz_after_hc_num+1);
			if ( m_hc_flag == 0 )
			{
				cur_program_state("���������    �λ���");
				cur_program_hc_num((g_sz_after_hc_num+1));
				m_hc_flag = 1;
			}
			if ( m_cur_time > m_hc_time )
			{
				g_sz_after_hc_num++;
				cur_program_state("���������    �λ���");
				cur_program_hc_num((g_sz_after_hc_num+1));
				m_hc_num -= g_sz_after_hc_num;
				if ( m_hc_num > 0 )
				{
					g_sz_read_or_write  = 0;
					g_SZ_rec_start_flag = 1;
					g_sz_send_read_flag = 2;	//����ǰ�й�������־
					g_sz_send_read_offset = 2;  //����ǰ�й�����
				}
				else if ( m_hc_num == 0 )
				{
					g_sz_read_or_write  = 0;
					g_SZ_rec_start_flag = 1;
					g_sz_send_read_flag = 2;	//����ǰ�й�������־
					g_sz_send_read_offset = 2;  //����ǰ�й�����
					g_sz_after_hc_finish_flag = 1;//�������
					cur_program_state("��ǰ�����������    ");
					m_hc_flag = 0;
				}
			}
		}
	}
	else if ( m_hc_num == 0 )
	{
		g_sz_after_hc_num = 0;
		g_sz_test_step_flag = 4;
		cur_program_state("�޻�������          ");
		m_hc_flag = 0;
	}
	
	return;
}

/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :���鲽�账��
* @���� :NONE
* @��� :NONE
********************************************************************/
static void sz_test_handle( void )
{	
	if ( g_sz_test_start_flag == 1 )
	{	
		if ( g_sz_test_time_start == 1 )
		cur_program_run_time();
		if ( g_sz_test_step_flag == 1)		//����1:��������,��ȡ��ʼʱ��
		{
			sz_test_step1();
		}
		else if ( g_sz_test_step_flag == 2 )//����2:���������ڻ���ʱ�����������
		{			
			sz_test_step2();
		}
		else if ( g_sz_test_step_flag == 3 )//����3:�������ں�Ļ�����������
		{
			sz_test_step3();
		}
		else if ( g_sz_test_step_flag == 4 )//����4:��ǰ���Ե�Ľ������
		{
			cur_program_state("��ǰ���Ե�������  ");
			GUI_3D_window_title_mod(m_RMwin_SZWIN,"��ǰ���Ե��������!");
			cur_TP_result_handle();
			g_sz_test_step_flag = 5;
		}
		else if ( g_sz_test_step_flag == 5 )//����5:��һ�����Ե����д���
		{
			sz_state_flag_init();
			GUI_3D_window_title_mod(m_RMwin_SZWIN,"�����Ѿ����!");
			cur_program_state("����������������  ");
		}
	}
	return;
}

/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :ʵ�����������ȡ
* @���� :NONE
* @��� :NONE
********************************************************************/
static void real_max_xl_get( void )
{
	//float m_real_xl = 0;
	uint8_t m_multiple = 0;
	float m_cur_xl  = 0;
	uint8_t m_xl_period = global_atoi(g_TT_buf.m_XL_period);
	if ( g_sz_send_read_offset == 2 || g_sz_send_read_offset == 3 )
	{
		SZ_Pulse_Get(SZ_energy_count_end);//��ȡ��ǰ�ۻ����ܳ�ʼֵ
		if ( SZ_energy_count_end > SZ_energy_count_start )
		{
			SZ_energy_count_real = (float)(SZ_energy_count_end - SZ_energy_count_start);
			SZ_energy_count_real = SZ_energy_count_real / SZ_const_energy;
		}
		else if ( SZ_energy_count_end < SZ_energy_count_start )
		{
			SZ_energy_count_real = (float)(0xffffffff - SZ_energy_count_start + SZ_energy_count_end);
			SZ_energy_count_real = SZ_energy_count_real / SZ_const_energy;
		}
	}
	m_multiple = (uint8_t)(60 / m_xl_period); //ǧ��ʱת�ɵ�ǰ������������ƽ������ֵ
	//m_real_xl = global_atof(g_result_buf.m_Rxl[g_sz_testPt_offset]);
	m_cur_xl  = SZ_energy_count_real * m_multiple;//�����ĵ�ǰ�й�����
	//if ( m_cur_xl > m_real_xl )
	{
		global_ftoa(m_cur_xl, g_result_buf.m_Rxl[g_sz_testPt_offset],4);
	}
	
	return;
}

/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :�������Ĵ�����ʾ
* @���� :NONE
* @��� :NONE
********************************************************************/
static void SZ_XLreg_read_display( void )
{
	GUI_window_hwin_req( m_RMwin_TTedit );
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_YELLOW);
	GUI_edit_string_display_At((char *)g_TT_buf.m_XL_period,EDIT_SZ_PERIOD);
	GUI_edit_string_display_At((char *)g_TT_buf.m_HC_time,EDIT_SZ_HC_TIME);
	GUI_edit_string_display_At((char *)g_TT_buf.m_HC_num,EDIT_SZ_HC_NUM);
	GUI_window_hwin_release( m_RMwin_TTedit );
	return;
}

/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :���������ʾ
* @���� :NONE
* @��� :NONE
********************************************************************/
static void SZ_password_display( void )
{
	GUI_window_hwin_req( m_RMwin_TMedit );
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_YELLOW);
	GUI_edit_string_display_At(m_meter_Password,EDIT_SZ_PASSWORD);
	GUI_window_hwin_release( m_RMwin_TMedit );
	return;
}

/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :�����������ʾ
* @���� :NONE
* @��� :NONE
********************************************************************/
static void SZ_result_display( void )
{
	GUI_window_hwin_req( m_RMwin_TMedit );
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_YELLOW);
	GUI_edit_string_display_At(g_result_buf.m_Mxl[0],EDIT_SZ_TM_MXL1);
	GUI_edit_string_display_At(g_result_buf.m_Rxl[0],EDIT_SZ_TM_RXL1);
	GUI_set_Fgcolor(C_GUI_GREEN);
	GUI_edit_string_display_At(g_result_buf.m_XL_SZR[0],EDIT_SZ_TM_SZE1);
	GUI_edit_string_display_At(g_result_buf.m_XL_ERR[0],EDIT_SZ_TM_ERR1);
	//GUI_set_Fgcolor(C_GUI_YELLOW);
	//GUI_edit_string_display_At(g_result_buf.m_XL_RES[0],EDIT_SZ_TM_RES1);
	GUI_window_hwin_release( m_RMwin_TMedit );
	return;
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :(�༭�����ûص�����)
 * @���� :NONE
 * @��� :NONE
********************************************************************/
uint8_t app_RMmod_SZcode_setting( hwin_t*hp )
{
	uint8_t key;
	Bsize_t member;
	
	GUI_window_hwin_req( hp );
	hand_wirte_input_box_pos_set(150,100);
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
			if ( member == EDIT_SZ_PASSWORD )
			{
				key_method_input_shadow( key, m_meter_Password);
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
 * @���� :(�༭�����ûص�����)
 * @���� :NONE
 * @��� :NONE
********************************************************************/
uint8_t app_RMmod_SZTT_setting( hwin_t*hp )
{
	uint8_t key;
	Bsize_t member;
	
	GUI_window_hwin_req( hp );
	hand_wirte_input_box_pos_set(20,315);
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
			if ( member == EDIT_SZ_HC_NUM )
			{
				key_method_input_shadow( key, g_TT_buf.m_HC_num );
			}
			/*else if ( member == EDIT_SZ_PERIOD )
			{
				key_method_input_shadow( key, g_TT_buf.m_XL_period );
			}
			else if ( member == EDIT_SZ_HC_TIME )
			{
				key_method_input_shadow( key, g_TT_buf.m_HC_time );
			}*/
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
* @���� :97�����������
* @���� :NONE
* @��� :NONE
********************************************************************/
static void XL_clear_97( void )
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
	send_cmd_97.cmd=0x10;
	send_cmd_97.len=0x00;	
	send_cmd_97.biaoshi_L=global_sum_array((char*)&send_cmd_97.head1,sizeof(send_data_97_t)-7,IS_CHAR);
	send_cmd_97.biaoshi_H=0x16;
	//send_cmd_97.crc=global_sum_array((char*)&send_cmd_97.head1,sizeof(send_data_97_t)-5,IS_CHAR);
	//send_cmd_97.end=0x16;
	app_RMmod_Usart3_send((char*)&send_cmd_97,sizeof(send_data_97_t)-2);

	return;
}

/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :07�����������
* @���� :NONE
* @��� :NONE
********************************************************************/
static void XL_clear_07( void )
{
	char m_buf[25] = { 0, };

	m_buf[0]  = 0xFE;
	m_buf[1]  = 0xFE;
	m_buf[2]  = 0xFE;
	m_buf[3]  = 0x68;
	m_buf[4]  = (( CharToHex(&m_meter07_addr[10]) & 0x0F )<< 4 ) + ( CharToHex(&m_meter07_addr[11])& 0x0F );
	m_buf[5]  = (( CharToHex(&m_meter07_addr[8]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[9]) & 0x0F );
	m_buf[6]  = (( CharToHex(&m_meter07_addr[6]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[7]) & 0x0F );
	m_buf[7]  = (( CharToHex(&m_meter07_addr[4]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[5]) & 0x0F );
	m_buf[8]  = (( CharToHex(&m_meter07_addr[2]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[3]) & 0x0F );
	m_buf[9]  = (( CharToHex(&m_meter07_addr[0]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[1]) & 0x0F );
	m_buf[10] = 0x68;
	m_buf[11] = 0x19;
	m_buf[12] = 0x08;
	//Ȩ�޵�һλ,����������(˵��:��������Ϊ12 34 56 78  ,����ʱ��:12 78 56 34)
	m_buf[13] = 0x33+ (( CharToHex(&m_meter_Password[0]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_Password[1]) & 0x0F );
	m_buf[14] = 0x33+ (( CharToHex(&m_meter_Password[6]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_Password[7]) & 0x0F );
	m_buf[15] = 0x33+ (( CharToHex(&m_meter_Password[4]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_Password[5]) & 0x0F );
	m_buf[16] = 0x33+ (( CharToHex(&m_meter_Password[2]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_Password[3]) & 0x0F );
	m_buf[17] = 0x00+0x33;
	m_buf[18] = 0x00+0x33;
	m_buf[19] = 0x00+0x33;
	m_buf[20] = 0x00+0x33;
	m_buf[21] = global_sum_array((char*)&m_buf[3],18,IS_CHAR);
	m_buf[22] = 0x16;
	
	app_RMmod_Usart3_send((char*)&m_buf[0],23);
	return;
}

/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :�����������
* @���� :NONE
* @��� :NONE
********************************************************************/
static void MAX_XL_Clear( void )
{
	long m_min = 0;
	if(m_meter_data.guiyue==0) //97
	{
		XL_clear_97();
		g_SZ_rec_start_flag = 0;
		//��ȡ��ǰʱ�����
		m_min = global_atoi(g_TT_buf.m_XL_period);
		g_sz_time_test  = (unsigned long)(m_min * 6000);//����ת��10ms
		m_min = global_atoi(g_TT_buf.m_HC_time);
		g_sz_time_hc    = (unsigned long)(m_min * 6000);//����ת��10ms
		g_sz_test_time_start = 1;
		g_sz_time_start = OSTimeGet();
		//��ȡ��ǰʱ�����
		SZ_Pulse_Get(SZ_energy_count_start);//��ȡ��ǰ�ۻ����ܳ�ʼֵ
		if ( g_sz_test_step_flag == 1 )
		{
			g_sz_test_step_flag = 2;
			g_sz_test_hc_num = 0;
		}
	}
	else if(m_meter_data.guiyue==1)//07
	{
		XL_clear_07();
		g_SZ_rec_start_flag = 0;
		//��ȡ��ǰʱ�����
		m_min = global_atoi(g_TT_buf.m_XL_period);
		g_sz_time_test  = (unsigned long)(m_min * 6000);//����ת��10ms
		m_min = global_atoi(g_TT_buf.m_HC_time);
		g_sz_time_hc    = (unsigned long)(m_min * 6000);//����ת��10ms
		g_sz_test_time_start = 1;
		g_sz_time_start = OSTimeGet();
		//��ȡ��ǰʱ�����
		SZ_Pulse_Get(SZ_energy_count_start);//��ȡ��ǰ�ۻ����ܳ�ʼֵ
	}

	return;
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :�����Ĵ������÷���
 * @���� :NONE
 * @��� :NONE
********************************************************************/
static void SZ_XLreg_set_analysis( void )
{
	uint8_t m_int = 0; 
	uint8_t m_xlpd= 0;
	uint8_t m_temp= 0;
	uint8_t m_hc_time = 0;
	uint8_t m_hc_after_time = 0;
	m_int = global_atoi(g_TT_buf.m_XL_period);
	if ( m_int < 5 || m_int > 60 )
	{
		MsgBox(300,200,200,200,"��ʾ!","��������������÷�ΧΪ5~60����,����ʱ�����÷�ΧΪ1~12����.",APP_HALF_SEC*10);
		m_int = 15;
		global_itoa(m_int, g_TT_buf.m_XL_period);
	}
	m_xlpd= global_atoi(g_TT_buf.m_XL_period);
	m_int = global_atoi(g_TT_buf.m_HC_time);
	if ( m_int > 12 || m_int == 0 )
	{
		MsgBox(300,200,200,200,"��ʾ!","��������������÷�ΧΪ5~60����,����ʱ�����÷�ΧΪ1~12����.",APP_HALF_SEC*10);
		m_int = 1;
		global_itoa(m_int, g_TT_buf.m_HC_time);
	}
	m_temp = (uint8_t)(m_xlpd / m_int);
	if ( m_temp < 5 )
	{
		MsgBox(300,200,200,200,"��ʾ!","��������������ñ�����ڻ����5������ʱ��",APP_HALF_SEC*10);
		m_int = 1;
		global_itoa(m_int, g_TT_buf.m_HC_time);
		m_xlpd = 15;
		global_itoa(m_xlpd, g_TT_buf.m_XL_period);
	}
	m_int = global_atoi(g_TT_buf.m_HC_num);
	m_hc_time = global_atoi(g_TT_buf.m_HC_time);
	m_hc_after_time = m_int * m_hc_time;
	if ( m_int > 10 || m_int >= m_xlpd || m_hc_after_time > m_xlpd )
	{
		MsgBox(300,200,200,200,"��ʾ!","�����������ڹ��󻬴��������÷�ΧΪ0~10��,�һ�����ʱ��С����������.",APP_HALF_SEC*10);
		m_int = 0;
		global_itoa(m_int, g_TT_buf.m_HC_num);
	}
	
	data_mend_zero_handle2((uint8_t *)g_TT_buf.m_XL_period);
	data_mend_zero_handle2((uint8_t *)g_TT_buf.m_HC_time);
	data_mend_zero_handle2((uint8_t *)g_TT_buf.m_HC_num);
	
	return;
}

/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :97��Լ���ݷ���
* @���� :*data
* @��� :NONE
********************************************************************/
void app_RMmod_SZhandle_guiyue_97(uint8_t*data)
{
	uint8_t s_data = 0;
	uint8_t m_temp[6] = {0,};
	uint8_t offset1=0;
	uint8_t biaoshi1=0;
	uint8_t biaoshi2=0;
	float   f_data=0;  
	float 	fbs;
	
	biaoshi1=(data[11]-0x33) & 0xff;
	biaoshi2=(data[10]-0x33) & 0xff;	

	if ( (biaoshi1 == m_SZcmd_97_DI1[0]) && (biaoshi2 == m_SZcmd_97_DI0[0]) )
	{
		if(	data[12] < 0x33 ) s_data = 0;
		else
		{
			s_data = data[12] - 0x33;
			if ( (s_data & 0xff) == 0xff )	s_data = 0;
		}
		bcd_convert_hex(s_data,&m_temp[0]);
		global_itoa(m_temp[0], (char *)g_TT_buf.m_XL_period);
		data_mend_zero_handle2((uint8_t *)g_TT_buf.m_XL_period);
	}
	else if ( (biaoshi1 == m_SZcmd_97_DI1[1]) && (biaoshi2 == m_SZcmd_97_DI0[1]) )
	{
		if(	data[12] < 0x33 ) s_data = 0;
		else
		{
			s_data = data[12] - 0x33;
			if ( (s_data & 0xff) == 0xff )	s_data = 0;
		}
		bcd_convert_hex(s_data,&m_temp[0]);
		global_itoa(m_temp[0], (char *)g_TT_buf.m_HC_time);
		data_mend_zero_handle2((uint8_t *)g_TT_buf.m_HC_time);
	}
	else if ((biaoshi1 == m_SZcmd_97_DI1[2]) && (biaoshi2 == m_SZcmd_97_DI0[2]))
	{//��ǰ�����й�����
		fbs= 1.0;
		f_data = 0;
		for ( offset1= 0;offset1 <3;offset1++ )
	 	{
			s_data = data[12+offset1] - 0x33;
			if ( (s_data & 0xff) == 0xff )	s_data = 0;
			f_data += fbs * (((s_data>> 4 ) & 0x0F ) * 10 + ( s_data & 0x0F ) );
			fbs *= 100.0;
		}
		f_data = f_data * 0.0001;
		g_cur_xl_buf = f_data;
		//if ( g_sz_test_step_flag == 2 && g_sz_test_hc_num < 11 )
			 //g_xl_hc_buf[g_sz_test_hc_num-1] = f_data;			 
		//else if ( g_sz_test_step_flag == 3 )
			//g_after_xl_buf[g_sz_after_hc_num-1] = f_data;
	}
	else if ((biaoshi1 == m_SZcmd_97_DI1[3]) && (biaoshi2 == m_SZcmd_97_DI0[3]))
	{//��ǰ�����޹�����
		fbs= 1.0;
		f_data = 0;
		for ( offset1= 0;offset1 <3;offset1++ )
	 	{
			s_data = data[12+offset1] - 0x33;
			if ( (s_data & 0xff) == 0xff )	s_data = 0;
			f_data += fbs * (((s_data>> 4 ) & 0x0F ) * 10 + ( s_data & 0x0F ) );
			fbs *= 100.0;
		}
		f_data = f_data * 0.0001;
	}
	
	return;
}

/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :07��Լ���ݷ���
* @���� :*data
* @��� :NONE
********************************************************************/
void app_RMmod_SZhandle_guiyue_07(uint8_t *data)
{
	uint8_t s_data = 0;
	uint8_t m_temp[6] = {0,};
	uint8_t offset2=0;
	uint8_t biaoshi1=0,biaoshi2=0,biaoshi3=0,biaoshi4=0;
	float   f_data=0;  
	float 	fbs;
	
	biaoshi1=(data[13]-0x33) & 0xff;
	biaoshi2=(data[12]-0x33) & 0xff;
	biaoshi3=(data[11]-0x33) & 0xff;	
	biaoshi4=(data[10]-0x33) & 0xff;

	if ( (biaoshi1 == m_SZcmd_07_DI3[0]) && (biaoshi2 == m_SZcmd_07_DI2[0]) &&
		 (biaoshi3 == m_SZcmd_07_DI1[0]) && (biaoshi4 == m_SZcmd_07_DI0[0]))
	{
		if(	data[14] < 0x33 ) s_data = 0;
		else
		{
			s_data = data[14] - 0x33;
			if ( (s_data & 0xff) == 0xff )	s_data = 0;
		}
		bcd_convert_hex(s_data,&m_temp[0]);
		global_itoa(m_temp[0], (char *)g_TT_buf.m_XL_period);
		data_mend_zero_handle2((uint8_t *)g_TT_buf.m_XL_period);
	}
	else if ( (biaoshi1 == m_SZcmd_07_DI3[1]) && (biaoshi2 == m_SZcmd_07_DI2[1]) &&
		 (biaoshi3 == m_SZcmd_07_DI1[1]) && (biaoshi4 == m_SZcmd_07_DI0[1]))
	{
		if(	data[14] < 0x33 ) s_data = 0;
		else
		{
			s_data = data[14] - 0x33;
			if ( (s_data & 0xff) == 0xff )	s_data = 0;
		}
		bcd_convert_hex(s_data,&m_temp[0]);
		global_itoa(m_temp[0], (char *)g_TT_buf.m_HC_time);
		data_mend_zero_handle2((uint8_t *)g_TT_buf.m_HC_time);
	}
	else if ( (biaoshi1 == m_SZcmd_07_DI3[2]) && (biaoshi2 == m_SZcmd_07_DI2[2]) &&
		 (biaoshi3 == m_SZcmd_07_DI1[2]) && (biaoshi4 == m_SZcmd_07_DI0[2]))
	{
		fbs = 1.0;
		f_data = 0;
		for (offset2 = 0;offset2 < 3;offset2++ )
		{
			s_data = data[14+offset2] - 0x33;
			if ( (s_data & 0xff) == 0xff )	s_data = 0;
			if ( (offset2 == 2) && (s_data & 0x80) == 0x80 )
				s_data &= 0x7f;
			f_data += fbs * (((s_data>> 4 ) & 0x0F ) * 10 + ( s_data & 0x0F ) );
			fbs *= 100.0;
		}
		f_data = f_data * 0.0001;
		if (((data[14+2] - 0x33) & 0x80) == 0x80 )
			f_data = -f_data;
		g_cur_xl_buf = f_data;
		//if ( g_sz_test_step_flag == 2 && g_sz_test_hc_num < 11 )
			 //g_xl_hc_buf[g_sz_test_hc_num-1] = f_data;			 
		//else if ( g_sz_test_step_flag == 3 )
			//g_after_xl_buf[g_sz_after_hc_num-1] = f_data;
	}
	else if ( (biaoshi1 == m_SZcmd_07_DI3[3]) && (biaoshi2 == m_SZcmd_07_DI2[3]) &&
		 (biaoshi3 == m_SZcmd_07_DI1[3]) && (biaoshi4 == m_SZcmd_07_DI0[3]))
	{
		fbs = 1.0;
		f_data = 0;
		for (offset2 = 0;offset2 < 3;offset2++ )
		{
			s_data = data[14+offset2] - 0x33;
			if ( (s_data & 0xff) == 0xff )	s_data = 0;
			if ( (offset2 == 2) && (s_data & 0x80) == 0x80 )
				s_data &= 0x7f;
			f_data += fbs * (((s_data>> 4 ) & 0x0F ) * 10 + ( s_data & 0x0F ) );
			fbs *= 100.0;
		}
		f_data = f_data * 0.0001;
		if (((data[14+2] - 0x33) & 0x80) == 0x80 )
			f_data = -f_data;
	}
	else if ( (biaoshi1 == m_SZcmd_07_DI3[4]) && (biaoshi2 == m_SZcmd_07_DI2[4]) &&
		 (biaoshi3 == m_SZcmd_07_DI1[4]) && (biaoshi4 == m_SZcmd_07_DI0[4]))
	{
		fbs = 1.0;
		f_data = 0;
		for (offset2 = 0;offset2 < 3;offset2++ )
		{
			s_data = data[14+offset2] - 0x33;
			if ( (s_data & 0xff) == 0xff )	s_data = 0;
			if ( (offset2 == 2) && (s_data & 0x80) == 0x80 )
				s_data &= 0x7f;
			f_data += fbs * (((s_data>> 4 ) & 0x0F ) * 10 + ( s_data & 0x0F ) );
			fbs *= 100.0;
		}
		f_data = f_data * 0.0001;
		if (((data[14+2] - 0x33) & 0x80) == 0x80 )
			f_data = -f_data;
	}
	
	return;
}

/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :ģ�鷢�Ͷ�97Э����������
* @���� :NONE
* @��� :NONE
********************************************************************/
static void SZ_send_read_meter_cmd_97(uint8_t offset)
{
	send_data_97_t send_cmd_97={0,};
	GLOBAL_MEMCLR(&send_cmd_97, sizeof(send_data_97_t));
	send_cmd_97.head[0]=0xFE;
	send_cmd_97.head[1]=0xFE;
	send_cmd_97.head[2]=0xFE;
	send_cmd_97.head1	= 0x68;	
	send_cmd_97.addr[0] = (( CharToHex(&m_meter_addr[10]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[11]) & 0x0F );//0x99;
	send_cmd_97.addr[1] = (( CharToHex(&m_meter_addr[8]) & 0x0F ) << 4 ) + (CharToHex(& m_meter_addr[9]) & 0x0F );//0x99;
	send_cmd_97.addr[2] = (( CharToHex(&m_meter_addr[6]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[7]) & 0x0F );//0x99;
	send_cmd_97.addr[3] = (( CharToHex(&m_meter_addr[4]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[5]) & 0x0F );//0x99;
	send_cmd_97.addr[4] = (( CharToHex(&m_meter_addr[2]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[3]) & 0x0F );//0x99;
	send_cmd_97.addr[5] = (( CharToHex(&m_meter_addr[0]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[1]) & 0x0F );//0x99;
	send_cmd_97.head2	= 0x68;
	send_cmd_97.cmd		= 0x01;
	send_cmd_97.len		= 0x02;	
	send_cmd_97.biaoshi_L=m_SZcmd_97_DI0[offset]+0x33;
	send_cmd_97.biaoshi_H=m_SZcmd_97_DI1[offset]+0x33;
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
static void SZ_send_read_meter_cmd_07(uint8_t offset)
{
	send_data_07_t send_cmd_07={0,};
	GLOBAL_MEMCLR(&send_cmd_07, sizeof(send_data_07_t));
	send_cmd_07.head[0]=0xFE;
	send_cmd_07.head[1]=0xFE;
	send_cmd_07.head[2]=0xFE;
	send_cmd_07.head1	= 0x68;	
	send_cmd_07.addr[0] = (( CharToHex(&m_meter07_addr[10]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[11]) & 0x0F );//0x99;
	send_cmd_07.addr[1] = (( CharToHex(&m_meter07_addr[8]) & 0x0F ) << 4 ) + (CharToHex(& m_meter07_addr[9]) & 0x0F );//0x99;
	send_cmd_07.addr[2] = (( CharToHex(&m_meter07_addr[6]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[7]) & 0x0F );//0x99;
	send_cmd_07.addr[3] = (( CharToHex(&m_meter07_addr[4]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[5]) & 0x0F );//0x99;
	send_cmd_07.addr[4] = (( CharToHex(&m_meter07_addr[2]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[3]) & 0x0F );//0x99;
	send_cmd_07.addr[5] = (( CharToHex(&m_meter07_addr[0]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[1]) & 0x0F );//0x99;
	send_cmd_07.head2	= 0x68;
	send_cmd_07.cmd		= 0x11;
	send_cmd_07.len		= 0x04;	
	send_cmd_07.biaoshi[0]=m_SZcmd_07_DI0[offset]+0x33;
	send_cmd_07.biaoshi[1]=m_SZcmd_07_DI1[offset]+0x33;
	send_cmd_07.biaoshi[2]=m_SZcmd_07_DI2[offset]+0x33;
	send_cmd_07.biaoshi[3]=m_SZcmd_07_DI3[offset]+0x33;
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
static void SZ_send_read_meter_cmd(void)
{
	if(m_meter_data.guiyue==0)
	{
		SZ_send_read_meter_cmd_97(g_sz_send_read_offset);
	}
	else if(m_meter_data.guiyue==1)
	{
		SZ_send_read_meter_cmd_07(g_sz_send_read_offset);
	}
	if( g_sz_test_step_flag == 3 )
	{
		SZ_Pulse_Get(g_energy_after_buf[g_sz_after_hc_num-1]);
	}
	else if( g_sz_test_step_flag == 2 && g_sz_test_XL_zq_finish == 1 && 
		g_sz_send_read_offset == 2 )
	{
		real_max_xl_get();
	}
	else if( g_sz_test_step_flag == 2 && g_sz_send_read_offset == 2 &&
		 g_sz_test_hc_num < 16 )
	{
		SZ_Pulse_Get(g_energy_hc_buf[g_sz_test_hc_num-1]);
	}
	return;
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :��ȡ��ǰ�����������������
 * @���� :NONE
 * @��� :NONE
********************************************************************/
static void read_xl_step_handle( void )
{
	if ( g_sz_test_step_flag == 2 )
	{
		if ( g_sz_test_XL_zq_finish == 1 )
		{
			g_sz_test_step_flag = 3;
		}
	}
	else if ( g_sz_test_step_flag == 3 )
	{
		if ( g_sz_after_hc_finish_flag == 1 )
		{
			g_sz_test_step_flag = 4;
		}
	}
	else if ( g_sz_test_step_flag == 4 )
	{
		g_sz_test_step_flag = 5;
	}
	return;
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :���Ͷ���ָ��ƫ�ƴ���
 * @���� :NONE
 * @��� :NONE
********************************************************************/
static void SZ_97read_meter_cmd_handle(void)
{
	if ( g_sz_send_read_flag == 1 )
	{
		g_sz_send_read_offset++;
		if ( g_sz_send_read_offset > 2 )
		{
			g_SZ_rec_start_flag = 0;
			g_sz_send_read_offset = 0;
			if ( g_sz_read_error_flag == 1 )
				 GUI_3D_window_title_mod(m_RMwin_SZWIN,"��ȡ���ܱ����,�޲�����������!");
			else GUI_3D_window_title_mod(m_RMwin_SZWIN,"��ȡ���ܱ����!");
		}
	}
	else if ( g_sz_send_read_flag == 2 )
	{
		g_SZ_rec_start_flag = 0;
		if ( g_sz_read_error_flag == 1 )
		{
			g_sz_test_start_flag = 0;
			g_sz_test_step_flag = 0;
			GUI_3D_window_title_mod(m_RMwin_SZWIN,"��ȡ��ǰ�й�����ʧ��,������ܱ��Ƿ�߱�������ǰ��������!");
			cur_program_state("�����Ѿ�ֹͣ        ");
		}
		else
		{
			read_xl_step_handle();
			GUI_3D_window_title_mod(m_RMwin_SZWIN,"�ɹ���ȡ��ǰ�й�����,����������!");
		}
	}
	return;
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :���Ͷ���ָ��ƫ�ƴ���
 * @���� :NONE
 * @��� :NONE
********************************************************************/
static void SZ_07read_meter_cmd_handle(void)
{
	if ( g_sz_send_read_flag == 1 )
	{
		g_sz_send_read_offset++;
		if ( g_sz_send_read_offset > 2 )
		{
			g_SZ_rec_start_flag = 0;
			g_sz_send_read_offset = 0;
			if ( g_sz_read_error_flag == 1 )
				 GUI_3D_window_title_mod(m_RMwin_SZWIN,"��ȡ���ܱ����,�޲�����������!");
			else GUI_3D_window_title_mod(m_RMwin_SZWIN,"��ȡ���ܱ����!");
		}
	}
	else if ( g_sz_send_read_flag == 2 )
	{
		g_SZ_rec_start_flag = 0;
		if ( g_sz_read_error_flag == 1 )
		{
			g_sz_test_start_flag = 0;
			g_sz_test_step_flag = 0;
			GUI_3D_window_title_mod(m_RMwin_SZWIN,"��ȡ��ǰ�й�����ʧ��,������ܱ��Ƿ�߱�������ǰ��������!");
			cur_program_state("�����Ѿ�ֹͣ        ");
		}
		else 
		{
			read_xl_step_handle();
			GUI_3D_window_title_mod(m_RMwin_SZWIN,"�ɹ���ȡ��ǰ�й�����,����������!");
		}
	}
	
	return;
}

/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :����ͨѶ���ݷ�������
* @���� :NONE
* @��� :NONE
********************************************************************/
static void _SZ_rectask(void* arg)
{
	Bsize_t size;
	Bsize_t Fsize = 5;
	uint8_t m_cnt = 0;
	uint8_t m_error = 0;
	uint8_t rev_data[100];
	GLOBAL_MEMCLR(rev_data, 100);
	
	while(1)
	{
		msleep( TYPE_DELAY );
		
		if ( g_SZ_rec_start_flag == 1 )
		{
			os_driver_ioctl(g_usart3_dev_id, SET_EMPTY_FIFO, 0);
			if ( g_sz_read_or_write == 0)
				SZ_send_read_meter_cmd();
			else if ( g_sz_read_or_write == 1)
				MAX_XL_Clear();
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
				if ( g_sz_read_or_write != 1 && m_meter_data.guiyue != 0 )
					GUI_3D_window_title_mod(m_RMwin_SZWIN,"����ܱ�ͨѶ����!");
				m_cnt++;
				if ( m_cnt > 5 )
				{
					g_SZ_rec_start_flag = 0;
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
					if (g_SZ_rec_start_flag == 0)
					GUI_3D_window_title_mod(m_RMwin_SZWIN,"��������������,����������!");
					if ( g_sz_test_step_flag == 1 )
					{
						g_sz_test_step_flag = 2;
						g_sz_test_hc_num	= 0;
					}
					continue;
				}
				else if ( rev_data[9] == 1 )//��վ�쳣Ӧ��
				{
					m_error = rev_data[10] - 0x33; //������Ϣ��
					if ( rev_data[8] == 0xD9 )//д���վ�쳣Ӧ��(07��Լ�����������ʧ��)
					{
						if ( (m_error & 0x04) == 0x04 )
							 GUI_3D_window_title_mod(m_RMwin_SZWIN,"������δ��Ȩ,�����������ʧ��!");
						else 
							 GUI_3D_window_title_mod(m_RMwin_SZWIN,"�쳣Ӧ����������,�����������ʧ��!");
						g_SZ_rec_start_flag = 0;
						app_global_key_dummy_press(GUI_KEY_SZ_TM_STOP);//��������
						continue;
					}
					else if ( rev_data[8] == 0xD1 || rev_data[8] == 0xC1)//�����վ�쳣Ӧ��
					{
						if ( (m_error & 0x02) == 0x02 )
						{
							g_sz_read_error_flag = 1;
						}
						if(m_meter_data.guiyue==0) //97
							SZ_97read_meter_cmd_handle();
						else if(m_meter_data.guiyue==1)//07
							SZ_07read_meter_cmd_handle();
						continue;
					}
				}
			}
			if(m_meter_data.guiyue==0) //97
			{
				app_RMmod_SZhandle_guiyue_97(rev_data);
				SZ_97read_meter_cmd_handle();
				if ( g_sz_send_read_flag == 1 )
					SZ_XLreg_read_display();
			}
			else if(m_meter_data.guiyue==1)//07
			{
				app_RMmod_SZhandle_guiyue_07(rev_data);
				SZ_07read_meter_cmd_handle();
				if ( g_sz_send_read_flag == 1 )
					SZ_XLreg_read_display();
			}
		}
	}
}

/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :����ʾֵ�����������
* @���� :NONE
* @��� :NONE
********************************************************************/
static void _SZtask(void*arg)
{
	uint8_t* key_buf = 0;
	uint8_t  err;

	while(1)
	{
		msleep( TYPE_DELAY );

		sz_test_handle();

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
			if ( g_sz_test_start_flag == 0 )
			{
				if ( Gui_start_editOBJ_obtain() == m_RMwin_TTedit )
				{
					GUI_edit_start( m_RMwin_TTedit,app_RMmod_SZTT_setting );
					SZ_XLreg_set_analysis();
					SZ_XLreg_read_display();
				}
				else if ( Gui_start_editOBJ_obtain() == m_RMwin_TMedit )
				{
					GUI_edit_start( m_RMwin_TMedit,app_RMmod_SZcode_setting );
					SZ_password_display();
				}
			}
			else
			{
				MsgBox(200,200,200,200,"��ʾ!","�������ڽ�����,����ֹͣ�����Ž�����ز���!",APP_HALF_SEC*10);
			}
		}*/
		else if ( *key_buf == GUI_KEY_SZ_TT_SET || *key_buf == KEY_F1)
		{
			if ( g_sz_test_start_flag == 0 )
			{
				GUI_edit_start( m_RMwin_TTedit,app_RMmod_SZTT_setting );
				SZ_XLreg_set_analysis();
				SZ_XLreg_read_display();
			}
			else
			{
				MsgBox(200,200,200,200,"��ʾ!","�������ڽ�����,����ֹͣ�����Ž�����ز���!",APP_HALF_SEC*10);
			}
		}
		else if ( *key_buf == GUI_KEY_SZ_TM_SET || *key_buf == KEY_SETTING)
		{
			if ( g_sz_test_start_flag == 0 )
			{
				GUI_edit_start( m_RMwin_TMedit,app_RMmod_SZcode_setting );
				SZ_password_display();
			}
			else
			{
				MsgBox(200,200,200,200,"��ʾ!","�������ڽ�����,����ֹͣ�����Ž�����ز���!",APP_HALF_SEC*10);
			}
		}
		else if ( *key_buf == GUI_KEY_SZ_TT_READ || *key_buf == KEY_F2)
		{
			if (MsgBox(200,200,200,200,"��ʾ!","��ȷ��ͨ��ͨѶ������ʹ�ñ�����,�����޷�����ܱ�ͨѶ,��<ȷ��>����",APP_HALF_SEC*10)
				== GUIE_KEY_ENTER )
			{
				g_sz_read_or_write = 0;
				g_sz_send_read_offset = 0;
				g_sz_send_read_flag = 1;
				g_SZ_rec_start_flag = 1;
				g_sz_read_error_flag = 0;
				GUI_3D_window_title_mod(m_RMwin_SZWIN,"���ڶ�ȡ��������Ĵ���");
			}
		}
		else if ( *key_buf == GUI_KEY_SZ_TM_XLCLEAR || *key_buf == KEY_F3)
		{
			if (MsgBox(200,200,200,200,"��ʾ!","DL/T 645-2007��Լ��ʹ���������������Ҫ����д��Ȩ�����벢�򿪱�̿���,��<ȷ��>����",APP_HALF_SEC*10)
				== GUIE_KEY_ENTER )
			{
				g_sz_read_or_write = 1;
				g_SZ_rec_start_flag = 1;
				g_sz_read_error_flag = 0;
				GUI_3D_window_title_mod(m_RMwin_SZWIN,"����������������ѷ���!");
			}
		}
		else if ( *key_buf == GUI_KEY_SZ_TM_START || *key_buf == KEY_F4)
		{
			if ( g_sz_test_start_flag == 0 )
			{
				if (MsgBox(200,200,200,200,"��ʾ!","��ȷ��ͨ��ͨѶ���ʹ򿪱�̿��غ��ٿ�ʼ����,�����޷�������������,��<ȷ��>��ʼ",APP_HALF_SEC*10)
					== GUIE_KEY_ENTER )
				{
					g_sz_test_start_flag = 1;
					if ( g_DL_param.phase_a.f_U < 30 )
					{
						GUI_3D_window_title_mod(m_RMwin_SZWIN,"��·�����޵�ѹ,�����ֳ��Ƿ��и����ٿ�ʼ����!");
						g_sz_test_start_flag = 0;
						continue;
					}
					g_sz_test_step_flag = 1;
					g_sz_testPt_offset = 0;
					g_sz_step1_start = 1;
					GLOBAL_MEMCLR(&g_result_buf, sizeof(sz_result_t));
					SZ_result_display();
				}
			}
			else MsgBox(200,200,200,200,"��ʾ!","��������������!",APP_HALF_SEC*10);
		}
		else if ( *key_buf == GUI_KEY_SZ_TM_STOP || *key_buf == KEY_F5)
		{
			sz_state_flag_init();
			cur_program_state("�����Ѿ�ֹͣ        ");
			cur_TP_result_handle();
		}
	}
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :����У������ʾֵ���У��
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_RM_verify_funtion4( void )
{
	uint8_t key_buf;
	int cnt = 60;
	_SZtask_id 
		= os_task_assign_stk_build( _SZtask,3);
	_SZRECtask_id
		= os_task_assign_stk_build( _SZ_rectask,5);
	
	app_RMmod_SZ_window_create();
	GLOBAL_MEMCLR(&g_TT_buf, sizeof(sz_TT_t));
	GLOBAL_MEMCLR(&g_result_buf, sizeof(sz_result_t));
	GLOBAL_MEMCLR(g_energy_hc_buf,sizeof(uint32_t)*15);
	GLOBAL_MEMCLR(g_energy_after_buf,sizeof(uint32_t)*10);
	SZ_password_display();
	global_memcpy(g_TT_buf.m_XL_period, "15", 2);
	global_memcpy(g_TT_buf.m_HC_time, "01", 2);
	global_memcpy(g_TT_buf.m_HC_num, "00", 2);
	SZ_XLreg_read_display();
	sz_state_flag_init();
	cur_program_state("����        ");
	while (1)
	{
		msleep( TYPE_DELAY );
		if( ++cnt%80==0 )
		{
			cnt= 0;
		    app_global_DLarg_obtain(&g_DL_param);
			app_RM_DLarg_display(&g_DL_param);
		}
		if ( app_global_key_obtain( &key_buf, 1 ) != 1 )  //��������ȡ����ֵ
		{
			continue;
		}
		if ( key_buf == KEY_EXIT || key_buf == GUI_KEY_EXIT)
		{
			app_RMmod_SZ_window_destory();
			if ( _SZtask_id != INVALID_PRO )
			{
				os_task_delete( _SZtask_id );
				_SZtask_id = INVALID_PRO;
			}
			if ( _SZRECtask_id != INVALID_PRO )
			{
				os_task_delete( _SZRECtask_id );
				_SZRECtask_id = INVALID_PRO;
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



/*********************************************************************************************
*            Öéº£¿ÆÜöµçÆ÷ÓĞÏŞ¹«Ë¾
*            http://www.zhkh.com
* Ä£¿éÃû³Æ:Ä£¿éÖ÷Ìå³ÌĞò
* ´´½¨Ê±¼ä:2013-04-26
* ËùÊô¹«Ë¾ :¿ÆÜö
* ÎÄ¼şÃû³Æ:app_VMmod_fun.c
* ×÷Õß :ÖÜ±şÈ¨
* ¹¦ÄÜÃèÊö:ÊµÏÖÆğÊ¼Ä£¿é¹¦ÄÜ
******************************************************************************************/
#include	"app_VM_inside.h"

static uint8_t 	 _task_pro 	  = INVALID_PRO;
static uint8_t 	 _task2_pro   = INVALID_PRO;

uint8_t   	m_VMoffset_guiyue=0,m_VMoffset_baud=0,m_VMoffset_crc=0;
char m_VMmeter_addr[13] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', 0};
char m_VMmeter07_addr[13] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', 0};
VMMeter_dl_info_t	VMmeter_data_buf = { 0, };

static    dev_id_t  s_usart3_dev_id 	= INVALID_DEV_ID;

/********************************************************************
 * @×÷Õß :ÖÜ±şÈ¨
 * @¹¦ÄÜ :ÉèÖÃÊı¾İ±£´æ
 * @ÊäÈë :NONE
 * @Êä³ö :NONE
********************************************************************/
void app_VM_97set_save( VMMeter_dl_info_t *m_VM_buf )
{
	app_VMmod_set_data_save(m_VM_buf,VM97_DATA_FILE);
	return;
}

void app_VM_07set_save( VMMeter_dl_info_t *m_VM_buf )
{
	app_VMmod_set_data_save(m_VM_buf,VM07_DATA_FILE);
	return;
}

/********************************************************************
 * @×÷Õß :ÖÜ±şÈ¨
 * @¹¦ÄÜ :ÉèÖÃÊı¾İ¶ÁÈ¡
 * @ÊäÈë :NONE
 * @Êä³ö :NONE
********************************************************************/
void app_VM_97set_read( VMMeter_dl_info_t *m_VM_buf )
{
	app_VMmod_set_data_read(m_VM_buf,VM97_DATA_FILE);
	return;
}

void app_VM_07set_read( VMMeter_dl_info_t *m_VM_buf )
{
	app_VMmod_set_data_read(m_VM_buf,VM07_DATA_FILE);
	return;
}

/********************************************************************
 * @×÷Õß :ÖÜ±şÈ¨
 * @¹¦ÄÜ :×Ö·ûĞÍ×ª»»hexĞÍÊı¾İ
 * @ÊäÈë :*strÊı¾İ 
 * @Êä³ö :NONE
********************************************************************/
int VMCharToHex(char * str)
{
	int value=0;
	if(*str>='A'&&*str<='F')
		value=*str-55;
	else
		value=*str-48;
	return value;
} 
	
/********************************************************************
 * @×÷Õß :ÖÜ±şÈ¨
 * @¹¦ÄÜ :·¢ËÍÊı¾İ
 * @ÊäÈë :*dataÊı¾İ  len³¤¶È
 * @Êä³ö :NONE
********************************************************************/
void app_VMmod_Usart3_send(void *data,Bsize_t len)
{
	os_driver_write( s_usart3_dev_id, data,len, 0 );
	return;
}
/********************************************************************
 * @×÷Õß :ÖÜ±şÈ¨
 * @¹¦ÄÜ :²¨ÌØÂÊÑ¡Ôñ
 * @ÊäÈë :
 * @Êä³ö :NONE
********************************************************************/
void app_VMmod_baud_select(uint8_t m_offset)
{
	uint32_t m_baud;	
	switch(m_offset)
	{
		case 0:
			m_baud=300;
			g_delay_baud_flag = 0;
			break;
		case 1:
			m_baud=600;
			g_delay_baud_flag = 1;
			break;
		case 2:
			m_baud=1200;
			g_delay_baud_flag = 2;
			break;
		case 3:
			m_baud=2400;
			g_delay_baud_flag = 3;
			break;
		case 4:
			m_baud=4800;
			g_delay_baud_flag = 4;
			break;	
		case 5:
			m_baud=9600;
			g_delay_baud_flag = 5;
			break;	
		case 6:
			m_baud=19200;
			g_delay_baud_flag = 6;
			break;	
		default:
			m_baud=300;
			g_delay_baud_flag = 0;
			break;
	}	
	os_driver_ioctl(s_usart3_dev_id, SET_SERIAL_BAUD, &m_baud);
	return;
}
/********************************************************************
 * @×÷Õß :ÖÜ±şÈ¨
 * @¹¦ÄÜ :Ğ£Ñé·½Ê½Ñ¡Ôñ
 * @ÊäÈë :
 * @Êä³ö :NONE
********************************************************************/
void app_VMmod_CRC_select(uint8_t m_offset)
{
	uint32_t m_CRC;	
	switch(m_offset)
	{
		case 0:
			m_CRC=0;  //ÎŞĞ£Ñé
			break;
		case 1:
			m_CRC=1;  //ÆæĞ£Ñé
			break;
		case 2:		
			m_CRC=2;  //Å¼Ğ£Ñé
			break;
		default:
			m_CRC=0;  //ÎŞĞ£Ñé
			break;
	}

	os_driver_ioctl(s_usart3_dev_id, SET_SERIAL_CHECK, &m_CRC);
	return;
}

/********************************************************************
 * @×÷Õß :
 * @¹¦ÄÜ :ÉèÖÃ³ÉÔ±ÇĞ»»
 * @ÊäÈë :cur_member µ±Ç°³ÉÔ±£¬KEY °´¼üÖµ
 * @Êä³ö :NONE
********************************************************************/
void app_VMmod_edit_switch( uint8_t key )
{
	Bsize_t member;
	member = GUI_edit_id_obtain();		
	if ( key == KEY_UP )
	{
		if(member==EDIT_M_ADDR)
			GUI_edit_select(EDIT_SECOND);
		else if(member==EDIT_SECOND)
			GUI_edit_select(EDIT_MINUTE);
		else if(member==EDIT_MINUTE)
			GUI_edit_select(EDIT_HOUR);
		else if(member==EDIT_HOUR)
			GUI_edit_select(EDIT_DAY);
		else if(member==EDIT_DAY)
			GUI_edit_select(EDIT_MONTH);
		else if(member==EDIT_MONTH)
			GUI_edit_select(EDIT_YEAR);
		else if(member==EDIT_YEAR)
			GUI_edit_select(EDIT_C_I);
		else if(member==EDIT_C_I)
			GUI_edit_select(EDIT_B_I);
		else if(member==EDIT_B_I)
			GUI_edit_select(EDIT_A_I);
		else if(member==EDIT_A_I)
			GUI_edit_select(EDIT_C_U);
		else if(member==EDIT_C_U)
			GUI_edit_select(EDIT_B_U);
		else if(member==EDIT_B_U)
			GUI_edit_select(EDIT_A_U);
		else if(member==EDIT_A_U)
			GUI_edit_select(EDIT_G_4);
		else if(member==EDIT_G_4)
			GUI_edit_select(EDIT_P_4);
		else if(member==EDIT_P_4)
			GUI_edit_select(EDIT_F_4);
		else if(member==EDIT_F_4)
			GUI_edit_select(EDIT_J_4);
		else if(member==EDIT_J_4)
			GUI_edit_select(EDIT_Z_4);
		else if(member==EDIT_Z_4)
			GUI_edit_select(EDIT_G_3);
		else if(member==EDIT_G_3)
			GUI_edit_select(EDIT_P_3);
		else if(member==EDIT_P_3)
			GUI_edit_select(EDIT_F_3);
		else if(member==EDIT_F_3)
			GUI_edit_select(EDIT_J_3);
		else if(member==EDIT_J_3)
			GUI_edit_select(EDIT_Z_3);
		else if(member==EDIT_Z_3)
			GUI_edit_select(EDIT_G_2);
		else if(member==EDIT_G_2)
			GUI_edit_select(EDIT_P_2);
		else if(member==EDIT_P_2)
			GUI_edit_select(EDIT_F_2);
		else if(member==EDIT_F_2)
			GUI_edit_select(EDIT_J_2);
		else if(member==EDIT_J_2)
			GUI_edit_select(EDIT_Z_2);
		else if(member==EDIT_Z_2)
			GUI_edit_select(EDIT_G_1);
		else if(member==EDIT_G_1)
			GUI_edit_select(EDIT_P_1);
		else if(member==EDIT_P_1)
			GUI_edit_select(EDIT_F_1);
		else if(member==EDIT_F_1)
			GUI_edit_select(EDIT_J_1);
		else if(member==EDIT_J_1)
			GUI_edit_select(EDIT_Z_1);
		else if(member==EDIT_Z_1)
			GUI_edit_select(EDIT_M_ADDR);
	}
	else if ( key == KEY_DOWN )
	{
		if(member==EDIT_M_ADDR)
			GUI_edit_select(EDIT_Z_1);
		else if(member==EDIT_Z_1)
			GUI_edit_select(EDIT_J_1);
		else if(member==EDIT_J_1)
			GUI_edit_select(EDIT_F_1);
		else if(member==EDIT_F_1)
			GUI_edit_select(EDIT_P_1);
		else if(member==EDIT_P_1)
			GUI_edit_select(EDIT_G_1);
		else if(member==EDIT_G_1)
			GUI_edit_select(EDIT_Z_2);
		else if(member==EDIT_Z_2)
			GUI_edit_select(EDIT_J_2);
		else if(member==EDIT_J_2)
			GUI_edit_select(EDIT_F_2);
		else if(member==EDIT_F_2)
			GUI_edit_select(EDIT_P_2);
		else if(member==EDIT_P_2)
			GUI_edit_select(EDIT_G_2);
		else if(member==EDIT_G_2)
			GUI_edit_select(EDIT_Z_3);
		else if(member==EDIT_Z_3)
			GUI_edit_select(EDIT_J_3);
		else if(member==EDIT_J_3)
			GUI_edit_select(EDIT_F_3);
		else if(member==EDIT_F_3)
			GUI_edit_select(EDIT_P_3);
		else if(member==EDIT_P_3)
			GUI_edit_select(EDIT_G_3);
		else if(member==EDIT_G_3)
			GUI_edit_select(EDIT_Z_4);
		else if(member==EDIT_Z_4)
			GUI_edit_select(EDIT_J_4);
		else if(member==EDIT_J_4)
			GUI_edit_select(EDIT_F_4);
		else if(member==EDIT_F_4)
			GUI_edit_select(EDIT_P_4);
		else if(member==EDIT_P_4)
			GUI_edit_select(EDIT_G_4);
		else if(member==EDIT_G_4)
			GUI_edit_select(EDIT_A_U);
		else if(member==EDIT_A_U)
			GUI_edit_select(EDIT_B_U);
		else if(member==EDIT_B_U)
			GUI_edit_select(EDIT_C_U);
		else if(member==EDIT_C_U)
			GUI_edit_select(EDIT_A_I);
		else if(member==EDIT_A_I)
			GUI_edit_select(EDIT_B_I);
		else if(member==EDIT_B_I)
			GUI_edit_select(EDIT_C_I);
		else if(member==EDIT_C_I)
			GUI_edit_select(EDIT_YEAR);
		else if(member==EDIT_YEAR)
			GUI_edit_select(EDIT_MONTH);
		else if(member==EDIT_MONTH)
			GUI_edit_select(EDIT_DAY);
		else if(member==EDIT_DAY)
			GUI_edit_select(EDIT_HOUR);
		else if(member==EDIT_HOUR)
			GUI_edit_select(EDIT_MINUTE);
		else if(member==EDIT_MINUTE)
			GUI_edit_select(EDIT_SECOND);
		else if(member==EDIT_SECOND)
			GUI_edit_select(EDIT_M_ADDR);
	}
	return;
}

/********************************************************************
 * @×÷Õß :ÖÜ±şÈ¨
 * @¹¦ÄÜ :ÉèÖÃÊäÈë´¦Àí
 * @ÊäÈë :cur_member µ±Ç°³ÉÔ±£¬KEY °´¼üÖµ
 * @Êä³ö  :NONE
********************************************************************/
void app_VMmod_edit_input_handle( uint8_t m_key )
{
	Bsize_t m_member;
	m_member = GUI_edit_id_obtain();     /*»ñÈ¡µ±Ç°±à¼­³ÉÔ±ID*/
	if ( m_member == EDIT_M_ADDR)
	{
		if ( m_VMoffset_guiyue == 1 )
		key_method_input_shadow( m_key, m_VMmeter07_addr);
		else key_method_input_shadow( m_key, m_VMmeter_addr);
	}
	else if ( m_member == EDIT_Z_1 )
	{
		key_method_input_shadow( m_key, VMmeter_data_buf.zy_power_ALL);
	}
	else if ( m_member == EDIT_J_1 )
	{
		key_method_input_shadow( m_key, VMmeter_data_buf.zy_power_J);
	}
	else if ( m_member == EDIT_F_1 )
	{
		key_method_input_shadow( m_key, VMmeter_data_buf.zy_power_F);
	}
	else if ( m_member == EDIT_P_1 )
	{
		key_method_input_shadow( m_key, VMmeter_data_buf.zy_power_P);
	}
	else if ( m_member == EDIT_G_1 )
	{
		key_method_input_shadow( m_key, VMmeter_data_buf.zy_power_G);
	}

	else if ( m_member == EDIT_Z_2 )
	{
		key_method_input_shadow( m_key, VMmeter_data_buf.zw_power_ALL);
	}
	else if ( m_member == EDIT_J_2 )
	{
		key_method_input_shadow( m_key, VMmeter_data_buf.zw_power_J);
	}
	else if ( m_member == EDIT_F_2 )
	{
		key_method_input_shadow( m_key, VMmeter_data_buf.zw_power_F);
	}
	else if ( m_member == EDIT_P_2 )
	{
		key_method_input_shadow( m_key, VMmeter_data_buf.zw_power_P);
	}
	else if ( m_member == EDIT_G_2 )
	{
		key_method_input_shadow( m_key, VMmeter_data_buf.zw_power_G);
	}

	else if ( m_member == EDIT_Z_3 )
	{
		key_method_input_shadow( m_key, VMmeter_data_buf.fy_power_ALL);
	}
	else if ( m_member == EDIT_J_3 )
	{
		key_method_input_shadow( m_key, VMmeter_data_buf.fy_power_J);
	}
	else if ( m_member == EDIT_F_3 )
	{
		key_method_input_shadow( m_key, VMmeter_data_buf.fy_power_F);
	}
	else if ( m_member == EDIT_P_3 )
	{
		key_method_input_shadow( m_key, VMmeter_data_buf.fy_power_P);
	}
	else if ( m_member == EDIT_G_3 )
	{
		key_method_input_shadow( m_key, VMmeter_data_buf.fy_power_G);
	}

	else if ( m_member == EDIT_Z_4 )
	{
		key_method_input_shadow( m_key, VMmeter_data_buf.fw_power_ALL);
	}
	else if ( m_member == EDIT_J_4 )
	{
		key_method_input_shadow( m_key, VMmeter_data_buf.fw_power_J);
	}
	else if ( m_member == EDIT_F_4 )
	{
		key_method_input_shadow( m_key, VMmeter_data_buf.fw_power_F);
	}
	else if ( m_member == EDIT_P_4 )
	{
		key_method_input_shadow( m_key, VMmeter_data_buf.fw_power_P);
	}
	else if ( m_member == EDIT_G_4 )
	{
		key_method_input_shadow( m_key, VMmeter_data_buf.fw_power_G);
	}

	else if ( m_member == EDIT_A_U )
	{
		key_method_input_shadow( m_key, VMmeter_data_buf.voltage_A);
	}
	else if ( m_member == EDIT_B_U )
	{
		key_method_input_shadow( m_key, VMmeter_data_buf.voltage_B);
	}
	else if ( m_member == EDIT_C_U )
	{
		key_method_input_shadow( m_key, VMmeter_data_buf.voltage_C);
	}
	else if ( m_member == EDIT_A_I )
	{
		key_method_input_shadow( m_key, VMmeter_data_buf.current_A);
	}
	else if ( m_member == EDIT_B_I )
	{
		key_method_input_shadow( m_key, VMmeter_data_buf.current_B);
	}
	else if ( m_member == EDIT_C_I )
	{
		key_method_input_shadow( m_key, VMmeter_data_buf.current_C);
	}
	else if ( m_member == EDIT_YEAR)
	{
		key_method_input_shadow( m_key, VMmeter_data_buf.meter_time_year);
	}
	else if ( m_member == EDIT_MONTH)
	{
		key_method_input_shadow( m_key, VMmeter_data_buf.meter_time_month);
	}
	else if ( m_member == EDIT_DAY)
	{
		key_method_input_shadow( m_key, VMmeter_data_buf.meter_time_day);
	}
	else if ( m_member == EDIT_HOUR)
	{
		key_method_input_shadow( m_key, VMmeter_data_buf.meter_time_hour);
	}
	else if ( m_member == EDIT_MINUTE)
	{
		key_method_input_shadow( m_key, VMmeter_data_buf.meter_time_minute);
	}
	else if ( m_member == EDIT_SECOND)
	{
		key_method_input_shadow( m_key, VMmeter_data_buf.meter_time_second);
	}
	return;
}

/********************************************************************
 * @×÷Õß :ÖÜ±şÈ¨
 * @¹¦ÄÜ :Ä£¿éÖ÷ÉèÖÃ
 * @ÊäÈë :NONE
 * @Êä³ö :NONE
********************************************************************/
uint8_t app_VMmod_edit_setting( hwin_t*hp )
{
	uint8_t key;
	Bsize_t member;
	hand_wirte_input_box_pos_set(346,10);
	app_global_key_control_power_req();
	GUI_window_hwin_req( hp );
	key_input_method_assign( KEY_INPUT_123 );
	key_input_method_filter( KEY_INPUT_123 );
	GUI_window_hwin_release( hp );
	while ( 1 )
	{
		msleep( TYPE_DELAY );
		if ( app_global_key_force_obtain( &key, 1 ) != 1 )/*ÅĞ¶ÏÇı¶¯ÊÇ·ñÊÕµ½°´¼üÏìÓ¦*/
		{
			continue;
		}
		GUI_window_hwin_req( hp );
		GUI_edit_run_hook( key );
		GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
		GUI_set_Fgcolor( VM_FG_COLOR );
		if (( app_global_decide_inputkey( key ) ) != FAIL )/*ÅĞ¶ÏÊÇ·ñÊÇÊäÈë·¨Ïà¹Ø°´¼ü°´ÏÂ*/
		{
			app_VMmod_edit_input_handle(key);
		}
		else if ( key == KEY_F3 )
		{
			member = GUI_edit_id_obtain();
			if ( member == EDIT_M_ADDR )
			{
				if ( m_VMoffset_guiyue == 1 ) GLOBAL_MEMCLR(m_VMmeter07_addr,13 ); 
				else GLOBAL_MEMCLR(m_VMmeter_addr,13 ); 
			}
		}
		else if ( key == KEY_DOWN || key == KEY_UP )      
		{
			app_VMmod_edit_switch( key );
		}
		if ( key == KEY_ENTER || key == GUI_KEY_ENTER )
		{
			if ( m_VMoffset_guiyue == 1 )
				 global_memcpy(VMmeter_data_buf.meter_Addr,m_VMmeter07_addr,13);
			else global_memcpy(VMmeter_data_buf.meter_Addr,m_VMmeter_addr,13);
			app_VMmod_meter_addr_display(VMmeter_data_buf.meter_Addr);
			break;
		}
		else if ( key == KEY_EXIT )
		{
			if ( m_VMoffset_guiyue == 1 )
				 global_memcpy(VMmeter_data_buf.meter_Addr,m_VMmeter07_addr,13);
			else global_memcpy(VMmeter_data_buf.meter_Addr,m_VMmeter_addr,13);
			app_VMmod_meter_addr_display(VMmeter_data_buf.meter_Addr);
			break;
		}
		GUI_window_hwin_release( hp );
	}
	GUI_window_hwin_release( hp );
	app_global_key_control_power_release();
	return key;
}

/********************************************************************
 * @×÷Õß :ÖÜ±şÈ¨
 * @¹¦ÄÜ :Ä£¿éÖ÷¹¦ÄÜÈÎÎñ
 * @ÊäÈë :NONE
 * @Êä³ö :NONE
********************************************************************/
static void _task2( void*arg )
{
	uint8_t m_rec_buf[60] = { 0, };
	uint8_t m_data_size = 0;
	uint8_t m_rec_crc = 0;
	m_rec_crc = m_rec_crc;
	while(1)
	{
		msleep(TYPE_DELAY);
		if ( os_driver_read( s_usart3_dev_id, m_rec_buf,1, 0 ) != 1 )
			continue;
		if ( m_rec_buf[0] != 0x68 )    continue;
		if ( VMmeter_data_buf.baud == 0 )		msleep(90);	//300
		else if ( VMmeter_data_buf.baud == 1 )	msleep(45);	//600
		else if ( VMmeter_data_buf.baud == 2 )	msleep(22);	//1200
		else if ( VMmeter_data_buf.baud == 3 )	msleep(16);	//2400
		else if ( VMmeter_data_buf.baud == 4 )	msleep(10);	//4800
		else if ( VMmeter_data_buf.baud == 5 )	msleep(6);	//9600
		else if ( VMmeter_data_buf.baud == 6 )	msleep(4);	//19200
		if ( os_driver_read( s_usart3_dev_id, &m_rec_buf[1],7, 0 ) != 7 )
			continue;
		if ( m_rec_buf[7] != 0x68 )	continue;
		if ( app_VMmod_rec_addr_judge(&m_rec_buf[1]) != 1 ) continue;
		if ( os_driver_read( s_usart3_dev_id, &m_rec_buf[8],2, 0 ) != 2 )
			continue;
		m_data_size = m_rec_buf[9];
		if ( os_driver_read( s_usart3_dev_id, &m_rec_buf[10],m_data_size, 0 ) != m_data_size )
			continue;
		if ( os_driver_read( s_usart3_dev_id, &m_rec_buf[10+m_data_size],2, 0 ) != 2 )
			continue;
		if ( m_rec_buf[10+m_data_size+1] != 0x16 )	 continue;
		//m_rec_crc = global_sum_array((char*)m_rec_buf, (10+m_data_size),IS_CHAR);
		//m_rec_crc &= 0xff;
		//if ( m_rec_crc != m_rec_buf[10+m_data_size]) continue;
		
		if (app_VMmod_rec_code_flag_judge(&m_rec_buf[8]) != 1 )
			continue;
		app_VMmod_send_VMdata();
	}
}
/********************************************************************
 * @×÷Õß :ÖÜ±şÈ¨
 * @¹¦ÄÜ :¹æÔ¼ÉèÖÃ²Ù×÷
 * @ÊäÈë :NONE
 * @Êä³ö :NONE
********************************************************************/
void app_VM_guiyue_set( void )
{
	if ( m_VMoffset_guiyue == 0 )
		app_VM_97set_read( &VMmeter_data_buf );
	else if ( m_VMoffset_guiyue == 1 )
		app_VM_07set_read( &VMmeter_data_buf );
	VMmeter_data_buf.guiyue = m_VMoffset_guiyue;
	app_VMmod_main_edit_display(&VMmeter_data_buf);
	m_VMoffset_baud = VMmeter_data_buf.baud;
	m_VMoffset_crc  = VMmeter_data_buf.crc;
	app_VMmod_CRC_select(m_VMoffset_crc);		//ÉèÖÃĞ£Ñé·½Ê½
	app_VMmod_baud_select(m_VMoffset_baud); 	//ÉèÖÃ²¨ÌØÂÊ
	return;
}

/********************************************************************
 * @×÷Õß :ÖÜ±şÈ¨
 * @¹¦ÄÜ :²¨ÌØÂÊÉèÖÃ²Ù×÷
 * @ÊäÈë :NONE
 * @Êä³ö :NONE
********************************************************************/
void app_VM_baud_set( void )
{
	VMmeter_data_buf.baud = m_VMoffset_baud;
	app_VMmod_baud_select(m_VMoffset_baud); 	//ÉèÖÃ²¨ÌØÂÊ
	return;
}
/********************************************************************
 * @×÷Õß :ÖÜ±şÈ¨
 * @¹¦ÄÜ :Ğ£Ñé·½Ê½ÉèÖÃ²Ù×÷
 * @ÊäÈë :NONE
 * @Êä³ö :NONE
********************************************************************/
void app_VM_crc_set( void )
{
	VMmeter_data_buf.crc = m_VMoffset_crc;
	app_VMmod_CRC_select(m_VMoffset_crc);		//ÉèÖÃĞ£Ñé·½Ê½
	return;
}

/********************************************************************
 * @×÷Õß :Ü±şÈ¨
 * @¹¦ÄÜ :Ä£¿éÖ÷ÈÎÎñ
 * @ÊäÈë :NONE
 * @ä³ö :NONE
********************************************************************/
//void GUI_touch_correct( void );
static void _task1( void*arg )
{
	uint8_t key_buf;
	const char *m_tip1[2] 		= {"ÕıÔÚ±£´æ²ÎÊı...",""};
	const char *m_tip2[2] 		= {"²ÎÊı±£´æÍê³É!",""};
	uint8_t m_guiyue_bak = 0;
	//GUI_touch_correct();
	app_VMmod_window_create();
	app_VM_97set_read( &VMmeter_data_buf );
	app_VMmod_set_init_display();
	GUI_WINready(m_VM_droplist_guiyue,m_VMoffset_guiyue);
	m_guiyue_bak = m_VMoffset_guiyue;
	while(1)
	{
		msleep(TYPE_DELAY);
		
		if ( app_global_key_obtain( &key_buf, 1 ) != 1 )
		continue;
		if( key_buf == KEY_EXIT)
		{
			//app_module_assignExt("START");
			asm( "  lb 0x300000" );
		}
		else if( key_buf == KEY_SETTING || key_buf == GUI_KEY_SETTING || key_buf == GUI_KEY_EDIT_START )
		{
			GUI_edit_start( m_VMwin_edit,app_VMmod_edit_setting );
			app_VMmod_main_edit_display(&VMmeter_data_buf);
		}
		else if( key_buf == KEY_SAVE || key_buf == GUI_KEY_SAVE )
		{
			GUI_3D_window_title_mod(m_VMwin_pWIN,(char *)m_tip1[g_language]);
			VMmeter_data_buf.m_crc_flag = 0x5a;
			if ( VMmeter_data_buf.guiyue == 0 )
				app_VM_97set_save( &VMmeter_data_buf );
			else if ( VMmeter_data_buf.guiyue == 1 )
				app_VM_07set_save( &VMmeter_data_buf );
			GUI_3D_window_title_mod(m_VMwin_pWIN,(char *)m_tip2[g_language]);
		}
		else if( key_buf == GUI_KEY_GUIYUE_SELECT)
		{	
			m_VMoffset_guiyue = GUI_drop_list_unfold(m_VM_droplist_guiyue);
			if ( m_guiyue_bak != m_VMoffset_guiyue )
			{
				m_guiyue_bak = m_VMoffset_guiyue;
				app_VM_guiyue_set();
			}
		}
		else if( key_buf == GUI_KEY_BAUD_SELECT)
		{	
			m_VMoffset_baud = GUI_drop_list_unfold(m_VM_droplist_baud);
			app_VM_baud_set();
		}
		else if( key_buf == GUI_KEY_CRC_SELECT )
		{	
			m_VMoffset_crc = GUI_drop_list_unfold(m_VM_droplist_crc);
			app_VM_crc_set();
		}
		else if ( key_buf==KEY_RIGHT )
		{
			if(m_CURwidget != m_VM_droplist_crc
				|| m_pMember != m_CURwidget->subord_end )
		  	GUI_WINshift(0);
		}
		else if ( key_buf==KEY_LEFT )
		{
			if(m_CURwidget!= m_VM_droplist_guiyue
					||m_pMember!=m_CURwidget->subord_head)
		  	GUI_WINshift(1);
		}			 
		else if ( key_buf==KEY_ENTER )
		{
			GUI_WINdo();
			m_VMoffset_baud = GUI_drop_list_member_obtain(m_VM_droplist_baud);
			app_VM_baud_set();
			m_VMoffset_crc = GUI_drop_list_member_obtain(m_VM_droplist_crc);
			app_VM_crc_set();
			m_VMoffset_guiyue = GUI_drop_list_member_obtain(m_VM_droplist_guiyue);
			if ( m_guiyue_bak != m_VMoffset_guiyue )
			{
				m_guiyue_bak = m_VMoffset_guiyue;
				app_VM_guiyue_set();
			}
		}
	}
}
/********************************************************************
 * @×÷Õß :ÖÜ±şÈ¨
 * @¹¦ÄÜ :Ö÷Ä£¿éÖ÷¹¦ÄÜ³õÊ¼»¯
 * @ÊäÈë :NONE
 * @Êä³ö :NONE
********************************************************************/
void app_VMmod_fun_entry( void )
{
	_task_pro
	= os_task_assign_stk_build( _task1, 3);
	_task2_pro
	= os_task_assign_stk_build( _task2, 4);
	GLOBAL_MEMCLR(&VMmeter_data_buf, sizeof(VMMeter_dl_info_t));
	s_usart3_dev_id = os_driver_open( "D232" );
	return;
}
/********************************************************************
 * @×÷Õß :ÖÜ±şÈ¨
 * @¹¦ÄÜ :Ö÷Ä£¿éÖ÷¹¦ÄÜÊÍ·Å×ÊÔ´
 * @ÊäÈë :NONE
 * @Êä³ö :NONE
********************************************************************/
void app_VMmod_fun_exit( void )
{
	app_VMmod_window_destory();
	if ( _task_pro != INVALID_PRO )
	{
		os_task_delete( _task_pro );
		_task_pro = INVALID_PRO;
	}
	if ( _task2_pro != INVALID_PRO )
	{
		os_task_delete( _task2_pro );
		_task2_pro = INVALID_PRO;
	}
	os_driver_close(s_usart3_dev_id);
	return;
}


/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:ģ���������
* ����ʱ��:2013-05-17
* ������˾ :����
* �ļ�����:app_VM_protocol_analysis.c
* ���� :�ܱ�Ȩ
* ��������:ʵ��������Э����������ݴ�����
******************************************************************************************/
#include	"app_VM_inside.h"


uint8_t g_send_BZ_size = 0;
uint8_t g_send_data_BZ[4] = { 0, };
uint8_t g_send_data_size = 0;
uint8_t g_send_data_buf[30] = { 0, };

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :���յ�ַ���ж�
 * @���� :NONE
 * @��� :������������1�����򷵻�0
********************************************************************/
uint8_t app_VMmod_rec_addr_judge( uint8_t *m_buf )
{
	uint8_t m_rt = 0;
	uint8_t m_temp[6] = { 0, };
	uint8_t m_wildcard_c = 0;
	
	if ( VMmeter_data_buf.guiyue == 0 )	m_wildcard_c = 0x99;
	else if (  VMmeter_data_buf.guiyue == 1 )	m_wildcard_c = 0xAA;
	if ( (m_buf[0] == m_wildcard_c) && (m_buf[1] == m_wildcard_c) &&
		(m_buf[2] == m_wildcard_c) && (m_buf[3] == m_wildcard_c) &&
		(m_buf[4] == m_wildcard_c) && (m_buf[5] == m_wildcard_c) )
	{
		m_rt = 1;
		return m_rt;
	}
	else
	{
		m_temp[0] = (( VMCharToHex(&VMmeter_data_buf.meter_Addr[10]) & 0x0F ) << 4 ) + 
					( VMCharToHex(&VMmeter_data_buf.meter_Addr[11]) & 0x0F );
		m_temp[1] = (( VMCharToHex(&VMmeter_data_buf.meter_Addr[8]) & 0x0F ) << 4 ) + 
					(VMCharToHex(& VMmeter_data_buf.meter_Addr[9]) & 0x0F );
		m_temp[2] = (( VMCharToHex(&VMmeter_data_buf.meter_Addr[6]) & 0x0F ) << 4 ) + 
					( VMCharToHex(&VMmeter_data_buf.meter_Addr[7]) & 0x0F );
		m_temp[3] = (( VMCharToHex(&VMmeter_data_buf.meter_Addr[4]) & 0x0F ) << 4 ) + 
					( VMCharToHex(&VMmeter_data_buf.meter_Addr[5]) & 0x0F );
		m_temp[4] = (( VMCharToHex(&VMmeter_data_buf.meter_Addr[2]) & 0x0F ) << 4 ) + 
					( VMCharToHex(&VMmeter_data_buf.meter_Addr[3]) & 0x0F );
		m_temp[5] = (( VMCharToHex(&VMmeter_data_buf.meter_Addr[0]) & 0x0F ) << 4 ) + 
					( VMCharToHex(&VMmeter_data_buf.meter_Addr[1]) & 0x0F );
		if ( (m_buf[0] == m_temp[0]) && (m_buf[1] == m_temp[1]) &&
			(m_buf[2] == m_temp[2]) && (m_buf[3] == m_temp[3]) &&
			(m_buf[4] == m_temp[4]) && (m_buf[5] == m_temp[5]) )
		{
			m_rt = 1;
			return m_rt;
		}
	}
	return m_rt;
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :���͵����ݴ���
 * @���� :m_ftemp:��������,m_moffset:���ͻ���ƫ��;
 *  		m_pt:С����־,1,2Ϊ��С��,0����;
 * @��� :NONE
********************************************************************/
void app_VMmod_send_4fdata_handle( float m_ftemp, uint8_t m_moffset )
{
	unsigned long l_temp = 0;
	unsigned int  i_temp = 0;
	uint8_t u8_temp = 0;
	uint8_t ngt_flag = 0;
	
	if ( m_ftemp < 0 )
	{
		m_ftemp = GLOBAL_ABS(m_ftemp);
		ngt_flag = 1;
	}
	l_temp = (unsigned long)m_ftemp;
	i_temp = (unsigned int)((m_ftemp - l_temp)*100.0);
	hex_convert_bcd(i_temp, &u8_temp);
	g_send_data_buf[m_moffset] = u8_temp;
	hex_convert_bcd((uint8_t)(l_temp % 100), &u8_temp);
	g_send_data_buf[m_moffset+1] = u8_temp;
	hex_convert_bcd((uint8_t)(((l_temp % 10000) / 100)), &u8_temp);
	g_send_data_buf[m_moffset+2] = u8_temp;
	hex_convert_bcd((uint8_t)(l_temp / 10000), &u8_temp);
	g_send_data_buf[m_moffset+3] = u8_temp;
	if ( ngt_flag == 1 )	g_send_data_buf[m_moffset+3] |= 0x80;
	g_send_data_size += 4;
	return;
}

void app_VMmod_send_2fdata_handle( float m_ftemp, uint8_t m_moffset,uint8_t m_pt )
{
	unsigned long l_temp = 0;
	unsigned int  i_temp = 0;
	uint8_t u8_temp = 0;
	uint8_t u8_temp2 = 0;
	uint8_t ngt_flag = 0;
	
	if ( m_ftemp < 0 )
	{
		m_ftemp = GLOBAL_ABS(m_ftemp);
		ngt_flag = 1;
	}
	l_temp = (unsigned long)m_ftemp;
	i_temp = (unsigned int)((m_ftemp - l_temp)*100.0);
	if ( m_pt == 0 )
	{
		hex_convert_bcd((uint8_t)(l_temp % 100), &u8_temp);
		g_send_data_buf[m_moffset] = u8_temp;
		hex_convert_bcd((uint8_t)(l_temp / 100), &u8_temp);
		g_send_data_buf[m_moffset+1] = u8_temp;
		g_send_data_size += 2;
	}
	else if ( m_pt == 1 )//2λС��
	{
		hex_convert_bcd(i_temp, &u8_temp);
		g_send_data_buf[m_moffset] = u8_temp;
		hex_convert_bcd((uint8_t)(l_temp % 100), &u8_temp);
		g_send_data_buf[m_moffset+1] = u8_temp;
		g_send_data_size += 2;
	}
	else if ( m_pt == 2 )// 1λС��
	{
		i_temp = (uint8_t)(i_temp / 10) & 0x0F;
		u8_temp2 = ((uint8_t)(l_temp % 10) << 4) + i_temp;
		g_send_data_buf[m_moffset] = u8_temp2;
		hex_convert_bcd((uint8_t)(l_temp / 10), &u8_temp);
		g_send_data_buf[m_moffset+1] = u8_temp;
		g_send_data_size += 2;
	}
	if ( ngt_flag == 1 )	g_send_data_buf[m_moffset+1] |= 0x80;
	return;
}

void app_VMmod_send_3fdata_handle( float m_ftemp, uint8_t m_moffset,uint8_t m_pt )
{
	unsigned long l_temp = 0;
	unsigned int  i_temp = 0;
	uint8_t u8_temp = 0;
	uint8_t u8_temp2 = 0;
	uint8_t ngt_flag = 0;
	
	if ( m_ftemp < 0 )
	{
		m_ftemp = GLOBAL_ABS(m_ftemp);
		ngt_flag = 1;
	}
	l_temp = (unsigned long)m_ftemp;
	i_temp = (unsigned int)((m_ftemp - l_temp)*100.0);
	if ( m_pt == 0 )
	{
		hex_convert_bcd((uint8_t)(l_temp % 100), &u8_temp);
		g_send_data_buf[m_moffset] = u8_temp;
		hex_convert_bcd((uint8_t)(((l_temp % 10000) / 100)), &u8_temp);
		g_send_data_buf[m_moffset+1] = u8_temp;
		hex_convert_bcd((uint8_t)(l_temp  / 10000), &u8_temp);
		g_send_data_buf[m_moffset+2] = u8_temp;
		g_send_data_size += 3;
	}
	else if ( m_pt == 1 )//2λС��
	{
		hex_convert_bcd(i_temp, &u8_temp);
		g_send_data_buf[m_moffset] = u8_temp;
		hex_convert_bcd((uint8_t)(l_temp % 100), &u8_temp);
		g_send_data_buf[m_moffset+1] = u8_temp;
		hex_convert_bcd((uint8_t)(((l_temp % 10000) / 100)), &u8_temp);
		g_send_data_buf[m_moffset+2] = u8_temp;
		g_send_data_size += 3;
	}
	else if ( m_pt == 2 )//3λС��
	{	
		i_temp = (unsigned int)((m_ftemp - l_temp)*1000.0);
		u8_temp2 = (uint8_t)(i_temp % 100);
		hex_convert_bcd(u8_temp2, &u8_temp);
		g_send_data_buf[m_moffset] = u8_temp;
		u8_temp2 = (uint8_t)(i_temp / 100);
		i_temp = (unsigned int)(l_temp % 10);
		u8_temp2 = ((uint8_t)(i_temp << 4) & 0xF0) + (u8_temp2 & 0x0F);
		g_send_data_buf[m_moffset+1] = u8_temp2;
		hex_convert_bcd((uint8_t)((l_temp / 10)), &u8_temp);
		g_send_data_buf[m_moffset+2] = u8_temp;
		g_send_data_size += 3;
	}
	if ( ngt_flag == 1 )	g_send_data_buf[m_moffset+2] |= 0x80;
	return;
}

void app_VMmod_send_date_handle( void )
{
	uint8_t m_temp1 = 0;
	uint8_t m_temp2 = 0;
	
	m_temp1 = global_CtoN(VMmeter_data_buf.meter_time_day[0]);
	m_temp2 = global_CtoN(VMmeter_data_buf.meter_time_day[1]);
	
	g_send_data_buf[0] = 0;	//�ܴ�
	g_send_data_buf[1] = (uint8_t)(((m_temp1 << 4) & 0xF0) + (m_temp2 & 0x0F));
	m_temp1 = global_CtoN(VMmeter_data_buf.meter_time_month[0]);
	m_temp2 = global_CtoN(VMmeter_data_buf.meter_time_month[1]);
	g_send_data_buf[2] = (uint8_t)(((m_temp1 << 4) & 0xF0) + (m_temp2 & 0x0F));
	m_temp1 = global_CtoN(VMmeter_data_buf.meter_time_year[2]);
	m_temp2 = global_CtoN(VMmeter_data_buf.meter_time_year[3]);
	g_send_data_buf[3] = (uint8_t)(((m_temp1 << 4) & 0xF0) + (m_temp2 & 0x0F));
	g_send_data_size += 4;
	return;
}

void app_VMmod_send_time_handle( void )
{
	uint8_t m_temp1 = 0;
	uint8_t m_temp2 = 0;
	
	m_temp1 = global_CtoN(VMmeter_data_buf.meter_time_second[0]);
	m_temp2 = global_CtoN(VMmeter_data_buf.meter_time_second[1]);
	
	g_send_data_buf[0] = (uint8_t)(((m_temp1 << 4) & 0xF0) + (m_temp2 & 0x0F));
	m_temp1 = global_CtoN(VMmeter_data_buf.meter_time_minute[0]);
	m_temp2 = global_CtoN(VMmeter_data_buf.meter_time_minute[1]);
	g_send_data_buf[1] = (uint8_t)(((m_temp1 << 4) & 0xF0) + (m_temp2 & 0x0F));
	m_temp1 = global_CtoN(VMmeter_data_buf.meter_time_hour[0]);
	m_temp2 = global_CtoN(VMmeter_data_buf.meter_time_hour[1]);
	g_send_data_buf[2] = (uint8_t)(((m_temp1 << 4) & 0xF0) + (m_temp2 & 0x0F));
	g_send_data_size += 3;
	return;
}

void app_VMmod_send_MeterNum_handle( void )
{
	g_send_data_buf[0] = (( VMCharToHex(&VMmeter_data_buf.meter_Addr[10]) & 0x0F ) << 4 ) + 
				( VMCharToHex(&VMmeter_data_buf.meter_Addr[11]) & 0x0F );
	g_send_data_buf[1] = (( VMCharToHex(&VMmeter_data_buf.meter_Addr[8]) & 0x0F ) << 4 ) + 
				(VMCharToHex(& VMmeter_data_buf.meter_Addr[9]) & 0x0F );
	g_send_data_buf[2] = (( VMCharToHex(&VMmeter_data_buf.meter_Addr[6]) & 0x0F ) << 4 ) + 
				( VMCharToHex(&VMmeter_data_buf.meter_Addr[7]) & 0x0F );
	g_send_data_buf[3] = (( VMCharToHex(&VMmeter_data_buf.meter_Addr[4]) & 0x0F ) << 4 ) + 
				( VMCharToHex(&VMmeter_data_buf.meter_Addr[5]) & 0x0F );
	g_send_data_buf[4] = (( VMCharToHex(&VMmeter_data_buf.meter_Addr[2]) & 0x0F ) << 4 ) + 
				( VMCharToHex(&VMmeter_data_buf.meter_Addr[3]) & 0x0F );
	g_send_data_buf[5] = (( VMCharToHex(&VMmeter_data_buf.meter_Addr[0]) & 0x0F ) << 4 ) + 
				( VMCharToHex(&VMmeter_data_buf.meter_Addr[1]) & 0x0F );
	g_send_data_size += 6;
	return;
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :���ݱ�ʶ��Ӧ���ݴ���(97)
 * @���� :NONE
 * @��� :������������1�����򷵻�0
********************************************************************/
void app_VMmod_97BZ_YG_handle( uint8_t *m_buf )//4�ֽ�
{
	float f_temp = 0.0;
	
	if ( m_buf[0] == 0x10 )		//Z��
	{
		f_temp = global_atof(VMmeter_data_buf.zy_power_ALL);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x11 )	//Z��
	{
		f_temp = global_atof(VMmeter_data_buf.zy_power_J);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x12 )	//Z��
	{
		f_temp = global_atof(VMmeter_data_buf.zy_power_F);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x13 )	//Zƽ
	{
		f_temp = global_atof(VMmeter_data_buf.zy_power_P);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x14 )	//Z��
	{
		f_temp = global_atof(VMmeter_data_buf.zy_power_G);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x1F )	//Z��
	{
		f_temp = global_atof(VMmeter_data_buf.zy_power_ALL);
		app_VMmod_send_4fdata_handle(f_temp,0);
		f_temp = global_atof(VMmeter_data_buf.zy_power_J);
		app_VMmod_send_4fdata_handle(f_temp,4);
		f_temp = global_atof(VMmeter_data_buf.zy_power_F);
		app_VMmod_send_4fdata_handle(f_temp,8);
		f_temp = global_atof(VMmeter_data_buf.zy_power_P);
		app_VMmod_send_4fdata_handle(f_temp,12);
		f_temp = global_atof(VMmeter_data_buf.zy_power_G);
		app_VMmod_send_4fdata_handle(f_temp,16);
	}
	else if ( m_buf[0] == 0x20 )	//F��
	{
		f_temp = global_atof(VMmeter_data_buf.fy_power_ALL);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x21 )	//F��
	{
		f_temp = global_atof(VMmeter_data_buf.fy_power_J);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x22 )	//F��
	{
		f_temp = global_atof(VMmeter_data_buf.fy_power_F);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x23 )	//Fƽ
	{
		f_temp = global_atof(VMmeter_data_buf.fy_power_P);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x24 )	//F��
	{
		f_temp = global_atof(VMmeter_data_buf.fy_power_G);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x2F )	//F��
	{
		f_temp = global_atof(VMmeter_data_buf.fy_power_ALL);
		app_VMmod_send_4fdata_handle(f_temp,0);
		f_temp = global_atof(VMmeter_data_buf.fy_power_J);
		app_VMmod_send_4fdata_handle(f_temp,4);
		f_temp = global_atof(VMmeter_data_buf.fy_power_F);
		app_VMmod_send_4fdata_handle(f_temp,8);
		f_temp = global_atof(VMmeter_data_buf.fy_power_P);
		app_VMmod_send_4fdata_handle(f_temp,12);
		f_temp = global_atof(VMmeter_data_buf.fy_power_G);
		app_VMmod_send_4fdata_handle(f_temp,16);
	}	
	return;
}

void app_VMmod_97BZ_WG_handle( uint8_t *m_buf )//4�ֽ�
{
	float f_temp = 0.0;
	if ( m_buf[0] == 0x10 )		//Z��
	{
		f_temp = global_atof(VMmeter_data_buf.zw_power_ALL);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x11 )	//Z��
	{
		f_temp = global_atof(VMmeter_data_buf.zw_power_J);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x12 )	//Z��
	{
		f_temp = global_atof(VMmeter_data_buf.zw_power_F);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x13 )	//Zƽ
	{
		f_temp = global_atof(VMmeter_data_buf.zw_power_P);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x14 )	//Z��
	{
		f_temp = global_atof(VMmeter_data_buf.zw_power_G);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x1F )	//Z��
	{
		f_temp = global_atof(VMmeter_data_buf.zw_power_ALL);
		app_VMmod_send_4fdata_handle(f_temp,0);
		f_temp = global_atof(VMmeter_data_buf.zw_power_J);
		app_VMmod_send_4fdata_handle(f_temp,4);
		f_temp = global_atof(VMmeter_data_buf.zw_power_F);
		app_VMmod_send_4fdata_handle(f_temp,8);
		f_temp = global_atof(VMmeter_data_buf.zw_power_P);
		app_VMmod_send_4fdata_handle(f_temp,12);
		f_temp = global_atof(VMmeter_data_buf.zw_power_G);
		app_VMmod_send_4fdata_handle(f_temp,16);
	}
	else if ( m_buf[0] == 0x20 )	//F��
	{
		f_temp = global_atof(VMmeter_data_buf.fw_power_ALL);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x21 )	//F��
	{
		f_temp = global_atof(VMmeter_data_buf.fw_power_J);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x22 )	//F��
	{
		f_temp = global_atof(VMmeter_data_buf.fw_power_F);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x23 )	//Fƽ
	{
		f_temp = global_atof(VMmeter_data_buf.fw_power_P);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x24 )	//F��
	{
		f_temp = global_atof(VMmeter_data_buf.fw_power_G);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x2F )	//F��
	{
		f_temp = global_atof(VMmeter_data_buf.fw_power_ALL);
		app_VMmod_send_4fdata_handle(f_temp,0);
		f_temp = global_atof(VMmeter_data_buf.fw_power_J);
		app_VMmod_send_4fdata_handle(f_temp,4);
		f_temp = global_atof(VMmeter_data_buf.fw_power_F);
		app_VMmod_send_4fdata_handle(f_temp,8);
		f_temp = global_atof(VMmeter_data_buf.fw_power_P);
		app_VMmod_send_4fdata_handle(f_temp,12);
		f_temp = global_atof(VMmeter_data_buf.fw_power_G);
		app_VMmod_send_4fdata_handle(f_temp,16);
	}

	return;
}

void app_VMmod_97BZ_UI_handle( uint8_t *m_buf )
{
	float f_temp = 0.0;
	if ( m_buf[0] == 0x11 )		//A���ѹ,1.5�ֽ�
	{
		f_temp = global_atof(VMmeter_data_buf.voltage_A);
		app_VMmod_send_2fdata_handle(f_temp,0,0);
	}
	else if ( m_buf[0] == 0x12 )	//B���ѹ,1.5�ֽ�
	{
		f_temp = global_atof(VMmeter_data_buf.voltage_B);
		app_VMmod_send_2fdata_handle(f_temp,0,0);
	}
	else if ( m_buf[0] == 0x13 )	//C���ѹ,1.5�ֽ�
	{
		f_temp = global_atof(VMmeter_data_buf.voltage_C);
		app_VMmod_send_2fdata_handle(f_temp,0,0);
	}
	else if ( m_buf[0] == 0x1F )	//��ѹ���ݿ�,6�ֽ�
	{
		f_temp = global_atof(VMmeter_data_buf.voltage_A);
		app_VMmod_send_2fdata_handle(f_temp,0,0);
		f_temp = global_atof(VMmeter_data_buf.voltage_B);
		app_VMmod_send_2fdata_handle(f_temp,2,0);
		f_temp = global_atof(VMmeter_data_buf.voltage_C);
		app_VMmod_send_2fdata_handle(f_temp,4,0);
	}
	else if ( m_buf[0] == 0x21 )	//A�����,2�ֽ�
	{
		f_temp = global_atof(VMmeter_data_buf.current_A);
		app_VMmod_send_2fdata_handle(f_temp,0,1);
	}
	else if ( m_buf[0] == 0x22 )	//B�����,2�ֽ�
	{
		f_temp = global_atof(VMmeter_data_buf.current_B);
		app_VMmod_send_2fdata_handle(f_temp,0,1);
	}
	else if ( m_buf[0] == 0x23 )	//C�����,2�ֽ�
	{
		f_temp = global_atof(VMmeter_data_buf.current_C);
		app_VMmod_send_2fdata_handle(f_temp,0,1);
	}
	else if ( m_buf[0] == 0x2F )	//�������ݿ�,6�ֽ�
	{
		f_temp = global_atof(VMmeter_data_buf.current_A);
		app_VMmod_send_2fdata_handle(f_temp,0,1);
		f_temp = global_atof(VMmeter_data_buf.current_B);
		app_VMmod_send_2fdata_handle(f_temp,2,1);
		f_temp = global_atof(VMmeter_data_buf.current_C);
		app_VMmod_send_2fdata_handle(f_temp,4,1);
	}
	return;
}

void app_VMmod_97BZ_TIME_NUM_handle( uint8_t *m_buf )
{
	if ( m_buf[0] == 0x10 )		//��������,4�ֽ�
	{
		app_VMmod_send_date_handle();
	}
	else if ( m_buf[0] == 0x11 )	//ʱ����,3�ֽ�
	{
		app_VMmod_send_time_handle();
	}
	else if ( m_buf[0] == 0x32 )	//���,6�ֽ�
	{
		app_VMmod_send_MeterNum_handle();
	}

	return;
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :���ݱ�ʶ��Ӧ���ݴ���(07)
 * @���� :NONE
 * @��� :������������1�����򷵻�0
********************************************************************/
void app_VMmod_07BZ_ZYG_handle( uint8_t *m_buf )//4�ֽ�
{
	float f_temp = 0.0;
	
	if ( m_buf[0] == 0x00 )		//Z��
	{
		f_temp = global_atof(VMmeter_data_buf.zy_power_ALL);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x01 )	//Z��
	{
		f_temp = global_atof(VMmeter_data_buf.zy_power_J);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x02 )	//Z��
	{
		f_temp = global_atof(VMmeter_data_buf.zy_power_F);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x03 )	//Zƽ
	{
		f_temp = global_atof(VMmeter_data_buf.zy_power_P);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x04 )	//Z��
	{
		f_temp = global_atof(VMmeter_data_buf.zy_power_G);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0xFF )	//Z��
	{
		f_temp = global_atof(VMmeter_data_buf.zy_power_ALL);
		app_VMmod_send_4fdata_handle(f_temp,0);
		f_temp = global_atof(VMmeter_data_buf.zy_power_J);
		app_VMmod_send_4fdata_handle(f_temp,4);
		f_temp = global_atof(VMmeter_data_buf.zy_power_F);
		app_VMmod_send_4fdata_handle(f_temp,8);
		f_temp = global_atof(VMmeter_data_buf.zy_power_P);
		app_VMmod_send_4fdata_handle(f_temp,12);
		f_temp = global_atof(VMmeter_data_buf.zy_power_G);
		app_VMmod_send_4fdata_handle(f_temp,16);
	}
	return;
}

void app_VMmod_07BZ_FYG_handle( uint8_t *m_buf )//4�ֽ�
{
	float f_temp = 0.0;
	
	if ( m_buf[0] == 0x00 )		//Z��
	{
		f_temp = global_atof(VMmeter_data_buf.fy_power_ALL);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x01 )	//Z��
	{
		f_temp = global_atof(VMmeter_data_buf.fy_power_J);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x02 )	//Z��
	{
		f_temp = global_atof(VMmeter_data_buf.fy_power_F);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x03 )	//Zƽ
	{
		f_temp = global_atof(VMmeter_data_buf.fy_power_P);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x04 )	//Z��
	{
		f_temp = global_atof(VMmeter_data_buf.fy_power_G);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0xFF )	//Z��
	{
		f_temp = global_atof(VMmeter_data_buf.fy_power_ALL);
		app_VMmod_send_4fdata_handle(f_temp,0);
		f_temp = global_atof(VMmeter_data_buf.fy_power_J);
		app_VMmod_send_4fdata_handle(f_temp,4);
		f_temp = global_atof(VMmeter_data_buf.fy_power_F);
		app_VMmod_send_4fdata_handle(f_temp,8);
		f_temp = global_atof(VMmeter_data_buf.fy_power_P);
		app_VMmod_send_4fdata_handle(f_temp,12);
		f_temp = global_atof(VMmeter_data_buf.fy_power_G);
		app_VMmod_send_4fdata_handle(f_temp,16);
	}
	return;
}

void app_VMmod_07BZ_ZWG_handle( uint8_t *m_buf )//4�ֽ�
{
	float f_temp = 0.0;
	
	if ( m_buf[0] == 0x00 )		//Z��
	{
		f_temp = global_atof(VMmeter_data_buf.zw_power_ALL);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x01 )	//Z��
	{
		f_temp = global_atof(VMmeter_data_buf.zw_power_J);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x02 )	//Z��
	{
		f_temp = global_atof(VMmeter_data_buf.zw_power_F);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x03 )	//Zƽ
	{
		f_temp = global_atof(VMmeter_data_buf.zw_power_P);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x04 )	//Z��
	{
		f_temp = global_atof(VMmeter_data_buf.zw_power_G);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0xFF )	//Z��
	{
		f_temp = global_atof(VMmeter_data_buf.zw_power_ALL);
		app_VMmod_send_4fdata_handle(f_temp,0);
		f_temp = global_atof(VMmeter_data_buf.zw_power_J);
		app_VMmod_send_4fdata_handle(f_temp,4);
		f_temp = global_atof(VMmeter_data_buf.zw_power_F);
		app_VMmod_send_4fdata_handle(f_temp,8);
		f_temp = global_atof(VMmeter_data_buf.zw_power_P);
		app_VMmod_send_4fdata_handle(f_temp,12);
		f_temp = global_atof(VMmeter_data_buf.zw_power_G);
		app_VMmod_send_4fdata_handle(f_temp,16);
	}
	return;
}

void app_VMmod_07BZ_FWG_handle( uint8_t *m_buf )//4�ֽ�
{
	float f_temp = 0.0;
	
	if ( m_buf[0] == 0x00 )		//F��
	{
		f_temp = global_atof(VMmeter_data_buf.fw_power_ALL);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x01 )	//F��
	{
		f_temp = global_atof(VMmeter_data_buf.fw_power_J);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x02 )	//F��
	{
		f_temp = global_atof(VMmeter_data_buf.fw_power_F);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x03 )	//Fƽ
	{
		f_temp = global_atof(VMmeter_data_buf.fw_power_P);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x04 )	//F��
	{
		f_temp = global_atof(VMmeter_data_buf.fw_power_G);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0xFF )	//F��
	{
		f_temp = global_atof(VMmeter_data_buf.fw_power_ALL);
		app_VMmod_send_4fdata_handle(f_temp,0);
		f_temp = global_atof(VMmeter_data_buf.fw_power_J);
		app_VMmod_send_4fdata_handle(f_temp,4);
		f_temp = global_atof(VMmeter_data_buf.fw_power_F);
		app_VMmod_send_4fdata_handle(f_temp,8);
		f_temp = global_atof(VMmeter_data_buf.fw_power_P);
		app_VMmod_send_4fdata_handle(f_temp,12);
		f_temp = global_atof(VMmeter_data_buf.fw_power_G);
		app_VMmod_send_4fdata_handle(f_temp,16);
	}
	return;
}

void app_VMmod_07BZ_U_handle( uint8_t *m_buf )//2�ֽ�
{
	float f_temp = 0.0;
	
	if ( m_buf[0] == 0x01 )		//A
	{
		f_temp = global_atof(VMmeter_data_buf.voltage_A);
		app_VMmod_send_2fdata_handle(f_temp,0,2);
	}
	else if ( m_buf[0] == 0x02 )	//B
	{
		f_temp = global_atof(VMmeter_data_buf.voltage_B);
		app_VMmod_send_2fdata_handle(f_temp,0,2);
	}
	else if ( m_buf[0] == 0x03 )	//C
	{
		f_temp = global_atof(VMmeter_data_buf.voltage_C);
		app_VMmod_send_2fdata_handle(f_temp,0,2);
	}
	else if ( m_buf[0] == 0xFF )	//��
	{
		f_temp = global_atof(VMmeter_data_buf.voltage_A);
		app_VMmod_send_2fdata_handle(f_temp,0,2);
		f_temp = global_atof(VMmeter_data_buf.voltage_B);
		app_VMmod_send_2fdata_handle(f_temp,2,2);
		f_temp = global_atof(VMmeter_data_buf.voltage_C);
		app_VMmod_send_2fdata_handle(f_temp,4,2);
	}
	return;
}

void app_VMmod_07BZ_I_handle( uint8_t *m_buf )//3�ֽ�
{
	float f_temp = 0.0;
	
	if ( m_buf[0] == 0x01 )		//A
	{
		f_temp = global_atof(VMmeter_data_buf.current_A);
		app_VMmod_send_3fdata_handle(f_temp,0,2);
	}
	else if ( m_buf[0] == 0x02 )	//B
	{
		f_temp = global_atof(VMmeter_data_buf.current_B);
		app_VMmod_send_3fdata_handle(f_temp,0,2);
	}
	else if ( m_buf[0] == 0x03 )	//C
	{
		f_temp = global_atof(VMmeter_data_buf.current_C);
		app_VMmod_send_3fdata_handle(f_temp,0,2);
	}
	else if ( m_buf[0] == 0xFF )	//��
	{
		f_temp = global_atof(VMmeter_data_buf.current_A);
		app_VMmod_send_3fdata_handle(f_temp,0,2);
		f_temp = global_atof(VMmeter_data_buf.current_B);
		app_VMmod_send_3fdata_handle(f_temp,3,2);
		f_temp = global_atof(VMmeter_data_buf.current_C);
		app_VMmod_send_3fdata_handle(f_temp,6,2);
	}
	return;
}

void app_VMmod_07BZ_Date_handle( void )//4�ֽ�
{
	app_VMmod_send_date_handle();
	return;
}

void app_VMmod_07BZ_Time_handle( void )//3�ֽ�
{
	app_VMmod_send_time_handle();
	return;
}

void app_VMmod_07BZ_MeterNum_handle( void )//6�ֽ�
{
	app_VMmod_send_MeterNum_handle();
	return;
}
/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :����������ݱ�ʶ�ж�
 * @���� :NONE
 * @��� :������������1�����򷵻�0
********************************************************************/
uint8_t app_VMmod_rec_code_flag_judge( uint8_t *m_buf )
{
	uint8_t m_rt = 0;
	uint8_t m_temp[4] = { 0, };
	
	if ( VMmeter_data_buf.guiyue == 0 )		//97
	{
		if ( m_buf[0] == 0x01 )				//�����ݿ�����
		{
			m_temp[0] = (m_buf[2] - 0x33) & 0xff;
			m_temp[1] = (m_buf[3] - 0x33) & 0xff;
			g_send_data_BZ[0] = (m_buf[2] - 0x33) & 0xff;
			g_send_data_BZ[1] = (m_buf[3] - 0x33) & 0xff;
			g_send_BZ_size = 2;
			g_send_data_size = 0;
			if ( m_temp[1] == 0x90 )		//�й�
			{
				app_VMmod_97BZ_YG_handle(&m_temp[0]);
				m_rt = 1;
			}
			else if ( m_temp[1] == 0x91 )   //�޹�
			{
				app_VMmod_97BZ_WG_handle(&m_temp[0]);
				m_rt = 1;
			}
			else if ( m_temp[1] == 0xB6 )	//V��I��P��Q
			{
				app_VMmod_97BZ_UI_handle(&m_temp[0]);
				m_rt = 1;
			}
			else if ( m_temp[1] == 0xC0 )	//ʱ�����ڱ�ų�����
			{
				app_VMmod_97BZ_TIME_NUM_handle(&m_temp[0]);
				m_rt = 1;
			}
			else if ( m_temp[1] == 0xB3 )	//ʧѹ
			{
				g_send_data_size = 3;
				GLOBAL_MEMCLR(g_send_data_buf, 30);
				m_rt = 1;
			}
			else 	//�쳣Ӧ��,�޴�����
			{
				app_VMmod_unusual_send_VMdata();
			}
		}
		else if ( m_buf[0] == 0x0A )		//д�豸��ַ������
		{

		}
	}
	else if ( VMmeter_data_buf.guiyue == 1 )//07
	{
		if ( m_buf[0] == 0x11 )				//�����ݿ�����
		{
			m_temp[0] = (m_buf[2] - 0x33) & 0xff;
			m_temp[1] = (m_buf[3] - 0x33) & 0xff;
			m_temp[2] = (m_buf[4] - 0x33) & 0xff;
			m_temp[3] = (m_buf[5] - 0x33) & 0xff;
			g_send_data_BZ[0] = (m_buf[2] - 0x33) & 0xff;
			g_send_data_BZ[1] = (m_buf[3] - 0x33) & 0xff;
			g_send_data_BZ[2] = (m_buf[4] - 0x33) & 0xff;
			g_send_data_BZ[3] = (m_buf[5] - 0x33) & 0xff;
			g_send_BZ_size = 4;
			g_send_data_size = 0;
			if ( m_temp[0] == 0x00 && m_temp[2] == 0x00 &&
				m_temp[3] == 0x00 )			//����й�
			{
				app_VMmod_07BZ_ZYG_handle(&m_temp[1]);
				m_rt = 1;
			}
			else if ( m_temp[0] == 0x00 && m_temp[2] == 0x01 &&
				m_temp[3] == 0x00 )			//�����й�
			{
				app_VMmod_07BZ_ZYG_handle(&m_temp[1]);
				m_rt = 1;
			}
			else if ( m_temp[0] == 0x00 && m_temp[2] == 0x02 &&
				m_temp[3] == 0x00 )			//�����й�
			{
				app_VMmod_07BZ_FYG_handle(&m_temp[1]);
				m_rt = 1;
			}
			else if ( m_temp[0] == 0x00 && m_temp[2] == 0x03 &&
				m_temp[3] == 0x00 )			//����޹�1
			{
				app_VMmod_07BZ_ZWG_handle(&m_temp[1]);
				m_rt = 1;
			}
			else if ( m_temp[0] == 0x00 && m_temp[2] == 0x04 &&
				m_temp[3] == 0x00 )			//����޹�2
			{
				app_VMmod_07BZ_FWG_handle(&m_temp[1]);
				m_rt = 1;
			}
			else if ( m_temp[0] == 0x00 && m_temp[2] == 0x01 &&
				m_temp[3] == 0x02 )			//��ѹ
			{
				app_VMmod_07BZ_U_handle(&m_temp[1]);
				m_rt = 1;
			}
			else if ( m_temp[0] == 0x00 && m_temp[2] == 0x02 &&
				m_temp[3] == 0x02 )			//����
			{
				app_VMmod_07BZ_I_handle(&m_temp[1]);
				m_rt = 1;
			}
			else if ( m_temp[0] == 0x01 && m_temp[1] == 0x01 && 
				m_temp[2] == 0x00 && m_temp[3] == 0x04 )//����
			{
				app_VMmod_07BZ_Date_handle();
				m_rt = 1;
			}
			else if ( m_temp[0] == 0x02 && m_temp[1] == 0x01 && 
				m_temp[2] == 0x00 && m_temp[3] == 0x04 )//ʱ��
			{
				app_VMmod_07BZ_Time_handle();
				m_rt = 1;
			}
			else if ( m_temp[0] == 0x02 && m_temp[1] == 0x04 && 
				m_temp[2] == 0x00 && m_temp[3] == 0x04 )//���
			{
				app_VMmod_07BZ_MeterNum_handle();
				m_rt = 1;
			}
			else if ( m_temp[0] == 0x01 && m_temp[1] == 0x04 && 
				m_temp[2] == 0x00 && m_temp[3] == 0x04 )//ͨ�ŵ�ַ
			{
				app_VMmod_07BZ_MeterNum_handle();
				m_rt = 1;
			}
			else if ( m_temp[0] == 0x00 && m_temp[1] == 0x00 && 
				m_temp[2] == 0x05 && m_temp[3] == 0x03 )//��ʧѹ
			{
				g_send_data_size = 6;
				GLOBAL_MEMCLR(g_send_data_buf, 30);
				m_rt = 1;
			}
			else if ( m_temp[0] == 0x00 && m_temp[1] == 0x00 && 
				m_temp[2] == 0x01 && m_temp[3] == 0x03 )//ʧѹABC
			{
				g_send_data_size = 6*3;
				GLOBAL_MEMCLR(g_send_data_buf, 30);
				m_rt = 1;
			}
			else 	//�쳣Ӧ��,�޴�����
			{
				app_VMmod_unusual_send_VMdata();
			}
		}
		else if ( m_buf[0] == 0x13 )		//���豸��ַ������
		{

		}
		else if ( m_buf[0] == 0x15 )		//д�豸��ַ������
		{

		}
	}
	return m_rt;
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :��������������
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_VMmod_send_VMdata( void )
{
	send_VMdata_t m_send_buf = { 0, };
	uint8_t m_send[35] = { 0, };
	uint8_t m_offset = 0;

	GLOBAL_MEMCLR(m_send, 35);
	m_send_buf.head1 = 0x68;
	m_send_buf.addr[0] = (( VMCharToHex(&VMmeter_data_buf.meter_Addr[10]) & 0x0F ) << 4 ) + 
				( VMCharToHex(&VMmeter_data_buf.meter_Addr[11]) & 0x0F );
	m_send_buf.addr[1] = (( VMCharToHex(&VMmeter_data_buf.meter_Addr[8]) & 0x0F ) << 4 ) + 
				(VMCharToHex(& VMmeter_data_buf.meter_Addr[9]) & 0x0F );
	m_send_buf.addr[2] = (( VMCharToHex(&VMmeter_data_buf.meter_Addr[6]) & 0x0F ) << 4 ) + 
				( VMCharToHex(&VMmeter_data_buf.meter_Addr[7]) & 0x0F );
	m_send_buf.addr[3] = (( VMCharToHex(&VMmeter_data_buf.meter_Addr[4]) & 0x0F ) << 4 ) + 
				( VMCharToHex(&VMmeter_data_buf.meter_Addr[5]) & 0x0F );
	m_send_buf.addr[4] = (( VMCharToHex(&VMmeter_data_buf.meter_Addr[2]) & 0x0F ) << 4 ) + 
				( VMCharToHex(&VMmeter_data_buf.meter_Addr[3]) & 0x0F );
	m_send_buf.addr[5] = (( VMCharToHex(&VMmeter_data_buf.meter_Addr[0]) & 0x0F ) << 4 ) + 
				( VMCharToHex(&VMmeter_data_buf.meter_Addr[1]) & 0x0F );
	m_send_buf.head2 = 0x68;
	
	if ( VMmeter_data_buf.guiyue == 0 )		m_send_buf.cmd = 0x81;
	else if ( VMmeter_data_buf.guiyue == 1 )m_send_buf.cmd = 0x91;
	m_send_buf.len = g_send_BZ_size + g_send_data_size;
	if ( g_send_BZ_size == 2 )
	{
		m_send[0] = g_send_data_BZ[0];
		m_send[1] = g_send_data_BZ[1];
	}
	else if ( g_send_BZ_size == 4 )
	{
		m_send[0] = g_send_data_BZ[0];
		m_send[1] = g_send_data_BZ[1];
		m_send[2] = g_send_data_BZ[2];
		m_send[3] = g_send_data_BZ[3];
	}
	global_memcpy(&m_send[g_send_BZ_size], g_send_data_buf, g_send_data_size);
	for ( m_offset = 0;m_offset < m_send_buf.len;m_offset++)
	{
		m_send[m_offset] += 0x33;
	}
	m_send_buf.crc = global_sum_array((char*)&m_send_buf.head1,sizeof(send_VMdata_t)-2,IS_CHAR);
	m_send_buf.crc += global_sum_array((char*)m_send,m_send_buf.len,IS_CHAR);
	m_send_buf.end = 0x16;
	app_VMmod_Usart3_send(&m_send_buf,10);
	app_VMmod_Usart3_send(&m_send,m_send_buf.len);
	app_VMmod_Usart3_send(&m_send_buf.crc,2);
	return;
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :�޴�����ʱ�����쳣Ӧ��
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_VMmod_unusual_send_VMdata( void )
{
	send_VMdata_t m_send_buf = { 0, };
	uint8_t m_send = 0;
	
	m_send_buf.head1 = 0x68;
	m_send_buf.addr[0] = (( VMCharToHex(&VMmeter_data_buf.meter_Addr[10]) & 0x0F ) << 4 ) + 
				( VMCharToHex(&VMmeter_data_buf.meter_Addr[11]) & 0x0F );
	m_send_buf.addr[1] = (( VMCharToHex(&VMmeter_data_buf.meter_Addr[8]) & 0x0F ) << 4 ) + 
				(VMCharToHex(& VMmeter_data_buf.meter_Addr[9]) & 0x0F );
	m_send_buf.addr[2] = (( VMCharToHex(&VMmeter_data_buf.meter_Addr[6]) & 0x0F ) << 4 ) + 
				( VMCharToHex(&VMmeter_data_buf.meter_Addr[7]) & 0x0F );
	m_send_buf.addr[3] = (( VMCharToHex(&VMmeter_data_buf.meter_Addr[4]) & 0x0F ) << 4 ) + 
				( VMCharToHex(&VMmeter_data_buf.meter_Addr[5]) & 0x0F );
	m_send_buf.addr[4] = (( VMCharToHex(&VMmeter_data_buf.meter_Addr[2]) & 0x0F ) << 4 ) + 
				( VMCharToHex(&VMmeter_data_buf.meter_Addr[3]) & 0x0F );
	m_send_buf.addr[5] = (( VMCharToHex(&VMmeter_data_buf.meter_Addr[0]) & 0x0F ) << 4 ) + 
				( VMCharToHex(&VMmeter_data_buf.meter_Addr[1]) & 0x0F );
	m_send_buf.head2 = 0x68;
	
	if ( VMmeter_data_buf.guiyue == 0 )		
	{
		m_send_buf.cmd = 0xC1;
		m_send = 0x01+0x33;
	}
	else if ( VMmeter_data_buf.guiyue == 1 )
	{
		m_send_buf.cmd = 0xD1;
		m_send = 0x02+0x33;
	}
	m_send_buf.len = 1;
	
	m_send_buf.crc = global_sum_array((char*)&m_send_buf.head1,sizeof(send_VMdata_t)-2,IS_CHAR);
	m_send += m_send_buf.crc;
	m_send_buf.crc = (uint8_t)m_send;
	m_send_buf.end = 0x16;
	app_VMmod_Usart3_send(&m_send_buf,10);
	app_VMmod_Usart3_send(&m_send,m_send_buf.len);
	app_VMmod_Usart3_send(&m_send_buf.crc,2);
	return;
}


/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:模块主体程序
* 创建时间:2013-05-17
* 所属公司 :科荟
* 文件名称:app_VM_protocol_analysis.c
* 作者 :周炳权
* 功能描述:实现虚拟电表协议分析和数据处理功能
******************************************************************************************/
#include	"app_VM_inside.h"


uint8_t g_send_BZ_size = 0;
uint8_t g_send_data_BZ[4] = { 0, };
uint8_t g_send_data_size = 0;
uint8_t g_send_data_buf[30] = { 0, };

/********************************************************************
 * @作者 :周炳权
 * @功能 :接收地址域判断
 * @输入 :NONE
 * @输出 :符合条件返回1，否则返回0
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
 * @作者 :周炳权
 * @功能 :发送的数据处理
 * @输入 :m_ftemp:输入电参数,m_moffset:发送缓存偏移;
 *  		m_pt:小数标志,1,2为有小数,0则无;
 * @输出 :NONE
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
	else if ( m_pt == 1 )//2位小数
	{
		hex_convert_bcd(i_temp, &u8_temp);
		g_send_data_buf[m_moffset] = u8_temp;
		hex_convert_bcd((uint8_t)(l_temp % 100), &u8_temp);
		g_send_data_buf[m_moffset+1] = u8_temp;
		g_send_data_size += 2;
	}
	else if ( m_pt == 2 )// 1位小数
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
	else if ( m_pt == 1 )//2位小数
	{
		hex_convert_bcd(i_temp, &u8_temp);
		g_send_data_buf[m_moffset] = u8_temp;
		hex_convert_bcd((uint8_t)(l_temp % 100), &u8_temp);
		g_send_data_buf[m_moffset+1] = u8_temp;
		hex_convert_bcd((uint8_t)(((l_temp % 10000) / 100)), &u8_temp);
		g_send_data_buf[m_moffset+2] = u8_temp;
		g_send_data_size += 3;
	}
	else if ( m_pt == 2 )//3位小数
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
	
	g_send_data_buf[0] = 0;	//周次
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
 * @作者 :周炳权
 * @功能 :数据标识对应数据处理(97)
 * @输入 :NONE
 * @输出 :符合条件返回1，否则返回0
********************************************************************/
void app_VMmod_97BZ_YG_handle( uint8_t *m_buf )//4字节
{
	float f_temp = 0.0;
	
	if ( m_buf[0] == 0x10 )		//Z总
	{
		f_temp = global_atof(VMmeter_data_buf.zy_power_ALL);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x11 )	//Z尖
	{
		f_temp = global_atof(VMmeter_data_buf.zy_power_J);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x12 )	//Z峰
	{
		f_temp = global_atof(VMmeter_data_buf.zy_power_F);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x13 )	//Z平
	{
		f_temp = global_atof(VMmeter_data_buf.zy_power_P);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x14 )	//Z谷
	{
		f_temp = global_atof(VMmeter_data_buf.zy_power_G);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x1F )	//Z块
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
	else if ( m_buf[0] == 0x20 )	//F总
	{
		f_temp = global_atof(VMmeter_data_buf.fy_power_ALL);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x21 )	//F尖
	{
		f_temp = global_atof(VMmeter_data_buf.fy_power_J);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x22 )	//F峰
	{
		f_temp = global_atof(VMmeter_data_buf.fy_power_F);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x23 )	//F平
	{
		f_temp = global_atof(VMmeter_data_buf.fy_power_P);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x24 )	//F谷
	{
		f_temp = global_atof(VMmeter_data_buf.fy_power_G);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x2F )	//F块
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

void app_VMmod_97BZ_WG_handle( uint8_t *m_buf )//4字节
{
	float f_temp = 0.0;
	if ( m_buf[0] == 0x10 )		//Z总
	{
		f_temp = global_atof(VMmeter_data_buf.zw_power_ALL);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x11 )	//Z尖
	{
		f_temp = global_atof(VMmeter_data_buf.zw_power_J);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x12 )	//Z峰
	{
		f_temp = global_atof(VMmeter_data_buf.zw_power_F);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x13 )	//Z平
	{
		f_temp = global_atof(VMmeter_data_buf.zw_power_P);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x14 )	//Z谷
	{
		f_temp = global_atof(VMmeter_data_buf.zw_power_G);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x1F )	//Z块
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
	else if ( m_buf[0] == 0x20 )	//F总
	{
		f_temp = global_atof(VMmeter_data_buf.fw_power_ALL);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x21 )	//F尖
	{
		f_temp = global_atof(VMmeter_data_buf.fw_power_J);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x22 )	//F峰
	{
		f_temp = global_atof(VMmeter_data_buf.fw_power_F);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x23 )	//F平
	{
		f_temp = global_atof(VMmeter_data_buf.fw_power_P);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x24 )	//F谷
	{
		f_temp = global_atof(VMmeter_data_buf.fw_power_G);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x2F )	//F块
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
	if ( m_buf[0] == 0x11 )		//A相电压,1.5字节
	{
		f_temp = global_atof(VMmeter_data_buf.voltage_A);
		app_VMmod_send_2fdata_handle(f_temp,0,0);
	}
	else if ( m_buf[0] == 0x12 )	//B相电压,1.5字节
	{
		f_temp = global_atof(VMmeter_data_buf.voltage_B);
		app_VMmod_send_2fdata_handle(f_temp,0,0);
	}
	else if ( m_buf[0] == 0x13 )	//C相电压,1.5字节
	{
		f_temp = global_atof(VMmeter_data_buf.voltage_C);
		app_VMmod_send_2fdata_handle(f_temp,0,0);
	}
	else if ( m_buf[0] == 0x1F )	//电压数据块,6字节
	{
		f_temp = global_atof(VMmeter_data_buf.voltage_A);
		app_VMmod_send_2fdata_handle(f_temp,0,0);
		f_temp = global_atof(VMmeter_data_buf.voltage_B);
		app_VMmod_send_2fdata_handle(f_temp,2,0);
		f_temp = global_atof(VMmeter_data_buf.voltage_C);
		app_VMmod_send_2fdata_handle(f_temp,4,0);
	}
	else if ( m_buf[0] == 0x21 )	//A相电流,2字节
	{
		f_temp = global_atof(VMmeter_data_buf.current_A);
		app_VMmod_send_2fdata_handle(f_temp,0,1);
	}
	else if ( m_buf[0] == 0x22 )	//B相电流,2字节
	{
		f_temp = global_atof(VMmeter_data_buf.current_B);
		app_VMmod_send_2fdata_handle(f_temp,0,1);
	}
	else if ( m_buf[0] == 0x23 )	//C相电流,2字节
	{
		f_temp = global_atof(VMmeter_data_buf.current_C);
		app_VMmod_send_2fdata_handle(f_temp,0,1);
	}
	else if ( m_buf[0] == 0x2F )	//电流数据块,6字节
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
	if ( m_buf[0] == 0x10 )		//年月日周,4字节
	{
		app_VMmod_send_date_handle();
	}
	else if ( m_buf[0] == 0x11 )	//时分秒,3字节
	{
		app_VMmod_send_time_handle();
	}
	else if ( m_buf[0] == 0x32 )	//表号,6字节
	{
		app_VMmod_send_MeterNum_handle();
	}

	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :数据标识对应数据处理(07)
 * @输入 :NONE
 * @输出 :符合条件返回1，否则返回0
********************************************************************/
void app_VMmod_07BZ_ZYG_handle( uint8_t *m_buf )//4字节
{
	float f_temp = 0.0;
	
	if ( m_buf[0] == 0x00 )		//Z总
	{
		f_temp = global_atof(VMmeter_data_buf.zy_power_ALL);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x01 )	//Z尖
	{
		f_temp = global_atof(VMmeter_data_buf.zy_power_J);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x02 )	//Z峰
	{
		f_temp = global_atof(VMmeter_data_buf.zy_power_F);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x03 )	//Z平
	{
		f_temp = global_atof(VMmeter_data_buf.zy_power_P);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x04 )	//Z谷
	{
		f_temp = global_atof(VMmeter_data_buf.zy_power_G);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0xFF )	//Z块
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

void app_VMmod_07BZ_FYG_handle( uint8_t *m_buf )//4字节
{
	float f_temp = 0.0;
	
	if ( m_buf[0] == 0x00 )		//Z总
	{
		f_temp = global_atof(VMmeter_data_buf.fy_power_ALL);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x01 )	//Z尖
	{
		f_temp = global_atof(VMmeter_data_buf.fy_power_J);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x02 )	//Z峰
	{
		f_temp = global_atof(VMmeter_data_buf.fy_power_F);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x03 )	//Z平
	{
		f_temp = global_atof(VMmeter_data_buf.fy_power_P);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x04 )	//Z谷
	{
		f_temp = global_atof(VMmeter_data_buf.fy_power_G);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0xFF )	//Z块
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

void app_VMmod_07BZ_ZWG_handle( uint8_t *m_buf )//4字节
{
	float f_temp = 0.0;
	
	if ( m_buf[0] == 0x00 )		//Z总
	{
		f_temp = global_atof(VMmeter_data_buf.zw_power_ALL);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x01 )	//Z尖
	{
		f_temp = global_atof(VMmeter_data_buf.zw_power_J);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x02 )	//Z峰
	{
		f_temp = global_atof(VMmeter_data_buf.zw_power_F);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x03 )	//Z平
	{
		f_temp = global_atof(VMmeter_data_buf.zw_power_P);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x04 )	//Z谷
	{
		f_temp = global_atof(VMmeter_data_buf.zw_power_G);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0xFF )	//Z块
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

void app_VMmod_07BZ_FWG_handle( uint8_t *m_buf )//4字节
{
	float f_temp = 0.0;
	
	if ( m_buf[0] == 0x00 )		//F总
	{
		f_temp = global_atof(VMmeter_data_buf.fw_power_ALL);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x01 )	//F尖
	{
		f_temp = global_atof(VMmeter_data_buf.fw_power_J);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x02 )	//F峰
	{
		f_temp = global_atof(VMmeter_data_buf.fw_power_F);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x03 )	//F平
	{
		f_temp = global_atof(VMmeter_data_buf.fw_power_P);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0x04 )	//F谷
	{
		f_temp = global_atof(VMmeter_data_buf.fw_power_G);
		app_VMmod_send_4fdata_handle(f_temp,0);
	}
	else if ( m_buf[0] == 0xFF )	//F块
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

void app_VMmod_07BZ_U_handle( uint8_t *m_buf )//2字节
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
	else if ( m_buf[0] == 0xFF )	//块
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

void app_VMmod_07BZ_I_handle( uint8_t *m_buf )//3字节
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
	else if ( m_buf[0] == 0xFF )	//块
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

void app_VMmod_07BZ_Date_handle( void )//4字节
{
	app_VMmod_send_date_handle();
	return;
}

void app_VMmod_07BZ_Time_handle( void )//3字节
{
	app_VMmod_send_time_handle();
	return;
}

void app_VMmod_07BZ_MeterNum_handle( void )//6字节
{
	app_VMmod_send_MeterNum_handle();
	return;
}
/********************************************************************
 * @作者 :周炳权
 * @功能 :控制域和数据标识判断
 * @输入 :NONE
 * @输出 :符合条件返回1，否则返回0
********************************************************************/
uint8_t app_VMmod_rec_code_flag_judge( uint8_t *m_buf )
{
	uint8_t m_rt = 0;
	uint8_t m_temp[4] = { 0, };
	
	if ( VMmeter_data_buf.guiyue == 0 )		//97
	{
		if ( m_buf[0] == 0x01 )				//读数据控制码
		{
			m_temp[0] = (m_buf[2] - 0x33) & 0xff;
			m_temp[1] = (m_buf[3] - 0x33) & 0xff;
			g_send_data_BZ[0] = (m_buf[2] - 0x33) & 0xff;
			g_send_data_BZ[1] = (m_buf[3] - 0x33) & 0xff;
			g_send_BZ_size = 2;
			g_send_data_size = 0;
			if ( m_temp[1] == 0x90 )		//有功
			{
				app_VMmod_97BZ_YG_handle(&m_temp[0]);
				m_rt = 1;
			}
			else if ( m_temp[1] == 0x91 )   //无功
			{
				app_VMmod_97BZ_WG_handle(&m_temp[0]);
				m_rt = 1;
			}
			else if ( m_temp[1] == 0xB6 )	//V、I、P、Q
			{
				app_VMmod_97BZ_UI_handle(&m_temp[0]);
				m_rt = 1;
			}
			else if ( m_temp[1] == 0xC0 )	//时间日期表号常数等
			{
				app_VMmod_97BZ_TIME_NUM_handle(&m_temp[0]);
				m_rt = 1;
			}
			else if ( m_temp[1] == 0xB3 )	//失压
			{
				g_send_data_size = 3;
				GLOBAL_MEMCLR(g_send_data_buf, 30);
				m_rt = 1;
			}
			else 	//异常应答,无此数据
			{
				app_VMmod_unusual_send_VMdata();
			}
		}
		else if ( m_buf[0] == 0x0A )		//写设备地址控制码
		{

		}
	}
	else if ( VMmeter_data_buf.guiyue == 1 )//07
	{
		if ( m_buf[0] == 0x11 )				//读数据控制码
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
				m_temp[3] == 0x00 )			//组合有功
			{
				app_VMmod_07BZ_ZYG_handle(&m_temp[1]);
				m_rt = 1;
			}
			else if ( m_temp[0] == 0x00 && m_temp[2] == 0x01 &&
				m_temp[3] == 0x00 )			//正向有功
			{
				app_VMmod_07BZ_ZYG_handle(&m_temp[1]);
				m_rt = 1;
			}
			else if ( m_temp[0] == 0x00 && m_temp[2] == 0x02 &&
				m_temp[3] == 0x00 )			//反向有功
			{
				app_VMmod_07BZ_FYG_handle(&m_temp[1]);
				m_rt = 1;
			}
			else if ( m_temp[0] == 0x00 && m_temp[2] == 0x03 &&
				m_temp[3] == 0x00 )			//组合无功1
			{
				app_VMmod_07BZ_ZWG_handle(&m_temp[1]);
				m_rt = 1;
			}
			else if ( m_temp[0] == 0x00 && m_temp[2] == 0x04 &&
				m_temp[3] == 0x00 )			//组合无功2
			{
				app_VMmod_07BZ_FWG_handle(&m_temp[1]);
				m_rt = 1;
			}
			else if ( m_temp[0] == 0x00 && m_temp[2] == 0x01 &&
				m_temp[3] == 0x02 )			//电压
			{
				app_VMmod_07BZ_U_handle(&m_temp[1]);
				m_rt = 1;
			}
			else if ( m_temp[0] == 0x00 && m_temp[2] == 0x02 &&
				m_temp[3] == 0x02 )			//电流
			{
				app_VMmod_07BZ_I_handle(&m_temp[1]);
				m_rt = 1;
			}
			else if ( m_temp[0] == 0x01 && m_temp[1] == 0x01 && 
				m_temp[2] == 0x00 && m_temp[3] == 0x04 )//日期
			{
				app_VMmod_07BZ_Date_handle();
				m_rt = 1;
			}
			else if ( m_temp[0] == 0x02 && m_temp[1] == 0x01 && 
				m_temp[2] == 0x00 && m_temp[3] == 0x04 )//时间
			{
				app_VMmod_07BZ_Time_handle();
				m_rt = 1;
			}
			else if ( m_temp[0] == 0x02 && m_temp[1] == 0x04 && 
				m_temp[2] == 0x00 && m_temp[3] == 0x04 )//表号
			{
				app_VMmod_07BZ_MeterNum_handle();
				m_rt = 1;
			}
			else if ( m_temp[0] == 0x01 && m_temp[1] == 0x04 && 
				m_temp[2] == 0x00 && m_temp[3] == 0x04 )//通信地址
			{
				app_VMmod_07BZ_MeterNum_handle();
				m_rt = 1;
			}
			else if ( m_temp[0] == 0x00 && m_temp[1] == 0x00 && 
				m_temp[2] == 0x05 && m_temp[3] == 0x03 )//总失压
			{
				g_send_data_size = 6;
				GLOBAL_MEMCLR(g_send_data_buf, 30);
				m_rt = 1;
			}
			else if ( m_temp[0] == 0x00 && m_temp[1] == 0x00 && 
				m_temp[2] == 0x01 && m_temp[3] == 0x03 )//失压ABC
			{
				g_send_data_size = 6*3;
				GLOBAL_MEMCLR(g_send_data_buf, 30);
				m_rt = 1;
			}
			else 	//异常应答,无此数据
			{
				app_VMmod_unusual_send_VMdata();
			}
		}
		else if ( m_buf[0] == 0x13 )		//读设备地址控制码
		{

		}
		else if ( m_buf[0] == 0x15 )		//写设备地址控制码
		{

		}
	}
	return m_rt;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :发送虚拟电表数据
 * @输入 :NONE
 * @输出 :NONE
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
 * @作者 :周炳权
 * @功能 :无此数据时发送异常应答
 * @输入 :NONE
 * @输出 :NONE
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


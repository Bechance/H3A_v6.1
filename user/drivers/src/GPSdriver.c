/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:GPS驱动
* 创建时间:2012-09-11
* 所属公司 :科荟
* 文件名称:GPSdriver.c   //for 28335
* 作者 :周炳权
* 功能描述:提供GPS NMEA-0183协议解析并提取有效数据接口
*----------------------------------------------------------------
修改记录:

修改日期:

修改人:

**********************************************************************************************************/
#include "../Osdriver.h"

static dev_id_t         m_GPS_dev_id = INVALID_DEV_ID;
uint8_t					comma_count = 0;
GPS_Time_Date_t			GPS_Time_Date_buf = {0,};
extern		uint8_t      m_serialB_rev_fifo[];
uint16_t	m_fifo_count = 0;
uint8_t		m_fifo_over = 0;
#define     GPS_INT_EN()  		{PieCtrlRegs.PIEIER9.bit.INTx3 = 1;}
#define     GPS_INT_DIS() 		{PieCtrlRegs.PIEIER9.bit.INTx3 = 0;}
#define		GPS_serial_OPEN()	{GpioDataRegs.GPBSET.bit.GPIO52 = 1;}		
#define		RM_serial_OPEN()	{GpioDataRegs.GPBCLEAR.bit.GPIO52 = 1;}
//$GPRMC,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,<10>,<11>,<12>*hh<CR><LF>
/********************************************************************
* @作者 :周炳权
* @功能 :GPS数据有效性分析
* @输入 :NONE
* @输出 :成功:1;失败:0;
********************************************************************/
uint8_t GPS_data_validity_analysis( void )
{
	long m_temp = global_atoi(GPS_Time_Date_buf.year_buf);
	if (  (m_temp > 11) && (m_temp < 99) )
	{
		m_temp = global_atoi(GPS_Time_Date_buf.month_buf);
		if ( (m_temp > 0) && ( m_temp < 13) )
		{			
			m_temp = global_atoi(GPS_Time_Date_buf.day_buf);
			if ( (m_temp > 0) && ( m_temp < 32) )
			{
				m_temp = global_atoi(GPS_Time_Date_buf.hour_buf);
				if ( (m_temp >= 0) && ( m_temp < 25) )
				{
					m_temp = global_atoi(GPS_Time_Date_buf.min_buf);
					if ( (m_temp >= 0) && ( m_temp < 61 ) )
					{
						m_temp = global_atoi(GPS_Time_Date_buf.sec_buf);
						if ( (m_temp >= 0) && ( m_temp < 61 ) )
						{
							return 1;
						}
					}
				}
			}
		}
	}
	return 0;
}
/********************************************************************
* @作者 :周炳权
* @功能 :GPS时间日期转换成北京时间日期
* @输入 :NONE
* @输出 :成功:1;失败:0;
********************************************************************/
static uint8_t Leap_Year_decide( long m_year )
{
	if ( (m_year%400 == 0) || (m_year%4 == 0 && m_year%100 != 0))
	return 1;
	return 0;
}
static void Year_over_handle(uint8_t m_month,uint8_t m_year,GPS_Time_Date_t m_GPS_buf)
{
	m_month += 1;
	if ( m_month < 13 )
	{
		global_itoa(m_month, m_GPS_buf.month_buf);
		return;
	}
	else if ( m_month == 13 )
	{
		m_month -= 12;
		m_year += 1;
		global_itoa(m_month, m_GPS_buf.month_buf);
		global_itoa(m_year, m_GPS_buf.year_buf);
		return;
	}
	return;
}
void UTC_Time_convert_BJ_Time( GPS_Time_Date_t *m_GPS_Time_Date_buf )
{
	uint8_t m_day_temp = (uint8_t)global_atoi(m_GPS_Time_Date_buf->day_buf);
	uint8_t m_month_temp = (uint8_t)global_atoi(m_GPS_Time_Date_buf->month_buf);
	uint8_t m_year_temp = (uint8_t)global_atoi(m_GPS_Time_Date_buf->year_buf);
	uint8_t m_temp = (uint8_t)global_atoi(m_GPS_Time_Date_buf->hour_buf);
	m_temp += 8;
	if ( m_temp >= 24 )
	{
		m_temp -= 24;
		global_itoa(m_temp, m_GPS_Time_Date_buf->hour_buf);
	}
	else 
	{
		global_itoa(m_temp, m_GPS_Time_Date_buf->hour_buf);
		if ( m_GPS_Time_Date_buf->hour_buf[1] == 0 )
		{
			if ( m_GPS_Time_Date_buf->hour_buf[0] == 0 )
			m_GPS_Time_Date_buf->hour_buf[1] = 0x30;
			else m_GPS_Time_Date_buf->hour_buf[1] = m_GPS_Time_Date_buf->hour_buf[0];
			m_GPS_Time_Date_buf->hour_buf[0] = 0x30;// '0'
		}
		return;
	}
	m_day_temp += 1;
	if ( (m_month_temp == 1 || m_month_temp == 3 || m_month_temp == 5 || 
		m_month_temp == 7 || m_month_temp == 8 || m_month_temp == 10 || 
		m_month_temp == 12) && (m_day_temp < 32 ))
	{
		global_itoa(m_day_temp, m_GPS_Time_Date_buf->day_buf);
		return;
	}
	else if ( (m_month_temp == 1 || m_month_temp == 3 || m_month_temp == 5 || 
		m_month_temp == 7 || m_month_temp == 8 || m_month_temp == 10 || 
		m_month_temp == 12) && (m_day_temp == 32 ))
	{
		m_day_temp -= 31;
		global_itoa(m_day_temp, m_GPS_Time_Date_buf->day_buf);
		Year_over_handle(m_month_temp,m_year_temp,*m_GPS_Time_Date_buf);
	}
	else if ( (m_month_temp == 4 || m_month_temp == 6 || m_month_temp == 9 || 
		m_month_temp == 11 ) && (m_day_temp < 31 ))
	{
		global_itoa(m_day_temp, m_GPS_Time_Date_buf->day_buf);
		return;
	}
	else if ( (m_month_temp == 4 || m_month_temp == 6 || m_month_temp == 9 || 
		m_month_temp == 11 ) && (m_day_temp == 31 ))
	{
		m_day_temp -= 30;
		global_itoa(m_day_temp, m_GPS_Time_Date_buf->day_buf);
		Year_over_handle(m_month_temp,m_year_temp,*m_GPS_Time_Date_buf);
	}
	else if ( m_month_temp == 2 && m_day_temp < 30 )
	{
		if ( Leap_Year_decide(m_year_temp) )
		{
			global_itoa(m_day_temp, m_GPS_Time_Date_buf->day_buf);
			return;
		}
		else if ( m_day_temp < 29 )
		{
			global_itoa(m_day_temp, m_GPS_Time_Date_buf->day_buf);
			return;
		}
		else if ( m_day_temp == 29 )
		{
			m_day_temp -= 28;
			global_itoa(m_day_temp, m_GPS_Time_Date_buf->day_buf);
			Year_over_handle(m_month_temp,m_year_temp,*m_GPS_Time_Date_buf);
		}
	}
	else if ( m_month_temp == 2 && m_day_temp == 30 )
	{
		if ( Leap_Year_decide(m_year_temp) )
		{
			m_day_temp -= 29;
			global_itoa(m_day_temp, m_GPS_Time_Date_buf->day_buf);
			Year_over_handle(m_month_temp,m_year_temp,*m_GPS_Time_Date_buf);
		}
	}
	return;
}
/********************************************************************
* @作者 :周炳权
* @功能 :GPS协议头分析
* @输入 :NONE
* @输出 :成功:1;失败:0;
********************************************************************/
uint8_t	Protocol_head_analysis( void )
{
	if ( m_serialB_rev_fifo[m_fifo_count] == '$' )
	{
		(m_fifo_count == 1023 )?(m_fifo_over = 1):(m_fifo_count++);
		if ( m_serialB_rev_fifo[m_fifo_count] == 'G' )
		{
			(m_fifo_count == 1023 )?(m_fifo_over = 1):(m_fifo_count++);
			if ( m_serialB_rev_fifo[m_fifo_count] == 'P' )
			{
				(m_fifo_count == 1023 )?(m_fifo_over = 1):(m_fifo_count++);
				if ( m_serialB_rev_fifo[m_fifo_count] == 'R' )
				{
					(m_fifo_count == 1023 )?(m_fifo_over = 1):(m_fifo_count++);
					if ( m_serialB_rev_fifo[m_fifo_count] == 'M' )
					{
						(m_fifo_count == 1023 )?(m_fifo_over = 1):(m_fifo_count++);
						if ( m_serialB_rev_fifo[m_fifo_count] == 'C' )
						{
							(m_fifo_count == 1023 )?(m_fifo_over = 1):(m_fifo_count++);
							return 1;
						}
					}
				}
			}
		}
	}
	(m_fifo_count == 1023 )?(m_fifo_over = 1):(m_fifo_count++);
	return 0;
}
/********************************************************************
* @作者 :周炳权
* @功能 :GPS Time获取
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void GPS_Time_obtian( void )
{
	GPS_Time_Date_buf.hour_buf[0] = m_serialB_rev_fifo[m_fifo_count];
	(m_fifo_count == 1023 )?(m_fifo_count = 0):(m_fifo_count++);
	GPS_Time_Date_buf.hour_buf[1] = m_serialB_rev_fifo[m_fifo_count];
	(m_fifo_count == 1023 )?(m_fifo_count = 0):(m_fifo_count++);
	GPS_Time_Date_buf.min_buf[0] = m_serialB_rev_fifo[m_fifo_count];
	(m_fifo_count == 1023 )?(m_fifo_count = 0):(m_fifo_count++);
	GPS_Time_Date_buf.min_buf[1] = m_serialB_rev_fifo[m_fifo_count];
	(m_fifo_count == 1023 )?(m_fifo_count = 0):(m_fifo_count++);
	GPS_Time_Date_buf.sec_buf[0] = m_serialB_rev_fifo[m_fifo_count];
	(m_fifo_count == 1023 )?(m_fifo_count = 0):(m_fifo_count++);
	GPS_Time_Date_buf.sec_buf[1] = m_serialB_rev_fifo[m_fifo_count];
	(m_fifo_count == 1023 )?(m_fifo_count = 0):(m_fifo_count++);
	(m_fifo_count == 1023 )?(m_fifo_count = 0):(m_fifo_count++);		//小数点不获取，加1跳过
	GPS_Time_Date_buf.msec_buf[0] = m_serialB_rev_fifo[m_fifo_count];
	(m_fifo_count == 1023 )?(m_fifo_count = 0):(m_fifo_count++);
	GPS_Time_Date_buf.msec_buf[1] = m_serialB_rev_fifo[m_fifo_count];
	(m_fifo_count == 1023 )?(m_fifo_count = 0):(m_fifo_count++);
	GPS_Time_Date_buf.msec_buf[2] = m_serialB_rev_fifo[m_fifo_count];
	(m_fifo_count == 1023 )?(m_fifo_count = 0):(m_fifo_count++);
	return;
}
/********************************************************************
* @作者 :周炳权
* @功能 :GPS Date获取
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void GPS_Date_obtian( void )
{
	GPS_Time_Date_buf.day_buf[0] = m_serialB_rev_fifo[m_fifo_count];
	(m_fifo_count == 1023 )?(m_fifo_count = 0):(m_fifo_count++);
	GPS_Time_Date_buf.day_buf[1] = m_serialB_rev_fifo[m_fifo_count];
	(m_fifo_count == 1023 )?(m_fifo_count = 0):(m_fifo_count++);
	GPS_Time_Date_buf.month_buf[0] = m_serialB_rev_fifo[m_fifo_count];
	(m_fifo_count == 1023 )?(m_fifo_count = 0):(m_fifo_count++);
	GPS_Time_Date_buf.month_buf[1] = m_serialB_rev_fifo[m_fifo_count];
	(m_fifo_count == 1023 )?(m_fifo_count = 0):(m_fifo_count++);
	GPS_Time_Date_buf.year_buf[0] = m_serialB_rev_fifo[m_fifo_count];
	(m_fifo_count == 1023 )?(m_fifo_count = 0):(m_fifo_count++);
	GPS_Time_Date_buf.year_buf[1] = m_serialB_rev_fifo[m_fifo_count];
	(m_fifo_count == 1023 )?(m_fifo_count = 0):(m_fifo_count++);
	return;
}
/********************************************************************
* @作者 :周炳权
* @功能 :GPS协议分析
* @输入 :NONE
* @输出 :成功:1;失败:0;
********************************************************************/
uint8_t GPS_Protocol_analysis_GPRMC( void )
{
	GPS_INT_DIS();
	while (1)
	{
		if ( m_fifo_over == 1 )
		{
			comma_count = 0;
			m_fifo_count = 0;
			m_fifo_over = 0;
			os_driver_ioctl(m_GPS_dev_id, SET_EMPTY_FIFO, 0);
			GLOBAL_MEMCLR(m_serialB_rev_fifo, GPS_REV_FIFO_SIZE);
			GPS_INT_EN();
			return 0;
		}
		if ( Protocol_head_analysis() == 1 )
		{
			if ( m_serialB_rev_fifo[m_fifo_count] == ',' )
			{
				comma_count++;
				(m_fifo_count == 1023 )?(m_fifo_count = 0):(m_fifo_count++);
				GPS_Time_obtian();
				if ( m_serialB_rev_fifo[m_fifo_count] == ',' )
				{
					comma_count++;
					(m_fifo_count == 1023 )?(m_fifo_count = 0):(m_fifo_count++);
					while (1)
					{
						if ( m_serialB_rev_fifo[m_fifo_count] == ',' )
						{
							comma_count++;
						}
						(m_fifo_count == 1023 )?(m_fifo_count = 0):(m_fifo_count++);
						if ( comma_count == 9 )
							break;
					}
					GPS_Date_obtian();
					comma_count = 0;
					m_fifo_count = 0;
					m_fifo_over = 0;
					os_driver_ioctl(m_GPS_dev_id, SET_EMPTY_FIFO, 0);
					GLOBAL_MEMCLR(m_serialB_rev_fifo, GPS_REV_FIFO_SIZE);
					//GPS_INT_EN();
					if ( GPS_data_validity_analysis() == 0 )
					return 0;
					return 1;
				}
			}
		}
	}
}
/********************************************************************
* @作者 :周炳权
* @功能 :GPS驱动写
* @输入 :NONE
* @输出 :NONE
********************************************************************/
Bsize_t GPS_driver_write( uint8_t*buf, Bsize_t size , void*pos )
{
	Bsize_t r_size = size;
	r_size = os_driver_write(m_GPS_dev_id, buf, size, pos);
	return r_size;
}
/********************************************************************
* @作者 :周炳权
* @功能 :GPS驱动读
* @输入 :NONE
* @输出 :NONE
********************************************************************/
Bsize_t GPS_driver_read( uint8_t*buf, Bsize_t size , void*pos )
{
	Bsize_t r_size = size;
	r_size = os_driver_read(m_GPS_dev_id, buf, size, pos);
	return r_size;
}
/********************************************************************
* @作者 :周炳权
* @功能 :GPS接收初始化
* @输入 :NONE
* @输出 :NONE
********************************************************************/
static  void GPS_init( void )
{
	uint32_t baud 		= 4800;		//GPS波特率:4800bps;
	m_GPS_dev_id = serialB1_driver_register();
	if ( m_GPS_dev_id != INVALID_DEV_ID )
	{
		os_driver_init( m_GPS_dev_id );		//初始化串口驱动;	
	}
	os_driver_ioctl( m_GPS_dev_id, SET_SERIAL_BAUD, &baud );//波特率设置;
	return;
}
/********************************************************************
* @作者 :周炳权
* @功能 :GPS波特率设置
* @输入 :NONE
* @输出 :NONE
********************************************************************/
static  void GPS_set_serial_baud( uint32_t *m_baud )
{
	os_driver_ioctl( m_GPS_dev_id, SET_SERIAL_BAUD, m_baud );//波特率设置;
	return;
}
/********************************************************************
* @作者 :周炳权
* @功能 :GPS校验设置
* @输入 :NONE
* @输出 :NONE
********************************************************************/
static  void GPS_set_serial_check( uint8_t *m_baud )
{
	os_driver_ioctl( m_GPS_dev_id, SET_SERIAL_CHECK, m_baud );//波特率设置;
	return;
}
/********************************************************************
* @作者 :周炳权
* @功能 :缓存清零
* @输入 :NONE
* @输出 :NONE
********************************************************************/
static  void GPS_set_clear_buf( void )
{
	os_driver_ioctl( m_GPS_dev_id, SET_EMPTY_FIFO, 0 );
	return;
}
/********************************************************************
* @作者 :周炳权
* @功能 :GPS控制
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void GPS_driver_ioctl( uint8_t cmd, void*arg )
{
	uint8_t	analysis_state = 0;
	switch ( cmd )
	{
		case GPS_START:
			GPS_init();
			break;
		case GPS_ANALYSIS_START:
			analysis_state = GPS_Protocol_analysis_GPRMC();
			*(uint8_t *)arg = analysis_state;
			os_driver_uninstall(m_GPS_dev_id);
			break;
		case GPS_TIME_GET:
			*(GPS_Time_Date_t *)arg = GPS_Time_Date_buf;
			break;
		case GPS_END:
			os_driver_uninstall(m_GPS_dev_id);
			break;
		case GPS_CLOSE:
			RM_serial_OPEN();
			break;
		case GPS_OPEN:
			GPS_serial_OPEN();
			break;
		case GPS_SET_BAUD:
			GPS_set_serial_baud(arg);
			break;
		case GPS_SET_CHECK:
			GPS_set_serial_check(arg);
			break;
		case GPS_EMPTY_FIFO:
			GPS_set_clear_buf();
			break;
		default:
			break;
	}
	return;
}
/********************************************************************
* @作者 :周炳权
* @功能 :GPS驱动初始化
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void GPS_driver_init( void )
{
	GLOBAL_MEMCLR(&GPS_Time_Date_buf, sizeof(GPS_Time_Date_t));
	return;
}
/********************************************************************
* @作者 :周炳权
* @功能 :GPS驱动卸载
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void GPS_driver_uninstall( void )
{
	os_driver_uninstall(m_GPS_dev_id);
	return;
}
/********************************************************************
 * @作者: 周炳权
 * @功能: GPS驱动注册
 * @输入:none
 * @输出:NONE
********************************************************************/
dev_id_t GPS_driver_register( void )
{
	uint8_t ret;
	os_dirver_operate_t temp;
	temp.os_driver_init    = GPS_driver_init;
	temp.os_driver_ioctl  = GPS_driver_ioctl;
	temp.os_driver_read  = GPS_driver_read;
	temp.os_driver_write = GPS_driver_write;
	temp.os_driver_uninstall = GPS_driver_uninstall;
	ret = os_driver_add( &temp, "GPS" );
	return ret;
}

#pragma DATA_SECTION (GPS_list,"DR_LIST_ADDR")
const DR_member_t GPS_list[]=
{
	GPS_driver_register,
	0x5a5a
};

/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:时钟芯片驱动程序(ht1380)
* 创建时间:2010-08-06
* 所属公司 :科荟
* 文件名称:Ht1380driver.c
* 创建人 :揭成
* 功能描述:提供获取时间，重设时间底层支持
*
*----------------------------------------------------------------
修改记录:

修改目的:

修改日期:

修改人:

*********************************************************************************************************
*/#include "../Osdriver.h"

/*ht1380 通讯操作宏*/
#define DELAY_2NOP()  				{asm(" nop") ;asm(" nop");}

#define DSP_CLEAN_HT1380_RST()  	{GpioDataRegs.GPBCLEAR.bit.GPIO48=1; DELAY_2NOP();}
#define DSP_SET_HT1380_RST()  		{GpioDataRegs.GPBSET.bit.GPIO48  =1; DELAY_2NOP();}
#define DSP_CLEAN_HT1380_DIO()  	{GpioDataRegs.GPBCLEAR.bit.GPIO32=1; DELAY_2NOP();}
#define DSP_SET_HT1380_DIO()  		{GpioDataRegs.GPBSET.bit.GPIO32=1;   DELAY_2NOP();}
#define DSP_CLEAN_HT1380_SCLK() 	{GpioDataRegs.GPBCLEAR.bit.GPIO33=1; DELAY_2NOP();}
#define DSP_SET_HT1380_SCLK()  		{GpioDataRegs.GPBSET.bit.GPIO33=1;   DELAY_2NOP();}
#define DSP_READ_HT1380_DIO()   	 GpioDataRegs.GPBDAT.bit.GPIO32

//gpio设置为输入或输出
#define DSP_GPIO_OUTPUT_HT1380_RST()  GpioCtrlRegs.GPBDIR.bit.GPIO48=1
#define DSP_GPIO_OUTPUT_HT1380_DIO()  GpioCtrlRegs.GPBDIR.bit.GPIO32=1
#define DSP_GPIO_inPUT_HT1380_DIO()   GpioCtrlRegs.GPBDIR.bit.GPIO32=0
#define DSP_GPIO_OUTPUT_HT1380_SCLK() GpioCtrlRegs.GPBDIR.bit.GPIO33=1

void global_strcat( char*pstr1, char*pstr2 );
/********************************************************************
 * @创建人:揭成
 * @功能:向1380写一个字节数据
 * @输入:NONE
 * @输出:NONE
********************************************************************/
static void ht1380_driver_write_byte( uint8_t send_data )
{
	uint8_t dd_temp, i;
	EALLOW;
	DSP_GPIO_OUTPUT_HT1380_RST();
	DSP_GPIO_OUTPUT_HT1380_DIO();
	DSP_GPIO_OUTPUT_HT1380_SCLK();
	EDIS;
	for ( i = 0;i < 8;i++ )
	{
		DSP_CLEAN_HT1380_SCLK();
		dd_temp = send_data & 0x01;
		if ( dd_temp == 1 )
		{
			DSP_SET_HT1380_DIO();
		}
		else
		{
			DSP_CLEAN_HT1380_DIO();
		}
		DSP_SET_HT1380_SCLK();
		send_data = send_data >> 1;
	}
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能:向1380读一个字节数据
 * @输入:	NONE
 * @输出:	NONE
********************************************************************/
static uint8_t ht1380_driver_read_byte( void )
{
	uint8_t re_data;
	Bsize_t bit_index;
	EALLOW;
	DSP_GPIO_OUTPUT_HT1380_RST();
	DSP_GPIO_inPUT_HT1380_DIO();
	DSP_GPIO_OUTPUT_HT1380_SCLK();
	EDIS;
	re_data = 0;
	for ( bit_index = 0;bit_index < 8;bit_index++ )
	{
		DSP_SET_HT1380_SCLK();        /*产生一个下降沿*/
		DSP_SET_HT1380_SCLK();
		DSP_SET_HT1380_SCLK();
		DSP_CLEAN_HT1380_SCLK();
		DSP_CLEAN_HT1380_SCLK();
		DSP_CLEAN_HT1380_SCLK();
		re_data = re_data >> 1;
		if ( DSP_READ_HT1380_DIO()!=0)
		{
			re_data |= 0x80;
		}
		else
		{
			re_data &= 0x7f;
		}
		DSP_SET_HT1380_SCLK();
	}
	return re_data;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 根据命令向1380读数据
 * @输入:	NONE
 * @输出:	NONE
********************************************************************/
static uint8_t ht1380_driver_cmd_read( uint8_t cmd )
{
	uint8_t Rdata = 0;
	DSP_CLEAN_HT1380_SCLK();
	DSP_CLEAN_HT1380_RST();
	DSP_SET_HT1380_RST();
	ht1380_driver_write_byte( cmd );
	Rdata = ht1380_driver_read_byte();
	DSP_CLEAN_HT1380_RST();
	DSP_SET_HT1380_SCLK();
	return( Rdata );
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 根据命令向1380写数据
 * @输入:	NONE
 * @输出:	NONE
********************************************************************/
static void ht1380_driver_cmd_write( uint8_t cmd, uint8_t Sdata )
{
	DSP_CLEAN_HT1380_SCLK();
	DSP_CLEAN_HT1380_RST();
	DSP_SET_HT1380_RST();
	ht1380_driver_write_byte( cmd );
	ht1380_driver_write_byte( Sdata );
	DSP_CLEAN_HT1380_RST();
	DSP_SET_HT1380_SCLK();
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 时间的合法性判断
 * @输入:none
 * @输出:NONE
********************************************************************/
static BOOL ht1380_driver_time_fawful( time_value_t*tvalue )
{
	Bsize_t ret = TURE;
	if ( tvalue->year < 1 || tvalue->year > 0x99 )
	{
		tvalue->year = 8;
		ret = FAIL;
	}
	if ( tvalue->month < 1 || tvalue->month > 0x12 )
	{
		tvalue->month = 0x11;
		ret = FAIL;
	}
	if ( tvalue->day < 1 || tvalue->day > 0x31 )
	{
		tvalue->day = 0x28;
		ret = FAIL;
	}
	if ( tvalue->hour > 0x24 )
	{
		tvalue->hour = 8;
		ret = FAIL;
	}
	if ( tvalue->min > 0x59 )
	{
		tvalue->min = 0x30;
		ret = FAIL;
	}
	if ( tvalue->sec > 0x59 )
	{
		tvalue->sec = 0x16;
		ret = FAIL;
	}
	return ret;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 保存当前时间
 * @输入:	NONE
 * @输出:	NONE
********************************************************************/
static void ht1380_driver_time_save( time_value_t*pbuf )
{
	ht1380_driver_time_fawful( pbuf );
	ht1380_driver_cmd_write( 0x8e, 0 ) ;			/*写保护寄存器0x8e=0,写允许*/
	ht1380_driver_cmd_write( 0x80, pbuf->sec ) ;	/*秒*/
	ht1380_driver_cmd_write( 0x82, pbuf->min ) ;	/*写分*/
	ht1380_driver_cmd_write( 0x84, pbuf->hour ) ;	/*写时*/
	ht1380_driver_cmd_write( 0x86, pbuf->day ) ;	/*写日*/
	ht1380_driver_cmd_write( 0x88, pbuf->month );	/*写月*/
	ht1380_driver_cmd_write( 0x8c, pbuf->year ) ;	/*写年*/
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 获取当前时间
 * @输入:	NONE
 * @输出:	NONE
********************************************************************/
static void ht1380_driver_time_obtain( time_value_t*pbuf )
{
	pbuf->sec   = ht1380_driver_cmd_read( 0x81 ) ;//读秒
	pbuf->min   = ht1380_driver_cmd_read( 0x83 ) ;//读分
	pbuf->hour  = ht1380_driver_cmd_read( 0x85 ) ;//读时

	pbuf->day   = ht1380_driver_cmd_read( 0x87 ) ;//读日
	pbuf->week  = ht1380_driver_cmd_read( 0x8B ) ;//读星期
	pbuf->month = ht1380_driver_cmd_read( 0x89 ) ;//读月
	pbuf->year  = ht1380_driver_cmd_read( 0x8D ) ;//读年
	return;
}

/********************************************************************
 * @创建人: 揭成
 * @功能: 字节数字转换成字符
 * @输入:none
 * @输出:NONE
********************************************************************/
static void ht1380_driver_timebyte_NtoC( uint8_t*buf, uint8_t tvalue )
{
	buf[0] = ( char )(( tvalue >> 4 ) + 48 );
	buf[1] = ( char )(( tvalue & 0x0f ) + 48 );
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 字节字符转换成数字
 * @输入:none
 * @输出:NONE
********************************************************************/
static uint8_t ht1380_driver_timebyte_CtoN( uint8_t*buf )
{
	uint8_t num_h;
	uint8_t num_l;
	num_h =	( GLOBAL_CTON( buf[0] ) << 4 );
	num_l =	( GLOBAL_CTON( buf[1] ) );
	return( num_h + num_l );
}
static void ht1380_driver_timebuf_NtoC( time_string_t*pdtime, time_value_t*pvtime )
{
	ht1380_driver_timebyte_NtoC(( uint8_t* )pdtime->year_buf, pvtime->year );	/*年*/
	ht1380_driver_timebyte_NtoC(( uint8_t* )pdtime->month_buf, pvtime->month );/*月*/
	ht1380_driver_timebyte_NtoC(( uint8_t* )pdtime->day_buf, pvtime->day ); 	/*日*/
	ht1380_driver_timebyte_NtoC(( uint8_t* )pdtime->hour_buf, pvtime->hour );	/*时*/
	ht1380_driver_timebyte_NtoC(( uint8_t* )pdtime->min_buf, pvtime->min );		/*分*/
	ht1380_driver_timebyte_NtoC(( uint8_t* )pdtime->sec_buf, pvtime->sec );		/*秒*/
	ht1380_driver_timebyte_NtoC(( uint8_t* )pdtime->week_buf, pvtime->week );	/*星期*/
	return;
}
static void ht1380_driver_timebuf_CtoN( time_string_t*pdtime, time_value_t*pvtime )
{
	pvtime->year = ht1380_driver_timebyte_CtoN(( uint8_t* )pdtime->year_buf );
	pvtime->month = ht1380_driver_timebyte_CtoN(( uint8_t* )pdtime->month_buf );
	pvtime->day = ht1380_driver_timebyte_CtoN(( uint8_t* )pdtime->day_buf );
	pvtime->hour = ht1380_driver_timebyte_CtoN(( uint8_t* ) pdtime->hour_buf );
	pvtime->min = ht1380_driver_timebyte_CtoN(( uint8_t* )pdtime->min_buf );
	pvtime->sec = ht1380_driver_timebyte_CtoN(( uint8_t* ) pdtime->sec_buf );
	pvtime->week= ht1380_driver_timebyte_CtoN(( uint8_t* ) pdtime->week_buf );
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 获取当前时间,按字符串输出
 * @输入:	NONE
 * @输出:	NONE
********************************************************************/
static void ht1380_driver_time_string_obtain( time_string_t*pbuf )
{
	time_value_t  temp;
	ht1380_driver_time_obtain( &temp );
	GLOBAL_MEMCLR( pbuf, sizeof( time_string_t ) );
	ht1380_driver_timebuf_NtoC( pbuf, &temp );
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 获取标准格式时间字符串
 * @输入:	NONE
 * @输出:	NONE
********************************************************************/
static void ht1380_driver_time_DISstring_obtain( char*pbuf )
{
	time_string_t  tmr_buf;
	ht1380_driver_time_string_obtain( &tmr_buf );
	global_memcpy( pbuf, tmr_buf.year_buf, 2 );
	global_strcat( pbuf, "-" );
	global_strcat( pbuf, tmr_buf.month_buf );
	global_strcat( pbuf, "-" );
	global_strcat( pbuf, tmr_buf.day_buf );
	global_strcat( pbuf, " " );
	global_strcat( pbuf, tmr_buf.hour_buf );
	global_strcat( pbuf, ":" );
	global_strcat( pbuf, tmr_buf.min_buf );
	global_strcat( pbuf, ":" );
	global_strcat( pbuf, tmr_buf.sec_buf );
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 获取当前时间,按字符串保存
 * @输入:	NONE
 * @输出:	NONE
********************************************************************/
static void ht1380_driver_time_string_save( time_string_t*pbuf )
{
	time_value_t  temp;
	ht1380_driver_timebuf_CtoN( pbuf, &temp );
	ht1380_driver_time_save( &temp );
	return;
}

/********************************************************************
 * @创建人: 揭成
 * @功能: 驱动底层读入口
 * @输入:	NONE
 * @输出:	NONE
********************************************************************/
static Bsize_t ht1380_driver_read( uint8_t*buf, Bsize_t size , void*pos )
{
	return 0;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 驱动模块控制
 * @输入:	NONE
 * @输出:	NONE
********************************************************************/
static void  ht1380_driver_ioctl( uint8_t cmd, void*arg )
{
	switch ( cmd )
	{
	case TMR_CMD_VALUE_SAVE:
		{
			ht1380_driver_time_save(( time_value_t* )arg );
			break;
		}
	case TMR_CMD_TIME_VALUE_GET:
		{
			ht1380_driver_time_obtain(( time_value_t* )arg );
			break;
		}
	case TMR_CMD_TIME_STRING_GET:
		{
			GLOBAL_MEMCLR( arg, sizeof( time_string_t ) );
			ht1380_driver_time_string_obtain(( time_string_t* )arg );
			break;
		}
	case  TMR_CMD_TIME_DISSTRING_GET:
		{
			ht1380_driver_time_DISstring_obtain(( char* )arg );
			break;
		}
	case TMR_CMD_TIME_STRING_SAVE:
		{
			ht1380_driver_time_string_save(( time_string_t* )arg );
			break;
		}
	default:
		break;
	}
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 驱动底层写入口
 * @输入:	NONE
 * @输出:	NONE
********************************************************************/
static Bsize_t ht1380_driver_write( uint8_t*buf, Bsize_t size , void*pos )
{
	return 0;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 驱动卸载
 * @输入:	NONE
 * @输出:	NONE
********************************************************************/
static void ht1380_driver_uninstall( void )
{
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 驱动初始化
 * @输入:	NONE
 * @输出:	NONE
********************************************************************/
static void ht1380_driver_init( void )
{
	EALLOW;
	DSP_GPIO_OUTPUT_HT1380_RST();
	DSP_GPIO_OUTPUT_HT1380_DIO();
	DSP_GPIO_OUTPUT_HT1380_SCLK();
	EDIS;
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能:驱动注册
 * @输入:none
 * @输出:NONE
********************************************************************/
dev_id_t timer_driver_register( void )
{
	uint8_t ret;
	os_dirver_operate_t temp;
	temp.os_driver_init   =  ht1380_driver_init;
	temp.os_driver_ioctl  = ht1380_driver_ioctl;
	temp.os_driver_read  = ht1380_driver_read;
	temp.os_driver_write = ht1380_driver_write;
	temp.os_driver_uninstall = ht1380_driver_uninstall;
	ret = os_driver_add( &temp, "TIME" );
	return ret;
}
/*driver AUTO Register*/
#pragma DATA_SECTION (timer_list,"DR_LIST_ADDR")
const DR_member_t timer_list[]=
{
	timer_driver_register,
	0x5a5a
};


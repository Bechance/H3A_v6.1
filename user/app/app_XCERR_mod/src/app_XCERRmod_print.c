/********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:XCERR
* 创建时间:2014-02-24
* 所属公司 :科荟
* 文件名称:app_XCERRmod_print.c
* 创建人 :阳永锋
* 功能描述:打印校验记录
******************************************************************************************/
#include    "app_XCerrmod_inside.h"
#include 	<string.h>

#define XCERR_PRINT_DEBUG 0

/********************************************************************
 * @作者 :阳永锋
 * @功能 :printer模块打印数据
 * @输入 :打印单个字符
 * @输出 :NONE
********************************************************************/
static void _print_single_char( uint8_t code )
{
	os_driver_write( m_XCERRmod_print_dev_id, &code, 1, 0 );
	return;
}

/********************************************************************
 * @作者 :阳永锋
 * @功能 :字符串打印
 * @输入 :str:要打印的字符串
 * @输出 :NONE
********************************************************************/
static void _print_string( char* str)
{
	Bsize_t offset = 0;
	for ( offset = 0;offset < global_strlen( str );offset++ )
	{
		_print_single_char( str[offset] );
	}
	return;
}

/********************************************************************
 * @作者 :阳永锋
 * @功能 :浮点数打印
 * @输入 :浮点数(fsrc)
 * @输出 :NONE
********************************************************************/
static void _print_float( float fsrc )
{
	Bsize_t offset = 0;
	char     str[15] = "";
	global_ftoa( fsrc, str, 5 );
	for ( offset = 0;offset < global_strlen( str );offset++ )
	{
		_print_single_char( str[offset] );
	}
	return;
}

/********************************************************************
 * @作者 :范文斌
 * @功能 :printer模块打印设置
 * @输入 :命令(cmd),命令1(cmd1),数据(data)
 * @输出 :NONE
********************************************************************/
static void _send_print_cmd( uint8_t cmd, uint8_t cmd1, uint8_t data )
{
	_print_single_char( cmd );
	_print_single_char( cmd1 );
	_print_single_char( data );
	return;
}

/********************************************************************
 * @作者 :范文斌
 * @功能 :printer模块打印设置
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
static void _send_print_cmd_1( uint8_t cmd, uint8_t cmd1,
		uint8_t data, uint8_t data1 )
{
	_print_single_char( cmd );
	_print_single_char( cmd1 );
	_print_single_char( data );
	_print_single_char( data1 );
	return;
}

/********************************************************************
 * @作者 :范文斌
 * @功能 :打印value个回车
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
static void  _send_print_enter_cmd( Bsize_t value )
{
	Bsize_t offset;
	for ( offset = 0;offset < value;offset++ )
	{
		_print_single_char( 0x0d );
	}	
	return;
}


/********************************************************************
 * @作者 :范文斌
 * @功能 :打印n个字符空格
 * @输入 :要打印空格个数(n)
 * @输出 :NONE
********************************************************************/
static void  _send_print_space_cmd( uint8_t n )
{
	_send_print_cmd_1( 0x1b, 0x66, 0, n );
	return;
}

/********************************************************************
 * @作者 :阳永锋
 * @功能 :设置打印机工作方式
 * @输入 :
 * @输出 :NONE
 * @命令注释: 1b,40:打印机初始化; 0x1b,0x63,0x00:禁止反向打印
 *            0x1b,0x31,0x0:设置行间距为0; 0x1b,0x55,0x1横向放大1倍
 *            0x1b,0x56,1:纵向放大1倍; 0x1b,0x4b: 设置点击打印
********************************************************************/
static uint8_t _printer_mode_set(char *pdes,uint16_t lcd_width,uint8_t w_times,uint8_t h_times)
{
	unsigned char Cmd_init[12]={0x1b, 0x40, 0x1b, 0x63, 0x00, 0x1b, 0x31, 0x0, 0x1b, 0x57, 0x1};	 
	unsigned char cmd_Row_timers[3]={0x1b,0x55,1};
	unsigned char cmd_Col_timers[3]={0x1b,0x56,1};
	unsigned char lattice[4]={0x1b,0x4b};//点阵打印 
	uint8_t pos = 0;
	cmd_Row_timers[2] = w_times;		//设置打印放大倍数
	cmd_Col_timers[2] = h_times;
	lattice[2] = lcd_width%256;			//设置打印宽度
	lattice[3] = lcd_width/256;
	
	global_memcpy(pdes, Cmd_init, 11);
	pos += 11;
	global_memcpy(pdes+pos, cmd_Row_timers,3);
	pos += 3;
	global_memcpy(pdes+pos, cmd_Col_timers, 3);
	pos += 3;
	global_memcpy(pdes+pos, lattice, 4);
	pos  += 4;
	return pos;
}




/********************************************************************
 * @作者 :阳永锋
 * @功能 :将一列中的8个像素点转成一个点阵打印数值
 * @输入 :
 * @输出 :NONE
 * @备注: 0x2145为六解图圆圈前景色
********************************************************************/
static uint8_t _8LCDpixel_to_1Dotpixel(uint16_t *lcd_buf,uint16_t y_start,uint16_t y_width)
{
	uint16_t 	rCOR = 0;
	uint8_t		wCOR = 0;
	uint8_t 	y_offset = 0;
	uint16_t	*ptemp = lcd_buf;
	uint16_t	offset = 0;
	
	for ( y_offset = 0;y_offset < 8;y_offset++ )
	{
		wCOR = wCOR << 1;
		offset = y_start + y_offset*y_width;
		rCOR = *(ptemp + offset);
		if((rCOR  == C_GUI_YELLOW) ||(rCOR  == C_GUI_GREEN) || 	\
		   (rCOR  == C_GUI_RED) ||(rCOR  == C_GUI_WHITE)||(rCOR==0x2145))
			wCOR|= 1;
	}
	return( wCOR );
}


static uint16_t _8COL_LCDpicdata_to_Dotpicdata(uint8_t*  print_buf, uint16_t*lcd_buf, uint16_t y_width )
{
	uint16_t	size = 0;
	uint16_t 	offset = 0;
	for(offset = 0; offset < y_width; offset++)
	{
		print_buf[offset] = _8LCDpixel_to_1Dotpixel(lcd_buf,y_width-offset,y_width);
	}
	size = offset;
	return size;
}








static uint8_t _fill_dot_pic_print_setting(uint8_t*pdes,uint8_t w_times,uint8_t h_times)
{
	uint8_t len = 0;
	uint8_t	buf_cmd[12] 	 	= {0x1b, 0x40, 0x1b, 0x63, 0x00, 0x1b, 0x31, 0x0, 0x1b, 0x57, 0x1};//缓冲器的容量和打印机初始化内容
	uint8_t strRow_cmd[3] 	 	= {0x1b, 0x55, 0};
	uint8_t strCol_cmd[3]    	= {0x1b, 0x56, 0};
	//uint8_t hanzimod_cmd[3]  	= {0x1b, 0x38, 4};
	//uint8_t vertial_mode[3]  	= {0x1c,0x49, 0x1};
	uint8_t latitice_cmd[4]	 	= {0x1b, 0x4b,0x00,0x00};//点阵打印
	
	strRow_cmd[2] = w_times;
	strCol_cmd[2] = h_times;
	
	global_memcpy(pdes,buf_cmd,8);
	len += 8;
	global_memcpy(pdes+len,strRow_cmd,3);
	len += 3;
	global_memcpy(pdes+len,strCol_cmd,3);
	len += 3;
	global_memcpy(pdes+len,latitice_cmd,4);
	len += 4;
	return len;
}

/********************************************************************
* @作者 :阳永锋
* @功能 :打印机打印汉字参数设置
* @输入 : 参数缓冲指针，w_times:横向放大,h_times:纵向放大(进纸方向)
* @输出 :NONE
********************************************************************/
static uint8_t _fill_chinese_print_setting(uint8_t*pdes,uint8_t zoom_times)
{
	uint8_t len = 0;
	uint8_t	buf_cmd[12] 	 	= {0x1b, 0x40, 0x1b, 0x63, 0x00, 0x1b, 0x31, 0x3};//缓冲器的容量和打印机初始化内容
	uint8_t set_chinese_cmd[2]	= {0x1C,0x26};		//选择中文打印，国标一，二级字库打印
	uint8_t zoom_cmd[3] 		= {0x1c, 0x57, 2};
	
	zoom_cmd[2] = zoom_times;
	
	global_memcpy(pdes+len,buf_cmd,8);
	len += 8;
	global_memcpy(pdes+len,zoom_cmd,3);
	len += 3;
	global_memcpy(pdes+len,set_chinese_cmd,2);
	len += 2;
	return len;
}



static void _Dotpicdata_to_LCDpicdata(uint16_t *lcd_buf, uint8_t *print_buf, uint16_t y_witdh)
{
	uint16_t i = 0;
	uint8_t  y_offset = 0;
	uint16_t offset = 0;
	
	for(offset = 0; offset < y_witdh; offset++)
	{
		for(i = 0x80,y_offset=0; i != 0; i >>= 1,y_offset++)
		{
			if(print_buf[offset] & i)
			{
				lcd_buf[y_witdh-offset+y_offset*y_witdh] = C_GUI_WHITE;
			}
			else
			{
				lcd_buf[y_witdh-offset+y_offset*y_witdh] = C_GUI_BlACK;
			}
		}
	}
	
}

static void _print_line(uint16_t line_length)
{
	const uint8_t line_data = 0x0f;
	uint16_t mode_set_len = 0;
	uint8_t  *print_buf = PNULL;
	uint16_t write_len_total = 0;
	uint16_t data_len = 0;
	const	uint8_t	LF_cmd = 0x0a;
	
	print_buf = (uint8_t*)app_malloc(XCERRMOD_LCD_WIDTH*1);
	if(!print_buf) return;
	
	mode_set_len = _fill_dot_pic_print_setting(print_buf, 1,  1);
	global_memset(print_buf+mode_set_len, line_data, line_length);
	data_len = line_length;
	write_len_total = data_len + mode_set_len;
	print_buf[mode_set_len-2] = data_len%256;		//填充点阵长度
	print_buf[mode_set_len-1] = data_len/256;
	print_buf[write_len_total++] = LF_cmd;
	os_driver_write( m_XCERRmod_print_dev_id,print_buf,write_len_total, 0 );	
	app_free(print_buf);
	return;
}

static void _print_setting_and_LJT(Errsave_t*pD,uint8_t w_times,uint8_t h_times)
{
	uint16_t mode_set_len = 0;
	uint8_t *print_buf = PNULL;
	uint16_t *lcd_buf = PNULL;
	uint16_t x_loop = 0;
	uint16_t xpos = 0;
	uint16_t data_len = 0;
	uint16_t write_len_total = 0;
	const	uint8_t	LF_cmd = 0x0a;
	
	print_buf = (uint8_t*)app_malloc(XCERRMOD_LCD_WIDTH*2);
	lcd_buf  = (uint16_t*)app_malloc(XCERRMOD_LCD_WIDTH*9);
	if(print_buf == PNULL || lcd_buf== PNULL)
	{
		app_free(print_buf);
		app_free(lcd_buf);
		return;
	}
	GLOBAL_MEMCLR(print_buf, XCERRMOD_LCD_WIDTH*2);
	GLOBAL_MEMCLR(lcd_buf, XCERRMOD_LCD_WIDTH*9);
	mode_set_len = _fill_dot_pic_print_setting(print_buf,  w_times,  h_times);
	//打印参数设置及六角图
	x_loop =  640/8;
	for(xpos = x_loop; xpos > 0; xpos--)
	{
		GLOBAL_MEMCLR(lcd_buf, XCERRMOD_LCD_WIDTH*9);
		GUI_read_block((xpos-1)*8,33,xpos*8,283,lcd_buf);
		data_len = _8COL_LCDpicdata_to_Dotpicdata(print_buf+mode_set_len,lcd_buf,283-33);
		#if XCERR_PRINT_DEBUG			//开启回显调试测试
		_Dotpicdata_to_LCDpicdata(lcd_buf,print_buf+mode_set_len,283-33);
		GUI_write_block((xpos-1)*8,33,xpos*8,283,lcd_buf,0);
		#endif
		write_len_total = data_len + mode_set_len;
		print_buf[mode_set_len-2] = data_len%256;		//填充点阵长度
		print_buf[mode_set_len-1] = data_len/256;
		print_buf[write_len_total++] = LF_cmd;
		
		os_driver_write( m_XCERRmod_print_dev_id,print_buf,write_len_total, 0 );
	}
	_print_line(250);
	app_free(print_buf);
	app_free(lcd_buf);
	return;
}


static void _print_DL_result(Errsave_t*pD,uint8_t w_times,uint8_t h_times)
{
	uint16_t mode_set_len = 0;
	uint8_t *print_buf = PNULL;
	uint16_t *lcd_buf = PNULL;
	uint16_t x_loop = 0;
	uint16_t xpos = 0;
	uint16_t data_len = 0;
	uint16_t write_len_total = 0;
	const	uint8_t	LF_cmd = 0x0a;
	
	print_buf = (uint8_t*)app_malloc(XCERRMOD_LCD_WIDTH*2);
	lcd_buf  = (uint16_t*)app_malloc(XCERRMOD_LCD_WIDTH*9);
	if(print_buf == PNULL || lcd_buf== PNULL)
	{
		app_free(print_buf);
		app_free(lcd_buf);
		return;
	}
	GLOBAL_MEMCLR(print_buf, XCERRMOD_LCD_WIDTH*2);
	GLOBAL_MEMCLR(lcd_buf, XCERRMOD_LCD_WIDTH*9);
	mode_set_len = _fill_dot_pic_print_setting(print_buf,  w_times,  h_times);
	//打印电力参数测试结果表格
	x_loop =  640/8;
	for(xpos = x_loop; xpos > 0; xpos--)
	{
		GLOBAL_MEMCLR(lcd_buf, XCERRMOD_LCD_WIDTH*8);
		GUI_read_block((xpos-1)*8,279,xpos*8,442,lcd_buf);
		data_len = _8COL_LCDpicdata_to_Dotpicdata(print_buf+mode_set_len,lcd_buf,442-279);
		#if XCERR_PRINT_DEBUG //开启回显调试测试
		_Dotpicdata_to_LCDpicdata(lcd_buf,print_buf+mode_set_len,442-279);
		GUI_write_block((xpos-1)*8,279,xpos*8,442,lcd_buf,0);
		#endif
		write_len_total = data_len + mode_set_len;
		print_buf[mode_set_len-2] = data_len%256;		//填充点阵长度
		print_buf[mode_set_len-1] = data_len/256;
		print_buf[write_len_total++] = LF_cmd;
		os_driver_write( m_XCERRmod_print_dev_id,print_buf,write_len_total, 0 );
	}
	_print_line(320);
	app_free(print_buf);
	app_free(lcd_buf);
	return;
}

static uint8_t _copy_fixed_size(char *pdes,char *psrc, uint8_t size)
{
	uint8_t offset = 0;
	uint8_t real_copy_size = 0;
	
	for(offset = 0; offset < size; )
	{
		if( (psrc[offset] & 0xff) >= 0x81)	//汉字
		{
			if(offset + 2 > size)
				break;
			else
			{
				global_memcpy(pdes+offset, psrc+offset,2);
				offset += 2;
			}
		}
		else
		{
			global_memcpy(pdes+offset, psrc+offset,1);
			offset += 1;
		}
	}
	pdes[offset] = ASCII_NULL;
	real_copy_size = offset;
	return real_copy_size;
}

static void _print_worker(Errsave_t*pD)
{
	char *p = PNULL;
	uint16_t data_len = 0;
	uint8_t offset = 0;
	char 	name1[31] = "";
	char 	name2[31] = "";
	//打印校验员
	p = pD->DL_app_user_info.worker;
	data_len = global_strlen(p);
	if(data_len > 16)
	{
		offset = _copy_fixed_size(name1, p, 16);
		global_strcpy(name2, p+offset);

		_send_print_space_cmd(9);
		_print_string(name2);
		_send_print_enter_cmd(2);
		_print_string((char*)m_XCERRmod_MSG_46[g_language]);
		_print_string(name1);
	}
	else
	{
		global_memcpy(name1, p, data_len);
		_print_string((char*)m_XCERRmod_MSG_46[g_language]);
		_print_string(name1);
	}
	_send_print_enter_cmd(2);
	return;
}
/********************************************************************
* @作者 :阳永锋
* @功能 :打印时间
* @输入 :pD:记录数据指针
* @输出 :NONE
* @备注: 因为时间格式由 2014-01-01  15:11:00改为2014-01-01 15:11:00
*        即中间空格由2个改为1个,因此特殊处理了一下，以免不兼容
********************************************************************/
static void _print_time(Errsave_t*pD)
{
	char date[13] = "";
	char time[13] = "";
	char temp[40] = "";

	GLOBAL_MEMCLR(temp, sizeof(temp));
	global_memcpy(temp,pD->DL_app_user_info.time_buf,20);
	global_string_filter(temp, ' ');
	global_memcpy(date, temp, 10);
	date[10] = ASCII_NULL;
	global_memcpy(time, temp+10, 8);
	time[8] = ASCII_NULL;

	//打印时间
	_send_print_enter_cmd(2);
	_print_string((char*)"*********************");
	_send_print_enter_cmd(1);
	_send_print_space_cmd(12);
	_print_string(time);
	_send_print_enter_cmd(1);
	_print_string((char*)m_XCERRmod_MSG_47[g_language]);
	_print_string(date);
	_send_print_enter_cmd(1);
	_print_string((char*)"*********************");
	_send_print_enter_cmd(1);
	return;
}

void _print_user_info(Errsave_t*pD,uint8_t zoom_times)
{
	uint16_t mode_set_len = 0;
	uint8_t *print_buf = PNULL;
	
	
	print_buf = print_buf = (uint8_t*)app_malloc(XCERRMOD_LCD_WIDTH*1);
	if(!print_buf) return;
	GLOBAL_MEMCLR(print_buf, XCERRMOD_LCD_WIDTH*1);
	
	mode_set_len = _fill_chinese_print_setting(print_buf,zoom_times);
	os_driver_write( m_XCERRmod_print_dev_id,print_buf,mode_set_len, 0 );
	_send_print_enter_cmd(1);
	//打印汉字: 客户签字
	_send_print_enter_cmd(1);
	_print_string((char*)m_XCERRmod_MSG_45[g_language]);
	_send_print_enter_cmd(4);
	_print_worker(pD);
	_print_time(pD);
	app_free(print_buf);
	return;
}



/********************************************************************
* @作者 :阳永锋
* @功能 :打印主函数
* @输入 :pD:记录数据指针
* @输出 :NONE
********************************************************************/
void app_XCERRmod_record_print(Errsave_t*pD)
{
	uint8_t init[2]={0x1b,0x40};
	os_driver_write( m_XCERRmod_print_dev_id,init,2, 0 );
	msleep(1);
	_print_user_info(pD,2);
	_print_DL_result(pD,2,1);
	_print_setting_and_LJT(pD,1,1);
	
	return;
}

/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:串口驱动
* 创建时间:2010-07-14
* 所属公司 :科荟
* 文件名称:serialdriver.c   //for 28335
* 创建人 :揭成
* 功能描述:提供串口操作API接口，创建串口底层FIFO 128BYTES
*----------------------------------------------------------------
修改记录:

修改日期:

修改人:

*********************************************************************************************************
*/
#include "../Osdriver.h"
#include <ctype.h>


#define 	SELECT_TOUCH  0
#define 	SELECT_WIFI   1

#define     SERIALB_INT_EN()  {PieCtrlRegs.PIEIER9.bit.INTx3 = 1;}
#define     SERIALB_INT_DIS() {PieCtrlRegs.PIEIER9.bit.INTx3 =0;}

//#define 	BLUETOOTH_DEFAULT_PIN_CODE	"10010\r"
#define 	BLUETOOTH_DEFAULT_PIN_CODE	"10010"
//#define		BLUETOOTH_DEFAULT_ADDR		"00:06:42:8A:03:21\r"
#define		BLUETOOTH_DEFAULT_ADDR		"AC:F7:F3:32:A7:41"

#define 	BT_MASTER_ROLE 1
#define		BT_SLAVE_ROLE  0

#if 		 COMPILER_MALLOC_SUPPORT >0
fifo_tcb_t*		m_bluetooth_serialB_fifo_tcb = 0;
#else
uint8_t      	m_bluetooth_serialB_rev_fifo[MAX_REV_FIFO_SIZE] = {0, };  //串口FIFO
fifo_tcb_t   	m_bluetooth_serialB_fifo_tcb_body = {0, };  				//串口控制TCB
fifo_tcb_t*   	m_bluetooth_serialB_fifo_tcb = &m_bluetooth_serialB_fifo_tcb_body;  //串口控制TCB
#endif
a_lock_t        *m_bluetooth_serialB_read_lock  = INVALID_LOCK;
a_lock_t        *m_bluetooth_serialB_write_lock = INVALID_LOCK;


const char* m_BTmod_AT_command_tbl[]=
{
	"+++OPEN\n",		//进入AT模式命令
	"gc200808\n",		//进入AT模式口令
	"AT+ENTER\n",		//退出AT模式，返回正常模式命令
	"AT\n",				//AT模式握手命令
	"AT+DEFAULT\n",		//恢复默认参数
	"AT+ADDR?\n",		//本地蓝牙地址显示
	"AT+VER?\n",		//固件版本显示
	"AT+PIN?\n",		//显示蓝牙当前pin码
	"AT+PIN=",			//后加设置的pin及回车，pin码仅可为数字可字母
	"AT+UART?\n",		//显示当前蓝牙模块波特率
	"AT+UARTBAUDRATE=",	//设置蓝牙波特率，=号加波特率及回车
	"AT+UARTSTOP=",		//设置停止位长, =号后加停止位长及回车
	"AT+UARTPARITY=",	//设置串口校验位，=号后加校验位及回车，N无校验，E偶校验，O奇校验
	"AT+ROLE?\n",		//主从角色设显示
	"AT+ROLE=",			//主从角色设置， =号加角色及回车，0:slave,1:master
	"AT+INQMODE?\n",	//显示查询模式参数
	"AT+INQMAXNUM=",	//设置最大查询蓝牙设备数，=号后加设备数及回车
	"AT+INQTIME=",		//设置查询时间,=号后加时间及回车
	"AT+INQUIRY\n",		//开始查询周围蓝牙设备
	"AT+BINDADDR?\n",	//显示绑定的从地址
	"AT+BINDADDR0=",	//绑定0号地址
	"AT+DEVICENAME?\n",	//显示本端蓝牙设备名
	"AT+DEVICENAME=",	//设置蓝牙模块名字，=号后加名字及回车，仅支择数字和字母
	"AT+MSGENABLE?\n",	//串口提示信息显示命令
	"AT+MSGENABLE=",	//串口提示信息显示开关命令，=号后加命令及回车，1:开，0关
};

 /*
 2812 波特率列表
*/
baud_set_t my_bluetooth_baud_tab[] =
{
	{300, 0xF4, 0x23},
	{600, 0x7A, 0x11},
	{1200, 0x3D, 0x08},
	{2400, 0x1E, 0x83},
	{4800, 0x0F, 0x41},
	{9600, 0x07, 0xA0},
	{14400, 0x05, 0x15},
	{19200, 0x03, 0xCF},
	{38400, 0x01, 0xE7},
	{57600, 0x01, 0x44},
	{115200, 0x00, 0xA1}
};


/********************************************************************
 * @创建人 :揭成
 * @功能:设置波特率
 * @输入:baud 要设置的baud
 * @输出:实际写入大小
********************************************************************/
void bluetooth_serialB_set_baud( uint32_t baud )
{
	Bsize_t index;
	for ( index = 0;index < SIZE_OF_ARRAY( my_bluetooth_baud_tab );index++ )
	{
		if ( my_bluetooth_baud_tab[index].set_baud == baud )
		{
			ScibRegs.SCIHBAUD = my_bluetooth_baud_tab[index].set_SCIHBAUD;
			ScibRegs.SCILBAUD = my_bluetooth_baud_tab[index].set_SCILBAUD;
			return;
		}
	}
	//ScibRegs.SCIHBAUD = 0x3D;
	//ScibRegs.SCILBAUD = 0x08;
	return;
}

/********************************************************************
 * @创建人 :揭成
 * @功能:串口接收中断
 *
 * @输入:
 *
 *@输出:
********************************************************************/

interrupt void bluetooth_rx_isr( void )
{
	uint8_t buf;
	if ( ScibRegs.SCIRXST.bit.RXERROR == 1 )
	{
		ScibRegs.SCICTL1.all = 0x0003;
		ScibRegs.SCICTL1.all = 0x0023;
		ScibRegs.SCIFFRX.bit.RXFFINTCLR = 1; 					/*Clear Interrupt flag*/
		PieCtrlRegs.PIEACK.all |= 0x100;    		 			/*Issue PIE ack*/
		return;
	}
	buf = ( ScibRegs.SCIRXBUF.all ) & 0x00ff;
	fifo_buffer_push( m_bluetooth_serialB_fifo_tcb, &buf, 1 );	/*将接收到的数据装进FIFO*/
	ScibRegs.SCIFFRX.bit.RXFFINTCLR = 1; 						/*Clear Interrupt flag*/
	PieCtrlRegs.PIEACK.all |= 0x100;        					/*Issue PIE ack*/
	return;
}


#pragma CODE_SECTION(bluetooth_serialB_us_delay,"ramfuncs") 		//将该段代码限制在ramfuncs。
#pragma CODE_SECTION(DSP28x_usDelay,"ramfuncs") //将该段代码限制在ramfuncs。
static void bluetooth_serialB_us_delay( uint16_t x )	/* 1μs,150	MHz	*/
{
	uint16_t count = 0;
	//m_offset_cnt = (uint16_t)(x * 7.5);
	count = (uint16_t)(x * 45);
	DSP28x_usDelay(count);
	return;
}

/********************************************************************
 * @创建人 :揭成
 * @功能:串口写入一个数组
 *
 * @输入:buf 待写入数组头，size 数组大小
 *
 *@输出:实际写入大小
********************************************************************/
Bsize_t bluetooth_driver_write( uint8_t*buf, Bsize_t buf_size , void*pos )
{
	Bsize_t i;
	for ( i = 0;i < buf_size;i++ )
	{
		//while ( ScibRegs.SCIFFTX.bit.TXFFST == 0X10 ) ;
		while ( ScicRegs.SCICTL2.bit.TXRDY == 0 ) ;
		ScibRegs.SCITXBUF = *buf++;
		//因在flash与ram里运行速度不一样，因此将延时函数
		//限制在ram中运行，以保证延时时间一样
		//bluetooth_serialB_us_delay(220);  //延时520us
		bluetooth_serialB_us_delay(250);  //延时520us
	}
	return buf_size;
}


/********************************************************************
 * @创建人 :揭成
 * @功能:串口写入一个数组
 *
 * @输入:buf 待写入数组头，size 数组大小
 *
 *@输出:实际写入大小
********************************************************************/
static Bsize_t bluetooth_serialB_write( uint8_t*buf, Bsize_t buf_size )
{
	Bsize_t i;
	for ( i = 0;i < buf_size;i++ )
	{
		//while ( ScibRegs.SCIFFTX.bit.TXFFST == 0X10 ) ;
		while ( ScicRegs.SCICTL2.bit.TXRDY == 0 ) ;
		ScibRegs.SCITXBUF = *buf++;
		//因在flash与ram里运行速度不一样，因此将延时函数
		//限制在ram中运行，以保证延时时间一样
		bluetooth_serialB_us_delay(250);  //延时520us
	}
	return buf_size;
}

/********************************************************************
 * @创建人 :阳永锋
 * @功能:发送字符串，如错误(无返回)，最多重发30次，
 *       返回发送设置状态: 1:成功，0失败
 *
 * @输入:待发送的数据缓冲区
 *
 *@输出:实际写入大小
********************************************************************/
static uint8_t _bt_cmd_retry_send(char* ch)
{
	uint8_t try_times = 0;
	while(try_times++ < 30)
	{
		bluetooth_serialB_write((uint8_t*)ch,global_strlen(ch));
		msleep(50);  
		if( fifo_buffer_get_size( m_bluetooth_serialB_fifo_tcb ) > 0)
		{
			fifo_buffer_empty( m_bluetooth_serialB_fifo_tcb );
			break;
		}
	}
	if(try_times >= 30)
		return 0;
	else
		return 1;
}

/********************************************************************
 * @创建人 :阳永锋
 * @功能:发送字符串，如错误(无返回)，最多重发50次，
 *       返回发送设置状态: 1:成功，0失败
 *
 * @输入:待发送的数据缓冲区, 发送的AT命令
 *
 *@输出:实际写入大小
********************************************************************/
static uint8_t _retry_send_at_cmd(char* ch, char *cmd)
{
	uint8_t try_times = 0;
	char *pfind =  PNULL;
	uint16_t recv_sizes = 0;
	
	while(try_times++ <50)
	{
		bluetooth_serialB_write((uint8_t*)ch,global_strlen(ch));
		msleep(50);  
		if( fifo_buffer_get_size( m_bluetooth_serialB_fifo_tcb ) > 0)
		{
			recv_sizes = fifo_buffer_get_size( m_bluetooth_serialB_fifo_tcb );
			pfind = global_string_locate_end((char*)m_bluetooth_serialB_rev_fifo,
				(char*)m_bluetooth_serialB_rev_fifo+recv_sizes,cmd);
			if(pfind != PNULL)
			{
				fifo_buffer_empty( m_bluetooth_serialB_fifo_tcb );
				break;
			}
		}
	}
	if(try_times >= 50)
		return 0;
	else
		return 1;
}

/********************************************************************
 * @创建人 :阳永锋
 * @功能: 显示蓝牙搜索参数
 *
 * @输入:
 *
 *@输出:
********************************************************************/
static void bluetooth_inqury_param_get(char *param)
{
	char ch[50]= "";
	uint8_t state = 0;
	state = state;								//去除编译警告
	if(param == PNULL) return;
	
	global_strcpy(ch, (char*)"+++OPEN\r");		//进入AT模式
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"gc200808\r");		//输入AT模式口令
	state = _bt_cmd_retry_send(ch); 
	
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+INQMODE?\r");	//发送查询命令
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+RESET\r");	//退出AT模式
	state = _bt_cmd_retry_send(ch); 
	return;
	
}


/********************************************************************
 * @创建人 :阳永锋
 * @功能: 启动查询
 *
 * @输入:
 *
 *@输出:
********************************************************************/
static void bluetooth_inqury_start(char *addr)
{
	char ch[50]= "";
	uint8_t state = 0;

	if(addr == PNULL) return;
	addr = addr;								//去除编译警告
	state = state;								//去除编译警告
	global_strcpy(ch, (char*)"+++OPEN\r");		//进入AT模式
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"gc200808\r");		//输入AT模式口令
	state = _bt_cmd_retry_send(ch); 
	
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+INQUIRY\r");	//发送查询命令
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+RESET\r");	//退出AT式
	state = _bt_cmd_retry_send(ch); 
	return;
	
}


/********************************************************************
 * @创建人 :阳永锋
 * @功能: 启动查询
 *
 * @输入:
 *
 *@输出:
********************************************************************/
static void bluetooth_set_inqury_time(char *ptime)
{
	char ch[50]= "";
	uint8_t state = 0;
	uint8_t index = 0;

	state = state;								//去除编译警告
	if(ptime == PNULL)
		return;
	for(index = 0; index < global_strlen(ptime); index++)	//pin码仅能为数字
	{
		if( !isdigit( *(ptime +index) ) )
			return;
	}
	global_strcpy(ch, (char*)"+++OPEN\r");		//进入AT模式
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"gc200808\r");		//输入AT模式口令
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+INQTIME=");	//设置蓝牙查询时间
	global_strcat(ch, ptime);
	global_strcat(ch, "\r");
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+RESET\r");	//退出AT模式
	state = _bt_cmd_retry_send(ch); 
	return;
	
}

/********************************************************************
 * @创建人 :阳永锋
 * @功能: 设置pin码
 *
 * @输入:从端pin密码
 *
 *@输出:
********************************************************************/
static void bluetooth_pin_set(char *pin)
{
	char ch[50]= "";
	uint8_t state = 0;
	uint8_t index = 0;

	state = state;								//去除编译警告

	if(pin == PNULL)
		return;
	for(index = 0; index < global_strlen(pin); index++)	//pin码仅能为数字或英文字母
	{
		if( !isalnum( *(pin +index) ) )
			return;
	}
	global_strcpy(ch, (char*)"+++OPEN\r");		//进入AT模式
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"gc200808\r");		//输入AT模式口令
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+PIN=");		//发送pin码设置命令
	global_strcat(ch, pin);
	global_strcat(ch,"\r");
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+RESET\r");	//退出AT模式
	state = _bt_cmd_retry_send(ch); 
	return;
}

/********************************************************************
 * @创建人 :阳永锋
 * @功能: 绑定地址
 *
 * @输入:从端蓝牙地址
 *
 *@输出:
********************************************************************/
static void  bluetooth_bindaddr_set(char *addr)
{
	char ch[50]= "";
	uint8_t index = 0;
	uint8_t state = 0;
	
	state = state;								//去除编译警告

	if(addr == PNULL)
		return;
	//地址码仅能为数字或英文字母或":"
	for(index = 0; index < global_strlen(addr); index++)	
	{
		if( (!isalnum(*(addr +index))) && ( *(addr+index) != ':') )
			return;
	}
	global_strcpy(ch, (char*)"+++OPEN\r");		//进入AT模式
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"gc200808\r");		//输入AT模式口令
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+BINDADDR0=");		//绑定从端蓝牙地址
	global_strcat(ch, addr);
	global_strcat(ch,"\r");
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+RESET\r");	//退出AT模式
	state = _bt_cmd_retry_send(ch); 
	
	return;
}



/********************************************************************
 * @创建人 :阳永锋
 * @功能: 开关串口状态提示信息
 *
 * @输入:开关命令: 1: 开; 0:关
 *
 *@输出:
********************************************************************/
static void  bluetooth_message_onoff(uint8_t *onoff)
{
	char ch[50]= "";
	uint8_t state = 0;
	
	state = state;								//去除编译警告

	if(onoff == PNULL) return;
	global_strcpy(ch, (char*)"+++OPEN\r");		//进入AT模式
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"gc200808\r");		//输入AT模式口令
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+MSGENABLE=");	
	if(*onoff == 1)
	{
		global_strcat(ch,"1");
	}
	else
		global_strcat(ch,"0");
	global_strcat(ch,"\r");
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+RESET\r");	//退出AT模式
	state = _bt_cmd_retry_send(ch); 
	return;
}


/********************************************************************
 * @创建人 :阳永锋
 * @功能: 显示当前绑定蓝牙地址
 *
 * @输入:空
 *
 *@输出:
********************************************************************/
static void  bluetooth_view_cur_bind_addr(char *addr)
{
	char ch[50]= "";
	uint8_t state = 0;
	
	state = state;								//去除编译警告
	if(addr == PNULL) return;
	GLOBAL_MEMCLR(m_bluetooth_serialB_rev_fifo, sizeof(m_bluetooth_serialB_rev_fifo));
	GLOBAL_MEMCLR(ch, sizeof(ch));
	addr = addr;
	
	global_strcpy(ch, (char*)"+++OPEN\r");		//进入AT模式
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"gc200808\r");		//输入AT模式口令
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+BINDADDR?");	
	global_strcat(ch,"\r");
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+RESET\r");	//退出AT模式
	state = _bt_cmd_retry_send(ch); 
	return;
}

/********************************************************************
 * @创建人 :阳永锋
 * @功能: 显示当前绑定蓝牙地址
 *
 * @输入:空
 *
 *@输出:
********************************************************************/
static void  bluetooth_set_role(uint8_t *role)
{
	char ch[50]= "";
	uint8_t state = 0;
	
	state = state;								//去除编译警告
	if(role == PNULL) return;
	GLOBAL_MEMCLR(m_bluetooth_serialB_rev_fifo, sizeof(m_bluetooth_serialB_rev_fifo));

	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"+++OPEN\r");		//进入AT模式
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"gc200808\r");		//输入AT模式口令
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));	
	global_strcpy(ch, (char*)"AT+ROLE=");		//设置主，从模式
	if(*role == BT_MASTER_ROLE)
	{
		global_strcat(ch,"1");
	}	
	else
	{
		global_strcat(ch,"0");
	}
	global_strcat(ch,"\r");
	state = _retry_send_at_cmd(ch, (char*)"AT+ROLE="); 

	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+PIN=");		//发送pin码设置命令
	global_strcat(ch, BLUETOOTH_DEFAULT_PIN_CODE);
	global_strcat(ch,"\r");
	state = _retry_send_at_cmd(ch,(char*)"AT+PIN=");
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+RESET\r");	//退出AT模式
	state = _retry_send_at_cmd(ch, (char*)"AT+RESET"); 
	
	return;
}


/********************************************************************
 * @创建人 :阳永锋
 * @功能: 设置蓝牙用主模式，关闭调试信息输入，设置固定PIN码，
 *        绑定设定蓝牙
 *
 * @输入:
 *       
 *@输出:
********************************************************************/
static void bluetooth_set_bt_default_state(char *addr)
{
	char ch[50]= "";
	uint8_t state = 0;
	
	state = state;								//去除编译警告
	if(addr == PNULL) return;
	
	GLOBAL_MEMCLR(m_bluetooth_serialB_rev_fifo, sizeof(m_bluetooth_serialB_rev_fifo));
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"+++OPEN\r");			//进入AT模式
	state = _bt_cmd_retry_send(ch);
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"gc200808\r");			//输入AT模式口令
	state = _bt_cmd_retry_send(ch);
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+ROLE=1\r");	
	state = _retry_send_at_cmd(ch,(char*)"AT+ROLE=");
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+PIN=");		//发送pin码设置命令
	global_strcat(ch, BLUETOOTH_DEFAULT_PIN_CODE);
	global_strcat(ch,"\r");
	state = _retry_send_at_cmd(ch,(char*)"AT+PIN=");
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+MSGENABLE=0\r");		//关闭串口提示信息
	state = _retry_send_at_cmd(ch,(char*)"AT+MSGENABLE=");
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+BINDADDR0=");		//发送pin码设置命令
	global_strcat(ch, addr);
	global_strcat(ch,"\r");
	state = _retry_send_at_cmd(ch,(char*)"AT+BINDADDR0=");  
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+RESET\r");			//退出AT模式
	state = _retry_send_at_cmd(ch,(char*)"AT+RESET"); 
	return;
}




/********************************************************************
 * @创建人 :
 * @功能: 驱动参数设置接口
 *
 * @输入:none
 *
 *@输出:none
********************************************************************/
void  bluetooth_driver_ioctl( uint8_t cmd, void*arg )
{
	Bsize_t *pstate = ( Bsize_t* )arg;
	
	switch ( cmd )
	{
		case AT_CMD_INQ_START :
			fifo_buffer_empty( m_bluetooth_serialB_fifo_tcb );		//清空确保查找结果是不会搜到重复内容	
			bluetooth_inqury_start((char*)arg);
			fifo_buffer_empty( m_bluetooth_serialB_fifo_tcb );
			break;
		case AT_CMD_PIN_SET:
			fifo_buffer_empty( m_bluetooth_serialB_fifo_tcb );
			bluetooth_pin_set((char*)arg);
			fifo_buffer_empty( m_bluetooth_serialB_fifo_tcb );
			break;
		case AT_CMD_BINDADDR_SET:
			fifo_buffer_empty( m_bluetooth_serialB_fifo_tcb );
			bluetooth_bindaddr_set((char*)arg);
			fifo_buffer_empty( m_bluetooth_serialB_fifo_tcb );
			break;
		case SET_EMPTY_FIFO:
			SERIALB_INT_DIS();
			fifo_buffer_empty( m_bluetooth_serialB_fifo_tcb );
			SERIALB_INT_EN();
		case  DRIVER_LOCK_REVERT:
			os_lock_revert( m_bluetooth_serialB_read_lock );
			os_lock_revert( m_bluetooth_serialB_write_lock );
			break;
		case GET_FIFO_STATE:
			SERIALB_INT_DIS();
			*pstate = fifo_buffer_get_size( m_bluetooth_serialB_fifo_tcb );
			SERIALB_INT_EN();
			break;
		case AT_CMD_INQ_TIME_SET:
			fifo_buffer_empty( m_bluetooth_serialB_fifo_tcb );		//清空确保查找结果是不会搜到重复内容	
			bluetooth_set_inqury_time( (char*)arg );
			fifo_buffer_empty( m_bluetooth_serialB_fifo_tcb );
			break;
		case AT_CMD_INQMOD_GET:
			fifo_buffer_empty( m_bluetooth_serialB_fifo_tcb );		
			bluetooth_inqury_param_get((char*)arg);
			fifo_buffer_empty( m_bluetooth_serialB_fifo_tcb );
			break;
		case AT_CMD_MSG_VIEW_ONOFF:
			fifo_buffer_empty( m_bluetooth_serialB_fifo_tcb );			
			bluetooth_message_onoff((uint8_t*)arg);
			fifo_buffer_empty( m_bluetooth_serialB_fifo_tcb );
			break;
		case AT_CMD_BINDADDR_VIEW:
			fifo_buffer_empty( m_bluetooth_serialB_fifo_tcb );			
			bluetooth_view_cur_bind_addr((char*)arg);
			fifo_buffer_empty( m_bluetooth_serialB_fifo_tcb );
			break;
		case AT_CMD_ROLE_SET:
			fifo_buffer_empty( m_bluetooth_serialB_fifo_tcb );			
			bluetooth_set_role((uint8_t*)arg);
			fifo_buffer_empty( m_bluetooth_serialB_fifo_tcb );
			break;
		case CMD_SET_BT_AT_DEFAULT_STATE:
			fifo_buffer_empty( m_bluetooth_serialB_fifo_tcb );			
			bluetooth_set_bt_default_state((char*)arg);
			fifo_buffer_empty( m_bluetooth_serialB_fifo_tcb );
			break;
		default:
			break;
	}
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能:从串口读出数据
 *
 * @输入 : buf:指向读出缓冲区
                  size:需要数据大小
 *               port :串口号
 *@输出:实际读出大小
********************************************************************/
Bsize_t  bluetooth_driver_read( uint8_t*buf, Bsize_t size , void*pos )
{
	Bsize_t real_size = 0;
	os_lock( m_bluetooth_serialB_read_lock );
	SERIALB_INT_DIS();
	real_size	= fifo_buffer_get_size( m_bluetooth_serialB_fifo_tcb );
	SERIALB_INT_EN();

	if ( real_size < size )
	{
		msleep(( size - real_size ) );
		SERIALB_INT_DIS();
		real_size	= fifo_buffer_get_size( m_bluetooth_serialB_fifo_tcb );
		SERIALB_INT_EN();
		if ( real_size == 0 )
		{
			os_unlock( m_bluetooth_serialB_read_lock );
			return 0; 
		}
	}
	if ( real_size > size )
	{
		real_size = size;
	}
	SERIALB_INT_DIS();
	fifo_buffer_pop( m_bluetooth_serialB_fifo_tcb, buf, real_size );
	SERIALB_INT_EN();
	os_unlock( m_bluetooth_serialB_read_lock );
	return real_size;
}

/********************************************************************
 * @创建人 :
 * @功能 :串口通道选择
 * @输入 :
 * @输出 :
********************************************************************/

static bluetooth_serialB_select_path(void)
{
	
	EALLOW;
		/*断开触摸控制串口*/
		GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 0;   
		GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 0;   	
		GpioCtrlRegs.GPADIR.bit.GPIO18  = 0;    //设置成输入
		GpioCtrlRegs.GPADIR.bit.GPIO19  = 0;    //设置成输入
    	//配置14 15为串口
		GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;	   // Enable pull-up for GPIO14 (SCITXDB)
		GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;	   // Enable pull-up for GPIO15 (SCIRXDB)
		GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 3;  // Asynch input GPIO15 (SCIRXDB)
		GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 2;   // Configure GPIO14 for SCITXDB operation
		GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 2;   // Configure GPIO15 for SCIRXDB operation
	EDIS;
    return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :串口硬件设置
 * @输入 :
 * @输出 :
********************************************************************/
static bluetooth_serialB_hardware_set(void)
{
	ScibRegs.SCICCR.all = 0x0007;  // 1 stop bit,  No loopback
	// No parity,8 char bits,
	// async mode, idle-line protocol
	ScibRegs.SCICTL1.all = 0x0003; // enable TX, RX, internal SCICLK,
	// Disable RX ERR, SLEEP, TXWAKE
	ScibRegs.SCICTL2.bit.TXINTENA = 0;
	ScibRegs.SCICTL2.bit.RXBKINTENA = 1;
	bluetooth_serialB_set_baud( 38400 );	//set baud is 9600
	ScibRegs.SCICCR.bit.LOOPBKENA = 0; // Enable loop back
	//ScibRegs.SCIFFTX.all = 0xc03f;//0xC028;
	//ScibRegs.SCIFFRX.all = 0x0021;
	//ScibRegs.SCIFFCT.all = 0x00;

	ScibRegs.SCICTL1.all = 0x0023;    // Relinquish SCI from Reset
	ScibRegs.SCICTL1.all = 0x0063;    //RX错误中断允许

	//ScibRegs.SCIFFTX.bit.TXFIFOXRESET = 1;
	//ScibRegs.SCIFFRX.bit.RXFIFORESET = 1;
	//ScibRegs.SCIPRI.bit.SOFT = 1;
	//ScibRegs.SCIPRI.bit.FREE = 0;
	EALLOW;
	PieCtrlRegs.PIEIER9.bit.INTx3= 1;
	PieVectTable.SCIRXINTB = &bluetooth_rx_isr;
	EDIS;
    return;
}


/********************************************************************
 * @创建人 :揭成
 * @功能:串口初始化
 *
 * @输入:
 *                   baud:波特率
 *                   parity:是否启用奇偶校验
 *                   stop_bit:是否要停止位
 *
 *@输出:none
********************************************************************/
static void bluetooth_serialB1_init( void )
{
    bluetooth_serialB_hardware_set();
	bluetooth_serialB_select_path();
	return;
}

void bluetooth_driver_init(void)
{
	fifo_buffer_init( m_bluetooth_serialB_fifo_tcb, MAX_REV_FIFO_SIZE, m_bluetooth_serialB_rev_fifo );
	bluetooth_serialB1_init();				//初始化串口
	//bluetooth_hardware_set();
	os_lock_creat( &m_bluetooth_serialB_read_lock );
	os_lock_creat( &m_bluetooth_serialB_write_lock );
	fifo_buffer_init( m_bluetooth_serialB_fifo_tcb, MAX_REV_FIFO_SIZE, m_bluetooth_serialB_rev_fifo );
}

/********************************************************************
 * @创建人 :揭成
 * @功能:串口卸载
 *
 * @输入:
 *
 *@输出:none
********************************************************************/
void bluetooth_driver_uninstall( void )
{
	SERIALB_INT_DIS();
#if  COMPILER_MALLOC_SUPPORT >0
	fifo_buffer_destroy( m_bluetooth_serialB_fifo_tcb );
#endif
	return;
}



dev_id_t bluetooth_driver_register( void )    //蓝牙模块串口
{
	uint8_t ret;
	os_dirver_operate_t temp;
	temp.os_driver_init      = bluetooth_driver_init;
	temp.os_driver_ioctl     = bluetooth_driver_ioctl;
	temp.os_driver_read      = bluetooth_driver_read;
	temp.os_driver_write     = bluetooth_driver_write;
	temp.os_driver_uninstall = bluetooth_driver_uninstall;
	ret = os_driver_add( &temp, "bluetooth Driver" );
	return ret;
}

#pragma DATA_SECTION (bluetooth_list,"DR_LIST_ADDR")
const DR_member_t bluetooth_list[]=
{
	bluetooth_driver_register,
	0x5a5a
};



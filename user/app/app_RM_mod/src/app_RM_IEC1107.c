/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:关于IEC1107规约和DLMS规约族的简单解析和读表应用
* 创建时间:2013-05-02
* 所属公司:科荟
* 文件名称:app_RM_IEC1107.c
* 作者 :周炳权
* 功能描述:实现读表功能
******************************************************************************************/
#include	"app_RM_inside.h"

/*
	消息内容说明
1.启动字符"/"(2FH);
2.结束符"!"(21H);
3.完成符(CR,回车,0DH;LF,换行,0AH);
4.应答符(ACK,06H);
5.数据块校验字符中的帧启动符(STX,02H);
6.数据块中的结束字符(ETX,03H);
7.分块数据中的结束字符(EOT,04H);
8.数据块校验字符(BCC);
9.传输请求命令"?"(3FH);
10.协议控制字符V:模式C和E中的使用,
				0--正常协议程序,
				1--第二协议程序,
				2--HDLC协议程序,
				3~9--保留;
11.模式控制符Y:模式C和E中的使用,
				0--数据控制,
				1--编程模式,
				2--二进制模式(HDLC),
				3~5--备用,
				6~9--厂家使用,
				A~Z--备用,
12.制造厂的标识符:;
13.波特率的标识符:
   模式C协议:0--300baud;
			 1--600baud;
			 2--1200baud;
			 3--2400baud;
			 4--4800baud;
			 5--9600baud;
			 6--19200baud;
			 7、8、9扩展备用;
14.识别区,制造厂专用字符,最多16个字符,"/"和"!"除外;
15.含测量值的数据块;
16.重复请求字符(NAK:否定应答,代码15H);
17.读书起始符(SOH:读数起始,代码01H)
18.命令消息识别符,
		P----密码命令;
		W----写命令;
		R----读命令;
		E----执行命令;
		B----退出命令(中断通讯);
19.命令类型识别符(区别命令的差别)值:

20.数据组,

21.错误消息，
22.装置地址:最多32个字符;(/?装置地址! CR LF);
   缺省地址区被视作通配地址(/?!CR LF);
23.序列分隔符("\",代码5CH);
24.增强波特率和改变模式标识符;
*/

/***********************读取数据步骤*********************
			模式C:直接本地数据交换的正常协议数据控制
*1. send    请求消息:  		/ ? 装备地址 ! <CR> <LF>		//300baud,7E1
*2. receive 识别消息:  		/ X X X Z \ W 标识 <CR> <LF>	//300baud,7E1
	(兰吉尔电表)例:    		/LGZ4\2ZFD4104407.B14<CR><LF>	
*3.send应答/选择消息:  		ACK '0' Z '0' <CR> <LF>			//300baud,7E1
*4.	receive 数据消息:  		STK 数据块 ! <CR> <LF> <ETX> <BCC>	//Z baud,7E1
			模式E:直接本地数据交换的计量HDLC协议(当前使用本模式)
*1. send    请求消息:  		/ ? 装备地址 ! <CR> <LF>		//300baud,7E1
*2. receive 识别消息:  		/ X X X Z \ W 标识 <CR> <LF>	//300baud,7E1
	(兰吉尔电表)例:    		/LGZ4\2ZFD4104407.B14<CR><LF>	
*3.send应答/选择消息:  		ACK '2' Z '2' <CR> <LF>			//300baud,7E1
*4.	receive 转换确认消息:  	ACK '2' Z '2' <CR> <LF>			//Z baud,7E1
*5.send_init 计量HDLC协议:									//Z baud,8E1
*注:a:装备地址为0时是通配地址;
*	b:数据传输都以字符形式;
********************************************************/
uint32_t baud_buf_IEC1107[7] 	= {300,600,1200,2400,4800,9600,19200};	
char ack_message_buf[6] 		= {0x06,0x32,'0',0x32,0x0D,0x0A};//DLMS
char ack_IECmessage_buf[6] 		= {0x06,'0','0','0',0x0D,0x0A};//IEC62056-21
char g_baud_flag = '0';				//全局的波特率字符
char g_IEC_meter_addr[18] = { 0, };	//本协议装置地址
char g_DLMS_rec_buf[150] = { 0, };
int g_frame_data_len = 0;	//当前数据帧的长度,(以字节为单位,不包括两个0x7e)
uint8_t g_frame_control_word = 0; 	//当前数据帧的控制字
char g_control_field_flag = 0; 		//当前控制域标志
uint8_t g_dest_addr = 0;			//当前目的地址
uint8_t g_src_addr = 0;				//当前源地址
uint8_t g_meter_logic_addr = 0;		//表逻辑地址
uint8_t g_RFN = 0;					//接收序列号
uint8_t g_SFN = 0;					//发送序列号
uint8_t g_Poll_bit = 0;				//查询位
uint8_t g_Final_bit = 0;			//结束位
uint8_t g_PF_bit = 0;				//P/F：poll/final bit
uint8_t g_frame_Rfinish = 0;		//帧接收完成标志
uint8_t g_control_set[3] = { 0,};	//控制设置
uint8_t g_send_meterCMD_offset = 0;	//发送电表命令偏移
uint8_t g_MCMD_high[25];
uint8_t g_MCMD_low[25];
uint8_t g_guiye_DLMS_flag = 0;		//全局的DLMS协议标志
char g_compatible_DLMS = 0;			//兼容DLMS协议字符缓存,'2'=DLMS兼容表
float m_fdata[4] = { 0.0, };		//读取电能表数据缓存

/*移植H3A部分程序*/
/*校验表*/
const unsigned int mFCS_crc_tab[256]=
{
		0x0000,0x1189,0x2312,0x329b,0x4624,0x57ad,0x6536,0x74bf,
		0x8c48,0x9dc1,0xaf5a,0xbed3,0xca6c,0xdbe5,0xe97e,0xf8f7,
		0x1081,0x0108,0x3393,0x221a,0x56a5,0x472c,0x75b7,0x643e,
		0x9cc9,0x8d40,0xbfdb,0xae52,0xdaed,0xcb64,0xf9ff,0xe876,
		0x2102,0x308b,0x0210,0x1399,0x6726,0x76af,0x4434,0x55bd,
		0xad4a,0xbcc3,0x8e58,0x9fd1,0xeb6e,0xfae7,0xc87c,0xd9f5,
	    0x3183,0x200a,0x1291,0x0318,0x77a7,0x662e,0x54b5,0x453c,
	    0xbdcb,0xac42,0x9ed9,0x8f50,0xfbef,0xea66,0xd8fd,0xc974,
	    0x4204,0x538d,0x6116,0x709f,0x0420,0x15a9,0x2732,0x36bb,
	    0xce4c,0xdfc5,0xed5e,0xfcd7,0x8868,0x99e1,0xab7a,0xbaf3,
	    0x5285,0x430c,0x7197,0x601e,0x14a1,0x0528,0x37b3,0x263a,
	    0xdecd,0xcf44,0xfddf,0xec56,0x98e9,0x8960,0xbbfb,0xaa72,
	    0x6306,0x728f,0x4014,0x519d,0x2522,0x34ab,0x0630,0x17b9,
	    0xef4e,0xfec7,0xcc5c,0xddd5,0xa96a,0xb8e3,0x8a78,0x9bf1,
	    0x7387,0x620e,0x5095,0x411c,0x35a3,0x242a,0x16b1,0x0738,
	    0xffcf,0xee46,0xdcdd,0xcd54,0xb9eb,0xa862,0x9af9,0x8b70,
	    0x8408,0x9581,0xa71a,0xb693,0xc22c,0xd3a5,0xe13e,0xf0b7,
	    0x0840,0x19c9,0x2b52,0x3adb,0x4e64,0x5fed,0x6d76,0x7cff,
	    0x9489,0x8500,0xb79b,0xa612,0xd2ad,0xc324,0xf1bf,0xe036,
	    0x18c1,0x0948,0x3bd3,0x2a5a,0x5ee5,0x4f6c,0x7df7,0x6c7e,
	    0xa50a,0xb483,0x8618,0x9791,0xe32e,0xf2a7,0xc03c,0xd1b5,
	    0x2942,0x38cb,0x0a50,0x1bd9,0x6f66,0x7eef,0x4c74,0x5dfd,
	    0xb58b,0xa402,0x9699,0x8710,0xf3af,0xe226,0xd0bd,0xc134,
		0x39c3,0x284a,0x1ad1,0x0b58,0x7fe7,0x6e6e,0x5cf5,0x4d7c,
	    0xc60c,0xd785,0xe51e,0xf497,0x8028,0x91a1,0xa33a,0xb2b3,
	    0x4a44,0x5bcd,0x6956,0x78df,0x0c60,0x1de9,0x2f72,0x3efb,
	    0xd68d,0xc704,0xf59f,0xe416,0x90a9,0x8120,0xb3bb,0xa232,
	    0x5ac5,0x4b4c,0x79d7,0x685e,0x1ce1,0x0d68,0x3ff3,0x2e7a,
	    0xe70e,0xf687,0xc41c,0xd595,0xa12a,0xb0a3,0x8238,0x93b1,
	    0x6b46,0x7acf,0x4854,0x59dd,0x2d62,0x3ceb,0x0e70,0x1ff9,
		0xf78f,0xe606,0xd49d,0xc514,0xb1ab,0xa022,0x92b9,0x8330,
	    0x7bc7,0x6a4e,0x58d5,0x495c,0x3de3,0x2c6a,0x1ef1,0x0f78
};

void DLMS_init_old_Command()
{	
	                                   //+p总, +p1,+p2, +p3, +Q总, +Q1, +Q2, +Q3,-p总, -p1, -p2, -p3,-Q总, -Q1, -Q2, -Q3,A失压,B失压,C失压, 总失压,日期时间
	unsigned char uMLZhigh2[]={0x47,0x2A,0x17,0x03,0x04,0x05,0x19,0x08,0x09,0x0A,0x18,0x06,0x07,0x07,0x19,0x0A,0x0B,0x0C,0x8A, 0x8B , 0x8B, 0x8C  ,  0x2B};
    unsigned char uMLZLow2[]= {0xE8,0x38,0x78,0xf0,0xB8,0x80,0x08,0xa0,0x68,0x30,0x40,0x48,0x10,0xD8,0xd0,0xF8,0xc0,0x88,0x50, 0x18 , 0xe0, 0xA8  ,  0xC8};
	global_memcpy(g_MCMD_high,uMLZhigh2,sizeof(uMLZhigh2));
	global_memcpy(g_MCMD_low,uMLZLow2,sizeof(uMLZLow2));
	return;
}

void DLMS_init_new_Command()
{	
	                                   //+p总, +p1,+p2, +p3, +Q总, +Q1, +Q2, +Q3,-p总, -p1, -p2, -p3,-Q总, -Q1, -Q2, -Q3,A失压,B失压,C失压, 总失压,日期时间
	unsigned char uMLZhigh2[]={0x47,0x2A,0x1C,0x03,0x04,0x05,0x19,0x08,0x09,0x0A,0x18,0x06,0x07,0x07,0x17,0x0A,0x0B,0x0C,0x8A, 0x8B , 0x8B, 0x8C  ,  0x2B};
    unsigned char uMLZLow2[]= {0xE8,0x38,0x28,0xf0,0xB8,0x80,0x08,0xa0,0x68,0x30,0x40,0x48,0x10,0xD8,0x78,0xF8,0xc0,0x88,0x50, 0x18 , 0xe0, 0xA8  ,  0xC8};
	global_memcpy(g_MCMD_high,uMLZhigh2,sizeof(uMLZhigh2));
	global_memcpy(g_MCMD_low,uMLZLow2,sizeof(uMLZLow2));
	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :串口读
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
Bsize_t app_RMmod_Usart3_read(void *data,Bsize_t len)
{
	Bsize_t m_r = 0;
	m_r = os_driver_read( g_usart3_dev_id, data, len, 0 );
	return m_r;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :串口设置波特率
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_RMmod_Usart3_baud_set(char *m_baud_flag)
{
	uint8_t m_offset = 0;
	if ( *m_baud_flag == '0' )
	{
		m_offset = 0;
	}
	else if ( *m_baud_flag == '1' )
	{
		m_offset = 1;
	}
	else if ( *m_baud_flag == '2' )
	{
		m_offset = 2;
	}
	else if ( *m_baud_flag == '3' )
	{
		m_offset = 3;
	}
	else if ( *m_baud_flag == '4' )
	{
		m_offset = 4;
	}
	else if ( *m_baud_flag == '5' )
	{
		m_offset = 5;
	}
	else if ( *m_baud_flag == '6' )
	{
		m_offset = 6;
	}
	g_delay_baud_flag = m_offset;
	os_driver_ioctl(g_usart3_dev_id, SET_SERIAL_BAUD, &baud_buf_IEC1107[m_offset]);
	app_RMmod_baud_display((m_offset+1));
	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :串口FIFO大小获取
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
Bsize_t app_RMmod_Usart3_FIFO_get( void )
{
	Bsize_t m_size = 0;
	os_driver_ioctl(g_usart3_dev_id, GET_FIFO_STATE, &m_size);
	return m_size;
}
/********************************************************************
 * @作者 :周炳权
 * @功能 :发送请求消息
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_RM_send_req_msg( void )
{
	char request_message_buf[5] 	= {'/','?','!',0x0D,0x0A};
	//char request_message_buf[5] 	= {0x2F,0x3F,0x21,0x0D,0x0A};
	long m_addr_int = global_atoi(g_IEC_meter_addr);
	g_baud_flag = '0';
	app_RMmod_Usart3_baud_set(&g_baud_flag);
	app_RMmod_CRC_select(3,1);	// 根据标准12页:5.6设置为偶校验
	app_RMmod_CRC_display(3);
	msleep(1);
	if ( m_addr_int == 0 )
	{
		app_RMmod_Usart3_send(request_message_buf,5);
	}
	else 
	{
		app_RMmod_Usart3_send(request_message_buf,2);
		app_RMmod_Usart3_send(g_IEC_meter_addr,18);
		app_RMmod_Usart3_send(&(request_message_buf[2]),3);
	}
	//显示提示内容:起始协议为IEC1107,正在连接通讯
	app_RMmod_rev_tip_display("正在请求与电能表通讯...");
	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :识别消息分析
 * @输入 :NONE
 * @输出 :成功返回1;无数据返回0xff;
********************************************************************/
char app_RM_rec_identify_msg( void )
{
	//char m_mf_flag[3]	= { 0, };   		//制造厂的标识符
	//char m_mf_special_flag[18] = { 0, };	//识别区,制造厂专用字符,最多16个字符
	char m_read_buf[30]	= { 0, };
//	uint8_t m_offset = 0;
	char m_r = 0xff;
	if ( app_RMmod_Usart3_read(m_read_buf,1) == 1 )
	{
		if ( m_read_buf[0] == '/' )
		{
			msleep(80);	//等待接收完成,按最低300buad计算
			if ( app_RMmod_Usart3_read(&(m_read_buf[1]),5) == 5 )
			{	
				if ( m_read_buf[5] == 0x5C ) // '\'
				{
					//global_memcpy(m_mf_flag, &(m_read_buf[1]), 3);
					//m_offset = 6;
					g_baud_flag = m_read_buf[4];
					app_RMmod_Usart3_read(&g_compatible_DLMS,1);//读取标识符
					app_RMmod_Usart3_read(&(m_read_buf[8]),20);
					/*while ( (m_read_buf[m_offset] != 0x0A) )	
					//判断换行符或超时退出
					{
						if ( app_RMmod_Usart3_read(&(m_read_buf[m_offset]),1) == 1 )
						{
							m_offset++;
						}
					}*/
					m_r = 1;
				}
			}
		}
	}
	return m_r;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :发送数据读取应答消息
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_RM_send_ack_msg( void )
{
	ack_message_buf[2] = g_baud_flag;
	app_RMmod_Usart3_send(ack_message_buf,6);
	msleep(10);
	app_RMmod_Usart3_baud_set(&g_baud_flag);
	return;
}

void app_RM_send_IECack_msg( void )
{
	ack_IECmessage_buf[2] = g_baud_flag;
	app_RMmod_Usart3_send(ack_IECmessage_buf,6);
	msleep(10);
	app_RMmod_Usart3_baud_set(&g_baud_flag);
	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :转换确认消息获取分析
 * @输入 :NONE
 * @输出 :成功返回1;无数据返回0xff;
********************************************************************/
char app_RM_convert_affirm_msg( void )
{
	char m_r = 0xff;
	char m_read_buf[6]	= { 0, };
	
	if ( app_RMmod_Usart3_read(m_read_buf,1) == 1 )
	{	
		if ( m_read_buf[0] == ack_message_buf[0] )			//06H
		{
			//等待接收完成
			if ( (g_baud_flag - '0') == 0 )		msleep(20);
			else if ( (g_baud_flag - '0') == 1 )msleep(10);
			else if ( (g_baud_flag - '0') == 2 )msleep(6);
			else if ( (g_baud_flag - '0') == 3 )msleep(4);
			else if ( (g_baud_flag - '0') == 4 )msleep(3);
			else if ( (g_baud_flag - '0') == 5 )msleep(2);
			else if ( (g_baud_flag - '0') == 6 )msleep(1);
			if ( app_RMmod_Usart3_read((m_read_buf+1),5) == 5 )
			{
				if ( m_read_buf[1] == ack_message_buf[1] && 
					 m_read_buf[2] == ack_message_buf[2] &&
					 m_read_buf[3] == ack_message_buf[3] &&
					 m_read_buf[4] == ack_message_buf[4] &&
					 m_read_buf[5] == ack_message_buf[5] )
				{
					m_r = 1;
					app_RMmod_CRC_select(1,1);		//Z baud 8E1,无校验都是8位数据位
					app_RMmod_CRC_display(1);
					//显示提示内容:通讯连接成功,读取数据中...
					app_RMmod_rev_tip_display("正在建立链路层连接...");
				}
		
			}
		}
	}
	return m_r;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :发送DLMS初始化消息帧(建立连接帧SNRM)
 * @输入 :NONE
 * @输出 :NONE
 * 注:MAC子层，不包含应用层数据信息,
 * SNRM：设置正常相应数据帧。用于建立链路层连接。
 * UA：对SNRM和DISC的响应帧。(DISC:结束链路帧,53H)
********************************************************************/
unsigned int DLMS_CalculateCRC( unsigned int fcs,unsigned char *c,unsigned int len )
{          
    while(len--)
        fcs=(fcs>>8)^mFCS_crc_tab[(fcs^*c++)&0xff];
	return (fcs);
}

void app_RM_DLMS_SNRM_send( void )
{
	unsigned int m_crc;
	unsigned char m_send[9];	//7EA0070321930F017E      
	m_send[0]=	0x7E;
	m_send[1]=	0xA0;//A格式
    m_send[2]=	0x07;//长度
	m_send[3]=	0x03;
	m_send[4]=	0x21;
	m_send[5]=	0x93;//帧控制字,SNRM
	m_crc 	 = 	DLMS_CalculateCRC(0x0ffff,m_send+1,5);
	m_crc 	^= 	0xffff;
	m_send[6]=	m_crc&0xff;
	m_send[7]=	(m_crc>>8)&0xff;	
	m_send[8]=	0x7E;	
	app_RMmod_Usart3_send(m_send,9);	

	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :发送数据帧后对返回帧的分析
 * @输入 :NONE
 * @输出 :返回帧数据正确后返回1;错误返回0;
 * 注:client端发送SNRM数据帧，server端响应UA数据帧表示已建立连接，
 * server端响应DM数据帧表示链路断开，连接没有建立。
********************************************************************/
char app_RM_DLMS_return_processed( void )
{
	char m_r = 0;
	unsigned int 	m_crc0,m_crc1;
	unsigned char   m_crcLow,m_crcHigh;

	while( app_RMmod_Usart3_FIFO_get() )
	{
	if ( app_RMmod_Usart3_read(g_DLMS_rec_buf,1) == 1 )
	{
		if ( g_DLMS_rec_buf[0] == 0x7E )
		{
			//等待接收完整一帧数据
			if ( (g_baud_flag - '0') == 0 )		msleep(150);	//300
			else if ( (g_baud_flag - '0') == 1 )msleep(120);	//600
			else if ( (g_baud_flag - '0') == 2 )msleep(60);	//1200
			else if ( (g_baud_flag - '0') == 3 )msleep(30);	//2400
			else if ( (g_baud_flag - '0') == 4 )msleep(16);	//4800
			else if ( (g_baud_flag - '0') == 5 )msleep(8);	//9600
			else if ( (g_baud_flag - '0') == 6 )msleep(4);	//19200
			
			if ( app_RMmod_Usart3_read((g_DLMS_rec_buf+1),1) == 1 )
			{
				if ( (g_DLMS_rec_buf[1] & 0xF0 ) == 0xA0 )//帧类型,DLMS协议对于电表恒为A(1010)
				{
					if ( app_RMmod_Usart3_read((g_DLMS_rec_buf+2),1) == 1 )
					{
						g_frame_data_len = ((int)(g_DLMS_rec_buf[1]&7))*256 + g_DLMS_rec_buf[2];
						if ( app_RMmod_Usart3_read((g_DLMS_rec_buf+3),g_frame_data_len-1) == (g_frame_data_len-1) )
						{
							if ( g_DLMS_rec_buf[g_frame_data_len+1] == 0x7E )//帧尾
							{
								m_crc1  = DLMS_CalculateCRC(0x0ffff,(unsigned char*)(g_DLMS_rec_buf+1),5);
								m_crc0  = m_crc1;
								m_crc1 ^= 0xffff;
								m_crcLow  = (m_crc1>>8)&0xff;
								m_crcHigh =  m_crc1&0xff;
								if((g_DLMS_rec_buf[6]==m_crcHigh)&&(g_DLMS_rec_buf[7]==m_crcLow))
								{
									m_crc1	=DLMS_CalculateCRC(m_crc0,(unsigned char*)(g_DLMS_rec_buf+6),g_frame_data_len-7);//5+2
									m_crc1	^=	0xffff;
									m_crcLow =	(m_crc1>>8)&0xff;
									m_crcHigh=	m_crc1&0xff;	
									if((g_DLMS_rec_buf[g_frame_data_len-1] == m_crcHigh)&&
										(g_DLMS_rec_buf[g_frame_data_len] == m_crcLow))
									{
										g_frame_control_word = g_DLMS_rec_buf[5];
										g_dest_addr = g_DLMS_rec_buf[4];
    									g_src_addr = g_DLMS_rec_buf[3];
										m_r = 1;
										return m_r;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	}
	return m_r;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :控制域格式分析处理
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void DLMS_control_field_format( void )
{
	unsigned char bytContr,MSB,LSB;
    bytContr = g_frame_control_word;
    if((bytContr&1)==0)
	{
        g_control_field_flag = 'I';
        g_RFN 	 = bytContr>>5;			//RRR
        g_SFN 	 = (bytContr&0x0E)>>1;		//SSS
        g_Poll_bit = (bytContr&0x10)>>4;
        bytContr = g_DLMS_rec_buf[1];
        g_PF_bit = (bytContr&0x0F)>>3;
        bytContr = g_SFN;
        g_SFN 	 = g_RFN;
        g_RFN 	 = (bytContr + 1)%8;//取模
	}
    else
	{
        MSB = bytContr>>5; //高三位
        LSB = bytContr&0x0F;
        if(LSB==1)
		{
            g_control_field_flag ='R'; //_T("RR");
            g_Poll_bit =(bytContr&0x10)>>4;
		}
        else if(LSB==5)
		{
            g_control_field_flag ='N'; //_T("RNR");
            g_Poll_bit = (bytContr&0x10)>>4;
		}
        else if(LSB==7)
		{
            g_control_field_flag ='M'; //_T("FRMR");
            g_Final_bit = (bytContr&0x10)>>4;
		}
        else if(LSB==15)
		{
            g_control_field_flag ='D'; //_T("DM");
            g_Final_bit = (bytContr&0x10)>>4;
		}
        else if((LSB==3) && (MSB==4))
		{
            g_control_field_flag ='S'; //_T("SNRM");
            g_Poll_bit = (bytContr&0x10)>>4;
		}
        else if((LSB==3)&&(MSB==2))
		{
            g_control_field_flag ='C'; //_T("DISC");
            g_Poll_bit = (bytContr&0x10)>>4;
		}
        else if((LSB==3)&&(MSB==3))
		{
            g_control_field_flag ='U'; //_T("UA");
            g_Final_bit = (bytContr&0x10)>>4;
		}
        else if((LSB==3)&&(MSB==0))
		{
            g_control_field_flag ='i'; //_T("UI");
            g_Poll_bit = (bytContr&0x10)>>4;
		}
	}
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :读控制头信息
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void DLMS_ReadContrHeadInf( void )
{	
	g_frame_Rfinish = 1;
	if((g_control_field_flag=='I')&&(g_PF_bit==1)) //没发完,以RR响应
	{
        g_control_set[2] = g_RFN * 32 + g_Poll_bit * 16 + 1; //Control Field 
		g_frame_Rfinish = 0;
	}
    else if((g_control_field_flag=='I')&&(g_PF_bit==0))
        g_control_set[2] = g_RFN * 32 + g_Poll_bit * 16 + g_SFN * 2;	 
	g_control_set[1] = g_src_addr;
	g_control_set[0] = g_dest_addr; 
	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :断开通讯连接 
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void DLMS_DisconetCom( void )
{   
    unsigned char m_send[9];
	unsigned int m_crc1;
	DLMS_control_field_format();
	g_control_set[2]=(g_Poll_bit * 16) | 67; 
	g_control_set[1]=g_src_addr;
	g_control_set[0]=g_dest_addr; 	 
	m_send[0]=0x7E;
	m_send[1]=0xA0;//A格式
    m_send[2]=0x07;//长度
	m_send[3]=g_control_set[0];
	m_send[4]=g_control_set[1];
	m_send[5]=g_control_set[2];
	m_crc1=DLMS_CalculateCRC(0x0ffff,m_send+1,5);
	m_crc1^=0xffff;
	m_send[6]= m_crc1 & 0xff;
	m_send[7]=(m_crc1>>8) & 0xff;
	m_send[8]= 0x7E;
	app_RMmod_Usart3_send(m_send,9);		
	return;	 	 
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :接收剩余数据
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void DLMS_SendRecRemnant( void )
{	 
	unsigned char m_send[9];  
	unsigned int m_crc1;
	
	m_send[0]=0x7E;
	m_send[1]=0xA0;//A格式
    m_send[2]=0x07;//长度
	m_send[3]=0x03;
	m_send[4]=0x21;
	m_send[5]=g_control_set[2];//0x93;
	m_crc1	 =DLMS_CalculateCRC(0x0ffff,m_send+1,5);
	m_crc1	^=0xffff;
	m_send[6]=m_crc1&0xff;
	m_send[7]=(m_crc1>>8)&0xff;	
	m_send[8]=0x7E; 
	app_RMmod_Usart3_send(m_send,9);
	return;	 
} 

/********************************************************************
 * @作者 :周炳权
 * @功能 :发送电表信息获取数据
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void DLMS_SendMeterInf( void )
{	 
	unsigned char m_send[19];
	unsigned int m_crc0,m_crc1;
	DLMS_control_field_format();
	DLMS_ReadContrHeadInf();
	if(!g_frame_Rfinish)
	{
		g_send_meterCMD_offset--;
		DLMS_SendRecRemnant();
		return;
	}
	m_send[0]=0x7E;
	m_send[1]=0xA0;//A格式
    m_send[2]=0x11;//长度
	m_send[3]=g_control_set[0];
	m_send[4]=g_control_set[1];
	m_send[5]=g_control_set[2];
	m_crc1	 =DLMS_CalculateCRC(0x0ffff,m_send+1,5);
	m_crc0	 =m_crc1;
	m_crc1	^=0xffff;
	m_send[6]=m_crc1&0xff;
	m_send[7]=(m_crc1>>8)&0xff;
	m_send[8]=0xE6;
	m_send[9]=0xE6;
	m_send[10]=0x00;
	m_send[11]=0x05;
	m_send[12]=0x01;
	m_send[13]=0x02;		
	m_send[14]=g_MCMD_high[g_send_meterCMD_offset-2];
	m_send[15]=g_MCMD_low[g_send_meterCMD_offset-2];	
	m_crc1	  =DLMS_CalculateCRC(m_crc0,m_send+6,10);
	m_crc1	 ^=0xffff;
	m_send[16]=m_crc1&0xff;
	m_send[17]=(m_crc1>>8)&0xff;		
	m_send[18]=0x7E;
	app_RMmod_Usart3_send(m_send,19);
	return;	 
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :发送AARQ帧建立应用层连接(服务端响应AARE帧)
 * @输入 :NONE
 * @输出 :NONE
 * 注:首先建立所需要的低层连接
********************************************************************/
void DLMS_SendMeterLowLevel( void )
{
	//"032110", "E6E600602080020780A109060760857405080102BE0F040D01000000065F04001802200000"
    unsigned char m_send[48];	 // AARQ /AARE
    unsigned int m_crc0,m_crc1;
	m_send[0]=0x7E;
	m_send[1]=0xA0;//A格式
    m_send[2]=0x2E;//长度
	m_send[3]=0x03; //source
	m_send[4]=0x21; //DST
	m_send[5]=0x10; // Control
	m_crc1	 =DLMS_CalculateCRC(0x0ffff,m_send+1,5);
	m_crc0	 =m_crc1;
	m_crc1	^=0xffff;
	m_send[6]=m_crc1 & 0xff;  //CRC H
	m_send[7]=(m_crc1>>8) & 0xff; //CRC L
	m_send[8]=0xE6;// 标头1
	m_send[9]=0xE6;// 标头2
	m_send[10]=0x00;// 标头3
	m_send[11]=0x60;// [application 0] implicit sequence    固有的序列
	m_send[12]=0x20;// 长度 36个 47－11＝26
	m_send[13]=0x80;// 80020780 protocol version 协议版本
	m_send[14]=0x02;//
	m_send[15]=0x07;//
	m_send[16]=0x80;//
	m_send[17]=0xA1;//元素一 Application context name 应用程序上下文名称
	m_send[18]=0x09;//长度 9个
	m_send[19]=0x06;//object identifiler  第一个class ID为6 register Activaltion   see 4.2.5  bule book 
	m_send[20]=0x07;//  长度为7
	m_send[21]=0x60;//   60 85 74 05 08 01 02 start 
	m_send[22]=0x85;//
	m_send[23]=0x74;//   
	m_send[24]=0x05;//
	m_send[25]=0x08;//
	m_send[26]=0x01;//
	m_send[27]=0x02;//  60 85 74 05 08 01 02 End 长度7 END ,长度9 END
	m_send[28]=0xBE;//元素二  implicit Association-information ::=sequence of external  导入协议信息 (扩展序列)
	m_send[29]=0x0F;// 长度 15字节
	m_send[30]=0x04;//  4 为octet string
	m_send[31]=0x0D;// octet string长度为D即 13个
	m_send[32]=0x01;//  implcit sequence 
	m_send[33]=0x00;//  octet string optional-> not  selected
	m_send[34]=0x00;//  boolean default  true->default
	m_send[35]=0x00;//  proposed quality of  service  optional->not  selected 
	m_send[36]=0x06;//  proposed-dlms -version -number  unsigned8->version
	m_send[37]=0x5F;// [application 31 ]implicit bit  string,size(24)->BER encoded
	m_send[38]=0x04;//  length=4
	m_send[39]=0x00;//  number of unused bits
	m_send[40]=0x18;// 18 02  20  see p36 <dlms gry dialog meter>  0001(read) 1(write)000=18
	m_send[41]=0x02;// 0000 001(multiple references)0(not information report)
	m_send[42]=0x20;// 001(parameterized)0 0000  长度4 END  
	m_send[43]=0x00;//00 00 Unsigned16 -> unlimited, using HDLC segmentation 
	m_send[44]=0x00;//  长度13 END ,	 长度15 END  
	m_crc1=DLMS_CalculateCRC(m_crc0,m_send+6,39);
	m_crc1^=0xffff;
	m_send[45]=m_crc1&0xff;	//CRC H  =02
	m_send[46]=(m_crc1>>8)&0xff; //CRC L = F6	
	m_send[47]=0x7E;  // END
	app_RMmod_Usart3_send(m_send,48);		 
	return;	 
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :与电表建立物理链接
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
char app_RM_build_PH_connect( void )
{
	char m_r = 0;
	char m_r_flag = 0;
	uint8_t m_offset = 0;
	app_RM_send_req_msg();
	while ( m_offset < 10 )
	{
		msleep(10);
		m_offset++;
		m_r_flag = app_RM_rec_identify_msg();
		if ( m_r_flag == 0xff )		//无数据
			continue;
		else if ( m_r_flag == 1 )	//第一步成功
		{
			app_RM_send_ack_msg();
			m_offset = 10;
		}
	}
	if ( m_r_flag == 1 )
	{
		while ( m_offset < 100 )
		{
			m_offset++;
			msleep(2);
			m_r_flag = app_RM_convert_affirm_msg();
			if ( m_r_flag == 0xff )	   //无数据
				continue;
			else if ( m_r_flag == 1 )	//建立连接成功
			{
				m_r = 1;
				m_offset = 255;
			}
		}
	}
	
	return m_r;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :与电表建立数据链路(DLMS协议)
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
char app_RM_communcation_to_meter_by_DLMS( void )
{
	char m_r = 0;
	char m_cnt = 10;
	while( m_cnt > 1 )
	{
		m_cnt--;
		os_driver_ioctl(g_usart3_dev_id, SET_EMPTY_FIFO, 0);
		if ( app_RM_build_PH_connect() == 1 )
		{
			m_cnt = 0;
			msleep(1);
			app_RM_DLMS_SNRM_send();
			msleep(10);
			if ( app_RM_DLMS_return_processed() == 1 )
			{
				if(0xF8==g_DLMS_rec_buf[13])		//旧表485 rec
					DLMS_init_old_Command();
				else if(0x3E==g_DLMS_rec_buf[13])	//旧表光电 rec
					DLMS_init_old_Command();
				else 
					DLMS_init_new_Command();
				DLMS_SendMeterLowLevel();		
				m_r = 1;
				g_send_meterCMD_offset = 1;
				app_RMmod_rev_tip_display("正在建立应用层连接...");
				return m_r;
			}
		}
	}
	return m_r;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :电表数据类型处理
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void DLMS_TypeToStrData(char *str,float *fData)
{
	//7EA01D210352856F E6E700 0C0100 09 0C07D50404010F3209FF800000 B9827E
	//char *sTmp;
	//char *sTmpAll;
	unsigned char typeN,n=1,i,j;
	unsigned char ucharTmp10[10],ucharTmp8[8],ucharTmp4[4],ucharTmp2[2];
	char int8Tmp;
	int int16Tmp,wei;
	long int int32Tmp;	
	double int64Tmp;
	unsigned long int Unsign32Tmp;	
    unsigned int unsignInt16Tmp;	
	double lngDblTmp;	
	wei = wei;
	typeN=g_DLMS_rec_buf[14];
	if(typeN==1)//对列array
	{
		n=2;
		typeN=g_DLMS_rec_buf[15];
	}
	if(typeN==5) //integer32 
	{
		for(i=0;i<4;i++)
				ucharTmp4[3-i]=g_DLMS_rec_buf[14+n+i];	 
		int32Tmp=ucharTmp4[3];//<<24)+(ucharTmp4[1]<<16)+(ucharTmp4[2]<<8)+ucharTmp4[3];
		int32Tmp<<=24;
		*fData=(float)int32Tmp;
		int32Tmp=ucharTmp4[2];
		int32Tmp<<=16;
		*fData+=(float)int32Tmp;	
		int32Tmp=ucharTmp4[1];
		int32Tmp<<=8;
		*fData+=(float)int32Tmp;	
		int32Tmp=ucharTmp4[0];		 
		*fData+=(float)int32Tmp;
	}
	else if(typeN==6) //Unsigned32  
	{
		for(i=0;i<4;i++)
			ucharTmp4[3-i]=g_DLMS_rec_buf[14+n+i];	 
		Unsign32Tmp=ucharTmp4[3];//<<24)+(ucharTmp4[1]<<16)+(ucharTmp4[2]<<8)+ucharTmp4[3];
		Unsign32Tmp<<=24;
		*fData=(float)Unsign32Tmp;
		Unsign32Tmp=ucharTmp4[2];
		Unsign32Tmp<<=16;
		*fData+=(float)Unsign32Tmp;	
		Unsign32Tmp=ucharTmp4[1];
		Unsign32Tmp<<=8;
		*fData+=(float)Unsign32Tmp;	
		Unsign32Tmp=ucharTmp4[0];		 
		*fData+=(float)Unsign32Tmp;
	}
	else if(typeN==9) //字符串
	{
		j=0;
		for(i=14+n;i<g_frame_data_len-1;i++)
		{
			str[j++]=(char)g_DLMS_rec_buf[i];
		}
		str[j]=0;
	}
	else if(typeN==13) //integer 8
	{
		int8Tmp=(char)g_DLMS_rec_buf[14+n];
		//sTmp.Format(_T("%d"),int8Tmp);
		wei=sprintf(str,"%d",int8Tmp);
		*fData=(float)int8Tmp;
	}
	else if(typeN==15) //int8
	{
		int8Tmp=(char)g_DLMS_rec_buf[14+n];
		//sTmp.Format(_T("%d"),int8Tmp);
		wei=sprintf(str,"%d",int8Tmp);
		*fData=(float)int8Tmp;
	}
	else if(typeN==16) //int16
	{
		for(i=0;i<2;i++)
			ucharTmp2[1-i]=g_DLMS_rec_buf[14+n+i];	 	 	
		int16Tmp=ucharTmp2[1];
		int16Tmp<<=8;
		*fData=(float)int16Tmp;	
		int16Tmp=ucharTmp2[0];		 
		*fData+=(float)int16Tmp;
	}
	else if(typeN==17) //unsigned8
	{		 
		//sTmp.Format(_T("%d"),gBufComA[gPosStart+14+n]);
		wei=sprintf(str,"%d",g_DLMS_rec_buf[14+n]);
	}
	else if(typeN==18) //unsigned16
	{
		for(i=0;i<2;i++)
			ucharTmp2[1-i]=g_DLMS_rec_buf[14+n+i];	 
		unsignInt16Tmp=ucharTmp2[1];
		unsignInt16Tmp<<=8;
		*fData=(float)unsignInt16Tmp;	
		unsignInt16Tmp=ucharTmp2[0];		 
		*fData+=(float)unsignInt16Tmp;
	}
	else if(typeN==20) //int64
	{
		for(i=0;i<8;i++)
			ucharTmp8[7-i]=g_DLMS_rec_buf[14+n+i];	 
		int64Tmp=*((unsigned long int *)ucharTmp8);	///
		//sTmp.Format(_T("%d"),int64Tmp);
		//wei=sprintf(str,"%d",int64Tmp);///
		*fData=(float)int64Tmp;	 
	}
	else if(typeN==21) //unsigned64
	{   
		for(i=0;i<8;i++)
			ucharTmp10[7-i]=g_DLMS_rec_buf[14+n+i];
		ucharTmp10[8]=0;
	    ucharTmp10[9]=0;
		lngDblTmp=*((double *)ucharTmp10);	
		//sTmp.Format(_T("%f"),lngDblTmp);
		//wei=sprintf(str,"%f.3",lngDblTmp);
		*fData=(float)lngDblTmp;		 
	}	  	 	    	    
	
	return;
} 
/********************************************************************
 * @作者 :周炳权
 * @功能 :读取电表数据处理
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_RM_meter_data_handle( uint8_t m_moffset )
{
	char chSZ[30];	
	float fdata;
	char m_RM_time_buf[15]	= { 0, };
	int  m_temp = 0;
	unsigned long int m_idata = 0;
	if(2==m_moffset)//表地址
	{	
		global_memcpy(m_meter_data.meter_Addr, &g_DLMS_rec_buf[16], sizeof(char)*8);
		app_RMmod_meter_addr_display(m_meter_data.meter_Addr);
	}
	else if(3==m_moffset)//表逻辑地址
	{		
		g_meter_logic_addr=g_DLMS_rec_buf[16];
		app_RMmod_logic_addr_display(g_meter_logic_addr);
	}	 
	else if(4==m_moffset)//正向有功总
	{		
	}
	else if(5==m_moffset)//正向有功峰  0000000000028B07H=16.6663kWh
	{		 
		DLMS_TypeToStrData(chSZ,&fdata);
		fdata*=0.0001;
		m_fdata[0] = 0;
		m_fdata[1] = fdata;
		global_ftoa(fdata, m_meter_data.zy_power_F, 4);
		global_ftoa(m_fdata[0], m_meter_data.zy_power_J, 4);
		app_RMmod_rev_data_display(m_meter_data.zy_power_F,EDIT_F_1);
		app_RMmod_rev_data_display(m_meter_data.zy_power_J,EDIT_J_1);
	}	
	else if(6==m_moffset)//正向有功平
	{	
		DLMS_TypeToStrData(chSZ,&fdata);	
		fdata*=0.0001;
		m_fdata[2] = fdata;
		global_ftoa(fdata, m_meter_data.zy_power_P, 4);
		app_RMmod_rev_data_display(m_meter_data.zy_power_P,EDIT_P_1);
	}
	else if(7==m_moffset)//正向有功谷
	{		
		DLMS_TypeToStrData(chSZ,&fdata);	
		fdata*=0.0001;
		m_fdata[3] = fdata;
		global_ftoa(fdata, m_meter_data.zy_power_G, 4);
		global_ftoa((m_fdata[0]+m_fdata[1]+m_fdata[2]+m_fdata[3]), m_meter_data.zy_power_ALL, 4);
		app_RMmod_rev_data_display(m_meter_data.zy_power_G,EDIT_G_1);
		app_RMmod_rev_data_display(m_meter_data.zy_power_ALL,EDIT_Z_1);
	}
	else if(8==m_moffset)//正向无功总
	{		
	} 
	else if(9==m_moffset)//正向无功峰
	{		
		DLMS_TypeToStrData(chSZ,&fdata);
		fdata*=0.0001;
		m_fdata[0] = 0;
		m_fdata[1] = fdata;
		global_ftoa(fdata, m_meter_data.zw_power_F, 4);
		global_ftoa(m_fdata[0], m_meter_data.zw_power_J, 4);
		app_RMmod_rev_data_display(m_meter_data.zw_power_F,EDIT_F_2);
		app_RMmod_rev_data_display(m_meter_data.zw_power_J,EDIT_J_2);
	} 
	else if(10==m_moffset)//正向无功平
	{		
		DLMS_TypeToStrData(chSZ,&fdata);	
		fdata*=0.0001;
		m_fdata[2] = fdata;
		global_ftoa(fdata, m_meter_data.zw_power_P, 4);
		app_RMmod_rev_data_display(m_meter_data.zw_power_P,EDIT_P_2);
	} 
	else if(11==m_moffset)//正向无功谷 
	{		
		DLMS_TypeToStrData(chSZ,&fdata);	
		fdata*=0.0001;
		m_fdata[3] = fdata;
		global_ftoa(fdata, m_meter_data.zw_power_G, 4);
		global_ftoa((m_fdata[0]+m_fdata[1]+m_fdata[2]+m_fdata[3]), m_meter_data.zw_power_ALL, 4);
		app_RMmod_rev_data_display(m_meter_data.zw_power_G,EDIT_G_2);
		app_RMmod_rev_data_display(m_meter_data.zw_power_ALL,EDIT_Z_2);
	} 
	else if(12==m_moffset)//反向有功总
	{		
	}
	else if(13==m_moffset)//反向有功峰
	{		 
		DLMS_TypeToStrData(chSZ,&fdata);
		fdata*=0.0001;
		m_fdata[0] = 0;
		m_fdata[1] = fdata;
		global_ftoa(fdata, m_meter_data.fy_power_F, 4);
		global_ftoa(m_fdata[0], m_meter_data.fy_power_J, 4);
		app_RMmod_rev_data_display(m_meter_data.fy_power_F,EDIT_F_3);
		app_RMmod_rev_data_display(m_meter_data.fy_power_J,EDIT_J_3);
	}	
	else if(14==m_moffset)//反向有功平
	{	
		DLMS_TypeToStrData(chSZ,&fdata);	
		fdata*=0.0001;
		m_fdata[2] = fdata;
		global_ftoa(fdata, m_meter_data.fy_power_P, 4);
		app_RMmod_rev_data_display(m_meter_data.fy_power_P,EDIT_P_3);
	}
	else if(15==m_moffset)//反向有功谷
	{		
		DLMS_TypeToStrData(chSZ,&fdata);	
		fdata*=0.0001;
		m_fdata[3] = fdata;
		global_ftoa(fdata, m_meter_data.fy_power_G, 4);
		global_ftoa((m_fdata[0]+m_fdata[1]+m_fdata[2]+m_fdata[3]), m_meter_data.fy_power_ALL, 4);
		app_RMmod_rev_data_display(m_meter_data.fy_power_G,EDIT_G_3);
		app_RMmod_rev_data_display(m_meter_data.fy_power_ALL,EDIT_Z_3);
	}
	else if(16==m_moffset)//反向无功总
	{		
	} 
	else if(17==m_moffset)//反向无功峰
	{		
		DLMS_TypeToStrData(chSZ,&fdata);
		fdata*=0.0001;
		m_fdata[0] = 0;
		m_fdata[1] = fdata;
		global_ftoa(fdata, m_meter_data.fw_power_F, 4);
		global_ftoa(m_fdata[0], m_meter_data.fw_power_J, 4);
		app_RMmod_rev_data_display(m_meter_data.fw_power_F,EDIT_F_4);
		app_RMmod_rev_data_display(m_meter_data.fw_power_J,EDIT_J_4);
	} 
	else if(18==m_moffset)//反向无功平
	{		
		DLMS_TypeToStrData(chSZ,&fdata);	
		fdata*=0.0001;
		m_fdata[2] = fdata;
		global_ftoa(fdata, m_meter_data.fw_power_P, 4);
		app_RMmod_rev_data_display(m_meter_data.fw_power_P,EDIT_P_4);
	} 
	else if(19==m_moffset)//反向无功谷 
	{		
		DLMS_TypeToStrData(chSZ,&fdata);
		fdata*=0.0001;
		m_fdata[3] = fdata;
		global_ftoa(fdata, m_meter_data.fw_power_G, 4);
		global_ftoa((m_fdata[0]+m_fdata[1]+m_fdata[2]+m_fdata[3]), m_meter_data.fw_power_ALL, 4);
		app_RMmod_rev_data_display(m_meter_data.fw_power_G,EDIT_G_4);
		app_RMmod_rev_data_display(m_meter_data.fw_power_ALL,EDIT_Z_4);
	} 
	else if(20==m_moffset)//总断相次数
	{		
		DLMS_TypeToStrData(chSZ,&fdata);
		m_idata = (unsigned long int)fdata;
		global_ltoa(m_idata, m_meter_data.lost_ALL);
		app_RMmod_rev_data_display(m_meter_data.lost_ALL,EDIT_SY_ALL);
	} 
	else if(21==m_moffset)//A相断相次数
	{		 
		DLMS_TypeToStrData(chSZ,&fdata);
		m_idata = (unsigned long int)fdata;
		global_ltoa(m_idata, m_meter_data.lost_A);
		app_RMmod_rev_data_display(m_meter_data.lost_A,EDIT_SY_A);
	} 
	else if(22==m_moffset)//B相断相次数
	{		 
		DLMS_TypeToStrData(chSZ,&fdata);
		m_idata = (unsigned long int)fdata;
		global_ltoa(m_idata, m_meter_data.lost_B);
		app_RMmod_rev_data_display(m_meter_data.lost_B,EDIT_SY_B);
	}
	else if(23==m_moffset)//C相断相次数
	{		
		DLMS_TypeToStrData(chSZ,&fdata);
		m_idata = (unsigned long int)fdata;
		global_ltoa(m_idata, m_meter_data.lost_C);
		app_RMmod_rev_data_display(m_meter_data.lost_C,EDIT_SY_C);
	} 
	else if(24==m_moffset)//日期时间
	{		
		m_temp = g_DLMS_rec_buf[16]*256+g_DLMS_rec_buf[17];
		global_itoa(m_temp, m_RM_time_buf);
		m_meter_data.meter_time[0] =m_RM_time_buf[2];//年
		m_meter_data.meter_time[1] =m_RM_time_buf[3];
		m_meter_data.meter_time[2] = '-';
		m_meter_data.meter_time[3] =(g_DLMS_rec_buf[18] / 10) + '0';//月
		m_meter_data.meter_time[4] =(g_DLMS_rec_buf[18] % 10) + '0';
		m_meter_data.meter_time[5] = '-';
		m_meter_data.meter_time[6] =(g_DLMS_rec_buf[19] / 10) + '0';//日
		m_meter_data.meter_time[7] =(g_DLMS_rec_buf[19] % 10) + '0';
		m_meter_data.meter_time[8] = ' ';
		//g_DLMS_rec_buf[20]; 星期几
		m_meter_data.meter_time[9] =(g_DLMS_rec_buf[21] / 10) + '0';//时
		m_meter_data.meter_time[10]=(g_DLMS_rec_buf[21] % 10) + '0';
		m_meter_data.meter_time[11]= ':';
		m_meter_data.meter_time[12]=(g_DLMS_rec_buf[22] / 10) + '0';//分
		m_meter_data.meter_time[13]=(g_DLMS_rec_buf[22] % 10) + '0';
		m_meter_data.meter_time[14]= ':';
		m_meter_data.meter_time[15]=(g_DLMS_rec_buf[23] / 10) + '0';//秒
		m_meter_data.meter_time[16]=(g_DLMS_rec_buf[23] % 10) + '0';
		m_meter_data.meter_time[17]= '\0';
		app_RMmod_rev_data_display(m_meter_data.meter_time,EDIT_M_TIME);
		DLMS_DisconetCom(); //断开通讯连接
		g_send_meterCMD_offset = 25;
	} 

	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :读取电表相关电参数数值(DLMS协议)
 * @输入 :NONE
 * @输出 :NONE
 *  注:应用直接调用本函数进行西门子电表数据的读取;
********************************************************************/
void app_RM_readout_to_meterARG( void )
{
	if ( app_RM_communcation_to_meter_by_DLMS() == 1 )
	{
		msleep(10);
		if ( app_RM_DLMS_return_processed() == 1 )
		{
			app_RMmod_rev_tip_display("正在读取电能表数据...");
			while( g_send_meterCMD_offset < 24 )
			{
				g_send_meterCMD_offset++;
				DLMS_SendMeterInf();
				msleep(10);
				if ( app_RM_DLMS_return_processed() == 1 )
				{
					app_RM_meter_data_handle(g_send_meterCMD_offset);
				}
			}
			app_RMmod_rev_tip_display("读取电能表数据完成!");
			app_RMmod_protocol_tip_display();
			msleep(10);
			if ( app_RM_DLMS_return_processed() == 1 )
			{
				//收到通讯结束确认
			}
		}
	}
	else 
	{
		app_RMmod_rev_tip_display("与电能表建立通讯失败...");
	}
	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :读取电表相关电参数数值(IEC62056-21协议)
 * @输入 :NONE
 * @输出 :NONE
 *  注:应用直接调用本函数进行西门子电表数据的读取;
********************************************************************/
void app_RM_readout_data_IEC( void )
{
//	char m_r = 0;
	char m_r_flag = 0;
	uint8_t m_offset = 0;
	app_RM_send_req_msg();
	while ( m_offset < 5 )
	{
		msleep(50);
		m_offset++;
		m_r_flag = app_RM_rec_identify_msg();
		if ( m_r_flag == 0xff )		//无数据
			continue;
		else if ( m_r_flag == 1 )	//第一步成功
		{
			app_RM_send_IECack_msg();
			break;
		}
	}
	if ( m_r_flag == 1 )
	{
		

	}

	return;
}




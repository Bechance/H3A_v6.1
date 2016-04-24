/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:触摸屏驱动
* 创建时间:2010-10-26
* 所属公司 :科荟
* 文件名称:touchdriver.c
* 创建人 :揭成
* 功能描述:获取触摸屏幕坐标
*----------------------------------------------------------------
修改记录:

修改目的:

修改日期:

修改人:

*********************************************************************************************************
*/
#include "../Osdriver.h"
static dev_id_t         m_serialC_dev_id = INVALID_DEV_ID;
touch_data_t     		m_touch_data = {0, };
static uint8_t			m_touch_refresh = 0;
static uint32_t         m_time_tick = 0;
char                    m_touch_hz_box[30];
BOOL                    m_touch_hz_flag = FAIL;
uint16_t				AX_S, AY_S, AX_E, AY_E;	//设置手写区域的坐标
const unsigned char 	CRCTable[256] =
{
	0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15, 0x38, 0x3F, 0x36, 0x31,
	0x24, 0x23, 0x2A, 0x2D, 0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65,
	0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D, 0xE0, 0xE7, 0xEE, 0xE9,
	0xFC, 0xFB, 0xF2, 0xF5, 0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
	0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85, 0xA8, 0xAF, 0xA6, 0xA1,
	0xB4, 0xB3, 0xBA, 0xBD, 0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2,
	0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA, 0xB7, 0xB0, 0xB9, 0xBE,
	0xAB, 0xAC, 0xA5, 0xA2, 0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
	0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32, 0x1F, 0x18, 0x11, 0x16,
	0x03, 0x04, 0x0D, 0x0A, 0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42,
	0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A, 0x89, 0x8E, 0x87, 0x80,
	0x95, 0x92, 0x9B, 0x9C, 0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
	0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC, 0xC1, 0xC6, 0xCF, 0xC8,
	0xDD, 0xDA, 0xD3, 0xD4, 0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C,
	0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44, 0x19, 0x1E, 0x17, 0x10,
	0x05, 0x02, 0x0B, 0x0C, 0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,
	0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B, 0x76, 0x71, 0x78, 0x7F,
	0x6A, 0x6D, 0x64, 0x63, 0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B,
	0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13, 0xAE, 0xA9, 0xA0, 0xA7,
	0xB2, 0xB5, 0xBC, 0xBB, 0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
	0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB, 0xE6, 0xE1, 0xE8, 0xEF,
	0xFA, 0xFD, 0xF4, 0xF3
};
/*eph1200  通讯常数*/
#define   EPH1200_PACT_HEAD  0x50
/*IO设置*/

void WriteSwitchModePacket( unsigned char bit, uint32_t PaintMode );
void WritePrevCalibratePacket( unsigned char bbit,uint32_t Coordinate );

/********************************************************************
 * @创建人: 揭成
 * @功能:
 * @输入: NONE
 * @输出: NONE
********************************************************************/
void touch_driver_open( void )
{
	GpioDataRegs.GPBCLEAR.bit.GPIO59 =1;
    BSP_Dly(10);
	GpioDataRegs.GPBSET.bit.GPIO59 = 1;
	return;
}
/*eph1200 设置*/

#define PACKET_HEADER		   			0x50     // packet header
#define COMMAND_TYPE					0x10     //set character recognize mode
#define COMMAND_INKING_SWITCH			0x14     // inking or not transmit switch
#define COMMAND_PEN_WAITING_TIME		0x1A     // pen up waiting time 
#define COMMAND_RECOGNIZE_BEFORE_TIMEOUT	0x1B 	// recognition before timeout
#define COMMAND_HOST_READY       		0x1C 	// host ready,
#define COMMAND_TAP_WAKEUP				0x1D
#define COMMAND_PENUP_TIME_OFFSET		0x1E //TouchPane
#define COMMAND_WAKEUP                  0x33
#define COMMAND_VERSION					0x40
#define COMMAND_RETURN_CODE				0x41 //
#define COMMAND_SOFTWARE_RESET			0x42
#define COMMAND_ABORT_INKING			0x43 //
#define COMMAND_CALIBRATE				0x44
#define COMMAND_SET_WRITING_AREA		0x46
#define COMMAND_SWITCH_RECOGorGRA		0x49 // recognized or graphic
#define COMMAND_POWER_SAVING			0x4a  // power saving mode
#define COMMAND_AD_SET                  0x4b
#define COMMAND_ROTATE_COORDINATE       0x4C
#define COMMAND_CHECKSUM				0xF0  // output PROM/DROM checksum

//****************************************************
#define ACK_ERROR                       0x00
#define ACK_INKING                      0x16
#define ACK_TOUCH_BUTTON                0x17
#define ACK_RECOGNIZED                  0x18
#define COMMAND_EXIT_POWER_SAVING       0x33
#define COMMAND_INIT_POWER_ON           0x42
#define SIZE_OF_COMMAND_PACKET			8
#define	SIZE_OF_ACK_PACKET				32

#define CHINESE_1_TYPE              0x0001
#define CHINESE_2_TYPE              0x0002
#define UPPER_ALPHA_TYPE            0x0004
#define NUMA_TYPE                   0x0008
#define SYMBOL_TYPE   				0x0010
#define GESTURE_TYPE                0x0020
#define LOWER_ALPHA_TYPE            0x0100
#define SYM_TYPE                	(SYMBOL_TYPE|GESTURE_TYPE)
#define LOW_TYPE					(LOWER_ALPHA_TYPE|GESTURE_TYPE)
#define UPPER_TYPE         			(UPPER_ALPHA_TYPE|GESTURE_TYPE)
#define CHINESE_TYPE				(CHINESE_1_TYPE|CHINESE_2_TYPE|GESTURE_TYPE)
#define NUM_TYPE				 	(NUMA_TYPE|GESTURE_TYPE)
#define ALPHA_TYPE              	(UPPER_ALPHA_TYPE|LOWER_ALPHA_TYPE|GESTURE_TYPE)
#define ASCII_TYPE					(ALPHA_TYPE|NUMA_TYPE|SYMBOL_TYPE)
#define ALL_TYPE					(CHINESE_TYPE|ASCII_TYPE|GESTURE_TYPE)//


/********************************************************************
 * @创建人 :揭成
 * @功能: 驱动参数设置接口
 *
 * @输入:cmd 控制命令  arg控制参数
 *
 * @输出:TURE 成功 FAIL 失败
********************************************************************/
void  touch_driver_ioctl( uint8_t cmd, void*arg )
{
//	uint16_t *pdata=(uint16_t*)arg;
	char *pch=(char*)arg;
	char  temp[3]="";
	Bsize_t offset,foffset;
 	switch ( cmd )
	{
	case TOUCH_CMD_SW_WRITE_MOD:
		//WriteSwitchModePacket(1,0);
         break;
	case TOUCH_CMD_SW_TOUCH_MOD:
	//	WriteSwitchModePacket(1,1);
         break;
	case TOUCH_CMD_GET_HZ_BOX:	 
		 foffset =0;
		 offset =0;
		 while(1)
		 {
		 	 if((uint8_t)m_touch_hz_box[foffset]>0x80)
		 	 {
				 temp[1] =m_touch_hz_box[foffset];
				 temp[0] =m_touch_hz_box[foffset+1];
				 global_memcpy(pch,temp,2);
				 pch+=2;
				 if(++offset>10)break;
				 foffset+=2;
			 }
			 else
			 {
					foffset+=1;
			 }
			 if(foffset>26)break;
		 }
		 break;
	case TOUCH_CMD_GET_HZ_FLAG:
		 *((BOOL*)arg) = m_touch_hz_flag;
		 m_touch_hz_flag = FAIL;
		 break;
	case TOUCH_CMD_SET_WRITE_AREA:
		 break;
	default:
		break;
	}   
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能:没有定义
 * @输入:buf 待写缓冲 size:要写入大小
 * @输出:实际写入字节
********************************************************************/
Bsize_t touch_driver_write( uint8_t*buf, Bsize_t size , void*pos )
{
	return 0;
}

uint8_t GenerateCRC( uint8_t *Packet, uint16_t size )
{
	int i;
	unsigned char value;
	value = 0;
	for ( i = 0;i < size - 1;i++ )
	{
		value ^= Packet[i];
		value = CRCTable[value];
	}
	return value;
}
/*
void WriteCheckSumPacket( unsigned char bit )
{
	if ( bit )
	{
		unsigned char version;
		unsigned char Packet[SIZE_OF_COMMAND_PACKET];
		unsigned char PktAck[SIZE_OF_ACK_PACKET];
		unsigned char CRCValue;

		//::ZeroMemory(Packet,SIZE_OF_COMMAND_PACKET);
		Packet[0] = PACKET_HEADER;					//
		Packet[1] = COMMAND_CHECKSUM;				//
		Packet[2] = 4;								//
		Packet[3] = 0xff;							//
		Packet[4] = 0xff;							//
		Packet[5] = 0xff;                           //
		Packet[6] = 0xff;							//
		
		CRCValue = Packet[7] = GenerateCRC( Packet, 8 );
		os_driver_write(m_serialB_dev_id,( char* )Packet,SIZE_OF_COMMAND_PACKET, 0 );
	}
}
void ComandWakeUp( unsigned char bit )   //OX33

{
	if ( bit )
	{
		unsigned char Packet[SIZE_OF_COMMAND_PACKET];
		Packet[0] = 0x50;//PACKET_HEADER;
		Packet[1] = 0x33;//PACKET_COMMAND_TYPE //wake up command
		Packet[2] = 0x04;
		Packet[3] = 0x00;
		Packet[4] = 0x00;
		Packet[5] = 0x00;
		Packet[6] = 0x00;
		Packet[7] = 0xC7;
		os_driver_write(m_serialB_dev_id,( char* )Packet,SIZE_OF_COMMAND_PACKET, 0 );
	}

}

// select the writing type for eph1200;
void WriteTypePacket( unsigned char bit, uint16_t RecogType )
//void WriteTypePacket(uint16_t globalRecogType)
{
	if ( bit )
	{
		unsigned   char Packet[SIZE_OF_COMMAND_PACKET];
		unsigned   char PktAck[SIZE_OF_ACK_PACKET];
		unsigned   char CRCValue;

		Packet[0] = PACKET_HEADER;					//
		Packet[1] = COMMAND_TYPE;		//
		Packet[2] = 4;								//
		Packet[3] = ( unsigned char )RecogType & 0x00ff;    //   low byte is used
		Packet[4] = ( unsigned char )( RecogType >> 8 ) & 0x00ff;//  high byte is used
		Packet[5] = 0xff;
		Packet[6] = 0xff;
		CRCValue = Packet[7] = GenerateCRC(( char * )Packet, 8 );
		os_driver_write(m_serialB_dev_id,( char* )Packet,SIZE_OF_COMMAND_PACKET, 0 );
	}
}
// init the eph1200 , what code will be return from eph1200;
void WriteChangeCodeReturn( unsigned char bit, unsigned char CodeReturn )
{
	if ( bit )
	{
		unsigned   char Packet[SIZE_OF_COMMAND_PACKET];
		unsigned   char PktAck[SIZE_OF_ACK_PACKET];
		unsigned   char CRCValue;

		Packet[0] = PACKET_HEADER;					//
		Packet[1] = COMMAND_RETURN_CODE;	//
		Packet[2] = 4;								//
		Packet[3] = CodeReturn;						//
		Packet[4] = 0xff;							//
		Packet[5] = 0xff;
		Packet[6] = 0xff;
		CRCValue = Packet[7] = GenerateCRC( Packet, 8 );
		os_driver_write(m_serialB_dev_id,( char* )Packet,SIZE_OF_COMMAND_PACKET, 0 );
	}
}

// when writing , the inking coordinate will be translated to host or not
void  WriteInkingPacket( unsigned char bit, unsigned char SendInking )
{
	if ( bit )
	{
		unsigned   char Packet[SIZE_OF_COMMAND_PACKET];
		unsigned   char PktAck[SIZE_OF_ACK_PACKET];
		unsigned   char CRCValue;

		Packet[0] = PACKET_HEADER;            //
		Packet[1] = COMMAND_INKING_SWITCH;    //
		Packet[2] = 4;                        //
		Packet[3] = SendInking;              //
		Packet[4] = 0xff;                     //
		Packet[5] = 0xff;
		Packet[6] = 0xff;
		CRCValue = Packet[7] = GenerateCRC( Packet, 8 );
		os_driver_write(m_serialB_dev_id,( char* )Packet,SIZE_OF_COMMAND_PACKET, 0 );
	}
}

// reset the eph1200, after the function , we must set writing area and writing type
void  WriteResetPacket( unsigned char RST )
{
	if ( RST )
	{
		unsigned   char Packet[SIZE_OF_COMMAND_PACKET];
		unsigned   char PktAck[SIZE_OF_ACK_PACKET];
		unsigned   char CRCValue;
		Packet[0] = PACKET_HEADER;					//
		Packet[1] = COMMAND_SOFTWARE_RESET;			//
		Packet[2] = 4;								//
		Packet[3] = 0xff;							//
		Packet[4] = 0xff;							//
		Packet[5] = 0xff;
		Packet[6] = 0xff;
		CRCValue = Packet[7] = GenerateCRC( Packet, 8 );
		os_driver_write(m_serialB_dev_id,( char* )Packet,SIZE_OF_COMMAND_PACKET, 0 );
	}
}
// write time out , we use the default time;
void WriteTimeOutPacket( unsigned char bit, uint16_t slice_count )  // 1A
{
	if ( bit )
	{
		unsigned   char Packet[SIZE_OF_COMMAND_PACKET];
		unsigned   char PktAck[SIZE_OF_ACK_PACKET];
		unsigned   char CRCValue;
		Packet[0] = PACKET_HEADER;					//㏑
		Packet[1] = COMMAND_PENUP_TIME_OFFSET;		//㏑
		Packet[2] = 4;								//㏑
		Packet[3] = ( unsigned char )slice_count;	//
		Packet[4] = 0xff;							//
		Packet[5] = 0xff;
		Packet[6] = 0xff;
		CRCValue = Packet[7] = GenerateCRC( Packet, 8 );
		os_driver_write(m_serialB_dev_id,( char* )Packet,SIZE_OF_COMMAND_PACKET, 0 );
	}
}
// ilde control , if we want to save power , we can use the function , if
// use the touch panel again , we must send wake up eph1200 again ;
void WritePowerSavingPacket( unsigned char bit, uint16_t TouchToWake, uint16_t Idletime )
{
	if ( bit )
	{
		unsigned		char Packet[SIZE_OF_COMMAND_PACKET];
		unsigned		char PktAck[SIZE_OF_ACK_PACKET];
		unsigned		char CRCValue;
		Packet[0] = PACKET_HEADER;					//
		Packet[1] = COMMAND_POWER_SAVING;	//
		Packet[2] = 4;								//
		Packet[3] = TouchToWake;					//
		Packet[4] = Idletime;						//
		Packet[5] = 0xff;
		Packet[6] = 0xff;
		CRCValue = Packet[7] = GenerateCRC( Packet, 8 );
		os_driver_write(m_serialB_dev_id,( char* )Packet,SIZE_OF_COMMAND_PACKET, 0 );	}
}

// recognize or graphic mode; use eph1200 in recognize mode
void WriteSwitchModePacket( unsigned char bit, uint32_t PaintMode )
{
	if ( bit )
	{
		unsigned   char Packet[SIZE_OF_COMMAND_PACKET];
		unsigned   char PktAck[SIZE_OF_ACK_PACKET];
		unsigned char CRCValue;

		Packet[0] = PACKET_HEADER;					//
		Packet[1] = COMMAND_SWITCH_RECOGorGRA;	//
		Packet[2] = 4;								//
		Packet[3] = PaintMode;		//当为0时,为识别模式,当为1时为图形模式用于按键操作
//  Packet[3] = 0x01;
		Packet[4] = 0xff;							//
		Packet[5] = 0xff;
		Packet[6] = 0xff;
		CRCValue = Packet[7] = GenerateCRC( Packet, 8 );
		os_driver_write(m_serialB_dev_id,( char* )Packet,SIZE_OF_COMMAND_PACKET, 0 );	}
}
// abort current inking , no use the function here ;
void WriteAbortInkingPacket( unsigned char bit )
{
	if ( bit )
	{
		unsigned   char Packet[SIZE_OF_COMMAND_PACKET];
		unsigned   char PktAck[SIZE_OF_ACK_PACKET];
		unsigned char CRCValue;

		Packet[0] = PACKET_HEADER;					//
		Packet[1] = COMMAND_ABORT_INKING; //PACKET_COMMAND_ABORT_INKING		//
		Packet[2] = 4;								//
		Packet[3] = 0xff;								//
		Packet[4] = 0xff;							//
		Packet[5] = 0xff;
		Packet[6] = 0xff;
		CRCValue = Packet[7] = GenerateCRC( Packet, 8 );
		os_driver_write(m_serialB_dev_id,( char* )Packet,SIZE_OF_COMMAND_PACKET, 0 );	}
}

// calibrate coordinate for the panel; if we calibrate the panel ,
// we must set the writing area again;
void WriteCalibratePacket( unsigned char bit )
{
	if ( bit )
	{
		unsigned   char Packet[SIZE_OF_COMMAND_PACKET];
		unsigned   char PktAck[SIZE_OF_ACK_PACKET];
		unsigned char CRCValue;

		Packet[0] = PACKET_HEADER;					//
		Packet[1] = COMMAND_CALIBRATE;		//
		Packet[2] = 4;								//
		Packet[3] = 0xff;							//
		Packet[4] = 0xff;							//
		Packet[5] = 0xff;
		Packet[6] = 0xff;
		CRCValue = Packet[7] = GenerateCRC( Packet, 8 );
		os_driver_write(m_serialB_dev_id,( char* )Packet,SIZE_OF_COMMAND_PACKET, 0 );
	}
}
// set the current writing area,
void WritePrevCalibratePacket( unsigned char bit,
							   uint32_t Coordinate )
{
	if ( bit )
	{
		unsigned   char Packet[SIZE_OF_COMMAND_PACKET];
		unsigned   char PktAck[SIZE_OF_ACK_PACKET];
		unsigned char CRCValue;
		Packet[0] = PACKET_HEADER;					// 
		Packet[1] = COMMAND_SET_WRITING_AREA;	//
		Packet[2] = 4;	//0x644DD39A

		if (( 0xFF == AX_S ) && ( 0xFF == AY_S ) && ( 0xFF == AX_E ) && ( 0xFF == AY_E ) )	//判断是设为点阵否
		{
			Packet[3] = 0xFF; //top left x=设置显示手写区域起始地址-16,					//把计
			Packet[4] = 0xFF; //top left y=设置显示手写区域起始地址+34,如有标题显示,则要多+20								//ぃì干0xff
			Packet[5] = 0xFF;  //bottom right x=设置显示手写区域的X起点+宽度-32
			Packet[6] = 0xFF;

		}
		else	//否则设为手写区域
		{
			Packet[3] = AX_S / 1.23; //top left x=设置显示手写区域起始地址-16,					//把计
			Packet[4] = AY_S / 0.94;//(240/254); //top left y=设置显示手写区域起始地址+34,如有标题显示,则要多+20								//ぃì干0xff
			Packet[5] = AX_E / 1.23;  //bottom right x=设置显示手写区域的X起点+宽度-32
			Packet[6] = AY_E / 0.94;//(240/254);

		}

		CRCValue = Packet[7] = GenerateCRC( Packet, 8 );
		os_driver_write(m_serialB_dev_id,( char* )Packet,SIZE_OF_COMMAND_PACKET, 0 );
	}
}
//==================================================================
// use the function , we can get the version of eph1200
void WriteVersionPacket( unsigned char bit )
{
	if ( bit )
	{
		unsigned char Packet[SIZE_OF_COMMAND_PACKET];
		unsigned char PktAck[SIZE_OF_ACK_PACKET];
		unsigned char CRCValue;

		Packet[0] = PACKET_HEADER;					//
		Packet[1] = COMMAND_VERSION;		//
		Packet[2] = 4;								//
		Packet[3] = 0xff;						  //
		Packet[4] = 0xff;							//
		Packet[5] = 0xff;
		Packet[6] = 0xff;
		CRCValue = Packet[7] = GenerateCRC( Packet, 8 );
		os_driver_write(m_serialB_dev_id,( char* )Packet,SIZE_OF_COMMAND_PACKET, 0 );
	}
}
*/
//==============================================================
//Set Panl X/Y的手写方向
void WritePanlXY( unsigned char bit )
{
	unsigned char Packet[SIZE_OF_COMMAND_PACKET];
//	unsigned char PktAck[SIZE_OF_ACK_PACKET];
	unsigned char CRCValue;
	if ( bit )
	{
		Packet[0] = PACKET_HEADER;					//
		Packet[1] = COMMAND_ROTATE_COORDINATE;		//
		Packet[2] = 0x04;								//
		Packet[3] = 0x10;						  //
		Packet[4] = 0xff;							//
		Packet[5] = 0xff;
		Packet[6] = 0xff;
		CRCValue = Packet[7] = GenerateCRC( Packet, 8 );
        CRCValue =CRCValue;
		os_driver_write(m_serialC_dev_id,(uint8_t* )Packet,SIZE_OF_COMMAND_PACKET, 0 );
	}
}
/********************************************************************
 * @创建人: 周炳权
 * @功能: 设置抬笔等待时间
 * @输入:	time_num : (默认0为600ms,1~10有效，其它数无限等待),
 *			每1表示200ms,1S则是5
 * @输出:	NONE
********************************************************************/
void SetPenUpTime( unsigned char time_num )
{
	unsigned char Packet[SIZE_OF_COMMAND_PACKET];
	unsigned char CRCValue;
	if ( time_num )
	{
		Packet[0] = PACKET_HEADER;					//
		Packet[1] = COMMAND_PEN_WAITING_TIME;		//
		Packet[2] = 0x04;								//
		Packet[3] = time_num;						  //
		Packet[4] = 0xff;							//
		Packet[5] = 0xff;
		Packet[6] = 0xff;
		CRCValue = Packet[7] = GenerateCRC( Packet, 8 );
        CRCValue =CRCValue;
		os_driver_write(m_serialC_dev_id,(uint8_t* )Packet,SIZE_OF_COMMAND_PACKET, 0 );
	}
}
/******************以上EPH1610 命令发送****************************************************************/
/********************************************************************
 * @创建人: 揭成
 * @功能: 获得当前键值
 *
 * @输入:定时轮寻，20MS检测一次看是否有触摸数据

 * @输出:实际读出大小
********************************************************************/
Bsize_t m_cnt =0;
void touch_driver_read_touch_data(void)
{
	Bsize_t size;
	uint8_t pdata[20];
	eph1200_pact_t*phead;
	uint32_t tick = OSTimeGet();;
	size = os_driver_read(m_serialC_dev_id,pdata,3, 0 );
	if ( size != 3 )return;
	phead = ( eph1200_pact_t* )pdata;
	if ( phead->head != EPH1200_PACT_HEAD )return;
	if ( phead->cmd == 0x16||phead->cmd == 0x17)
	{
		size = os_driver_read(
				   m_serialC_dev_id,
				   pdata + sizeof( eph1200_pact_t ),
				   phead->lenth + 1, 0 );
		if ((size ==  phead->lenth + 1) )
		{
			if ( pdata[4] == 0xff||pdata[3] == 0xff )
			{
				m_touch_data.state =0;
				m_touch_refresh = 1;
				m_touch_data.x =7777;
				m_touch_data.y =7777;
				os_driver_ioctl(m_serialC_dev_id,SET_EMPTY_FIFO,0);
			}
			else //if(tick-m_time_tick>50)
			{
				m_touch_refresh = 1;
				m_touch_data.y= pdata[4];
				m_touch_data.x= pdata[3];
				m_touch_data.state =1;
				m_time_tick = tick;
				m_cnt++;
			}
		}
	}
	else if(phead->cmd == 0x18)
	{
		GLOBAL_MEMCLR(m_touch_hz_box,30);
		size = os_driver_read(
		   m_serialC_dev_id,(uint8_t*)m_touch_hz_box,
		   phead->lenth + 1, 0 );
		m_touch_hz_flag = TURE;
	}
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 获得当前键值
 *
 * @输入:buf 存放读出数据缓冲
 *               size:需要读出的大小
 *
 * @输出:实际读出大小
********************************************************************/
Bsize_t touch_driver_read( uint8_t*buf, Bsize_t size , void*pos )
{
	Bsize_t real_size = 0;
	touch_driver_read_touch_data();
	if ( m_touch_refresh == 1 )
	{
		global_memcpy( buf,&m_touch_data,sizeof( touch_data_t ) );
		m_touch_refresh = 0;
		real_size = sizeof( touch_data_t );
	}
	return real_size;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 按键驱动初始化
 * @输入: NONE
 * @输出: NONE
********************************************************************/
BOOL touch_driver_is_start = FAIL;


void touch_rest_gpio_init(void)
{
	EALLOW;
    GpioCtrlRegs.GPBMUX2.bit.GPIO59= 0; 
	GpioCtrlRegs.GPBDIR.bit.GPIO59= 1;
	EDIS;
	return;
}
dev_id_t serialC_driver_register( void );

void touch_driver_init( void )
{
	uint32_t baud = 9600;
	m_time_tick = OSTimeGet();
	touch_rest_gpio_init();
	m_serialC_dev_id = serialC_driver_register();
	os_driver_init( m_serialC_dev_id );
	os_driver_ioctl( m_serialC_dev_id, SET_SERIAL_BAUD, &baud );	//波特率设置;
    if( !touch_driver_is_start )
    {
		touch_driver_is_start = TURE;
		touch_driver_open();
	    msleep(3);
		WritePanlXY(0);
		SetPenUpTime(5);		//设置1S
    }
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 按键驱动卸载
 * @输入:	NONE
 * @输出:	NONE
********************************************************************/
void touch_driver_uninstall( void )
{
	os_driver_uninstall( m_serialC_dev_id );
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 按键驱动注册
 * @输入:none
 * @输出:NONE
********************************************************************/
dev_id_t touch_driver_register( void )
{
	uint8_t ret;
	os_dirver_operate_t temp;
	temp.os_driver_init   =  touch_driver_init;
	temp.os_driver_ioctl  = touch_driver_ioctl;
	temp.os_driver_read  = touch_driver_read;
	temp.os_driver_write = touch_driver_write;
	temp.os_driver_uninstall = touch_driver_uninstall;
	ret = os_driver_add( &temp, "Touch  Driver" );
	return ret;
}

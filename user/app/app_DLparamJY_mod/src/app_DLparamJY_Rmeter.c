/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:电参数校验读表程序
* 创建时间:2013年7月9日
* 所属公司:科荟
* 文件名称:app_DLparamJYmod_Rmeter.c
* 作者    :周炳权
* 功能描述:实现电参数校验功能
******************************************************************************************/
#include "app_DLparamJY_inside.h"

uint8_t   	_task_cmt_id = INVALID_PRO;

uint8_t	  	g_DLparam_data_finish 		= 0;	//读取数据完成标志
static    	dev_id_t  s_usart3_dev_id 	= INVALID_DEV_ID;
uint8_t   	s_send_offset		= 0;  	//发送第几个命令
uint8_t   	s_offset_baud		= 0;
uint8_t   	s_baud_delay		= 0;
uint8_t		s_offset_crc		= 0;
uint8_t		s_self_adapt_end	= 0;	//自适应完成标志
uint8_t		s_self_adapt_num	= 0;	//自适应次数
uint8_t		s_self_adapt_succeed= 0;	//自适应成功标志
uint8_t		s_read_cnt			= 0;	//读多次

#define   MAX_97_CMD_SIZE		12
#define   MAX_07_CMD_SIZE		4
									 //有功 //电压A,B,C    //电流A,B,C	  //瞬时有功A,B,C //瞬时无功A,B,C //功率因数总,A,B,C
static const uint8_t s_cmd_97_DI1[] = {0x90,0xB6,0xB6,0xB6,0xB6,0xB6,0xB6,0xB6,0xB6,0xB6, 0xB6,0xB6,0xB6}; //0xB6,0xB6,0xB6,0xB6};
static const uint8_t s_cmd_97_DI0[] = {0x1F,0x11,0x12,0x13,0x21,0x22,0x23,0x31,0x32,0x33, 0x41,0x42,0x43}; //0x50,0x51,0x52,0x53};
									//有功 //电压 //电流 //P A,B,C  //Q A,B,C 	//功率因数A,B,C
static const uint8_t s_cmd_07_DI3[] = {0x00,0x02, 0x02,  0x02, 	    0x02,		0x02};
static const uint8_t s_cmd_07_DI2[] = {0x00,0x01, 0X02,	 0x03, 		0x04,		0x06};
static const uint8_t s_cmd_07_DI1[] = {0xFF,0xFF, 0xFF,  0xFF, 		0xFF,		0xFF};
static const uint8_t s_cmd_07_DI0[] = {0x00,0x00, 0x00,  0x00, 		0x00,		0x00};

/********************************************************************
 * @作者 :周炳权
 * @功能 :发送数据
 * @输入 :*data数据  len长度
 * @输出 :NONE
********************************************************************/
static void Usart3_send(void *data,Bsize_t len)
{
	os_driver_write( s_usart3_dev_id, data,len, 0 );
	return;
}
/********************************************************************
 * @作者 :周炳权
 * @功能 :波特率选择
 * @输入 :m_offset:波特率选择偏移
 * @输出 :NONE
********************************************************************/
static void baud_select(uint8_t m_offset)
{
	uint32_t m_baud;	
	switch(m_offset)
	{
		case 1:
			m_baud=1200;
			s_baud_delay = 3;
			break;
		case 2:
			m_baud=2400;
			s_baud_delay = 4;
			break;
		case 3:
			m_baud=4800;
			s_baud_delay = 5;
			break;
		case 4:
			m_baud=9600;
			s_baud_delay = 6;
			break;
		case 5:
			m_baud=600;
			s_baud_delay = 2;
			break;	
		case 6:
			m_baud=300;
			s_baud_delay = 1;
			break;	
		case 7:
			m_baud=19200;
			s_baud_delay = 7;
			break;	
		default:
			m_baud=1200;
			s_baud_delay = 3;
			break;
	}	
	os_driver_ioctl(s_usart3_dev_id, SET_SERIAL_BAUD, &m_baud);
	return;
}
/********************************************************************
 * @作者 :周炳权
 * @功能 :校验方式选择
 * @输入 :m_offset:校验选择偏移;
 * @输出 :NONE
********************************************************************/
static void CRC_select( uint8_t m_offset )
{
	uint32_t m_CRC;	
	switch(m_offset)
	{
		case 1:
			m_CRC=2;  //偶校验
			break;
		case 2:
			m_CRC=0;  //无校验
			break;
		case 3:		
			m_CRC=1;  //奇校验
			break;
		default:
			m_CRC=0;  //无校验
			break;
	}
	os_driver_ioctl(s_usart3_dev_id, SET_SERIAL_CHECK, &m_CRC);
	return;
}

/********************************************************************
* @作者 :周炳权
* @功能 :发送读97协议数据命令
* @输入 :m_offset:命令选择偏移
* @输出 :NONE
********************************************************************/
static void send_read_meter_cmd_97(uint8_t m_offset)
{
	send_data_97_t send_cmd_97={0,};
	GLOBAL_MEMCLR(&send_cmd_97, sizeof(send_data_97_t));
	
	send_cmd_97.head[0]	= 0xFE;
	send_cmd_97.head[1]	= 0xFE;
	send_cmd_97.head[2]	= 0xFE;
	send_cmd_97.head1	= 0x68;	
	send_cmd_97.addr[0] = 0x99;
	send_cmd_97.addr[1] = 0x99;
	send_cmd_97.addr[2] = 0x99;
	send_cmd_97.addr[3] = 0x99;
	send_cmd_97.addr[4] = 0x99;
	send_cmd_97.addr[5] = 0x99;
	send_cmd_97.head2	= 0x68;
	send_cmd_97.cmd		= 0x01;
	send_cmd_97.len		= 0x02;	
	send_cmd_97.biaoshi_L=s_cmd_97_DI0[m_offset]+0x33;
	send_cmd_97.biaoshi_H=s_cmd_97_DI1[m_offset]+0x33;
	send_cmd_97.crc		= (uint8_t)global_sum_array((char*)&send_cmd_97.head1,sizeof(send_data_97_t)-5,IS_CHAR);
	send_cmd_97.end		= 0x16;
	Usart3_send((char*)&send_cmd_97,sizeof(send_data_97_t));
	
	return;
}
/********************************************************************
* @作者 :周炳权
* @功能 :发送读07协议数据命令
* @输入 :m_offset:命令选择偏移
* @输出 :NONE
********************************************************************/
static void send_read_meter_cmd_07(uint8_t offset)
{
	send_data_07_t send_cmd_07={0,};
	GLOBAL_MEMCLR(&send_cmd_07, sizeof(send_data_07_t));
	send_cmd_07.head[0]	= 0xFE;
	send_cmd_07.head[1]	= 0xFE;
	send_cmd_07.head[2]	= 0xFE;
	send_cmd_07.head1	= 0x68;	
	send_cmd_07.addr[0] = 0xAA;
	send_cmd_07.addr[1] = 0xAA;
	send_cmd_07.addr[2] = 0xAA;
	send_cmd_07.addr[3] = 0xAA;
	send_cmd_07.addr[4] = 0xAA;
	send_cmd_07.addr[5] = 0xAA;
	send_cmd_07.head2	= 0x68;
	send_cmd_07.cmd		= 0x11;
	send_cmd_07.len		= 0x04;	
	send_cmd_07.biaoshi[0]=s_cmd_07_DI0[offset]+0x33;
	send_cmd_07.biaoshi[1]=s_cmd_07_DI1[offset]+0x33;
	send_cmd_07.biaoshi[2]=s_cmd_07_DI2[offset]+0x33;
	send_cmd_07.biaoshi[3]=s_cmd_07_DI3[offset]+0x33;
	send_cmd_07.crc		= (uint8_t)global_sum_array((char*)&send_cmd_07.head1,sizeof(send_data_07_t)-5,IS_CHAR);
	send_cmd_07.end		= 0x16;
	Usart3_send((char*)&send_cmd_07,sizeof(send_data_07_t));
	
	return;
}
/********************************************************************
 * @作者 :周炳权
 * @功能 :发送读表指令
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
static void send_read_meter_cmd_select(void)
{
	if( g_DLparamJY_07_or_97 == 0 )
	{
		send_read_meter_cmd_97(s_send_offset);
	}
	else if( g_DLparamJY_07_or_97 ==1 )
	{
		send_read_meter_cmd_07(s_send_offset);
	}
	return;
}
/********************************************************************
* @作者 :周炳权
* @功能 :97规约数据分析
* @输入 :*data:数据缓存指针
* @输出 :NONE
********************************************************************/
static void data_handle_guiyue_97(uint8_t*data)
{
	uint8_t offset1=0,offset2=0;
	uint8_t biaoshi1=0;
	uint8_t biaoshi2=0;
	float   f_power[6]={0,}; 
	uint8_t s_data=0;		  //减去了0x33的数据
	float   f_data=0;  
	float 	fbs;
	biaoshi1=(data[11]-0x33) & 0xff;
	biaoshi2=(data[10]-0x33) & 0xff;	
	if ((( biaoshi2 & 0x0F ) == 0x0F) && (biaoshi1 == 0x90 || biaoshi1 == 0x91)) //数据块
	{
		for ( offset1 = 0;offset1 < 5;offset1++ )
		{
			fbs = 1.0;
			f_data = 0;
			for (offset2 = 0;offset2 < 4;offset2++ )
			{
				s_data = data[12+offset1*4+offset2] - 0x33;
				if ( (s_data & 0xff) == 0xff )	s_data = 0;
				f_data += fbs * (((s_data>> 4 ) & 0x0F ) * 10 + ( s_data & 0x0F ) );
				fbs *= 100.0;
			}
			f_power[offset1] = f_data * 0.01;
		}
	}
	if (( biaoshi1 == 0x90 ) && ( biaoshi2 == 0x1F ) )//正向有功电能数据块
	{
		f_power[0] = f_power[1] + f_power[2] + f_power[3] + f_power[4];
	}
	else if (( biaoshi1 == 0x91 ) && ( biaoshi2 == 0x1F ) )//正向无功电能数据块
	{
		f_power[0] = f_power[1] + f_power[2] + f_power[3] + f_power[4];
	}
	else if (( biaoshi1 == 0x90 ) && ( biaoshi2 == 0x2F ) )//反向有功电能数据块
	{
		f_power[0] = f_power[1] + f_power[2] + f_power[3] + f_power[4];
	}
	else if (( biaoshi1 == 0x91 ) && ( biaoshi2 == 0x2F ) )//反向无功电能数据块
	{
		f_power[0] = f_power[1] + f_power[2] + f_power[3] + f_power[4];
	}
/****************************电压电流*************************************/
	if((biaoshi1 == 0xB6) && ((biaoshi2 & 0xf0) == 0x20 || (biaoshi2 & 0xf0)== 0x10 ))
	{
		fbs= 1.0;
		f_data = 0;
		if((biaoshi2 & 0xf0)== 0x10)//电压
		{
			for ( offset1= 0;offset1 <2;offset1++ )
			{
				s_data = data[12+offset1] - 0x33;
				if ( (s_data & 0xff) == 0xff )	s_data = 0;
				f_data += fbs * (((s_data>> 4 ) & 0x0F ) * 10 + ( s_data & 0x0F ) );
				fbs *= 100.0;
			}
			if ( f_data > 1000 )	f_data = 0;
			if(biaoshi2 == 0x11)
				g_DLparam_buf.meter_U[0] = f_data;
			else if(biaoshi2 == 0x12)
				g_DLparam_buf.meter_U[1] = f_data;
			else if(biaoshi2 == 0x13)
			{
				g_DLparam_buf.meter_U[2] = f_data;
				app_DLparamJYmod_err1_calc();
			   	app_DLparamJYmod_meter1_param_err_display(&g_DLparam_buf);
			}
		}
		else if((biaoshi2 & 0xf0)== 0x20)//电流
		{
			for ( offset1= 0;offset1 <2;offset1++ )
			{
				s_data = data[12+offset1] - 0x33;
				if ( (s_data & 0xff) == 0xff )	s_data = 0;
				f_data += fbs * (((s_data>> 4 ) & 0x0F ) * 10 + ( s_data & 0x0F ) );
				fbs *= 100.0;
			}
			f_data = f_data * 0.01;
			if ( f_data > 1000 )	f_data = 0;
			if(biaoshi2 == 0x21)
				g_DLparam_buf.meter_I[0] = f_data;
			else if(biaoshi2 == 0x22)
				g_DLparam_buf.meter_I[1] = f_data;
			else if(biaoshi2 == 0x23)
			{
				g_DLparam_buf.meter_I[2] = f_data;
				app_DLparamJYmod_err2_calc();
		   		app_DLparamJYmod_meter2_param_err_display(&g_DLparam_buf);
			}
		}
	}
	
	//功率因数
	if(biaoshi1 == 0xB6 && ((biaoshi2 & 0x50) ==0x50))
	{
		fbs= 1.0;
		f_data = 0;
		for ( offset1 = 0;offset1 < 2;offset1++ )
		{
			s_data = data[12+offset1] - 0x33;
			if ( (s_data & 0xff) == 0xff )	s_data = 0;
			f_data += fbs * (((s_data>> 4 ) & 0x0F ) * 10 + ( s_data & 0x0F ) );
			fbs *= 100;
		}
		f_data = f_data*0.001;	
		if ( biaoshi2 == 0x50 )
			g_DLparam_buf.meter_COS[0] = f_data;//总功率因数
		else if ( biaoshi2 == 0x51 )
			g_DLparam_buf.meter_COS[0] = f_data;//A相功率因数
		else if ( biaoshi2 == 0x52 )
			g_DLparam_buf.meter_COS[1] = f_data;//B相功率因数
		else if ( biaoshi2 == 0x53 )
			g_DLparam_buf.meter_COS[2] = f_data;//C相功率因数
	}
	else if(biaoshi1 == 0xB6 && ((biaoshi2 & 0x30) ==0x30) )
	{
		fbs= 1.0;
		f_data = 0;
		for ( offset1 = 0;offset1 <3;offset1++ )
		{
			s_data = data[12+offset1] - 0x33;
			if ( (s_data & 0xff) == 0xff )	s_data = 0;
			f_data += fbs * (((s_data>> 4 ) & 0x0F ) * 10 + ( s_data & 0x0F ) );
			fbs *= 100;
		}
		f_data = f_data*0.0001;
		if ( biaoshi2 == 0x31 )
			g_DLparam_buf.meter_P[0] = (f_data * 1000.0);//瞬时A相有功功率//kW->W
		else if ( biaoshi2 == 0x32 )
			g_DLparam_buf.meter_P[1] = (f_data * 1000.0);//瞬时B相有功功率
		else if ( biaoshi2 == 0x33 )
		{
			g_DLparam_buf.meter_P[2] = (f_data * 1000.0);//瞬时C相有功功率
			app_DLparamJYmod_err3_calc();
		   	app_DLparamJYmod_meter3_param_err_display(&g_DLparam_buf);
		}
	}
	else if(biaoshi1 == 0xB6 && ((biaoshi2 & 0x40) ==0x40) )
	{
		fbs= 1.0;
		f_data = 0;
		for ( offset1 = 0;offset1 < 2;offset1++ )
		{
			s_data = data[12+offset1] - 0x33;
			if ( (s_data & 0xff) == 0xff )	s_data = 0;
			f_data += fbs * (((s_data>> 4 ) & 0x0F ) * 10 + ( s_data & 0x0F ) );
			fbs *= 100;
		}
		f_data = f_data*0.01;
		if ( biaoshi2 == 0x41 )
			g_DLparam_buf.meter_Q[0] = (f_data * 1000.0);//瞬时A相无功功率//kvar->var
		else if ( biaoshi2 == 0x42 )
			g_DLparam_buf.meter_Q[1] = (f_data * 1000.0);//瞬时B相无功功率
		else if ( biaoshi2 == 0x43 )
		{
			g_DLparam_buf.meter_Q[2] = (f_data * 1000.0);//瞬时C相无功功率
			app_DLparamJYmod_err4_calc();
		   	app_DLparamJYmod_meter4_param_err_display(&g_DLparam_buf);
		}
	}

	return;
}
/********************************************************************
* @作者 :周炳权
* @功能 :07规约数据分析
* @输入 :*data:数据缓存指针
* @输出 :NONE
********************************************************************/
static void data_handle_guiyue_07(uint8_t *data)
{

	uint8_t offset1=0,offset2=0;
	uint8_t biaoshi1=0,biaoshi2=0,biaoshi3=0,biaoshi4=0;
	float   f_power[6]={0,}; 
	uint8_t s_data=0;		  //减去了0x33的数据
	float   f_data=0;  
	float 	fbs;
	biaoshi1=(data[13]-0x33) & 0xff;
	biaoshi2=(data[12]-0x33) & 0xff;
	biaoshi3=(data[11]-0x33) & 0xff;	
	biaoshi4=(data[10]-0x33) & 0xff;
	if((biaoshi1==0x00 && biaoshi3==0xff && biaoshi4==0x00) &&
		(biaoshi2==0x00||biaoshi2==0x01||biaoshi2==0x02||biaoshi2==0x03||biaoshi2==0x04))//数据块
	{
		for ( offset1 = 0;offset1 < 5;offset1++ )
		{
			fbs = 1.0;
			f_data = 0;
			for (offset2 = 0;offset2 < 4;offset2++ )
			{
				s_data = data[14+offset1*4+offset2] - 0x33;
				if ( (s_data & 0xff) == 0xff )	s_data = 0;
				f_data += fbs * (((s_data>> 4 ) & 0x0F ) * 10 + ( s_data & 0x0F ) );
				fbs *= 100.0;
			}
			f_power[offset1] = f_data * 0.01;
		}
		if ( (biaoshi2 == 0x01||biaoshi2 == 0x00) && biaoshi1 == 0x00 )//正向有功电能数据块
		{
			f_power[0] = f_power[1] + f_power[2] + f_power[3] + f_power[4];
		}
		else if ( biaoshi2 == 0x03 && biaoshi1 == 0x00)//正向无功电能数据块
		{
			f_power[0] = f_power[1] + f_power[2] + f_power[3] + f_power[4];
		}
		else if ( biaoshi2 == 0x02 && biaoshi1 == 0x00)//反向有功电能数据块
		{
			f_power[0] = f_power[1] + f_power[2] + f_power[3] + f_power[4];
		}
		else if ( biaoshi2 == 0x04 && biaoshi1 == 0x00)//反向无功电能数据块
		{
			f_power[0] = f_power[1] + f_power[2] + f_power[3] + f_power[4];
		}
	}
/****************************电压电流*************************************/
	if((biaoshi1 == 0x02 && biaoshi4==0x00 && biaoshi3 ==0xff)
		&&(biaoshi2 == 0x01 || biaoshi2 ==0x02))
	{
		fbs= 1.0;
		f_data = 0;
		if( biaoshi2 == 0x01 && biaoshi1 == 0x02)//电压
		{
			for ( offset1 = 0;offset1 < 3;offset1++ )
			{
				fbs = 1.0;
				f_data = 0;
				for (offset2 = 0;offset2 < 2;offset2++ )
				{
					s_data = data[14+offset1*2+offset2] - 0x33;
					if ( (s_data & 0xff) == 0xff )	s_data = 0;
					if ( (offset2 == 1) && (s_data & 0x80) == 0x80 )
						s_data &= 0x7f;
					f_data += fbs * (((s_data>> 4 ) & 0x0F ) * 10 + ( s_data & 0x0F ) );
					fbs *= 100.0;
				}
				f_power[offset1] = f_data * 0.1;
				if (((data[14+offset1*2+1] - 0x33) & 0x80) == 0x80 )
					f_power[offset1] = -f_power[offset1];
			}
			if ( f_power[0] > 1000 )	f_power[0] = 0;
			if ( f_power[1] > 1000 )	f_power[1] = 0;
			if ( f_power[2] > 1000 )	f_power[2] = 0;
			g_DLparam_buf.meter_U[0] = f_power[0];
			g_DLparam_buf.meter_U[1] = f_power[1];
			g_DLparam_buf.meter_U[2] = f_power[2];
			app_DLparamJYmod_err1_calc();
			app_DLparamJYmod_meter1_param_err_display(&g_DLparam_buf);
		}
		else if(biaoshi2 == 0x02 && biaoshi1 == 0x02)//电流
		{
			for ( offset1 = 0;offset1 < 3;offset1++ )
			{
				fbs = 1.0;
				f_data = 0;
				for (offset2 = 0;offset2 < 3;offset2++ )
				{
					s_data = data[14+offset1*3+offset2] - 0x33;
					if ( (s_data & 0xff) == 0xff )	s_data = 0;
					if ( (offset2 == 2) && (s_data & 0x80) == 0x80 )
						s_data &= 0x7f;
					f_data += fbs * (((s_data>> 4 ) & 0x0F ) * 10 + ( s_data & 0x0F ) );
					fbs *= 100.0;
				}
				f_power[offset1] = f_data * 0.001;
				if (((data[14+offset1*3+2] - 0x33) & 0x80) == 0x80 )
					f_power[offset1] = -f_power[offset1];
			}
			if ( f_power[0] > 1000 )	 f_power[0] = 0;
			if ( f_power[1] > 1000 )	 f_power[1] = 0;
			if ( f_power[2] > 1000 )	 f_power[2] = 0;
			g_DLparam_buf.meter_I[0] = f_power[0];
			g_DLparam_buf.meter_I[1] = f_power[1];
			g_DLparam_buf.meter_I[2] = f_power[2];
			app_DLparamJYmod_err2_calc();
			app_DLparamJYmod_meter2_param_err_display(&g_DLparam_buf);
		}
	}
	
	//功率因数
	if(biaoshi1 == 0x02 && biaoshi2 ==0x06 && biaoshi3 ==0xFF && biaoshi4 ==0x00)
	{
		for ( offset1 = 0;offset1 < 4;offset1++ )
		{
			fbs= 1.0;
			f_data = 0;
			for ( offset2 = 0;offset2 < 2;offset2++ )
			{
				s_data = data[14+offset1*2+offset2] - 0x33;
				if ( (s_data & 0xff) == 0xff )	s_data = 0;
				f_data += fbs * (((s_data>> 4 ) & 0x0F ) * 10 + ( s_data & 0x0F ) );
				fbs *= 100;
			}
			f_power[offset1] = f_data*0.001;

		}
		g_DLparam_buf.meter_COS[0] = f_power[0];//总功率因数
		//g_DLparam_buf.meter_COS[0] = f_power[1];//A
		//g_DLparam_buf.meter_COS[1] = f_power[2];//B
		//g_DLparam_buf.meter_COS[2] = f_power[3];//C
	}
	//瞬时有功功率
	else if(biaoshi1 == 0x02 && biaoshi2 ==0x03 &&biaoshi3 == 0xFF && biaoshi4 ==0x00 )
	{
		for ( offset1 = 0;offset1 < 4;offset1++ )
		{
			fbs= 1.0;
			f_data = 0;
			for ( offset2 = 0;offset2 <3;offset2++ )
			{
				s_data = data[14+offset1*3+offset2] - 0x33;
				if ( (s_data & 0xff) == 0xff )	s_data = 0;
				if ( (offset2 == 2) && (s_data & 0x80) == 0x80 )
					s_data &= 0x7f;
				f_data += fbs * (((s_data>> 4 ) & 0x0F ) * 10 + ( s_data & 0x0F ) );
				fbs *= 100;
			}
			f_power[offset1] = f_data*0.0001;
			if (((data[14+offset1*3+2] - 0x33) & 0x80) == 0x80 )
				f_power[offset1] = -f_power[offset1];
		}
		g_DLparam_buf.meter_P[0] = (f_power[1] * 1000.0);//kW->W
		g_DLparam_buf.meter_P[1] = (f_power[2] * 1000.0);
		g_DLparam_buf.meter_P[2] = (f_power[3] * 1000.0);
		app_DLparamJYmod_err3_calc();
		app_DLparamJYmod_meter3_param_err_display(&g_DLparam_buf);
	}
	//瞬时无功功率
	else if(biaoshi1 == 0x02 && biaoshi2 ==0x04 &&biaoshi3 == 0xFF && biaoshi4 ==0x00 )
	{
		for ( offset1 = 0;offset1 < 4;offset1++ )
		{
			fbs= 1.0;
			f_data = 0;
			for ( offset2 = 0;offset2 <3;offset2++ )
			{
				s_data = data[14+offset1*3+offset2] - 0x33;
				if ( (s_data & 0xff) == 0xff )	s_data = 0;
				if ( (offset2 == 2) && (s_data & 0x80) == 0x80 )
					s_data &= 0x7f;
				f_data += fbs * (((s_data>> 4 ) & 0x0F ) * 10 + ( s_data & 0x0F ) );
				fbs *= 100;
			}
			f_power[offset1] = f_data*0.0001;
			if (((data[14+offset1*3+2] - 0x33) & 0x80) == 0x80 )
				f_power[offset1] = -f_power[offset1];
		}
		g_DLparam_buf.meter_Q[0] = (f_power[1] * 1000.0);//kvar->var
		g_DLparam_buf.meter_Q[1] = (f_power[2] * 1000.0);
		g_DLparam_buf.meter_Q[2] = (f_power[3] * 1000.0);
		app_DLparamJYmod_err4_calc();
		app_DLparamJYmod_meter4_param_err_display(&g_DLparam_buf);
	}
	return;
}
/********************************************************************
* @作者 :周炳权
* @功能 :自适应读表数据发送
* @输入 :NONE
* @输出 :NONE
********************************************************************/
static void auto_read_data_send(void)
{
	s_offset_crc++;	
	if ( s_offset_crc > 3 )
	{
		s_offset_crc = 1;
		s_offset_baud++;
		if ( s_offset_baud > 7 )
		{	
			s_offset_baud = 0;
			s_self_adapt_num++;
			if ( s_self_adapt_num > 2 )
			{
				s_self_adapt_num = 0;
				s_self_adapt_end = 1;
			}
		}
	}
	
	baud_select(s_offset_baud);	
	CRC_select(s_offset_crc);
	msleep(1);
	send_read_meter_cmd_select();
	return;
}

/********************************************************************
 * @创建人 :周炳权
 * @功能 :模块确定通讯任务
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
static void ensure_communication( void*parg )
{
	Bsize_t size;
	Bsize_t Fsize = 5;
	const char *tip_buf[2] 		= {"无法与电能表建立通讯,请检查线路和供电是否正常!",""};
	const char *tip2_buf[2] 	= {"正在与电能表建立通讯并读取测量参数.",""};
	uint8_t rev_data[100];
	s_offset_crc 			= 0;
	s_offset_baud 			= 1;
	s_baud_delay			= 3;
	s_self_adapt_succeed 	= 0;
	s_send_offset			= 0;
	s_self_adapt_end 		= 0;
	s_self_adapt_num		= 0;
	//baud_select(s_offset_baud+1);	
	//CRC_select(1);
	GLOBAL_MEMCLR(rev_data, 100);
	GUI_3D_window_title_mod(m_DLparamJYmod_win,(char *)tip2_buf[g_language]);
	while (1)
	{
		msleep(TYPE_DELAY);
		if ( os_task_delete_req( SELF_PRO ) == OS_TASK_DEL_REQ )
		{
			_task_cmt_id = INVALID_PRO;
			os_task_delete( SELF_PRO );   			
		}
		if ( s_self_adapt_succeed == 0 )	auto_read_data_send();
		else if ( s_self_adapt_succeed == 1 )
		{
			s_send_offset++;
			send_read_meter_cmd_select();
		}
		msleep(10);
		if ( s_self_adapt_end == 1 )
		{
			s_self_adapt_end = 0;
			GUI_3D_window_title_mod(m_DLparamJYmod_win,(char *)tip_buf[g_language]);
			_task_cmt_id = INVALID_PRO;
			os_task_delete( SELF_PRO ); 
		}

		while ( Fsize )
		{
			size = os_driver_read( s_usart3_dev_id, &rev_data[0], 1, 0 );
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
			continue;
		}
		
		if ( s_baud_delay == 1 )	  	msleep(90);	//300
		else if ( s_baud_delay == 2 )	msleep(45);	//600
		else if ( s_baud_delay == 3 )	msleep(25);	//1200
		else if ( s_baud_delay == 4 )	msleep(15);	//2400
		else if ( s_baud_delay == 5 )	msleep(10);	//4800
		else if ( s_baud_delay == 6 )	msleep(5);	//9600
		else if ( s_baud_delay == 7 )	msleep(3);	//19200
		size = os_driver_read( s_usart3_dev_id, &rev_data[1], 7, 0 );
		if ( size != 7 || ( rev_data[7] != 0x68 ) )  //收第二个68H；
		{
			continue;
		}
		size = os_driver_read( s_usart3_dev_id, &rev_data[8], 2, 0 );
		if ( size != 2 )	  						//收控制码和长度,8为控制码,9长度;
		{
			continue;
		}
		size = os_driver_read( s_usart3_dev_id, &rev_data[10], rev_data[9], 0 );		
		size = os_driver_read( s_usart3_dev_id, &rev_data[10+rev_data[9]], 2, 0 );		
		if( (size == 2) && (rev_data[10+rev_data[9]+1]==0x16) )
		{
			if( g_DLparamJY_07_or_97 == 0 ) //97
			{
				data_handle_guiyue_97(rev_data);
			}
			else if( g_DLparamJY_07_or_97 == 1 )//07
			{
				data_handle_guiyue_07(rev_data);
			}
			s_self_adapt_succeed = 1;
			s_self_adapt_end = 0;
			if ( g_DLparamJY_07_or_97 == 0 )
			{
				if ( s_send_offset >= MAX_97_CMD_SIZE )
				{
					s_read_cnt++;
					if ( s_read_cnt >= 1 )
					{
						s_send_offset = 0;
						s_read_cnt = 0;
						g_DLparam_data_finish = 1;
						_task_cmt_id = INVALID_PRO;
						os_task_delete( SELF_PRO );
					}
					else s_send_offset = 0;
				}
			}
			else if( g_DLparamJY_07_or_97 == 1 )//07
			{
				if ( s_send_offset >= MAX_07_CMD_SIZE )
				{
					s_read_cnt++;
					if ( s_read_cnt >= 1 )
					{
						s_send_offset = 0;
						s_read_cnt = 0;
						g_DLparam_data_finish = 1;
						_task_cmt_id = INVALID_PRO;
						os_task_delete( SELF_PRO );
					}
					else s_send_offset = 0;
				}
			}
		}
	}
}

/********************************************************************
* @作者 :周炳权
* @功能 :读电力参数计算及显示任务
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void ReadDL_param_handle( void )
{
	if(_task_cmt_id == INVALID_PRO)
	_task_cmt_id = 
		os_task_assign_stk_build( ensure_communication,6);
}
/********************************************************************
 * @创建人 :周炳权
 * @功能 :读表初始化
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_DLparamJY_Rmeter_init( void )
{
	s_usart3_dev_id = os_driver_open( "D232" );
	return;
}

/********************************************************************
 * @创建人 :周炳权
 * @功能 :读表驱动卸载
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_DLparamJY_Rmeter_uninstall( void )
{
	os_driver_close(s_usart3_dev_id);
	return;
}



/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:����ģ�鹦�ܺ���
* ����ʱ��:2012-07-10
* ������˾:����
* �ļ�����:app_zzmod_fun.c
* ������ :�ܱ�Ȩ
* ��������:ʵ������ģ�鹦��
* �޸�ʱ��:2013-01-15
* �޸���  :�ܱ�Ȩ
* �޸�����:��ԭ���Ļ�еʽ������ֹ��ܸĳɵ��ӵ��ͻ�е�����ݵ����ֹ���ģ��,
* ��HAD�����ϻ����������⸺�ع��ܣ�
**********************************************************************************************/
#include "app_ZZmod_inside.h"

static    uint8_t   _task_id 	 	= INVALID_PRO;
static    uint8_t   _task_cmt_id 	= INVALID_PRO;

zzmod_arg	m_zz_arg = { 0, };

uint32_t 		s_zz_time_start  	= 0;	//��ʱ��ʼ����,1��Ӧ10ms
uint32_t 		s_zz_time_preset 	= 0;	//Ԥ�ü�����С,1��Ӧ10ms
static uint8_t 	s_zz_time_flag 		= 0;	//���ֶ�ʱ��־
uint8_t			g_zz_ITorMC_meter 	= 0;
uint8_t 		g_zz_07_or_97 		= 1;
uint8_t			g_zz_F5_count    	= 0; 	//F5������������,0:���ֿ�ʼ,1:���ֽ���

static 	const	float 	const_energy 	= 72000.0;		//���ܳ���
uint32_t	energy_count_start 		= 0;
uint32_t	energy_count_end 		= 0;
float		energy_count_real 		= 0.00000000;

#define   		ZOUZI_COUNTER_ADDR	0x42b8
#define         ZZ_Pulse_Get(param)		{param = READ32BIT_DATA(ZOUZI_COUNTER_ADDR);}

static    	dev_id_t  s_usart3_dev_id 	= INVALID_DEV_ID;
uint8_t   	zz_send_offset		= 0;  	//���͵ڼ�������
uint8_t   	zz_offset_baud		= 0;
uint8_t   	zz_baud_delay		= 0;
uint8_t		zz_offset_crc		= 0;
uint8_t		zz_self_adapt_end	= 0;	//����Ӧ��ɱ�־
uint8_t		zz_self_adapt_num	= 0;	//����Ӧ����
uint8_t		s_self_adapt_first	= 0;	//����Ӧ�״����б�־
uint8_t		s_zz_run_flag		= 0;	//�������б�־,0:��ʼ;1:����;
uint8_t		s_zz_running_flag	= 0;	//�����������б�־,0:û������;1:��������;
uint8_t   	b_offset_baud		= 0;
uint8_t		b_offset_crc		= 0;
									 //�й� //�޹�
static const uint8_t s_cmd_97_DI1[] = {0x90,0x91};
static const uint8_t s_cmd_97_DI0[] = {0x10,0x10};
								//����й� //����޹�1
static const uint8_t s_cmd_07_DI3[] = {0x00,0x00};
static const uint8_t s_cmd_07_DI2[] = {0x00,0x03};
static const uint8_t s_cmd_07_DI1[] = {0x00,0x00};
static const uint8_t s_cmd_07_DI0[] = {0x00,0x00};

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :ģ�����ݱ�־��ʼ��
 * @���� :NONE
 * @��� :NONE
********************************************************************/
static void mod_flags_init( void )
{
	s_zz_run_flag		= 0;
	s_self_adapt_first	= 0;
	zz_self_adapt_num	= 0;
	zz_self_adapt_end	= 0;
	zz_offset_crc		= 0;
	zz_offset_baud		= 0;
	zz_send_offset		= 0;
	s_zz_running_flag   = 0;
	g_zz_F5_count		= 0;
	GLOBAL_MEMCLR(&m_zz_arg, sizeof(zzmod_arg));
	global_memcpy( m_zz_arg.energy_measure, "0.000", 5 );
	global_memcpy( m_zz_arg.energy_real, "0.000", 5 );
	return;
}
/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :�ж��Ƿ�ﵽԤ�����ֵ���,�����������
 * @���� :NONE
 * @��� :NONE
********************************************************************/
static void zz_preset_reach_end( void )
{
	float	m_fenergy = 0;
	float	m_penergy = 0;
	
	m_fenergy = global_atof(m_zz_arg.energy_measure);
	m_penergy = global_atof(m_zz_arg.energy_preset);
	if ( (m_penergy > 0.001) && (m_fenergy >= m_penergy) && (s_zz_running_flag == 1) )
	{
		buz_response(20);
		app_global_key_dummy_press(GUI_KEY_ZZ_END);
	}

	return;
}
/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :�ж��Ƿ�ﵽ���ֶ�ʱʱ��,�����������
 * @���� :NONE
 * @��� :NONE
********************************************************************/
static void zz_time_reach_end( void )
{
	uint32_t m_ms_remain = 0;
	long	m_Rmin = 0;
	if ( (s_zz_time_flag == 1) && (s_zz_running_flag == 1) )
	{
		m_ms_remain = OSTimeGet() - s_zz_time_start;
		if( m_ms_remain >= s_zz_time_preset )
		{
			m_Rmin = 0;
			app_zzmod_time_display(m_Rmin);
			buz_response(20);
			app_global_key_dummy_press(GUI_KEY_ZZ_END);
		}
		else
		{
			m_ms_remain = s_zz_time_preset - m_ms_remain;
			m_Rmin = (long)(m_ms_remain / 6000);
			m_Rmin += 1;
			app_zzmod_time_display(m_Rmin);
		}
	}

	return;
}
/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :���ֶ�ʱʱ�俪ʼ����
 * @���� :NONE
 * @��� :NONE
********************************************************************/
static void zz_time_start_handle( void )
{
	long m_min = global_atoi(m_zz_arg.zz_time_min);
	if ( s_zz_time_flag == 1 )
	{
		s_zz_time_start = OSTimeGet();
		s_zz_time_preset = (m_min * 6000);//����ת��10ms
	}

	return;
}
/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :������ֽ������㴦����
 * @���� :NONE
 * @��� :NONE
********************************************************************/
static void zz_end_for_calc_handle( void )
{
	float	x_start = 0, y_rate = 1.0, z_end = 0, zzmod_temp = 0;
	const char *m_title_buf[2] = {"���ֲ�������",""};
	app_zzmod_measure_energy_calc();
	x_start = global_atof( m_zz_arg.energy_start );
	z_end = global_atof( m_zz_arg.energy_end );
	zzmod_temp = ( z_end - x_start ) * y_rate;
	global_ftoa( zzmod_temp , m_zz_arg.energy_real , 6 );
	zzmod_temp = ((( z_end - x_start ) - energy_count_real) / energy_count_real) * 100;
	global_ftoa( zzmod_temp , m_zz_arg.energy_zz_err, 6 );
	GLOBAL_MEMCLR(m_zz_arg.zz_time_min, 8);
	app_zzmod_edit_display();
	GUI_3D_window_title_mod(m_zz_3DWIN,(char *)m_title_buf[g_language]);
	app_ZZ_control_tip(g_zz_F5_count);
	return;
}
/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :�������ܼ���
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_zzmod_measure_energy_calc( void )
{
	ZZ_Pulse_Get( energy_count_end );
	if ( energy_count_end > energy_count_start )
	{
		energy_count_real = (float)(energy_count_end - energy_count_start);
		energy_count_real = energy_count_real / const_energy;
	}
	else if ( energy_count_end < energy_count_start )
	{
		energy_count_real = (float)(0xffffffff - energy_count_start + energy_count_end);
		energy_count_real = energy_count_real / const_energy;
	}
	global_ftoa( energy_count_real , m_zz_arg.energy_measure, 4 );
	app_ZZmod_edit_instrument_energy_display(energy_count_real);
	return;
}
/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :��������
 * @���� :*data����  len����
 * @��� :NONE
********************************************************************/
static void Usart3_send(void *data,Bsize_t len)
{
	os_driver_write( s_usart3_dev_id, data,len, 0 );
	return;
}
/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :������ѡ��
 * @���� :m_offset:������ѡ��ƫ��
 * @��� :NONE
********************************************************************/
static void baud_select(uint8_t m_offset)
{
	uint32_t m_baud;	
	switch(m_offset)
	{
		case 1:
			m_baud=1200;
			zz_baud_delay = 3;
			break;
		case 2:
			m_baud=2400;
			zz_baud_delay = 4;
			break;
		case 3:
			m_baud=4800;
			zz_baud_delay = 5;
			break;
		case 4:
			m_baud=9600;
			zz_baud_delay = 6;
			break;
		case 5:
			m_baud=600;
			zz_baud_delay = 2;
			break;	
		case 6:
			m_baud=300;
			zz_baud_delay = 1;
			break;	
		case 7:
			m_baud=19200;
			zz_baud_delay = 7;
			break;	
		default:
			m_baud=1200;
			zz_baud_delay = 3;
			break;
	}	
	os_driver_ioctl(s_usart3_dev_id, SET_SERIAL_BAUD, &m_baud);
	return;
}
/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :У�鷽ʽѡ��
 * @���� :m_offset:У��ѡ��ƫ��;
 * @��� :NONE
********************************************************************/
static void CRC_select( uint8_t m_offset )
{
	uint32_t m_CRC;	
	switch(m_offset)
	{
		case 1:
			m_CRC=2;  //żУ��
			break;
		case 2:
			m_CRC=0;  //��У��
			break;
		case 3:		
			m_CRC=1;  //��У��
			break;
		default:
			m_CRC=0;  //��У��
			break;
	}
	os_driver_ioctl(s_usart3_dev_id, SET_SERIAL_CHECK, &m_CRC);
	return;
}

/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :���Ͷ�97Э����������
* @���� :m_offset:����ѡ��ƫ��
* @��� :NONE
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
	send_cmd_97.crc=global_sum_array((char*)&send_cmd_97.head1,sizeof(send_data_97_t)-5,IS_CHAR);
	send_cmd_97.end		= 0x16;
	Usart3_send((char*)&send_cmd_97,sizeof(send_data_97_t));
	
	return;
}
/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :���Ͷ�07Э����������
* @���� :m_offset:����ѡ��ƫ��
* @��� :NONE
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
	send_cmd_07.crc		= global_sum_array((char*)&send_cmd_07.head1,sizeof(send_data_07_t)-5,IS_CHAR);
	send_cmd_07.end		= 0x16;
	Usart3_send((char*)&send_cmd_07,sizeof(send_data_07_t));
	
	return;
}
/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :���Ͷ���ָ��
 * @���� :NONE
 * @��� :NONE
********************************************************************/
static void send_read_meter_cmd_select(void)
{
	if( g_zz_07_or_97== 0 )
	{
		send_read_meter_cmd_97(zz_send_offset);
	}
	else if( g_zz_07_or_97 ==1 )
	{
		send_read_meter_cmd_07(zz_send_offset);
	}
	return;
}
/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :97��Լ���ݷ���
* @���� :*data:���ݻ���ָ��
* @��� :NONE
********************************************************************/
static void data_handle_guiyue_97(uint8_t*data)
{
	uint8_t offset1=0,offset2=0;
	uint8_t biaoshi1=0;
	uint8_t biaoshi2=0;
	float   f_power[6]={0,}; 
	uint8_t s_data=0;		  //��ȥ��0x33������
	float   f_data=0;  
	float 	fbs;
	biaoshi1=(data[11]-0x33) & 0xff;
	biaoshi2=(data[10]-0x33) & 0xff;	
	if ((( biaoshi2 & 0x0F ) == 0x0F) && (biaoshi1 == 0x90 || biaoshi1 == 0x91)) //���ݿ�
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
	else if ( ( biaoshi1 == 0x90 ) && ( biaoshi2 == 0x10 ) )
	{
		fbs = 1.0;
		f_data = 0;
		for (offset2 = 0;offset2 < 4;offset2++ )
		{
			s_data = data[12+0*4+offset2] - 0x33;
			if ( (s_data & 0xff) == 0xff )	s_data = 0;
			f_data += fbs * (((s_data>> 4 ) & 0x0F ) * 10 + ( s_data & 0x0F ) );
			fbs *= 100.0;
		}
		f_power[0] = f_data * 0.01;
		if ( s_zz_run_flag == 0 )
			global_ftoa(f_power[0], m_zz_arg.energy_start, 3);
		else if ( s_zz_run_flag == 1 )
			global_ftoa(f_power[0], m_zz_arg.energy_end, 3);
		app_ZZmod_edit_meter_energy_display(
			global_atof(m_zz_arg.energy_start), 
			global_atof(m_zz_arg.energy_end));
	}
	else if ( ( biaoshi1 == 0x91 ) && ( biaoshi2 == 0x10 ) )
	{
		fbs = 1.0;
		f_data = 0;
		for (offset2 = 0;offset2 < 4;offset2++ )
		{
			s_data = data[12+0*4+offset2] - 0x33;
			if ( (s_data & 0xff) == 0xff )	s_data = 0;
			f_data += fbs * (((s_data>> 4 ) & 0x0F ) * 10 + ( s_data & 0x0F ) );
			fbs *= 100.0;
		}
		f_power[0] = f_data * 0.01;
		if ( s_zz_run_flag == 0 )
			global_ftoa(f_power[0], m_zz_arg.energy_start, 3);
		else if ( s_zz_run_flag == 1 )
			global_ftoa(f_power[0], m_zz_arg.energy_end, 3);
		app_ZZmod_edit_meter_energy_display(
			global_atof(m_zz_arg.energy_start), 
			global_atof(m_zz_arg.energy_end));
	}
	
	if (( biaoshi1 == 0x90 ) && ( biaoshi2 == 0x1F ) )//�����й��������ݿ�
	{
		f_power[0] = f_power[1] + f_power[2] + f_power[3] + f_power[4];
		if ( s_zz_run_flag == 0 )
			global_ftoa(f_power[0], m_zz_arg.energy_start, 3);
		else if ( s_zz_run_flag == 1 )
			global_ftoa(f_power[0], m_zz_arg.energy_end, 3);
		app_ZZmod_edit_meter_energy_display(
			global_atof(m_zz_arg.energy_start), 
			global_atof(m_zz_arg.energy_end));
	}
	else if (( biaoshi1 == 0x91 ) && ( biaoshi2 == 0x1F ) )//�����޹��������ݿ�
	{
		f_power[0] = f_power[1] + f_power[2] + f_power[3] + f_power[4];
		if ( s_zz_run_flag == 0 )
			global_ftoa(f_power[0], m_zz_arg.energy_start, 3);
		else if ( s_zz_run_flag == 1 )
			global_ftoa(f_power[0], m_zz_arg.energy_end, 3);
		app_ZZmod_edit_meter_energy_display(
			global_atof(m_zz_arg.energy_start), 
			global_atof(m_zz_arg.energy_end));
	}
	else if (( biaoshi1 == 0x90 ) && ( biaoshi2 == 0x2F ) )//�����й��������ݿ�
	{
		f_power[0] = f_power[1] + f_power[2] + f_power[3] + f_power[4];
	}
	else if (( biaoshi1 == 0x91 ) && ( biaoshi2 == 0x2F ) )//�����޹��������ݿ�
	{
		f_power[0] = f_power[1] + f_power[2] + f_power[3] + f_power[4];
	}

	return;
}
/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :07��Լ���ݷ���
* @���� :*data:���ݻ���ָ��
* @��� :NONE
********************************************************************/
static void data_handle_guiyue_07(uint8_t *data)
{

	uint8_t offset1=0,offset2=0;
	uint8_t biaoshi1=0,biaoshi2=0,biaoshi3=0,biaoshi4=0;
	float   f_power[6]={0,}; 
	uint8_t s_data=0;		  //��ȥ��0x33������
	float   f_data=0;  
	float 	fbs;
	biaoshi1=(data[13]-0x33) & 0xff;
	biaoshi2=(data[12]-0x33) & 0xff;
	biaoshi3=(data[11]-0x33) & 0xff;	
	biaoshi4=(data[10]-0x33) & 0xff;
	if((biaoshi1==0x00 && biaoshi3==0xff && biaoshi4==0x00) &&
		(biaoshi2==0x00||biaoshi2==0x01||biaoshi2==0x02||biaoshi2==0x03||biaoshi2==0x04))//���ݿ�
	{
		for ( offset1 = 0;offset1 < 5;offset1++ )
		{
			fbs = 1.0;
			f_data = 0;
			for (offset2 = 0;offset2 < 4;offset2++ )
			{
				s_data = data[14+offset1*4+offset2] - 0x33;
				if ( (s_data & 0xff) == 0xff )	s_data = 0;
				if ( (offset2 == 3) && (s_data & 0x80) == 0x80 )
					s_data &= 0x7f; 
				f_data += fbs * (((s_data>> 4 ) & 0x0F ) * 10 + ( s_data & 0x0F ) );
				fbs *= 100.0;
			}
			f_power[offset1] = f_data * 0.01;
			if (((data[14+offset1*4+3] - 0x33) & 0x80) == 0x80 )
				f_power[offset1] = -f_power[offset1];
		}
		if ( (biaoshi2 == 0x01||biaoshi2 == 0x00) && biaoshi1 == 0x00 )//�����й��������ݿ�
		{
			f_power[0] = f_power[1] + f_power[2] + f_power[3] + f_power[4];
			if ( s_zz_run_flag == 0 )
				global_ftoa(f_power[0], m_zz_arg.energy_start, 3);
			else if ( s_zz_run_flag == 1 )
				global_ftoa(f_power[0], m_zz_arg.energy_end, 3);
			app_ZZmod_edit_meter_energy_display(
			global_atof(m_zz_arg.energy_start), 
			global_atof(m_zz_arg.energy_end));
		}
		else if ( biaoshi2 == 0x03 && biaoshi1 == 0x00)//�����޹��������ݿ�
		{
			f_power[0] = f_power[1] + f_power[2] + f_power[3] + f_power[4];
			if ( s_zz_run_flag == 0 )
				global_ftoa(f_power[0], m_zz_arg.energy_start, 3);
			else if ( s_zz_run_flag == 1 )
				global_ftoa(f_power[0], m_zz_arg.energy_end, 3);
			app_ZZmod_edit_meter_energy_display(
			global_atof(m_zz_arg.energy_start), 
			global_atof(m_zz_arg.energy_end));
		}
		else if ( biaoshi2 == 0x02 && biaoshi1 == 0x00)//�����й��������ݿ�
		{
			f_power[0] = f_power[1] + f_power[2] + f_power[3] + f_power[4];
		}
		else if ( biaoshi2 == 0x04 && biaoshi1 == 0x00)//�����޹��������ݿ�
		{
			f_power[0] = f_power[1] + f_power[2] + f_power[3] + f_power[4];
		}
	}
	else if ( biaoshi1==0x00 && biaoshi2==0x00 && biaoshi3==0x00 && biaoshi4==0x00 )
	{//����й�
		fbs = 1.0;
		f_data = 0;
		for (offset2 = 0;offset2 < 4;offset2++ )
		{
			s_data = data[14+0*4+offset2] - 0x33;
			if ( (s_data & 0xff) == 0xff )	s_data = 0;
			if ( (offset2 == 3) && (s_data & 0x80) == 0x80 )
				s_data &= 0x7f; 
			f_data += fbs * (((s_data>> 4 ) & 0x0F ) * 10 + ( s_data & 0x0F ) );
			fbs *= 100.0;
		}
		f_power[0] = f_data * 0.01;
		if (((data[14+offset1*4+3] - 0x33) & 0x80) == 0x80 )
			f_power[offset1] = -f_power[offset1];
		if ( s_zz_run_flag == 0 )
				global_ftoa(f_power[0], m_zz_arg.energy_start, 3);
		else if ( s_zz_run_flag == 1 )
			global_ftoa(f_power[0], m_zz_arg.energy_end, 3);
		app_ZZmod_edit_meter_energy_display(
		global_atof(m_zz_arg.energy_start), 
		global_atof(m_zz_arg.energy_end));
	}
	else if ( biaoshi1==0x00 && biaoshi2==0x03 && biaoshi3==0x00 && biaoshi4==0x00 )
	{//����޹�
		fbs = 1.0;
		f_data = 0;
		for (offset2 = 0;offset2 < 4;offset2++ )
		{
			s_data = data[14+0*4+offset2] - 0x33;
			if ( (s_data & 0xff) == 0xff )	s_data = 0;
			if ( (offset2 == 3) && (s_data & 0x80) == 0x80 )
				s_data &= 0x7f; 
			f_data += fbs * (((s_data>> 4 ) & 0x0F ) * 10 + ( s_data & 0x0F ) );
			fbs *= 100.0;
		}
		f_power[0] = f_data * 0.01;
		if (((data[14+offset1*4+3] - 0x33) & 0x80) == 0x80 )
			f_power[offset1] = -f_power[offset1];
		if ( s_zz_run_flag == 0 )
				global_ftoa(f_power[0], m_zz_arg.energy_start, 3);
		else if ( s_zz_run_flag == 1 )
			global_ftoa(f_power[0], m_zz_arg.energy_end, 3);
		app_ZZmod_edit_meter_energy_display(
		global_atof(m_zz_arg.energy_start), 
		global_atof(m_zz_arg.energy_end));
	}
	
	return;
}
/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :����Ӧ�������ݷ���
* @���� :NONE
* @��� :NONE
********************************************************************/
static void auto_read_data_send(void)
{
	if ( s_zz_run_flag == 0 || s_self_adapt_first == 0 )
	{
		zz_offset_crc++;	
		if ( zz_offset_crc > 3 )
		{
			zz_offset_crc = 1;
			zz_offset_baud++;
			if ( zz_offset_baud > 7 )
			{	
				zz_offset_baud = 0;
				zz_self_adapt_num++;
				if ( zz_self_adapt_num > 2 )
				{
					zz_self_adapt_num = 0;
					zz_self_adapt_end = 1;
				}
			}
		}
		
		baud_select(zz_offset_baud);	
		CRC_select(zz_offset_crc);
		msleep(1);
		send_read_meter_cmd_select();
	}
	else 
	{
		//s_self_adapt_first = 0;
		//baud_select(b_offset_baud);	
		//CRC_select(b_offset_crc);
		msleep(1);
		send_read_meter_cmd_select();
	}
	return;
}

/********************************************************************
 * @������ :�ܱ�Ȩ
 * @���� :ģ��ȷ��ͨѶ����
 * @���� :NONE
 * @��� :NONE
********************************************************************/
static void ensure_communication( void*parg )
{
	Bsize_t size;
	Bsize_t Fsize = 5;
	const char *tip_buf[2] 		= {"�޷�����ܱ���ͨѶ,������·�͹����Ƿ�����!",""};
	const char *tip2_buf[2] 	= {"��������ܱ���ͨѶ����ȡ����ֵ.",""};
	const char *m_title_buf[2] 	= {"���ڽ������ֲ���",""};
	uint8_t rev_data[100];
	zz_offset_crc 			= 0;
	zz_offset_baud 			= 1;
	zz_baud_delay			= 3;
	zz_send_offset			= 0;
	zz_self_adapt_end 		= 0;
	zz_self_adapt_num		= 0;
	GLOBAL_MEMCLR(rev_data, 100);

	GUI_3D_window_title_mod(m_zz_3DWIN,(char *)tip2_buf[g_language]);
	while (1)
	{
		msleep(TYPE_DELAY);
		if ( os_task_delete_req( SELF_PRO ) == OS_TASK_DEL_REQ )
		{
			_task_cmt_id = INVALID_PRO;
			os_task_delete( SELF_PRO );   			
		}
		
		auto_read_data_send();
		msleep(10);
		if ( zz_self_adapt_end == 1 )
		{
			zz_self_adapt_end = 0;
			GUI_3D_window_title_mod(m_zz_3DWIN,(char *)tip_buf[g_language]);
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
		if ( zz_baud_delay == 1 )	 msleep(90);//300
		else if ( zz_baud_delay == 2 )msleep(45);//600
		else if ( zz_baud_delay == 3 )msleep(25);//1200
		else if ( zz_baud_delay == 4 )msleep(15);//2400
		else if ( zz_baud_delay == 5 )msleep(10);//4800
		else if ( zz_baud_delay == 6 )msleep(5);	//9600
		else if ( zz_baud_delay == 7 )msleep(3);	//19200
		size = os_driver_read( s_usart3_dev_id, &rev_data[1], 7, 0 );
		if ( size != 7 || ( rev_data[7] != 0x68 ) )  //�յڶ���68H��
		{
			continue;
		}
		size = os_driver_read( s_usart3_dev_id, &rev_data[8], 2, 0 );
		if ( size != 2 )	  						//�տ�����ͳ���,8Ϊ������,9����;
		{
			continue;
		}
		size = os_driver_read( s_usart3_dev_id, &rev_data[10], rev_data[9], 0 );		
		size = os_driver_read( s_usart3_dev_id, &rev_data[10+rev_data[9]], 2, 0 );		
		if( (size == 2) && (rev_data[10+rev_data[9]+1]==0x16) )
		{
			if( g_zz_07_or_97 == 0 ) //97
			{
				data_handle_guiyue_97(rev_data);
				s_self_adapt_first = 1;
				b_offset_baud = zz_offset_baud;
				b_offset_crc  = zz_offset_crc;
			}
			else if( g_zz_07_or_97 == 1 )//07
			{
				data_handle_guiyue_07(rev_data);
				s_self_adapt_first = 1;
				b_offset_baud = zz_offset_baud;
				b_offset_crc  = zz_offset_crc;
			}
			if ( s_zz_run_flag == 0 )
			{
				ZZ_Pulse_Get( energy_count_start );
				app_zzmod_edit_display();
				GUI_3D_window_title_mod(m_zz_3DWIN,(char *)m_title_buf[g_language]);
				app_ZZ_control_tip(g_zz_F5_count);
				s_zz_running_flag = 1;
				_task_cmt_id = INVALID_PRO;
				os_task_delete( SELF_PRO ); 
			}
			else if ( s_zz_run_flag == 1 )
			{
				s_zz_running_flag = 0;
				zz_end_for_calc_handle();
				_task_cmt_id = INVALID_PRO;
				os_task_delete( SELF_PRO ); 
			}
		}
	}
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :�����ܲ�����������
 * @���� :arg�������
 * @��� :NONE
********************************************************************/
uint8_t	app_zzmod_arg_set( hwin_t *hp )
{
	uint8_t	key;
	uint8_t m_flag = 0;
	Bsize_t	member;
	
	GUI_window_hwin_req( hp );
	hand_wirte_input_box_pos_set(30,280);
	key_input_method_filter(KEY_INPUT_123);
	GUI_window_hwin_release( hp );
	if ( s_zz_run_flag == 1 && g_zz_ITorMC_meter == 1 )
	{
		app_global_key_dummy_press( KEY_LEFT );
	}
	app_global_key_control_power_req();
	while ( 1 )
	{
		msleep( TYPE_DELAY );
		if ( app_global_key_force_obtain( &key, 1 ) !=  1 )	/*�ж������Ƿ��յ�������Ӧ*/
		{
			continue;
		}
		GUI_window_hwin_req( hp );
		if ( s_zz_run_flag == 1 && g_zz_ITorMC_meter == 1 && m_flag == 0 )
		{
			GUI_edit_select(EDIT_ENERGY_END);
			m_flag = 1;
		}
		GUI_edit_run_hook( key );
		GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
		GUI_set_Fgcolor( C_GUI_YELLOW );
		
		if (( app_global_decide_inputkey( key ) ) != FAIL )	/*�ж��Ƿ������뷨��ذ�������*/
		{
			member = GUI_edit_id_obtain();
			
			if ( member == EDIT_ENERGY_START )
			{
				key_method_input_shadow( key, m_zz_arg.energy_start );
			}
			else if ( member == EDIT_ENERGY_END )
			{
				key_method_input_shadow( key , m_zz_arg.energy_end );
			}
			else if ( member == EDIT_ENERGY_PRESET )
			{
				key_method_input_shadow( key , m_zz_arg.energy_preset );
			}
			else if ( member == EDIT_ENERGY_TIME )
			{
				key_method_input_shadow( key , m_zz_arg.zz_time_min );
			}
		}
		else if( key == KEY_DOWN || key == KEY_UP )
		{
			member = GUI_edit_id_obtain();
			if ( key == KEY_DOWN )
			{
				if ( member == EDIT_ENERGY_START )
					GUI_edit_select(EDIT_ENERGY_END);
				else if ( member == EDIT_ENERGY_END )
					GUI_edit_select(EDIT_ENERGY_PRESET);
				else if ( member == EDIT_ENERGY_PRESET )
					GUI_edit_select(EDIT_ENERGY_TIME);
				else if ( member == EDIT_ENERGY_TIME )
					GUI_edit_select(EDIT_ENERGY_START);
			}
			else if ( key == KEY_UP )
			{
				if ( member == EDIT_ENERGY_START )
					GUI_edit_select(EDIT_ENERGY_TIME);
				else if ( member == EDIT_ENERGY_TIME )
					GUI_edit_select(EDIT_ENERGY_PRESET);
				else if ( member == EDIT_ENERGY_PRESET )
					GUI_edit_select(EDIT_ENERGY_END);
				else if ( member == EDIT_ENERGY_END )
					GUI_edit_select(EDIT_ENERGY_START);
			}
		}
		GUI_window_hwin_release( hp );
		if ( key == GUI_KEY_ENTER || key == KEY_ENTER )
		{
			if ( g_zz_ITorMC_meter == 1 && s_zz_run_flag == 1 )
			{
				GLOBAL_MEMCLR(m_zz_arg.zz_time_min, 8);
				app_global_key_dummy_press( KEY_ENTER );
			}
			if ( global_atof(m_zz_arg.zz_time_min) > 0.1 )
				s_zz_time_flag = 1;
			break;
		}
		else if ( key == KEY_EXIT )
		{
			GLOBAL_MEMCLR( &m_zz_arg, sizeof( zzmod_arg ) );
			global_memcpy( &m_zz_arg.energy_measure, "0.000000", 8 );
			s_zz_time_flag = 0;
			break;
		}
	}
	app_global_key_control_power_release();
	return key;
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :���ֿ�ʼԴ�ȶ�������
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_zzmod_zz_XNFZ_steady_handle( void )
{
	const char *m_title_buf[2] 	= {"���⸺�ػ�û�ȶ�,���ȶ����ٿ�ʼ���ֲ���",""};
	GUI_3D_window_title_mod(m_zz_3DWIN,(char *)m_title_buf[g_language]);
	return;
}

void app_zzmod_XNFZ_steady_handle( void )
{
	const char *m_title_buf[2] 	= {"���⸺���Ѿ��ȶ�,�ȵ��λ��ɺ�Ϳ��Կ�ʼ���ֲ���!",""};
	GUI_3D_window_title_mod(m_zz_3DWIN,(char *)m_title_buf[g_language]);
	return;
}

/********************************************************************
 * @������ :�ܱ�Ȩ
 * @���� :���ֿ�ʼ����
 * @���� :NONE
 * @��� :NONE
********************************************************************/
static void zz_start_handle( void )
{
	const char *m_title_buf[2] 	= {"���ڽ������ֲ���",""};

	if ( g_zz_ITorMC_meter == 0 )
	{
		GLOBAL_MEMCLR( m_zz_arg.energy_start, 12 );
	}
	GLOBAL_MEMCLR( m_zz_arg.energy_end, 12 );
	GLOBAL_MEMCLR( m_zz_arg.energy_real, 8 );
	GLOBAL_MEMCLR( m_zz_arg.energy_measure, 8 );
	GLOBAL_MEMCLR( m_zz_arg.energy_zz_err, 8 );
	app_zzmod_edit_display();
	zz_time_start_handle();
	s_zz_run_flag		= 0;
	if ( g_zz_ITorMC_meter == 0 )
	{
		if (MsgBox(200,200,230,240,"��ʾ!","Ϊ�˲�����������ʵ�����,��������Ԥ�����ֵ��ܴ���1ǧ��ʱ�����ֶ�ʱ��30��������!��<ȷ��>��ʼ����.",APP_HALF_SEC*30) == GUIE_KEY_ENTER )
		{
			if ( _task_cmt_id == INVALID_PRO )
			{
				_task_cmt_id = os_task_assign_stk_build( ensure_communication,4);
			}
		}
	}
	else if ( g_zz_ITorMC_meter == 1 )
	{
		ZZ_Pulse_Get( energy_count_start );
		GUI_3D_window_title_mod(m_zz_3DWIN,(char *)m_title_buf[g_language]);
		s_zz_running_flag = 1;
		app_ZZ_control_tip(g_zz_F5_count);
	}
	return;
}

/********************************************************************
 * @������ :�ܱ�Ȩ
 * @���� :���ֽ�������
 * @���� :NONE
 * @��� :NONE
********************************************************************/
static void zz_end_handle( void )
{
	const char *m_title10_buf[2] = {"������������ܺ�ȷ��",""};

	s_zz_run_flag		= 1;
	s_zz_time_flag		= 0;
	if ( g_zz_ITorMC_meter == 0 )
	{
		if ( _task_cmt_id == INVALID_PRO )
		{
			_task_cmt_id = os_task_assign_stk_build( ensure_communication,4);
		}
	}
	else if ( g_zz_ITorMC_meter == 1 )
	{
		app_global_key_dummy_press( GUI_KEY_SET );
		GUI_3D_window_title_mod(m_zz_3DWIN,(char *)m_title10_buf[g_language]);
		s_zz_running_flag = 0;
	}
	g_zz_F5_count = 0;
	app_ZZ_control_tip(g_zz_F5_count);
	return;
}

/********************************************************************
 * @������ :�ܱ�Ȩ
 * @���� :���ְ���������
 * @���� :NONE
 * @��� :NONE
********************************************************************/
static void key_handle(void)
{
	uint8_t   key_buf;
	const char *m_title1_buf[2] = {"�й���������",""};
	const char *m_title2_buf[2] = {"����������������ʼ����",""};
	const char *m_title3_buf[2] = {"��·�����޵�ѹ,���鸺���Ƿ�������!",""};
	
	if ( app_global_key_obtain( &key_buf, 1 ) == 1 )
	{
		if( key_buf == KEY_EXIT || key_buf == GUI_KEY_EXIT)
		{
			//app_module_assignExt("START");
			app_module_switch_last_mod();
		}
		else if ( key_buf == KEY_F5 )
		{
			if ( g_zz_F5_count == 0 )
			{
				g_zz_F5_count = 1;
				app_global_key_dummy_press( GUI_KEY_ZZ_BEGIN );
			}
			else if ( g_zz_F5_count == 1 )
			{
				g_zz_F5_count = 0;
				app_global_key_dummy_press( GUI_KEY_ZZ_END );
			}
		}
		else if ( key_buf == KEY_SETTING || key_buf == GUI_KEY_SET || key_buf == GUI_KEY_EDIT_START )
		{
			GUI_edit_start( m_zz_edit_win, app_zzmod_arg_set);
			app_zzmod_edit_display();
			app_ZZmod_edit_meter_energy_display(
			global_atof(m_zz_arg.energy_start), 
			global_atof(m_zz_arg.energy_end));
		}
		else if ( key_buf == GUI_KEY_METER_ITMC )
		{
			g_zz_ITorMC_meter = GUI_radio_curmember_obtain(m_zzmod_radio_ITORMC);
			if ( g_zz_ITorMC_meter == 1 )
			{
				GUI_3D_window_title_mod(m_zz_3DWIN,(char *)m_title2_buf[g_language]);
			}
			else 
			{
				GUI_3D_window_title_mod(m_zz_3DWIN,(char *)m_title1_buf[g_language]);
			}
		}
		else if ( key_buf == GUI_KEY_TERATY_SELECT )
		{
			g_zz_07_or_97 = GUI_radio_curmember_obtain(m_zzmod_radio_TREATYSELECT);
		}
		else if ( key_buf == GUI_KEY_ZZ_BEGIN )
		{
			if ( g_DL_param.phase_a.f_U < 30 )
			{
				g_zz_F5_count = 0;
				app_ZZ_control_tip(g_zz_F5_count);
				GUI_3D_window_title_mod(m_zz_3DWIN,(char *)m_title3_buf[g_language]);
			}
			else 
			{
				//if (MsgBox(200,200,250,200,"��ʾ!","������ܱ��Ƿ�����������״̬,����<ȷ��>��ʼ����,����<�˳�>������",APP_HALF_SEC*30) == GUIE_KEY_ENTER )
				{
					zz_start_handle();
				}
			}
		}
		else if ( key_buf == GUI_KEY_ZZ_END && (s_zz_running_flag == 1) )
		{
			zz_end_handle();
		}
		else if ( (key_buf == KEY_ENTER || key_buf == GUI_KEY_ENTER) && 
			s_zz_run_flag == 1 && g_zz_ITorMC_meter == 1 )
		{
			zz_end_for_calc_handle();
			s_zz_run_flag = 0;
		}
		else if ( key_buf==KEY_RIGHT )
		{
			if(m_CURwidget != m_zzmod_radio_TREATYSELECT
				|| m_pMember != m_CURwidget->subord_end )
		  	GUI_WINshift(0);
		}
		else if ( key_buf==KEY_LEFT )
		{
			if(m_CURwidget!= m_zzmod_radio_ITORMC
					||m_pMember!=m_CURwidget->subord_head)
		  	GUI_WINshift(1);
		}			 
		else if ( key_buf==KEY_ENTER )
		{
			GUI_WINdo();
		}	
	}
	return;
}
/********************************************************************
 * @������ :�ܱ�Ȩ
 * @���� :ģ������������
 * @���� :NONE
 * @��� :NONE
********************************************************************/
static void _task( void*parg )
{
	int		cnt = 60;
	int		cnt2 = 0;

	//DL_flag_t DL_flag;
	app_zzmod_window_main_creat();
	fatfs_readARG("USERDATA",&g_user_arg,sizeof(DL_app_user_all_info_t));
    g_user_arg.DL_app_user_set.meter_1OR3 = 0;
    g_user_arg.DL_app_user_set.PorQ = 0;
    g_user_arg.DL_app_user_set.handOR_GD = 0;
    app_global_set_user_dl_arg(&g_user_arg);
	app_zzmod_edit_display();
	GUI_radio_select(m_zzmod_radio_ITORMC, g_zz_ITorMC_meter);
	GUI_radio_select(m_zzmod_radio_TREATYSELECT, g_zz_07_or_97);
	GUI_WINready(m_zzmod_radio_ITORMC,g_zz_ITorMC_meter);
	app_ZZ_control_tip(0);
	while (1)
	{
	    msleep(TYPE_DELAY);
		//app_global_DLflag_obtain(&DL_flag);

		if ( os_task_delete_req( SELF_PRO ) == OS_TASK_DEL_REQ )
		{
			if ( _task_cmt_id != INVALID_PRO )  OS_DELETE_REQ( _task_cmt_id );
			os_task_delete( SELF_PRO );   			
		}
		
		//if(++cnt%66==0 && DL_flag.DL_data_is_new )
		if( ++cnt%66==0 )
		{
		    app_global_DLarg_obtain(&g_DL_param);
			app_ZZmod_DLarg_display(&g_DL_param);
			zz_time_reach_end();
		}
		if ( (++cnt2%23==0) && (s_zz_running_flag == 1) )
		{
			app_zzmod_energy_trends_display();
			zz_preset_reach_end();
		}
		key_handle();
	}
}
/********************************************************************
 * @������ :�ܱ�Ȩ
 * @���� :ģ�������ܳ�ʼ��
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void  app_zzmod_fun_entry( void )
{
	_task_id = os_task_assign_stk_build( _task,4);
	app_global_DL_measure_entery();
	s_usart3_dev_id = os_driver_open( "D232" );
	mod_flags_init();
	return;
}
/********************************************************************
 * @������ :�ܱ�Ȩ
 * @���� :ģ���������ͷ���Դ
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void  app_zzmod_fun_exit( void )
{
	app_zzmod_window_destroy();
	if ( _task_id != INVALID_PRO )
	{
		OS_DELETE_REQ( _task_id );
		_task_id = INVALID_PRO;
	}
	if ( _task_cmt_id != INVALID_PRO )
	{
		OS_DELETE_REQ( _task_cmt_id );
		_task_cmt_id = INVALID_PRO;
	}
	os_driver_close(s_usart3_dev_id);
	os_driver_ioctl(g_UIout_dev,UIOUT_CLOSE,0);
	app_global_DL_measure_exit();
	return;
}


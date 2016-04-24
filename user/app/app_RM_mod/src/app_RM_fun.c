/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:��ʼģ���������
* ����ʱ��:2013-04-26
* ������˾ :����
* �ļ�����:app_RMmod_fun.c
* ���� :�ܱ�Ȩ
* ��������:ʵ��ģ�鹦��
******************************************************************************************/
#include	"app_RM_inside.h"

static  	uint8_t   _main_task_id	=	INVALID_PRO;
static  	uint8_t   _task_id		=	INVALID_PRO;
static  	uint8_t   _task2_id		=	INVALID_PRO;
uint8_t   	m_offset_guiyue=0,m_offset_baud=0,m_offset_crc=0;
uint8_t   	m_send_offset		=0;  	//���͵ڼ�������
dev_id_t	g_usart3_dev_id = INVALID_DEV_ID;
dev_id_t	g_timer_dev_id  = INVALID_DEV_ID;
dev_id_t	g_gps_dev_id  	= INVALID_DEV_ID;
dev_id_t	g_sht11_dev_id  = INVALID_DEV_ID;
uint8_t   	m_RM_rev_flag	=FAIL;  //�յ����ݱ�־
uint8_t  	m_start_flag	=FAIL;	//��ʼ�����ݱ�־
uint8_t   	m_baud_add_flag	=TURE;
uint8_t   	m_baud_auto_flag=TURE;	//����������Ӧ��־
uint8_t   	m_CRC_auto_flag	=TURE; 	//У�鷽ʽ����Ӧ��־
uint8_t   	m_read_cnt		=0;		//�����������(��������δ����������Ϊ��ʱ)
uint8_t		g_read_continue =0;
uint8_t		g_broadcast_flag =0;
uint8_t		g_moon_CheckTime_flag =0;
uint8_t		g_moon_CheckTime_finish =0;
uint8_t		g_moon_CheckTime_cnt =0;
uint8_t		g_CheckTime_flag = 0;// 0:������ʱ;1:������ʱ;

GPS_Time_Date_t	GPS_TD_buf = {0,};

RMeter_dl_info_t m_meter_data={0,};
RM_save_data_t RM_save_data={0,};

char m_meter_addr[13] = {'9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', 0};
char m_meter07_addr[13] = {'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 0};
time_val_t m_meter_time_buf={0,};

const uint8_t m_cmd_97_DI1[] = {0x90,0x91,0x90,0x91,0xB6,0xB6,0xB6,0xB6,0xB6,0xB6, 0xB3,0xB3,0xB3,0xB3, 0xC0,0xC0};
const uint8_t m_cmd_97_DI0[] = {0x1F,0x1F,0x2F,0x2F,0x11,0x12,0x13,0x21,0x22,0x23, 0x21,0x22,0x23,0x20, 0x10,0x11};

const uint8_t m_cmd_07_DI3[] = {0x00,0x00,0x00,0x00,0x02,0x02, 0x03,0x03,0x04,0x04};
const uint8_t m_cmd_07_DI2[] = {0x00,0x03,0x02,0x04,0x01,0x02, 0x01,0x05,0x00,0x00};
const uint8_t m_cmd_07_DI1[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF, 0x00,0x00,0x01,0x01};
const uint8_t m_cmd_07_DI0[] = {0x00,0x00,0x00,0x00,0x00,0x00, 0x00,0x00,0x01,0x02};

//uint8_t 	g_RM_XNFZ_start = 0;
//uint8_t 	g_RM_XNFZ_steady = 0;
void *	    m_RMmod_keymsg_buf[MAX_HMSG_SIZE];		/*��Ϣ�������ݻ���*/
a_q_t *  	m_RMmod_keymsg_qevent = INVALID_SEM;    /*������Ϣ�����¼���־*/
hwin_t*	   _RMsave_edit;





static void _SaveData_load(RM_save_data_t *p)
{
	GLOBAL_MEMCLR(p, sizeof(RM_save_data_t));
	global_ftoa(g_pic_wd, m_meter_data.m_temp, 2);
	global_ftoa(g_pic_sd, m_meter_data.m_hum, 2);
	global_memcpy(&p->m_RM_data,&m_meter_data,sizeof(RMeter_dl_info_t));
	return;
}

static  void _Save_file(void)
{
	RM_save_data_t *p;
	p = app_malloc(sizeof(RM_save_data_t));
	if(p == PNULL) return;
	GLOBAL_MEMCLR(p, sizeof(RM_save_data_t));
    _SaveData_load(p);
	app_save_PECD("RMDATA",p,sizeof(RM_save_data_t));
	app_free(p);
    return;
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :����Уʱ���ú���
 * @���� :NONE;
 * @��� :NONE;
********************************************************************/
void mood_check_time_handle( void )
{
	int		cnt2 = 0;
	uint8_t	m_GPS_state = 0;
	while(g_moon_CheckTime_flag)
	{
		app_RMmod_rev_tip_display("���ڻ�ȡ���Ǳ�׼ʱ��.");
		os_driver_ioctl( g_gps_dev_id, GPS_START, 0);
		msleep(100);
		os_driver_ioctl( g_gps_dev_id, GPS_ANALYSIS_START, (void *)&m_GPS_state);
		if ( m_GPS_state == 1 )
		{
			os_driver_ioctl( g_gps_dev_id, GPS_TIME_GET, (void *)&GPS_TD_buf);
			UTC_Time_convert_BJ_Time(&GPS_TD_buf);
			os_driver_ioctl( g_timer_dev_id, TMR_CMD_TIME_STRING_SAVE, &GPS_TD_buf);
			if(m_meter_data.guiyue==0)//97
			{
				app_RMmod_time_hms_97_set();
			}
			else if(m_meter_data.guiyue==1)// 07
			{	
				app_RMmod_time_hms_07_set();
			}
			app_RMmod_rev_tip_display("��ȡ����ʱ��ɹ�,����Уʱ");
			msleep(3);
			g_moon_CheckTime_flag = 0;
			g_moon_CheckTime_finish = 1;
			m_start_flag = 1;
			cnt2 = 0;
		}
		cnt2++;
		if ( cnt2 > 55 )
		{
			cnt2 = 0;
			app_RMmod_rev_tip_display("�޷���ȡ��Ч������ʱ��!");
			g_moon_CheckTime_flag = 0;
			g_moon_CheckTime_finish = 0;
		}
	}
	return;
}
/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :д���ݣ���վ����֡(97)(��վ���վ������������(����))
 * @���� :m_DI:���ݱ�ʶ;m_buf:���ݻ���;m_size:���ݳ���;
 * @��� :NONE
********************************************************************/
void app_RMmod_97_set_meter(uint8_t* m_DI, void* m_buf, uint8_t m_size )
{
	uint8_t m_crc = 0;
	uint8_t m_offset = 0;
	uint8_t *m_buf_point = (uint8_t *)m_buf;
	set_data_97_t m_data_buf = {0,};
	
	m_data_buf.head[0]	=	0xFE;
	m_data_buf.head[1]	=	0xFE;
	m_data_buf.head[2]	=	0xFE;
	m_data_buf.head1	=	0x68;	
	m_data_buf.addr[0] 	= (( CharToHex(&m_meter_addr[10]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[11]) & 0x0F );//0x99;
	m_data_buf.addr[1] 	= (( CharToHex(&m_meter_addr[8]) & 0x0F ) << 4 ) + (CharToHex(& m_meter_addr[9]) & 0x0F );//0x99;
	m_data_buf.addr[2] 	= (( CharToHex(&m_meter_addr[6]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[7]) & 0x0F );//0x99;
	m_data_buf.addr[3] 	= (( CharToHex(&m_meter_addr[4]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[5]) & 0x0F );//0x99;
	m_data_buf.addr[4] 	= (( CharToHex(&m_meter_addr[2]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[3]) & 0x0F );//0x99;
	m_data_buf.addr[5] 	= (( CharToHex(&m_meter_addr[0]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[1]) & 0x0F );//0x99;
	m_data_buf.head2	=	0x68;
	m_data_buf.cmd		=	0x04;
	m_data_buf.len		=	0x02+m_size+4;	
	m_data_buf.biaoshi_L=	m_DI[0]+0x33;
	m_data_buf.biaoshi_H=	m_DI[1]+0x33;
	//Ȩ�޵�һλ,����������(˵��:��������Ϊ12 34 56 78  ,����ʱ��:12 78 56 34)
	m_data_buf.password[0]= 0x33+ (( CharToHex(&m_meter_Password[0]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_Password[1]) & 0x0F );
	m_data_buf.password[1]= 0x33+ (( CharToHex(&m_meter_Password[6]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_Password[7]) & 0x0F );
	m_data_buf.password[2]= 0x33+ (( CharToHex(&m_meter_Password[4]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_Password[5]) & 0x0F );
	m_data_buf.password[3]= 0x33+ (( CharToHex(&m_meter_Password[2]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_Password[3]) & 0x0F );
	m_crc = global_sum_array((char*)&m_data_buf.head1,sizeof(set_data_97_t)-5,IS_CHAR);
	m_data_buf.crc 		=	m_crc;
	m_data_buf.end		=	0x16;
	for ( m_offset = 0;m_offset < m_size; m_offset++ )
	{
		m_buf_point[m_offset] += 0x33;
	}
	m_crc = global_sum_array((char*)m_buf_point,m_size,IS_CHAR);
	m_crc += m_data_buf.crc;
	m_data_buf.crc = (m_crc & 0xff);
	
	app_RMmod_scia_send((char*)&m_data_buf.head1,sizeof(set_data_97_t)-5);
	app_RMmod_scia_send((char*)m_buf_point,m_size);
	app_RMmod_scia_send((char*)&m_data_buf.crc,2);
	return;
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :д���ݣ���վ����֡(07)(��վ���վ������������(����))
 * @���� :m_DI:���ݱ�ʶ;m_buf:���ݻ���;m_size:���ݳ���;
 * @��� :NONE
********************************************************************/
void app_RMmod_07_set_meter(uint8_t* m_DI, void* m_buf, uint8_t m_size )
{
	uint8_t m_crc = 0;
	uint8_t m_offset = 0;
	uint8_t *m_buf_point = (uint8_t *)m_buf;
	set_data_07_t m_data_buf = {0,};
	
	m_data_buf.head[0]	=	0xFE;
	m_data_buf.head[1]	=	0xFE;
	m_data_buf.head[2]	=	0xFE;
	m_data_buf.head1	=	0x68;	
	m_data_buf.addr[0] 	= (( CharToHex(&m_meter07_addr[10]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[11]) & 0x0F );
	m_data_buf.addr[1] 	= (( CharToHex(&m_meter07_addr[8]) & 0x0F ) << 4 ) + (CharToHex(&m_meter07_addr[9]) & 0x0F );
	m_data_buf.addr[2] 	= (( CharToHex(&m_meter07_addr[6]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[7]) & 0x0F );
	m_data_buf.addr[3] 	= (( CharToHex(&m_meter07_addr[4]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[5]) & 0x0F );
	m_data_buf.addr[4] 	= (( CharToHex(&m_meter07_addr[2]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[3]) & 0x0F );
	m_data_buf.addr[5] 	= (( CharToHex(&m_meter07_addr[0]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[1]) & 0x0F );
	m_data_buf.head2	=	0x68;
	m_data_buf.cmd		=	0x14;
	m_data_buf.len		=	4+4+4+m_size;
	m_data_buf.biaoshi[0]	=	m_DI[0]+0x33;
	m_data_buf.biaoshi[1]	=	m_DI[1]+0x33;
	m_data_buf.biaoshi[2]	=	m_DI[2]+0x33;
	m_data_buf.biaoshi[3]	=	m_DI[3]+0x33;
	//Ȩ�޵�һλ,����������(˵��:��������Ϊ12 34 56 78  ,����ʱ��:12 78 56 34)
	m_data_buf.password[0] 	=	0x33+ (( CharToHex(&m_meter_Password[0]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_Password[1]) & 0x0F );
	m_data_buf.password[1] 	=	0x33+ (( CharToHex(&m_meter_Password[6]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_Password[7]) & 0x0F );
	m_data_buf.password[2] 	=	0x33+ (( CharToHex(&m_meter_Password[4]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_Password[5]) & 0x0F );
	m_data_buf.password[3] 	=	0x33+ (( CharToHex(&m_meter_Password[2]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_Password[3]) & 0x0F );
	m_data_buf.user_number[0]=0x00+0x33;
	m_data_buf.user_number[1]=0x00+0x33;
	m_data_buf.user_number[2]=0x00+0x33;
	m_data_buf.user_number[3]=0x00+0x33;
	m_crc = global_sum_array((char*)&m_data_buf.head1,sizeof(set_data_07_t)-5,IS_CHAR);
	m_data_buf.crc 		= 	m_crc;
	m_data_buf.end 		= 	0x16;
	for ( m_offset = 0;m_offset < m_size; m_offset++ )
	{
		m_buf_point[m_offset] += 0x33;
	}
	m_crc = global_sum_array((char*)m_buf_point,m_size,IS_CHAR);
	m_crc += m_data_buf.crc;
	m_data_buf.crc = (m_crc & 0xff);
	
	app_RMmod_scia_send((char*)&m_data_buf.head1,sizeof(set_data_07_t)-5);
	app_RMmod_scia_send((char*)m_buf_point,m_size);
	app_RMmod_scia_send((char*)&m_data_buf.crc,2);
	return;
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :Դ�ȶ���ʾ
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_RMmod_XNFZ_steady_handle( void )
{
	const char *m_title_buf[2] 	= {"���⸺���Ѿ��ȶ�,�ȵ��λ��ɺ��ٽ��ж������!",""};
	GUI_3D_window_title_mod(m_RMwin_pWIN,(char *)m_title_buf[g_language]);
	return;
}

/********************************************************************
 * @���� :���ı�
 * @���� :��������
 * @���� :*data����  len����
 * @��� :NONE
********************************************************************/
void app_RMmod_Usart3_send(void *data,Bsize_t len)
{
	os_driver_write( g_usart3_dev_id, data,len, 0 );
	return;
}
/********************************************************************
 * @���� :���ı�
 * @���� :������ѡ��
 * @���� :
 * @��� :NONE
********************************************************************/
void app_RMmod_baud_select(uint8_t m_offset)
{
	uint32_t m_baud;	
	switch(m_offset)
	{
		case 1:
			m_baud=300;
			g_delay_baud_flag = 0;
			break;
		case 2:
			m_baud=600;
			g_delay_baud_flag = 1;
			break;
		case 3:
			m_baud=1200;
			g_delay_baud_flag = 2;
			break;
		case 4:
			m_baud=2400;
			g_delay_baud_flag = 3;
			break;
		case 5:
			m_baud=4800;
			g_delay_baud_flag = 4;
			break;	
		case 6:
			m_baud=9600;
			g_delay_baud_flag = 5;
			break;	
		case 7:
			m_baud=19200;
			g_delay_baud_flag = 6;
			break;	
		default:
			m_baud=300;
			g_delay_baud_flag = 0;
			break;
			
	}	
	os_driver_ioctl(g_usart3_dev_id, SET_SERIAL_BAUD, &m_baud);
	return;
}
/********************************************************************
 * @���� :���ı�
 * @���� :У�鷽ʽѡ��
 * @���� :
 * @��� :NONE
********************************************************************/
void app_RMmod_CRC_select(uint8_t m_offset,uint8_t m_flag)
{
	uint32_t m_CRC;	
	switch(m_offset)
	{
		case 1:
			m_CRC=0;  //��У��
			break;
		case 2:
			m_CRC=1;  //��У��
			break;
		case 3:		
			m_CRC=2;  //żУ��
			break;
		default:
			m_CRC=0;  //��У��
			break;
	}

	if ( m_flag == 1 )
	os_driver_ioctl(g_usart3_dev_id, SET_SERIAL_CHECK2, &m_CRC);
	else os_driver_ioctl(g_usart3_dev_id, SET_SERIAL_CHECK, &m_CRC);
	return;
}
/********************************************************************
 * @���� :���ı�
 * @���� :�ַ���ת��hex������
 * @���� :*str���� 
 * @��� :NONE
********************************************************************/
int CharToHex(char * str)
{
	int value=0;
	if(*str>='A'&&*str<='F')
		value=*str-55;
	else
		value=*str-48;
	return value;
} 
/********************************************************************
* @���� :���ı�
* @���� :meterģ�鷢�Ͷ�97Э����������
* @���� :NONE
* @��� :NONE
********************************************************************/
void app_RMmod_send_read_meter_cmd_97(uint8_t offset)
{
	send_data_97_t send_cmd_97={0,};
	GLOBAL_MEMCLR(&send_cmd_97, sizeof(send_data_97_t));
	send_cmd_97.head[0]=0xFE;
	send_cmd_97.head[1]=0xFE;
	send_cmd_97.head[2]=0xFE;
	send_cmd_97.head1=0x68;	
	send_cmd_97.addr[0] = (( CharToHex(&m_meter_addr[10]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[11]) & 0x0F );//0x99;
	send_cmd_97.addr[1] = (( CharToHex(&m_meter_addr[8]) & 0x0F ) << 4 ) + (CharToHex(& m_meter_addr[9]) & 0x0F );//0x99;
	send_cmd_97.addr[2] = (( CharToHex(&m_meter_addr[6]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[7]) & 0x0F );//0x99;
	send_cmd_97.addr[3] = (( CharToHex(&m_meter_addr[4]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[5]) & 0x0F );//0x99;
	send_cmd_97.addr[4] = (( CharToHex(&m_meter_addr[2]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[3]) & 0x0F );//0x99;
	send_cmd_97.addr[5] = (( CharToHex(&m_meter_addr[0]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[1]) & 0x0F );//0x99;
	send_cmd_97.head2=0x68;
	send_cmd_97.cmd=0x01;
	send_cmd_97.len=0x02;	
	send_cmd_97.biaoshi_L=m_cmd_97_DI0[offset]+0x33;
	send_cmd_97.biaoshi_H=m_cmd_97_DI1[offset]+0x33;
	send_cmd_97.crc=global_sum_array((char*)&send_cmd_97.head1,sizeof(send_data_97_t)-5,IS_CHAR);
	send_cmd_97.end=0x16;
	app_RMmod_Usart3_send((char*)&send_cmd_97,sizeof(send_data_97_t));
	
	return;
}
/********************************************************************
* @���� :���ı�
* @���� :meterģ�鷢�Ͷ�07Э����������
* @���� :NONE
* @��� :NONE
********************************************************************/
void app_RMmod_send_read_meter_cmd_07(uint8_t offset)
{
	send_data_07_t send_cmd_07={0,};
	GLOBAL_MEMCLR(&send_cmd_07, sizeof(send_data_07_t));
	send_cmd_07.head[0]=0xFE;
	send_cmd_07.head[1]=0xFE;
	send_cmd_07.head[2]=0xFE;
	send_cmd_07.head1=0x68;	
	send_cmd_07.addr[0] = (( CharToHex(&m_meter07_addr[10]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[11]) & 0x0F );//0x99;
	send_cmd_07.addr[1] = (( CharToHex(&m_meter07_addr[8]) & 0x0F ) << 4 ) + (CharToHex(& m_meter07_addr[9]) & 0x0F );//0x99;
	send_cmd_07.addr[2] = (( CharToHex(&m_meter07_addr[6]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[7]) & 0x0F );//0x99;
	send_cmd_07.addr[3] = (( CharToHex(&m_meter07_addr[4]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[5]) & 0x0F );//0x99;
	send_cmd_07.addr[4] = (( CharToHex(&m_meter07_addr[2]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[3]) & 0x0F );//0x99;
	send_cmd_07.addr[5] = (( CharToHex(&m_meter07_addr[0]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[1]) & 0x0F );//0x99;
	send_cmd_07.head2=0x68;
	send_cmd_07.cmd=0x11;
	send_cmd_07.len=0x04;	
	send_cmd_07.biaoshi[0]=m_cmd_07_DI0[offset]+0x33;
	send_cmd_07.biaoshi[1]=m_cmd_07_DI1[offset]+0x33;
	send_cmd_07.biaoshi[2]=m_cmd_07_DI2[offset]+0x33;
	send_cmd_07.biaoshi[3]=m_cmd_07_DI3[offset]+0x33;
	send_cmd_07.crc=global_sum_array((char*)&send_cmd_07.head1,sizeof(send_data_07_t)-5,IS_CHAR);
	send_cmd_07.end=0x16;
	app_RMmod_Usart3_send((char*)&send_cmd_07,sizeof(send_data_07_t));
	
	return;
}
/********************************************************************
 * @���� :���ı�
 * @���� :���Ͷ���ָ��
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_RMmod_send_read_meter_cmd_select(void)
{
	if(m_meter_data.guiyue==0)
	{
		app_RMmod_send_read_meter_cmd_97(m_send_offset);
	}
	else if(m_meter_data.guiyue==1)
	{
		app_RMmod_send_read_meter_cmd_07(m_send_offset);
	}
	return;
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :(�༭�����ûص�����)
 * @���� :NONE
 * @��� :NONE
********************************************************************/
uint8_t app_RMmod_edit_setting( hwin_t*hp )
{
	uint8_t key;
	Bsize_t member;
	
	hand_wirte_input_box_pos_set(20,315);
	key_input_method_filter( KEY_INPUT_123 | KEY_INPUT_ABC | KEY_INPUT_abc );
	app_global_key_control_power_req();
	GUI_window_hwin_req( hp );
	GUI_edit_select(EDIT_M_ADDR);
	GUI_window_hwin_release( hp );
	while ( 1 )
	{
		msleep( TYPE_DELAY );
		if ( app_global_key_force_obtain( &key, 1 ) != 1 )/*�ж������Ƿ��յ�������Ӧ*/
		{
			continue;
		}
		GUI_window_hwin_req( hp );
		GUI_edit_run_hook( key );
		GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
		GUI_set_Fgcolor( RM_FG_COLOR );
		if (( app_global_decide_inputkey( key ) ) != FAIL )/*�ж��Ƿ������뷨��ذ�������*/
		{
			member = GUI_edit_id_obtain();     /*��ȡ��ǰ�༭��ԱID*/
			if ( member == EDIT_M_ADDR)
			{
				if ( m_offset_guiyue == 1 )
					 key_method_input_shadow( key, m_meter07_addr);
				else key_method_input_shadow( key, m_meter_addr);
			}
			else if ( member == EDIT_M_PASSWORD)
			{
				key_method_input_shadow( key, m_meter_Password);
			}
		}
		else if(key == KEY_DOWN)GUI_edit_shift( EDIT_NEXT);	
		else if(key == KEY_UP)	GUI_edit_shift( EDIT_LAST );
		GUI_window_hwin_release( hp );
		if ( key == KEY_ENTER || key == GUI_KEY_ENTER )
		{
			m_meter_Password[8] = ASCII_NULL;
			if ( m_offset_guiyue == 1 )
				 global_memcpy(m_meter_data.meter_Addr,m_meter07_addr,13);
			else global_memcpy(m_meter_data.meter_Addr,m_meter_addr,13);
			app_RMmod_meter_addr_display(m_meter_data.meter_Addr);
			break;
		}
		else if ( key == KEY_EXIT)
		{
			m_meter_Password[8] = ASCII_NULL;
			if ( m_offset_guiyue == 1 )
				 global_memcpy(m_meter_data.meter_Addr,m_meter07_addr,13);
			else global_memcpy(m_meter_data.meter_Addr,m_meter_addr,13);
			app_RMmod_meter_addr_display(m_meter_data.meter_Addr);
			break;
		}
	}
	app_global_key_control_power_release();
	return key;
}

void app_RMmod_dispaly_meter_addr(uint8_t *data)
{
	char meter_addr[15]="";
	GLOBAL_MEMCLR(m_meter_data.meter_Addr, 13);
	sprintf( &meter_addr[0], "%02X%02X%02X%02X%02X%02X", data[6], data[5], data[4], data[3], data[2], data[1] );
	global_memcpy(m_meter_data.meter_Addr,meter_addr,13);
	if ( m_offset_guiyue == 1 )
		 global_memcpy(m_meter07_addr,m_meter_data.meter_Addr,13);
	else global_memcpy(m_meter_addr,m_meter_data.meter_Addr,13);
	app_RMmod_rev_data_display(m_meter_data.meter_Addr,EDIT_M_ADDR);
	return;
}
/********************************************************************
* @���� :���ı�
* @���� :97��Լ���ݷ���
* @���� :*data
* @��� :NONE
********************************************************************/
void app_RMmod_handle_guiyue_97(uint8_t*data)
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
	if (( biaoshi1 == 0x90 ) && ( biaoshi2 == 0x1F ) )//�����й��������ݿ�
	{
		app_RMmod_dispaly_meter_addr(data);
		f_power[0] = f_power[1] + f_power[2] + f_power[3] + f_power[4];
		app_RMmod_rev_data_ftoa_and_display(f_power[0],m_meter_data.zy_power_ALL,EDIT_Z_1);
		app_RMmod_rev_data_ftoa_and_display(f_power[1],m_meter_data.zy_power_J,EDIT_J_1);
		app_RMmod_rev_data_ftoa_and_display(f_power[2],m_meter_data.zy_power_F,EDIT_F_1);
		app_RMmod_rev_data_ftoa_and_display(f_power[3],m_meter_data.zy_power_P,EDIT_P_1);
		app_RMmod_rev_data_ftoa_and_display(f_power[4],m_meter_data.zy_power_G,EDIT_G_1);
	}
	else if (( biaoshi1 == 0x91 ) && ( biaoshi2 == 0x1F ) )//�����޹��������ݿ�
	{
		f_power[0] = f_power[1] + f_power[2] + f_power[3] + f_power[4];
		app_RMmod_rev_data_ftoa_and_display(f_power[0],m_meter_data.zw_power_ALL,EDIT_Z_2);
		app_RMmod_rev_data_ftoa_and_display(f_power[1],m_meter_data.zw_power_J,EDIT_J_2);
		app_RMmod_rev_data_ftoa_and_display(f_power[2],m_meter_data.zw_power_F,EDIT_F_2);
		app_RMmod_rev_data_ftoa_and_display(f_power[3],m_meter_data.zw_power_P,EDIT_P_2);
		app_RMmod_rev_data_ftoa_and_display(f_power[4],m_meter_data.zw_power_G,EDIT_G_2);
	}
	else if (( biaoshi1 == 0x90 ) && ( biaoshi2 == 0x2F ) )//�����й��������ݿ�
	{
		f_power[0] = f_power[1] + f_power[2] + f_power[3] + f_power[4];
		app_RMmod_rev_data_ftoa_and_display(f_power[0],m_meter_data.fy_power_ALL,EDIT_Z_3);
		app_RMmod_rev_data_ftoa_and_display(f_power[1],m_meter_data.fy_power_J,EDIT_J_3);
		app_RMmod_rev_data_ftoa_and_display(f_power[2],m_meter_data.fy_power_F,EDIT_F_3);
		app_RMmod_rev_data_ftoa_and_display(f_power[3],m_meter_data.fy_power_P,EDIT_P_3);
		app_RMmod_rev_data_ftoa_and_display(f_power[4],m_meter_data.fy_power_G,EDIT_G_3);
	}
	else if (( biaoshi1 == 0x91 ) && ( biaoshi2 == 0x2F ) )//�����޹��������ݿ�
	{
		f_power[0] = f_power[1] + f_power[2] + f_power[3] + f_power[4];
		app_RMmod_rev_data_ftoa_and_display(f_power[0],m_meter_data.fw_power_ALL,EDIT_Z_4);
		app_RMmod_rev_data_ftoa_and_display(f_power[1],m_meter_data.fw_power_J,EDIT_J_4);
		app_RMmod_rev_data_ftoa_and_display(f_power[2],m_meter_data.fw_power_F,EDIT_F_4);
		app_RMmod_rev_data_ftoa_and_display(f_power[3],m_meter_data.fw_power_P,EDIT_P_4);
		app_RMmod_rev_data_ftoa_and_display(f_power[4],m_meter_data.fw_power_G,EDIT_G_4);
	}
/****************************��ѹ����*************************************/
	if(biaoshi1 == 0xB6)
	{
		fbs= 1.0;
		f_data = 0;
		if((biaoshi2 & 0xf0)== 0x10)//��ѹ
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
				app_RMmod_rev_data_ftoa_and_display(f_data,m_meter_data.voltage_A,EDIT_A_U);//a
			else if(biaoshi2 == 0x12)
				app_RMmod_rev_data_ftoa_and_display(f_data,m_meter_data.voltage_B,EDIT_B_U);//b
			else if(biaoshi2 == 0x13)
				app_RMmod_rev_data_ftoa_and_display(f_data,m_meter_data.voltage_C,EDIT_C_U);//c
		}
		else if((biaoshi2 & 0xf0)== 0x20)//����
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
				app_RMmod_rev_data_ftoa_and_display(f_data,m_meter_data.current_A,EDIT_A_I);//a
			else if(biaoshi2 == 0x22)
				app_RMmod_rev_data_ftoa_and_display(f_data,m_meter_data.current_B,EDIT_B_I);//b
			else if(biaoshi2 == 0x23)
				app_RMmod_rev_data_ftoa_and_display(f_data,m_meter_data.current_C,EDIT_C_I);//c
		}
	}
/***************************ʧѹ**************************************/
	if(biaoshi1 == 0xB3 && (biaoshi2 &0x20) ==0x20)
	{
		fbs= 1.0;
		f_data = 0;
		for ( offset1= 0;offset1 <3;offset1++ )
	 	{
			s_data = data[12+offset1] - 0x33;
			if ( (s_data & 0xff) == 0xff )	s_data = 0;
			f_data += fbs * (((s_data>> 4 ) & 0x0F ) * 10 + ( s_data & 0x0F ) );
			fbs *= 100.0;
		}
		if(biaoshi2==0x20)
			app_RMmod_rev_data_ftoa_and_display(f_data,m_meter_data.lost_ALL,EDIT_SY_ALL);//��
		else if(biaoshi2==0x21)
			app_RMmod_rev_data_ftoa_and_display(f_data,m_meter_data.lost_A,EDIT_SY_A);//a
		else if(biaoshi2==0x22)
			app_RMmod_rev_data_ftoa_and_display(f_data,m_meter_data.lost_B,EDIT_SY_B);//b
		else if(biaoshi2==0x23)
			app_RMmod_rev_data_ftoa_and_display(f_data,m_meter_data.lost_C,EDIT_SY_C);//c
	}
/***************************��ʱ��**************************************/
	if ( biaoshi1== 0xC0 && biaoshi2 == 0x10 )//����
	{
		m_meter_data.meter_time[8]=' ';
		m_meter_data.meter_time[7]=( (data[13]-0x33) & 0x0f ) + 48 ; //��
		m_meter_data.meter_time[6]=( (data[13]-0x33) >> 4 ) + 48 ;
		m_meter_data.meter_time[5]='-';
		m_meter_data.meter_time[4]=( (data[14]-0x33) & 0x0f ) + 48 ; //��
		m_meter_data.meter_time[3]=( (data[14]-0x33) >> 4 ) + 48 ;
		m_meter_data.meter_time[2]='-';
		m_meter_data.meter_time[1]=( (data[15]-0x33) & 0x0f ) + 48 ; //��
		m_meter_data.meter_time[0]=( (data[15]-0x33) >> 4 ) + 48 ;
		/**************����ʱ�����ڹ㲥Уʱ***************/
		m_meter_time_buf.day=data[13]-0x33;
		m_meter_time_buf.month=data[14]-0x33;
		m_meter_time_buf.year=data[15]-0x33;
	}
	else if ( biaoshi1== 0xC0 && biaoshi2 == 0x11 )//ʱ��
	{
		m_meter_data.meter_time[17]='\0';
		m_meter_data.meter_time[16]=( (data[12]-0x33) & 0x0f ) + 48 ; //��
		m_meter_data.meter_time[15]=( (data[12]-0x33) >> 4 ) + 48 ;
		m_meter_data.meter_time[14]=':';
		m_meter_data.meter_time[13]=( (data[13]-0x33) & 0x0f ) + 48 ; //��
		m_meter_data.meter_time[12]=( (data[13]-0x33) >> 4 ) + 48 ;
		m_meter_data.meter_time[11]=':';
		m_meter_data.meter_time[10]=( (data[14]-0x33) & 0x0f ) + 48 ; //ʱ
		m_meter_data.meter_time[9] =( (data[14]-0x33) >> 4 ) + 48 ;
		app_RMmod_rev_data_display(m_meter_data.meter_time,EDIT_M_TIME);
		/**************����ʱ�����ڹ㲥Уʱ***************/
		m_meter_time_buf.sec=data[12]-0x33;
		m_meter_time_buf.min=data[13]-0x33;
		m_meter_time_buf.hour=data[14]-0x33;
	}
	if(m_send_offset<sizeof(m_cmd_97_DI0))
	{
		//os_sem_post(m_meter_sem);
		g_read_continue = 1;
	}
	else
	{
		m_send_offset=0;
		m_RM_rev_flag=FAIL;
		m_start_flag=FAIL;
		app_RMmod_rev_tip_display("���ݶ�ȡ���!");
		if ( g_broadcast_flag == 1 )
		{
			app_RMmod_rev_tip_display("��ʼ���й㲥Уʱ!");
			app_RMmod_broadcast_CheckTime_handle();
			g_broadcast_flag = 0;
		}
	}
	return;
}
/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :07��Լ���ݷ���
* @���� :*data
* @��� :NONE
********************************************************************/
void app_RMmod_handle_guiyue_07(uint8_t *data)
{

	uint8_t offset1=0,offset2=0;
	uint8_t biaoshi1=0,biaoshi2=0,biaoshi3=0,biaoshi4=0;
	float   f_power[6]={0,}; 
	uint8_t s_data=0;		  //��ȥ��0x33������
	float   f_data=0;  
	float 	fbs;
	uint8_t hex_data 	= 0;
	uint8_t m_temp[4] 	= {0,};
	long    m_P_num[6] 	= {0,};
	
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
				//if ( biaoshi2==0x00 || biaoshi2==0x03 || biaoshi2==0x04 )
				{
					if ( (offset2 == 3) && (s_data & 0x80) == 0x80 )
						s_data &= 0x7f; 
				}
				f_data += fbs * (((s_data>> 4 ) & 0x0F ) * 10 + ( s_data & 0x0F ) );
				fbs *= 100.0;
			}
			f_power[offset1] = f_data * 0.01;
			//if ( biaoshi2==0x00 || biaoshi2==0x03 || biaoshi2==0x04 )
			{//����й����޹����λ�Ƿ���λ,0��1��
				if (((data[14+offset1*4+3] - 0x33) & 0x80) == 0x80 )
					f_power[offset1] = -f_power[offset1];
			}
		}
		if ( (biaoshi2 == 0x01||biaoshi2 == 0x00) && biaoshi1 == 0x00 )//�����й��������ݿ�
		{
			app_RMmod_dispaly_meter_addr(data);
			f_power[0] = f_power[1] + f_power[2] + f_power[3] + f_power[4];
			app_RMmod_rev_data_ftoa_and_display(f_power[0],m_meter_data.zy_power_ALL,EDIT_Z_1);
			app_RMmod_rev_data_ftoa_and_display(f_power[1],m_meter_data.zy_power_J,EDIT_J_1);
			app_RMmod_rev_data_ftoa_and_display(f_power[2],m_meter_data.zy_power_F,EDIT_F_1);
			app_RMmod_rev_data_ftoa_and_display(f_power[3],m_meter_data.zy_power_P,EDIT_P_1);
			app_RMmod_rev_data_ftoa_and_display(f_power[4],m_meter_data.zy_power_G,EDIT_G_1);
		}
		else if ( biaoshi2 == 0x03 && biaoshi1 == 0x00)//�����޹��������ݿ�
		{
			f_power[0] = f_power[1] + f_power[2] + f_power[3] + f_power[4];
			app_RMmod_rev_data_ftoa_and_display(f_power[0],m_meter_data.zw_power_ALL,EDIT_Z_2);
			app_RMmod_rev_data_ftoa_and_display(f_power[1],m_meter_data.zw_power_J,EDIT_J_2);
			app_RMmod_rev_data_ftoa_and_display(f_power[2],m_meter_data.zw_power_F,EDIT_F_2);
			app_RMmod_rev_data_ftoa_and_display(f_power[3],m_meter_data.zw_power_P,EDIT_P_2);
			app_RMmod_rev_data_ftoa_and_display(f_power[4],m_meter_data.zw_power_G,EDIT_G_2);
		}
		else if ( biaoshi2 == 0x02 && biaoshi1 == 0x00)//�����й��������ݿ�
		{
			f_power[0] = f_power[1] + f_power[2] + f_power[3] + f_power[4];
			app_RMmod_rev_data_ftoa_and_display(f_power[0],m_meter_data.fy_power_ALL,EDIT_Z_3);
			app_RMmod_rev_data_ftoa_and_display(f_power[1],m_meter_data.fy_power_J,EDIT_J_3);
			app_RMmod_rev_data_ftoa_and_display(f_power[2],m_meter_data.fy_power_F,EDIT_F_3);
			app_RMmod_rev_data_ftoa_and_display(f_power[3],m_meter_data.fy_power_P,EDIT_P_3);
			app_RMmod_rev_data_ftoa_and_display(f_power[4],m_meter_data.fy_power_G,EDIT_G_3);
		}
		else if ( biaoshi2 == 0x04 && biaoshi1 == 0x00)//�����޹��������ݿ�
		{
			f_power[0] = f_power[1] + f_power[2] + f_power[3] + f_power[4];
			app_RMmod_rev_data_ftoa_and_display(f_power[0],m_meter_data.fw_power_ALL,EDIT_Z_4);
			app_RMmod_rev_data_ftoa_and_display(f_power[1],m_meter_data.fw_power_J,EDIT_J_4);
			app_RMmod_rev_data_ftoa_and_display(f_power[2],m_meter_data.fw_power_F,EDIT_F_4);
			app_RMmod_rev_data_ftoa_and_display(f_power[3],m_meter_data.fw_power_P,EDIT_P_4);
			app_RMmod_rev_data_ftoa_and_display(f_power[4],m_meter_data.fw_power_G,EDIT_G_4);
		}
	}
/****************************��ѹ����*************************************/
	if((biaoshi1 == 0x02 && biaoshi4==0x00 && biaoshi3 ==0xff)
		&&(biaoshi2 == 0x01 || biaoshi2 ==0x02))
	{
		fbs= 1.0;
		f_data = 0;
		if( biaoshi2 == 0x01 && biaoshi1 == 0x02)//��ѹ
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
			if ( f_power[0] > 1000 )	 f_power[0] = 0;
			if ( f_power[1] > 1000 )	 f_power[1] = 0;
			if ( f_power[2] > 1000 )	 f_power[2] = 0;
			app_RMmod_rev_data_ftoa_and_display(f_power[0],m_meter_data.voltage_A,EDIT_A_U);//a
			app_RMmod_rev_data_ftoa_and_display(f_power[1],m_meter_data.voltage_B,EDIT_B_U);//b
			app_RMmod_rev_data_ftoa_and_display(f_power[2],m_meter_data.voltage_C,EDIT_C_U);//c
		}
		else if(biaoshi2 == 0x02 && biaoshi1 == 0x02)//����
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
			app_RMmod_rev_data_ftoa_and_display(f_power[0],m_meter_data.current_A,EDIT_A_I);//a
			app_RMmod_rev_data_ftoa_and_display(f_power[1],m_meter_data.current_B,EDIT_B_I);//b
			app_RMmod_rev_data_ftoa_and_display(f_power[2],m_meter_data.current_C,EDIT_C_I);//c
		}
	}
/***************************ʧѹʱ��**************************************/
	if(biaoshi1 == 0x03 && biaoshi2 ==0x01 && biaoshi3 ==0x00&& biaoshi4 ==0x00)
	{

		for ( offset1 = 0;offset1 < 6;offset1++ )
		{
			for (offset2 = 0;offset2 < 3;offset2++ )
			{
				s_data = data[14+offset1*3+offset2] - 0x33;
				if ( (s_data & 0xff) == 0xff )	s_data = 0;
				bcd_convert_hex(s_data,&hex_data);
				m_temp[offset2] = hex_data;
			}
			m_P_num[offset1] = m_temp[0]+((long)m_temp[1]*100)+((long)m_temp[2]*10000);
		}
		app_RMmod_rev_data_ftoa_and_display(m_P_num[1],m_meter_data.lost_A,EDIT_SY_A);//a
		app_RMmod_rev_data_ftoa_and_display(m_P_num[3],m_meter_data.lost_B,EDIT_SY_B);//b
		app_RMmod_rev_data_ftoa_and_display(m_P_num[5],m_meter_data.lost_C,EDIT_SY_C);//c
	}
	else if(biaoshi1 == 0x03 && biaoshi2 ==0x05 && biaoshi3 ==0x00&& biaoshi4 ==0x00)
	{

		for ( offset1 = 0;offset1 < 2;offset1++ )
		{
			for (offset2 = 0;offset2 < 3;offset2++ )
			{
				s_data = data[14+offset1*3+offset2] - 0x33;
				if ( (s_data & 0xff) == 0xff )	s_data = 0;
				bcd_convert_hex(s_data,&hex_data);
				m_temp[offset2] = hex_data;
			}
			m_P_num[offset1] = m_temp[0]+((long)m_temp[1]*100)+((long)m_temp[2]*10000);
		}
		app_RMmod_rev_data_ftoa_and_display(m_P_num[1],m_meter_data.lost_ALL,EDIT_SY_ALL);//��
	}

/***************************��ʱ��**************************************/
	if((biaoshi1==0x04&& biaoshi2==0x00&& biaoshi3==0x01)&&(biaoshi4==0x01||biaoshi4 == 0x02 ))//����
	{
		if(biaoshi4==0x01)//����
		{
			m_meter_data.meter_time[8]=' ';
			m_meter_data.meter_time[7]=( (data[15]-0x33) & 0x0f ) + 48 ; //��
			m_meter_data.meter_time[6]=( (data[15]-0x33) >> 4 ) + 48 ;
			m_meter_data.meter_time[5]='-';
			m_meter_data.meter_time[4]=( (data[16]-0x33) & 0x0f ) + 48 ; //��
			m_meter_data.meter_time[3]=( (data[16]-0x33) >> 4 ) + 48 ;
			m_meter_data.meter_time[2]='-';
			m_meter_data.meter_time[1]=( (data[17]-0x33) & 0x0f ) + 48 ; //��
			m_meter_data.meter_time[0]=( (data[17]-0x33) >> 4 ) + 48 ;
			/**************����ʱ�����ڹ㲥Уʱ***************/
			m_meter_time_buf.day=data[15]-0x33;
			m_meter_time_buf.month=data[16]-0x33;
			m_meter_time_buf.year=data[17]-0x33;
		}
		else if(biaoshi4 == 0x02 )//ʱ��
		{
			m_meter_data.meter_time[17]='\0';
			m_meter_data.meter_time[16]=( (data[14]-0x33) & 0x0f ) + 48 ; //��
			m_meter_data.meter_time[15]=( (data[14]-0x33) >> 4 ) + 48 ;
			m_meter_data.meter_time[14]=':';
			m_meter_data.meter_time[13]=( (data[15]-0x33) & 0x0f ) + 48 ; //��
			m_meter_data.meter_time[12]=( (data[15]-0x33) >> 4 ) + 48 ;
			m_meter_data.meter_time[11]=':';
			m_meter_data.meter_time[10]=( (data[16]-0x33) & 0x0f ) + 48 ; //ʱ
			m_meter_data.meter_time[9] =( (data[16]-0x33) >> 4 ) + 48 ;
			app_RMmod_rev_data_display(m_meter_data.meter_time,EDIT_M_TIME);
			/**************����ʱ�����ڹ㲥Уʱ***************/
			m_meter_time_buf.sec=data[14]-0x33;
			m_meter_time_buf.min=data[15]-0x33;
			m_meter_time_buf.hour=data[16]-0x33;
		}
	}
	if(m_send_offset<sizeof(m_cmd_07_DI0))
	{
		//os_sem_post(m_meter_sem);
		g_read_continue = 1;
	}
	else
	{
		m_send_offset=0;
		m_RM_rev_flag=FAIL;
		m_start_flag=FAIL;
		app_RMmod_rev_tip_display("���ݶ�ȡ���!");
		if ( g_broadcast_flag == 1 )
		{
			app_RMmod_rev_tip_display("��ʼ���й㲥Уʱ!");
			app_RMmod_broadcast_CheckTime_handle();
			g_broadcast_flag = 0;
		}
	}
	return;
}
/********************************************************************
* @���� :���ı�
* @���� :����Ӧ�������ݷ���
* @���� :void
* @��� :NONE
********************************************************************/
void app_RMmod_auto_read_data_send(void)
{
	if(m_start_flag==TURE)
	{
		if(m_RM_rev_flag==TURE)
		{
			if( g_read_continue == 1 )
			{
				g_read_continue = 0;
				app_RMmod_send_read_meter_cmd_select();
			}
		}
		else if(m_RM_rev_flag==FAIL)
		{	
			if(m_baud_auto_flag==TURE || m_CRC_auto_flag==TURE)
			{
				app_RMmod_rev_tip_display("����Ӧ��,���Ժ�...");
			}
			else
			{
				app_RMmod_rev_tip_display("���ڶ�ȡ����,���Ժ�...");
				m_read_cnt++;
				if(m_read_cnt>20)
				{
					m_read_cnt=0;
					m_start_flag=FAIL;
					app_RMmod_rev_tip_display("��ȡ����ʧ�ܡ�");
					g_broadcast_flag = 0;
				}
			}
			if(m_baud_auto_flag==TURE && m_CRC_auto_flag==TURE)
			{
				if(m_baud_add_flag==TURE)
				{
					m_offset_baud++;
					m_baud_add_flag=FAIL;
					if(m_offset_baud==7)
					{
						m_offset_baud=1;
					}
					app_RMmod_baud_select(m_offset_baud);
				}
				m_offset_crc++;
				if(m_offset_crc==4)
				{
					m_baud_add_flag=TURE;
					m_offset_crc=1;	
					if(m_offset_baud==6)
					{
						m_offset_baud=1;
						m_start_flag=FAIL;
						app_RMmod_baud_select(m_offset_baud);
						app_RMmod_CRC_select(m_offset_crc,0);
						app_RMmod_rev_tip_display("����Ӧ���,δ��ͨѶ��.");
						m_offset_crc = 0;
						m_offset_baud = 0;
						g_broadcast_flag = 0;
						return;
					}
				}
				app_RMmod_CRC_select(m_offset_crc,0);
			}			
			else if(m_baud_auto_flag==TURE && m_CRC_auto_flag==FAIL)
			{
				m_offset_baud++;
				m_baud_add_flag=FAIL;
				if(m_offset_baud==7)
				{
					m_offset_baud=1;
					m_start_flag=FAIL;
					app_RMmod_baud_select(m_offset_baud);
					app_RMmod_rev_tip_display("����Ӧ���,δ��ͨѶ��.");
					m_offset_baud = 0;
					g_broadcast_flag = 0;
					return;
				}
				app_RMmod_baud_select(m_offset_baud);
			}
			else if(m_baud_auto_flag==FAIL && m_CRC_auto_flag==TURE)
			{
				m_offset_crc++;
				if(m_offset_crc==4)
				{
					m_baud_add_flag=TURE;
					m_offset_crc=1;	
					m_start_flag=FAIL;
					app_RMmod_CRC_select(m_offset_crc,0);
					app_RMmod_rev_tip_display("����Ӧ���,δ��ͨѶ��.");
					m_offset_crc = 0;
					g_broadcast_flag = 0;
					return;
				}
				app_RMmod_CRC_select(m_offset_crc,0);
			}
			msleep(1);
			app_RMmod_send_read_meter_cmd_select();
		}
	}
	return;
}

/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :meterģ���������
* @���� :NONE
* @��� :NONE
********************************************************************/
static void _task2(void*arg)
{
	Bsize_t size;
	Bsize_t Fsize = 5;
	uint8_t rev_data[100];
	int     m_cnt = 0;
	uint8_t m_error = 0;
	GLOBAL_MEMCLR(rev_data, 100);
	while(1)
	{
		msleep(TYPE_DELAY);
		if ( (g_guiye_DLMS_flag == 1) && (m_start_flag == TURE) )
		{
			GLOBAL_MEMCLR(m_meter_data.meter_Addr, sizeof(char)*13);
			app_RMmod_meter_addr_display(m_meter_data.meter_Addr);
			app_RM_readout_to_meterARG();
			m_start_flag = 0;
			os_driver_ioctl(g_usart3_dev_id, SET_EMPTY_FIFO, 0);
		}
		else
		{
			if(m_start_flag==TURE)
			{
				if( g_read_continue == 0 && g_moon_CheckTime_finish == 0 )
				{
					m_cnt++;
					if ( m_cnt > 15 )
					{
						m_cnt = 0;
						app_global_key_dummy_press(GUI_KEY_READ_M);
					}
				}
				else m_cnt = 0;
			}
			else
			{
				m_cnt = 0;
				continue;
			}
			if ( g_moon_CheckTime_finish == 0 )
			{
				if(m_RM_rev_flag==FAIL)
				{
					app_RMmod_auto_read_data_send();
				}
				else if(m_RM_rev_flag==TURE)
				{
					app_RMmod_auto_read_data_send();
				}
			}
			msleep(10);
			
			while ( Fsize )
			{
				size = os_driver_read( g_usart3_dev_id, &rev_data[0], 1, 0 );
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
				if ( g_moon_CheckTime_finish == 1 )
				{
					app_RMmod_rev_tip_display("����ܱ�ͨѶ����.");
					m_start_flag = 0;
				}
				continue;
			}
			
			if ( m_offset_baud == 0 )	   msleep(80);	//����Ӧ
			else if ( m_offset_baud == 1 ) msleep(90);	//300
			else if ( m_offset_baud == 2 ) msleep(55);	//600
			else if ( m_offset_baud == 3 ) msleep(40);	//1200
			else if ( m_offset_baud == 4 ) msleep(20);	//2400
			else if ( m_offset_baud == 5 ) msleep(15);	//4800
			else if ( m_offset_baud == 6 ) msleep(10);	//9600
			else if ( m_offset_baud == 7 ) msleep(6);	//19200
			
			size = os_driver_read( g_usart3_dev_id, &rev_data[1], 7, 0 );
			if ( size != 7 || ( rev_data[7] != 0x68 ) )  //�յڶ���68H��
			{
				continue;
			}
			size = os_driver_read( g_usart3_dev_id, &rev_data[8], 2, 0 );
			if ( size != 2 )	  						//�տ�����ͳ���,8Ϊ������,9����;
			{
				continue;
			}
			size = os_driver_read( g_usart3_dev_id, &rev_data[10], rev_data[9], 0 );		
			size = os_driver_read( g_usart3_dev_id, &rev_data[10+rev_data[9]], 2, 0 );		
			if(size==2&&rev_data[10+rev_data[9]+1]==0x16)
			{
				if ( rev_data[9] == 0 )//��վ����Ӧ��
				{
					g_moon_CheckTime_cnt++;
					if( g_moon_CheckTime_cnt == 2 )
					{
						if ( g_CheckTime_flag == 0 )
						{
							app_RMmod_rev_tip_display("���ܱ�������ʱ�ɹ�.");
							GUI_3D_window_title_mod(m_RMwin_pWIN,"���ܱ�������ʱ�ɹ�.");
						}
						else if ( g_CheckTime_flag == 1 )
						{
							app_RMmod_rev_tip_display("���ܱ�������ʱ�ɹ�.");
							GUI_3D_window_title_mod(m_RMwin_pWIN,"���ܱ�������ʱ�ɹ�.");
						}
						m_start_flag = 0;
						continue;
					}
					if(m_meter_data.guiyue==0)//97
					{
						app_RMmod_time_ymdw_97_set();
						GUI_3D_window_title_mod(m_RMwin_pWIN,"ʱ�������óɹ�,��������������.");
					}
					else if(m_meter_data.guiyue==1)// 07
					{	
						app_RMmod_time_ymdw_07_set();
						GUI_3D_window_title_mod(m_RMwin_pWIN,"ʱ�������óɹ�,��������������.");
					}
					continue;
				}
				else if ( rev_data[9] == 1 )//��վ�쳣Ӧ��
				{
					m_error = rev_data[10] - 0x33;
					if ( rev_data[8] == 0xD4 || rev_data[8] == 0xC4)//д���վ�쳣Ӧ��
					{
						if ( (m_error & 0x04) == 0x04 )
							 GUI_3D_window_title_mod(m_RMwin_pWIN,"������δ��Ȩ,Уʱʧ��!");
						else 
							 GUI_3D_window_title_mod(m_RMwin_pWIN,"�쳣Ӧ����������,Уʱʧ��!");
						m_start_flag = 0;
						continue;
					}
				}
				m_send_offset++;
				m_RM_rev_flag=TURE;  //�յ����ݱ�־
				app_RMmod_baud_display(m_offset_baud);
				app_RMmod_CRC_display(m_offset_crc);
				m_baud_auto_flag=FAIL;
				m_CRC_auto_flag=FAIL;
				m_meter_data.baud=m_offset_baud;
				m_meter_data.crc=m_offset_crc;
				app_RMmod_rev_tip_display("���ڶ�ȡ����,���Ժ�...");
			}
			if(m_meter_data.guiyue==0) //97
			{
				app_RMmod_handle_guiyue_97(rev_data);
			}
			else if(m_meter_data.guiyue==1)//07
			{
				app_RMmod_handle_guiyue_07(rev_data);
			}
		}
	}
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :��Լ���ò���
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_RM_guiyue_set( void )
{
	m_meter_data.guiyue = m_offset_guiyue;			
	if(m_meter_data.guiyue == 0)//97
		global_memset(m_meter_addr, '9', 12);
	else if(m_meter_data.guiyue ==1)//07
		global_memset(m_meter07_addr, 'A', 12);
	
	if ( m_offset_guiyue == 2 )		g_guiye_DLMS_flag = 1;
	else 
	{
		g_guiye_DLMS_flag = 0;
	}
	if ( m_offset_guiyue == 1 )
		 global_memcpy(m_meter_data.meter_Addr,m_meter07_addr,13);
	else global_memcpy(m_meter_data.meter_Addr,m_meter_addr,13);
	app_RMmod_meter_addr_display(m_meter_data.meter_Addr);
	return;
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :���������ò���
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_RM_baud_set( void )
{
	m_meter_data.baud = m_offset_baud;
	return;
}
/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :У�鷽ʽ���ò���
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_RM_crc_set( void )
{
	m_meter_data.crc = m_offset_crc;
	return;
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :ģ��������
 * @���� :NONE
 * @��� :NONE
********************************************************************/
static void _task(void*arg)
{
	uint8_t* key_buf = 0;
	int		cnt = 59;
	uint8_t err;
	
	app_RMmod_window_create(0);	
	app_RMmod_set_info_display();
	GUI_WINready(m_meter_droplist_guiyue,m_offset_guiyue);
	while(1)
	{
		msleep(TYPE_DELAY);
		
		if ( ++cnt%60 == 0 )
		{
			app_RMmod_notice_window_display();
		}
		mood_check_time_handle();
		
		key_buf = (uint8_t*)os_q_pend(
					  m_RMmod_keymsg_qevent,
					  APP_100_MS, &err );                       
		if ( err != OS_NO_ERR )
		continue;
		if( *key_buf == KEY_SETTING || *key_buf == GUI_KEY_SETTING || *key_buf == GUI_KEY_EDIT_START)
		{
			if ( GUI_edit_start( m_RMwin_edit,app_RMmod_edit_setting ) /*�����ؼ��༭*/
				 == KEY_ENTER )
			{
				m_read_cnt=0;
				GLOBAL_MEMCLR(&m_meter_data.zy_power_ALL,sizeof(RMeter_dl_info_t)-16);
				app_RMmod_clean_display();
			}
		}
		else if( *key_buf == KEY_F1 || *key_buf == GUI_KEY_READ_M )
		{
			if(m_offset_baud!=0)
			{
				m_baud_auto_flag=FAIL;			
				app_RMmod_baud_select(m_offset_baud); 	//���ò�����
				GUI_drop_list_member_assign(m_meter_droplist_baud,m_offset_baud);
			}
			else 
			{
				m_baud_auto_flag=TURE;
			}
			if(m_offset_crc!=0)
			{
				m_CRC_auto_flag=FAIL; 
				app_RMmod_CRC_select(m_offset_crc,0);		//����У�鷽ʽ
				GUI_drop_list_member_assign(m_meter_droplist_crc,m_offset_crc);
			}
			else
			{
				m_CRC_auto_flag=TURE;
			}
			
			m_start_flag=TURE;
			g_read_continue = 1;
			m_read_cnt=0;
			m_send_offset=0;
			g_moon_CheckTime_cnt = 0;
			g_moon_CheckTime_finish = 0;
			GLOBAL_MEMCLR(&m_meter_data.zy_power_ALL,sizeof(RMeter_dl_info_t)-16);
			app_RMmod_clean_display();
			if ( g_broadcast_flag == 1 )
			GUI_3D_window_title_mod(m_RMwin_pWIN,"����ܱ�ͨѶ��ɺ��Զ����й㲥Уʱ.");
		}
		else if( *key_buf == GUI_KEY_GUIYUE_SELECT)
		{	
			m_offset_guiyue = GUI_drop_list_unfold(m_meter_droplist_guiyue);
			app_RM_guiyue_set();
		}
		else if( *key_buf == GUI_KEY_BAUD_SELECT)
		{	
			m_offset_baud = GUI_drop_list_unfold(m_meter_droplist_baud);
			app_RM_baud_set();
		}
		else if( *key_buf == GUI_KEY_CRC_SELECT )
		{	
			m_offset_crc = GUI_drop_list_unfold(m_meter_droplist_crc);
			app_RM_crc_set();
		}
		else if(*key_buf == KEY_F2 || *key_buf == GUI_KEY_METER_BROADCAST_JIAOSHI)//�㲥��ʱ
		{
			if ( g_guiye_DLMS_flag == 0 )
			{
				if (MsgBox(200,200,230,240,"��ʾ!","���������͵��ܱ�ʱ��������5�������ڹ㲥Уʱ����Ч,һ��ֻ����һ��,��������ҹ0ʱУʱ.��<ȷ��>����Уʱ.",APP_HALF_SEC*30) == GUIE_KEY_ENTER )
				{
					g_broadcast_flag = 1;
					g_moon_CheckTime_flag = 0;
					app_global_key_dummy_press(GUI_KEY_READ_M);
				}
			}
			else MsgBox(200,200,200,200,"��ʾ!","�㲥Уʱֻ��DL/T 645��Լ����Ч!",APP_HALF_SEC*10);
		}
		else if(*key_buf == KEY_F3 || *key_buf == GUI_KEY_METER_MOON_JIAOSHI)//������ʱ
		{
			if ( g_guiye_DLMS_flag == 0 )
			{
				if(MsgBox(200,200,230,240,"��ʾ!","��ȷ�������Ƿ�߱�GPSģ��.����<ȷ��>��������Уʱ,����<�˳�>��������Уʱ.",APP_HALF_SEC*60) == GUIE_KEY_ENTER )
				{
					g_CheckTime_flag = 0;
					if(MsgBox(200,200,230,240,"��ʾ!","����Уʱ�����������ȡ����ʱ��,Уʱ֮ǰ��֤����ܱ�ͨѶ������Ȩ����������.��<ȷ��>����Уʱ.",APP_HALF_SEC*30) == GUIE_KEY_ENTER )
					{
						g_moon_CheckTime_flag 	= 	1;
						g_broadcast_flag 		= 	0;
						g_moon_CheckTime_finish =	0;
						g_moon_CheckTime_cnt	= 	0;
					}
				}
				else
				{
					g_CheckTime_flag = 1;
					if(MsgBox(200,200,230,240,"��ʾ!","����Уʱǿ�Ƶ��ܱ�ʱ�������ʱ��ͬ��,Уʱ֮ǰ��֤����ܱ�ͨѶ������Ȩ����������.��<ȷ��>����Уʱ.",APP_HALF_SEC*30) == GUIE_KEY_ENTER )
					{
						if(m_meter_data.guiyue==0) //97
							app_RMmod_time_hms_97_set();
						else if(m_meter_data.guiyue==1)//07
							app_RMmod_time_hms_07_set();
						g_moon_CheckTime_finish = 1;
						m_start_flag = 1;
					}
				}
			}
			else MsgBox(200,200,200,200,"��ʾ!","����Уʱֻ��DL/T 645��Լ����Ч!",APP_HALF_SEC*10);
		}
		else if ( *key_buf == KEY_RIGHT )
		{
			if(m_CURwidget != m_meter_droplist_crc
				|| m_pMember != m_CURwidget->subord_end )
		  	GUI_WINshift(0);
		}
		else if ( *key_buf == KEY_LEFT )
		{
			if(m_CURwidget!= m_meter_droplist_guiyue
					||m_pMember!=m_CURwidget->subord_head)
		  	GUI_WINshift(1);
		}			 
		else if ( *key_buf == KEY_ENTER )
		{
			GUI_WINdo();
			m_offset_guiyue = GUI_drop_list_member_obtain(m_meter_droplist_guiyue);
			app_RM_guiyue_set();
			m_offset_baud 	= GUI_drop_list_member_obtain(m_meter_droplist_baud);
			app_RM_baud_set();
			m_offset_crc 	= GUI_drop_list_member_obtain(m_meter_droplist_crc);
			app_RM_crc_set();
		}
	}
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :����У�����������ͨѶ����
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_RM_verify_funtion1( void )
{
	uint8_t key_buf;
	
	_task_id
		= os_task_assign_stk_build( _task, 3);
	_task2_id 
		= os_task_assign_stk_build( _task2,5);
	
	g_broadcast_flag 		=	0;
	g_moon_CheckTime_flag 	=	0;
	m_start_flag 			= 	0;
	g_moon_CheckTime_finish =	0;
	g_moon_CheckTime_cnt	= 	0;
	g_CheckTime_flag		=	0;
	while (1)
	{
		msleep( TYPE_DELAY );
		if ( app_global_key_obtain( &key_buf, 1 ) != 1 )  //��������ȡ����ֵ
		{
			continue;
		}
		if ( key_buf == KEY_EXIT || key_buf == GUI_KEY_EXIT)
		{
			app_RMmod_window_destory();
			if ( _task_id != INVALID_PRO )
			{
				os_task_delete( _task_id );
				_task_id = INVALID_PRO;
			}
			if ( _task2_id != INVALID_PRO )
			{
				os_task_delete( _task2_id );
				_task2_id = INVALID_PRO;
			}
			break;
		}
		else if (key_buf==KEY_SAVE)
		{
			if(app_USERinfo_Save()==KEY_ENTER)
         	{
				_Save_file();
		   		fatfs_writeARG("USERDATA",&g_user_arg,sizeof(DL_app_user_all_info_t));
         	}
		}
		else if (key_buf==KEY_SEARCH)
		{
			app_RMmod_window_destory();
			if ( _task_id != INVALID_PRO )
			{
				os_task_delete( _task_id );
				_task_id = INVALID_PRO;
			}
			if ( _task2_id != INVALID_PRO )
			{
				os_task_delete( _task2_id );
				_task2_id = INVALID_PRO;
			}
			app_module_assignExt( "SEARCH" );
		}
		else 
		{
			os_q_post(m_RMmod_keymsg_qevent,(void *)&key_buf);
		}
	}
	return;
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :��ģ��������
 * @���� :NONE
 * @��� :NONE
********************************************************************/
//void app_upload_main( void );
static void _main_task(void*arg)
{
	uint8_t key_buf;
	int cnt = 60;
	DL_flag_t DL_flag;
	uint8_t m_flag = 0;
	app_RMmod_main_window_create();
	fatfs_readARG("USERDATA",&g_user_arg,sizeof(DL_app_user_all_info_t));
    app_global_set_user_dl_arg(&g_user_arg);
	while ( 1 )
	{
		msleep( TYPE_DELAY );
		if( ++cnt%80==0 )
		{
			cnt= 0;
		    app_global_DLarg_obtain(&g_DL_param);
		    app_global_DLflag_obtain(&DL_flag);
			if( m_flag == 0 )
			{
				app_RM_DLarg_display(&g_DL_param);
			}
		}
		if ( app_global_key_obtain( &key_buf, 1 ) == 1 ) 	/*��ȡ��������ֵ*/
		{
			if ( key_buf == KEY_EXIT||key_buf==GUI_KEY_EXIT)
			{
				//app_module_assignExt("START");
				asm( "  lb 0x300000" );
			}
			else if ( key_buf == KEY_1 )
			{
				m_flag = 1;
				app_RMmod_main_window_destory();
				app_RM_verify_funtion1();
				app_RMmod_main_window_create();
				m_flag = 0;
				cnt = 60;
			}
			else if ( key_buf == KEY_2 )
			{
				//MsgBox(200,200,200,200,
				//"��ʾ!","���޴˹���,������������ϵ����.",APP_HALF_SEC*10);
				if ( (m_meter_data.guiyue==1) || (m_meter_data.guiyue==0))
				{
					m_flag = 1;
					app_RMmod_main_window_destory();
					app_RM_verify_funtion2();
					app_RMmod_main_window_create();
					m_flag = 0;
					cnt = 60;
				}
				else 
				MsgBox(200,200,200,200,
				"��ʾ!","�˹���ֻ�����ڹ���DL/T 645 ��Լ��.",APP_HALF_SEC*10);
			}
			else if ( key_buf == KEY_3 )
			{
				//MsgBox(200,200,200,200,
				//"��ʾ!","���޴˹���,������������ϵ����.",APP_HALF_SEC*10);
				if ( (m_meter_data.guiyue==1) || (m_meter_data.guiyue==0))
				{
					m_flag = 1;
					app_RMmod_main_window_destory();
					app_RM_verify_funtion3();
					app_RMmod_main_window_create();
					m_flag = 0;
					cnt = 60;
				}
				else 
				MsgBox(200,200,200,200,
				"��ʾ!","�˹���ֻ�����ڹ���DL/T 645 ��Լ��.",APP_HALF_SEC*10);
			}
			else if ( key_buf == KEY_4 )
			{
				//MsgBox(200,200,200,200,
				//"��ʾ!","���޴˹���,������������ϵ����.",APP_HALF_SEC*10);
				if ( (m_meter_data.guiyue==1) || (m_meter_data.guiyue==0))
				{
					m_flag = 1;
					app_RMmod_main_window_destory();
					app_RM_verify_funtion4();
					//app_upload_main();//����
					app_RMmod_main_window_create();
					m_flag = 0;
					cnt = 60;
				}
				else 
				MsgBox(200,200,200,200,
				"��ʾ!","�˹���ֻ�����ڹ���DL/T 645 ��Լ��.",APP_HALF_SEC*10);
			}
			/*else if ( key_buf == KEY_SAVE )
			{
				if ( MsgBox(200,150,200,200,"��ʾ!","�Ƿ�洢��ǰ��������?��<ȷ��>�洢",APP_HALF_SEC*10)
					== GUIE_KEY_ENTER)
				{
					MsgBox(200,150,200,200,
				"��ʾ!","�洢��ǰ���ݳɹ�!",APP_HALF_SEC*5);
				}
			}*/
		}
	}
}

/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :��ģ�������ܳ�ʼ��
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_RMmod_fun_entry( void )
{
	_main_task_id
		= os_task_assign_stk_build( _main_task, 5);
	GLOBAL_MEMCLR(&m_meter_data, sizeof(RMeter_dl_info_t));
	GLOBAL_MEMCLR(&m_meter_Password[0], sizeof(char)*9);
	g_usart3_dev_id = os_driver_open( "D232" );
	g_timer_dev_id  = os_driver_open( "TIME" );
	g_gps_dev_id    = os_driver_open( "GPS" );
	g_sht11_dev_id  = os_driver_open( "SHT11" );
	m_start_flag = 0;
	g_guiye_DLMS_flag = 0;
	app_global_DL_measure_entery();
	os_q_creat( &m_RMmod_keymsg_qevent,
				&m_RMmod_keymsg_buf[0],
				MAX_HMSG_SIZE );
	return;
}
/********************************************************************
 * @���� :�ܱ�Ȩ
 * @���� :��ģ���������ͷ���Դ
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_RMmod_fun_exit( void )
{
	app_RMmod_main_window_destory();
	
	if ( _main_task_id != INVALID_PRO )
	{
		os_task_delete( _main_task_id );
		_main_task_id = INVALID_PRO;
	}
	os_driver_close( g_usart3_dev_id );
	os_driver_close( g_timer_dev_id );
	app_global_DL_measure_exit();
	return;
}


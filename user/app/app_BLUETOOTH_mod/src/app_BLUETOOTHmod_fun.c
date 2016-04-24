/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:��ѯģ���������
* ����ʱ��:
* ������˾ :����
* �ļ�����:app_UARTmod_fun.c
* ���� :
* ��������:ʵ���û�ģ�鹦��
******************************************************************************************/

#include "../../../includes.h"
#include "app_BLUETOOTHmod_inside.h"

//#include "../../..app_global.h"
//#include "global.h"


#define RECV_BUF_SIZE 			  256


static	uint8_t		BT_main_task_pro 	= INVALID_PRO;
static 	dev_id_t 	_BT_dev_id 	= INVALID_DEV_ID;

uint8_t		BT_current_meter_number  		= 0;
uint16_t	BT_total_record_counts   		= 0;
uint32_t 	BT_total_JB_items_uploaded 	= 0;
uint32_t	BT_current_JB_item_num		= 0;



/*�豸��Ϣ�ϴ�����Ϣǰ׺*/
const char *BTmod_dev_info_prefix_tbl[]=
{
	"ID=",	"ADDR=",	"MODEL=",	"DSP=",	"FPGA="
};


/********************************************************************
* @������ :������
* @���� : �̶����ڴ��������ַ����������ַ������ڴ�ĵ�ַ
* @���� : ��ַ��ַ���������ַ����������ڴ��ֽ���
* @��� : ��
********************************************************************/
char* app_BLUETOOTHmod_search_string(
			const char *pstart,	
			const char *psearch,	
			const uint16_t size
)
{
	const char *pend = PNULL;
	char *pnext = (char *)pstart;
	uint16_t search_len = 0;
	
	search_len = global_strlen((char *)psearch);
	pend = pstart + size - search_len;
	
	while(pnext < pend)
	{
		if( global_memcmp(pnext,(char *)psearch, search_len))
		{
			return pnext;
		}
		else
		{
			pnext++;
		}
	}
	return PNULL;
}

/********************************************************************
* @������ :������
* @���� : ����ģ256��У��,
* @���� : ���ݻ�������ַ����У���������
* @��� : У����
********************************************************************/
uint8_t app_BLUETOOTHmod_cal_mod256_sum(const uint8_t *buf,const uint16_t size)
{
	uint16_t count = 0;
	uint8_t  m_mod256_sum = 0;
	for(count = 0; count < size ; count++)
	{
		m_mod256_sum += buf[count];
	}
	m_mod256_sum = m_mod256_sum%256;
	return m_mod256_sum;
}





/********************************************************************
* @������ :������
* @���� : �������ݰ�512�ֽڴ�С�иÿ�����ֻ����512�ֽڣ�
* @���� : ���ڽ��ջ���
* @��� : 
********************************************************************/
void app_BLUETOOTHmod_uart_send_by512(const uint8_t* psrc, const uint16_t size)
{
	uint8_t 	i = 0;
	uint8_t		send_time = 0;
	uint16_t	remainder = 0;
	
	//��ΪPIC32���ջ������Ϊ1K��������Ƶ��������512bytes
	send_time = size / MAX_UART_SEND_SIZE;
	remainder = size % MAX_UART_SEND_SIZE;
	
	if(send_time > 0)
	{
		for(i = 0; i < send_time; i++)
		{
			os_driver_write(_BT_dev_id,(uint8_t*)psrc + i*MAX_UART_SEND_SIZE,MAX_UART_SEND_SIZE, 0);
		}
	}
	if(remainder > 0)
	{
		os_driver_write(_BT_dev_id,(uint8_t*)psrc + i*MAX_UART_SEND_SIZE,remainder, 0);
	}
}



/********************************************************************
* @������ :������
* @���� : ���ڽ��մ��󣬷��ش����ʶ֡
* @���� :NONE
* @��� :NONE
********************************************************************/
void app_BLUETOOTHmod_err_echo(
	uint8_t *m_recv_buf, 
	const uint8_t err_code
)
{
	uint8_t mod256_sum = 0;
	
	m_recv_buf[CTL_CODE_POS] |= 0xe0;
	m_recv_buf[DATA_LEN_HIGH_POS] = 0;		
	m_recv_buf[DATA_LEN_LOW_POS] =  3;
 
	mod256_sum = app_BLUETOOTHmod_cal_mod256_sum(m_recv_buf, ERR_ECHO_FRAME_LEN);
	m_recv_buf[ERR_ECHO_FRAME_LEN -3]	= err_code;
	m_recv_buf[ERR_ECHO_FRAME_LEN -2]	= mod256_sum;
	m_recv_buf[ERR_ECHO_FRAME_LEN -1]	= 0x16;
	app_BLUETOOTHmod_uart_send_by512(m_recv_buf, ERR_ECHO_FRAME_LEN);
	return;
}


/********************************************************************
* @������ :������
* @���� : �豸��Ϣ����
* @���� : ���ڽ��ջ��棬���ڽ������ݴ�С
* @��� : ��
********************************************************************/
static void app_BLUETOOTHmod_dev_info_download(uint8_t *m_recv_buf, const uint16_t size)
{
	char *ptemp	= PNULL;
	char *pstart = PNULL;
	char *pnext	= PNULL;
	char *pcopy_start = PNULL;
	
	uint16_t data_len = 0;
	uint16_t index = 0;
	uint8_t  search_string_len = 0;
	uint16_t real_string_len = 0;
	
	pcopy_start = pcopy_start;	//δ����ȥ������������
	real_string_len = real_string_len;

	data_len = size - FRAME_HEAD_LEN -2;
	pstart = (char *)m_recv_buf + FRAME_HEAD_LEN;
	ptemp = pstart;
	
	for(index = 0; index < ARRAY_SIZE(BTmod_dev_info_prefix_tbl); index++)
	{
		//�������ַ����ڵĵ�ַ
		ptemp = app_BLUETOOTHmod_search_string(pstart,(char*)BTmod_dev_info_prefix_tbl[index],data_len);
		if(ptemp != PNULL)
		{
			pnext = app_BLUETOOTHmod_search_string(ptemp,",",data_len);		//������һ��","
			if(pnext != PNULL)
			{
				search_string_len = global_strlen((char*)BTmod_dev_info_prefix_tbl[index]);
				real_string_len = pnext - ptemp - search_string_len;
				pcopy_start = ptemp + search_string_len;
				
				if(index == 0)
				{
					;					//δ����
				}
				else if(index == 1)
				{
					;					//δ����
				}
				else if(index == 2)
				{
					;					//δ����
				}
				else if(index == 3)		//δ����
				{
					;
				}
				else if(index == 4)
				{	
					;					//δ����
				}
				else
				{
					;					//δ����
				}
			}
		}
	}
	
	return;
}

/********************************************************************
* @������ :������
* @���� : �ϴ��豸��Ϣ
* @���� : ���ڽ��ջ���
* @��� : ��
********************************************************************/
static void app_BLUETOOTHmod_dev_info_upload(uint8_t *m_recv_buf)
{
	uint16_t 	data_len = 0;
	uint8_t 	mod256_sum = 0;
	uint8_t 	index = 0;
	uint16_t 	send_size = 0;
	char		str_temp[30] = "";
	char		*ptemp = (char *)m_recv_buf + FRAME_HEAD_LEN;
	FIL			*fp = PNULL;
	uint16_t	r = 0;
	
	m_recv_buf[CTL_CODE_POS] |= 0xc0;								//��ΪHAD����Ӧ��
	GLOBAL_MEMCLR(ptemp,RECV_BUF_SIZE - FRAME_HEAD_LEN);
	GLOBAL_MEMCLR(str_temp,sizeof(str_temp));
	
	for(index = 0; index < ARRAY_SIZE(BTmod_dev_info_prefix_tbl); index++)
	{
		global_strcat(ptemp,(char*)BTmod_dev_info_prefix_tbl[index]);
		if(index == 0)
		{
			fp = fatfs_open( "SYSID", FA_OPEN_EXISTING | FA_READ );
			if ( !fp )
			{
				global_strcat(str_temp,"0000-0000-0000-0000");
			}
			else
			{
				f_read( fp, str_temp, fp->fsize, &r );
				if ( r != fp->fsize )
				{
					GLOBAL_MEMCLR(str_temp,sizeof(str_temp));
					global_strcat(str_temp,"0000-0000-0000-0000");
				}	
			}
			fatfs_close( fp );
			global_strcat(ptemp, str_temp);
		}
		else if(index == 1)
		{
			global_strcat(ptemp,HAD_ADDR);
		}
		else if(index == 2)
		{
			global_strcat(ptemp,HAD_MODEL);
		}
		else if(index == 3)
		{
			GLOBAL_MEMCLR(str_temp,sizeof(str_temp));
			global_strcat( str_temp, __DATE__ );
			global_strcat( str_temp, " " );
			//global_strcat( str_temp, "HAD V1.3" );
			global_strcat(ptemp,str_temp);
		}
		else if(index == 4)
		{
			//app_Sysmod_read_FPGA_version(ptemp + global_strlen(ptemp));	
		}
		else
		{
			;
		}
		global_strcat(ptemp,",");
	}
	global_strcat(ptemp,";");

	ptemp = (char *)m_recv_buf;
	//ע: ���ɴӻ������׵�ַ��ʼͳ�Ƴ��ȣ���Ϊ֡ͷǰ13�������ܻ����0
	data_len = global_strlen((char *)m_recv_buf + FRAME_HEAD_LEN) + 2;			//����֡���ݳ���		
	ptemp[DATA_LEN_HIGH_POS] = (data_len & 0xff00)>>8;
	ptemp[DATA_LEN_LOW_POS] = data_len & 0x00ff;

	//����ģ256У��� 

	send_size = data_len + 11;
	mod256_sum = app_BLUETOOTHmod_cal_mod256_sum((uint8_t *)ptemp, send_size);
	ptemp[send_size++] = mod256_sum;
	
	ptemp[send_size++] = 0x16;		//֡β
	app_BLUETOOTHmod_uart_send_by512((uint8_t *)ptemp, send_size);
	return;
}





/********************************************************************
* @������ :������
* @���� : ���ڽ������ݣ����ؽ��յ����ݴ�С
* @���� : ���ջ����ַ
* @��� : ���ڽ�������
********************************************************************/
static Bsize_t app_BLUETOOTHmod_recv( uint8_t *m_recv_buf)
{
	uint16_t	m_recv_data_len = 0;
	uint16_t 	m_recv_size = 0;
	
	if( 1 == os_driver_read(_BT_dev_id,m_recv_buf,1,0))
	{
		if(m_recv_buf[0] == 0x68)														//��ȡ֡ͷ��1���ֽ�0x68
		{
			m_recv_size = 1;
			m_recv_size += os_driver_read(_BT_dev_id,m_recv_buf + m_recv_size,10,0);
			m_recv_data_len = (m_recv_buf[DATA_LEN_HIGH_POS] << 8)+ m_recv_buf[DATA_LEN_LOW_POS];
			m_recv_size += os_driver_read(_BT_dev_id,m_recv_buf + m_recv_size,m_recv_data_len,0);
			m_recv_size += os_driver_read(_BT_dev_id,m_recv_buf + m_recv_size,2,0);	
			if(m_recv_size != m_recv_data_len + 13)
			{
				app_BLUETOOTHmod_err_echo(m_recv_buf,ERR_UART_TIME_OUT);
				return 0;
			}
			return m_recv_size;			
		}
	}
	return 0;
}

static void app_BLUETOOTHmod_cmd_select(uint8_t *m_recv_buf, const uint16_t m_recv_size )
{
	if((0x00 == m_recv_buf[DATA_FLAG_1ST_POS]) && (0x01 == m_recv_buf[DATA_FLAG_2TH_POS]))
	{
		if( 0x11 == m_recv_buf[CTL_CODE_POS])
		{
			app_BLUETOOTHmod_dev_info_upload(m_recv_buf);
		}
		else 
		{
			//app_UARTmod_dev_info_download(m_recv_buf,m_recv_size);
			app_BLUETOOTHmod_err_echo(m_recv_buf,ERR_UART_DATA_FLAG_NOT_EXIT);
		}
	}
				
	else if((0x01 == m_recv_buf[DATA_FLAG_1ST_POS]) && (0x01 == m_recv_buf[DATA_FLAG_2TH_POS]))
	{
		if( 0x14 == m_recv_buf[CTL_CODE_POS])
		{
			//app_BLUETOOTHmod_manualJB_plan_download(m_recv_buf,m_recv_size);
			app_BLUETOOTHmod_err_echo(m_recv_buf,ERR_UART_DATA_FLAG_NOT_EXIT);
		}
	}
	else if((0x02 == m_recv_buf[DATA_FLAG_1ST_POS]) && (0x01 == m_recv_buf[DATA_FLAG_2TH_POS]))
	{
		if( 0x11 == m_recv_buf[CTL_CODE_POS])
		{
			app_BLUETOOTHmod_manualJB_upload_all(m_recv_buf);
		}
	}			
	else if((0x02 == m_recv_buf[DATA_FLAG_1ST_POS]) && (0x02 == m_recv_buf[DATA_FLAG_2TH_POS]))
	{
		if( 0x11 == m_recv_buf[CTL_CODE_POS])
		{
			app_BLUETOOTHmod_manualJB_upload_new(m_recv_buf);
		}
	}
	else 
	{
		app_BLUETOOTHmod_err_echo(m_recv_buf,ERR_UART_DATA_FLAG_NOT_EXIT);
	}
}

/********************************************************************
* @������ :������
* @���� : ���ڷ���ģ��������
* @���� :NONE
* @��� :NONE
********************************************************************/
static void app_BLUETOOTHmod_main_task( void*arg )
{
	uint8_t m_recv_buf[RECV_BUF_SIZE];
	Bsize_t m_recv_size 	= 0;
	uint8_t m_mod256_sum 	= 0;
	uint8_t key_buf			= 0;
	uint8_t refresh_count 	= 0;

	app_BLUETOOTHmod_window_main_creat();
	while ( 1 )
	{
		msleep( TYPE_DELAY );
		//2.1s ˢ�½�����ʾ
		if(refresh_count++ == 100) 
		{
			app_BLUETOOTHmod_window_refresh_comm_stat(WAIT_FOR_CONNECTION);
			refresh_count = 0;
		}
		if ( app_global_key_obtain( &key_buf, 1 ) == 1 ) 	/*��ȡ��������ֵ*/
		{
			if( key_buf==KEY_EXIT )
			{
				app_BLUETOOTHmod_window_destroy();
				//MOD_SWITCH_STARTMOD();
				asm( "  lb 0x300000" );
			}	
		}
		m_recv_size = app_BLUETOOTHmod_recv(m_recv_buf);
		if(m_recv_size > 0)
		{
			m_mod256_sum  = app_BLUETOOTHmod_cal_mod256_sum(m_recv_buf,m_recv_size-2);
			
			if( m_mod256_sum == m_recv_buf[m_recv_size -2])  //У���ж�
			{
				app_BLUETOOTHmod_cmd_select(m_recv_buf,m_recv_size);	
			}
			else
			{
				app_BLUETOOTHmod_err_echo(m_recv_buf,ERR_UART_SUM_CHECK);
			}
		}
		GLOBAL_MEMCLR(m_recv_buf, RECV_BUF_SIZE);
		if ( os_task_delete_req( SELF_PRO ) == OS_TASK_DEL_REQ )
		{
			os_task_delete( SELF_PRO );	
		}	
	}
}
/********************************************************************
 * @���� :������
 * @���� :�����շ�ģ�������ܳ�ʼ��
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void app_BLUETOOTHmod_fun_entry( void )
{
	uint32_t arg;
	
	BT_main_task_pro = os_task_assign_stk_build(app_BLUETOOTHmod_main_task, 8);
	os_task_name_set(BT_main_task_pro, "BLUETOOTHmod--Main task" );
	
	_BT_dev_id = os_driver_open(BTMOD_UART_DEV_NAME);
	arg = UARTMOD_BAUD;
	os_driver_ioctl(_BT_dev_id, SET_SERIAL_BAUD, &arg);
	arg = UARTMOD_UART_NO_CHECHK;
	os_driver_ioctl(_BT_dev_id, SET_SERIAL_CHECK,&arg);
	return;
}
/********************************************************************
 * @���� :������
 * @���� :�����շ�ģ���������ͷ���Դ
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void app_BLUETOOTHmod_fun_exit( void )
{
	app_BLUETOOTHmod_window_destroy();
	if ( BT_main_task_pro != INVALID_PRO )
	{
		OS_DELETE_REQ( BT_main_task_pro );
		BT_main_task_pro = INVALID_PRO;
	}
	os_driver_close(_BT_dev_id);
	_BT_dev_id = INVALID_DEV_ID;
	return;
}

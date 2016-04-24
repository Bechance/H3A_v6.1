/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:��������
* ����ʱ��:2010-07-14
* ������˾ :����
* �ļ�����:serialdriver.c   //for 28335
* ������ :�ҳ�
* ��������:�ṩ���ڲ���API�ӿڣ��������ڵײ�FIFO 128BYTES
*----------------------------------------------------------------
�޸ļ�¼:

�޸�����:

�޸���:

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
uint8_t      	m_bluetooth_serialB_rev_fifo[MAX_REV_FIFO_SIZE] = {0, };  //����FIFO
fifo_tcb_t   	m_bluetooth_serialB_fifo_tcb_body = {0, };  				//���ڿ���TCB
fifo_tcb_t*   	m_bluetooth_serialB_fifo_tcb = &m_bluetooth_serialB_fifo_tcb_body;  //���ڿ���TCB
#endif
a_lock_t        *m_bluetooth_serialB_read_lock  = INVALID_LOCK;
a_lock_t        *m_bluetooth_serialB_write_lock = INVALID_LOCK;


const char* m_BTmod_AT_command_tbl[]=
{
	"+++OPEN\n",		//����ATģʽ����
	"gc200808\n",		//����ATģʽ����
	"AT+ENTER\n",		//�˳�ATģʽ����������ģʽ����
	"AT\n",				//ATģʽ��������
	"AT+DEFAULT\n",		//�ָ�Ĭ�ϲ���
	"AT+ADDR?\n",		//����������ַ��ʾ
	"AT+VER?\n",		//�̼��汾��ʾ
	"AT+PIN?\n",		//��ʾ������ǰpin��
	"AT+PIN=",			//������õ�pin���س���pin�����Ϊ���ֿ���ĸ
	"AT+UART?\n",		//��ʾ��ǰ����ģ�鲨����
	"AT+UARTBAUDRATE=",	//�������������ʣ�=�żӲ����ʼ��س�
	"AT+UARTSTOP=",		//����ֹͣλ��, =�ź��ֹͣλ�����س�
	"AT+UARTPARITY=",	//���ô���У��λ��=�ź��У��λ���س���N��У�飬EżУ�飬O��У��
	"AT+ROLE?\n",		//���ӽ�ɫ����ʾ
	"AT+ROLE=",			//���ӽ�ɫ���ã� =�żӽ�ɫ���س���0:slave,1:master
	"AT+INQMODE?\n",	//��ʾ��ѯģʽ����
	"AT+INQMAXNUM=",	//��������ѯ�����豸����=�ź���豸�����س�
	"AT+INQTIME=",		//���ò�ѯʱ��,=�ź��ʱ�估�س�
	"AT+INQUIRY\n",		//��ʼ��ѯ��Χ�����豸
	"AT+BINDADDR?\n",	//��ʾ�󶨵Ĵӵ�ַ
	"AT+BINDADDR0=",	//��0�ŵ�ַ
	"AT+DEVICENAME?\n",	//��ʾ���������豸��
	"AT+DEVICENAME=",	//��������ģ�����֣�=�ź�����ּ��س�����֧�����ֺ���ĸ
	"AT+MSGENABLE?\n",	//������ʾ��Ϣ��ʾ����
	"AT+MSGENABLE=",	//������ʾ��Ϣ��ʾ�������=�ź������س���1:����0��
};

 /*
 2812 �������б�
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
 * @������ :�ҳ�
 * @����:���ò�����
 * @����:baud Ҫ���õ�baud
 * @���:ʵ��д���С
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
 * @������ :�ҳ�
 * @����:���ڽ����ж�
 *
 * @����:
 *
 *@���:
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
	fifo_buffer_push( m_bluetooth_serialB_fifo_tcb, &buf, 1 );	/*�����յ�������װ��FIFO*/
	ScibRegs.SCIFFRX.bit.RXFFINTCLR = 1; 						/*Clear Interrupt flag*/
	PieCtrlRegs.PIEACK.all |= 0x100;        					/*Issue PIE ack*/
	return;
}


#pragma CODE_SECTION(bluetooth_serialB_us_delay,"ramfuncs") 		//���öδ���������ramfuncs��
#pragma CODE_SECTION(DSP28x_usDelay,"ramfuncs") //���öδ���������ramfuncs��
static void bluetooth_serialB_us_delay( uint16_t x )	/* 1��s,150	MHz	*/
{
	uint16_t count = 0;
	//m_offset_cnt = (uint16_t)(x * 7.5);
	count = (uint16_t)(x * 45);
	DSP28x_usDelay(count);
	return;
}

/********************************************************************
 * @������ :�ҳ�
 * @����:����д��һ������
 *
 * @����:buf ��д������ͷ��size �����С
 *
 *@���:ʵ��д���С
********************************************************************/
Bsize_t bluetooth_driver_write( uint8_t*buf, Bsize_t buf_size , void*pos )
{
	Bsize_t i;
	for ( i = 0;i < buf_size;i++ )
	{
		//while ( ScibRegs.SCIFFTX.bit.TXFFST == 0X10 ) ;
		while ( ScicRegs.SCICTL2.bit.TXRDY == 0 ) ;
		ScibRegs.SCITXBUF = *buf++;
		//����flash��ram�������ٶȲ�һ������˽���ʱ����
		//������ram�����У��Ա�֤��ʱʱ��һ��
		//bluetooth_serialB_us_delay(220);  //��ʱ520us
		bluetooth_serialB_us_delay(250);  //��ʱ520us
	}
	return buf_size;
}


/********************************************************************
 * @������ :�ҳ�
 * @����:����д��һ������
 *
 * @����:buf ��д������ͷ��size �����С
 *
 *@���:ʵ��д���С
********************************************************************/
static Bsize_t bluetooth_serialB_write( uint8_t*buf, Bsize_t buf_size )
{
	Bsize_t i;
	for ( i = 0;i < buf_size;i++ )
	{
		//while ( ScibRegs.SCIFFTX.bit.TXFFST == 0X10 ) ;
		while ( ScicRegs.SCICTL2.bit.TXRDY == 0 ) ;
		ScibRegs.SCITXBUF = *buf++;
		//����flash��ram�������ٶȲ�һ������˽���ʱ����
		//������ram�����У��Ա�֤��ʱʱ��һ��
		bluetooth_serialB_us_delay(250);  //��ʱ520us
	}
	return buf_size;
}

/********************************************************************
 * @������ :������
 * @����:�����ַ����������(�޷���)������ط�30�Σ�
 *       ���ط�������״̬: 1:�ɹ���0ʧ��
 *
 * @����:�����͵����ݻ�����
 *
 *@���:ʵ��д���С
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
 * @������ :������
 * @����:�����ַ����������(�޷���)������ط�50�Σ�
 *       ���ط�������״̬: 1:�ɹ���0ʧ��
 *
 * @����:�����͵����ݻ�����, ���͵�AT����
 *
 *@���:ʵ��д���С
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
 * @������ :������
 * @����: ��ʾ������������
 *
 * @����:
 *
 *@���:
********************************************************************/
static void bluetooth_inqury_param_get(char *param)
{
	char ch[50]= "";
	uint8_t state = 0;
	state = state;								//ȥ�����뾯��
	if(param == PNULL) return;
	
	global_strcpy(ch, (char*)"+++OPEN\r");		//����ATģʽ
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"gc200808\r");		//����ATģʽ����
	state = _bt_cmd_retry_send(ch); 
	
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+INQMODE?\r");	//���Ͳ�ѯ����
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+RESET\r");	//�˳�ATģʽ
	state = _bt_cmd_retry_send(ch); 
	return;
	
}


/********************************************************************
 * @������ :������
 * @����: ������ѯ
 *
 * @����:
 *
 *@���:
********************************************************************/
static void bluetooth_inqury_start(char *addr)
{
	char ch[50]= "";
	uint8_t state = 0;

	if(addr == PNULL) return;
	addr = addr;								//ȥ�����뾯��
	state = state;								//ȥ�����뾯��
	global_strcpy(ch, (char*)"+++OPEN\r");		//����ATģʽ
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"gc200808\r");		//����ATģʽ����
	state = _bt_cmd_retry_send(ch); 
	
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+INQUIRY\r");	//���Ͳ�ѯ����
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+RESET\r");	//�˳�ATʽ
	state = _bt_cmd_retry_send(ch); 
	return;
	
}


/********************************************************************
 * @������ :������
 * @����: ������ѯ
 *
 * @����:
 *
 *@���:
********************************************************************/
static void bluetooth_set_inqury_time(char *ptime)
{
	char ch[50]= "";
	uint8_t state = 0;
	uint8_t index = 0;

	state = state;								//ȥ�����뾯��
	if(ptime == PNULL)
		return;
	for(index = 0; index < global_strlen(ptime); index++)	//pin�����Ϊ����
	{
		if( !isdigit( *(ptime +index) ) )
			return;
	}
	global_strcpy(ch, (char*)"+++OPEN\r");		//����ATģʽ
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"gc200808\r");		//����ATģʽ����
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+INQTIME=");	//����������ѯʱ��
	global_strcat(ch, ptime);
	global_strcat(ch, "\r");
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+RESET\r");	//�˳�ATģʽ
	state = _bt_cmd_retry_send(ch); 
	return;
	
}

/********************************************************************
 * @������ :������
 * @����: ����pin��
 *
 * @����:�Ӷ�pin����
 *
 *@���:
********************************************************************/
static void bluetooth_pin_set(char *pin)
{
	char ch[50]= "";
	uint8_t state = 0;
	uint8_t index = 0;

	state = state;								//ȥ�����뾯��

	if(pin == PNULL)
		return;
	for(index = 0; index < global_strlen(pin); index++)	//pin�����Ϊ���ֻ�Ӣ����ĸ
	{
		if( !isalnum( *(pin +index) ) )
			return;
	}
	global_strcpy(ch, (char*)"+++OPEN\r");		//����ATģʽ
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"gc200808\r");		//����ATģʽ����
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+PIN=");		//����pin����������
	global_strcat(ch, pin);
	global_strcat(ch,"\r");
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+RESET\r");	//�˳�ATģʽ
	state = _bt_cmd_retry_send(ch); 
	return;
}

/********************************************************************
 * @������ :������
 * @����: �󶨵�ַ
 *
 * @����:�Ӷ�������ַ
 *
 *@���:
********************************************************************/
static void  bluetooth_bindaddr_set(char *addr)
{
	char ch[50]= "";
	uint8_t index = 0;
	uint8_t state = 0;
	
	state = state;								//ȥ�����뾯��

	if(addr == PNULL)
		return;
	//��ַ�����Ϊ���ֻ�Ӣ����ĸ��":"
	for(index = 0; index < global_strlen(addr); index++)	
	{
		if( (!isalnum(*(addr +index))) && ( *(addr+index) != ':') )
			return;
	}
	global_strcpy(ch, (char*)"+++OPEN\r");		//����ATģʽ
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"gc200808\r");		//����ATģʽ����
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+BINDADDR0=");		//�󶨴Ӷ�������ַ
	global_strcat(ch, addr);
	global_strcat(ch,"\r");
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+RESET\r");	//�˳�ATģʽ
	state = _bt_cmd_retry_send(ch); 
	
	return;
}



/********************************************************************
 * @������ :������
 * @����: ���ش���״̬��ʾ��Ϣ
 *
 * @����:��������: 1: ��; 0:��
 *
 *@���:
********************************************************************/
static void  bluetooth_message_onoff(uint8_t *onoff)
{
	char ch[50]= "";
	uint8_t state = 0;
	
	state = state;								//ȥ�����뾯��

	if(onoff == PNULL) return;
	global_strcpy(ch, (char*)"+++OPEN\r");		//����ATģʽ
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"gc200808\r");		//����ATģʽ����
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
	global_strcpy(ch, (char*)"AT+RESET\r");	//�˳�ATģʽ
	state = _bt_cmd_retry_send(ch); 
	return;
}


/********************************************************************
 * @������ :������
 * @����: ��ʾ��ǰ��������ַ
 *
 * @����:��
 *
 *@���:
********************************************************************/
static void  bluetooth_view_cur_bind_addr(char *addr)
{
	char ch[50]= "";
	uint8_t state = 0;
	
	state = state;								//ȥ�����뾯��
	if(addr == PNULL) return;
	GLOBAL_MEMCLR(m_bluetooth_serialB_rev_fifo, sizeof(m_bluetooth_serialB_rev_fifo));
	GLOBAL_MEMCLR(ch, sizeof(ch));
	addr = addr;
	
	global_strcpy(ch, (char*)"+++OPEN\r");		//����ATģʽ
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"gc200808\r");		//����ATģʽ����
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+BINDADDR?");	
	global_strcat(ch,"\r");
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+RESET\r");	//�˳�ATģʽ
	state = _bt_cmd_retry_send(ch); 
	return;
}

/********************************************************************
 * @������ :������
 * @����: ��ʾ��ǰ��������ַ
 *
 * @����:��
 *
 *@���:
********************************************************************/
static void  bluetooth_set_role(uint8_t *role)
{
	char ch[50]= "";
	uint8_t state = 0;
	
	state = state;								//ȥ�����뾯��
	if(role == PNULL) return;
	GLOBAL_MEMCLR(m_bluetooth_serialB_rev_fifo, sizeof(m_bluetooth_serialB_rev_fifo));

	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"+++OPEN\r");		//����ATģʽ
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"gc200808\r");		//����ATģʽ����
	state = _bt_cmd_retry_send(ch); 
	
	GLOBAL_MEMCLR(ch, sizeof(ch));	
	global_strcpy(ch, (char*)"AT+ROLE=");		//����������ģʽ
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
	global_strcpy(ch, (char*)"AT+PIN=");		//����pin����������
	global_strcat(ch, BLUETOOTH_DEFAULT_PIN_CODE);
	global_strcat(ch,"\r");
	state = _retry_send_at_cmd(ch,(char*)"AT+PIN=");
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+RESET\r");	//�˳�ATģʽ
	state = _retry_send_at_cmd(ch, (char*)"AT+RESET"); 
	
	return;
}


/********************************************************************
 * @������ :������
 * @����: ������������ģʽ���رյ�����Ϣ���룬���ù̶�PIN�룬
 *        ���趨����
 *
 * @����:
 *       
 *@���:
********************************************************************/
static void bluetooth_set_bt_default_state(char *addr)
{
	char ch[50]= "";
	uint8_t state = 0;
	
	state = state;								//ȥ�����뾯��
	if(addr == PNULL) return;
	
	GLOBAL_MEMCLR(m_bluetooth_serialB_rev_fifo, sizeof(m_bluetooth_serialB_rev_fifo));
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"+++OPEN\r");			//����ATģʽ
	state = _bt_cmd_retry_send(ch);
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"gc200808\r");			//����ATģʽ����
	state = _bt_cmd_retry_send(ch);
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+ROLE=1\r");	
	state = _retry_send_at_cmd(ch,(char*)"AT+ROLE=");
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+PIN=");		//����pin����������
	global_strcat(ch, BLUETOOTH_DEFAULT_PIN_CODE);
	global_strcat(ch,"\r");
	state = _retry_send_at_cmd(ch,(char*)"AT+PIN=");
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+MSGENABLE=0\r");		//�رմ�����ʾ��Ϣ
	state = _retry_send_at_cmd(ch,(char*)"AT+MSGENABLE=");
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+BINDADDR0=");		//����pin����������
	global_strcat(ch, addr);
	global_strcat(ch,"\r");
	state = _retry_send_at_cmd(ch,(char*)"AT+BINDADDR0=");  
	
	GLOBAL_MEMCLR(ch, sizeof(ch));
	global_strcpy(ch, (char*)"AT+RESET\r");			//�˳�ATģʽ
	state = _retry_send_at_cmd(ch,(char*)"AT+RESET"); 
	return;
}




/********************************************************************
 * @������ :
 * @����: �����������ýӿ�
 *
 * @����:none
 *
 *@���:none
********************************************************************/
void  bluetooth_driver_ioctl( uint8_t cmd, void*arg )
{
	Bsize_t *pstate = ( Bsize_t* )arg;
	
	switch ( cmd )
	{
		case AT_CMD_INQ_START :
			fifo_buffer_empty( m_bluetooth_serialB_fifo_tcb );		//���ȷ�����ҽ���ǲ����ѵ��ظ�����	
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
			fifo_buffer_empty( m_bluetooth_serialB_fifo_tcb );		//���ȷ�����ҽ���ǲ����ѵ��ظ�����	
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
 * @������ :�ҳ�
 * @����:�Ӵ��ڶ�������
 *
 * @���� : buf:ָ�����������
                  size:��Ҫ���ݴ�С
 *               port :���ں�
 *@���:ʵ�ʶ�����С
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
 * @������ :
 * @���� :����ͨ��ѡ��
 * @���� :
 * @��� :
********************************************************************/

static bluetooth_serialB_select_path(void)
{
	
	EALLOW;
		/*�Ͽ��������ƴ���*/
		GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 0;   
		GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 0;   	
		GpioCtrlRegs.GPADIR.bit.GPIO18  = 0;    //���ó�����
		GpioCtrlRegs.GPADIR.bit.GPIO19  = 0;    //���ó�����
    	//����14 15Ϊ����
		GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;	   // Enable pull-up for GPIO14 (SCITXDB)
		GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;	   // Enable pull-up for GPIO15 (SCIRXDB)
		GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 3;  // Asynch input GPIO15 (SCIRXDB)
		GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 2;   // Configure GPIO14 for SCITXDB operation
		GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 2;   // Configure GPIO15 for SCIRXDB operation
	EDIS;
    return;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :����Ӳ������
 * @���� :
 * @��� :
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
	ScibRegs.SCICTL1.all = 0x0063;    //RX�����ж�����

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
 * @������ :�ҳ�
 * @����:���ڳ�ʼ��
 *
 * @����:
 *                   baud:������
 *                   parity:�Ƿ�������żУ��
 *                   stop_bit:�Ƿ�Ҫֹͣλ
 *
 *@���:none
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
	bluetooth_serialB1_init();				//��ʼ������
	//bluetooth_hardware_set();
	os_lock_creat( &m_bluetooth_serialB_read_lock );
	os_lock_creat( &m_bluetooth_serialB_write_lock );
	fifo_buffer_init( m_bluetooth_serialB_fifo_tcb, MAX_REV_FIFO_SIZE, m_bluetooth_serialB_rev_fifo );
}

/********************************************************************
 * @������ :�ҳ�
 * @����:����ж��
 *
 * @����:
 *
 *@���:none
********************************************************************/
void bluetooth_driver_uninstall( void )
{
	SERIALB_INT_DIS();
#if  COMPILER_MALLOC_SUPPORT >0
	fifo_buffer_destroy( m_bluetooth_serialB_fifo_tcb );
#endif
	return;
}



dev_id_t bluetooth_driver_register( void )    //����ģ�鴮��
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



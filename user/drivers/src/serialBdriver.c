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
#define 	SELECT_TOUCH  0
#define 	SELECT_WIFI   1

#define     SERIALB_INT_EN()  {PieCtrlRegs.PIEIER9.bit.INTx3 = 1;}
#define     SERIALB_INT_DIS() {PieCtrlRegs.PIEIER9.bit.INTx3 =0;}
#if 		 COMPILER_MALLOC_SUPPORT >0
fifo_tcb_t*	m_serialB_fifo_tcb = 0;
#else
uint8_t      	m_serialB_rev_fifo[GPS_REV_FIFO_SIZE] = {0, };  //����FIFO
fifo_tcb_t   	m_serialB_fifo_tcb_body = {0, };  				//���ڿ���TCB
fifo_tcb_t*   	m_serialB_fifo_tcb = &m_serialB_fifo_tcb_body;  //���ڿ���TCB
#endif
a_lock_t        *m_serialB_read_lock  = INVALID_LOCK;
a_lock_t        *m_serialB_write_lock = INVALID_LOCK;
void 			 serialB_driver_init( void );
void 			 serialB1_driver_init( void );
uint8_t          m_path_select = SELECT_WIFI;
/********************************************************************
 * @������ :�ҳ�
 * @����:���ڽ����ж�
 *
 * @����:
 *
 *@���:
********************************************************************/

interrupt void serialB_rx_isr( void )
{
	uint8_t buf;
	if ( ScibRegs.SCIRXST.bit.RXERROR == 1 )
	{
		ScibRegs.SCICTL1.all = 0x0003;
		ScibRegs.SCICTL1.all = 0x0023;
		//if(m_path_select==SELECT_WIFI)
			//serialB1_driver_init();
	//	else 
	//		serialB_driver_init();
		ScibRegs.SCIFFRX.bit.RXFFINTCLR = 1; 	/*Clear Interrupt flag*/
		PieCtrlRegs.PIEACK.all |= 0x100;    		 /*Issue PIE ack*/
		return;
	}
	buf = ( ScibRegs.SCIRXBUF.all ) & 0x00ff;
	fifo_buffer_push( m_serialB_fifo_tcb, &buf, 1 );/*�����յ�������װ��FIFO*/
	ScibRegs.SCIFFRX.bit.RXFFINTCLR = 1; /*Clear Interrupt flag*/
	PieCtrlRegs.PIEACK.all |= 0x100;        /*Issue PIE ack*/
	return;
}
/*
 2812 �������б�
*/
baud_set_t my_serialB_baud_tab[] =
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
void serialB_set_baud( uint32_t baud )
{
	Bsize_t index;
	for ( index = 0;index < SIZE_OF_ARRAY( my_serialB_baud_tab );index++ )
	{
		if ( my_serialB_baud_tab[index].set_baud == baud )
		{
			ScibRegs.SCIHBAUD = my_serialB_baud_tab[index].set_SCIHBAUD;
			ScibRegs.SCILBAUD = my_serialB_baud_tab[index].set_SCILBAUD;
			return;
		}
	}
	//ScibRegs.SCIHBAUD = 0x3D;
	//ScibRegs.SCILBAUD = 0x08;
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @����:����У�鷽ʽ
 *
 * @����:chSelect ��ʽѡ��
 *
 *@���:ʵ��д���С
********************************************************************/
void serialB_set_check( uint8_t chSelect )
{
	switch ( chSelect )
	{
	case EVEN_CHECK://żУ��
		ScibRegs.SCICCR.all = 0x0067;
		break;
	case ODD_CHECK://��У��
		ScibRegs.SCICCR.all = 0x0027;
		break;
	default:
		ScibRegs.SCICCR.all = 0x0007; // 1 stop bit,  No loopback
		// No parity,8 char bits,
		// async mode, idle-line protocol
		break;
	}
}

#pragma CODE_SECTION(serialB_us_delay,"ramfuncs") 		//���öδ���������ramfuncs��
#pragma CODE_SECTION(DSP28x_usDelay,"ramfuncs") //���öδ���������ramfuncs��
static void serialB_us_delay( uint16_t x )	/* 1��s,150	MHz	*/
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
Bsize_t serialB_driver_write( uint8_t*buf, Bsize_t buf_size , void*pos )
{
	Bsize_t i;
	for ( i = 0;i < buf_size;i++ )
	{
		//while ( ScibRegs.SCIFFTX.bit.TXFFST == 0X10 ) ;
		while ( ScicRegs.SCICTL2.bit.TXRDY == 0 ) ;
		ScibRegs.SCITXBUF = *buf++;
		//����flash��ram�������ٶȲ�һ������˽���ʱ����
		//������ram�����У��Ա�֤��ʱʱ��һ��
		serialB_us_delay(250);  //��ʱ520us
	}
	return buf_size;
}
/********************************************************************
 * @������ :�ҳ�
 * @����: �����������ýӿ�
 *
 * @����:none
 *
 *@���:none
********************************************************************/
void  serialB_driver_ioctl( uint8_t cmd, void*arg )
{
	Bsize_t *pstate = ( Bsize_t* )arg;
	switch ( cmd )
	{
	case SET_SERIAL_BAUD :
		serialB_set_baud( *(( uint32_t* )arg ) );
		break;
	case SET_SERIAL_CHECK:
		serialB_set_check( *(( uint8_t* )arg ) );
		break;
	case SET_EMPTY_FIFO:
		SERIALB_INT_DIS();
		fifo_buffer_empty( m_serialB_fifo_tcb );
		SERIALB_INT_EN();
		break;
	case  DRIVER_LOCK_REVERT:
		os_lock_revert( m_serialB_read_lock );
		os_lock_revert( m_serialB_write_lock );
		break;
	case GET_FIFO_STATE:
		SERIALB_INT_DIS();
		*pstate = fifo_buffer_get_size( m_serialB_fifo_tcb );
		SERIALB_INT_EN();
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
 * @���� : buf :ָ�����������
           size:��Ҫ���ݴ�С
 *         port:���ں�
 *@���:ʵ�ʶ�����С
********************************************************************/
Bsize_t  serialB_driver_read( uint8_t*buf, Bsize_t size , void*pos )
{
	Bsize_t real_size = 0;
	os_lock( m_serialB_read_lock );
	SERIALB_INT_DIS();
	real_size	= fifo_buffer_get_size( m_serialB_fifo_tcb );
	SERIALB_INT_EN();

	if ( real_size < size )
	{
		msleep(( size - real_size ) );
		SERIALB_INT_DIS();
		real_size	= fifo_buffer_get_size( m_serialB_fifo_tcb );
		SERIALB_INT_EN();
		if ( real_size == 0 )
		{
			os_unlock( m_serialB_read_lock );
			return 0; 
		}
	}
	if ( real_size > size )
	{
		real_size = size;
	}
	SERIALB_INT_DIS();
	fifo_buffer_pop( m_serialB_fifo_tcb, buf, real_size );
	SERIALB_INT_EN();
	os_unlock( m_serialB_read_lock );
	return real_size;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :����ͨ��ѡ��
 * @���� :
 * @��� :
********************************************************************/

static serialB_driver_select_path(uint8_t icon)
{
	if(icon==SELECT_WIFI)
	{
		EALLOW;
		/*�Ͽ��������ƴ���*/
		GpioCtrlRegs.GPAMUX2.bit.GPIO18  = 0;   
		GpioCtrlRegs.GPAMUX2.bit.GPIO19  = 0;   	
		GpioCtrlRegs.GPADIR.bit.GPIO18   = 0;   //���ó�����
		GpioCtrlRegs.GPADIR.bit.GPIO19   = 0;   //���ó�����
        //����14 15Ϊ����
		GpioCtrlRegs.GPAPUD.bit.GPIO14   = 0;	// Enable pull-up for GPIO14 (SCITXDB)
		GpioCtrlRegs.GPAPUD.bit.GPIO15   = 0;   // Enable pull-up for GPIO15 (SCIRXDB)
		GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 3;   // Asynch input GPIO15 (SCIRXDB)
		GpioCtrlRegs.GPAMUX1.bit.GPIO14  = 2;   // Configure GPIO14 for SCITXDB operation
		GpioCtrlRegs.GPAMUX1.bit.GPIO15  = 2;   // Configure GPIO15 for SCIRXDB operation
		EDIS;
	}
	else if(icon==SELECT_TOUCH)
	{
		EALLOW;
		/*�Ͽ�14 15��������*/
		GpioCtrlRegs.GPAMUX1.bit.GPIO14  = 0;   
		GpioCtrlRegs.GPAMUX1.bit.GPIO15  = 0;   	
		GpioCtrlRegs.GPADIR.bit.GPIO14   = 0;   //���ó�����
		GpioCtrlRegs.GPADIR.bit.GPIO15   = 0;   //���ó�����
        //����18 19Ϊ����
		GpioCtrlRegs.GPAPUD.bit.GPIO18   = 0;	// Enable pull-up for GPIO18 (SCITXDB)
		GpioCtrlRegs.GPAPUD.bit.GPIO19   = 0;	// Enable pull-up for GPIO19 (SCIRXDB)
		GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3;   // Asynch input GPIO19 (SCIRXDB)
		GpioCtrlRegs.GPAMUX2.bit.GPIO18  = 2;   // Configure GPIO18 for SCITXDB operation
		GpioCtrlRegs.GPAMUX2.bit.GPIO19  = 2;   // Configure GPIO19 for SCIRXDB operation
		EDIS;
	}
    return;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :����Ӳ������
 * @���� :
 * @��� :
********************************************************************/
static serialB_driver_hardware_set(void)
{
	ScibRegs.SCICCR.all = 0x0007;  // 1 stop bit,  No loopback
	// No parity,8 char bits,
	// async mode, idle-line protocol
	ScibRegs.SCICTL1.all = 0x0003; // enable TX, RX, internal SCICLK,
	// Disable RX ERR, SLEEP, TXWAKE
	ScibRegs.SCICTL2.bit.TXINTENA = 0;
	ScibRegs.SCICTL2.bit.RXBKINTENA = 1;
	serialB_set_baud( 9600 );	//set baud is 9600
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
	PieVectTable.SCIRXINTB = &serialB_rx_isr;
	EDIS;
    return;
}
/********************************************************************
 * @������ :�ҳ�
 * @����:���ڳ�ʼ��
 * @����:
 *  baud:������
 *  parity:�Ƿ�������żУ��
 *  stop_bit:�Ƿ�Ҫֹͣλ
 * @���:none
********************************************************************/
void serialB_driver_init( void )
{
    serialB_driver_hardware_set();
	m_path_select = SELECT_TOUCH;
	serialB_driver_select_path(SELECT_TOUCH);
	fifo_buffer_init( m_serialB_fifo_tcb, GPS_REV_FIFO_SIZE, m_serialB_rev_fifo );
	os_lock_creat( &m_serialB_read_lock );
	os_lock_creat( &m_serialB_write_lock );
	return;
}

void serialB1_driver_init( void )
{
    serialB_driver_hardware_set();
	m_path_select = SELECT_WIFI;
	serialB_driver_select_path(SELECT_WIFI);
	fifo_buffer_init( m_serialB_fifo_tcb, GPS_REV_FIFO_SIZE, m_serialB_rev_fifo );
	os_lock_creat( &m_serialB_read_lock );
	os_lock_creat( &m_serialB_write_lock );
	return;
}

/********************************************************************
 * @������ :�ҳ�
 * @����:����ж��
 *
 * @����:
 *
 *@���:none
********************************************************************/
void serialB_driver_uninstall( void )
{
	SERIALB_INT_DIS();
#if  COMPILER_MALLOC_SUPPORT >0
	fifo_buffer_destroy( m_serialB_fifo_tcb );
#endif
	return;
}
/********************************************************************
 * @������: �ҳ�
 * @����: ��������ע��
 * @����:none
 * @���:NONE
********************************************************************/
dev_id_t serialB_driver_register( void )    //��������
{
	uint8_t ret;
	os_dirver_operate_t temp;
	temp.os_driver_init      = serialB_driver_init;
	temp.os_driver_ioctl     = serialB_driver_ioctl;
	temp.os_driver_read      = serialB_driver_read;
	temp.os_driver_write     = serialB_driver_write;
	temp.os_driver_uninstall = serialB_driver_uninstall;
	ret = os_driver_add( &temp, "serialB Driver" );
	return ret;
}

dev_id_t serialB1_driver_register( void )    //WIFIģ�鴮��
{
	uint8_t ret;
	os_dirver_operate_t temp;
	temp.os_driver_init      = serialB1_driver_init;
	temp.os_driver_ioctl     = serialB_driver_ioctl;
	temp.os_driver_read      = serialB_driver_read;
	temp.os_driver_write     = serialB_driver_write;
	temp.os_driver_uninstall = serialB_driver_uninstall;
	ret = os_driver_add( &temp, "serialB1 Driver" );
	return ret;
}

#pragma DATA_SECTION (serialB1232_list,"DR_LIST_ADDR")
const DR_member_t serialB1232_list[]=
{
	serialB1_driver_register,
	0x5a5a
};



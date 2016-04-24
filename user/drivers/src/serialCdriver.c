/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:串口驱动
* 创建时间:2012-06-08
* 所属公司 :科荟
* 文件名称:serialCdriver.c   //for 28335
* 创建人 :揭成
* 功能描述:提供串口操作API接口，创建串口底层FIFO 128BYTES
*----------------------------------------------------------------
修改记录:

修改日期:

修改人:

*********************************************************************************************************
*/
#include "../Osdriver.h"

#define     SERIALC_INT_EN()  {PieCtrlRegs.PIEIER8.bit.INTx5 = 1;}
#define     SERIALC_INT_DIS() {PieCtrlRegs.PIEIER8.bit.INTx5 = 0;}

#if 		 COMPILER_MALLOC_SUPPORT >0
fifo_tcb_t*	m_serialC_fifo_tcb = 0;
#else
uint8_t      	m_serialC_rev_fifo[MAX_REV_FIFO_SIZE] = {0, };// 串口FIFO
fifo_tcb_t   	m_serialC_fifo_tcb_body = {0, };  //串口控制TCB
fifo_tcb_t*   	m_serialC_fifo_tcb = &m_serialC_fifo_tcb_body;  //串口控制TCB
#endif

a_lock_t        *m_serialC_read_lock = INVALID_LOCK;
a_lock_t        *m_serialC_write_lock = INVALID_LOCK;
uint8_t			g_delay_baud_flag = 0;
uint8_t			g_rec_crc_flag = 0;

void 		serialC_driver_init( void );
/********************************************************************
 * @创建人 :揭成
 * @功能:串口接收中断
 *
 * @输入:
 *
 *@输出:
********************************************************************/
interrupt void serialC_rx_isr( void )
{
	uint8_t buf;
	if ( ScicRegs.SCIRXST.bit.RXERROR == 1 )
	{
		ScicRegs.SCICTL1.all = 0x0003;
		ScicRegs.SCICTL1.all = 0x0023;
		//serialC_driver_init();
		ScicRegs.SCIFFRX.bit.RXFFINTCLR = 1; 		/*Clear Interrupt flag*/
		PieCtrlRegs.PIEACK.all |= 0x80;    		 	/*Issue PIE ack*/
		return;
	}
	buf = ( ScicRegs.SCIRXBUF.all ) & 0x00ff;
	if ( g_rec_crc_flag == 1 )	buf &= 0x7F;
	fifo_buffer_push( m_serialC_fifo_tcb, &buf, 1 );/*将接收到的数据装进FIFO*/
	ScicRegs.SCIFFRX.bit.RXFFINTCLR = 1; 			/*Clear Interrupt flag*/
	PieCtrlRegs.PIEACK.all |= 0x0080;        		/*Issue PIE ack*/
	//PieCtrlRegs.PIEACK.all = 0xFFFF;        		/*Issue PIE ack*/
	return;
}
/*
 2812 波特率列表
*/
baud_set_t my_serialC_baud_tab[] =
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
void serialC_set_baud( uint32_t baud )
{
	Bsize_t index;
	for ( index = 0;index < SIZE_OF_ARRAY( my_serialC_baud_tab );index++ )
	{
		if ( my_serialC_baud_tab[index].set_baud == baud )
		{
			ScicRegs.SCIHBAUD = my_serialC_baud_tab[index].set_SCIHBAUD;
			ScicRegs.SCILBAUD = my_serialC_baud_tab[index].set_SCILBAUD;
			return;
		}
	}
	//ScibRegs.SCIHBAUD = 0x3D;
	//ScibRegs.SCILBAUD = 0x08;
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能:设置校验方式
 *
 * @输入:chSelect 方式选择
 *
 *@输出:实际写入大小
********************************************************************/
void serialC_set_check( uint8_t chSelect )
{
	switch ( chSelect )
	{
	case EVEN_CHECK://偶校验
		ScicRegs.SCICCR.all = 0x0067;
		break;
	case ODD_CHECK://奇校验
		ScicRegs.SCICCR.all = 0x0027;
		break;
	default:
		ScicRegs.SCICCR.all = 0x0007; // 1 stop bit,  No loopback
		// No parity,8 char bits,
		// async mode, idle-line protocol
		break;
	}
}

void serialB_set_7E1check( uint8_t chSelect )
{
	switch ( chSelect )
	{
	case EVEN_CHECK://偶校验
		ScibRegs.SCICCR.all = 0x0066;
		g_rec_crc_flag = 1;
		break;
	case ODD_CHECK://奇校验
		ScibRegs.SCICCR.all = 0x0026;
		g_rec_crc_flag = 1;
		break;
	default:
		ScibRegs.SCICCR.all = 0x0007; // 1 stop bit,  No loopback
		g_rec_crc_flag = 0;
		// No parity,8 char bits,
		// async mode, idle-line protocol
		break;
	}
}
/********************************************************************
 * @创建人 :揭成
 * @功能:串口写入一个数组
 *
 * @输入:buf 待写入数组头，size 数组大小
 *
 *@输出:实际写入大小
********************************************************************/
Bsize_t serialC_driver_write( uint8_t*buf, Bsize_t buf_size , void*pos )
{
	Bsize_t i;
	for ( i = 0;i < buf_size;i++ )
	{
		//while ( ScicRegs.SCIFFTX.bit.TXFFST == 0x10 ) ;
		while ( ScicRegs.SCICTL2.bit.TXRDY == 0 ) ;
		ScicRegs.SCITXBUF = *buf++;
	}
	return buf_size;
}
/********************************************************************
 * @创建人 :揭成
 * @功能: 驱动参数设置接口
 *
 * @输入:none
 *
 *@输出:none
********************************************************************/
void  serialC_driver_ioctl( uint8_t cmd, void*arg )
{
	Bsize_t *pstate = ( Bsize_t* )arg;
	switch ( cmd )
	{
	case SET_SERIAL_BAUD :
		serialC_set_baud( *(( uint32_t* )arg ) );
		break;
	case SET_SERIAL_CHECK:
		serialC_set_check( *(( uint8_t* )arg ) );
		break;
	case SET_EMPTY_FIFO:

		SERIALC_INT_DIS();
		fifo_buffer_empty( m_serialC_fifo_tcb );
		SERIALC_INT_EN();
		break;
	case  DRIVER_LOCK_REVERT:
		os_lock_revert( m_serialC_read_lock );
		os_lock_revert( m_serialC_write_lock );
		break;
	case GET_FIFO_STATE:
		SERIALC_INT_DIS();
		*pstate = fifo_buffer_get_size( m_serialC_fifo_tcb );
		SERIALC_INT_EN();
		break;
	case SET_SERIAL_CHECK2:
		serialB_set_7E1check( *(( uint8_t* )arg ) );
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
Bsize_t  serialC_driver_read( uint8_t*buf, Bsize_t size , void*pos )
{
	Bsize_t real_size = 0;
	os_lock( m_serialC_read_lock );
	SERIALC_INT_DIS();
	real_size	= fifo_buffer_get_size( m_serialC_fifo_tcb );
	SERIALC_INT_EN();

	if ( real_size < size )
	{
		msleep(( size - real_size ) );
		SERIALC_INT_DIS();
		real_size	= fifo_buffer_get_size( m_serialC_fifo_tcb );
		SERIALC_INT_EN();
		if ( real_size == 0 )
		{
			os_unlock( m_serialC_read_lock );
			return 0; 
		}
	}
	if ( real_size > size )
	{
		real_size = size;
	}
	SERIALC_INT_DIS();
	fifo_buffer_pop( m_serialC_fifo_tcb, buf, real_size );
	SERIALC_INT_EN();
	os_unlock( m_serialC_read_lock );
	return real_size;
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
void serialC_driver_init( void )
{
	EALLOW;

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled disabled by the user.  
// This will enable the pullups for the specified pins.

	GpioCtrlRegs.GPBPUD.bit.GPIO62 = 0;    // Enable pull-up for GPIO62 (SCIRXDC)
	GpioCtrlRegs.GPBPUD.bit.GPIO63 = 0;	   // Enable pull-up for GPIO63 (SCITXDC)

/* Set qualification for selected pins to asynch only */
// Inputs are synchronized to SYSCLKOUT by default.  
// This will select asynch (no qualification) for the selected pins.

	GpioCtrlRegs.GPBQSEL2.bit.GPIO62 = 3;  // Asynch input GPIO62 (SCIRXDC)

/* Configure SCI-C pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be SCI functional pins.

	GpioCtrlRegs.GPBMUX2.bit.GPIO62 = 1;   // Configure GPIO62 for SCIRXDC operation
	GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 1;   // Configure GPIO63 for SCITXDC operation

	PieCtrlRegs.PIEIER8.bit.INTx5 = 1;
	PieVectTable.SCIRXINTC = &serialC_rx_isr;
    EDIS;
	
	ScicRegs.SCICCR.all = 0x0007;  // 1 stop bit,  No loopback
	// No parity,8 char bits,
	// async mode, idle-line protocol
	ScicRegs.SCICTL1.all = 0x0003; // enable TX, RX, internal SCICLK,
	// Disable RX ERR, SLEEP, TXWAKE
	ScicRegs.SCICTL2.bit.TXINTENA = 0;
	ScicRegs.SCICTL2.bit.RXBKINTENA = 1;
	serialC_set_baud( 2400 );	//set baud is 9600
	ScicRegs.SCICCR.bit.LOOPBKENA = 0; // Enable loop back
	//ScicRegs.SCIFFTX.all = 0xc03f;//0xC028;
	//ScicRegs.SCIFFRX.all = 0x0021;
	//ScicRegs.SCIFFCT.all = 0x00;

	ScicRegs.SCICTL1.all = 0x0023;    // Relinquish SCI from Reset
	ScicRegs.SCICTL1.all = 0x0063;    //RX错误中断允许

	//ScicRegs.SCIFFTX.bit.TXFIFOXRESET = 1;
	//ScicRegs.SCIFFRX.bit.RXFIFORESET = 1;
	//ScicRegs.SCIPRI.bit.SOFT = 1;
	//ScicRegs.SCIPRI.bit.FREE = 0;	
	
#if  COMPILER_MALLOC_SUPPORT >0
	m_serialC_fifo_tcb = fifo_buffer_init( MAX_REV_FIFO_SIZE );
#else
	fifo_buffer_init( m_serialC_fifo_tcb, MAX_REV_FIFO_SIZE, m_serialC_rev_fifo );
#endif
	os_lock_creat( &m_serialC_read_lock );
	os_lock_creat( &m_serialC_write_lock );
	return;
}

/********************************************************************
 * @创建人 :揭成
 * @功能:串口卸载
 *
 * @输入:
 *
 *@输出:none
********************************************************************/
void serialC_driver_uninstall( void )
{
	SERIALC_INT_DIS();
#if  COMPILER_MALLOC_SUPPORT >0
	fifo_buffer_destroy( m_serialC_fifo_tcb );
#endif
	return;
}

/********************************************************************
 * @创建人: 揭成
 * @功能: 串口驱动注册
 * @输入:none
 * @输出:NONE
********************************************************************/
dev_id_t serialC_driver_register( void )
{
	uint8_t ret;
	os_dirver_operate_t temp;
	temp.os_driver_init    = serialC_driver_init;
	temp.os_driver_ioctl  = serialC_driver_ioctl;
	temp.os_driver_read  = serialC_driver_read;
	temp.os_driver_write = serialC_driver_write;
	temp.os_driver_uninstall = serialC_driver_uninstall;
	ret = os_driver_add( &temp, "D232" );
	return ret;
}

#pragma DATA_SECTION (serialC232_list,"DR_LIST_ADDR")
const DR_member_t serialC232_list[]=
{
	serialC_driver_register,
	0x5a5a
};




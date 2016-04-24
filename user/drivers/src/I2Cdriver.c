/******************************Copyright (c)***************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:28335 i2c ����
* ���ʱ��2011-5-17
* ������˾ :����
* �ļ�����:I2c.c
* ������ :�ҳ�
* ��������:ʵ��I2C��д����
*----------------------------------------------------------------
�޸ļ�¼:

�޸�����:

�޸���:

*********************************************************************************************************
*/
#include "../Osdriver.h"
static uint16_t     m_chip_id = 0xff;

BOOL i2c_driver_xrdy( void )
{
	Bsize_t cnt = 0;
	while ( !( I2caRegs.I2CSTR.bit.XRDY ) )
	{
		DELAY_US( 10 );
		if ( ++cnt > 1000 )
		{
			return FAIL;
		}
	}
	return  TURE;
}

BOOL i2c_driver_rrdy( void )
{
	Bsize_t cnt = 0;
	while ( !( I2caRegs.I2CSTR.bit.RRDY ) )
	{
		DELAY_US( 10 );
		if ( ++cnt > 1000 )
		{
			return FAIL;
		}
	}
	return  TURE;
}
uint16_t i2c_driver_write_data( uint16_t*buf, uint16_t addr, uint16_t size )
{
	uint16_t i;
	if ( I2caRegs.I2CSTR.bit.BB == 1 )
	{
		return I2C_BUS_BUSY_ERROR;
	}
	if ( !i2c_driver_xrdy() )
		return I2C_BUS_BUSY_ERROR;
	I2caRegs.I2CSAR = m_chip_id;
	I2caRegs.I2CCNT = size + 1;
	I2caRegs.I2CDXR = addr;
	I2caRegs.I2CMDR.all = 0x6E20;
	for ( i = 0; i < size; i++ )
	{
		if ( !i2c_driver_xrdy() )
			return I2C_BUS_BUSY_ERROR;
		I2caRegs.I2CDXR = *buf;
		buf++;
		if ( I2caRegs.I2CSTR.bit.NACK == 1 )
			return	I2C_BUS_BUSY_ERROR;
	}
	return I2C_SUCCESS;
}

uint16_t i2c_driver_read_data( uint16_t  *buf, uint16_t addr, uint16_t size )
{
	uint16_t  i, Temp;

	if ( I2caRegs.I2CSTR.bit.BB == 1 )
	{
		return I2C_BUS_BUSY_ERROR;
	}
	if ( !i2c_driver_xrdy() )
		return I2C_BUS_BUSY_ERROR;
	I2caRegs.I2CSAR = m_chip_id;
	I2caRegs.I2CCNT = 1;
	I2caRegs.I2CDXR = addr;
	I2caRegs.I2CMDR.all = 0x6620;
	if ( I2caRegs.I2CSTR.bit.NACK == 1 )
		return	I2C_BUS_BUSY_ERROR;
	DELAY_US( 50 );
	if ( !i2c_driver_xrdy() )
		return I2C_BUS_BUSY_ERROR;
	I2caRegs.I2CSAR = m_chip_id;
	I2caRegs.I2CCNT = size;
	I2caRegs.I2CMDR.all = 0x6C20;
	if ( I2caRegs.I2CSTR.bit.NACK == 1 )
		return	I2C_BUS_BUSY_ERROR;
	for ( i = 0;i < size;i++ )
	{
		if ( !i2c_driver_rrdy() )
			return	I2C_BUS_BUSY_ERROR;
		Temp = I2caRegs.I2CDRR;
		if ( I2caRegs.I2CSTR.bit.NACK == 1 )
			return	I2C_BUS_BUSY_ERROR;
		*buf = Temp;
		buf++;
	}
	return I2C_SUCCESS;
}
/********************************************************************
 * @������: �ҳ�
 * @����: �����ײ�д���
 * @����:	buf д�뻺��      size Ҫд���ݵĴ�С
 * @���:	ʵ��д���Ĵ�С
********************************************************************/
static Bsize_t i2c_driver_write( uint8_t*buf, Bsize_t size , void*pos )
{
	Bsize_t  ret = 0;
	if ( i2c_driver_write_data(( uint16_t* )buf, *(( uint16_t* )pos ), ( uint16_t )size )
		 == I2C_SUCCESS )
	{
		ret = size;
	}
	return ret;
}
/********************************************************************
 * @������: �ҳ�
 * @����: �����ײ�����
 * @����:	buf ���뻺��      size Ҫ�����ݵĴ�С
 * @���:	ʵ�ʶ����Ĵ�С
********************************************************************/
static Bsize_t i2c_driver_read( uint8_t*buf, Bsize_t size , void*pos )
{
	Bsize_t  ret = 0;
	if ( i2c_driver_read_data(( uint16_t* )buf, *(( uint16_t* )pos ), ( uint16_t )size )
		 == I2C_SUCCESS )
	{
		ret = size;
	}
	return ret;
}
/********************************************************************
 * @������: �ҳ�
 * @����: �����ײ�����
 * @����:
 * @���:	ʵ�ʶ����Ĵ�С
********************************************************************/
void  i2c_driver_set_baud( uint32_t baud )
{
	return;
}
/********************************************************************
 * @������: �ҳ�
 * @����: ������������
 * @����:	cmd ��������  arg ���Ʋ���
 * @���:	NONE
********************************************************************/
static void  i2c_driver_ioctl( uint8_t cmd, void*arg )
{
	switch ( cmd )
	{
	case I2C_CMD_SET_CHIP_ID:
		m_chip_id = *(( uint16_t* )arg );
		break;
	case SET_I2C_BAUD:
		i2c_driver_set_baud( *(( uint32_t* )arg ) );
		break;
	default:
		break;
	}
	return;
}
/********************************************************************
 * @������: �ҳ�
 * @����: ����ж��
 * @����:	NONE
 * @���:	NONE
********************************************************************/
static void i2c_driver_uninstall( void )
{
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @����: 28335 i2c gpio ��ʼ��
 *
 * @����:none
 *
 *@���:none
********************************************************************/
static void i2c_driver_init_GPIO()
{
	EALLOW;
	/* Enable internal pull-up for the selected pins
	 Pull-ups can be enabled or disabled disabled by the user.
	 This will enable the pullups for the specified pins.
	Comment out other unwanted lines. */

	GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;    		/*Enable pull-up for GPIO32 (SDAA) */
	GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;	   		/* Enable pull-up for GPIO33 (SCLA) */

	/* Set qualification for selected pins to asynch only
	This will select asynch (no qualification) for the selected pins.
	Comment out other unwanted lines. */

	GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3; 		/*Asynch input GPIO32 (SDAA) */
	GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;  		/*Asynch input GPIO33 (SCLA)*/

	/* Configure SCI pins using GPIO regs
	This specifies which of the possible GPIO pins will be I2C functional pins.
	 Comment out other unwanted lines. */

	GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1;  /*Configure GPIO32 for SDAA operation*/
	GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1;   /*Configure GPIO33 for SCLA operation*/
	EDIS;
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @����: 28335 i2c ����
 *
 * @����:none
 *
 *@���:none
********************************************************************/

static void i2c_driver_config( void )
{
	// Initialize I2C-A:
	I2caRegs.I2CMDR.bit.IRS = 0;
	I2caRegs.I2CSAR = m_chip_id;
	I2caRegs.I2CPSC.all = 9;			// Prescaler - need 7-12 Mhz on module clk
	I2caRegs.I2CCLKL = 15;			// NOTE: must be non zero
	I2caRegs.I2CCLKH = 15;			// NOTE: must be non zero
	I2caRegs.I2CIER.all = 0x00;		// Enable SCD & ARDY interrupts

	I2caRegs.I2CMDR.bit.IRS = 1;	// Take I2C out of reset
	return;
}
/********************************************************************
 * @������: �ҳ�
 * @����: ������ʼ��
 * @����:	NONE
 * @���:	NONE
********************************************************************/
static void i2c_driver_init( void )
{
	i2c_driver_init_GPIO();
	i2c_driver_config();
	return ;
}
/********************************************************************
 * @������: �ҳ�
 * @����:����ע��
 * @����:none
 * @���:NONE
********************************************************************/
dev_id_t i2c_driver_register( void )
{
	uint8_t ret;
	os_dirver_operate_t temp;
	temp.os_driver_init   = i2c_driver_init;
	temp.os_driver_ioctl  = i2c_driver_ioctl;
	temp.os_driver_read  = i2c_driver_read;
	temp.os_driver_write = i2c_driver_write;
	temp.os_driver_uninstall = i2c_driver_uninstall;
	ret = os_driver_add( &temp, "I2C Driver" );
	return ret;
}

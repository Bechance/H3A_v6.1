/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:串口驱动
* 创建时间:2012-06-08
* 所属公司 :科荟
* 文件名称:serialDdriver.c   //for 28335
* 创建人 :揭成
* 功能描述:提供串口操作API接口，创建串口底层FIFO 128BYTES
*----------------------------------------------------------------
修改记录:

修改日期:

修改人:

*********************************************************************************************************
*/
#include "../Osdriver.h"

#define     	serialD_INT_EN()  {}
#define     	serialD_INT_DIS() {}
#define         SEND_WRITE_INFO_TO_PIC()        (GpioDataRegs.GPBSET.bit.GPIO60=1)
#define         CLOSE_WRITE_INFO_TO_PIC()       (GpioDataRegs.GPBCLEAR.bit.GPIO60=1)
#define  		_WATI_PIC_CLK()					{while (GpioDataRegs.GPBDAT.bit.GPIO61==1);while(GpioDataRegs.GPBDAT.bit.GPIO61==0);}

uint8_t      	m_serialD_rev_fifo[MAX_REV_FIFO_SIZE] = {0, };// 串口FIFO
fifo_tcb_t   	m_serialD_fifo_tcb_body = {0, };  //串口控制TCB
fifo_tcb_t*   	m_serialD_fifo_tcb = &m_serialD_fifo_tcb_body;  //串口控制TCB
a_lock_t        *m_serialD_read_lock = INVALID_LOCK;
a_lock_t        *m_serialD_write_lock = INVALID_LOCK;
void 			serialD_driver_init( void );

#define         PIC_BUS_ADDR        		0x4100
/********************************************************************
 * @创建人 :揭成
 * @功能:串口接收中断
 *
 * @输入:
 *
 *@输出:
********************************************************************/
void serialD_data_ISR(uint8_t *ch)
{
    fifo_buffer_push(m_serialD_fifo_tcb,ch,1);
	return;
}


typedef struct
{
     uint8_t head_1;
     uint8_t head_2;
	 uint8_t cmd;
     uint8_t write_or_read;  //1表示读，2表示写
	 uint8_t data_len_h;
	 uint8_t data_len_l;
}PIC_head_t;


static int _package_create(void*head,uint8_t*pbuf,Bsize_t buf_size)
{
    PIC_head_t     *phead = (PIC_head_t*)head;
	uint8_t*		pdata;
	uint8_t 		crc =0;
	uint16_t 		data_len = buf_size;
	phead->head_1 		= 0x68;
	phead->head_2 		= 0x86;
	phead->cmd    		= 0x07;	//#define PC_DATA_COM 0x07;#define GPS_DATA_COM	0x08
	phead->write_or_read= 2;	//WRITE_PIC
	phead->data_len_l 	= data_len&0xff;
	phead->data_len_h 	= ((data_len&0xff00)>>8);
	pdata				= (uint8_t*)&phead->data_len_l+1;
    global_memcpy(pdata,pbuf,data_len);
	pdata+=data_len;
	pdata[0]            = crc;
	pdata[1]            = 0x16;
	return (sizeof(PIC_head_t)+data_len+2);
}
/********************************************************************
 * @创建人 :揭成
 * @功能:串口写入一个数组
 *
 * @输入:buf 待写入数组头，size 数组大小
 *
 *@输出:实际写入大小
********************************************************************/
Bsize_t serialD_driver_write( uint8_t*buf, Bsize_t buf_size , void*pos )
{
	uint16_t offset = 0;
	uint8_t* pdata; 
	void*    phead;
	int      data_len;
	phead=app_malloc(buf_size+100);
	if(!phead) return 0;
    data_len=_package_create(phead,buf,buf_size);
	pdata = (uint8_t*)phead;
	DINT;
	SEND_WRITE_INFO_TO_PIC();
	for ( offset = 0;offset < data_len;offset+=4)
	{
		*( uint16_t* )(PIC_BUS_ADDR+0) = pdata[offset];
		*( uint16_t* )(PIC_BUS_ADDR+1) = pdata[offset+1];
		*( uint16_t* )(PIC_BUS_ADDR+2) = pdata[offset+2];
		*( uint16_t* )(PIC_BUS_ADDR+3) = pdata[offset+3];
		_WATI_PIC_CLK();
	}
	CLOSE_WRITE_INFO_TO_PIC();
	EINT;
	app_free(phead);
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
void  serialD_driver_ioctl( uint8_t cmd, void*arg )
{
	Bsize_t *pstate = ( Bsize_t* )arg;
	switch ( cmd )
	{
	case SET_SERIAL_BAUD :
		break;
	case SET_SERIAL_CHECK:
		break;
	case SET_EMPTY_FIFO:
		serialD_INT_DIS();
		fifo_buffer_empty( m_serialD_fifo_tcb );
		serialD_INT_EN();
		break;
	case GET_FIFO_STATE:
		serialD_INT_DIS();
		*pstate = fifo_buffer_get_size( m_serialD_fifo_tcb );
		serialD_INT_EN();
		break;
	default:
		break;
	}
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能:从串口读出数据
 *
 * @输入 : buf :指向读出缓冲区
           size:需要数据大小
 *         port:串口号
 *@输出:实际读出大小
********************************************************************/
Bsize_t  serialD_driver_read( uint8_t*buf, Bsize_t size , void*pos )
{
	Bsize_t real_size = 0;
	os_lock( m_serialD_read_lock );
	serialD_INT_DIS();
	real_size	= fifo_buffer_get_size( m_serialD_fifo_tcb );
	serialD_INT_EN();

	if ( real_size < size )
	{
		msleep(( size - real_size ) );
		serialD_INT_DIS();
		real_size	= fifo_buffer_get_size( m_serialD_fifo_tcb );
		serialD_INT_EN();
		if ( real_size == 0 )
		{
			os_unlock( m_serialD_read_lock );
			return 0; 
		}
	}
	if ( real_size > size )
	{
		real_size = size;
	}
	serialD_INT_DIS();
	fifo_buffer_pop( m_serialD_fifo_tcb, buf, real_size );
	serialD_INT_EN();
	os_unlock( m_serialD_read_lock );
	return real_size;
}

/********************************************************************
 * @创建人:揭成
 * @功能:串口初始化
 *
 * @输入:
 *        baud:波特率
 *        parity:是否启用奇偶校验
 *        stop_bit:是否要停止位
 *
 * @输出:none
********************************************************************/
void serialD_driver_init( void )
{
	fifo_buffer_init( m_serialD_fifo_tcb, MAX_REV_FIFO_SIZE, m_serialD_rev_fifo );
	os_lock_creat( &m_serialD_read_lock );
	os_lock_creat( &m_serialD_write_lock );
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
void serialD_driver_uninstall( void )
{
	return;
}

/********************************************************************
 * @创建人: 揭成
 * @功能: 串口驱动注册
 * @输入:none
 * @输出:NONE
********************************************************************/
dev_id_t serialD_driver_register( void )
{
	uint8_t ret;
	os_dirver_operate_t temp;
	temp.os_driver_init      = serialD_driver_init;
	temp.os_driver_ioctl     = serialD_driver_ioctl;
	temp.os_driver_read      = serialD_driver_read;
	temp.os_driver_write     = serialD_driver_write;
	temp.os_driver_uninstall = serialD_driver_uninstall;
	ret = os_driver_add( &temp, "serialD Driver" );
	return ret;
}

/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:��������
* ����ʱ��:2010-07-14
* ������˾ :����
* �ļ�����:keydriver
* ������ :�ҳ�
* ��������:��������֧��,�ж���Ӧ����֧��������
*                      ��֧����ϼ���
*----------------------------------------------------------------
�޸ļ�¼:

�޸�Ŀ��:

�޸�����:

�޸���:

*********************************************************************************************************
*/
#include "../Osdriver.h"
#define 		  KEY_FIFO_SIZE  2

static uint8_t 		_key_fifo[KEY_FIFO_SIZE] = {0, };
static fifo_tcb_t   _key_fifo_tcb_body 	     = {0, };
static fifo_tcb_t*  _key_fifo_tcb 	         = &_key_fifo_tcb_body;

void GUI_key_create(uint8_t key);

#define           FPGAKEY_ADDR   0X4001

#define           KEY_INT_EN()   {;}
#define           KEY_INT_DIS()  {;}
/********************************************************************
 * @������ :�ҳ�
 * @����: �����������ýӿ�
 *
 * @����:cmd ��������  arg���Ʋ���
 *
 * @���:TURE �ɹ� FAIL ʧ��
********************************************************************/
void  key_driver_ioctl( uint8_t cmd, void*arg )
{
	uint8_t *key_buf = ( uint8_t* )arg;
	switch ( cmd )
	{
	case  KEY_CMD_DUMMY_PRESS:
		fifo_buffer_push(_key_fifo_tcb, key_buf, 1 );
		GUI_key_create(*key_buf);
		break;
	case  KEY_CMD_BUF_EMPTY:
		fifo_buffer_empty(_key_fifo_tcb);
		break;
	default:
		break;
	}
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @����:û�ж���
 * @����:buf ��д����
 *       size:Ҫд���С
 * @���:ʵ��д���ֽ�
********************************************************************/
Bsize_t key_driver_write( uint8_t*buf, Bsize_t size , void*pos )
{
	return 0;
}

/********************************************************************
 * @������ :�ҳ�
 * @����:����ɨ��
 * @����:none
 * @���:TURE �ɹ� FAIL ʧ��
********************************************************************/
void key_read_isr( uint8_t key)
{
	fifo_buffer_push( _key_fifo_tcb, &key, 1 );
	GUI_key_create(key);
	return;
}

/********************************************************************
 * @������: �ҳ�
 * @����: ��õ�ǰ��ֵ
 *
 * @����:buf ��Ŷ������ݻ���
 *       size:��Ҫ�����Ĵ�С
 *
 * @���:ʵ�ʶ�����С
********************************************************************/
Bsize_t key_driver_read( uint8_t*buf, Bsize_t size , void*pos )
{
	Bsize_t real_size;
	KEY_INT_DIS();
	real_size = fifo_buffer_get_size( _key_fifo_tcb );
	if ( !real_size )
	{
		KEY_INT_EN();
		return 0;
	}
	if ( real_size > size )
	{
		real_size = size;
	}
	fifo_buffer_pop( _key_fifo_tcb, buf, real_size );
	KEY_INT_EN();
	return real_size;
}
/********************************************************************
 * @������: �ҳ�
 * @����: ����������ʼ��
 * @����: NONE
 * @���: NONE
********************************************************************/
void key_driver_init( void )
{
	fifo_buffer_init( _key_fifo_tcb, KEY_FIFO_SIZE, _key_fifo );/*��ʼ������fifo*/
	return;
}
/********************************************************************
 * @������: �ҳ�
 * @����: ��������ж��
 * @����: NONE
 * @���: NONE
********************************************************************/
void key_driver_uninstall( void )
{
	return;
}
/********************************************************************
 * @������: �ҳ�
 * @����: ��������ע��
 * @����:none
 * @���:NONE
********************************************************************/
dev_id_t key_driver_register( void )
{
	uint8_t ret;
	os_dirver_operate_t temp;
	temp.os_driver_init  = key_driver_init;
	temp.os_driver_ioctl = key_driver_ioctl;
	temp.os_driver_read  = key_driver_read;
	temp.os_driver_write = key_driver_write;
	temp.os_driver_uninstall = key_driver_uninstall;
	ret = os_driver_add( &temp, "KEY" );
	return ret;
}
/*driver AUTO Register*/
#pragma DATA_SECTION (key_list,"DR_LIST_ADDR")
const DR_member_t key_list[]=
{
	key_driver_register,
	0x5a5a
};


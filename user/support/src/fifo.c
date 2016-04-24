/********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:���FIFO֧��
* ����ʱ��:2010-07-26
* ������˾ :����
* �ļ�����:FIFO.C
* ������ :�ҳ�
* ��������: �ṩ���FIFO֧�֣�ע���ģ��ֳ���
*                        ����������̬�����ڴ�Ͳ�����̬����
				�ڴ�����������������ֻ�г�ʼ���ġ�
				ʱ��ͬ�����û�����COMPILER_MALLOC_SUPPORT
				������������ģʽ��
*
*----------------------------------------------------------------
�޸ļ�¼:

�޸�����:

�޸���:

*********************************************************************************************************
*/
#include "../app_support.h"
#if FIFO_EN >0
#if COMPILER_MALLOC_SUPPORT >0
/*********************************************************************
*   @��������:��ʼ��һ��FIFO
*    @������:�ҳ�
*   @����:
*     buffer_size :FIFO��������С
*   @ output :FIFO ���ƿ�λ��
*********************************************************************/
fifo_tcb_t * fifo_buffer_init( Bsize_t buffer_size )
{
	fifo_tcb_t *  	result = 0;
	Bsize_t 		total_struct_size = sizeof( fifo_tcb_t ) + buffer_size - 1;

	result = ( fifo_tcb_t * ) malloc( total_struct_size );
	if ( result )
	{
		global_memset(( uint8_t* )result, 0, total_struct_size );
		result->buffer_size = buffer_size;
	}
	return result;
}
/*********************************************************************
*   @��������:����һ��FIFO �ͷ�FIFO��ռ�ڴ�
*    @������:�ҳ�
*   @����:
*    self_ptr :FIFO TCB λ��
*   @ output :NONE
*********************************************************************/
void fifo_buffer_destroy( fifo_tcb_t * * self_ptr )
{
	if (( NULL == self_ptr ) || ( NULL == *self_ptr ) )
	{
		return ;
	}
	free( *self_ptr );
	*self_ptr = NULL;
	return;
}
#else  /*û��MALLOC ֧�ֵ�ʱ��д��*/
/*********************************************************************
*   @��������:��ʼ��һ��FIFO
*   @������:�ҳ�
*   @����:
*      	self  :FIFO����TCB
*     	buffer_size :FIFO��������С
*      	buf_ptr:FIFO������
*   @ output :NONE
*********************************************************************/
void  fifo_buffer_init( fifo_tcb_t*self, Bsize_t buffer_size, uint8_t *buf_ptr )
{
	self->buffer_size = buffer_size;
	self->buffer_ptr = buf_ptr ;
	self->end_pos = 0;
	self->start_pos = 0;
	return;
}
#endif
/*********************************************************************
*    @��������:�ƶ���ǰFIFOλ��+1
*   @������:�ҳ�
*   @����:
*      self  :FIFO����TCB
*      pos  :λ������
*    @ output :
*        �ƶ���λ��
*********************************************************************/
Bsize_t  fifo_buffer_shift( fifo_tcb_t * self, Bsize_t pos )
{
	if (( pos + 1 ) == self->buffer_size )
	{
		return ( 0 );
	}
	else
	{
		return ( pos + 1 );
	}
}
/*********************************************************************
   @��������:����СΪSIZE ������װ��FIFO
   @������:�ҳ�
   @����:self :FIFO����TCB
     data :�������ݻ���
     size :���ݴ�С
   @ output :NONE
*********************************************************************/
void fifo_buffer_push( fifo_tcb_t * self, unsigned char*data, Bsize_t size )
{
	uint16_t cur_size;
	for ( cur_size = 0;cur_size < size;cur_size++ )
	{
		self->buffer_ptr[self->end_pos ] = data[cur_size];
		self->end_pos  = fifo_buffer_shift( self, self->end_pos );

		if ( self->end_pos == self->start_pos )
		{
			self->start_pos  = fifo_buffer_shift( self, self->start_pos );
		}
	}
	return;
}
/*********************************************************************
*  @��������:����СΪsize�����Ƴ�FIFO
*    @������:�ҳ�
*  @����:
*     self  :FIFO����TCB
*     data  :�������ݻ���
*     size	:���ݴ�С
*  @ output :NONE
*********************************************************************/
void fifo_buffer_pop( fifo_tcb_t * self, uint8_t*data, Bsize_t size )
{
	uint16_t cur_size;
	for ( cur_size = 0;cur_size < size;cur_size++ )
	{
		data[cur_size] = self->buffer_ptr[self->start_pos];
		self->start_pos = fifo_buffer_shift( self, self->start_pos );
	}
	return ;
}
/********************************************************************
*   @��������:�õ���ǰFIFO�����ݴ�С
*   @������:�ҳ�
*   @����:
*    self  :FIFO����TCB
*   @ output :FIFO���ݴ�С
********************************************************************/
Bsize_t fifo_buffer_get_size( fifo_tcb_t * self )
{
	if ( self->end_pos >= self->start_pos )
	{
		return self->end_pos - self->start_pos;
	}
	else
	{
		return self->buffer_size - ( self->start_pos - self->end_pos );
	}
}
/********************************************************************
*   @��������:���FIFO
*   @������:�ҳ�
*   @����:
*    self  :FIFO����TCB
*   @ output :FIFO���ݴ�С
********************************************************************/
void fifo_buffer_empty( fifo_tcb_t * self )
{
	self->start_pos = 0;
	self->end_pos = 0;
	return;
}
#endif


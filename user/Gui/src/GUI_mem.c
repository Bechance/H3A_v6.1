/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:GUI�ڴ����ģ��
* ����ʱ��:2012-07-22
* ������˾ :����
* �ļ�����: GUI_MEM.c
* ������ :	�ҳ�
* ��������: ����GUI�ڴ�
*----------------------------------------------------------------*/
#include	"../GUI.h"

#define  		GUI_MEM_NBLK     	   (450)
#define  		GUI_BLOCK_SIZE    	 	100
#define  		GUI_MAX_MEM_NBLK      	GUI_MEM_NBLK
#define         GUI_MEM_IS_IDLE 		'F'/*��������*/
#define         GUI_MEM_IS_MOCCUPY 	 	'M'/*����ռ��*/
#define         GUI_MEM_IS_SOCCUPY  	'S'/*��������ռ��*/
uint8_t			GUI_mem_pdest[GUI_MAX_MEM_NBLK];
uint8_t 		GUI_malloc_mem[GUI_MEM_NBLK][GUI_BLOCK_SIZE];
uint8_t       	GUI_mem_ctl[GUI_MEM_NBLK+1];
#define  		MSIZE_TO_NBLK(size)	((size+ GUI_BLOCK_SIZE - 1 )/ GUI_BLOCK_SIZE)
a_lock_t * 		_MemLOCK = INVALID_LOCK;
#define  ALLOC_LOCK()      os_lock(_MemLOCK);
#define  ALLOC_UNLOCK()    os_unlock(_MemLOCK);
/********************************************************************
 * @������ :�ҳ�
 * @����:����nblk���ڴ��
 *
 * @����:pMmem     pmem
 *
 *@���: ������ڴ�ָ��
********************************************************************/
void*GUI_malloc( uint32_t mem_size )
{
	char*ptemp;
	uint8_t offset;
	uint8_t nblk;
	mem_size += 1;
	nblk = MSIZE_TO_NBLK( mem_size );
	if ( !mem_size ) return PNULL;
	ALLOC_LOCK();
	if ( nblk <= GUI_MEM_NBLK )  					/*�ж�NBLK�ĺϷ���*/
	{
		GUI_memset( GUI_mem_pdest,
					GUI_MEM_IS_IDLE,
					sizeof( char )*nblk );		/*��Ҫ�ҵ��ڴ�ϳ��ַ���*/
		GUI_mem_pdest[nblk] = PNULL;				/*Ϊ�ַ�����һ����β*/
		ptemp = GUI_string_locate(
					( char* )( GUI_mem_ctl ),
					( char* )GUI_mem_pdest );		/*��λ�ַ���*/
		if ( ptemp != PNULL )
		{
			*ptemp = GUI_MEM_IS_MOCCUPY; 			/*�������ȼ���Ӧ��PROΪ��ռ*/
			GUI_memset(( uint8_t* )( ptemp + 1 ),
					   GUI_MEM_IS_SOCCUPY,
					   ( nblk - 1 ) );		 	/*����Ϊ����*/
			offset = ( uint8_t* )ptemp -
					 ( uint8_t* )( GUI_mem_ctl ); 	/*�����ƫ��*/
			ptemp = ( char* )GUI_malloc_mem +
					(( GUI_BLOCK_SIZE ) * offset );	/*����������ڴ��ָ��ͷ*/
			ALLOC_UNLOCK();
			if ((( long )ptemp % 2 ) == 0 )  return ptemp;	/*�����������뵽��ָ��*/
			else     return	(( char* )ptemp + 1 );
		}
	}
	nblk = GUI_MEM_NBLK;
	ALLOC_UNLOCK();
	return PNULL;
}
/********************************************************************
 * @������ :�ҳ�
 * @����:�ͷ��ڴ��
 *
 * @����:pMmem     pmem
 *
 *@���: ������ڴ�ָ��
********************************************************************/
void GUI_free( void*pmem )
{
	uint8_t	offset = 0;
	if ( pmem < ( void* )GUI_malloc_mem[0]
		 || pmem > ( void* )GUI_malloc_mem[GUI_MEM_NBLK-1] )
		return;
	ALLOC_LOCK();
	offset = (( uint8_t* )pmem - ( uint8_t* )GUI_malloc_mem )
			 / GUI_BLOCK_SIZE;							/*�����ƫ��*/
	GUI_mem_ctl[offset] = GUI_MEM_IS_IDLE;					/*�ͷ���ռ��*/
	offset++;
	while ( GUI_mem_ctl[offset] == GUI_MEM_IS_SOCCUPY )
	{
		GUI_mem_ctl[offset] = GUI_MEM_IS_IDLE;				/*�ͷŸ�����*/
		offset++;
	}
	ALLOC_UNLOCK();
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @����:�ڴ������ʼ��
 *
 * @����:void
 *
 *@���: ���ƿ�λ��
********************************************************************/
void GUI_mem_maneger_init( void )
{
	GUI_memset( GUI_mem_ctl, GUI_MEM_IS_IDLE, GUI_MAX_MEM_NBLK );
	return;
}

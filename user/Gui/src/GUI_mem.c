/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:GUI内存管理模块
* 创建时间:2012-07-22
* 所属公司 :科荟
* 文件名称: GUI_MEM.c
* 创建人 :	揭成
* 功能描述: 管理GUI内存
*----------------------------------------------------------------*/
#include	"../GUI.h"

#define  		GUI_MEM_NBLK     	   (450)
#define  		GUI_BLOCK_SIZE    	 	100
#define  		GUI_MAX_MEM_NBLK      	GUI_MEM_NBLK
#define         GUI_MEM_IS_IDLE 		'F'/*空闲索引*/
#define         GUI_MEM_IS_MOCCUPY 	 	'M'/*任务占用*/
#define         GUI_MEM_IS_SOCCUPY  	'S'/*附属任务占用*/
uint8_t			GUI_mem_pdest[GUI_MAX_MEM_NBLK];
uint8_t 		GUI_malloc_mem[GUI_MEM_NBLK][GUI_BLOCK_SIZE];
uint8_t       	GUI_mem_ctl[GUI_MEM_NBLK+1];
#define  		MSIZE_TO_NBLK(size)	((size+ GUI_BLOCK_SIZE - 1 )/ GUI_BLOCK_SIZE)
a_lock_t * 		_MemLOCK = INVALID_LOCK;
#define  ALLOC_LOCK()      os_lock(_MemLOCK);
#define  ALLOC_UNLOCK()    os_unlock(_MemLOCK);
/********************************************************************
 * @创建人 :揭成
 * @功能:申请nblk个内存块
 *
 * @输入:pMmem     pmem
 *
 *@输出: 申请的内存指针
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
	if ( nblk <= GUI_MEM_NBLK )  					/*判断NBLK的合法性*/
	{
		GUI_memset( GUI_mem_pdest,
					GUI_MEM_IS_IDLE,
					sizeof( char )*nblk );		/*将要找的内存合成字符串*/
		GUI_mem_pdest[nblk] = PNULL;				/*为字符串补一个结尾*/
		ptemp = GUI_string_locate(
					( char* )( GUI_mem_ctl ),
					( char* )GUI_mem_pdest );		/*定位字符串*/
		if ( ptemp != PNULL )
		{
			*ptemp = GUI_MEM_IS_MOCCUPY; 			/*设置优先级对应的PRO为主占*/
			GUI_memset(( uint8_t* )( ptemp + 1 ),
					   GUI_MEM_IS_SOCCUPY,
					   ( nblk - 1 ) );		 	/*其余为附属*/
			offset = ( uint8_t* )ptemp -
					 ( uint8_t* )( GUI_mem_ctl ); 	/*计算出偏移*/
			ptemp = ( char* )GUI_malloc_mem +
					(( GUI_BLOCK_SIZE ) * offset );	/*计算出空闲内存块指针头*/
			ALLOC_UNLOCK();
			if ((( long )ptemp % 2 ) == 0 )  return ptemp;	/*返回任务申请到的指针*/
			else     return	(( char* )ptemp + 1 );
		}
	}
	nblk = GUI_MEM_NBLK;
	ALLOC_UNLOCK();
	return PNULL;
}
/********************************************************************
 * @创建人 :揭成
 * @功能:释放内存块
 *
 * @输入:pMmem     pmem
 *
 *@输出: 申请的内存指针
********************************************************************/
void GUI_free( void*pmem )
{
	uint8_t	offset = 0;
	if ( pmem < ( void* )GUI_malloc_mem[0]
		 || pmem > ( void* )GUI_malloc_mem[GUI_MEM_NBLK-1] )
		return;
	ALLOC_LOCK();
	offset = (( uint8_t* )pmem - ( uint8_t* )GUI_malloc_mem )
			 / GUI_BLOCK_SIZE;							/*计算出偏移*/
	GUI_mem_ctl[offset] = GUI_MEM_IS_IDLE;					/*释放主占块*/
	offset++;
	while ( GUI_mem_ctl[offset] == GUI_MEM_IS_SOCCUPY )
	{
		GUI_mem_ctl[offset] = GUI_MEM_IS_IDLE;				/*释放附属块*/
		offset++;
	}
	ALLOC_UNLOCK();
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能:内存块管理初始化
 *
 * @输入:void
 *
 *@输出: 控制块位置
********************************************************************/
void GUI_mem_maneger_init( void )
{
	GUI_memset( GUI_mem_ctl, GUI_MEM_IS_IDLE, GUI_MAX_MEM_NBLK );
	return;
}

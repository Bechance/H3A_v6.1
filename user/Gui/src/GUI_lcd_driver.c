/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:LCD 驱动
* 创建时间:2010-07-30
* 所属公司 :科荟
* 文件名称:lcddriver.c
* 创建人 :揭成
* 功能描述:GUI 移植LCD驱动部分，该部分直接供GUI使用，原则上应用不能使用该部分API
***********************************************************************************************
修改记录:

修改日期:

修改人:

*************************************************************************************************/
#include 		"GUI_lcd_driver.h"
#include 		"../Gui.h"

#define         CMD_WRITE_X_POS    		0X4080
#define         CMD_WRITE_Y_POS    		0X4081
#define         CMD_WRITE_X_LEN    		0X4082
#define         CMD_WRITE_Y_LEN    		0X4083
#define         CMD_WRITE_DOT_COR    	0X4084
#define         CMD_WRITE_BLOCK_COR    	0x4085
#define         CMD_READ_WRITE_FLAG     0x4002
#define         CMD_READ_DOT      		0x4086
#define         CMD_READ_DOT_DATA       0x4087
#define         CMD_READ_BLOCK       	0x4088
#define         CMD_WRITE_BLOCK       	0x4089

#define         CMD_WRITE_BGCOR       	0x408A
#define         CMD_WRITE_FGCOR       	0x408B
#define         CMD_WRITE_HZASC_CMD     0x408C
#define         CMD_WRITE_HZASC_OFFSET  0x408D
#define         CMD_WRITE_HZASC_DATA    0x408E

/********************************************************************
 * @创建人 :揭成
 * @功能: LCD 底层画点函数
 *
 * @输入:none
 *
 *@输出:none
********************************************************************/
#define DELAY_4_NOP() {asm( "	nop" );asm( "	nop" );asm( "	nop" );}
static void LCD_Setdot( uint16_t x, uint16_t y, uint16_t color ) 
{
	BUS_WRITE( x, CMD_WRITE_X_POS );
	BUS_WRITE( y, CMD_WRITE_Y_POS );
	DELAY_4_NOP();
	DELAY_4_NOP();
	BUS_WRITE(( uint16_t )color, CMD_WRITE_DOT_COR );
	while ( BUS_READ( CMD_READ_WRITE_FLAG ) );
}
/********************************************************************
 * @创建人 :揭成
 * @功能: LCD 底层画点函数
 *
 * @输入:none
 *
 *@输出:none
********************************************************************/
static void LCD_Getdot( uint16_t x, uint16_t y, uint16_t* color )
{
	BUS_WRITE( x, CMD_WRITE_X_POS );
	BUS_WRITE( y, CMD_WRITE_Y_POS );
	BUS_WRITE( 0, CMD_READ_DOT );
	while ( BUS_READ( CMD_READ_WRITE_FLAG ) );
	*(( uint16_t* )color ) = ( uint16_t )BUS_READ( CMD_READ_DOT_DATA );
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :填充一个矩形框
 * @输入 :(coord_x,coord_y)起点坐标,x_len 宽度，y_len  高度
 * @输出 :NONE
********************************************************************/
static void LCD_Fillrect( uint16_t x, uint16_t y, uint16_t wide, uint16_t y_len, uint16_t color )
{
	BUS_WRITE( x, CMD_WRITE_X_POS );
	BUS_WRITE( y, CMD_WRITE_Y_POS );
	BUS_WRITE( wide, CMD_WRITE_X_LEN );
	BUS_WRITE( y_len, CMD_WRITE_Y_LEN );
	BUS_WRITE( color, CMD_WRITE_BLOCK_COR );
	while ( BUS_READ( CMD_READ_WRITE_FLAG ) );
	LCD_Setdot( x + wide - 1, y + y_len - 1, color );//由于FPGA软件的失误,补写一个点，比修改FPGA方便
	return;
}


static void LCD_drawHline( uint16_t x1, uint16_t x2, uint16_t y, uint16_t color )
{
	BUS_WRITE( x1, CMD_WRITE_X_POS );
	BUS_WRITE( y, CMD_WRITE_Y_POS );
	BUS_WRITE(( x2 - x1 ), CMD_WRITE_X_LEN );
	BUS_WRITE( 1, CMD_WRITE_Y_LEN );
	BUS_WRITE( color, CMD_WRITE_BLOCK_COR );
	while ( BUS_READ( CMD_READ_WRITE_FLAG ) );
	LCD_Setdot( x2 - 1, y, color );
	LCD_Setdot( x2, y, color );
	return;
}

static void LCD_drawVline( uint16_t x, uint16_t y1, uint16_t y2, uint16_t color )
{
	BUS_WRITE( x, CMD_WRITE_X_POS );
	BUS_WRITE( y1, CMD_WRITE_Y_POS );
	BUS_WRITE( 1, CMD_WRITE_X_LEN );
	BUS_WRITE( y2 - y1, CMD_WRITE_Y_LEN );
	BUS_WRITE( color, CMD_WRITE_BLOCK_COR );
	while ( BUS_READ( CMD_READ_WRITE_FLAG ) );
	LCD_Setdot( x, y2 - 1, color );
	return;
}

/********************************************************************
 * @创建人 :揭成
 * @功能 :FPGA 200K 内存动态分配，分成200块用来LCD缓存备份
 * @输入 :
 * @输出 :
********************************************************************/
#define   	MAX_MEM_SECTOR    		(21)
#define     MEM_SECTOR_SIZE   		(10240)
#define  	EMSIZE_TO_NBLK(size) 	((size + MEM_SECTOR_SIZE - 1 )/ MEM_SECTOR_SIZE)
uint8_t 	m_memblock_occupy[MAX_MEM_SECTOR];
uint8_t		m_memblock_pdest[MAX_MEM_SECTOR];
a_lock_t*   m_memblock_lock = INVALID_LOCK;
char* 		GUI_string_locate( char*psrc, char*psearch );

BLKmem_t  BLKmem_malloc( uint32_t size )
{
	char*ptemp;
	uint8_t offset;
	uint8_t nblk = EMSIZE_TO_NBLK( size );
	if ( !size ) return PNULL;
	if ( nblk > MAX_MEM_SECTOR ) return PNULL;
	os_lock( m_memblock_lock );
	global_memset( m_memblock_pdest, 'F', sizeof( char )*nblk );	/*将要找的内存合成字符串*/
	m_memblock_pdest[nblk] = ASCII_NULL;							/*为字符串补一个结尾*/
	ptemp = GUI_string_locate(
				( char* )m_memblock_occupy,
				( char* )m_memblock_pdest );						/*定位字符串*/
	if ( ptemp != PNULL )
	{
		*ptemp = 'M'; 												/*设置优先级对应的PRO为主占*/
		global_memset(( uint8_t* )( ptemp + 1 ), 'S',
					  ( nblk - 1 ) );		 						/*其余为附属*/
		offset = ( uint8_t* )ptemp - ( uint8_t* )( m_memblock_occupy );/*计算出偏移*/
		os_unlock( m_memblock_lock );
		return (( offset + 1 )*10);
	}
	os_unlock( m_memblock_lock );
	return 0;
}

void  BLKmem_free( BLKmem_t Blk )
{
	os_lock( m_memblock_lock );
	Blk/=10;
	Blk -= 1;
	m_memblock_occupy[Blk] = 'F';			/*释放主占块*/
	Blk++;
	while ( m_memblock_occupy[Blk] == 'S' )
	{
		m_memblock_occupy[Blk] = 'F';	    /*释放附属块*/
		Blk++;
	}
	os_unlock( m_memblock_lock );
	return;
}

static void  LCD_Getblock_Ext( uint16_t x, uint16_t y, uint16_t wide, uint16_t high, Bsize_t sector )
{
	BUS_WRITE( x, CMD_WRITE_X_POS );
	BUS_WRITE( y, CMD_WRITE_Y_POS );
	BUS_WRITE( wide, CMD_WRITE_X_LEN );
	BUS_WRITE( high, CMD_WRITE_Y_LEN );
	BUS_WRITE( sector,CMD_READ_BLOCK );
	while ( BUS_READ( CMD_READ_WRITE_FLAG ));
	return;
}

static void  LCD_Setblock_Ext( uint16_t x, uint16_t y, uint16_t wide, uint16_t high, Bsize_t sector )
{
	BUS_WRITE( x, 	   CMD_WRITE_X_POS );
	BUS_WRITE( y, 	   CMD_WRITE_Y_POS );
	BUS_WRITE( wide,   CMD_WRITE_X_LEN );
	BUS_WRITE( high,   CMD_WRITE_Y_LEN );
	BUS_WRITE( sector, CMD_WRITE_BLOCK );
	while ( BUS_READ( CMD_READ_WRITE_FLAG ) );
	return;
}

void GUI_L0_DEVReg( void )
{
	GUI_conText.LCD_draw_dot      = LCD_Setdot;
	GUI_conText.LCD_Read_dot      = LCD_Getdot;
	GUI_conText.LCD_FillRect      = LCD_Fillrect;
	GUI_conText.LCD_draw_Hline    = LCD_drawHline;
	GUI_conText.LCD_draw_Vline    = LCD_drawVline;
	GUI_conText.LCD_Getblock_Ext  = LCD_Getblock_Ext;
	GUI_conText.LCD_Setblock_Ext  = LCD_Setblock_Ext;
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: LCD驱动初始化
 * @输入:none
 * @输出:NONE
********************************************************************/
void LCD_L0_init( void )
{
	os_lock_creat( &m_memblock_lock );
	GUI_memset( m_memblock_occupy, 'F', MAX_MEM_SECTOR );
	GUI_L0_DEVReg();
	return;
}

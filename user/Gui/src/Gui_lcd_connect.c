/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:lcd 连接层
* 创建时间:  2010-07-31
* 所属公司 :科荟
* 文件名称:lcd_connetct_layer.c
* 创建人:揭成
* 功能描述:LCD操作功能函数集合，与驱动对接层
*----------------------------------------------------------------
修改记录:对显示大类加锁
修改目的:解决显示过程中花屏问题

修改日期:8月23日

修改人:
*********************************************************************************************/
#include "../Gui.h"
#include "GUI_lcd_driver.h"
/*GLOBAL*/
GUI_const_t  GUI_conText;

void GUI_set_Fgcolor( uint16_t color ) /*设置前景色*/
{
	GUI_conText.Fcolor = color;
	return;
}
void GUI_set_Bgcolor( uint16_t color ) //背景色设置程序
{
	GUI_conText.Bcolor =  color;
	return;
}

void GUI_draw_dot( uint16_t X, uint16_t Y )
{
	if ( GUI_foucus_dot_divide( X, Y ) )
		return;
	GUI_conText.LCD_draw_dot( X, Y, GUI_conText.Fcolor );
	return;
}
void GUI_clean_dot( uint16_t X, uint16_t Y )
{
	if ( GUI_foucus_dot_divide( X, Y ) )
		return;
	GUI_conText.LCD_draw_dot( X, Y, GUI_conText.Bcolor );
	return;
}

void GUI_fill_Rect( uint16_t x1, uint16_t y1, Bsize_t wide, Bsize_t high )
{
	if ( GUI_foucus_area_divide( x1, y1, wide, high ) )
		return;
	GUI_conText.LCD_FillRect( x1, y1, wide, high, GUI_conText.Bcolor );
	return;
}

void GUI_fill_Rect_At( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2 )
{
	if ( GUI_foucus_area_divide( x1, y1, x2 - x1, y2 - y1 ) )
		return;
	GUI_conText.LCD_FillRect( x1, y1, x2 - x1, y2 - y1, GUI_conText.Bcolor );
	return;
}


void GUI_read_block( int x1, int y1, int x2, int y2, void *BlockData )
{
	uint16_t xs, ys;
	Corlor_t *buf = ( Corlor_t * )BlockData;
	if ( GUI_foucus_area_divide( x1, y1, x2 - x1, y2 - y1 ) )
		return;
	GUI_LOCK();
	for ( xs = x1;xs < x2;xs++ )
	{
		for ( ys = y1;ys < y2;ys++ )
		{
			GUI_conText.LCD_Read_dot( xs, ys, buf++ );
		}
	}
	GUI_UNLOCK();
	return;
}

void GUI_write_block( int x1, int y1, int x2, int y2, void *BlockData, char sele )
{
	uint16_t xs, ys;
	uint16_t data;
	Corlor_t*buf = ( Corlor_t* )BlockData;
	if ( GUI_foucus_area_divide( x1, y1, x2 - x1, y2 - y1 ) )
		return;
	GUI_LOCK();
	for ( xs = x1;xs < x2;xs++ )
	{
		for ( ys = y1;ys < y2;ys++ )
		{
			data = *buf;
			buf++;
			if ( sele == 0 )
				GUI_conText.LCD_draw_dot( xs, ys, data );
			else
				GUI_conText.LCD_draw_dot( xs, ys, ~data );
		}
	}
	GUI_UNLOCK();
	return;
}


BLKmem_t GUI_read_block_Extp( int x1, int y1, int x2, int y2 )
{
	BLKmem_t sec;
	uint32_t size;
	size = (( uint32_t )x2 - ( uint32_t )x1 ) * (( uint32_t )y2 - ( uint32_t )y1 );
	sec = BLKmem_malloc( size );
	if ( sec )
	{
		GUI_LOCK();
		GUI_conText.LCD_Getblock_Ext( x1, y1, ( x2 - x1 ), ( y2 - y1 ), sec );
		GUI_UNLOCK();
	}
	return sec;
}

void GUI_write_block_Extp( int x1, int y1, int x2, int y2, BLKmem_t sec )
{
	GUI_LOCK();
	GUI_conText.LCD_Setblock_Ext( x1, y1, ( x2 - x1 ), ( y2 - y1 ), sec );
	GUI_UNLOCK();
	BLKmem_free( sec );
	return;
}

GUI_BLK_t* GUI_read_memBlock( int x1, int y1, int x2, int y2 )
{
     GUI_BLK_t*p = GUI_malloc(sizeof(GUI_BLK_t));
	 p->x1 = x1;p->x2 = x2;
	 p->y1 = y1;p->y2 = y2;
	 p->sec=GUI_read_block_Extp(x1,y1,x2,y2);
     return p;
}

void GUI_write_memBlock(GUI_BLK_t*p)
{
    GUI_write_block_Extp(p->x1,p->y1,p->x2,p->y2,p->sec);
	GUI_free(p);
	return;
}



void GUI_draw_Hline( int x1, int x2, int y )
{
	int xmin, xmax;
	xmin = GUI_MIN( x1, x2 );
	xmax = GUI_MAX( x1, x2 );
	if ( GUI_foucus_area_divide( xmin, y, xmax - xmin, 1 ) )
		return;
	GUI_LOCK();
	GUI_conText.LCD_draw_Hline( xmin, xmax, y, GUI_conText.Fcolor );
	GUI_UNLOCK();
	return;
}

void GUI_draw_Vline( int x, int y1, int y2 )
{
	int ymin, ymax;
	ymin = GUI_MIN( y1, y2 );
	ymax = GUI_MAX( y1, y2 );
	if ( GUI_foucus_area_divide( x, ymin, 1, ymax - ymin ) )
		return;
	GUI_LOCK();
	GUI_conText.LCD_draw_Vline( x, ymin, ymax, GUI_conText.Fcolor );
	GUI_UNLOCK();
	return;
}



static void _SwapInt( int* pa, int* pb )
{
	int t;
	t = *pa;
	*pa = *pb;
	*pb = t;
}

void GUI_draw_line( int x0, int y0, int x1, int y1 )
{
	int xdiff = x1 - x0;
	int ydiff = y1 - y0;
	int xdiffby2;
	int i;
	char Swapped = 0;
	if ( !( xdiff | ydiff ) )
	{
		GUI_draw_dot( x0, y0 );
		return;
	}
	if ( GUI_ABS( xdiff ) < GUI_ABS( ydiff ) )
	{
		_SwapInt( &xdiff, &ydiff );
		_SwapInt( &x0, &y0 );
		_SwapInt( &x1, &y1 );
		Swapped = 1;
	}
	if ( xdiff != GUI_ABS( xdiff ) )
	{
		xdiff = -xdiff;
		ydiff = -ydiff;
		_SwapInt( &x0, &x1 );
		_SwapInt( &y0, &y1 );
	}
	xdiffby2 = xdiff / 2;
	if ( ydiff < 0 )
		xdiffby2 = -xdiffby2;
	for ( i = 0; i <= xdiff; i++ )
	{
		long l = (( long )ydiff ) * i + xdiffby2;
		int y = ( GUI_ABS( l ) < 32767 ) ? ( y0 + (( int )l ) / xdiff ) : ( y0 + l / xdiff );
		if ( !Swapped )
			GUI_draw_dot( x0 + i, y );
		else
			GUI_draw_dot( y, x0 + i );
	}
	return;
}

void GUI_draw_Rect( uint16_t x1, uint16_t y1, Bsize_t wide, Bsize_t high )
{
	GUI_draw_line( x1, y1, x1, y1 + high );//|
	GUI_draw_line( x1, y1, x1 + wide, y1 );//－
	GUI_draw_line( x1 + wide, y1, x1 + wide, y1 + high );//|
	GUI_draw_line( x1, y1 + high, x1 + wide, y1 + high );//－
	return;
}

void GUI_clean_screen( void )
{
	GUI_fill_Rect( 0, 0, LCD_WIDE, LCD_HIGH );
	return;
}


void GUI_libASC_display( int x, int y, uint16_t wide, uint16_t high, char*plib )
{
	Bsize_t Hoffset = 0;
	Bsize_t Woffset = 0;
	Bsize_t Boffset = 0;
	char  cdata;
	for ( Hoffset = 0;Hoffset < high;Hoffset++ )
	{
		for ( Woffset = 0;Woffset < ( wide / 8 );Woffset++ )
		{
			cdata = plib[Hoffset*( wide/8 )+Woffset];
			for ( Boffset = 0;Boffset < 8;Boffset++ )
			{
				if ( !( cdata&( 0x80 >> Boffset ) ) )
				{
					GUI_clean_dot( x + 8*Woffset + Boffset, y + Hoffset );
				}
				else
				{
					GUI_draw_dot( x + 8*Woffset + Boffset, y + Hoffset );
				}
			}
		}
	}
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :BMP显示
 *
 * @输入 :
 *
 *@输出  :NONE
 *
********************************************************************/
void GUI_bmp_display( int x0, int y0, void*pDATA )
{
	int yPos, xPos, wide, high;
	Corlor_t Colos, pDes, pos = 0;
	uint8_t* ctemp = ( uint8_t* )pDATA;
	GUI_LOCK();
	wide = ( int )ctemp[3] * 0x100 + ctemp[2];
	high = ( int )ctemp[5] * 0x100 + ctemp[4];
	ctemp += 8;
	for ( yPos = 0;yPos < high;yPos++ )
	{
		for ( xPos = 0;xPos < wide;xPos++ )
		{
			Colos  = ( Corlor_t )ctemp[pos*2+1] * 0x100;
			pDes   = Colos + ctemp[pos*2];
			GUI_conText.LCD_draw_dot( x0 + xPos, y0 + yPos, pDes );
			pos++;
		}
	}
	GUI_UNLOCK();
	return;
}

void GUI_bmp_displayExt( int x0, int y0, void*pDATA, int sel )
{
	int yPos, xPos, wide, high;
	Corlor_t Colos, pDes, pos = 0;
	uint8_t* ctemp = ( uint8_t* )pDATA;
	GUI_LOCK();
	wide = ( int )ctemp[3] * 0x100 + ctemp[2];
	high = ( int )ctemp[5] * 0x100 + ctemp[4];
	ctemp += 8;
	for ( yPos = 0;yPos < high;yPos++ )
	{
		for ( xPos = 0;xPos < wide;xPos++ )
		{
			Colos  = ( Corlor_t )ctemp[pos*2+1] * 0x100;
			pDes   = Colos + ctemp[pos*2];
			if ( sel == 0 )
				GUI_conText.LCD_draw_dot( x0 + xPos, y0 + yPos, pDes );
			else
				GUI_conText.LCD_draw_dot( x0 + xPos, y0 + yPos, ~pDes );
			pos++;
		}
	}
	GUI_UNLOCK();
	return;
}

void GUI_lcdDEV_Reg( void )
{
	GUI_L0_DEVReg();
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能 :初始化LCD 驱动
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
void GUI_lcd_resource_req( void )
{
	LCD_L0_init();
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能 :初始化LCD 驱动
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
void GUI_lcd_resource_release( void )
{
	return;
}

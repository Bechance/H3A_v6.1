/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:lcd 连接层
* 创建时间:2013-04-01
* 所属公司:科荟
* 文件名称:GUI_circle.c
* 创建人:揭成
*********************************************************************************************/
#include "../Gui.h"
void GUI_draw_Hline( int x1, int x2, int y );
#define  CIRCLE_HLINE(x1,y,x2)  GUI_draw_Hline(x1,x2,y)

/*画一个圆*/
static  void GUI_draw8Point( uint16_t x0, uint16_t y0, uint16_t xoff, uint16_t yoff, uint16_t color )
{
	GUI_set_Fgcolor( color );
	GUI_draw_dot( x0 + xoff, y0 + yoff );
	GUI_draw_dot( x0 - xoff, y0 + yoff );
	GUI_draw_dot( x0 + yoff, y0 + xoff );
	GUI_draw_dot( x0 + yoff, y0 - xoff );
	if ( yoff )
	{
		GUI_draw_dot( x0 + xoff, y0 - yoff );
		GUI_draw_dot( x0 - xoff, y0 - yoff );
		GUI_draw_dot( x0 - yoff, y0 + xoff );
		GUI_draw_dot( x0 - yoff, y0 - xoff );
	}
	return;
}

void GUI_drawcircle( uint16_t x0, uint16_t y0, uint16_t r , uint16_t color )
{
	uint32_t i;
	uint16_t  cor;

	int imax = (( uint32_t )(( uint32_t )r * 707 ) ) / 1000 + 1;
	uint32_t sqmax = ( uint32_t )r * ( uint32_t )r + ( uint32_t )r / 2;
	uint32_t y = r;
	GUI_LOCK();
	cor = GUI_conText.Fcolor;
	GUI_draw8Point( x0, y0, r, 0, color );
	for ( i = 1; i <= imax; i++ )
	{
		if (( i*i + y*y ) > sqmax )
		{
			GUI_draw8Point( x0, y0, i, y, color );
			y--;
		}
		GUI_draw8Point( x0, y0, i, y, color );
	}
	GUI_conText.Fcolor = cor;
	GUI_UNLOCK();
	return ;
}

/*填充一个圆*/
void GUI_fillcircle( uint16_t x0, uint16_t y0, uint16_t r, uint16_t color )
{
	uint32_t i;
	int imax = (( uint32_t )(( uint32_t )r * 707 ) ) / 1000 + 1;
	uint32_t sqmax = ( uint32_t )r * ( uint32_t )r + ( uint32_t )r / 2;
	uint32_t x = r;
	uint16_t  cor;
	GUI_LOCK();
	cor = GUI_conText.Fcolor;
	GUI_set_Fgcolor( color );
	CIRCLE_HLINE( x0 - r, y0, x0 + r );
	for ( i = 1; i <= imax; i++ )
	{
		if (( i*i + x*x ) > sqmax )
		{
			if ( x > imax )
			{
				CIRCLE_HLINE( x0 - i + 1, y0 + x, x0 + i - 1 );
				CIRCLE_HLINE( x0 - i + 1, y0 - x , x0 + i - 1 );
			}
			x--;
		}
		CIRCLE_HLINE( x0 - x, y0 + i, x0 + x );
		CIRCLE_HLINE( x0 - x, y0 - i , x0 + x );
	}
	GUI_conText.Fcolor = cor;
	GUI_UNLOCK();
	return;
}

//填充画一个椭圆
static void GL_FillEllipse( int x0, int y0, int rx, int ry )
{
	int32_t OutConst, Sum, SumY;
	int x, y;
	uint32_t _rx = rx;
	uint32_t _ry = ry;
	OutConst = _rx * _rx * _ry * _ry  /* Constant as explaint above */
			   + ( _rx * _rx * _ry >> 1 ); /* To compensate for rounding */
	x = rx;
	for ( y = 0; y <= ry; y++ )
	{
		SumY = (( int32_t )( rx * rx ) ) * (( int32_t )( y * y ) ); /* Does not change in loop */
		while ( Sum = SumY + (( int32_t )( ry * ry ) ) * (( int32_t )( x * x ) ),
				( x > 0 ) && ( Sum > OutConst ) )
		{
			x--;
		}
		CIRCLE_HLINE( x0 - x, y0 + y, x0 + x );
		if ( y )CIRCLE_HLINE( x0 - x, y0 - y, x0 + x );
	}
}

void GUI_FillEllipse( int x0, int y0, int rx, int ry, uint16_t color )
{
	uint16_t  cor;
	GUI_LOCK();
	cor = GUI_conText.Fcolor;
	GUI_set_Fgcolor( color );
	GL_FillEllipse( x0, y0, rx, ry );
	GUI_conText.Fcolor = cor;
	GUI_UNLOCK();
	return;
}


void GL_DrawEllipse( int x0, int y0, int rx, int ry )
{
	int32_t OutConst, Sum, SumY;
	int x, y;
	int xOld;
	uint32_t _rx = rx;
	uint32_t _ry = ry;
	OutConst = _rx * _rx * _ry * _ry  /* Constant as explaint above */
			   + ( _rx * _rx * _ry >> 1 ); /* To compensate for rounding */
	xOld = x = rx;
	for ( y = 0; y <= ry; y++ )
	{
		if ( y == ry )
		{
			x = 0;
		}
		else
		{
			SumY = (( int32_t )( rx * rx ) ) * (( int32_t )( y * y ) ); /* Does not change in loop */
			while ( Sum = SumY + (( int32_t )( ry * ry ) ) * (( int32_t )( x * x ) ),
					( x > 0 ) && ( Sum > OutConst ) ) x--;
		}
		if ( y )
		{
			GUI_draw_line( x0 - xOld, y0 - y + 1, x0 - x, y0 - y );
			GUI_draw_line( x0 - xOld, y0 + y - 1, x0 - x, y0 + y );
			GUI_draw_line( x0 + xOld, y0 - y + 1, x0 + x, y0 - y );
			GUI_draw_line( x0 + xOld, y0 + y - 1, x0 + x, y0 + y );
		}
		xOld = x;
	}
}

void GUI_DrawEllipse( int x0, int y0, int rx, int ry, uint16_t color )
{
	uint16_t  cor;
	GUI_LOCK();
	cor = GUI_conText.Fcolor;
	GUI_set_Fgcolor( color );
	GL_DrawEllipse( x0, y0, rx, ry );
	GUI_conText.Fcolor = cor;
	GUI_UNLOCK();
	return;
}

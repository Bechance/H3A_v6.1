/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:GUI多边型绘制
* 创建时间:2013-03-11
* 所属公司 :科荟
* 文件名称:Gui_polygon.c
* 创建人 :揭成
* 功能描述:多边形绘制处理
*----------------------------------------------------------------
修改记录:
修改日期:
修改人:
********************************************************************/
#include  <math.h>
#include "../Gui.h"


void GUI_draw_Hline( int x1, int x2, int y );
#define  POLYGON_HLINE(x1,x2,y)  GUI_draw_Hline(x1,x2,y)

#define  MAX_BORD_NUM      15   //最大边数支持


static int m_ay[MAX_BORD_NUM];

#define LODE_XY(point,x,y){point.x = point.x+x;point.y = point.y+y;}


static int _GET_x0( GUI_POINT*next, GUI_POINT*cUR, int y0 )
{
	float k, b;
	int x0;
	if ( next->x != cUR->x )
	{
		k = ( next->y - cUR->y );
		k = k / ( next->x - cUR->x );
		b = cUR->y - k * cUR->x;
		x0 = ( int )((( float )y0 - b ) / k );
	}
	else
	{
		x0 = next->x;
	}
	return x0;
}

static void GL_sort_x( int*p, int size )
{
	int xpos, ypos;
	int itemp;
	for ( xpos = 0;xpos < size;xpos++ )
	{
		for ( ypos = xpos + 1;ypos < size;ypos++ )
		{
			if ( p[ypos] < p[xpos] )
			{
				itemp   = p[xpos];
				p[xpos] = p[ypos];
				p[ypos] = itemp	;
			}
		}
	}
	return;
}
static int GUI_Polygon_cross_obtain(
	const GUI_POINT*p,
	int size, int x, int y, int y0
)
{
	int pos, xpos = 0, n, flags;
	int xmax, xmin, ymax, ymin;
	int x0;
	GUI_POINT p1, p2;
	for ( pos = 0;pos < size;pos++ )
	{
		if ( pos != size - 1 )
		{
			p1 = p[pos];
			p2 = p[pos+1];
		}
		else
		{
			p1 = p[pos];
			p2 = p[0];
		}
		LODE_XY( p1, x, y );
		LODE_XY( p2, x, y );
		if ( y0 == p1.y )x0 = p1.x;
		else if ( y0 == p2.y )x0 = p2.x;
		else x0 = _GET_x0( &p1, &p2, y0 );
		xmin = GUI_MIN( p1.x, p2.x );
		xmax = GUI_MAX( p1.x, p2.x );
		ymin = GUI_MIN( p1.y, p2.y );
		ymax = GUI_MAX( p1.y, p2.y );
		if ( x0 < xmin )continue;
		if ( x0 > xmax )continue;
		if ( y0 < ymin )continue;
		if ( y0 > ymax )continue;
		flags = 1;
		for ( n = 0;n < xpos;n++ )
		{
			if ( m_ay[n] == x0 )
			{
				flags = 0;
				break;
			}
		}
		if ( flags )m_ay[xpos++] = x0;
	}
	if ( xpos > 1 )GL_sort_x( m_ay, xpos );
	if ( xpos % 2 != 0 )xpos = xpos - 1;
	return xpos;
}




void GL_FillPolygon( int x, int y, const GUI_POINT*p, int size )
{
	int ypos, pos, ymax = 0, ymin = LCD_HIGH, n;
	GUI_POINT cUR;
	for ( pos = 0; pos < size; pos++ )
	{
		cUR = p[pos];
		LODE_XY( cUR, x, y );
		ymin = GUI_MIN( cUR.y, ymin );
		ymax = GUI_MAX( cUR.y, ymax );
	}
	for ( ypos = ymin;ypos <= ymax;ypos++ )
	{
		n = GUI_Polygon_cross_obtain( p, size, x, y, ypos );
		for ( pos = 0;pos < n;pos += 2 )
		{
			POLYGON_HLINE( m_ay[pos], m_ay[pos+1], ypos );
		}
	}
	return;
}


void GUI_polygon_draw( int x, int y, const GUI_POINT*p, int size, uint16_t color )
{
	int pos;
	uint16_t  cor;
	GUI_POINT next, cUR;
	GUI_LOCK();
	cor = GUI_conText.Fcolor;
	GUI_set_Fgcolor( color );
	cUR = p[0];
	LODE_XY( cUR, x, y );
	for ( pos = 1; pos < size; pos++ )
	{
		next = p[pos];
		LODE_XY( next, x, y );
		GUI_draw_line( cUR.x, cUR.y, next.x, next.y );
		cUR = next;
	}
	next = p[0];
	LODE_XY( next, x, y );
	GUI_draw_line( cUR.x, cUR.y, next.x, next.y );
	GUI_conText.Fcolor = cor;
	GUI_UNLOCK();
	return;
}

void GUI_polygon_fill( int x, int y, const GUI_POINT*p, int size, uint16_t color )
{
	uint16_t  cor;
	GUI_LOCK();
	cor = GUI_conText.Fcolor;
	GUI_set_Fgcolor( color );
	GL_FillPolygon( x, y, p, size );
	GUI_conText.Fcolor = cor;
	GUI_UNLOCK();
	return;
}


void GUI_polygon_rotate( GUI_POINT* pDest, const GUI_POINT* pSrc, int size, float Angle )
{
	int pos;
	float fangle = Angle * 3.1415926 / 180;
	float fcos = cos( fangle );
	float fsin = sin( fangle );
	for ( pos = 0; pos < size; pos++ )
	{
		int x = ( pSrc + pos )->x;
		int y = ( pSrc + pos )->y;
		( pDest + pos )->x =  x * fcos + y * fsin;
		( pDest + pos )->y = -x * fsin + y * fcos;
	}
	return;
}

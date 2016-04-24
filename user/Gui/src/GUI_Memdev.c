/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:GUI����ͻ���
* ����ʱ��:2013-03-11
* ������˾ :����
* �ļ�����:Gui_memdev.c
* ������ :�ҳ�
* �洢���豸
*----------------------------------------------------------------*/
#include	"../GUI.h"
#include 	"GUI_lcd_driver.h"

typedef struct
{
	void*pmem;
	BOOL busy;
	int x;
	int y;
	int wide;
	int high;
	uint32_t size;
}GUI_Memdev_t;

GUI_Memdev_t  	m_GUI_onlydev;
void 			GUI_lcdDEV_Reg( void );


//�洢ģʽ��������
static void GUI_Memdev_drawdot( uint16_t x, uint16_t y, uint16_t color )
{
	GUI_Memdev_t*pdev = &m_GUI_onlydev;
	uint16_t*pmem;
	uint32_t pos;
	pmem = ( uint16_t* )pdev->pmem;
	pos = (( uint32_t )( y - pdev->y ) ) * pdev->wide + ( x - pdev->x );
	if ( pos <= pdev->size )
		pmem[pos] = color;
	return;
}

//�洢ģʽ��ֱ�����
static void GUI_Memdev_drawHline( uint16_t x1, uint16_t x2, uint16_t y, uint16_t color )
{
	int xs, xmin, xmax;
	GUI_Memdev_t*pdev = &m_GUI_onlydev;
	uint16_t*pmem;
	uint32_t pos;
	xmin = GUI_MIN( x1, x2 );
	xmax = GUI_MAX( x1, x2 );
	pmem = ( uint16_t* )pdev->pmem;
	pos  = (( uint32_t )( y - pdev->y ) ) * pdev->wide + ( xmin - pdev->x );
	for ( xs = xmin;xs < xmax;xs++ )
	{
		if ( pos <= pdev->size ) 
			pmem[pos++] = color;
	}
	return;
}
static void GUI_Memdev_drawVline( uint16_t x, uint16_t y1, uint16_t y2, uint16_t color )
{
	int ys, ymin, ymax;
	ymin = GUI_MIN( y1, y2 );
	ymax = GUI_MAX( y1, y2 );
	for ( ys = ymin;ys < ymax;ys++ )
	{
		GUI_Memdev_drawdot( x, ys, color );
	}
}


//�洢ģʽ���������
static void GUI_Memdev_Fill( uint16_t x, uint16_t y, uint16_t wide, uint16_t high, uint16_t color )
{
	int x0, y0, x1, y1;
	int yc;
	x0 = x;
	x1 = x + wide;
	y0 = y;
	y1 = y + high;
	for ( yc = y0; yc <= y1; yc++ )
	{
		GUI_Memdev_drawHline( x0, x1, yc, color );
	}
	return;
}


//�洢ģʽ������ע��
static void GUI_Memdev_Reg( void )
{
	GUI_conText.LCD_draw_dot   = GUI_Memdev_drawdot;
	GUI_conText.LCD_FillRect   = GUI_Memdev_Fill;
	GUI_conText.LCD_draw_Hline = GUI_Memdev_drawHline;
	GUI_conText.LCD_draw_Vline = GUI_Memdev_drawVline;
	return;
}


//�洢ģʽ���ݵ���LCD
void GUI_Memdev_toLCD( void )
{
	int xs, ys;
	uint32_t pos;
	GUI_Memdev_t*pdev = &m_GUI_onlydev;
	uint16_t*pmem;
	GUI_LOCK();
	GUI_lcdDEV_Reg();
	pmem = ( uint16_t* )pdev->pmem;
	for ( ys = pdev->y;ys < pdev->y + pdev->high;ys++ )
	{
		for ( xs = pdev->x;xs < pdev->x + pdev->wide;xs++ )
		{
			pos = (( uint32_t )( ys - pdev->y ) ) * pdev->wide + ( xs - pdev->x );
			GUI_conText.LCD_draw_dot( xs, ys, pmem[pos] );
		}
	}
	GUI_Memdev_Reg();
	GUI_UNLOCK();
	return;
}
//�洢ģʽ����
void GUI_Memdev_create( int x, int y, int wide, int high, void*pmem, uint32_t size )
{
	GUI_Memdev_t*pdev = &m_GUI_onlydev;
	if ( pdev->busy ) return;
	pdev->busy = 1;
	pdev->pmem = pmem;
	pdev->size = size;
	pdev->x   = x;
	pdev->y   = y;
	pdev->high = high;
	pdev->wide = wide;
	GUI_Memdev_Reg();
	return;
}
//�洢ģʽȡ��
void GUI_Memdev_destroy( void )
{
	GUI_lcdDEV_Reg();
	m_GUI_onlydev.busy = 0;
	return;
}

void GUI_Memdev_init( void )
{
	m_GUI_onlydev.busy = 0;
	return;
}


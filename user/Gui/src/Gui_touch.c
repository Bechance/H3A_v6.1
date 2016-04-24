/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:GUI 触摸屏管理
* 创建时间:2010-10-11
* 所属公司 :科荟
* 文件名称:Gui_touch.c
* 创建人 :揭成
* 功能描述:触摸数据处理，屏幕校正
*----------------------------------------------------------------
修改记录:

修改日期:

修改人:
********************************************************************/
#include "../Gui.h"
#include "../../Fatfs\FatFs.h"
#include 	"GUI_lcd_driver.h"
#if GUI_TOUCH_EN >0
touch_scale_t 	          		   m_touch_scale;
dev_id_t 			      		   m_touch_dev_id = INVALID_DEV_ID;
a_lock_t*                          m_touch_read_lock = INVALID_LOCK;
#define   CORRECT_ERROR            100
#define   CORRECT_FILE             "Stdata"
/********************************************************************
 * @创建人:揭成
 * @功能 :保存触摸系数，该系数应该存放在固定存储
 *                 区，如FLASH  ，EPPROM 等
 * @输入 :
 *
 * @输出 : NONE
********************************************************************/
static  void GUI_touch_scale_save( void )
{
	FRESULT ret;
	UINT  w;
	FIL  *file = fatfs_FIobj_obtain();
	if ( file )
	{
		ret = f_open( file, CORRECT_FILE, FA_OPEN_ALWAYS | FA_WRITE );
		if ( ret == FR_OK )
		{
			fatfs_write_bytes( file, &m_touch_scale, sizeof( touch_scale_t ), &w );
			f_close( file );
		}
		fatfs_FIobj_put( file );
	}
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能 :导入触摸系数，该系数应该存放在固定存储
 *                 区，如FLASH  ，EPPROM 等
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
static  void GUI_touch_scale_loade( void )
{
	FRESULT ret;
	UINT r;
	FIL *file = fatfs_FIobj_obtain();
	if ( file )
	{
		ret = f_open( file, CORRECT_FILE, FA_OPEN_EXISTING | FA_READ );
		if ( ret != FR_OK )
		{
			GUI_FREE( file );
			m_touch_scale.Kx     = 0.31875; //(255.0/800.0)
			m_touch_scale.Ky     = 0.425;   //(255.0/600.0)
			m_touch_scale.lcdx   = 0;
			m_touch_scale.lcdy   = 0;
			m_touch_scale.touchx = 0;
			m_touch_scale.touchy = 0;
		}
		else
		{
			fatfs_read_bytes( file, &m_touch_scale, sizeof( touch_scale_t ), &r );
			f_close( file );
		}
	}
	fatfs_FIobj_put( file );
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能 :将触摸数据转换成坐标
 * @输入 :
 *
 * @输出 : NONE
********************************************************************/
static void GUI_touch_data_match( app_touch_t*pdata )
{
	int touchx = pdata->x;
	int touchy = pdata->y;
	pdata->x = ( float )( touchx - m_touch_scale.touchx )
			   / ( float ) m_touch_scale.Kx + m_touch_scale.lcdx;
	pdata->y = ( float )( touchy - m_touch_scale.touchy )
			   / ( float )m_touch_scale.Ky + m_touch_scale.lcdy;
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能 :计算矫正后的触摸系数
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
static  BOOL GUI_touch_scale_count( void*pmem )
{
	Bsize_t err;
	touch_coord_t*pdata = ( touch_coord_t* )pmem;
	touch_scale_t*pshadow ;
	float Kt, x, y;
	err = GLOBAL_ABS( pdata[0].touchx - pdata[2].touchx );
	if ( err > 10 ) return FAIL;
	err = GLOBAL_ABS( pdata[2].touchy - pdata[3].touchy );
	if ( err > 10 ) return FAIL;
	pshadow = GUI_MALLOC( sizeof( touch_scale_t ) );
	global_memcpy( pshadow, &m_touch_scale, sizeof( touch_scale_t ) );
	m_touch_scale.Kx = ( float )( pdata[1].touchx - pdata[0].touchx )
					   / ( float )( pdata[1].lcdx - pdata[0].lcdx );
	Kt = ( float )( pdata[2].touchx - pdata[3].touchx )
		 / ( float )( pdata[2].lcdx - pdata[3].lcdx );
	m_touch_scale.Kx = ( float )( m_touch_scale.Kx + Kt ) / 2;

	m_touch_scale.Ky = ( float )( pdata[0].touchy - pdata[2].touchy )
					   / ( float )( pdata[0].lcdy - pdata[2].lcdy );
	Kt = ( float )( pdata[1].touchy - pdata[3].touchy )
		 / ( float )( pdata[1].lcdy - pdata[3].lcdy );
	m_touch_scale.Ky = ( float )( m_touch_scale.Ky + Kt ) / 2;

	x = ( float )( pdata[4].touchx - pdata[2].touchx )
		/ ( float )m_touch_scale.Kx + pdata[2].lcdx;
	y = ( float )( pdata[4].touchy - pdata[2].touchy )
		/ ( float )m_touch_scale.Ky + pdata[2].lcdy;
	err = GLOBAL_ABS( pdata[4].lcdx - x );
	if ( err > 10 )
	{
		global_memcpy( &m_touch_scale, pshadow, sizeof( touch_scale_t ) );
		GUI_FREE( pshadow );
		return FAIL;
	}
	err = GLOBAL_ABS( pdata[4].lcdy - y );
	if ( err > 10 )
	{
		global_memcpy( &m_touch_scale, pshadow, sizeof( touch_scale_t ) );
		GUI_FREE( pshadow );
		return FAIL;
	}
	m_touch_scale.lcdx = pdata[2].lcdx;
	m_touch_scale.lcdy = pdata[2].lcdy;
	m_touch_scale.touchx = pdata[2].touchx;
	m_touch_scale.touchy = pdata[2].touchy;
	GUI_touch_scale_save();
	return TURE;
}
/********************************************************************
 * @创建人:揭成
 * @功 能 :GUI 读触摸数据
 * @输 入 :
 * @输 出 : NONE
********************************************************************/
Bsize_t GUI_touch_read( void*pdata, Bsize_t size )
{
	Bsize_t ret;
	ret = os_driver_read( m_touch_dev_id, ( uint8_t* )pdata, size, 0 );
	if ( ret == size )
	{
		GUI_touch_data_match( pdata );
	}
	return ret;
}
/********************************************************************
 * @创建人:揭成
 * @功能 :一直等待直到有数据为止
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
static void GUI_touch_wait_data( app_touch_t*pdata )
{
	while ( 1 )
	{
		if ( os_driver_read( m_touch_dev_id,
							 ( uint8_t* )pdata,
							 sizeof( app_touch_t ), 0 )
			 == sizeof( app_touch_t ) ) break;
		msleep( 1 );
	}
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能 :触摸屏真实数据误差计算
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
static BOOL GUI_touch_error_match( app_touch_t*pdata, hwin_t*hp )
{
	int xmax = hp->win_x + CORRECT_ERROR;
	int xmin  = hp->win_x - CORRECT_ERROR;
	int ymax = hp->win_y + CORRECT_ERROR;
	int ymin  = hp->win_y - CORRECT_ERROR;
	( xmax > GUI_LCD_WIDE ) ? ( xmax = GUI_LCD_WIDE ) : ( xmax = xmax );
	( xmin < 0 ) ? ( xmin = 0 ) : ( xmin = xmin );
	( ymax > GUI_LCD_HIGH ) ? ( ymax = GUI_LCD_HIGH ) : ( ymax = ymax );
	( ymin < 0 ) ? ( ymin = 0 ) : ( ymin = ymin );
	GUI_touch_data_match( pdata );
	if ( pdata->x > xmax ) return FAIL;
	if ( pdata->x < xmin )  return FAIL;
	if ( pdata->y > ymax ) return FAIL;
	if ( pdata->y < ymin )  return FAIL;
	return  TURE;
}
#if  GUI_WIDGET_FORM_EN <1
/********************************************************************
 * @创建人: 揭成
 * @功能: 创建一个网格控件
 *
 * @输入:(xcoord  ycoord)起始坐标,parg 网格子参数
 :			nline 网格行数
 *               nrow 网格列数，wide 总宽  high  总高
 *               fatherhp  所属的父窗口
 * @输出:控件句柄
********************************************************************/
hwin_t* GUI_form_creat( int xcoord,
						int 	ycoord,
						Bsize_t nline,
						Bsize_t nrow,
						Bsize_t grid_wide,
						Bsize_t grid_high,
						hwin_t* fatherhp )
{
	hwin_t*hp;
	Bsize_t offset;
	Bsize_t dy;
	Bsize_t dx;
	Bsize_t high  = grid_high * nline;
	Bsize_t wide = grid_wide * nrow;
	hp = GUI_window_child_creat(
			 xcoord, ycoord, wide,
			 high, 0, fatherhp );
	if ( !hp ) return PNULL;
	GUI_window_hwin_req( hp );
	GUI_set_Fgcolor( FORM_FRAME_COLOR );
	GUI_set_Bgcolor( FORM_BG_COLOR );
	xcoord = hp->win_x;
	ycoord = hp->win_y;
	grid_high = hp->win_high / nline;
	grid_wide = hp->win_wide / nrow;
	for ( offset = 0;offset < nline + 1;offset++ )
	{
		dy = ycoord + offset * grid_high;
		dx = xcoord + nrow * grid_wide;
		GUI_draw_line( xcoord, dy, dx, dy );
	}
	for ( offset = 0;offset < nrow + 1;offset++ )
	{
		dy = ycoord + nline * grid_high;
		dx = xcoord + offset * grid_wide;
		GUI_draw_line( dx, ycoord, dx, dy );
	}
	GUI_window_hwin_release( hp );
	return hp;
}
#endif
/********************************************************************
 * @创建人:揭成
 * @功能 :将触摸屏校正
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
#define  TLINE_LEN    20
void GUI_touch_correct( void )
{
	app_touch_t shadow;
	hwin_t*hp;
	hwin_t*formhwin[5] = {0, };
	Bsize_t offset;
	Bsize_t recnt = 0;
	touch_coord_t *pmem = ( touch_coord_t* )GUI_MALLOC( 200 );
	if ( !pmem ) return;
	GUI_set_Bgcolor( C_GUI_HIGH_BLUE );
	GUI_clean_screen();
	hp = GUI_window_creat( 5, 5, LCD_WIDE - 8, LCD_HIGH - 20, WIN_PAINT );
	if ( !hp )
	{
		GUI_FREE( pmem );
		return;
	}
	f_unlink( CORRECT_FILE );
	GUI_touch_scale_loade();
	os_lock( m_touch_read_lock );
	GUI_window_hwin_req( hp );
	GUI_set_Bgcolor( C_GUI_BlACK );
	GUI_set_Fgcolor( C_GUI_WHITE );
	formhwin[0] = GUI_form_creat( 0, 0, 2, 2, 20, 20, hp );
	formhwin[1] = GUI_form_creat( GET_XBORDER( hp ) - 45, 0, 2, 2, 20, 20, hp );
	formhwin[2] = GUI_form_creat( 0, GET_YBORDER( hp ) - 45, 2, 2, 20, 20, hp );
	formhwin[3] = GUI_form_creat( GET_XBORDER( hp ) - 45, GET_YBORDER( hp ) - 45, 2, 2, 20, 20, hp );
	formhwin[4] = GUI_form_creat( hp->win_wide / 2 - 20, hp->win_high / 2 - 20, 2, 2, 20, 20, hp );
	while ( 1 )
	{
		offset = 0;
		while ( offset < 5 )
		{
			GUI_touch_wait_data( &shadow );
			pmem[offset].touchx = shadow.x;
			pmem[offset].touchy = shadow.y;
			if ( !GUI_touch_error_match( &shadow, formhwin[offset] ) )
			{
				//		GUI_string_display_At( 100, 50, "误差太大" );
				if ( ++recnt < 8 ) continue;
				else
				{
					GUI_window_hwin_release( hp );
					GUI_window_destory( hp );
					GUI_FREE( pmem );
					os_unlock( m_touch_read_lock );
					return;
				}
			}
			recnt = 0;
			pmem[offset].lcdx	= formhwin[offset]->win_x + 20;
			pmem[offset].lcdy = formhwin[offset]->win_y + 20;
			GUI_window_hwin_req( formhwin[offset] );
			GUI_set_Fgcolor( C_GUI_RED );
			GUI_WINDIS_HLINE( 0, formhwin[offset]->win_wide, formhwin[offset]->win_high / 2 );
			GUI_WINDIS_VLINE( formhwin[offset]->win_wide / 2, 0, formhwin[offset]->win_high );
			GUI_window_hwin_release( formhwin[offset] );
			offset++;
		}
		if ( !GUI_touch_scale_count( pmem ) )
		{
			for ( offset = 0;offset < 5;offset++ )
			{
				GUI_window_hwin_req( formhwin[offset] );
				GUI_set_Fgcolor( C_GUI_WHITE );
				GUI_WINDIS_HLINE( 0, formhwin[offset]->win_wide, formhwin[offset]->win_high / 2 );
				GUI_WINDIS_VLINE( formhwin[offset]->win_wide / 2, 0, formhwin[offset]->win_high );
				GUI_window_hwin_release( formhwin[offset] );
			}
			continue;
		}
		break;
	}
	GUI_set_Fgcolor( C_GUI_WHITE );
	GUI_set_Bgcolor( C_GUI_HIGH_BLUE );
	GUI_string_display_At( 100, 100, "修正完成" );
	GUI_window_hwin_release( hp );
	GUI_window_destory( hp );
	GUI_FREE( pmem );
	os_unlock( m_touch_read_lock );
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能 :初始化触摸驱动
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
void GUI_touch_resource_req( void )
{
	os_lock_creat( &m_touch_read_lock );
	m_touch_dev_id = touch_driver_register();
	if ( m_touch_dev_id != INVALID_DEV_ID )
	{
		os_driver_init( m_touch_dev_id );
	}
	GUI_touch_scale_loade();
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能 :释放触摸驱动资源
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
void GUI_touch_resource_release( void )
{
	os_driver_uninstall( m_touch_dev_id );
	m_touch_dev_id = INVALID_DEV_ID;
	return;
}
#else
void GUI_touch_correct( void )
{
	return;
}
#endif

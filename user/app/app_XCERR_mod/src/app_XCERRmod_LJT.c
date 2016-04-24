/********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:绘制六角图
* 创建时间:2010-11-18
* 所属公司 :科荟
* 文件名称:app_startmod_LJT.c
* 创建人 :揭成
* 功能描述:根据电参数绘制六角图
******************************************************************************************/
#include    "app_XCerrmod_inside.h"
static 		uint16_t  _x0;		/*圆点X坐标*/
static 		uint16_t  _y0;		/*圆点Y坐标*/
static      uint16_t  _Bg 	   		= 0X10A3;  
static      uint16_t  _Fg 	   		= 0X2145;  
static      uint16_t  _lenMAX  		= 110; 
static      uint16_t  _lenMID  		= 70; 
static      int 	  _Rcircle;
static      float  	  _angle_FIFO[6];
static      Bsize_t   _timecnt;


#define     ANGLE_ERR          			2
#define  	LJT_BG_COLOR             	_Bg
#define  	LJT_A_FG_COLOR         		C_GUI_DLA_COLOR
#define  	LJT_B_FG_COLOR         		C_GUI_DLB_COLOR
#define  	LJT_C_FG_COLOR         		C_GUI_DLC_COLOR
#define   	LJT_BASE_FG_COLOR       	_Fg
#define   	LJT_MAX_LEN                 _lenMAX
#define   	LJT_MID_LEN                 _lenMID
#define     LJT_LINE(angle,len,str)     GUI_window_angle_line_display(_x0,_y0,angle,len,1,str);
#define     LJT_BASE_DRAW_ABC()         {GUI_set_Fgcolor( LJT_BASE_FG_COLOR);LJT_LINE(90,LJT_MAX_LEN,"Ua");LJT_LINE(90+120,LJT_MAX_LEN,"Uc");LJT_LINE(90+240,LJT_MAX_LEN,"Ub");}
#define     LJT_BASE_DRAW_CBA()         {GUI_set_Fgcolor( LJT_BASE_FG_COLOR);LJT_LINE(90,LJT_MAX_LEN,"Uc");LJT_LINE(90+120,LJT_MAX_LEN,"Ua");LJT_LINE(90+240,LJT_MAX_LEN,"Ub");}
/********************************************************************
 * @创建人 :揭成
 * @功能 :六角图成员显示
 * @输入 :
 * @输出 :NONE
********************************************************************/
static void _member_display( float angle, Bsize_t len, uint16_t color, char*title )
{
	GUI_set_Bgcolor( LJT_BG_COLOR );
	GUI_set_Fgcolor( color );
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	angle=(angle<0)?(angle+360):(angle);
	GUI_window_angle_line_display(_x0, _y0, angle, len, 1, title );
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :分析角度是否改变
 *
 * @输入 :
 *
 * @输出 : NONE
********************************************************************/
static BOOL _angle_sys( measure_param_t*Pdl )
{
	Bsize_t offset = 0;
	if ( ++_timecnt % 10 == 0 )
		return TURE;
	if ( GLOBAL_ABS( _angle_FIFO[offset] - Pdl->phase_a.f_J ) > ANGLE_ERR )
	{
		_angle_FIFO[offset] = Pdl->phase_a.f_J;
		return TURE;
	}
	offset++;
	if ( GLOBAL_ABS( _angle_FIFO[offset] - Pdl->phase_b.f_J ) > ANGLE_ERR )
	{
		_angle_FIFO[offset] = Pdl->phase_b.f_J;
		return TURE;
	}
	offset++;
	if ( GLOBAL_ABS( _angle_FIFO[offset] - Pdl->phase_c.f_J ) > ANGLE_ERR )
	{
		_angle_FIFO[offset] = Pdl->phase_c.f_J;
		return TURE;
	}
	offset++;
	if ( GLOBAL_ABS( _angle_FIFO[offset] - Pdl->f_JUab ) > ANGLE_ERR )
	{
		_angle_FIFO[offset] = Pdl->f_JUab;
		return TURE;
	}
	offset++;
	if ( GLOBAL_ABS( _angle_FIFO[offset] - Pdl->f_JUac ) > ANGLE_ERR )
	{
		_angle_FIFO[offset] = Pdl->f_JUac;
		return TURE;
	}
	offset++;
	if ( GLOBAL_ABS( _angle_FIFO[offset] - Pdl->f_JUbc ) > ANGLE_ERR )
	{
		_angle_FIFO[offset] = Pdl->f_JUbc;
		return TURE;
	}
	return  FAIL;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :六角图显示
 *
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
void  app_LJT_display( hwin_t*hp, measure_param_t*Pdl,DL_flag_t*Pdl_flag,int flag)
{
	float angle_Ua;
	float angle_Ub;
	float angle_Uc;
	float angle_I;
	uint8_t phase3or4;
	phase3or4 = Pdl_flag->phase3or4;
	if(flag==1)
	{
		if ( !_angle_sys( Pdl ) )return;
	}
	GUI_window_hwin_req( hp );
	GUI_set_Bgcolor( LJT_BG_COLOR );
	GUI_clean_window();
	GUI_drawcircle(hp->win_x+_x0,hp->win_y+_y0,_lenMAX,_Fg);
	GUI_drawcircle(hp->win_x+_x0,hp->win_y+_y0,_lenMID,_Fg);
	if ( phase3or4 == JXMOD_3_3 )
	{
		if ( Pdl->f_JUac < 0 )
		{
			LJT_BASE_DRAW_ABC();
			angle_Ua = 120;
		}
		else
		{
			LJT_BASE_DRAW_CBA();
			angle_Ua = 120 + 59;
		}
		if ( Pdl->phase_a.f_U > 0 )
			_member_display( angle_Ua,LJT_MAX_LEN,LJT_A_FG_COLOR, "Uab" );
		angle_I = angle_Ua - Pdl->phase_a.f_J;
		if ( Pdl->phase_a.f_I > 0 )
			_member_display( angle_I,LJT_MID_LEN,LJT_A_FG_COLOR, "Ia" );

		angle_Uc = angle_Ua - Pdl->f_JUac;
		if ( Pdl->phase_c.f_U > 0 )
			_member_display( angle_Uc,LJT_MAX_LEN,LJT_C_FG_COLOR, "Ucb" );
		angle_I = angle_Uc - Pdl->phase_c.f_J;
		if ( Pdl->phase_c.f_I > 0 ) 
			_member_display( angle_I,LJT_MID_LEN,LJT_C_FG_COLOR, "Ic" );
	}
	else 
	{
		angle_Ua = 90;
		if ( Pdl->phase_a.f_U > 0 )
			_member_display( angle_Ua, LJT_MAX_LEN, LJT_A_FG_COLOR, "Ua" );
		angle_I = angle_Ua - Pdl->phase_a.f_J;
		if ( Pdl->phase_a.f_I > 0 )
			_member_display( angle_I, LJT_MID_LEN, LJT_A_FG_COLOR, "Ia" );

		angle_Ub = angle_Ua - Pdl->f_JUab;
		if ( Pdl->phase_b.f_U > 0 )
			_member_display( angle_Ub, LJT_MAX_LEN, LJT_B_FG_COLOR, "Ub" );
		angle_I = angle_Ub - Pdl->phase_b.f_J;
		if ( Pdl->phase_b.f_I > 0 )
			_member_display( angle_I, LJT_MID_LEN, LJT_B_FG_COLOR, "Ib" );

		angle_Uc = angle_Ua - Pdl->f_JUac;
		if ( Pdl->phase_c.f_U > 0 )
			_member_display( angle_Uc, LJT_MAX_LEN, LJT_C_FG_COLOR, "Uc" );
		angle_I = angle_Uc - Pdl->phase_c.f_J;
		if ( Pdl->phase_c.f_I > 0 )
			_member_display( angle_I, LJT_MID_LEN, LJT_C_FG_COLOR, "Ic" );
	}
	GUI_window_hwin_release( hp );
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :六角图参数初始化
 *
 * @输入 :hp  六角图显示框句柄
 *
 *@输出 : NONE
********************************************************************/
void app_LJT_init( hwin_t*hp)
{
    float flen;
	GUI_window_hwin_req(hp);
	_angle_FIFO[1] = 1;
	_angle_FIFO[0] = 1;
	_Bg = GUI_GetwinBgcor();
	_Fg = 0X2145; 
	_timecnt   = 95;
	_lenMAX  = (MIN(hp->win_high,hp->win_wide))/2-5;
	_x0      = ( hp->win_wide ) / 2; /*计算出圆心*/
	_y0      = ( hp->win_high ) / 2;
    flen =  (float)(MIN(hp->win_wide,hp->win_high));
	flen =  flen*2/6;
	_lenMID = (uint16_t)flen;
	GUI_drawcircle(hp->win_x+_x0,hp->win_y+_y0,_lenMAX,_Fg);
	GUI_drawcircle(hp->win_x+_x0,hp->win_y+_y0,_lenMID,_Fg);
	
	GUI_window_hwin_release(hp);
	return;
}

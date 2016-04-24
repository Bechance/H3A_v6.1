/********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:绘制六角图
* 创建时间:2010-11-18
* 所属公司 :科荟
* 文件名称:app_Esummod_DL.c
* 创建人 :揭成
* 功能描述:测量电参数
******************************************************************************************/
#include			"app_Esummod_inside.h"
Esum_arg_t          m_Esum_arg;
static uint32_t		_energy_s 		= 0;
static uint32_t		_energy_e 		= 0;
static float		_energy_2real 	= 0.00000000;
static float		_energy_1real 	= 0.00000000;
static const float 	_const_energy 	= 72000.0;		//电能常数
static int          _view_cnt       = 0;

#define         	PULSE_GET(param)	{param = READ32BIT_DATA(0x42b8);}
static 				time_string_t 	_stime;
static				time_string_t 	_etime;
/********************************************************************
 * @作者 :周炳权
 * @功能 :测量电能计算
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
static void _energy_calc( void )
{
	PULSE_GET( _energy_e );
	if ( _energy_e > _energy_s)
	{
		_energy_2real = (float)(_energy_e - _energy_s);
		_energy_2real = _energy_2real / _const_energy;
	}
	else if ( _energy_e < _energy_s )
	{
		_energy_2real = (float)(0xffffffff - _energy_s + _energy_e);
		_energy_2real = _energy_2real / _const_energy;
	}
	GLOBAL_MEMCLR(m_Esum_arg.Eg_1,20);
	GLOBAL_MEMCLR(m_Esum_arg.Eg_2,20);
    global_ftoa(_energy_2real,m_Esum_arg.Eg_2,4);
	_energy_1real = _energy_2real*(float)global_atoi(m_Esum_arg.BL);
    global_ftoa(_energy_1real,m_Esum_arg.Eg_1,4);
    if(++_view_cnt%3==0)
    {
	    GUI_window_hwin_req(m_Esummod_edit);
		GUI_set_Fgcolor(ESummod_FG_COLOR);
		GUI_edit_string_display_At(m_Esum_arg.Eg_1,14);
		GUI_edit_string_display_At(m_Esum_arg.Eg_2,13);
	    GUI_window_hwin_release(m_Esummod_edit);
    }
	return;
}

//获取系统时间
static void _Systime( dev_id_t dev,char*pch,time_string_t*ptime)
{
	os_driver_ioctl( dev, TMR_CMD_TIME_STRING_GET, ptime);
    GLOBAL_MEMCLR(pch,20);
    global_strcat(pch,"20");	
    global_strcat(pch,ptime->year_buf);	
    global_strcat(pch,"-");
    global_strcat(pch,ptime->month_buf);
    global_strcat(pch,"-");
    global_strcat(pch,ptime->day_buf);
	global_strcat(pch, "/" );
    global_strcat(pch,ptime->hour_buf);
	global_strcat(pch, ":" );
	global_strcat(pch,ptime->min_buf );
	return;
}


static long  _SUMtime(void)
{
    long nSEC = 0;
	long dx;
    if(!global_memcmp(_stime.year_buf,_etime.year_buf,3))
    {
          return 0;
	}
    if(!global_memcmp(_stime.month_buf,_etime.month_buf,3))
    {
          return 0;
	}
    if(!global_memcmp(_stime.day_buf,_etime.day_buf,3))
    {
         dx   =global_atoi(_etime.day_buf)-global_atoi(_stime.day_buf);
		 nSEC =dx*86400;
	}
    if(!global_memcmp(_stime.hour_buf,_etime.hour_buf,3))
    {
         dx   =global_atoi(_etime.hour_buf)-global_atoi(_stime.hour_buf);
		 nSEC+=(dx*3600);
	}
    if(!global_memcmp(_stime.min_buf,_etime.min_buf,3))
    {
         dx   =global_atoi(_etime.min_buf)-global_atoi(_stime.min_buf);
		 nSEC+=(dx*60);
	}
    if(!global_memcmp(_stime.sec_buf,_etime.sec_buf,3))
    {
         dx   =global_atoi(_etime.sec_buf)-global_atoi(_stime.sec_buf);
		 nSEC+=(dx);
	}
	return nSEC;
}


void app_Esummod_SUM_task( void*arg )
{
	dev_id_t dev;
    long 	 nSEC = 0;
	dev = os_driver_open( "TIME" );
    _Systime(dev,m_Esum_arg.start_time,&_stime);
    GUI_window_hwin_req(m_Esummod_edit);
	GUI_set_Fgcolor(ESummod_FG_COLOR);
	GUI_edit_string_display_At(m_Esum_arg.start_time,10);
    GUI_window_hwin_release(m_Esummod_edit);
	while ( 1 )
	{
		msleep(APP_100_MS);
		if ( os_task_delete_req( SELF_PRO ) == OS_TASK_DEL_REQ )
		{
		    _Systime(dev,m_Esum_arg.end_time,&_etime);
			nSEC=_SUMtime();
			if(nSEC!=0)global_ltoa(nSEC,m_Esum_arg.second);
		    GUI_window_hwin_req(m_Esummod_edit);
			GUI_set_Fgcolor(ESummod_FG_COLOR);
			GUI_edit_string_display_At(m_Esum_arg.end_time,11);
			GUI_edit_string_display_At(m_Esum_arg.second,15);
			GUI_edit_string_display_At(m_Esum_arg.Eg_1,14);
			GUI_edit_string_display_At(m_Esum_arg.Eg_2,13);
			GUI_window_hwin_release(m_Esummod_edit);
			os_task_delete( SELF_PRO );   			
		}
        _energy_calc();
	} 
}

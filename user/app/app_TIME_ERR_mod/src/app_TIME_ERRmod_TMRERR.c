#include    "app_TIME_errmod_inside.h"

static hwin_t* _edit     = 0;
static hwin_t* _errWin[2]={0,0};
float   m_TMR_err[4];
static float   m_TMR_err_K;
float		   m_error_buf[10] = {0,};
uint8_t		   m_error_offset = 0;
float		   m_day_timer_error = 0;

static hwin_t*_PaintERR(hwin_t*hOBJ,int x,int y,int wide,char*title)
{
    hwin_t*err_win;
    GUI_window_hwin_req(hOBJ);
    err_win = Gui_txtbox_creat(x,y,wide,100,WIN_3D,hOBJ);
    GUI_window_hwin_req(err_win);
    GUI_set_Bgcolor(C_GUI_BlACK);
	GUI_clean_window();
    GUI_SetFront(&GUI_F16X24_ASCII);
    GUI_set_Fgcolor(C_GUI_WHITE);
	GUI_string_display_At(2,2,title);
    GUI_window_hwin_release(err_win);
    GUI_window_hwin_release(hOBJ);
	return err_win;
}

static hwin_t*_PaintWIN(hwin_t *hp)
{
	hwin_t*pOBJ;
	GUI_window_hwin_req(hp);
	GUI_set_Bgcolor(TIME_errmod_WIN_BG_COLOR);    
	GUI_set_Fgcolor(TIME_errmod_FG_COLOR);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	pOBJ = GUI_window_child_creat(2,0,626,30,WIN_PAINT,hp);
    GUI_window_hwin_req(pOBJ);
	GUI_set_Fgcolor(C_GUI_GREEN);
    GUI_string_display_At(5,1,"时钟误差频率定义为:表时钟脉冲输出间隔如 1Hz");
    GUI_window_hwin_release(pOBJ);
	GUI_edit_memeber_add(5,45,5,"时钟频率",MEMBER_3D|TMR1_CS);
	GUI_edit_memeber_add(5,85,5,"测量圈数",MEMBER_3D|METER1_QS);
	GUI_string_display_At(170,48,"Hz");
	GUI_string_display_At(220,90,"日计时误差:");
	GUI_edit_memeber_add(220,125,8,"",MEMBER_3D|ERR_DAY);
	GUI_string_display_At(325,128,"S/d");
	GUI_string_display_At(220,165,"试验结论:");
	GUI_edit_memeber_add(220,195,8,"",MEMBER_3D|ERR_DAY_RS);
	_errWin[0]=_PaintERR(hp,19,hp->win_high-115,180,"Err:");
	GUI_WINDIS_VLINE(375,35,hp->win_high);
	GUI_set_Bgcolor(0x10A3);
	m_TIME_ERR_LJT_win= GUI_window_child_creat(395,36,225,205,WIN_FILL,hp);
	app_TIME_LJT_init(m_TIME_ERR_LJT_win);
    GUI_window_hwin_release(hp);
	return hp;
}

/********************************************************************
* @创建人 :周炳权
* @功能 :日计时误差计算
* @输入 :error:PPM
* @输出 :NONE
********************************************************************/
#define   DAY_SECOND   0.0864		//PPM转化成秒/天
static void TMRErr_day_display( float m_error )
{
	float 	m_average = 0;
	uint8_t m_offset = 0;
	float   TMR_const;
	int     plus_N;
	float   m_temp = 0;
	
	TMR_const = global_atof(g_user_arg.DL_app_user_set.TMRcs);
	plus_N    = global_atoi(g_user_arg.DL_app_user_set.qs);
	m_temp 	  = plus_N * TMR_const;
	m_error_buf[m_error_offset] = m_error / m_temp;//计算每秒的误差
	m_error_offset++;
	if ( m_error_offset == 10 )
	{
		m_error_offset = 0;
		for ( m_offset = 0;m_offset < 10;m_offset++ )
		{
			m_average += m_error_buf[m_offset];
		}
		m_average = (float)(m_average / 10.0);		//计算10次的平均误差
		m_day_timer_error = m_average * DAY_SECOND;
		GUI_window_hwin_req(m_TIME_ERR_main_win);
	    GUI_set_Bgcolor(C_GUI_BlACK);
		GUI_set_Fgcolor(C_GUI_GREEN);
		GUI_edit_float_display_at(m_day_timer_error, ERR_DAY, 8);
		GUI_set_Fgcolor(C_GUI_YELLOW);
		if ( GLOBAL_ABS(m_day_timer_error) > 0.5 )
		{
			GUI_edit_string_display_At("不合格", ERR_DAY_RS);
			GUI_set_Fgcolor(C_GUI_RED);
			GUI_edit_float_display_at(m_day_timer_error, ERR_DAY, 8);
		}
		else 
			GUI_edit_string_display_At("合格", ERR_DAY_RS);
		GUI_window_hwin_release(m_TIME_ERR_main_win);
	}
	return;
}


static float _TMR_K_obtain(void)
{
    FIL*fp;
	float TMR_K=1.0;
	UINT r;
	FRESULT ret;
	fp = fatfs_open("TMRk",FA_OPEN_EXISTING|FA_READ);
    if(fp)
    {
        ret=fatfs_read_bytes(fp,&TMR_K,sizeof(float),&r);
        fatfs_close(fp);
		if(ret==FR_OK)return TMR_K;
		return 1;
	}
    return 1;
}


static void _circle_display(Bsize_t circle,hwin_t*hp)
{
    char ctemp[10]="";
	if(!hp)return;
	GUI_window_hwin_req(hp);
    GUI_set_Bgcolor(C_GUI_BlACK);
	global_itoa((circle),ctemp);
	global_strcat(ctemp,"  ");
    GUI_SetFront(&GUI_F16X24_ASCII);
    GUI_set_Fgcolor(C_GUI_RED);
	GUI_string_display_At(10,30,ctemp);
	GUI_window_hwin_release(hp);
	return;
}

static void _Err_ftoa1(float err,char*str,Bsize_t point)
{
	Bsize_t len;
   if(err>1000) err = 1000.0;
   global_ftoa(err,str,point);
   len=global_strlen(str);
   if(len>7)str[7]=0;
   return;
}

static void _TMRErr_display(float*perr,hwin_t*hp,Bsize_t point)
{
    char ctemp[10]="";
	Bsize_t len;
	if(!hp)return;
	GUI_window_hwin_req(hp);
    GUI_set_Bgcolor(C_GUI_BlACK);
	GUI_set_Fgcolor(C_GUI_GREEN);
	_Err_ftoa1(perr[0],ctemp,point);
	global_strcat(ctemp,"ppm");
    len=global_strlen(ctemp);
	if(len<=9){len=9-len;
	while(len>0){global_strcat(ctemp," ");len --;}}
    GUI_SetFront(&GUI_F16X32_ASCII);
    GUI_set_Fgcolor(C_GUI_GREEN);
	GUI_string_display_At(2,60,"          ");
	GUI_string_display_At(2,60,ctemp);	
	GUI_set_Fgcolor(C_GUI_YELLOW);
    GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_fill_window_area(78,2,78+8*9,42+15);
	_Err_ftoa1(perr[3],ctemp,point);
    GUI_string_display_At(80,2,ctemp);
	_Err_ftoa1(perr[2],ctemp,point);
    GUI_string_display_At(80,22,ctemp);
	_Err_ftoa1(perr[1],ctemp,point);
    GUI_string_display_At(80,42,ctemp);
	GUI_window_hwin_release(hp);
	return;
}



static void _TMR_ERR_Exc(int*circle,int*last_circle,hwin_t**err_win)
{
#define  GET_TMR_FLAG      0
#define  GET_TMR1_ERR      1
#define  GET_TMR2_ERR      2
#define  GET_TMR3_ERR      3
    BOOL flag[3];
    uint32_t TMR_count;
	float    real=0.0,meter=0.0;
	float    TMR_const,err;
	int      plus_N,pos;
	circle[0] = app_global_path1_circle_obtain();
    pos = 0;
	if(circle[pos]!=last_circle[pos])
	{
		last_circle[pos] = circle[pos];
		_circle_display(circle[pos],err_win[pos]);
	}
    os_driver_ioctl(g_piccom_dev,GET_TMR_FLAG,flag);
	if(flag[0])
	{
    	os_driver_ioctl(g_piccom_dev,GET_TMR1_ERR,&TMR_count);
		TMR_const = global_atof(g_user_arg.DL_app_user_set.TMRcs);
		plus_N    = global_atoi(g_user_arg.DL_app_user_set.qs);
		meter     = TMR_const*plus_N*48000000*m_TMR_err_K;
		real      = TMR_count;
		err       = (meter-real)/real*10000.0;
		m_TMR_err[3] = m_TMR_err[2];
		m_TMR_err[2] = m_TMR_err[1];
		m_TMR_err[1] = m_TMR_err[0];
		m_TMR_err[0] = err;
		buz_response( APP_100_MS );
		_TMRErr_display(m_TMR_err,err_win[0],3);
		TMRErr_day_display(err);
	}
    return;
}

static void _ERRARG_display(hwin_t*pOBJ)
{
	DL_app_user_set_t* pARG = &g_user_arg.DL_app_user_set;
	GUI_window_hwin_req( pOBJ );
	GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
	GUI_set_Fgcolor( TIME_errmod_FG_COLOR );	
	GUI_edit_string_display_At(pARG->TMRcs, TMR1_CS);
	GUI_edit_string_display_At(pARG->qs,    METER1_QS);
	GUI_window_hwin_release( pOBJ );
    return;
}

void app_TMR_task(void*arg)
{
    uint8_t key;
	BOOL    Is_pause = TURE;
	int     circle[3],last_circle[3];
	
	_edit=_PaintWIN(m_TIME_ERR_main_win);
	GUI_MsgFIFO_create(_edit,2);
	m_TMR_err_K  = _TMR_K_obtain();
	_ERRARG_display(_edit);
	
	while(1)
	{   
		msleep(TYPE_DELAY);
		if ( os_task_delete_req( SELF_PRO ) == OS_TASK_DEL_REQ )
		{
		    GUI_window_destory(_edit);
			os_task_delete( SELF_PRO );   			
		}
        if(GUI_key_read(_edit,&key,1)==1)
        {
			 if(key==KEY_CONST_SETTING)
			 {
				GUI_edit_start(_edit,app_TIME_ERRARG_setting);
				fatfs_writeARG("USERDATA",&g_user_arg,sizeof(DL_app_user_all_info_t));
				app_global_set_user_dl_arg(&g_user_arg);
				_ERRARG_display(_edit);
			 }
			 else if(KEY_PAUSE==key)
			 {
				Is_pause=app_TIME_ERR_pause_view(_errWin[0]);
			 }		 
		}
		if(Is_pause)
		{
			_TMR_ERR_Exc(circle,last_circle,_errWin);
		}
	}
}


void  app_TMR_errPAINT(void)
{
	_edit = _PaintWIN(m_TIME_ERR_main_win);
	_ERRARG_display(_edit);
	_TMRErr_display(g_DL_param.f_path1_err,_errWin[0],3);
	TMRErr_day_display(global_atof(g_user_arg.DL_app_user_set.qs3));
	return;
}

/********************************************************************
* @创建人 :阳永锋
* @功能 :误差回显
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void  app_TMR_err_rePAINT(void)
{
	float day_error = 0.0;
	day_error = global_atof(g_user_arg.DL_app_user_set.qs3);

	_edit = _PaintWIN(m_TIME_ERR_main_win);
	_ERRARG_display(_edit);
	_TMRErr_display(g_DL_param.f_path1_err,_errWin[0],3);
	GUI_window_hwin_req(m_TIME_ERR_main_win);
    GUI_set_Bgcolor(C_GUI_BlACK);
	GUI_set_Fgcolor(C_GUI_GREEN);
	GUI_edit_float_display_at(day_error, ERR_DAY, 8);
	if ( GLOBAL_ABS(day_error) > 0.5 )
	{
		GUI_edit_string_display_At("不合格", ERR_DAY_RS);
		GUI_set_Fgcolor(C_GUI_RED);
	}
	else if ( GLOBAL_ABS(day_error) != 0 )
		GUI_edit_string_display_At("合格", ERR_DAY_RS);
	GUI_window_hwin_release(m_TIME_ERR_main_win);
	return;
}


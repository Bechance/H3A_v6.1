#include    "app_TIME_errmod_inside.h"

hwin_t *     m_TIME_errWIN 		 = 0;
static BOOL  m_ERR_pause  		 = TURE;

/*static void _Constselect_paint(hwin_t* pOBJ)
{
#define _XGAP  90
#define _YGAP  30

	hwin_t*p;
	uint16_t BK = 0xce79;
    p=Gui_button_creat(46,pOBJ->win_high-40,80,30,WIN_MSG_EN|BUTTON_PAINT|KEY_ENTER,pOBJ);
	GUI_button_title_display(p,"È·ÈÏ");
    p=Gui_button_creat(171,pOBJ->win_high-40,80,30,WIN_MSG_EN|BUTTON_PAINT|KEY_EXIT,pOBJ);
	GUI_button_title_display(p,"ÍË³ö");
	GUI_window_hwin_req(pOBJ);
    GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_set_Bgcolor(BK);
	GUI_set_Fgcolor(C_GUI_BlACK);
    GUI_radio_memeber_add(10+_XGAP*0,10+_YGAP*0,"  300",0);
    GUI_radio_memeber_add(10+_XGAP*1,10+_YGAP*0,"  600",1);
    GUI_radio_memeber_add(10+_XGAP*2,10+_YGAP*0,"   800",2);
    GUI_radio_memeber_add(10+_XGAP*0,10+_YGAP*1," 1200",3);
    GUI_radio_memeber_add(10+_XGAP*1,10+_YGAP*1," 2400",4);
    GUI_radio_memeber_add(10+_XGAP*2,10+_YGAP*1,"  3600",5);
    GUI_radio_memeber_add(10+_XGAP*0,10+_YGAP*2," 3000",6);
    GUI_radio_memeber_add(10+_XGAP*1,10+_YGAP*2," 4000",7);
    GUI_radio_memeber_add(10+_XGAP*2,10+_YGAP*2,"  8000",8);
    GUI_radio_memeber_add(10+_XGAP*0,10+_YGAP*3,"10000",9);
    GUI_radio_memeber_add(10+_XGAP*1,10+_YGAP*3,"36000",10);
    GUI_radio_memeber_add(10+_XGAP*2,10+_YGAP*3,"100000",11);
	GUI_MsgFIFO_create(pOBJ,2);
	GUI_WINready(pOBJ,1);
	GUI_TopWIN_Set(pOBJ);
	GUI_radio_select(pOBJ,0);
	GUI_window_hwin_release(pOBJ);
    return;
}

void app_TIME_Constselect(void)
{
    int x1,y1,x2,y2;
	Bsize_t  sec;
    uint8_t  key;
	hwin_t*  pOBJ;
	char*    pSTR;
	uint16_t BK = 0xce79;
	x1 = 100;y1 = 100;x2 = x1+300;y2 = y1+200;
	sec=GUI_read_block_Extp(x1,y1,x2, y2);
	GUI_window_hwin_req(GET_GUIWIN);
	Gui_button_set_color(C_GUI_LOW_BLUE,0xce79);
	GUI_set_Bgcolor(BK);
    GUI_SetFront(&GUI_HZK16_EXTASCII);
    pOBJ = GUI_radio_create(x1+1,y1+1,(x2-x1-8),(y2-y1-5),WIN_MSG_EN|WIN_3D|WIN_FILL,GET_GUIWIN);
    _Constselect_paint(pOBJ);
	GUI_window_hwin_release(GET_GUIWIN);
	app_global_key_control_power_req();
	while(1)
	{   
		msleep(TYPE_DELAY);
	    if(GUI_key_read(pOBJ,&key,1)==1
		||app_global_key_force_obtain(&key,1)==1)
	    {
			if(key==KEY_ENTER)
			{
				pSTR=(char*)GUI_radio_title_obtain(pOBJ);
				GLOBAL_MEMCLR(g_user_arg.DL_app_user_set.cs,9);
				GLOBAL_MEMCLR(g_user_arg.DL_app_user_set.cs2,9);
				global_strcpy(g_user_arg.DL_app_user_set.cs,pSTR);
				global_strcpy(g_user_arg.DL_app_user_set.cs2,pSTR);
				global_string_filter(g_user_arg.DL_app_user_set.cs,' ');
				global_string_filter(g_user_arg.DL_app_user_set.cs2,' ');
				break;
			}
			else if(key==KEY_EXIT)
			{
				break;
			}
			else if(key==KEY_UP)
			{
				GUI_WINshift(1);
			}
			else if(key==KEY_DOWN)
			{
				GUI_WINshift(0);
			}		
			else if(key == KEY_F5)
			{
				GUI_WINdo();
			}
		}
	}
	GUI_window_destory(pOBJ);
	GUI_write_block_Extp(x1,y1,x2, y2, sec);
	Gui_button_set_color(BUTTON_TXT_COLOR,BUTTON_BG_COLOR);
	app_global_key_control_power_release();
	return;
}
*/
int app_TIME_Qs_select(float W,float Const)
{
    float N;
    N=(W*2.0*Const)/(3600.0*1000.0)+0.5; 
	if(N<1)N=1;
    return(int)N;
}


static void _circle1_display(Bsize_t circle,hwin_t*hp)
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

static void _Err_ftoa(float err,char*str,Bsize_t point)
{
	Bsize_t len;
   if(err>1000) err = 1000.0;
   global_ftoa(err,str,point);
   len=global_strlen(str);
   if(len>7)str[7]=0;
   return;
}

static void _Err_display(float*perr,hwin_t*hp,Bsize_t point)
{
    char ctemp[10]="";
	Bsize_t len;
	if(!hp)return;
	GUI_window_hwin_req(hp);
    GUI_set_Bgcolor(C_GUI_BlACK);
	GUI_set_Fgcolor(C_GUI_GREEN);
	_Err_ftoa(perr[0],ctemp,point);
	global_strcat(ctemp,"%");
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
	_Err_ftoa(perr[3],ctemp,point);
    GUI_string_display_At(80,2,ctemp);
	_Err_ftoa(perr[2],ctemp,point);
    GUI_string_display_At(80,22,ctemp);
	_Err_ftoa(perr[1],ctemp,point);
    GUI_string_display_At(80,42,ctemp);
	GUI_window_hwin_release(hp);
	return;
}

BOOL app_TIME_ERR_pause_view(hwin_t*hp)
{
	GUI_window_hwin_req(hp);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_set_Bgcolor(C_GUI_BlACK);
	GUI_set_Fgcolor(C_GUI_RED); 
	if(m_ERR_pause)
	{
	    m_ERR_pause = FAIL;
		GUI_string_display_At(hp->win_wide-35,hp->win_high-20,"ÔÝÍ£");
	}
	else
	{
	    m_ERR_pause = TURE;
		GUI_string_display_At(hp->win_wide-35,hp->win_high-20,"    ");
	}
	GUI_window_hwin_release(hp);
    return m_ERR_pause; 
}

void app_TIME_ERR_Exc(int*circle,int*last_circle,hwin_t**err_win)
{
	DL_flag_t 	dl_flag;
	int pos;
	if(!m_ERR_pause) return;
	app_global_DLflag_obtain(&dl_flag);
	circle[0] = app_global_path1_circle_obtain();
	//circle[1] = app_global_path2_circle_obtain();
	for(pos=0;pos<3;pos++)
	{
		if(circle[pos]!=last_circle[pos])
		{
			last_circle[pos] = circle[pos];
			_circle1_display(circle[pos],err_win[pos]);
		}
	}
	if ( dl_flag.DL_path1_ERR_is_new)
	{
		buz_response( APP_100_MS );
		app_global_path1_DLerr_obtain(&g_DL_param);
		_Err_display(g_DL_param.f_path1_err,err_win[0],3);
	}
	/*if ( dl_flag.DL_path2_ERR_is_new)
	{
		buz_response( APP_100_MS );
		app_global_path2_DLerr_obtain(&g_DL_param);
		_Err_display(g_DL_param.f_path2_err,err_win[1],3);
	}*/
	return;
}


uint8_t app_TIME_ERRARG_setting(hwin_t*hp)
{
	uint8_t  key;
	Bsize_t  member;
	DL_app_user_set_t plocal;
    global_memcpy(&plocal,&g_user_arg.DL_app_user_set,sizeof(DL_app_user_set_t));
	hand_wirte_input_box_pos_set(20,315);
	key_input_method_filter(KEY_INPUT_123);
	app_global_key_control_power_req();
	while(1)
	{
		msleep( TYPE_DELAY );
		if ( app_global_key_force_obtain( &key, 1 ) != 1 )
		{
			continue;
		}
		GUI_window_hwin_req( hp );
		GUI_edit_run_hook( key );
		GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
		GUI_set_Fgcolor( TIME_errmod_FG_COLOR );		
		if ( app_global_decide_inputkey( key ) != FAIL )
		{
			member = GUI_edit_id_obtain();
			if ( member == METER1_CS )     key_method_input_shadow( key,plocal.cs);
			else if ( member == METER1_QS) key_method_input_shadow( key,plocal.qs);
			//else if ( member == METER2_CS) key_method_input_shadow( key,plocal.cs2);
			//else if ( member == METER2_QS) key_method_input_shadow( key,plocal.qs2);			
			else if ( member == TMR1_CS)   key_method_input_shadow( key,plocal.TMRcs);			
			else if ( member == CT_BL)     key_method_input_shadow( key,plocal.bb1);

		}
		else if(key == KEY_DOWN)GUI_edit_shift(EDIT_NEXT);	
		else if(key == KEY_UP)  GUI_edit_shift(EDIT_LAST);	
		GUI_window_hwin_release( hp );
		if ( key == KEY_ENTER)
		{
    		global_memcpy(&g_user_arg.DL_app_user_set,&plocal,sizeof(DL_app_user_set_t));
			break;
		}
		else if (key == KEY_EXIT)
		{
			break;
		}
	}
	app_global_key_control_power_release();
    return key;
}


void app_TIME_ERR_task(void*arg)
{
    uint8_t Pro = INVALID_PRO;
	m_ERR_pause = TURE;
	Pro = os_task_assign_stk_build(app_TMR_task,3);
	while(1)
	{   
		msleep(TYPE_DELAY);
		if ( os_task_delete_req( SELF_PRO ) == OS_TASK_DEL_REQ )
		{
			if(Pro!=INVALID_PRO){OS_DELETE_REQ( Pro );}
			os_task_delete( SELF_PRO );   			
		}		
	}
}


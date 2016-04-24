#include    "app_XCerrmod_inside.h"

static BOOL  m_ERR_pause     = TURE;
static hwin_t* _edit         = 0;
static hwin_t* _2radio[2]    ={0,0};
static hwin_t* _droplist     = 0;
static hwin_t* _droplistDJ   = 0;
static hwin_t* _droplistPQ   = 0;

static hwin_t* _errWin[2]  ={0,0};
static hwin_t* _errWinR[2] ={0,0};
/*********************************************************************/

/*********************************************************************/
static hwin_t*_PaintERR(hwin_t*hOBJ,int x,int y,int wide,char*title)
{
    hwin_t*err_win;
    GUI_window_hwin_req(hOBJ);
    GUI_set_Bgcolor(0X10A3);
    err_win = Gui_txtbox_creat(x,y,wide,100,WIN_3D|WIN_FILL,hOBJ);
    GUI_window_hwin_req(err_win);
    GUI_SetFront(&GUI_F16X24_ASCII);
    GUI_set_Fgcolor(C_GUI_WHITE);
	GUI_string_display_At(2,2,title);
    GUI_window_hwin_release(err_win);
    GUI_window_hwin_release(hOBJ);
	return err_win;
}


static hwin_t*_PaintWIN(hwin_t*hp)
{
	hwin_t*pOBJ;
	int    ytem;
	GUI_window_hwin_req(hp);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_set_Bgcolor(XCerrmod_WIN_BG_COLOR);    
	GUI_set_Fgcolor(XCerrmod_FG_COLOR); 
	GUI_edit_memeber_add(190,2+34*1,8,(char*)m_XCERRmod_MSG_07[g_language], MEMBER_3D|CT_BL);
	GUI_edit_memeber_add(0  ,2+34*2,8,(char*)m_XCERRmod_MSG_04[g_language],MEMBER_3D|METER1_CS);
	GUI_edit_memeber_add(190,2+34*2,8,(char*)m_XCERRmod_MSG_06[g_language],MEMBER_3D|METER2_CS);
	GUI_edit_memeber_add(0  ,2+34*3,8,(char*)m_XCERRmod_MSG_03[g_language], MEMBER_3D|METER1_QS);
	GUI_edit_memeber_add(190,2+34*3,8,(char*)m_XCERRmod_MSG_05[g_language],MEMBER_3D|METER2_QS);
    GUI_string_display_At(15,6+34*1,  (char*)m_XCERRmod_MSG_02[g_language]);

	pOBJ = GUI_radio_create(2,2,160,30,WIN_MSG_EN|WIN_PAINT|KEY_METER_SELECT,hp);
	ytem = (pOBJ->win_high-GUI_conText.CUR_Front->high)/2;
    GUI_window_hwin_req(pOBJ);
	GUI_radio_memeber_add(5,ytem,(char*)m_XCERRmod_MSG_08[g_language],0);
	GUI_radio_memeber_add(80,ytem,(char*)m_XCERRmod_MSG_09[g_language],1);
	GUI_radio_select(pOBJ,0);
    GUI_window_hwin_release(pOBJ);	   
	_2radio[0] = pOBJ;

	pOBJ = GUI_radio_create(167,2,160,30,WIN_MSG_EN|WIN_PAINT|KEY_GD_SELECT,hp);
    GUI_window_hwin_req(pOBJ);
	GUI_radio_memeber_add(5,  ytem,(char*)m_XCERRmod_MSG_11[g_language],0);
	GUI_radio_memeber_add(80, ytem,(char*)m_XCERRmod_MSG_12[g_language],1);
	GUI_radio_select(pOBJ, 0);
    GUI_window_hwin_release(pOBJ);	 
	_2radio[1] = pOBJ;
	GUI_SetFront(&GUI_HZK24_EXTASCII);
    _droplistPQ=Gui_drop_list_creat(342,2,m_XCERRmod_MSG_10[g_language],SIZE_OF_ARRAY(m_XCERRmod_MSG_10[g_language]),0,hp);
    _droplistDJ=Gui_drop_list_creat(502,2,m_XCERRmod_MSG_01[g_language],SIZE_OF_ARRAY(m_XCERRmod_MSG_01[g_language]),0,hp);
    _droplist  =Gui_drop_list_creat(63, 2+34*1,g_dlsr_option[g_language],SIZE_OF_ARRAY(g_dlsr_option[g_language]),KEY_I_IN,hp);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	_errWin[0]=_PaintERR(hp,5,hp->win_high-104,160,  "Err1:");
	_errWin[1]=_PaintERR(hp,193,hp->win_high-104,160,"Err2:");
	GUI_WINDIS_VLINE(375,36,hp->win_high);
	GUI_set_Bgcolor(0X10A3);  
	m_XCERR_LJT_win= GUI_window_child_creat(395,36,225,205,WIN_FILL,hp);
	app_LJT_init(m_XCERR_LJT_win);
	GUI_window_hwin_release(hp);
	return hp;
}


static void _Constselect_paint(hwin_t* pOBJ)
{
#define _XGAP  90
#define _YGAP  30
	hwin_t*p;
	uint16_t BK = 0xce79;
    p=Gui_button_creat(46,pOBJ->win_high-40,80,30,WIN_MSG_EN|BUTTON_PAINT|KEY_ENTER,pOBJ);
	GUI_button_title_display(p,(char*)m_XCERRmod_MSG_13[g_language]);
    p=Gui_button_creat(171,pOBJ->win_high-40,80,30,WIN_MSG_EN|BUTTON_PAINT|KEY_EXIT,pOBJ);
	GUI_button_title_display(p,(char*)m_XCERRmod_MSG_14[g_language]);
	GUI_window_hwin_req(pOBJ);
    GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_set_Bgcolor(BK);
	GUI_set_Fgcolor(C_GUI_BlACK);
    GUI_radio_memeber_add(10+_XGAP*0,10+_YGAP*0,"  460",0);
    GUI_radio_memeber_add(10+_XGAP*1,10+_YGAP*0,"  800",1);
    GUI_radio_memeber_add(10+_XGAP*2,10+_YGAP*0,"  1200",2);
    GUI_radio_memeber_add(10+_XGAP*0,10+_YGAP*1," 2400",3);
    GUI_radio_memeber_add(10+_XGAP*1,10+_YGAP*1," 4800",4);
    GUI_radio_memeber_add(10+_XGAP*2,10+_YGAP*1,"  3600",5);
    GUI_radio_memeber_add(10+_XGAP*0,10+_YGAP*2," 2000",6);
    GUI_radio_memeber_add(10+_XGAP*1,10+_YGAP*2," 5000",7);
    GUI_radio_memeber_add(10+_XGAP*2,10+_YGAP*2,"  8000",8);
    GUI_radio_memeber_add(10+_XGAP*0,10+_YGAP*3,"10000",9);
    GUI_radio_memeber_add(10+_XGAP*1,10+_YGAP*3,"36000",10);
    GUI_radio_memeber_add(10+_XGAP*2,10+_YGAP*3,"100000",11);
	GUI_MsgFIFO_create(pOBJ,2);
	GUI_TopWIN_Set(pOBJ);
	GUI_radio_select(pOBJ,0);
	GUI_window_hwin_release(pOBJ);
    return;
}

static void _Constselect(void)
{
    int x1,y1,x2,y2;
	Bsize_t  sec;
    uint8_t  key;
	hwin_t*  pOBJ;
	char*    pSTR;
	int      ID = 0,IDbak = 0;
	uint16_t BK = 0xce79;
	x1 = 100;y1 = 100;x2 = x1+300;y2 = y1+200;
	sec=GUI_read_block_Extp(x1,y1,x2, y2);
	GUI_window_hwin_req(GET_GUIWIN);
	Gui_button_set_color(C_GUI_LOW_BLUE,0xce79);
	GUI_set_Bgcolor(BK);
    GUI_SetFront(&GUI_HZK16_EXTASCII);
    pOBJ = GUI_radio_create(x1+1,y1+1,(x2-x1-8),(y2-y1-5),WIN_MSG_EN|WIN_3D|WIN_FILL,GET_GUIWIN);
    _Constselect_paint(pOBJ);
	GUI_WINready(pOBJ,1);
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
				GUI_WINdo();
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
				break;
			else if(key==KEY_DOWN)
                ID=((ID+3)<=11)?(ID+=3):(ID); 
			else if(key==KEY_UP)
                ID=(ID>2)?(ID-=3):(ID); 
			else if(key==KEY_RIGHT)
                ID=(ID<11)?(ID+=1):(ID); 
			else if(key==KEY_LEFT)
                ID=(ID>0)?(ID-=1):(ID); 
			if(ID!=IDbak){
				IDbak = ID;
				GUI_SUBordready(ID,1);
			}
		
		}
	}
	GUI_WINready(GUI_Getchild(KEY_METER_SELECT,_edit),1);
	GUI_window_destory(pOBJ);
	GUI_write_block_Extp(x1,y1,x2, y2, sec);
	app_global_key_control_power_release();
	return;
}

static int _Qs_select(float W,float Const)
{
    float N;
    N=(W*2.0*Const)/(3600.0*1000.0)+0.5; 
	if(N<1)N=1;
    return(int)N;
}


static void _circle_display(Bsize_t circle,hwin_t*hp)
{
    char ctemp[10]="";
	if(!hp)return;
	GUI_window_hwin_req(hp);
    GUI_set_Bgcolor(GUI_GetwinBgcor());
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
    GUI_set_Bgcolor(GUI_GetwinBgcor());
	GUI_set_Fgcolor(C_GUI_GREEN);
	_Err_ftoa(perr[0],ctemp,point);
	global_strcat(ctemp,"%");
    len=global_strlen(ctemp);
	if(len<=9){len=9-len;
	while(len>0){global_strcat(ctemp," ");len --;}}
    GUI_SetFront(&GUI_F16X32_ASCII);
    GUI_set_Fgcolor(C_GUI_GREEN);
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

static void _pause_view(hwin_t*hp)
{
	GUI_window_hwin_req(hp);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_RED); 
	if(m_ERR_pause)
	{
	    m_ERR_pause = FAIL;
		app_global_DL_measure_exit();
		GUI_string_display_At(hp->win_wide-35,hp->win_high-20,(char*)m_XCERRmod_MSG_15[g_language]);
	}
	else
	{
	    m_ERR_pause = TURE;
		app_global_DL_measure_entery();
		GUI_string_display_At(hp->win_wide-35,hp->win_high-20,"    ");
	}
	GUI_window_hwin_release(hp);
    return; 
}



void _ErrExc(int*circle,int*last_circle,hwin_t**err_win)
{
	DL_flag_t 	dl_flag;
	int pos;
	app_global_DLflag_obtain(&dl_flag);
	circle[0] = app_global_path1_circle_obtain();
	circle[1] = app_global_path2_circle_obtain();
	for(pos=0;pos<3;pos++)
	{
		if(circle[pos]!=last_circle[pos])
		{
			last_circle[pos] = circle[pos];
			_circle_display(circle[pos],err_win[pos]);
		}
	}
	if ( dl_flag.DL_path1_ERR_is_new)
	{
		buz_response( APP_100_MS );
		app_global_path1_DLerr_obtain(&g_DL_param);
		_Err_display(g_DL_param.f_path1_err,err_win[0],3);
	}
	if ( dl_flag.DL_path2_ERR_is_new)
	{
		buz_response( APP_100_MS );
		app_global_path2_DLerr_obtain(&g_DL_param);
		_Err_display(g_DL_param.f_path2_err,err_win[1],3);
	}
	return;
}


static void _callblackview(hwin_t*pOBJ,DL_app_user_set_t* pARG)
{
	GUI_window_hwin_req( pOBJ );
	GUI_set_Fgcolor( C_GUI_WHITE);    
	GUI_edit_string_display_At(pARG->bb1,4);
	GUI_edit_string_display_At(pARG->cs, 5);
	GUI_edit_string_display_At(pARG->qs, 6);
	GUI_edit_string_display_At(pARG->cs2,7);
	GUI_edit_string_display_At(pARG->qs2,8);
	GUI_edit_string_display_At(pARG->bb2,9);
	GUI_edit_string_display_At(g_user_arg.DL_app_user_info.data_order,12);	

	if(pARG->meter_1OR3==0)
		GUI_edit_string_display_At((char*)m_XCERRmod_MSG_08[g_language],0);
	else
		GUI_edit_string_display_At((char*)m_XCERRmod_MSG_09[g_language],0);

	if(pARG->handOR_GD==0)
		GUI_edit_string_display_At((char*)m_XCERRmod_MSG_11[g_language],2);
	else
		GUI_edit_string_display_At((char*)m_XCERRmod_MSG_12[g_language],2);	
	
	GUI_edit_string_display_At((char*)m_XCERRmod_MSG_10[g_language][pARG->PorQ],1);	
	GUI_edit_string_display_At((char*)g_dlsr_option[g_language][pARG->dlsr],3);	
	GUI_edit_string_display_At((char*)m_XCERRmod_MSG_01[g_language][pARG->meterDJ],10);	
	GUI_edit_string_display_At((char*)m_XCERRmod_MSG_01[g_language][pARG->meterDJ2[0]],11);	


	GUI_window_hwin_release( pOBJ );
    return;
}

static uint8_t _callblack(hwin_t*hp)
{
	uint8_t key;
	Bsize_t member;
	char    ch[12]="";
	DL_app_user_set_t*p;
	p = app_malloc( sizeof( DL_app_user_set_t ) );
	app_global_key_control_power_req();
	key_input_method_assign(KEY_INPUT_123);
	if ( !p ) return KEY_EXIT;
	global_memcpy(p,&g_user_arg.DL_app_user_set,sizeof( DL_app_user_set_t ) );
	global_memcpy(ch,g_user_arg.DL_app_user_info.data_order,12);
	while ( 1 )
	{
		msleep( TYPE_DELAY );
		if ( app_global_key_force_obtain( &key, 1 ) != 1 )
		{
			continue;
		}
		GUI_window_hwin_req( hp );
		GUI_set_Fgcolor( C_GUI_WHITE);    
	    if ( key == KEY_RIGHT)		
		{
			member = GUI_edit_id_obtain();     				
            if(member == 0)      INC_DECIDE_LOOP_(p->meter_1OR3,1);
            else if(member == 1) INC_DECIDE_LOOP_(p->PorQ,2);
            else if(member == 2) INC_DECIDE_LOOP_(p->handOR_GD,1);
            else if(member == 3) INC_DECIDE_LOOP_(p->dlsr,8);
            else if(member == 10)INC_DECIDE_LOOP_(p->meterDJ,6);
            else if(member == 11)INC_DECIDE_LOOP_(p->meterDJ2[0],6);
			if((member>3 && member<10)||member==12)
            	GUI_edit_csr_shift( CSR_RIGHT_SHIFT );
			else
			{
				_callblackview(hp,p);
				GUI_edit_select(member);
			}
		}
		else if (key == KEY_LEFT)		
		{
			member = GUI_edit_id_obtain();     	
            if(member == 0)DEC_DECIDE_LOOP_(p->meter_1OR3,1);
            else if(member == 1)DEC_DECIDE_LOOP_(p->PorQ,1);
            else if(member == 2)DEC_DECIDE_LOOP_(p->handOR_GD,1);
            else if(member == 3)DEC_DECIDE_LOOP_(p->dlsr,8);
            else if(member == 10)DEC_DECIDE_LOOP_(p->meterDJ,6);
            else if(member == 11)DEC_DECIDE_LOOP_(p->meterDJ2[0],6);
			if((member>3 && member<10)||member==12)
                GUI_edit_csr_shift( CSR_LEFT_SHIFT);
			else
			{
				_callblackview(hp,p);
				GUI_edit_select(member);
			}
		}
		else if(key == KEY_DOWN)GUI_edit_shift( EDIT_NEXT);	
		else if(key == KEY_UP)  GUI_edit_shift( EDIT_LAST );
		else if (( app_global_decide_inputkey( key ) ) != FAIL )
		{
			member = GUI_edit_id_obtain();     
			if ( member==4)key_method_input_shadow(key, p->bb1);
			else if ( member==5)key_method_input_shadow(key, p->cs);
			else if ( member==6)key_method_input_shadow(key, p->qs);
			else if ( member==7)key_method_input_shadow(key, p->cs2);
			else if ( member==8)key_method_input_shadow(key, p->qs2);
			else if ( member==9)key_method_input_shadow(key, p->bb2);	
			else if ( member==12)key_method_input_shadow(key,ch);
		}
		else if ( key == KEY_ENTER )
		{
			global_memcpy(&g_user_arg.DL_app_user_set,p,sizeof( DL_app_user_set_t ) );
			global_memcpy(g_user_arg.DL_app_user_info.data_order,ch,12);
			break;
		}
		else if ( key == KEY_EXIT )
		{
			break;
		}
		GUI_window_hwin_release( hp );
	}
	GUI_window_hwin_release( hp );
	app_free( p );
	app_global_key_control_power_release();
	return key;
}

static  _ERRARG_setting(void)
{
    int x1,y1,x2,y2;
	Bsize_t  sec;
	hwin_t*  pOBJ,*_3dWIN;
	uint16_t BK = 0xce79;
	x1 = 100;y1 = 100;x2 = x1+400;y2 = y1+300;
	sec=GUI_read_block_Extp(x1,y1,x2, y2);
	GUI_window_hwin_req(GET_GUIWIN);
	GUI_set_Bgcolor(BK);
	Gui_3D_window_set_color(C_GUI_WHITE,C_GUI_LOW_BLUE,C_GUI_RED);	
    _3dWIN = Gui_3D_window_creat(x1+1,y1+1,(x2-x1-8),(y2-y1-5),"校表参数设置",WIN_FILL|WIN_3D,GET_GUIWIN);
    pOBJ = GUI_edit_creat(0,0,_3dWIN->win_wide,_3dWIN->win_high,0,_3dWIN);
    GUI_window_hwin_req(pOBJ);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_set_Bgcolor(BK);    
	GUI_set_Fgcolor(C_GUI_BlACK); 
	GUI_edit_memeber_add(0    ,10+34*0,8,"常数1",MEMBER_3D|5);
	GUI_edit_memeber_add(200  ,10+34*0,8,"圈数1",MEMBER_3D|6);
	GUI_edit_memeber_add(0    ,10+34*1,8,"常数2",MEMBER_3D|7);
	GUI_edit_memeber_add(200  ,10+34*1,8,"圈数2",MEMBER_3D|8);
	GUI_edit_memeber_add(0    ,10+34*2,8," 输入",MEMBER_3D|3);
	GUI_edit_memeber_add(200  ,10+34*2,8," 倍率",MEMBER_3D|4);
	GUI_edit_memeber_add(0    ,10+34*3,8," 变比",MEMBER_3D|9);
	GUI_edit_memeber_add(176  ,10+34*3,8,"表1等级",MEMBER_3D|10);
	GUI_edit_memeber_add(0    ,10+34*4,8," 数量",MEMBER_3D|0);
	GUI_edit_memeber_add(176  ,10+34*4,8,"表2等级",MEMBER_3D|11);
	GUI_edit_memeber_add(0    ,10+34*5,8," 脉冲",MEMBER_3D|2);
	GUI_edit_memeber_add(200  ,10+34*5,8," 方式",MEMBER_3D|1);
	GUI_edit_memeber_add(0   ,10+34*6,11," 输出常数",MEMBER_3D|12);
    GUI_window_hwin_release(pOBJ);
	GUI_TopWIN_Set(_3dWIN);
	GUI_window_hwin_release(GET_GUIWIN);
	_callblackview(pOBJ,&g_user_arg.DL_app_user_set);
	GUI_edit_start(pOBJ,_callblack);
	GUI_write_block_Extp(x1,y1,x2, y2, sec);
	GUI_window_destory(_3dWIN);
    return;
}

static void _ERRWIN_Reserve(hwin_t**errWIN,hwin_t**errViewWIN,BOOL state)
{
	GUI_window_hwin_req(errWIN[1]);
	if(state==0)
	{
		errViewWIN[0] = errWIN[0];
		errViewWIN[1] = 0;
		GUI_set_Bgcolor(LCD_Color2Index_565(0x555555));
		GUI_clean_window();
		GUI_SetFront(&GUI_F16X24_ASCII);
		GUI_set_Fgcolor(0X2145);
		GUI_string_display_At(2,2,"Reserve");
	}
	else
	{
	    errViewWIN[0] = errWIN[0];
		errViewWIN[1] = errWIN[1];
		GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
		GUI_clean_window();
		GUI_SetFront(&GUI_F16X24_ASCII);
		GUI_set_Fgcolor(C_GUI_WHITE);
		GUI_string_display_At(2,2,(char*)"Err2");
	}
	GUI_window_hwin_release(errWIN[1]);  
    return;
}

static void _ERRARG_display(hwin_t*pOBJ)
{
	DL_app_user_set_t* pARG = &g_user_arg.DL_app_user_set;
	GUI_window_hwin_req( pOBJ );
	GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
	GUI_set_Fgcolor( XCerrmod_FG_COLOR );	
	GUI_edit_string_display_At(pARG->bb1,CT_BL);
	GUI_edit_string_display_At(pARG->cs, METER1_CS);
	GUI_edit_string_display_At(pARG->cs2,METER2_CS);
	GUI_edit_string_display_At(pARG->qs, METER1_QS);
	GUI_edit_string_display_At(pARG->qs2,METER2_QS);
	GUI_radio_select(_2radio[0],g_user_arg.DL_app_user_set.meter_1OR3);
	GUI_radio_select(_2radio[1],g_user_arg.DL_app_user_set.handOR_GD);
    GUI_drop_list_member_assign(_droplistPQ,g_user_arg.DL_app_user_set.PorQ);
    GUI_drop_list_member_assign(_droplist,g_user_arg.DL_app_user_set.dlsr);
    GUI_drop_list_member_assign(_droplistDJ,g_user_arg.DL_app_user_set.meterDJ);
	GUI_window_hwin_release( pOBJ );
    return;
}

static void _radio_scan(void)
{
     Bsize_t id;
     id=GUI_radio_curmember_obtain(_2radio[0]);
     g_user_arg.DL_app_user_set.meter_1OR3 = id;
     id=GUI_radio_curmember_obtain(_2radio[1]);
     g_user_arg.DL_app_user_set.handOR_GD = id;
	 app_global_set_user_dl_arg(&g_user_arg);
	 fatfs_writeARG("USERDATA",&g_user_arg,sizeof(DL_app_user_all_info_t));
	 _ERRWIN_Reserve(_errWin,_errWinR,g_user_arg.DL_app_user_set.meter_1OR3);
	 return;
}

static void _dlsr_scan(void)
{
     Bsize_t DLSR;
     os_driver_ioctl(g_DL_dev,MEASURE_CMD_GET_REAL_DLSR,&DLSR);
	 if(DLSR!=g_user_arg.DL_app_user_set.dlsr)
	 {
         g_user_arg.DL_app_user_set.dlsr = DLSR;
		 GUI_drop_list_member_assign(_droplist,g_user_arg.DL_app_user_set.dlsr);
	 }
	 return;
}



static void _QS_match(measure_param_t  *dl)
{
	float 	fk = 1.0;
	float   beilv = 1.0;	
	int     N = 0;

	fk = (float)atol(g_user_arg.DL_app_user_set.cs);
	N=_Qs_select(dl->f_Pabc,fk);
	if(g_user_arg.DL_app_user_set.dlsr>QB_N_20A)
	{
		beilv = global_atof(g_user_arg.DL_app_user_set.bb1);
		if(beilv != 0)
		{
			N /= beilv;
		}
	}
	if(N == 0) N = 1;
	global_ltoa(N,g_user_arg.DL_app_user_set.qs);
	
	
	fk = (float)atol(g_user_arg.DL_app_user_set.cs2);
	N=_Qs_select(dl->f_Qabc,fk);
	if(g_user_arg.DL_app_user_set.dlsr>QB_N_20A)
	{
		beilv = global_atof(g_user_arg.DL_app_user_set.bb1);
		if(beilv != 0)
		{
			N/=beilv;
		}
	}
	if(N == 0) N = 1;
	global_ltoa(N,g_user_arg.DL_app_user_set.qs2);
	app_global_set_user_dl_arg(&g_user_arg);
	fatfs_writeARG("USERDATA",&g_user_arg,sizeof(DL_app_user_all_info_t));
}



static void _reset_dl_param(void)
{
	GUI_WINdo();
	g_user_arg.DL_app_user_set.dlsr    = GUI_drop_list_member_obtain(_droplist);
	g_user_arg.DL_app_user_set.meterDJ = GUI_drop_list_member_obtain(_droplistDJ);
	g_user_arg.DL_app_user_set.PorQ    = GUI_drop_list_member_obtain(_droplistPQ);
//	_radio_scan();
	return;
}

void app_ERR_task(void*arg)
{
    uint8_t key;
	int     circle[3],last_circle[3];
	_edit = _PaintWIN(m_XCERR_ERR_win);
	m_ERR_pause = TURE;
	GUI_MsgFIFO_create(_edit,2);
	GUI_MsgFIFO_empty(_edit);
	_ERRARG_display(_edit);
	GUI_WINready(GUI_Getchild(KEY_METER_SELECT,_edit),1);
	//_radio_scan();
	_reset_dl_param();		//纠正返回主模块时，一定要按确认键的问题
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
             if(key==KEY_CONST_SELECT)
             {
                 _Constselect();   
				 app_global_set_user_dl_arg(&g_user_arg);
				 fatfs_writeARG("USERDATA",&g_user_arg,sizeof(DL_app_user_all_info_t));
				 _ERRARG_display(_edit);
			 }
			 else if(key==KEY_QS_SELECT)
			 {
			      _QS_match(&g_DL_param);
				  _ERRARG_display(_edit);
			 }
			 else if(key==KEY_CS_CHECK)
			 {
			       app_XCERRmod_check_entry();
				  _ERRARG_display(_edit);
			 }
			 else if(key==KEY_I_IN)
			 {
                  g_user_arg.DL_app_user_set.dlsr=GUI_drop_list_unfold(_droplist);
				  app_global_set_user_dl_arg(&g_user_arg);
				  fatfs_writeARG("USERDATA",&g_user_arg,sizeof(DL_app_user_all_info_t));
			 }
			 else if(key==KEY_METER_SELECT||key==KEY_GD_SELECT)
			 {
                  _radio_scan();
				  if(g_user_arg.DL_app_user_set.meter_1OR3==1)
				  {
                      g_user_arg.DL_app_user_set.PorQ = 2;
					  GUI_drop_list_member_assign(_droplistPQ,g_user_arg.DL_app_user_set.PorQ);
				  }
				  else 
				  {
                      g_user_arg.DL_app_user_set.PorQ = 0;
					  GUI_drop_list_member_assign(_droplistPQ,g_user_arg.DL_app_user_set.PorQ);
				  }
				  app_global_set_user_dl_arg(&g_user_arg);
				  fatfs_writeARG("USERDATA",&g_user_arg,sizeof(DL_app_user_all_info_t));
			 }
			 else if(key==KEY_CONST_SETTING)
			 {
			      _ERRARG_setting();
				  fatfs_writeARG("USERDATA",&g_user_arg,sizeof(DL_app_user_all_info_t));
				  app_global_set_user_dl_arg(&g_user_arg);
				  _ERRARG_display(_edit);
				  _ERRWIN_Reserve(_errWin,_errWinR,g_user_arg.DL_app_user_set.meter_1OR3);
			 }
			 else if(KEY_PAUSE==key)
			 {
				  _pause_view(_errWinR[0]);
			 }
			  else if(key==KEY_RIGHT)
			  {
			  	if(m_CURwidget!=_droplist)
                  	GUI_WINshift(0);
			  }
			  else if(key==KEY_LEFT)
			  {
			  	if(m_CURwidget!=GUI_Getchild(KEY_METER_SELECT,_edit)
					||m_pMember!=m_CURwidget->subord_head)
                  GUI_WINshift(1);
			  }			 
			  else if(key==KEY_ENTER)
			  {
                 GUI_WINdo();
				 g_user_arg.DL_app_user_set.dlsr    = GUI_drop_list_member_obtain(_droplist);
				 g_user_arg.DL_app_user_set.meterDJ = GUI_drop_list_member_obtain(_droplistDJ);
				 g_user_arg.DL_app_user_set.PorQ    = GUI_drop_list_member_obtain(_droplistPQ);
				_radio_scan();
			  }	
		}
		if(m_ERR_pause)
		{
			_ErrExc(circle,last_circle,_errWinR);
		}
		_dlsr_scan();
	}
}

void  app_XCERR_errPAINT(void)
{
     Bsize_t id;
	_edit = _PaintWIN(m_XCERR_ERR_win);
	_ERRARG_display(_edit);
     id=GUI_radio_curmember_obtain(_2radio[0]);
     g_user_arg.DL_app_user_set.meter_1OR3 = id;
     id=GUI_radio_curmember_obtain(_2radio[1]);
     g_user_arg.DL_app_user_set.handOR_GD = id;
	 app_global_set_user_dl_arg(&g_user_arg);
	 _ERRWIN_Reserve(_errWin,_errWinR,g_user_arg.DL_app_user_set.meter_1OR3);
	_Err_display(g_DL_param.f_path1_err,_errWin[0],3);
	if ( g_user_arg.DL_app_user_set.meter_1OR3==1)
	{
		_Err_display(g_DL_param.f_path2_err,_errWin[1],3);
	}
	return;
}


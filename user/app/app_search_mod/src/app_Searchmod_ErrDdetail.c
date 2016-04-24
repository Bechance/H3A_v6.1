/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:查询模块主体程序
* 创建时间:
* 所属公司 :科荟
* 文件名称:app_Searchmod_detail.c
* 作者 :
* 功能描述:实现校表数据查询
******************************************************************************************/
#include 	"app_Searchmod_inside.h"
static       hwin_t*   _pUset;
static       hwin_t*   _pDLWIN;
static       hwin_t*   _pLJT;
static 		 hwin_t*   _edit         = 0;
static 	     hwin_t*   _2radio[2]    ={0,0};
static 		 hwin_t*   _droplist     = 0;
static 	     hwin_t*   _droplistDJ   = 0;
static 		 hwin_t*   _droplistPQ   = 0;
static 		 hwin_t*   _errWin[2]  ={0,0};
/*********************************************************************/
static 		 const  char*   m_searchmod_MSG_01[2][7]={
	{"1级","2级","3级","0.5级", "0.5S级","0.2级","0.2S级"},
	{"1级","2级","3级","0.5级", "0.5S级","0.2级","0.2S级"}
};
static 		 const  char*_sr[2]={"输入 ",""};
static 		 const  char*_qs1[2]={"圈数a", ""};
static 		 const  char*_cs1[2]={"常数a", ""};
static 		 const  char*_qs2[2]={"圈数b", ""};
static 		 const  char*_cs2[2]={"常数b", ""};
static 		 const  char*_bl[2]={" 倍率",""};
static 		 const  char*_db[2]={"单表", ""};
static 		 const  char*_sb[2]={"双表", ""};
static 		 const  char*_ywgOP[2][3]={{"有功误差","无功误差","有功无功"},{"   P","   Q","   P_Q",}};
static 		 const  char*_gd[2]={"光电", ""};
static 		 const  char*_sd[2]={"手动", ""};
static 		 const  char*_STRDJ[2]  ={"等级",""};
static 		 const  char*_szpl[2]   ={"时钟频率",""};
static 		 const  char*_clqs[2]   ={"测量圈数",""};
static 		 const  char*_rjswc[2]  ={"日记时误差:",""};
static 		 const  char*_syjl[2]   ={"试验结论:",""};


static void _SDLmember_window_init(hwin_t*hp)
{
#define   DL_XLAB 15
#define   DL_GAP  90
#define   DL_VGAP 32
	GUI_window_hwin_req(hp);
	GUI_set_Bgcolor(m_GUI_3D_window_Bg_cor);
    GUI_fill_window_area(4,2,hp->win_wide,DL_VGAP);
    GUI_set_Fgcolor(Searchmod_FG_COLOR);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_string_display_At(24+DL_XLAB+DL_GAP*0, 2, "U(V)" );
	GUI_string_display_At(24+DL_XLAB+DL_GAP*1, 2, "I(A)" );
	GUI_string_display_At(12+DL_XLAB+DL_GAP*2, 2, "Φ(°)" );
	GUI_string_display_At(24+DL_XLAB+DL_GAP*3, 2, "P(W)" );
	GUI_string_display_At(12+DL_XLAB+DL_GAP*4, 2, "Q(Var)" );
	GUI_string_display_At(30+DL_XLAB+DL_GAP*5, 2, "ΦABC(°)" );

	GUI_set_Bgcolor(Searchmod_WIN_BG_COLOR);
    GUI_set_Fgcolor(Searchmod_FG_COLOR);
	GUI_set_Fgcolor( C_GUI_YELLOW );
	GUI_string_display_At(2,DL_VGAP*1+2, "A" );
	GUI_set_Fgcolor( C_GUI_GREEN );
	GUI_string_display_At(2,DL_VGAP*2+2, "B" );
	GUI_set_Fgcolor( C_GUI_RED );
	GUI_string_display_At(2,DL_VGAP*3+2, "C" );
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*0,DL_VGAP*1, 7, "", MEMBER_3D | EDIT_U_A);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*0,DL_VGAP*2, 7, "", MEMBER_3D | EDIT_U_B);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*0,DL_VGAP*3, 7, "", MEMBER_3D | EDIT_U_C);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*1,DL_VGAP*1, 7, "", MEMBER_3D | EDIT_I_A);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*1,DL_VGAP*2, 7, "", MEMBER_3D | EDIT_I_B);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*1,DL_VGAP*3, 7, "", MEMBER_3D | EDIT_I_C);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*2,DL_VGAP*1, 7, "", MEMBER_3D | EDIT_J_A);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*2,DL_VGAP*2, 7, "", MEMBER_3D | EDIT_J_B);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*2,DL_VGAP*3, 7, "", MEMBER_3D | EDIT_J_C);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*3,DL_VGAP*1, 7, "", MEMBER_3D | EDIT_P_A);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*3,DL_VGAP*2, 7, "", MEMBER_3D | EDIT_P_B);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*3,DL_VGAP*3, 7, "", MEMBER_3D | EDIT_P_C);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*4,DL_VGAP*1, 7, "", MEMBER_3D | EDIT_Q_A);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*4,DL_VGAP*2, 7, "", MEMBER_3D | EDIT_Q_B);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*4,DL_VGAP*3, 7, "", MEMBER_3D | EDIT_Q_C);
	GUI_set_Fgcolor(Searchmod_FG_COLOR);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*3,DL_VGAP*4, 7, "",  MEMBER_3D | EDIT_PABC);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*4,DL_VGAP*4, 7, "",  MEMBER_3D | EDIT_QABC);
	GUI_edit_memeber_add(2,DL_VGAP*4,7,   "cosΦ", MEMBER_3D | EDIT_COS );
	GUI_edit_memeber_add(159,DL_VGAP*4,6, "F", MEMBER_3D | EDIT_FEQ);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
    GUI_string_display_At(DL_XLAB+DL_GAP*3-16,DL_VGAP*4+4, "∑");
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_edit_memeber_add(5+DL_XLAB+DL_GAP*5,DL_VGAP*1,7, "ΦUab", MEMBER_3D | EDIT_JUAB );
	GUI_edit_memeber_add(5+DL_XLAB+DL_GAP*5,DL_VGAP*2,7, "ΦUac", MEMBER_3D | EDIT_JUAC);
	GUI_edit_memeber_add(5+DL_XLAB+DL_GAP*5,DL_VGAP*3,7, "ΦUbc", MEMBER_3D | EDIT_JUBC);
	GUI_edit_memeber_add(5+DL_XLAB+DL_GAP*5,DL_VGAP*4,7, "ΦIac", MEMBER_3D | EDIT_JIAC);
	GUI_WINDIS_VLINE(DL_XLAB+DL_GAP*5+5,DL_VGAP*1,DL_VGAP*5);
	GUI_WINDIS_HLINE(0,hp->win_wide,DL_VGAP*5);
	GUI_window_hwin_release(hp);
    return;
}

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

static void _SUSER_window_init(hwin_t*hp,Errsave_t*p)
{
	hwin_t*pOBJ;
	int    ytem;
	GUI_window_hwin_req(hp);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_set_Bgcolor(Searchmod_WIN_BG_COLOR);    
	GUI_set_Fgcolor(Searchmod_FG_COLOR); 
	if ( p->DL_app_user_set.ERRtype == 3 )
	{
		GUI_string_display_At(5,1,"时钟误差频率定义为:表时钟脉冲输出间隔如 1Hz");
		GUI_WINDIS_HLINE(5, hp->win_wide, 35);
		GUI_edit_memeber_add(5,45,5,"时钟频率",MEMBER_3D|TMR1_CS);
		GUI_edit_memeber_add(5,85,5,"测量圈数",MEMBER_3D|METER1_QS);
		GUI_string_display_At(170,48,"Hz");
		GUI_string_display_At(220,105,"日计时误差:");
		GUI_edit_memeber_add(220,140,8,"",MEMBER_3D|ERR_DAY);
		GUI_string_display_At(325,142,"S/d");
		GUI_string_display_At(220,180,"试验结论:");
		GUI_edit_memeber_add(220,210,8,"",MEMBER_3D|ERR_DAY_RS);		
	}
	else
	{
		GUI_edit_memeber_add(190,2+34*1,8,(char*)_bl[g_language], MEMBER_3D|CT_BL);
		GUI_edit_memeber_add(0  ,2+34*2,8,(char*)_cs1[g_language],MEMBER_3D|METER1_CS);
		GUI_edit_memeber_add(190,2+34*2,8,(char*)_cs2[g_language],MEMBER_3D|METER2_CS);
		GUI_edit_memeber_add(0  ,2+34*3,8,(char*)_qs1[g_language],MEMBER_3D|METER1_QS);
		GUI_edit_memeber_add(190,2+34*3,8,(char*)_qs2[g_language],MEMBER_3D|METER2_QS);
	    GUI_string_display_At(15,6+34*1,  (char*)_sr[g_language]);

		pOBJ = GUI_radio_create(2,2,160,30,WIN_MSG_EN|WIN_PAINT|0,hp);
		ytem = (pOBJ->win_high-GUI_conText.CUR_Front->high)/2;
	    GUI_window_hwin_req(pOBJ);
		GUI_radio_memeber_add(5,ytem,(char*)_db[g_language],0);
		GUI_radio_memeber_add(80,ytem,(char*)_sb[g_language],1);
		GUI_radio_select(pOBJ,0);
	    GUI_window_hwin_release(pOBJ);	   
		_2radio[0] = pOBJ;

		pOBJ = GUI_radio_create(167,2,160,30,WIN_MSG_EN|WIN_PAINT|1,hp);
	    GUI_window_hwin_req(pOBJ);
		GUI_radio_memeber_add(5,  ytem,(char*)_gd[g_language],0);
		GUI_radio_memeber_add(80, ytem,(char*)_sd[g_language],1);
		GUI_radio_select(pOBJ, 0);
	    GUI_window_hwin_release(pOBJ);	 
		_2radio[1] = pOBJ;
		GUI_SetFront(&GUI_HZK24_EXTASCII);
	    _droplistPQ=Gui_drop_list_creat(342,2,_ywgOP[g_language],SIZE_OF_ARRAY(_ywgOP[g_language]),0,hp);
	    _droplistDJ=Gui_drop_list_creat(502,2,m_searchmod_MSG_01[g_language],SIZE_OF_ARRAY(m_searchmod_MSG_01[g_language]),0,hp);
	    _droplist  =Gui_drop_list_creat(63, 2+34*1,g_dlsr_option[g_language],SIZE_OF_ARRAY(g_dlsr_option[g_language]),0,hp);
	}
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	_errWin[0]=_PaintERR(hp,5,hp->win_high-104,160,  "Err1:");
	_errWin[1]= 0;
	if(p->DL_app_user_set.meter_1OR3!=0)
		_errWin[1]=_PaintERR(hp,193,hp->win_high-104,160,"Err2:");
	GUI_WINDIS_VLINE(375,36,hp->win_high);
	GUI_set_Bgcolor(0X10A3);  
	_pLJT= GUI_window_child_creat(395,36,225,205,WIN_FILL,hp);
	GUI_window_hwin_release(hp);
	return;
}
/****************************************************
* @作者 :揭成
* @功能 :记录明细窗口创建
* @输入 :
* @输出 :NONE
****************************************************/
static void record_detailWIN_create(hwin_t*pOBJ,Errsave_t*p)
{
	GUI_window_hwin_req(pOBJ);
	_pUset = GUI_edit_creat(2,0,630,242,0,pOBJ);
    _SUSER_window_init(_pUset,p);
	_pDLWIN  = GUI_edit_creat(2,242,630,210,0,pOBJ);
	_SDLmember_window_init(_pDLWIN);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_set_Bgcolor(Searchmod_WIN_BG_COLOR);    
	GUI_set_Fgcolor(Searchmod_FG_COLOR); 
	GUI_string_display_At(10,pOBJ->win_high-30,"表号:");
	GUI_string_display(p->DL_app_user_info.meter_id1);
	GUI_string_display_At(320,pOBJ->win_high-30,"误差类型:");
	if ( p->DL_app_user_set.ERRtype == 3 )
		GUI_string_display("基频误差");
	else
		GUI_string_display("校验误差");
	GUI_window_hwin_release(pOBJ);
    return;
}
/*****************************************************/
/*USER SETTING DISPLAY                               */
/*****************************************************/
static void _USERsetting_view(DL_app_user_set_t*p)
{
	float m_error = global_atof(p->qs3);
	GUI_window_hwin_req( _pUset );
	GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
	GUI_set_Fgcolor( Searchmod_FG_COLOR );
	if ( p->ERRtype == 3 )
	{
		GUI_edit_string_display_At(p->TMRcs, TMR1_CS);
		GUI_edit_string_display_At(p->qs, METER1_QS);
		GUI_edit_string_display_At(p->qs3, ERR_DAY);
		if ( GLOBAL_ABS(m_error) > 0.5 )
			GUI_edit_string_display_At("不合格", ERR_DAY_RS);
		else if ( m_error != 0 )
			GUI_edit_string_display_At("合格", ERR_DAY_RS);
	}
	else
	{
		GUI_edit_string_display_At(p->bb1,CT_BL);
		GUI_edit_string_display_At(p->cs, METER1_CS);
		GUI_edit_string_display_At(p->cs2,METER2_CS);
		GUI_edit_string_display_At(p->qs, METER1_QS);
		GUI_edit_string_display_At(p->qs2,METER2_QS);
		GUI_radio_select(_2radio[0],p->meter_1OR3);
		GUI_radio_select(_2radio[1],p->handOR_GD);
	    GUI_drop_list_member_assign(_droplistPQ,p->PorQ);
	    GUI_drop_list_member_assign(_droplist,p->dlsr);
	    GUI_drop_list_member_assign(_droplistDJ,p->meterDJ);
	}
	GUI_window_hwin_release( _pUset );
    return;
}

static void _DLarg_display(DL_app_param_str_t*pDL)
{
//	char temp[10]="";
//	char ch[2]="";
	GUI_window_hwin_req(_pDLWIN);
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_DLA_COLOR);
	GUI_edit_string_display_At(pDL->phase_a.c_U,EDIT_U_A);
	GUI_edit_string_display_At(pDL->phase_a.c_I,EDIT_I_A);
	GUI_edit_string_display_At(pDL->phase_a.c_J,EDIT_J_A);
	GUI_edit_string_display_At(pDL->phase_a.c_P,EDIT_P_A);
	GUI_edit_string_display_At(pDL->phase_a.c_Q,EDIT_Q_A);
	GUI_set_Fgcolor(C_GUI_DLB_COLOR);
	GUI_edit_string_display_At(pDL->phase_b.c_U,EDIT_U_B);
	GUI_edit_string_display_At(pDL->phase_b.c_I,EDIT_I_B);
	GUI_edit_string_display_At(pDL->phase_b.c_J,EDIT_J_B);
	GUI_edit_string_display_At(pDL->phase_b.c_P,EDIT_P_B);
	GUI_edit_string_display_At(pDL->phase_b.c_Q,EDIT_Q_B);
	GUI_set_Fgcolor(C_GUI_DLC_COLOR);
	GUI_edit_string_display_At(pDL->phase_c.c_U,EDIT_U_C);
	GUI_edit_string_display_At(pDL->phase_c.c_I,EDIT_I_C);
	GUI_edit_string_display_At(pDL->phase_c.c_J,EDIT_J_C);
	GUI_edit_string_display_At(pDL->phase_c.c_P,EDIT_P_C);
	GUI_edit_string_display_At(pDL->phase_c.c_Q,EDIT_Q_C);
	GUI_set_Fgcolor(C_GUI_YELLOW);
	GUI_edit_string_display_At(pDL->c_Pabc,EDIT_PABC);
	GUI_edit_string_display_At(pDL->c_Qabc,EDIT_QABC);
	GUI_edit_string_display_At(pDL->c_JUab,EDIT_JUAB);
	GUI_edit_string_display_At(pDL->c_JUbc,EDIT_JUBC);
	GUI_edit_string_display_At(pDL->c_JUac,EDIT_JUAC);
	GUI_edit_string_display_At(pDL->c_JIac,EDIT_JIAC);
	GUI_edit_string_display_At(pDL->c_Feq, EDIT_FEQ);
	GUI_edit_string_display_At(pDL->c_Cos, EDIT_COS);
	GUI_window_hwin_release(_pDLWIN);
	return;
}
/*****************************************************/
/*ERR  DISPLAY                                       */
/*****************************************************/
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
/*LJT  DISPLAY*/
static 		 uint16_t  _x0;		/*圆点X坐标*/
static 		 uint16_t  _y0;		/*圆点Y坐标*/
static       uint16_t  _Bg = 0;  
static       uint16_t  _Fg = 0;  
static       uint16_t  _lenMAX  = 110; 
static       uint16_t  _lenMID  = 70; 
static       int 	   _Rcircle;
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
 * @功能 :六角图显示
 *
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
static void _LJT_display( hwin_t*hp, measure_param_t*Pdl,uint8_t phase3or4)
{
	float angle_Ua;
	float angle_Ub;
	float angle_Uc;
	float angle_I;
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
 * @功能   :六角图参数初始化
 * @输入   :hp  六角图显示框句柄
 * @输出   :NONE
********************************************************************/
void _LJT_init( hwin_t*hp)
{
    float flen;
	GUI_window_hwin_req(hp);
	_Bg 	 = GUI_GetwinBgcor();
	_Fg 	 = 0X2145; 
	_lenMAX  = (MIN(hp->win_high,hp->win_wide))/2-5;
	_x0      = ( hp->win_wide ) / 2; 
	_y0      = ( hp->win_high ) / 2;
    flen 	 = (float)(MIN(hp->win_wide,hp->win_high));
	flen 	 = flen*2/6;
	_lenMID  = (uint16_t)flen;
	GUI_drawcircle(hp->win_x+_x0,hp->win_y+_y0,_lenMAX,_Fg);
	GUI_drawcircle(hp->win_x+_x0,hp->win_y+_y0,_lenMID,_Fg);
	GUI_window_hwin_release(hp);
	return;
}


static void _DLstr_to_DLf(DL_app_param_str_t*pSrc,measure_param_t*pDes)
{
	float*  pfdata;
	char*   pch;
	Bsize_t offset;
	GLOBAL_MEMCLR(pDes, sizeof(measure_param_t));
	for(offset=0;offset <(sizeof(measure_param_t)/sizeof(float)-1);offset++ )
	{
		pfdata =&(pDes->phase_a.f_U );
		pch    = (pSrc->phase_a.c_U);
		pfdata =  pfdata+offset;
		pch   += ((DL_DSIZE)*offset);
		global_string_filter(pch,' ');
	   *pfdata = global_atof(pch);
	}
	return;
}

/*接线模式分析*/
static void _JXmod_sys(measure_param_t*pdl,uint8_t*jxMOD)
{
	if (( GLOBAL_ABS( pdl->phase_a.f_U - 380 ) < 45 ) 
		|| ( GLOBAL_ABS( pdl->phase_a.f_U - 100 ) < 20 ) 
		|| ( GLOBAL_ABS( pdl->phase_c.f_U - 380 ) < 45 ) 
		|| ( GLOBAL_ABS( pdl->phase_c.f_U - 100 ) < 20 ) )
	{
		if ( pdl->phase_b.f_U < 45 )
		{
			*jxMOD = JXMOD_3_3;
			return;
		}
	}
	*jxMOD = JXMOD_3_4;
	return;
}
/****************************************************
* @作者 :揭成
* @功能 :显示明细
* @输入 :
* @输出 :NONE
****************************************************/
void ERRDrecord_detail(void*data)
{
    uint8_t key;
    hwin_t*  pOBJ;
	uint8_t  JXmod;
	Errsave_t*p = (Errsave_t*)data;
	measure_param_t mDL_s;

	GUI_window_hwin_req(GET_GUIWIN);
	GUI_set_Bgcolor( Searchmod_BG_COLOR );
	GUI_clean_screen();
	GUI_set_Bgcolor( Searchmod_WIN_BG_COLOR );
	GUI_set_Fgcolor( Searchmod_FG_COLOR );
	
    pOBJ = Gui_3D_window_creat(0, 0, LCD_WIDE-2,LCD_HIGH,"信息详情",WIN_MSG_EN|WIN_3D|WIN_FILL,0);
	record_detailWIN_create(pOBJ,p);
	_LJT_init(_pLJT);
	GUI_window_hwin_release(GET_GUIWIN);
    _DLstr_to_DLf(&p->DLarg,&mDL_s);
	_JXmod_sys(&mDL_s,&JXmod);
    _LJT_display(_pLJT,&mDL_s,JXmod);
	_Err_display(mDL_s.f_path1_err,_errWin[0],4);
	_Err_display(mDL_s.f_path2_err,_errWin[1],4);
	_DLarg_display(&p->DLarg);
	_USERsetting_view(&p->DL_app_user_set);
	app_global_key_control_power_req();
    while(1)
    {
        msleep(1);
		if ( app_global_key_force_obtain( &key, 1 )== 1 )
		{
			if(key==GUI_KEY_3D_WINDOW_CLOSE)break;
			if(key==KEY_EXIT)break;
		}
	}
	GUI_window_destory(pOBJ);
	app_global_key_control_power_release();
	return;
}

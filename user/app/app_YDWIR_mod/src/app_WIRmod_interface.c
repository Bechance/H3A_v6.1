/********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:系统设置模块界面函数集合
* 创建时间:2010-12-19
* 所属公司 :科荟
* 文件名称:app_WIRmod_interface.c
* 创建人:揭成
* 功能描述:提供系统设置界面画屏函数API
******************************************************************************************/
#include"app_WIRmod_inside.h"
hwin_t* m_WIR_win 			= PNULL;
hwin_t* m_WIR_info_win 		= PNULL;
hwin_t* m_WIRdraw_win 		= PNULL;
hwin_t* m_WIR_info_EDIT 	= PNULL;
hwin_t* m_WIRresjult_win 	= PNULL;

draw_coord_t  m_Mcoord;
draw_coord_t  m_HGQcoord;
ELEcomponent_t*	m_WIRcomp_ground1;
ELEcomponent_t*	m_WIRcomp_ground2;
ELEcomponent_t*	m_WIRcomp_ground3;
ELEcomponent_t*	m_meter1;
ELEcomponent_t*	m_meter2;
ELEcomponent_t*	m_meter3;
ELEcomponent_t*	m_UHGQ_3_3;
ELEcomponent_t*	m_UHGQ_3_4;
ELEcomponent_t*	m_IHGQ1;
ELEcomponent_t*	m_IHGQ2;
ELEcomponent_t*	m_IHGQ3;
ELEcomponent_t*	m_PA;
ELEcomponent_t*	m_PB;
ELEcomponent_t*	m_PC;
void ELEcomponent_link_point( ELEcomponent_t*pcomp1, uint8_t branch1, uint16_t y );
/*元件位置*/
#define  		METER1_X     	160
#define  		METER1_Y      	10

#define  		METER2_X     	260
#define  		METER2_Y      	20

#define  		METER3_X     	360
#define  		METER3_Y      	30

#define  		UHGQ_3_3_X      60
#define  		UHGQ_3_3_Y      190
#define  		UHGQ_3_4_X      60
#define  		UHGQ_3_4_Y      190

#define  		L1Y    			240                         /*L1 起始Y*/

#define  		IHGQ1_X        	(METER1_X-2)
#define  		IHGQ1_Y         (L1Y-18)

#define  		IHGQ2_X        	(METER2_X-2)
#define  		IHGQ2_Y         (IHGQ1_Y+30)

#define  		IHGQ3_X        	(METER3_X-2)
#define  		IHGQ3_Y         (IHGQ2_Y+30)

#define         GROUND1_X        (UHGQ_3_3_X+60)
#define         GROUND1_Y        (UHGQ_3_3_Y+1)
#define         GROUND2_X        (METER3_X+60)
#define         GROUND2_Y        (METER3_Y+62)
#define         GROUND3_X        (METER3_X+60)
#define         GROUND3_Y        (METER1_Y)


#define         L1_POINT_X        (GET_UHGQ3_3_X1(UHGQ_3_3_X))
#define         L1_POINT_Y        (L1Y)
#define         L2_POINT_X        (GET_UHGQ3_3_X2(UHGQ_3_3_X))
#define         L2_POINT_Y        (L1_POINT_Y+30)
#define         L3_POINT_X        (GET_UHGQ3_3_X3(UHGQ_3_3_X))
#define         L3_POINT_Y        (L2_POINT_Y+30)

#define        UB_POINT_X        ((GET_MP_X2(METER1_X)+GET_MP_X2(METER2_X))/2)
#define        UB_POINT_Y        (GET_MP_Y1(METER1_Y))
/*中英对照*/
const char *LostUaTitle[] = {"A相失压", "Lost Ua "};
const char *LostUbTitle[] = {"B相失压", "Lost Ub "};
const char *LostUcTitle[] = {"C相失压", "Lost Uc "};
const char *LostIaTitle[] = {"A相失流", "Lost Ia "};
const char *LostIbTitle[] = {"B相失流", "Lost Ib "};
const char *LostIcTitle[] = {"C相失流", "Lost Ic "};
const char *zhengXU[]   =   {"正相序", "Phase+"};
const char *niXU[] 	   =    {"逆相序", "Phase-"};
const char *m_WIR_cos_option[] = {"感>0.5L", "感<=0.5L", "容>0.5C","容<=0.5C","等于1"};
/********************************************************************
 * @创建人 :揭成
 * @功能 :创建画图元件管理标识
 *
 * @输入 :flag     3相3线或3相4线
 *
 *@输出  :NONE
********************************************************************/
void app_WIRmod_comp_create( uint8_t flag )
{
	ELEcomponent_destory_all();
	m_WIRcomp_ground3 = ELEcomponent_creat( GROUND3_X, GROUND3_Y, GROUND, 3 );
	if ( flag == JXMOD_3_3 )
	{
		m_IHGQ1   = ELEcomponent_creat( IHGQ1_X, IHGQ1_Y, IHGQ, 0 );
		m_IHGQ2   = ELEcomponent_creat( IHGQ2_X, IHGQ3_Y, IHGQ, 0 );
		m_PB      = ELEcomponent_creat( UB_POINT_X, UB_POINT_Y, ELE_POINT, 0 );
		m_meter1  = ELEcomponent_creat( METER1_X, METER1_Y, METER_POINT, 0 );
		m_meter2  = ELEcomponent_creat( METER2_X, METER2_Y, METER_POINT, 0 );
		m_UHGQ_3_3= ELEcomponent_creat( UHGQ_3_3_X, UHGQ_3_3_Y, UHGQ_3_3, 0 );
	}
	else
	{
		m_meter1  = ELEcomponent_creat( METER1_X - 5, METER1_Y, METER_POINT, 0 );
		m_meter2  = ELEcomponent_creat( METER2_X - 5, METER1_Y, METER_POINT, 0 );
		m_meter3  = ELEcomponent_creat( METER3_X - 5, METER1_Y, METER_POINT, 0 );
		m_IHGQ1   = ELEcomponent_creat( IHGQ1_X - 5, IHGQ1_Y, IHGQ, 0 );
		m_IHGQ2   = ELEcomponent_creat( IHGQ2_X - 5, IHGQ2_Y, IHGQ, 0 );
		m_IHGQ3   = ELEcomponent_creat( IHGQ3_X - 5, IHGQ3_Y, IHGQ, 0 );
		m_UHGQ_3_4= ELEcomponent_creat( UHGQ_3_4_X, UHGQ_3_4_Y, UHGQ_3_4, 0 );
		m_WIRcomp_ground1 = ELEcomponent_creat( GROUND1_X + 10, GROUND1_Y+5, GROUND, 1 );
		m_WIRcomp_ground2 = ELEcomponent_creat( GROUND2_X, 	 GROUND2_Y, GROUND, 3 );	
		m_PA = ELEcomponent_creat( GET_MP_X3( m_meter1->x ), GROUND2_Y, ELE_POINT, 0 );
		m_PB = ELEcomponent_creat( GET_MP_X3( m_meter2->x ), GROUND2_Y, ELE_POINT, 0 );
		m_PC = ELEcomponent_creat( GET_MP_X3( m_meter3->x ), GROUND2_Y, ELE_POINT, 0 );
	}
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :初始连线
 *
 * @输入 :flag     3相3线或3相4线
 *
 *@输出  :NONE
********************************************************************/
void app_WIRmod_link_init( uint8_t flag )
{
	if ( flag == JXMOD_3_3 )
	{
		ELEcomponent_ponit_display( L1_POINT_X, L1_POINT_Y );
		ELEcomponent_ponit_display( L2_POINT_X, L2_POINT_Y );
		ELEcomponent_ponit_display( L3_POINT_X, L3_POINT_Y );
		
		ELEcomponent_link( m_meter1, 1, m_PB, 1 );
	//	ELEcomponent_link( m_meter2, 1, m_PB, 1 );
		ELEcomponent_link( m_PB, 1, m_WIRcomp_ground3, 1 );
		ELEcomponent_point_link(GET_MP_X2(METER2_X),GET_MP_Y1(METER2_Y), GET_MP_X2(METER2_X), METER1_Y);		
		ELEcomponent_point_link( GET_UHGQ3_3_X1( UHGQ_3_3_X ), GET_UHGQ3_3_Y2( UHGQ_3_3_Y ), L1_POINT_X, L1_POINT_Y );
		ELEcomponent_point_link( GET_UHGQ3_3_X2( UHGQ_3_3_X ), GET_UHGQ3_3_Y2( UHGQ_3_3_Y ), L2_POINT_X, L2_POINT_Y );
		ELEcomponent_point_link( GET_UHGQ3_3_X3( UHGQ_3_3_X ), GET_UHGQ3_3_Y2( UHGQ_3_3_Y ), L3_POINT_X, L3_POINT_Y );
	}
	else
	{
		ELEcomponent_link( m_meter1, 1, m_WIRcomp_ground3, 1 );
		ELEcomponent_link( m_meter2, 1, m_WIRcomp_ground3, 1 );
		ELEcomponent_link( m_meter3, 1, m_WIRcomp_ground3, 1 );

		
		ELEcomponent_link( m_meter1, 4, m_PA, 1 );
		ELEcomponent_link( m_meter2, 4, m_PB, 1 );
		ELEcomponent_link( m_meter3, 4, m_PC, 1 );
		ELEcomponent_link( m_PA, 1, m_PB, 1 );
		ELEcomponent_link( m_PB, 1, m_PC, 1 );
		ELEcomponent_link( m_PC, 1, m_WIRcomp_ground2, 1 );
		ELEcomponent_link_point( m_UHGQ_3_4, 7, L1Y);
		ELEcomponent_link_point( m_UHGQ_3_4, 9, L1Y + 30 );
		ELEcomponent_link_point( m_UHGQ_3_4, 11,L1Y + 30*2 );
		ELEcomponent_link_point( m_UHGQ_3_4, 8, L1Y+ 30*3 );
		ELEcomponent_link_point( m_UHGQ_3_4, 10,L1Y+ 30*3 );
		ELEcomponent_link_point( m_UHGQ_3_4, 12,L1Y+ 30*3 );
	//	ELEcomponent_link( m_UHGQ_3_4, 6, m_WIRcomp_ground1, 1 );

	}
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :匹配坐标，为自动连线确定坐标
 * @输入 :flag  看是三相三线 还是三相四线
 * @输出  :NONE
********************************************************************/
void app_WIRmod_componet_coord_match( uint8_t flag )
{
	if ( flag == JXMOD_3_3 )
	{
		m_Mcoord.Ua.pcomp = m_meter1;
		m_Mcoord.Ua.branch = 3;
		m_Mcoord.Ub.pcomp = m_PB;
		m_Mcoord.Ub.branch = 1;
		m_Mcoord.Uc.pcomp = m_meter2;
		m_Mcoord.Uc.branch = 3;
		m_HGQcoord.Ua.pcomp = m_UHGQ_3_3;
		m_HGQcoord.Ua.branch = 1;
		m_HGQcoord.Ub.pcomp = m_UHGQ_3_3;
		m_HGQcoord.Ub.branch = 2;
		m_HGQcoord.Uc.pcomp = m_UHGQ_3_3;
		m_HGQcoord.Uc.branch = 3;

		m_Mcoord.Ia_add.pcomp = m_meter1;
		m_Mcoord.Ia_add.branch = 2;
		m_Mcoord.Ia_sub.pcomp = m_meter1;
		m_Mcoord.Ia_sub.branch = 4;
		m_Mcoord.Ic_add.pcomp = m_meter2;
		m_Mcoord.Ic_add.branch = 2;
		m_Mcoord.Ic_sub.pcomp = m_meter2;
		m_Mcoord.Ic_sub.branch = 4;

		m_HGQcoord.Ia_add.pcomp = m_IHGQ1;
		m_HGQcoord.Ia_add.branch = 1;
		m_HGQcoord.Ia_sub.pcomp = m_IHGQ1;
		m_HGQcoord.Ia_sub.branch = 2;
		m_HGQcoord.Ic_add.pcomp = m_IHGQ2;
		m_HGQcoord.Ic_add.branch = 1;
		m_HGQcoord.Ic_sub.pcomp = m_IHGQ2;
		m_HGQcoord.Ic_sub.branch = 2;
	}
	else
	{
		m_Mcoord.Ua.pcomp = m_meter1;
		m_Mcoord.Ua.branch = 3;
		m_Mcoord.Ub.pcomp = m_meter2;
		m_Mcoord.Ub.branch = 3;
		m_Mcoord.Uc.pcomp = m_meter3;
		m_Mcoord.Uc.branch = 3;

		m_HGQcoord.Ua.pcomp = m_UHGQ_3_4;
		m_HGQcoord.Ua.branch = 1;
		m_HGQcoord.Ub.pcomp = m_UHGQ_3_4;
		m_HGQcoord.Ub.branch = 3;
		m_HGQcoord.Uc.pcomp = m_UHGQ_3_4;
		m_HGQcoord.Uc.branch = 5;

		m_Mcoord.Ia_add.pcomp = m_meter1;
		m_Mcoord.Ia_add.branch = 2;
		m_Mcoord.Ia_sub.pcomp = m_PA;
		m_Mcoord.Ia_sub.branch = 1;
		m_Mcoord.Ib_add.pcomp = m_meter2;
		m_Mcoord.Ib_add.branch = 2;
		m_Mcoord.Ib_sub.pcomp = m_PB;
		m_Mcoord.Ib_sub.branch = 1;
		m_Mcoord.Ic_add.pcomp = m_meter3;
		m_Mcoord.Ic_add.branch = 2;
		m_Mcoord.Ic_sub.pcomp = m_PC;
		m_Mcoord.Ic_sub.branch = 1;
		m_HGQcoord.Ia_add.pcomp = m_IHGQ1;
		m_HGQcoord.Ia_add.branch = 1;
		m_HGQcoord.Ia_sub.pcomp = m_IHGQ1;
		m_HGQcoord.Ia_sub.branch = 2;
		m_HGQcoord.Ib_add.pcomp = m_IHGQ2;
		m_HGQcoord.Ib_add.branch = 1;
		m_HGQcoord.Ib_sub.pcomp = m_IHGQ2;
		m_HGQcoord.Ib_sub.branch = 2;
		m_HGQcoord.Ic_add.pcomp = m_IHGQ3;
		m_HGQcoord.Ic_add.branch = 1;
		m_HGQcoord.Ic_sub.pcomp = m_IHGQ3;
		m_HGQcoord.Ic_sub.branch = 2;
	}
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能:对当前编辑成员进行显示，并清空原来内容
 *
 * @输入:

 * @输出:
********************************************************************/
void GUI_edit_string_redisplay( char*pstr )
{
	GUI_edit_clean();
	GUI_edit_string_display( pstr );
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 : 电压相之间连线
 * @输入 :相标识
 * @输出 :NONE
********************************************************************/
static void app_WIRmod_Uphase_link( char*Mphase, uint8_t phase )
{
	comp_branch_t *Mcomp;
	comp_branch_t *HGQcomp;

	/*
	if ( global_memcmp( Mphase, "Ua", 2 ) )
		Mcomp = &( m_Mcoord.Ua );
	else if ( global_memcmp( Mphase, "Ub", 2 ) )
		Mcomp = &( m_Mcoord.Ub );
	else if ( global_memcmp( Mphase, "Uc", 2 ) )
		Mcomp = &( m_Mcoord.Uc );
	if ( phase == 1 )HGQcomp = &( m_HGQcoord.Ua );
	if ( phase == 2 )HGQcomp = &( m_HGQcoord.Ub );
	if ( phase == 3 )HGQcomp = &( m_HGQcoord.Uc );
	*/
	if ( global_memcmp( Mphase, "Ua", 2 ) )
		Mcomp = &( m_HGQcoord.Ua );
	else if ( global_memcmp( Mphase, "Ub", 2 ) )
		Mcomp = &( m_HGQcoord.Ub );
	else if ( global_memcmp( Mphase, "Uc", 2 ) )
		Mcomp = &( m_HGQcoord.Uc );
	if ( phase == 1 )HGQcomp = &( m_Mcoord.Ua );
	if ( phase == 2 )HGQcomp = &( m_Mcoord.Ub );
	if ( phase == 3 )HGQcomp = &( m_Mcoord.Uc );

	ELEcomponent_link_EXT(
		Mcomp->pcomp, Mcomp->branch,
		HGQcomp->pcomp, HGQcomp->branch );
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :电流相之间连线
 * @输入 :相标识
 * @输出 :NONE
********************************************************************/
static void app_WIRmod_Iphase_link( char*Mphase, uint8_t phase )
{
	comp_branch_t *Mcomp_add;
	comp_branch_t *Mcomp_sub;
	comp_branch_t *HGQcomp_add;
	comp_branch_t *HGQcomp_sub;
	if ( global_memcmp( Mphase, "ia", 3 ) )
	{
		Mcomp_add = &( m_HGQcoord.Ia_add );
		Mcomp_sub = &( m_HGQcoord.Ia_sub );
	}
	else if ( global_memcmp( Mphase, "-ia", 3 ) )
	{
		Mcomp_add = &( m_HGQcoord.Ia_sub );
		Mcomp_sub = &( m_HGQcoord.Ia_add );
	}
	else if ( global_memcmp( Mphase, "ib", 3 ) )
	{
		Mcomp_add = &( m_HGQcoord.Ib_add );
		Mcomp_sub = &( m_HGQcoord.Ib_sub );
	}
	else if ( global_memcmp( Mphase, "-ib", 3 ) )
	{
		Mcomp_add = &( m_HGQcoord.Ib_sub );
		Mcomp_sub = &( m_HGQcoord.Ib_add );
	}
	else if ( global_memcmp( Mphase, "ic", 3 ) )
	{
		Mcomp_add = &( m_HGQcoord.Ic_add );
		Mcomp_sub = &( m_HGQcoord.Ic_sub );
	}
	else if ( global_memcmp( Mphase, "-ic", 3 ) )
	{
		Mcomp_add = &( m_HGQcoord.Ic_sub );
		Mcomp_sub = &( m_HGQcoord.Ic_add );
	}
	if ( phase == 1 )
	{
		HGQcomp_add = &( m_Mcoord.Ia_add );
		HGQcomp_sub = &( m_Mcoord.Ia_sub );
	}
	else if ( phase == 2 )
	{
		HGQcomp_add = &( m_Mcoord.Ib_add );
		HGQcomp_sub = &( m_Mcoord.Ib_sub );
	}
	else if ( phase == 3 )
	{
		HGQcomp_add = &( m_Mcoord.Ic_add );
		HGQcomp_sub = &( m_Mcoord.Ic_sub );
	}
	ELEcomponent_link(
		HGQcomp_add->pcomp, HGQcomp_add->branch, 
		Mcomp_add->pcomp, Mcomp_add->branch);
	GUI_set_Fgcolor( DRAW_WIN_FG_COLOR );
	ELEcomponent_link(		
		HGQcomp_sub->pcomp, HGQcomp_sub->branch, 
		Mcomp_sub->pcomp, Mcomp_sub->branch);
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :匹配坐标，为自动连线确定坐标
 * @输入 :flag  看是三相三线 还是三相四线
 * @输出  :NONE
********************************************************************/
void app_WIRmod_componet_auto_link( WIRresult_t*presult )
{
	GUI_window_hwin_req( m_WIRdraw_win );
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_set_Bgcolor( DRAW_WIN_BG_COLOR );
	GUI_set_Fgcolor( C_GUI_YELLOW );
	app_WIRmod_Uphase_link( presult->Result_MUa, 1 );
	app_WIRmod_Iphase_link( presult->Result_MIa, 1 );
	GUI_set_Fgcolor( C_GUI_GREEN );
	app_WIRmod_Uphase_link( presult->Result_MUb, 2 );
	if ( m_WIR_jxmod == JXMOD_3_4 )
		app_WIRmod_Iphase_link( presult->Result_MIb, 2 );
	GUI_set_Fgcolor( C_GUI_RED );
	app_WIRmod_Uphase_link( presult->Result_MUc, 3 );
	app_WIRmod_Iphase_link( presult->Result_MIc, 3 );
	GUI_window_hwin_release( m_WIRdraw_win );
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :WIR查询结果显示
 * @输入 :NONE
 * @输出  :NONE
********************************************************************/
static void app_WIRmod_Uphase_display( char*Mphase, char*HGQphase )
{
	char pch[15] = "";
	if(Mphase[0]!=ASCII_NULL)
	{
		global_strcat( pch, HGQphase );
		global_strcat( pch, " -> " );
		global_strcat( pch, Mphase );
	}
	GUI_edit_string_redisplay( pch );
	GUI_edit_shift( EDIT_NEXT );
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :WIR查询结果显示
 * @输入 :NONE
 * @输出  :NONE
********************************************************************/
static void app_WIRmod_Iphase_display( char*Mphase, char*HGQphase )
{
	char pch[15] = "";
	char pch_1[3] = "";
	if(Mphase[0]!=ASCII_NULL)
	{
		global_memcpy( pch_1, HGQphase, 2 );
		global_strcat( pch, pch_1 );
		global_strcat( pch, "->" );
		if ( Mphase[0] == '-' )
		{
			global_strcat( pch, &Mphase[1] );
			global_strcat( pch, "反" );
		}
		else
		{
			global_strcat( pch, Mphase );
		}
	}
	GUI_edit_string_redisplay( pch );
	GUI_edit_shift( EDIT_NEXT );
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :WIR查询结果显示
 * @输入 :NONE
 * @输出  :NONE
********************************************************************/
void app_WIRmod_result_lostUI_display( WIR_ALG_t*parg )
{
	Bsize_t offset;
	GUI_window_hwin_req( m_WIR_info_EDIT );
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
	GUI_set_Fgcolor( WIRmod_FG_COLOR );
	for ( offset = 0; offset < 8;offset++ )
	{
		GUI_edit_select( offset );
		GUI_edit_clean();
	}
	if ( parg->Ua_lost )
	{
		GUI_edit_select( 0 );
		GUI_edit_string_display(( char* )LostUaTitle[g_language] );
	}
	if ( parg->Ub_lost )
	{
		GUI_edit_select( 1 );
		GUI_edit_string_display(( char* )LostUbTitle[g_language] );
	}
	if ( parg->Uc_lost )
	{
		GUI_edit_select( 2 );
		GUI_edit_string_display(( char* )LostUcTitle[g_language] );
	}
	if ( parg->Ia_lost )
	{
		GUI_edit_select( 3 );
		GUI_edit_string_display(( char* )LostIaTitle[g_language] );
	}
	if ( parg->Ib_lost )
	{
		GUI_edit_select( 4 );
		GUI_edit_string_display(( char* )LostIbTitle[g_language] );
	}
	if ( parg->Ic_lost )
	{
		GUI_edit_select( 5 );
		GUI_edit_string_display(( char* )LostIcTitle[g_language] );
	}
	GUI_edit_select( 8 );
	GUI_set_Fgcolor( C_GUI_RED );
	GUI_edit_string_redisplay( "异常" );
	GUI_window_hwin_release( m_WIR_info_EDIT );
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :WIR查询结果显示
 * @输入 :NONE
 * @输出  :NONE
********************************************************************/
void app_WIRmod_result_display( WIRresult_t*presult )
{
	GUI_window_hwin_req( m_WIR_info_EDIT );
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
	GUI_set_Fgcolor( WIRmod_FG_COLOR );
	GUI_edit_select( 0 );
	if ( m_WIR_jxmod == JXMOD_3_3 )
	{
		app_WIRmod_Uphase_display( presult->Result_MUa, "Ua" );
		GUI_edit_clean();
		GUI_edit_shift( EDIT_NEXT );
		app_WIRmod_Uphase_display( presult->Result_MUc, "Uc" );
		app_WIRmod_Iphase_display( presult->Result_MIa, "ia" );
		GUI_edit_clean();
		GUI_edit_shift( EDIT_NEXT );
		app_WIRmod_Iphase_display( presult->Result_MIc, "ic" );
	}
	else
	{
		app_WIRmod_Uphase_display( presult->Result_MUa, "Ua" );
		app_WIRmod_Uphase_display( presult->Result_MUb, "Ub" );
		app_WIRmod_Uphase_display( presult->Result_MUc, "Uc" );
		app_WIRmod_Iphase_display( presult->Result_MIa, "ia" );
		app_WIRmod_Iphase_display( presult->Result_MIb, "ib" );
		app_WIRmod_Iphase_display( presult->Result_MIc, "ic" );

	}
	GUI_edit_clean();
	if ( presult->phaseDir == PHASE_ADD )
	{
		GUI_edit_string_display(( char* )zhengXU[g_language] );
	}
	else if ( presult->phaseDir == PHASE_SUB )
	{
		GUI_edit_string_display(( char* )niXU[g_language] );
	}
	GUI_edit_shift( EDIT_NEXT );
	if ( m_WIR_jxmod == JXMOD_3_3)
	{
		GUI_edit_string_display("三相三线");
	}
    else
    {
		GUI_edit_string_display("三相四线");
	}
	GUI_edit_shift( EDIT_NEXT );
	if ( presult->verdict )
	{
		GUI_set_Fgcolor( C_GUI_GREEN );
		GUI_edit_string_redisplay( "正常" );
	}
	else
	{
		GUI_set_Fgcolor( C_GUI_RED );
		GUI_edit_string_redisplay( "异常" );
	}
	GUI_window_hwin_release( m_WIR_info_EDIT );
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :WIR查询结果窗口创建
 * @输入 :NONE
 * @输出  :NONE
********************************************************************/
void app_WIRmod_resultedit_creat(void)
{
#define XLAB    25
#define YLHIGH  36
	GUI_window_hwin_req( m_WIR_win );
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_set_Bgcolor( WIRmod_WIN_BG_COLOR );
	GUI_set_Fgcolor( WIRmod_FG_COLOR );
	m_WIR_info_EDIT = GUI_edit_creat(471,0,m_WIR_win->win_wide-470,336,WIN_3D,m_WIR_win);
	GUI_window_hwin_req( m_WIR_info_EDIT );
	GUI_edit_memeber_add(XLAB,10+YLHIGH*0, 8,  "", MEMBER_3D | 0 );
	GUI_edit_memeber_add(XLAB,10+YLHIGH*1, 8,  "", MEMBER_3D | 1 );
	GUI_edit_memeber_add(XLAB,10+YLHIGH*2, 8,  "", MEMBER_3D | 2 );
	GUI_edit_memeber_add(XLAB,10+YLHIGH*3, 8,  "", MEMBER_3D | 3 );
	GUI_edit_memeber_add(XLAB,10+YLHIGH*4, 8,  "", MEMBER_3D | 4 );
	GUI_edit_memeber_add(XLAB,10+YLHIGH*5, 8,  "", MEMBER_3D | 5 );
	GUI_edit_memeber_add(XLAB,10+YLHIGH*6, 8,  "", MEMBER_3D | 6 );
	GUI_edit_memeber_add(XLAB,10+YLHIGH*7, 8,  "", MEMBER_3D | 7 );
	GUI_edit_memeber_add(XLAB,10+YLHIGH*8, 4,  "接线", MEMBER_3D | 8 );
	GUI_WINDIS_HLINE(2,m_WIR_info_EDIT->win_wide-2,10+YLHIGH*8-3);
	GUI_window_hwin_release( m_WIR_info_EDIT );
	GUI_window_hwin_release( m_WIR_win );
	return;
}

/********************************************************************
 * @创建人 :揭成
 * @功能 :WIR接线绘制图窗口创建
 * @输入 :NONE
 * @输出  :NONE
********************************************************************/
void app_WIRmod_draw_creat( uint8_t flag )
{
	GUI_window_hwin_req( m_WIRdraw_win );
	GUI_set_Bgcolor( DRAW_WIN_BG_COLOR );
	GUI_set_Fgcolor( DRAW_WIN_FG_COLOR );
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_clean_window();
	app_WIRmod_comp_create( flag );
	GUI_set_Fgcolor( C_GUI_YELLOW );
	GUI_window_angle_line_display( 15, L1Y, 0, 450, 1, "" );
	GUI_set_Fgcolor( C_GUI_GREEN );
	GUI_window_angle_line_display( 15, L1Y + 30*1, 0, 450, 1, "" );
	GUI_set_Fgcolor( C_GUI_RED );
	GUI_window_angle_line_display( 15, L1Y + 30*2, 0, 450, 1, "" );
	GUI_set_Fgcolor( DRAW_WIN_FG_COLOR );
	GUI_window_angle_line_display( 15, L1Y + 30*3, 0, 450, 1, "" );
	GUI_string_display_At( 15, L1Y - 17, "A" );
	GUI_string_display_At( 15, L1Y + 30*1 - 17, "B" );
	GUI_string_display_At( 15, L1Y + 30*2 - 17, "C" );
	GUI_string_display_At( 15, L1Y + 30*3 - 17, "N" );
	app_WIRmod_link_init( flag );
	GUI_window_hwin_release( m_WIRdraw_win );
	app_WIRmod_componet_coord_match( flag );
	return;
}


void app_WIRmod_resultRadio_init(hwin_t*hp)
{
#define HIGH_GAP  20
#define HIGH_LBR  5 
#define _S_YOBTAIN(offset) (HIGH_LBR+HIGH_GAP*(offset+1))

   	GUI_window_hwin_req( hp );
	GUI_set_Bgcolor(WIRmod_WIN_BG_COLOR);
	GUI_set_Fgcolor(WIRmod_FG_COLOR);	
	GUI_set_Fgcolor(C_GUI_RED);	
	GUI_string_display_At(44,5,"cosΦ");
	GUI_string_display_At(92+30*0,5,"Ua");
	GUI_string_display_At(92+30*1,5,"Ub");
	GUI_string_display_At(92+30*2,5,"Uc");
	GUI_string_display_At(92+30*3,5,"Ia");
	GUI_string_display_At(92+30*4,5,"Ib");
	GUI_string_display_At(92+30*5,5,"Ic");
	GUI_set_Fgcolor(C_GUI_WHITE);	
	GUI_WINDIS_HLINE(2,350,HIGH_LBR+HIGH_GAP-2);
	GUI_WINDIS_VLINE(41,2,hp->win_high);
	GUI_WINDIS_VLINE(350,2,hp->win_high);
    GUI_radio_memeber_add(15,_S_YOBTAIN(0),"",0);
    GUI_radio_memeber_add(15,_S_YOBTAIN(1),"",1);
    GUI_radio_memeber_add(15,_S_YOBTAIN(2),"",2);
    GUI_radio_memeber_add(15,_S_YOBTAIN(3),"",3);
	GUI_radio_select(hp,0);
   	GUI_window_hwin_release( hp );
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :WIR主窗口
 * @输入 :NONE
 * @输出  :NONE
********************************************************************/
void app_WIRmod_window_creat(int flag)
{
	GUI_window_hwin_req( GET_GUIWIN);
	GUI_set_Bgcolor( WIRmod_BG_COLOR );
	GUI_clean_screen();
	GUI_set_Bgcolor( WIRmod_WIN_BG_COLOR );
	GUI_set_Fgcolor( WIRmod_FG_COLOR );
	m_WIR_win = Gui_3D_window_creat(0,0,LCD_WIDE-2,LCD_HIGH, "用电检查", WIN_3D|WIN_FILL,0);
	GUI_window_hwin_req( m_WIR_win );
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_set_Bgcolor( DRAW_WIN_BG_COLOR );
	GUI_set_Fgcolor( C_GUI_WHITE);
	m_WIRdraw_win = GUI_window_child_creat(2,0,470,336,WIN_3D|WIN_FILL, m_WIR_win );
	GUI_set_Fgcolor( WIRmod_FG_COLOR );
	app_WIRmod_draw_creat(0);
    m_WIRresjult_win= GUI_radio_create(2,336,m_WIR_win->win_wide,m_WIR_win->win_high-336,WIN_3D|KEY_DOWN,m_WIR_win );
    app_WIRmod_resultRadio_init(m_WIRresjult_win);
	app_WIRmod_resultedit_creat();
	GUI_window_hwin_req( m_WIRresjult_win );
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_set_Bgcolor( WIRmod_WIN_BG_COLOR );
	GUI_set_Fgcolor( C_GUI_GREEN);
	GUI_string_display_At(360,10+25*0, "键盘操作提示 - ->" );
	GUI_set_Fgcolor( C_GUI_YELLOW);
	if ( flag == 1 )
	{
		GUI_string_display_At(360,10+25*1, "按<F4>进行电量追补." );
		GUI_string_display_At(360,10+25*2, "按<退出>返回.");
	}
	else
	{
		GUI_string_display_At(360,10+25*1, "按<F4>进行电量追补." );
		GUI_string_display_At(360,10+25*2, "按<UP>或<DOWN>选择功率因数方案." );
		GUI_string_display_At(360,10+25*3, "按<存储>保存,按<查询>查看存储数据.");
	}
	GUI_window_hwin_release( m_WIRresjult_win );
	GUI_window_hwin_release( m_WIR_win );
	GUI_window_hwin_release( GET_GUIWIN );
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :销毁WIR主窗口
 * @输入 :NONE
 * @输出  :NONE
********************************************************************/
void app_WIRmod_window_destroy( void )
{
	GUI_window_destory( m_WIR_win );
	return;
}


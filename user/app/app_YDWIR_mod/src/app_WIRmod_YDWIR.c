/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:查线管理模块主体程序
* 创建时间:2010-12-22
* 所属公司 :科荟
* 文件名称:app_WIRmod_fun.c
* 创建人 :揭成
* 功能描述:实现查线主功能
******************************************************************************************/
#include		    "app_WIRmod_inside.h"
#include		    "app_WIRmod_componentlib.h"
uint8_t 		 	m_WIR_main_task_pro = INVALID_PRO;
WIRresult_t  		m_WIRresult;
uint8_t             m_WIR_cosoffset = 10;
uint8_t             m_WIR_is_draw = FAIL;
uint8_t             m_WIR_jxmod = 3;

void app_WIRmod_WIRresult_obtain(WIR_ALG_t*pResult,WIRresult_t* presult)
{
   char *psrc;
   Bsize_t offset = 0;
   global_string_filter(pResult->c_I, ' ' );
   global_string_filter(pResult->c_U, ' ' );
   GLOBAL_MEMCLR(presult, sizeof(WIRresult_t));
   global_memcpy( presult->Result_MUa, &( pResult->c_U[0] ), 2 );
   global_memcpy( presult->Result_MUb, &( pResult->c_U[2] ), 2 );
   global_memcpy( presult->Result_MUc, &( pResult->c_U[4] ), 2 );
    psrc = pResult->c_I;
	if ( psrc[0] == '-' )
	{
		GLOBAL_MEMCLR( presult->Result_MIa, 4 );
		global_memcpy( presult->Result_MIa, &psrc[offset], 3 );
		offset += 3;
	}
	else
	{
		GLOBAL_MEMCLR( presult->Result_MIa, 4 );
		global_memcpy( presult->Result_MIa, &psrc[offset], 2 );
		offset += 2;
	}

	if ( pResult->jxmod == JXMOD_3_3 )
	{
		if ( psrc[offset] == '-' )
		{
			GLOBAL_MEMCLR( presult->Result_MIc, 4 );
			global_memcpy( presult->Result_MIc, &psrc[offset], 3 );
			offset += 3;
		}
		else
		{
			GLOBAL_MEMCLR( presult->Result_MIc, 4 );
			global_memcpy( presult->Result_MIc, &psrc[offset], 2 );
			offset += 2;
		}
	}
	else if ( pResult->jxmod == JXMOD_3_4 )
	{
		if ( psrc[offset] == '-' )
		{
			GLOBAL_MEMCLR( presult->Result_MIb, 4 );
			global_memcpy( presult->Result_MIb, &psrc[offset], 3 );
			offset += 3;
		}
		else
		{
			GLOBAL_MEMCLR( presult->Result_MIb, 4 );
			global_memcpy( presult->Result_MIb, &psrc[offset], 2 );
			offset += 2;
		}
		if ( psrc[offset] == '-' )
		{
			GLOBAL_MEMCLR( presult->Result_MIc, 4 );
			global_memcpy( presult->Result_MIc, &psrc[offset], 3 );
			offset += 3;
		}
		else
		{
			GLOBAL_MEMCLR( presult->Result_MIc, 4 );
			global_memcpy( presult->Result_MIc, &psrc[offset], 2 );
			offset += 2;
		}
	}
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :查线结果显示
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_WIRmod_WIRresult_display(void*pResult)
{
#define S_HIGH_GAP  20
#define S_HIGH_LBR  5 
#define S_YOBTAIN(offset) (S_HIGH_LBR+S_HIGH_GAP*(offset+1))
	WIRresult_t  UIWIR_Result;
	WIRresult_t* presult = &UIWIR_Result;
	WIR_ALG_t*pdata=(WIR_ALG_t*)pResult;
	Bsize_t offset;
	GUI_window_hwin_req(m_WIRresjult_win);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_set_Fgcolor(C_GUI_YELLOW);	
	GUI_set_Bgcolor(WIRmod_WIN_BG_COLOR);
    GUI_fill_window_area(43,S_YOBTAIN(0)-1,112+30*5,S_YOBTAIN(4)+1);
	for(offset =0;offset<4;offset++)
	{
	    if(pdata[offset].c_cos[0]==0||pdata[offset].c_cos[0]==' ')
		{
			GUI_string_display_At(44,S_YOBTAIN(offset),"保留");
			continue;
		}
		GUI_string_display_At(44,S_YOBTAIN(offset),pdata[offset].c_cos);
		app_WIRmod_WIRresult_obtain(&pdata[offset],presult);
		GUI_string_display_At(92+30*0,  S_YOBTAIN(offset),presult->Result_MUa);
		GUI_string_display_At(92+30*1,  S_YOBTAIN(offset),presult->Result_MUb);
		GUI_string_display_At(92+30*2,  S_YOBTAIN(offset),presult->Result_MUc);
		if(presult->Result_MIa[0]=='-')
			GUI_string_display_At(92+30*3-4,S_YOBTAIN(offset),presult->Result_MIa);
		else
			GUI_string_display_At(92+30*3,S_YOBTAIN(offset),presult->Result_MIa);
		if(presult->Result_MIb[0]=='-')
			GUI_string_display_At(92+30*4-4,S_YOBTAIN(offset),presult->Result_MIb);
		else
			GUI_string_display_At(92+30*4,S_YOBTAIN(offset),presult->Result_MIb);
		if(presult->Result_MIc[0]=='-')
			GUI_string_display_At(92+30*5-4,S_YOBTAIN(offset),presult->Result_MIc);
		else
			GUI_string_display_At(92+30*5,S_YOBTAIN(offset),presult->Result_MIc);		
	}
   	GUI_window_hwin_release(m_WIRresjult_win);
    return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :接线是否异常判断
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
BOOL app_WIRmod_UIwir_sys(char*pU,char*pI,uint8_t JXmod)
{
	if(JXmod==JXMOD_3_4)
	{
		if(pU[1]==pI[1]&&pU[3]==pI[3]&&pU[5]==pI[5])
		return TURE;	   
	}
	else
	{
		if(pU[1]==pI[1]&&pU[5]==pI[3])
		return TURE;	 
	}
	return FAIL;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :选择查线结果组
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
Bsize_t app_WIRmod_seletc_result( void )
{
	Bsize_t ID;
    ID = GUI_radio_curmember_obtain(m_WIRresjult_win);
	if(m_alg_presult[ID].c_cos[0]==0||m_alg_presult[ID].c_cos[0]==' ') return 5;
	return ID;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :电流查线结果分析
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void app_WIRmod_resulst_sys_I( WIRresult_t*presult, char*psrc )
{
	Bsize_t offset = 0;

	if ( psrc[0] == '-' )
	{
		GLOBAL_MEMCLR( presult->Result_MIa, 4 );
		global_memcpy( presult->Result_MIa, &psrc[offset], 3 );
		offset += 3;
	}
	else
	{
		GLOBAL_MEMCLR( presult->Result_MIa, 4 );
		global_memcpy( presult->Result_MIa, &psrc[offset], 2 );
		offset += 2;
	}

	if ( m_WIR_jxmod == JXMOD_3_3 )
	{
		if ( psrc[offset] == '-' )
		{
			GLOBAL_MEMCLR( presult->Result_MIc, 4 );
			global_memcpy( presult->Result_MIc, &psrc[offset], 3 );
			offset += 3;
		}
		else
		{
			GLOBAL_MEMCLR( presult->Result_MIc, 4 );
			global_memcpy( presult->Result_MIc, &psrc[offset], 2 );
			offset += 2;
		}
	}
	else if ( m_WIR_jxmod == JXMOD_3_4 )
	{
		if ( psrc[offset] == '-' )
		{
			GLOBAL_MEMCLR( presult->Result_MIb, 4 );
			global_memcpy( presult->Result_MIb, &psrc[offset], 3 );
			offset += 3;
		}
		else
		{
			GLOBAL_MEMCLR( presult->Result_MIb, 4 );
			global_memcpy( presult->Result_MIb, &psrc[offset], 2 );
			offset += 2;
		}
		if ( psrc[offset] == '-' )
		{
			GLOBAL_MEMCLR( presult->Result_MIc, 4 );
			global_memcpy( presult->Result_MIc, &psrc[offset], 3 );
			offset += 3;
		}
		else
		{
			GLOBAL_MEMCLR( presult->Result_MIc, 4 );
			global_memcpy( presult->Result_MIc, &psrc[offset], 2 );
			offset += 2;
		}
	}
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :查线结果分析
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
BOOL app_WIRmod_resulst_sys( WIRresult_t*presult )
{
	Bsize_t offset;
	if ( m_alg_presult->is_lost_UI )
	{
		app_WIRmod_result_lostUI_display( m_alg_presult );
		GUI_window_hwin_req( m_WIRdraw_win );
		GUI_set_Bgcolor( DRAW_WIN_BG_COLOR );
		GUI_set_Fgcolor( DRAW_WIN_FG_COLOR );
		GUI_clean_window();
		GUI_window_hwin_release( m_WIRdraw_win );
		return FAIL;
	}
	else
	{
		GLOBAL_MEMCLR( presult, sizeof( WIRresult_t ) );
		m_WIR_jxmod = m_alg_presult->jxmod;
		offset = app_WIRmod_seletc_result();
		if ( offset == 5 ) return FAIL;
		global_strcpy(presult->cos,m_alg_presult[offset].c_cos);
		global_string_filter( m_alg_presult[offset].c_I, ' ' );
		global_string_filter( m_alg_presult[offset].c_U, ' ' );
		global_memcpy( presult->Result_MUa, &( m_alg_presult[offset].c_U[0] ), 2 );
		global_memcpy( presult->Result_MUb, &( m_alg_presult[offset].c_U[2] ), 2 );
		global_memcpy( presult->Result_MUc, &( m_alg_presult[offset].c_U[4] ), 2 );
		app_WIRmod_resulst_sys_I( presult, m_alg_presult[offset].c_I );
		presult->phaseDir = m_alg_presult->phaseDir;
		if ( !global_memcmp( m_alg_presult[offset].c_U, "UaUbUc", sizeof( "UaUbUc" ) ) )
		{
			presult->verdict = FAIL;
			return TURE;
		}
		if ( m_WIR_jxmod == JXMOD_3_3 )
		{
			if ( !global_memcmp( m_alg_presult[offset].c_I, "iaic", sizeof( "iaic" ) ) )
			{
				presult->verdict = FAIL;
				return TURE;
			}
		}
		else
		{
			if ( !global_memcmp( m_alg_presult[offset].c_I, "iaibic", sizeof( "iaibic" ) ) )
			{
				presult->verdict = FAIL;
				return TURE;
			}
		}
		if ( m_alg_presult->phaseDir != PHASE_ADD )
		{
			presult->verdict = FAIL;
			return TURE;
		}
		presult->verdict = TURE;
		return TURE;
	}
}
/*            -------------------------------                               */
/*4月26日追加，找出修改最少的一种查线方式*/
/*            -------------------------------                               */
/********************************************************************
 * @创建人 :揭成
 * @功能 :将结果转换成单一字符表示，
 *                 flag =1    picon->pwir
                    flag =0    pwir->picon
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
char m_WIRmod_I_polarity[3];
void app_WIRmod_icon_mod(char*icon,char*pload)
{
        if(global_memcmp(icon,"Ua",2)) pload[0]='a';
        else if(global_memcmp(icon,"Ub",2)) pload[0]='b';
        else if(global_memcmp(icon,"Uc",2)) pload[0]='c';
        else if(global_memcmp(icon,"ia",2)) 
        {
			pload[0]='a';
        }
        else if(global_memcmp(icon,"-ia",3))
        {
			pload[0]='a';
        }
        else if(global_memcmp(icon,"ib",2))
        {
			pload[0]='b';
        }
        else if(global_memcmp(icon,"-ib",3))
        {
			pload[0]='b';
        }
	  else if(global_memcmp(icon,"ic",2)) 
        {
			pload[0]='c';
        }
        else if(global_memcmp(icon,"-ic",3))
        {
			pload[0]='c';
        }
        return;
}
void app_WIRmod_pwir_mod(char icon,char*pload,char mod)
{
     if(mod == 'U')
     {
         if(icon=='a') global_strcpy(pload,"Ua");
         else if(icon=='b') global_strcpy(pload,"Ub");
         else if(icon=='c') global_strcpy(pload,"Uc");
      }
     else if(mod == 'I')
     	{
         if(icon=='a')global_strcpy(pload,"ia");
         else if(icon=='b')global_strcpy(pload,"ib");
         else if(icon=='c')global_strcpy(pload,"ic");
	 }
        return;
}
void app_WIRmod_icon_load(char*picon,WIRresult_t*pwir,uint8_t flag)
{
       char buf[4]="";
       if(flag ==0)
       {  
            m_WIRmod_I_polarity[0]='+';
	      m_WIRmod_I_polarity[1]='+';
            m_WIRmod_I_polarity[2]='+';
            if(pwir->Result_MIa[0]=='-')
		m_WIRmod_I_polarity[0]='-';
            if(pwir->Result_MIb[0]=='-')
		m_WIRmod_I_polarity[1]='-';
            if(pwir->Result_MIc[0]=='-')
		m_WIRmod_I_polarity[2]='-';
            app_WIRmod_icon_mod(pwir->Result_MUa,&picon[0]);
            app_WIRmod_icon_mod(pwir->Result_MUb,&picon[1]);
            app_WIRmod_icon_mod(pwir->Result_MUc,&picon[2]);
            app_WIRmod_icon_mod(pwir->Result_MIa,&picon[3]);
            app_WIRmod_icon_mod(pwir->Result_MIb,&picon[4]);
            app_WIRmod_icon_mod(pwir->Result_MIc,&picon[5]);
			
	}
      else if(flag==1)
      	{
		app_WIRmod_pwir_mod(picon[0],pwir->Result_MUa,'U');
		app_WIRmod_pwir_mod(picon[1],pwir->Result_MUb,'U');
		app_WIRmod_pwir_mod(picon[2],pwir->Result_MUc,'U');
		app_WIRmod_pwir_mod(picon[3],pwir->Result_MIa,'I');
		app_WIRmod_pwir_mod(picon[4],pwir->Result_MIb,'I');
		app_WIRmod_pwir_mod(picon[5],pwir->Result_MIc,'I');
		if(m_WIRmod_I_polarity[0]=='-')
		{ 
		     GLOBAL_MEMCLR(buf, 4);
		     buf[0]='-';
		     global_strcat(buf,pwir->Result_MIa);	 
		     global_memcpy(pwir->Result_MIa,buf,3);
		}
		if(m_WIRmod_I_polarity[1]=='-')
		{
		     GLOBAL_MEMCLR(buf, 4);
		     buf[0]='-';
		     global_strcat(buf,pwir->Result_MIb);	 
		     global_memcpy(pwir->Result_MIb,buf,3);
		}
		if(m_WIRmod_I_polarity[2]=='-')
		{
		     GLOBAL_MEMCLR(buf, 4);
		     buf[0]='-';
		     global_strcat(buf,pwir->Result_MIc);	 
		     global_memcpy(pwir->Result_MIc,buf,3);
		}
	}
     return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :找出最简单接线错误算法
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void app_WIRmod_rank_mod(char*picon1,char*picon2)
{
     if(picon1[0]=='a')
     {
		picon2[0]='a';
     }
     else if(picon1[0]=='b')
     {
		picon2[1]='a';
     }
     else if(picon1[0]=='c')
     {
		picon2[2]='a';
     }
     if(picon1[1]=='a')
     {
		picon2[0]='b';
     }
     else if(picon1[1]=='b')
     {
		picon2[1]='b';
     }
     else if(picon1[1]=='c')
     {
		picon2[2]='b';
     }
	 
     if(picon1[2]=='a')
     {
		picon2[0]='c';
     }
     else if(picon1[2]=='b')
     {
		picon2[1]='c';
     }
     else if(picon1[2]=='c')
     {
		picon2[2]='c';
     }
     return;
}
void app_WIRmod_icon_shift(char*psrc,char*pdes)
{
      pdes[0]=psrc[1];
      pdes[1]=psrc[2];
      pdes[2]=psrc[0];
      pdes[3]=psrc[4];
      pdes[4]=psrc[5];
      pdes[5]=psrc[3];
	return;
}
typedef struct
{
   char*pmove;
   Bsize_t step;
} WIR_move_step_t;



const WIR_move_step_t m_WIR_move_step[6]=
{
   	{"abc",0},
   	{"acb",1},
   	{"bca",2},
   	{"bac",1},
   	{"cba",1},
   	{"cab",2},
};

Bsize_t app_WIRmod_move_step_obtain(char*src)
{
   Bsize_t offset;
   for(offset=0;offset<SIZE_OF_ARRAY(m_WIR_move_step);offset++)
   {
         if(global_memcmp(m_WIR_move_step[offset].pmove,src,3))
		return m_WIR_move_step[offset].step;
   }
   return 0;
}
void app_WIRmod_simpleness_alg(WIRresult_t*pwir)
{
	char icon_1[6]="";
	char icon_2[6]="";
	char icon_3[6]="";
	char icon_temp[6];
	char cos[10]="";
	Bsize_t step1;
	Bsize_t step2;
	Bsize_t step3;
	Bsize_t step;
	uint8_t temp1;
	uint8_t temp2;
	if(m_alg_presult->jxmod  == JXMOD_3_3)
	 	return;
	temp1 = pwir->phaseDir;
	temp2 = pwir->verdict;
      app_WIRmod_icon_load(icon_1,pwir,0);
	app_WIRmod_rank_mod(&icon_1[3],&icon_temp[3]);
	global_memcpy(&icon_1[3],&icon_temp[3],3);
	app_WIRmod_icon_shift(icon_1,icon_2);  
	app_WIRmod_icon_shift(icon_2,icon_3);  
	app_WIRmod_rank_mod(&icon_1[3],&icon_temp[3]);
	global_memcpy(&icon_1[3],&icon_temp[3],3);
	app_WIRmod_rank_mod(&icon_2[3],&icon_temp[3]);
	global_memcpy(&icon_2[3],&icon_temp[3],3);
	app_WIRmod_rank_mod(&icon_3[3],&icon_temp[3]);
	global_memcpy(&icon_3[3],&icon_temp[3],3);
	
	step1=app_WIRmod_move_step_obtain(icon_1)
		+app_WIRmod_move_step_obtain(&icon_1[3]);
	step2=app_WIRmod_move_step_obtain(icon_2)
		+app_WIRmod_move_step_obtain(&icon_2[3]);
	step3=app_WIRmod_move_step_obtain(icon_3)
		+app_WIRmod_move_step_obtain(&icon_3[3]);
	
      step=MIN(step1,step2);
      step=MIN(step,step3);   
	  global_strcpy(cos,  pwir->cos);
      GLOBAL_MEMCLR(pwir, sizeof(WIRresult_t));
      if(step==step1)
      	{
	      app_WIRmod_icon_load(icon_1,pwir,1);
	}
      else if(step==step2)
      	{
	      app_WIRmod_icon_load(icon_2,pwir,1);
	}
      else if(step==step3)
      	{
	      app_WIRmod_icon_load(icon_3,pwir,1);
	}		
	pwir->phaseDir=temp1;
	pwir->verdict=temp2;
	global_strcpy(pwir->cos,cos);
    return; 
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :WIR模块主任务
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
static void _task( void*arg )
{
	uint8_t err;
	while ( 1 )
	{
		msleep( TYPE_DELAY );
		if ( os_sem_pend( m_WIRmod_sem,APP_100_MS) == OS_NO_ERR )/*等待信号*/
		{
		    OSSemSet(m_WIRmod_sem,0,&err);
			app_WIRmod_WIRresult_display(m_alg_presult);
			if ( app_WIRmod_resulst_sys( &m_WIRresult ) )
			{
			    m_WIR_is_draw = TURE;
				app_WIRmod_simpleness_alg(&m_WIRresult);
				app_WIRmod_draw_creat( m_alg_presult->jxmod );
				app_WIRmod_result_display( &m_WIRresult );
				app_WIRmod_componet_auto_link( &m_WIRresult );
			}
		}
	}
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :主模块主功能初始化
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void app_WIRmod_YDWIR_entry( void )
{
	m_WIR_cosoffset = 10;  /*设置成复位值*/
	m_WIR_is_draw = FAIL;
	m_WIR_jxmod = 3;
	GLOBAL_MEMCLR( &m_WIRresult, sizeof( WIRresult_t ) );
	ELEcomponent_maneger_init();
	m_WIR_main_task_pro
	= os_task_assign_stk_build( _task, 4 );
	os_task_name_set( m_WIR_main_task_pro, "WIRmod--Main task" );
	app_WIRmod_alg_entery();
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :主模块主功能释放资源
 *
 * @输入 :NONE
 *
 * @输出 :NONE
********************************************************************/
void app_WIRmod_YDWIR_exit( void )
{
	if ( m_WIR_main_task_pro != INVALID_PRO )
	{
		os_task_delete( m_WIR_main_task_pro );
		m_WIR_main_task_pro = INVALID_PRO;
	}
	app_WIRmod_alg_exit();
	return;
}

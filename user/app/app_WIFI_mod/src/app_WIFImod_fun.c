#include		    "app_WIFImod_inside.h"
uint8_t 		    m_WIRtask_id       =  INVALID_PRO;
WIRresult_t  		m_WIRresult[4]     = {0,};
uint8_t             m_Jx3_4            =  3;
/********************************************************************
 * @创建人 :揭成
 * @功能   :电流查线结果分析
 * @输入   :NONE
 * @输出   :NONE
********************************************************************/
void m_resulst_sys_I( WIRresult_t*presult, char*psrc )
{
	Bsize_t offset = 0;
	if ( psrc[0] == '-' )
	{
		GLOBAL_MEMCLR( presult->Result_MIa, 4 );
		global_memcpy( presult->Result_MIa, &psrc[offset], 3);
		offset += 3;
	}
	else
	{
		GLOBAL_MEMCLR( presult->Result_MIa, 4 );
		global_memcpy( presult->Result_MIa, &psrc[offset], 2);
		offset += 2;
	}
	if ( m_Jx3_4 == JXMOD_3_3 )
	{
		if ( psrc[offset] == '-' )
		{
			GLOBAL_MEMCLR( presult->Result_MIc, 4 );
			global_memcpy( presult->Result_MIc, &psrc[offset], 3);
			offset += 3;
		}
		else
		{
			GLOBAL_MEMCLR( presult->Result_MIc, 4 );
			global_memcpy( presult->Result_MIc, &psrc[offset], 2 );
			offset += 2;
		}
	}
	else if ( m_Jx3_4 == JXMOD_3_4 )
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
 * @功能   :查线结果分析
 * @输入   :NONE
 * @输出   :NONE
********************************************************************/
void _resulst_sys( WIRresult_t*presult,Bsize_t offset )
{
	GLOBAL_MEMCLR(presult, sizeof( WIRresult_t ) );
    if(m_alg_presult[offset].c_cos[0]==0||m_alg_presult[offset].c_cos[0]==' ')
    return ;
	if ( offset == 5 ) return ;
	global_strcpy(presult->cos,m_alg_presult[offset].c_cos);
	global_string_filter( m_alg_presult[offset].c_I, ' ' );
	global_string_filter( m_alg_presult[offset].c_U, ' ' );
	global_memcpy(presult->Result_MUa,&(m_alg_presult[offset].c_U[0] ),2);
	global_memcpy(presult->Result_MUb,&(m_alg_presult[offset].c_U[2] ),2);
	global_memcpy(presult->Result_MUc,&(m_alg_presult[offset].c_U[4] ),2);
	m_resulst_sys_I( presult, m_alg_presult[offset].c_I );
	presult->phaseDir = m_alg_presult->phaseDir;
	if (!global_memcmp(m_alg_presult[offset].c_U,"UaUbUc",sizeof("UaUbUc")))
	{
		presult->verdict = FAIL;
		return;
	}
	if ( m_Jx3_4 == JXMOD_3_3 )
	{
		if (!global_memcmp(m_alg_presult[offset].c_I,"iaic",sizeof("iaic")))
		{
			presult->verdict = FAIL;
			return;
		}
	}
	else
	{
		if (!global_memcmp(m_alg_presult[offset].c_I,"iaibic",sizeof("iaibic")))
		{
			presult->verdict = FAIL;
			return;
		}
	}
	if ( m_alg_presult->phaseDir != PHASE_ADD )
	{
		presult->verdict = FAIL;
		return;
	}
	presult->verdict = TURE;
	return;
}
/*----------------------------------------------------------*/
/*4月26日追加，找出修改最少的一种查线方式*/
/*----------------------------------------------------------*/
/********************************************************************
 * @创建人 :揭成
 * @功能 :将结果转换成单一字符表示，
 *        flag =1    picon->pwir   
 *		  flag =0    pwir->picon
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
char m_WIFImod_I_polarity[3];
void app_WIFImod_icon_mod(char*icon,char*pload)
{
	if(global_memcmp(icon,"Ua",2)) 		pload[0]='a';
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

void app_WIFImod_pwir_mod(char icon,char*pload,char mod)
{
	if(mod == 'U')
	{
		if(icon=='a') 	   global_strcpy(pload,"Ua");
		else if(icon=='b') global_strcpy(pload,"Ub");
		else if(icon=='c') global_strcpy(pload,"Uc");
	}
	else if(mod == 'I')
	{
		if(icon=='a')	   global_strcpy(pload,"ia");
		else if(icon=='b') global_strcpy(pload,"ib");
		else if(icon=='c') global_strcpy(pload,"ic");
	}
	return;
}

void app_WIFImod_icon_load(char*picon,WIRresult_t*pwir,uint8_t flag)
{
	char buf[4]="";
	if(flag ==0)
	{  
		m_WIFImod_I_polarity[0]='+';
		m_WIFImod_I_polarity[1]='+';
		m_WIFImod_I_polarity[2]='+';
		if(pwir->Result_MIa[0]=='-')
		m_WIFImod_I_polarity[0]='-';
		if(pwir->Result_MIb[0]=='-')
		m_WIFImod_I_polarity[1]='-';
		if(pwir->Result_MIc[0]=='-')
		m_WIFImod_I_polarity[2]='-';
		app_WIFImod_icon_mod(pwir->Result_MUa,&picon[0]);
		app_WIFImod_icon_mod(pwir->Result_MUb,&picon[1]);
		app_WIFImod_icon_mod(pwir->Result_MUc,&picon[2]);
		app_WIFImod_icon_mod(pwir->Result_MIa,&picon[3]);
		app_WIFImod_icon_mod(pwir->Result_MIb,&picon[4]);
		app_WIFImod_icon_mod(pwir->Result_MIc,&picon[5]);
		
	}
    else if(flag==1)
	{
		app_WIFImod_pwir_mod(picon[0],pwir->Result_MUa,'U');
		app_WIFImod_pwir_mod(picon[1],pwir->Result_MUb,'U');
		app_WIFImod_pwir_mod(picon[2],pwir->Result_MUc,'U');
		app_WIFImod_pwir_mod(picon[3],pwir->Result_MIa,'I');
		app_WIFImod_pwir_mod(picon[4],pwir->Result_MIb,'I');
		app_WIFImod_pwir_mod(picon[5],pwir->Result_MIc,'I');
		if(m_WIFImod_I_polarity[0]=='-')
		{ 
			GLOBAL_MEMCLR(buf, 4);
			buf[0]='-';
			global_strcat(buf,pwir->Result_MIa);	 
			global_memcpy(pwir->Result_MIa,buf,3);
		}
		if(m_WIFImod_I_polarity[1]=='-')
		{
			GLOBAL_MEMCLR(buf, 4);
			buf[0]='-';
			global_strcat(buf,pwir->Result_MIb);	 
			global_memcpy(pwir->Result_MIb,buf,3);
		}
		if(m_WIFImod_I_polarity[2]=='-')
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
void app_WIFImod_rank_mod(char*picon1,char*picon2)
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
void app_WIFImod_icon_shift(char*psrc,char*pdes)
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

Bsize_t app_WIFImod_move_step_obtain(char*src)
{
   Bsize_t offset;
   for(offset=0;offset<SIZE_OF_ARRAY(m_WIR_move_step);offset++)
   {
         if(global_memcmp(m_WIR_move_step[offset].pmove,src,3))
		return m_WIR_move_step[offset].step;
   }
   return 0;
}
void app_WIFImod_simpleness_alg(WIRresult_t*pwir)
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
    if(pwir->cos[0]==0||pwir->cos[0]==' ')
    	return ;
	if(m_alg_presult->jxmod  == JXMOD_3_3)
	 	return;
	temp1 = pwir->phaseDir;
	temp2 = pwir->verdict;
      app_WIFImod_icon_load(icon_1,pwir,0);
	app_WIFImod_rank_mod(&icon_1[3],&icon_temp[3]);
	global_memcpy(&icon_1[3],&icon_temp[3],3);
	app_WIFImod_icon_shift(icon_1,icon_2);  
	app_WIFImod_icon_shift(icon_2,icon_3);  
	app_WIFImod_rank_mod(&icon_1[3],&icon_temp[3]);
	global_memcpy(&icon_1[3],&icon_temp[3],3);
	app_WIFImod_rank_mod(&icon_2[3],&icon_temp[3]);
	global_memcpy(&icon_2[3],&icon_temp[3],3);
	app_WIFImod_rank_mod(&icon_3[3],&icon_temp[3]);
	global_memcpy(&icon_3[3],&icon_temp[3],3);
	
	step1=app_WIFImod_move_step_obtain(icon_1)
		+app_WIFImod_move_step_obtain(&icon_1[3]);
	step2=app_WIFImod_move_step_obtain(icon_2)
		+app_WIFImod_move_step_obtain(&icon_2[3]);
	step3=app_WIFImod_move_step_obtain(icon_3)
		+app_WIFImod_move_step_obtain(&icon_3[3]);
	
      step=MIN(step1,step2);
      step=MIN(step,step3);   
	  global_strcpy(cos,  pwir->cos);
      GLOBAL_MEMCLR(pwir, sizeof(WIRresult_t));
      if(step==step1)
      	{
	      app_WIFImod_icon_load(icon_1,pwir,1);
	}
      else if(step==step2)
      	{
	      app_WIFImod_icon_load(icon_2,pwir,1);
	}
      else if(step==step3)
      	{
	      app_WIFImod_icon_load(icon_3,pwir,1);
	}		
	pwir->phaseDir=temp1;
	pwir->verdict=temp2;
	global_strcpy(pwir->cos,cos);
    return; 
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :WIFI发送结果
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void WIR_task( void*arg )
{
	uint8_t err;
	int  pos;
	while ( 1 )
	{
		if ( os_sem_pend( m_WIFImod_sem,APP_100_MS) == OS_NO_ERR )
		{
			OSSemSet(m_WIFImod_sem,0,&err);
			m_Jx3_4 = m_alg_presult->jxmod;
			for(pos=0;pos<4;pos++)
			{
				_resulst_sys(&m_WIRresult[pos],pos);
				app_WIFImod_simpleness_alg(&m_WIRresult[pos]);
				m_WIRresult[pos].jxmod = m_Jx3_4;
				app_WIFImod_redeem_GX_obtian(&m_WIRresult[pos]);
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
void app_WIFImod_fun_entry( void )
{
	m_Jx3_4 = 3;
	m_WIRtask_id= os_task_assign_stk_build( WIR_task, 3);
	app_WIFImod_alg_entery();
	Wifi_init();
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
void app_WIFImod_fun_exit( void )
{
	if ( m_WIRtask_id != INVALID_PRO )
	{
		os_task_delete( m_WIRtask_id );
		m_WIRtask_id = INVALID_PRO;
	}
	app_WIFImod_alg_exit();
	return;
}

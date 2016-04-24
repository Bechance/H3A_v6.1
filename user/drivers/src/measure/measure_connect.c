/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:系统电参数测量驱动
* 创建时间:2010-11-20
* 所属公司:科荟
* 文件名称:measure_connect.c
* 创建人:揭成
* 功能描述: 测量模块连接层次
***************************************************************/
#include	"measure.h"
uint8_t  					m_measure_task_pro = INVALID_PRO;
correct_level_arg       	m_correct_level_arg;
extern measure_param_t  	m_measure_param;
extern measure_param_t  	m_measure_Real;
extern DL_correct_step_t    m_measure_correct_step;
DDS_contain_t           	m_DDS_contain; 
BOOL                        m_IAUTO_level_F = 1;
static BOOL 				m_measure_task_state = TURE;
static const float 			_line_Iconst[]={0.05,0.5,0.1,0.5,2.0,10.0,50.0,100.0,150.0};
static const float 			_line_Uconst[]={380.0,220.0,100.0,57.7};
int                 	    m_OV_reg = 0;
int                  		m_IAUTO_times = 0;

void EXTIO_init(void);
/********************************************************************
 * @创建人: 揭成
 * @功能: DDS计算角线参数
 * @输入: NONE
 * @输出: NONE
********************************************************************/
void measure_DDS_cos_obtain(measure_param_t*pDL)
{
	float curJ;
    float Iram;
	float CURjline;
	float CURdxangle;
    correct_level_arg*pl=&m_correct_level_arg;
	correct_data_t *Perr=&m_err_correct_data;
	
    Iram       = _line_Iconst[pl->ADLSR_select];
    CURdxangle = Perr->k_dxangle[A_PHASE];
	if(pDL->phase_a.f_I == 0)
		CURjline = 0;
	else
    	CURjline   = Iram/pDL->phase_a.f_I*Perr->k_Jline[A_PHASE];
	curJ       = CURjline+CURdxangle;
	if(curJ<2)
	{
		m_DDS_PQk.DDS_kcos[A_PHASE] =cos(curJ*3.1415926/180.0);
		m_DDS_PQk.DDS_ksin[A_PHASE] =sin(curJ*3.1415926/180.0);
	}

    CURdxangle = Perr->k_dxangle[B_PHASE];
	if(pDL->phase_b.f_I == 0)
		CURjline = 0;
	else
    	CURjline   = Iram/pDL->phase_b.f_I*Perr->k_Jline[B_PHASE];
	curJ = CURjline+CURdxangle;
	if(curJ<2)
	{
		m_DDS_PQk.DDS_kcos[B_PHASE] =cos(curJ*3.1415926/180.0);
		m_DDS_PQk.DDS_ksin[B_PHASE] =sin(curJ*3.1415926/180.0);
	}
    CURdxangle = Perr->k_dxangle[C_PHASE];
	if(pDL->phase_c.f_I == 0)
		CURjline = 0;
	else
    	CURjline   = Iram/pDL->phase_c.f_I*Perr->k_Jline[C_PHASE];
	curJ = CURjline+CURdxangle;
	if(curJ<2)
	{
		m_DDS_PQk.DDS_kcos[C_PHASE] =cos(curJ*3.1415926/180.0);
		m_DDS_PQk.DDS_ksin[C_PHASE] =sin(curJ*3.1415926/180.0);
	}
   return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: DDS计算中线形修正常数计算
 * @输入: NONE
 * @输出: NONE
********************************************************************/
float measure_DDS_Iline_const_obtain(void)
{
    float Iram;
	float Uram;
	float Pconst;
    correct_level_arg*pl=&m_correct_level_arg;
    Uram = _line_Uconst[pl->Ualevel_offset];
    Iram = _line_Iconst[pl->ADLSR_select];
	Pconst = Uram*Iram;
    return Pconst;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: DDS参数计算
 * @输入: NONE
 * @输出: NONE
********************************************************************/
void meausre_set_DDS_PQk(void)
{
	correct_data_t *Perr = &m_err_correct_data;
	K_UIR_t        *pkr  = &m_sUI_kr;
	float          pconst;
	pconst = measure_DDS_Iline_const_obtain();
    GLOBAL_MEMCLR(&m_DDS_PQk, sizeof(DDS_k_t));
	m_DDS_PQk.DDS_kcos[A_PHASE] = 1;
	m_DDS_PQk.DDS_ksin[A_PHASE] = 0; 
	m_DDS_PQk.DDS_kcos[B_PHASE] = 1;
	m_DDS_PQk.DDS_ksin[B_PHASE] = 0; 
	m_DDS_PQk.DDS_kcos[C_PHASE] = 1;
	m_DDS_PQk.DDS_ksin[C_PHASE] = 0; 
	
	if(m_DDS_contain.DDS_contain_A==1)
	{
	    m_DDS_PQk.DDS_Iline[A_PHASE]= pconst*Perr->k_Iline[A_PHASE];
		m_DDS_PQk.DDS_krpq[A_PHASE] = pkr->kr_Rpq[A_PHASE];
		m_DDS_PQk.DDS_kcos[A_PHASE] = Perr->k_cos[A_PHASE];
		m_DDS_PQk.DDS_ksin[A_PHASE] = Perr->k_sin[A_PHASE];
	}
	if(m_DDS_contain.DDS_contain_B==1)
	{
	    m_DDS_PQk.DDS_Iline[B_PHASE]= pconst*Perr->k_Iline[B_PHASE];
		m_DDS_PQk.DDS_krpq[B_PHASE] = pkr->kr_Rpq[B_PHASE];
		m_DDS_PQk.DDS_kcos[B_PHASE] = Perr->k_cos[B_PHASE];
		m_DDS_PQk.DDS_ksin[B_PHASE] = Perr->k_sin[B_PHASE];
	}
	if(m_DDS_contain.DDS_contain_C==1)
	{
	    m_DDS_PQk.DDS_Iline[C_PHASE]= pconst*Perr->k_Iline[C_PHASE];
		m_DDS_PQk.DDS_krpq[C_PHASE] = pkr->kr_Rpq[C_PHASE];
		m_DDS_PQk.DDS_kcos[C_PHASE] = Perr->k_cos[C_PHASE];
		m_DDS_PQk.DDS_ksin[C_PHASE] = Perr->k_sin[C_PHASE];
	}
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: DDS功率通道选择
 * @输入: NONE
 * @输出: NONE
********************************************************************/
void meausre_DDS_out_select(uint32_t a_phase,uint32_t b_phase,uint32_t c_phase)
{
   m_DDS_contain.DDS_contain_A = a_phase;
   m_DDS_contain.DDS_contain_B = b_phase;
   m_DDS_contain.DDS_contain_C = c_phase;
   meausre_set_DDS_PQk();
   return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 电压自动换档
 * @输入: NONE
 * @输出: NONE
********************************************************************/
const Ulevel_change_t m_Ulevel_tab[]=
{
	{U_INPUT_LEVEL_57V,60,62,1},
	{U_INPUT_LEVEL_100V,120,122,59},
	{U_INPUT_LEVEL_220V,240,242,110},
	{U_INPUT_LEVEL_440V,440.0,440.0,230}
};
/*判断当前电压属于哪个档位*/
static Bsize_t _Ulevel_obtain(float fUmax)
{
	Bsize_t offset;
	for(offset=0;offset<SIZE_OF_ARRAY(m_Ulevel_tab);offset++)
	{
		if(fUmax>m_Ulevel_tab[offset].f_min
			&&fUmax<m_Ulevel_tab[offset].f_max)
		break;
	}
	if(offset==SIZE_OF_ARRAY(m_Ulevel_tab)) 
		return U_INPUT_LEVEL_440V;
    return m_Ulevel_tab[offset].level;
}

static void _ULV_handle(uint16_t Lv)
{
    m_correct_level_arg.Ualevel_offset = Lv;
    m_correct_level_arg.Ublevel_offset = Lv;
    m_correct_level_arg.Uclevel_offset = Lv;
	measure_CTScale_select_U_level(&m_correct_level_arg);
	measure_correct_K_renew(&m_correct_level_arg); 
    return;
}

static int   _ULVtimes = 0;
static float _fUbak    = 0;

BOOL _ULV_law(float fU)
{
   float dx;
   int   ntimes = 5;
   if(_ULVtimes==0)
   {
      _fUbak = fU;_ULVtimes++;
      return FAIL;
   }
   dx = _fUbak-fU;

   if(m_correct_level_arg.Ualevel_offset==U_INPUT_LEVEL_440V)
   		ntimes = 50;
   else
   	    ntimes = 50;
   
   if(GLOBAL_ABS(dx)<15)
   {
       if(_ULVtimes>ntimes)
	   {
            _ULVtimes = 0;return TURE;
	   }
	   _ULVtimes++;return FAIL;
   }
   _ULVtimes = 0; return FAIL;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 电压自动换档
 * @输入: pDLarg 电参数
 * @输出: NONE
********************************************************************/
void measure_driver_auto_change_Ulevel(measure_param_t*pdl)
{
    float    fU_lev = 0.0,fU_levMAX,fU_levMIN;
	Bsize_t  offset;
	float    Umax;
	uint16_t CURlv;
    Umax = MAX(pdl->phase_a.f_U,pdl->phase_b.f_U);
	Umax = MAX(Umax,pdl->phase_c.f_U);
	if(!_ULV_law(Umax)) return;
    CURlv =  m_correct_level_arg.Ualevel_offset;
    if(Umax<18)
    {
		if(CURlv!= U_INPUT_LEVEL_440V)
		{
			_ULV_handle(U_INPUT_LEVEL_440V); 
		}
		return;
	}
	for(offset=0;offset<SIZE_OF_ARRAY(m_Ulevel_tab);offset++)
	{ 
	    if(CURlv==m_Ulevel_tab[offset].level)
	    {
	    	  fU_lev = m_Ulevel_tab[offset].f_change_value;
			  fU_levMAX = m_Ulevel_tab[offset].f_max;
			  fU_levMIN = m_Ulevel_tab[offset].f_min;
	    }
	}
	if(Umax>fU_lev)
    {
		if(CURlv!= U_INPUT_LEVEL_440V)
		{
			_ULV_handle(U_INPUT_LEVEL_440V);  
		}
		return;
	}
	else if(Umax<=fU_levMAX&&Umax>fU_levMIN)
	{
     	 return;
	}
	offset = _Ulevel_obtain(Umax);
	if(CURlv!=offset)
    {
		_ULV_handle(offset); 
	}
	return;
}
/****************************************************************/
//电流自动换档
/****************************************************************/
static int  _Icheck_times = 0;
static void _ILV_handle(DLSR_CT_t Iin)
{
    m_IAUTO_times++;
    m_correct_level_arg.ADLSR_select   = Iin;
    m_correct_level_arg.BDLSR_select   = Iin;
    m_correct_level_arg.CDLSR_select   = Iin;
    if(m_correct_level_arg.ADLSR_select==DZ_N_1A
		||m_correct_level_arg.ADLSR_select==QB_N_1A)
    {
         m_correct_level_arg.Ilevel_offset = I_INPUT_10;
	}
	else 
    {
         m_correct_level_arg.Ilevel_offset = I_INPUT_1;
	}
	measure_CTScale_select_I_level(&m_correct_level_arg);
	measure_CTScale_select_U_level(&m_correct_level_arg);
	measure_correct_K_renew(&m_correct_level_arg); 
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能  : 电流自动换档
 * @输入  : pDLarg 电参数
 * @输出  : NONE
********************************************************************/
void measure_driver_Auto_change_Ilevel(measure_param_t*pdl)
{
	float    Imax;
	uint16_t CURlv;
	Imax = MAX(pdl->phase_a.f_I,pdl->phase_b.f_I);
	Imax = MAX(Imax,pdl->phase_c.f_I);
	CURlv =  m_correct_level_arg.ADLSR_select;
	if(CURlv == DZ_N_10A||CURlv==QB_N_5A)
	{
		if(Imax<0.8&&Imax>0.02)
		{
			if(++_Icheck_times>10)
			{
				_Icheck_times = 0; 
				if(CURlv==DZ_N_10A)
               	 	_ILV_handle(DZ_N_1A);
				else
               	 	_ILV_handle(QB_N_1A);
			}
		}
		else
		{
            _Icheck_times = 0;
		}
	}
	else if(CURlv == DZ_N_1A||CURlv==QB_N_1A)
	{
		if(Imax>1.0)
		{
			if(++_Icheck_times>4)
			{
				_Icheck_times = 0;   
				if(CURlv==DZ_N_1A)
	           	 	_ILV_handle(DZ_N_10A);
				else
	           	 	_ILV_handle(QB_N_5A);
			}
		}
		else
		{
            _Icheck_times = 0;
		}
	}
}


static void measure_OVUI_sys(void)
{
   int offset;
   int m_OVUI_reg = 0;  
   DLSR_CT_t CURlv;
   CURlv =  m_correct_level_arg.ADLSR_select;
   for(offset=0;offset<1024;offset++)
   {
      if(GLOBAL_ABS(m_Ia_buf[offset])>31000)m_OVUI_reg++;
      if(GLOBAL_ABS(m_Ib_buf[offset])>31000)m_OVUI_reg++;
      if(GLOBAL_ABS(m_Ic_buf[offset])>31000)m_OVUI_reg++;
      if(GLOBAL_ABS(m_Ua_buf[offset])>31000)m_OVUI_reg++;
      if(GLOBAL_ABS(m_Ub_buf[offset])>31000)m_OVUI_reg++;
      if(GLOBAL_ABS(m_Uc_buf[offset])>31000)m_OVUI_reg++;
   }
   if(m_OVUI_reg>1)
   {
        m_correct_level_arg.Ualevel_offset = U_INPUT_LEVEL_440V;
		m_correct_level_arg.Ublevel_offset = U_INPUT_LEVEL_440V;
		m_correct_level_arg.Uclevel_offset = U_INPUT_LEVEL_440V;
		if(CURlv==DZ_N_1A)
			_ILV_handle(DZ_N_10A);
		else if(CURlv==QB_N_1A)
			_ILV_handle(QB_N_5A);
		else {m_OV_reg++;_ILV_handle(CURlv);}
		EXTIO_init();  
   }
   else m_OV_reg = 0;
   return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能:接线模式分析
 * @输入: pDLarg 电参数
 * @输出:	NONE
********************************************************************/
void measure_driver_JXmod_sys( DL_flag_t*pflag,measure_param_t*pdl)
{
    if(GLOBAL_ABS(pdl->f_JUac)<70&&(pdl->phase_b.f_U <40)
    &&(pdl->phase_a.f_U >40)&&(pdl->phase_c.f_U >40))
    {
        pflag->phase3or4 = JXMOD_3_3;
    }
    else 
        pflag->phase3or4 = JXMOD_3_4;
    return;
}

BOOL measure_identity_check(void)
{
   return TURE;
}

static float measure_lostUI_minI_obtain(DLSR_CT_t DLSR_select)
{
    float min_level;
	int pos   = DLSR_select;
	min_level = _line_Iconst[pos]*0.1;
    return min_level;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 失压失流判断
 * @输入:	
 * @输出:NONE
********************************************************************/
void measure_lostUI_sys(DL_flag_t*pflag,measure_param_t*pdl)
{
    #define  MIN_LOST_U          15
	#define  DLARG_CLEAN(phase)  {phase.f_J=0;phase.f_Q=0;phase.f_P=0;}
	float  min_level  = measure_lostUI_minI_obtain(m_correct_level_arg.ADLSR_select);
	pflag->Ua_is_lost = FAIL;pflag->Ub_is_lost = FAIL;
	pflag->Uc_is_lost = FAIL;pflag->Ia_is_lost = FAIL;
	pflag->Ib_is_lost = FAIL;pflag->Ic_is_lost = FAIL;
	if ( GLOBAL_ABS( pdl->phase_a.f_U) < MIN_LOST_U )
	{
		pflag->Ua_is_lost = TURE;  
		pdl->phase_a.f_U = 0.00;
		pdl->f_JUab = 0.0;
		pdl->f_JUac = 0.0;
		DLARG_CLEAN( pdl->phase_a );
	}
	if ( GLOBAL_ABS( pdl->phase_b.f_U) < MIN_LOST_U )
	{
		pflag->Ub_is_lost = TURE;  
		pdl->phase_b.f_U = 0.00;
		pdl->f_JUab = 0.0;
		pdl->f_JUbc = 0.0;
		DLARG_CLEAN( pdl->phase_b);
	}
	if ( GLOBAL_ABS( pdl->phase_c.f_U) < MIN_LOST_U)
	{
		pflag->Uc_is_lost = TURE;  
		pdl->phase_c.f_U = 0.00;
		pdl->f_JUac = 0.0;
		pdl->f_JUbc = 0.0;
		DLARG_CLEAN( pdl->phase_c );
	}
	if ( GLOBAL_ABS( pdl->phase_a.f_I ) < min_level )
	{
		pflag->Ia_is_lost = TURE;  
		pdl->phase_a.f_I = 0;
		pdl->f_JIab = 0.0;
		pdl->f_JIac = 0.0;	
		DLARG_CLEAN( pdl->phase_a );
	}
	min_level  = measure_lostUI_minI_obtain(m_correct_level_arg.BDLSR_select);
	if ( GLOBAL_ABS( pdl->phase_b.f_I ) < min_level )
	{
		pdl->f_JIab = 0.0;
		pdl->f_JIbc = 0.0;	
		pflag->Ib_is_lost = TURE;  
		pdl->phase_b.f_I = 0;
		DLARG_CLEAN( pdl->phase_b );
	}
	min_level  = measure_lostUI_minI_obtain(m_correct_level_arg.CDLSR_select);
	if ( GLOBAL_ABS( pdl->phase_c.f_I ) < min_level )
	{
		pdl->f_JIac= 0.0;
		pdl->f_JIbc = 0.0;	
		pflag->Ic_is_lost = TURE;  
		pdl->phase_c.f_I = 0;
		DLARG_CLEAN( pdl->phase_c );
	}	
	if((m_measure_flag.phase3or4==JXMOD_3_3))
	{
		pflag->Ub_is_lost = TURE;  
		pdl->phase_b.f_U = 0.00;
		pdl->f_JIab = 0.0;
		pdl->f_JIbc = 0.0;	
		DLARG_CLEAN( pdl->phase_b );
	}
    pdl->f_Pabc = pdl->phase_a.f_P+pdl->phase_b.f_P+pdl->phase_c.f_P;
    pdl->f_Qabc = pdl->phase_a.f_Q+pdl->phase_b.f_Q+pdl->phase_c.f_Q;
    if(pdl->f_Pabc==0&&pdl->f_Qabc){pdl->f_Cos = 0;pdl->cos_LC = 0;}
    return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 判断当前的琐相信号是否正常
 * @输入:	
 * @输出:NONE
********************************************************************/
static BOOL measure_auto_select_PLL_is_ready(DL_flag_t*pflag)
{
   if(m_pll_pos!=0&&pflag->Ua_is_lost!=TURE) return TURE;
   if(m_pll_pos==0&&pflag->Ua_is_lost==TURE) return TURE;
   if(m_pll_pos==1&&pflag->Ub_is_lost==TURE) return TURE;
   if(m_pll_pos==2&&pflag->Uc_is_lost==TURE) return TURE;
   if(m_pll_pos==3&&pflag->Ia_is_lost==TURE) return TURE;
   if(m_pll_pos==4&&pflag->Ib_is_lost==TURE) return TURE;
   if(m_pll_pos==5&&pflag->Ic_is_lost==TURE) return TURE;
   return FAIL;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 自动选择锁相环输入信号
 * @输入:	
 * @输出:NONE
********************************************************************/
void measure_auto_select_PLL(DL_flag_t*pflag)
{
   if(measure_auto_select_PLL_is_ready(pflag)) /*判断是否需要换锁相信号*/
   {
      if(!pflag->Ua_is_lost) measure_CTScale_select_PLL('u','a'); //选择a相电压通道
      else if(!pflag->Ub_is_lost) measure_CTScale_select_PLL('u','b'); //选择b相电压通道
      else if(!pflag->Uc_is_lost) measure_CTScale_select_PLL('u','c'); //选择c相电压通道
      else if(!pflag->Ia_is_lost) measure_CTScale_select_PLL('i','a'); //选择a相电流通道
      else if(!pflag->Ib_is_lost) measure_CTScale_select_PLL('i','b'); //选择b相电流通道
      else if(!pflag->Ic_is_lost) measure_CTScale_select_PLL('i','c'); //选择c相电流通道
   }
   return;
}


/********************************************************************
 * @创建人: 揭成
 * @功能:测量任务
 * @输入:	
 * @输出:NONE
********************************************************************/
void measure_task(void*arg)
{
	Bsize_t phase = 0;
	measure_param_t *pDL,*pReal;
	int     cnt,rst = 0;
	pDL    = &m_measure_param;
	pReal  = &m_measure_Real;
    msleep(APP_100_MS*10);
	measure_driver_read_DLarg_init();
	GLOBAL_MEMCLR(pDL,sizeof(measure_param_t));
	GLOBAL_MEMCLR(pReal,sizeof(measure_param_t));
	while(1)
	{
		msleep(1);
		measure_driver_correct_handle(pDL,&m_measure_correct_step);
        measure_error_AA(pReal);
		if(++rst>100)
		{
			measure_driver_read_DLarg_init();
			measure_CTScale_select_I_level(&m_correct_level_arg);
			measure_CTScale_select_U_level(&m_correct_level_arg);
			measure_correct_K_renew(&m_correct_level_arg); 
			EXTIO_init();
		}
		if(m_read_ad_finish)
		{
			rst = 0;
			os_lock( m_measure_lock );
			measure_data_obtain(pReal);
			measure_driver_auto_change_Ulevel(pReal);
			if(m_IAUTO_level_F)measure_driver_Auto_change_Ilevel(pReal);
			measure_data_filtering(pReal,pDL);
			os_unlock( m_measure_lock );
		    measure_DDS_cos_obtain(pDL);
			measure_driver_JXmod_sys(&m_measure_flag,pDL);
			measure_lostUI_sys(&m_measure_flag,pDL);
			measure_driver_count_COS(pDL);
			measure_auto_select_PLL(&m_measure_flag);
			m_read_ad_finish = FAIL;
			m_measure_flag.DL_data_is_new = TURE;
		    if(++cnt%50==0)
		    {
				fft_Exc(phase);
				if(phase==5)
				{
					m_measure_flag.DL_XB_is_new = TURE;
				}
				phase=(phase==5)?(0):(phase+1);
		    }
			if(cnt%200==0)measure_OVUI_sys();
		}
	}
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 测量开始
 * @输入:	
 * @输出: NONE
********************************************************************/
void measure_driver_start(void)
{
	measue_GetADdata_start();
	meausre_DDS_out_select(1,1,1);
	if(!m_measure_task_state)
	{
		OSTaskResume(m_measure_task_pro);
		m_measure_task_state = TURE;
	}
	m_IAUTO_level_F = TURE;
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 测量停止
 * @输入:	
 * @输出: NONE
********************************************************************/
void measure_driver_stop(void)
{
   measue_GetADdata_stop();
   if(m_measure_task_state)
   {
      m_measure_task_state = FAIL;
      OSTaskSuspend(m_measure_task_pro);
   }
   return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 进入测量模块
 * @输入:	
 * @输出: NONE
********************************************************************/
void measure_entery(void)
{
    fft_init();
	measure_GetADdata_init();
	meausre_DDS_out_select(1,1,1);
    measure_correct_data_module_init();
	measure_CTScale_init();
 	measure_error_count_init();
	measure_driver_read_DLarg_init();
	m_measure_task_pro
	=os_task_assign_stk_build(measure_task,5);
	m_measure_task_state = TURE;
	m_IAUTO_level_F      = TURE;
	m_OV_reg             = 0;
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 退出测量模块
 * @输入:	
 * @输出: NONE
********************************************************************/
void measure_exit(void)
{
    PieCtrlRegs.PIEIER1.bit.INTx4 = 0;
	os_task_delete(m_measure_task_pro);
	return;
}

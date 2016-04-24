/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:ϵͳ�������������
* ����ʱ��:2012-02-20
* ������˾:����
* �ļ�����:measure_count.c
* ������:�ҳ�
* ��������: ���ܸ������������㷨�����ϡ�
***************************************************************/
#include	"measure.h"
/***************************************************************************
����ȥ�������ݻ����
***************************************************************************/
#define  FL_DATA_BUF_SIZE      	20
#define  ERR_TIMES   	 		5
#define  ERR_VALUE1   			0.001
float    			m_FLJa_buf[FL_DATA_BUF_SIZE];
float   			m_FLJb_buf[FL_DATA_BUF_SIZE];
float    			m_FLJc_buf[FL_DATA_BUF_SIZE];
filtering_data_t  	m_Ja_filtering_ctl;
filtering_data_t  	m_Jb_filtering_ctl;
filtering_data_t  	m_Jc_filtering_ctl;
period_average_t    m_U_period_ctl[3];
period_average_t    m_I_period_ctl[3];
period_average_t    m_P_period_ctl[3];
period_average_t    m_Q_period_ctl[3];
/********************************************************************
 * @������:�ҳ�
 * @����:ʵ��64BIT�������ݿ����������㺯������64λ������16λ
 * @����:datain 64λ�޷�������
 * @���:NONE
********************************************************************/
#pragma CODE_SECTION(measure_64bit_div_16_4,"ramfuncs") //���öδ���������ramfuncs��
static float measure_64bit_div_16_4(ad_64bit_u  datain)
{
	float    fdatah;
	float    fdatal;
	uint32_t div_data = 0x10000;
	fdatah=(float)datain.bytes_2.byte_1*0x10000;
	fdatal=(float)datain.bytes_2.byte_0/div_data;
	return (fdatah+fdatal);
}
/********************************************************************
 * @������:�ҳ�
 * @����:ʵ��64BIT�������ݿ�������
 * @����:indata 64λ�޷�������
 * @���:NONE
********************************************************************/
#pragma CODE_SECTION(measure_sqrt_64bit_data,"ramfuncs") //���öδ���������ramfuncs��
float measure_sqrt_64bit_data(uint64_t indata)
{
	float fdata;
	float res;
	ad_64bit_u  datain;
	datain.all = indata;
	if(datain.bytes_2.byte_1==0)		/*�����32λΪ0������������*/
	{   
	    fdata = (float)datain.bytes_2.byte_0;
		res=sqrt(fdata);
	}
	else
	{
		fdata=measure_64bit_div_16_4(datain);
		res=sqrt(fdata);
		res=res*0x100;
	}
	return res;
}
/********************************************************************
 * @������:�ҳ�
 * @����:����������
 * @����:pDLarg �����
 * @���:NONE
********************************************************************/
static char measure_LC_obtain( measure_param_t*pDL)
{
   #define GET_JA() (pDL->phase_a.f_J)
   #define GET_JB() (pDL->phase_b.f_J)
   #define GET_JC() (pDL->phase_c.f_J)
	if ( m_measure_flag.phase3or4== JXMOD_3_3 )
	{
		if ((GET_JA() <30.0 )&&(GET_JC()<-30.0)) return 'C';
		if ((GET_JA()> 30.0 )&&(GET_JC()> -30.0))return 'L';
		return (pDL->f_Qabc < 0)?('C'):('L');
	}
	else
	{
		if ((GET_JA()<0.0)&&(GET_JC()<0.0)&&(GET_JB()<0.0))return 'C';
		if ((GET_JA()>0.0)&&(GET_JC()>0.0)&&(GET_JB()>0.0))return 'L';
	    return (pDL->f_Qabc<0)?('C'):('L');
	}
}
/********************************************************************
 * @������: �ҳ�
 * @����: ���㹦������
 * @����: pDLarg �����
 * @���: NONE
********************************************************************/
void measure_driver_count_COS( measure_param_t*pDL)
{
#define MUL_UI(PHASE) (pDL->PHASE.f_U*pDL->PHASE.f_I)
	float S;
	uint8_t phase3or4;
	phase3or4 = m_measure_flag.phase3or4;
	if ( pDL->f_Pabc != 0 )
	{
		S=MUL_UI(phase_a)+MUL_UI(phase_b)+MUL_UI(phase_c);
		pDL->f_Cos = pDL->f_Pabc/S;
		if(phase3or4==JXMOD_3_3)
		{
			pDL->f_Cos = pDL->f_Cos/0.866;
		}
        pDL->cos_LC = measure_LC_obtain(pDL);
	}
	else
	{
		pDL->f_Cos = 0;
	}
	if(GLOBAL_ABS(pDL->f_Cos)>1.0)
	{
       pDL->f_Cos = 1.0;
	}
	if(pDL->f_Cos==1.0)pDL->cos_LC=' ';
	return;
}
/********************************************************************
 * @������: �ҳ�
 * @����: ��ѹ�������ݼ������
 * @����:	
 * @���: NONE 
********************************************************************/
static void measure_UI_data_obtain(measure_param_t*pDL,correct_data_t*Pcorrect)
{
#define   MSQRT64      				measure_sqrt_64bit_data 
#define   U_IS_LAWFUL(data)			(GLOBAL_ABS(data)>1000)?(0):(data);
#define   I_IS_LAWFUL(data)			(GLOBAL_ABS(data)>5000)?(0):(data);
    sum_data_t*psum = &m_sum_data_ext;
	/*����3���ѹ,����3�����*/
	pDL->phase_a.f_U = MSQRT64(psum->sum_U[A_PHASE])*(Pcorrect->k_Umain[A_PHASE]+1)*m_sUI_kr.kr_RU[A_PHASE];
	pDL->phase_b.f_U = MSQRT64(psum->sum_U[B_PHASE])*(Pcorrect->k_Umain[B_PHASE]+1)*m_sUI_kr.kr_RU[B_PHASE];
	pDL->phase_c.f_U = MSQRT64(psum->sum_U[C_PHASE])*(Pcorrect->k_Umain[C_PHASE]+1)*m_sUI_kr.kr_RU[C_PHASE];
	pDL->phase_a.f_I = MSQRT64(psum->sum_I[A_PHASE])*(Pcorrect->k_Imain[A_PHASE]+1)*m_sUI_kr.kr_RI[A_PHASE];	
	pDL->phase_b.f_I = MSQRT64(psum->sum_I[B_PHASE])*(Pcorrect->k_Imain[B_PHASE]+1)*m_sUI_kr.kr_RI[B_PHASE];
	pDL->phase_c.f_I = MSQRT64(psum->sum_I[C_PHASE])*(Pcorrect->k_Imain[C_PHASE]+1)*m_sUI_kr.kr_RI[C_PHASE];
    pDL->phase_a.f_U = U_IS_LAWFUL(pDL->phase_a.f_U);
    pDL->phase_b.f_U = U_IS_LAWFUL(pDL->phase_b.f_U);
    pDL->phase_c.f_U = U_IS_LAWFUL(pDL->phase_c.f_U);
    pDL->phase_a.f_I = I_IS_LAWFUL(pDL->phase_a.f_I);
    pDL->phase_b.f_I = I_IS_LAWFUL(pDL->phase_b.f_I);
    pDL->phase_c.f_I = I_IS_LAWFUL(pDL->phase_c.f_I);
	return;
}
/********************************************************************
 * @������: �ҳ�
 * @����: �й�,�޹����ݼ���
 * @����:	
 * @���: NONE
********************************************************************/
extern float        m_phase_fp[3];
extern float        m_phase_fq[3];
static void measure_PQ_data_obtain(measure_param_t*pDL,correct_data_t*Pcorrect)
{
	pDL->phase_a.f_P=m_phase_fp[A_PHASE];
	pDL->phase_a.f_Q=m_phase_fq[A_PHASE];
	pDL->phase_b.f_P=m_phase_fp[B_PHASE];
	pDL->phase_b.f_Q=m_phase_fq[B_PHASE];
	pDL->phase_c.f_P=m_phase_fp[C_PHASE];
	pDL->phase_c.f_Q=m_phase_fq[C_PHASE];
	pDL->f_Pabc = pDL->phase_a.f_P+pDL->phase_b.f_P+pDL->phase_c.f_P;
	pDL->f_Qabc = pDL->phase_a.f_Q+pDL->phase_b.f_Q+pDL->phase_c.f_Q;
    return;
}
/********************************************************************
 * @������: �ҳ�
 * @����: ������ȡ�Ƕ�ת��
 * @����: flag=0 (-180-180)   flag =1 (0-360)
 * @���: NONE
********************************************************************/
void measure_DLarg_angle_trans(measure_param_t*pDL)
{
#define  ANGLE_TRANS_1(angle)   (angle<-180.0)?(angle+360):(angle)
#define  ANGLE_TRANS_2(angle)   (angle>180)?(angle-360):(angle)
#define  ANGLE_TRANS(angle)     {angle=ANGLE_TRANS_1(angle);angle=ANGLE_TRANS_2(angle);}
	ANGLE_TRANS( pDL->phase_a.f_J );
	ANGLE_TRANS( pDL->phase_b.f_J );
	ANGLE_TRANS( pDL->phase_c.f_J );
	ANGLE_TRANS( pDL->f_JUab );
	ANGLE_TRANS( pDL->f_JUac );
	ANGLE_TRANS( pDL->f_JUbc );
	ANGLE_TRANS( pDL->f_JIab );
	ANGLE_TRANS( pDL->f_JIac );
	ANGLE_TRANS( pDL->f_JIbc );
	return;
}
/********************************************************************
 * @������: �ҳ�
 * @����: �Ƕȼ���
 * @����:	
 * @���: NONE
********************************************************************/
void meausre_angle_data_obtain(measure_param_t*pDL)
{
#define  UA_ANGLE_START  fft_angle_obtain(1,0)
#define  IA_ANGLE_START  fft_angle_obtain(1,1)
#define  UB_ANGLE_START  fft_angle_obtain(1,2)
#define  IB_ANGLE_START  fft_angle_obtain(1,3)
#define  UC_ANGLE_START  fft_angle_obtain(1,4)
#define  IC_ANGLE_START  fft_angle_obtain(1,5)
	pDL->phase_a.f_J = 0;
	pDL->phase_b.f_J = 0;
	pDL->phase_c.f_J = 0;
	pDL->f_JIab  	 = IB_ANGLE_START-IA_ANGLE_START;
	pDL->f_JIac 	 = IC_ANGLE_START-IA_ANGLE_START;
	pDL->f_JIbc 	 = IC_ANGLE_START-IB_ANGLE_START;
	pDL->f_JUab  	 = UB_ANGLE_START-UA_ANGLE_START;
	pDL->f_JUac 	 = UC_ANGLE_START-UA_ANGLE_START;
	pDL->f_JUbc 	 = UC_ANGLE_START-UB_ANGLE_START;	
	if ( pDL->phase_a.f_Q != 0 && pDL->phase_a.f_P != 0 )
	{
		pDL->phase_a.f_J = atan2(pDL->phase_a.f_Q,pDL->phase_a.f_P)/3.1415926*180 ;
	}
	if ( pDL->phase_b.f_Q != 0 && pDL->phase_b.f_P != 0 )
	{
		pDL->phase_b.f_J = atan2(pDL->phase_b.f_Q,pDL->phase_b.f_P)/3.1415926*180 ;
	}
	if ( pDL->phase_c.f_Q != 0 && pDL->phase_c.f_P != 0 )
	{
		pDL->phase_c.f_J = atan2(pDL->phase_c.f_Q,pDL->phase_c.f_P)/3.1415926*180 ;
	}
	measure_DLarg_angle_trans(pDL);
	return;
}
/********************************************************************
 * @������: �ҳ�
 * @����: Ƶ�ʼ���
 * @����:	
 * @���: NONE
********************************************************************/
static void measure_freq_data_obtain(measure_param_t*pDL)
{
	float 	 fdata;
	uint32_t data;
	data  =  READ32BIT_DATA(READ_FREQ);
	fdata = (float)data/6.0/1000.0/1000.0;
	if(fdata==0)
	{
		 pDL->f_Feq = 0;
		 return;
	}
 	pDL->f_Feq = 1.0/fdata;
	return;
}
/********************************************************************
 * @������: �ҳ�
 * @����: �������ݼ���
 * @����:	
 * @���: NONE
********************************************************************/
void measure_data_obtain(measure_param_t*pDL)
{	
	correct_data_t*Pcorrect =&m_err_correct_data; //��ȡ����ϵ��
	measure_UI_data_obtain(pDL,Pcorrect);
	measure_PQ_data_obtain(pDL,Pcorrect);
 	meausre_angle_data_obtain(pDL);
	measure_freq_data_obtain(pDL);
	return;
}
/********************************************************************
 * @������: �ҳ�
 * @����: ��������ȥ������
 * @����:	
 * @���: NONE
********************************************************************/
void measure_data_filtering(measure_param_t*psrc,measure_param_t*pdes)
{	
    OSSchedLock();
	*pdes = *psrc;
    pdes->phase_a.f_U=period_average_data_obtain(&m_U_period_ctl[A_PHASE],psrc->phase_a.f_U,0);
    pdes->phase_b.f_U=period_average_data_obtain(&m_U_period_ctl[B_PHASE],psrc->phase_b.f_U,0);
    pdes->phase_c.f_U=period_average_data_obtain(&m_U_period_ctl[C_PHASE],psrc->phase_c.f_U,0);
    pdes->phase_a.f_I=period_average_data_obtain(&m_I_period_ctl[A_PHASE],psrc->phase_a.f_I,0);
    pdes->phase_b.f_I=period_average_data_obtain(&m_I_period_ctl[B_PHASE],psrc->phase_b.f_I,0);
    pdes->phase_c.f_I=period_average_data_obtain(&m_I_period_ctl[C_PHASE],psrc->phase_c.f_I,0);
    pdes->phase_a.f_P=period_average_data_obtain(&m_P_period_ctl[A_PHASE],psrc->phase_a.f_P,0);
    pdes->phase_b.f_P=period_average_data_obtain(&m_P_period_ctl[B_PHASE],psrc->phase_b.f_P,0);
    pdes->phase_c.f_P=period_average_data_obtain(&m_P_period_ctl[C_PHASE],psrc->phase_c.f_P,0);
    pdes->phase_a.f_Q=period_average_data_obtain(&m_Q_period_ctl[A_PHASE],psrc->phase_a.f_Q,0);
    pdes->phase_b.f_Q=period_average_data_obtain(&m_Q_period_ctl[B_PHASE],psrc->phase_b.f_Q,0);
    pdes->phase_c.f_Q=period_average_data_obtain(&m_Q_period_ctl[C_PHASE],psrc->phase_c.f_Q,0);
    pdes->phase_a.f_J=shift_filtering_data_obtain(&m_Ja_filtering_ctl,psrc->phase_a.f_J);
    pdes->phase_b.f_J=shift_filtering_data_obtain(&m_Jb_filtering_ctl,psrc->phase_b.f_J);
    pdes->phase_c.f_J=shift_filtering_data_obtain(&m_Jc_filtering_ctl,psrc->phase_c.f_J);
    OSSchedUnlock();
	return;
}
/********************************************************************
 * @������:�ҳ�
 * @����:����ȥ����ʼ��
 * @����:	
 * @���:NONE
********************************************************************/
void  measure_driver_read_DLarg_init( void )
{
    period_average_create(&m_U_period_ctl[A_PHASE],64);
    period_average_create(&m_U_period_ctl[B_PHASE],64);
    period_average_create(&m_U_period_ctl[C_PHASE],64);
    period_average_create(&m_I_period_ctl[A_PHASE],64);
    period_average_create(&m_I_period_ctl[B_PHASE],64);
    period_average_create(&m_I_period_ctl[C_PHASE],64);
    period_average_create(&m_P_period_ctl[A_PHASE],64);
    period_average_create(&m_P_period_ctl[B_PHASE],64);
    period_average_create(&m_P_period_ctl[C_PHASE],64);
    period_average_create(&m_Q_period_ctl[A_PHASE],64);
    period_average_create(&m_Q_period_ctl[B_PHASE],64);
    period_average_create(&m_Q_period_ctl[C_PHASE],64);
	shift_filtering_data_algorithm_creat(m_FLJa_buf, FL_DATA_BUF_SIZE,IS_FLOAT,0.5,3, &m_Ja_filtering_ctl );
	shift_filtering_data_algorithm_creat(m_FLJb_buf, FL_DATA_BUF_SIZE,IS_FLOAT,0.5,3, &m_Jb_filtering_ctl );
	shift_filtering_data_algorithm_creat(m_FLJc_buf, FL_DATA_BUF_SIZE,IS_FLOAT,0.5,3, &m_Jc_filtering_ctl );
	return;
}

/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:ϵͳ�������������
* ����ʱ��:2010-12-20
* ������˾:����
* �ļ�����:measure_GetData.c
* ������:�ҳ�
* ��������: ����ģ�����ݻ�ȡ
*****************************************************************/
#include	"measure.h"
#define      AD_BITS           		16
/****************************************************************/
#define     AD_PAGE_SIZE            16
#define     MSQUARE(val)           (val*val)
#define     MSQRT(val)              sqrt(val)
#define     DDS_CONST              (4474)
#define     RSHIFT_BIT              10
/****************************************************************/
#pragma  	DATA_SECTION (m_Ia_buf, 		"FAST_RAM")/*�ѱ���������FAST_RAM*/
#pragma  	DATA_SECTION (m_Ib_buf, 		"FAST_RAM")/*�ѱ���������FAST_RAM*/
#pragma  	DATA_SECTION (m_Ic_buf, 		"FAST_RAM")/*�ѱ���������FAST_RAM*/
#pragma  	DATA_SECTION (m_Ua_buf, 		"FAST_RAM")/*�ѱ���������FAST_RAM*/
#pragma  	DATA_SECTION (m_Ub_buf, 		"FAST_RAM")/*�ѱ���������FAST_RAM*/
#pragma  	DATA_SECTION (m_Uc_buf, 		"FAST_RAM")/*�ѱ���������FAST_RAM*/
#pragma  	DATA_SECTION (m_ad_buf_offset, 	"FAST_RAM")/*�ѱ���������FAST_RAM*/
#pragma  	DATA_SECTION (m_int_offset, 	"FAST_RAM")/*�ѱ���������FAST_RAM*/
#pragma  	DATA_SECTION (m_addr_offset, 	"FAST_RAM")/*�ѱ���������FAST_RAM*/
#pragma  	DATA_SECTION (m_sum_data_int, 	"FAST_RAM")/*�ѱ���������FAST_RAM*/
#pragma  	DATA_SECTION (m_sum_data_ext, 	"FAST_RAM")/*�ѱ���������FAST_RAM*/
#pragma  	DATA_SECTION (m_start_offset, 	"FAST_RAM")/*�ѱ���������FAST_RAM*/
#pragma  	DATA_SECTION (m_read_ad_finish, "FAST_RAM")/*�ѱ���������FAST_RAM*/
#pragma  	DATA_SECTION (m_ad_64bit, 		"FAST_RAM")/*�ѱ���������FAST_RAM*/
#pragma  	DATA_SECTION (m_ad_32bit, 		"FAST_RAM")/*�ѱ���������FAST_RAM*/
#pragma  	DATA_SECTION (m_90_offset,      "FAST_RAM")/*�ѱ���������FAST_RAM*/
#pragma  	DATA_SECTION (m_DDS_PQk, 		"FAST_RAM")/*�ѱ���������FAST_RAM*/
#pragma  	DATA_SECTION (m_DDS_path1_data, "FAST_RAM")/*�ѱ���������FAST_RAM*/
#pragma  	DATA_SECTION (m_DDS_path2_data, "FAST_RAM")/*�ѱ���������FAST_RAM*/
#pragma  	DATA_SECTION (m_DC_Udx,         "FAST_RAM")/*�ѱ���������FAST_RAM*/
#pragma  	DATA_SECTION (m_DC_Idx,         "FAST_RAM")/*�ѱ���������FAST_RAM*/
#pragma  	DATA_SECTION (m_DDS_path_mod,   "FAST_RAM")/*�ѱ���������FAST_RAM*/
#pragma  	DATA_SECTION (m_phase_fp,       "FAST_RAM")/*�ѱ���������FAST_RAM*/
#pragma  	DATA_SECTION (m_phase_fq,   	"FAST_RAM")/*�ѱ���������FAST_RAM*/
#pragma  	DATA_SECTION (m_32bit_U,     	"FAST_RAM")/*�ѱ���������FAST_RAM*/
#pragma  	DATA_SECTION (m_32bit_I,     	"FAST_RAM")/*�ѱ���������FAST_RAM*/
#pragma  	DATA_SECTION (m_32bit_P,     	"FAST_RAM")/*�ѱ���������FAST_RAM*/
#pragma  	DATA_SECTION (m_32bit_Q,     	"FAST_RAM")/*�ѱ���������FAST_RAM*/
#pragma  	DATA_SECTION (m_64bit_P,     	"FAST_RAM")/*�ѱ���������FAST_RAM*/
#pragma  	DATA_SECTION (m_64bit_Q,     	"FAST_RAM")/*�ѱ���������FAST_RAM*/
#pragma  	DATA_SECTION (m_64bit_UI,     	"FAST_RAM")/*�ѱ���������FAST_RAM*/
/****************************************************************/
float 		 measure_sqrt_64bit_data(uint64_t indata);
static       Bsize_t 	 	m_int_offset  	=0;   	//�жϿ�����ʱ������Ϊ�˼����ж���ջ��������
static       Bsize_t 	 	m_start_offset	=0; 	//�жϿ�����ʱ������Ϊ�˼����ж���ջ��������
static       Bsize_t 	 	m_addr_offset 	=0; 	//�жϿ�����ʱ������Ϊ�˼����ж���ջ��������
static       Bsize_t     	m_ad_buf_offset	=0;		//��ǰ����λ��
static       Bsize_t     	m_90_offset		=768;	//�ƶ�90�Ⱥ����m_ad_buf_offset��ƫ�ơ�  	    	    
/****************************************************************/
int16_t 	 	m_Ia_buf[AD_SIZE];       			//A�����AD����BUF     
int16_t      	m_Ib_buf[AD_SIZE];       			//B�����AD����BUF     
int16_t      	m_Ic_buf[AD_SIZE];       			//C�����AD����BUF     
int16_t      	m_Ua_buf[AD_SIZE];       			//A���ѹAD����BUF     
int16_t      	m_Ub_buf[AD_SIZE];       			//B���ѹAD����BUF       
int16_t      	m_Uc_buf[AD_SIZE];       			//C���ѹAD����BUF    
int16_t      	m_selfU_buf[AD_SIZE];       		//��������ѹ��׼AD����BUF    
int16_t      	m_selfI_buf[AD_SIZE];       		//������������׼AD����BUF    
BOOL 	 		m_read_ad_finish = FAIL; 			//��ʾһ�ܲ������Ƿ����
sum_data_t   	m_sum_data_int;          			//�ж��д�Ÿ����ۼӺ�
sum_data_t   	m_sum_data_ext;                     //�����ⲿ�ĸ����ۼӺ�
ad_64bit_u   	m_ad_64bit;    						//64BIT�������ϣ�Ϊ�˷������������
uint32_t     	m_ad_32bit;                         //64BIT�������ϣ�Ϊ�˷������������ 
ad_32bit_u   	m_DDS_path1_data;                   //32BIT�������ϣ�Ϊ�˷������������ 
ad_32bit_u   	m_DDS_path2_data;                   //32BIT�������ϣ�Ϊ�˷������������ 
DDS_k_t      	m_DDS_PQk;                          //DDS��������ϵ����������Ӧ���е�����ϵ��һ��
path_select_t 	m_DDS_path_mod;  					//DDS����ͨ��ָʾ
extern          DL_flag_t m_measure_flag;
/*-------------------------------------------------------------------------------
DDS ������ر�����Ϊ���Ǹò����ܶ��������ò��ֱ��������г�
-------------------------------------------------------------------------------*/
float       m_phase_fp[3];
float       m_phase_fq[3];
float       m_dds_fdatap;
float 		m_dds_fdataQ;
float       m_dds_sumP;
float       m_dds_sumQ;
float 		m_dds_fsendp;
float 		m_dds_fsendq;
int32_t 	m_temp_32bit;
int32_t   	m_DC_Udx[3];
int32_t		m_DC_Idx[3];
float       m_dds_constBL = 1.0;
/*�����޹��������*/
int32_t      m_32bit_U;  
int32_t      m_32bit_I;  
int32_t      m_32bit_P;  
int32_t      m_32bit_Q;  
int64_t      m_64bit_P;  
int64_t      m_64bit_Q;  
int64_t      m_64bit_UI;  
/*******************************************************************************
*******************************************************************************
 �ж���������ļ����,Ϊ�˼���ϵͳ������û�����ɺ���
********************************************************************************/
#define   	READ_FPGA_AD_DATA(addr,buf)  {buf[m_ad_buf_offset]=BUS_READ(addr);}
/*---------------------------------------------------------------*/
/*���ж��ۻ����ݴ����ⲿ*/
#define measure_INTdata_to_Extdata()\
{\
    m_sum_data_ext.sum_I[A_PHASE] = m_sum_data_int.sum_I[A_PHASE]>>RSHIFT_BIT;\
    m_sum_data_ext.sum_I[B_PHASE] = m_sum_data_int.sum_I[B_PHASE]>>RSHIFT_BIT;\
    m_sum_data_ext.sum_I[C_PHASE] = m_sum_data_int.sum_I[C_PHASE]>>RSHIFT_BIT;\
    m_sum_data_ext.sum_U[A_PHASE] = m_sum_data_int.sum_U[A_PHASE]>>RSHIFT_BIT;\
    m_sum_data_ext.sum_U[B_PHASE] = m_sum_data_int.sum_U[B_PHASE]>>RSHIFT_BIT;\
    m_sum_data_ext.sum_U[C_PHASE] = m_sum_data_int.sum_U[C_PHASE]>>RSHIFT_BIT;\
    m_sum_data_ext.sum_P[A_PHASE] = m_sum_data_int.sum_P[A_PHASE]>>RSHIFT_BIT;\
    m_sum_data_ext.sum_P[B_PHASE] = m_sum_data_int.sum_P[B_PHASE]>>RSHIFT_BIT;\
    m_sum_data_ext.sum_P[C_PHASE] = m_sum_data_int.sum_P[C_PHASE]>>RSHIFT_BIT;\
	m_sum_data_ext.sum_Q[A_PHASE] = m_sum_data_int.sum_Q[A_PHASE]>>RSHIFT_BIT;\
    m_sum_data_ext.sum_Q[B_PHASE] = m_sum_data_int.sum_Q[B_PHASE]>>RSHIFT_BIT;\
    m_sum_data_ext.sum_Q[C_PHASE] = m_sum_data_int.sum_Q[C_PHASE]>>RSHIFT_BIT;\
}
/*---------------------------------------------------------------*/
/*���ж��ۻ�����BUF���*/
#define measure_INTdata_clean()\
{\
    m_sum_data_int.sum_I[A_PHASE] = 0;\
    m_sum_data_int.sum_I[B_PHASE] = 0;\
    m_sum_data_int.sum_I[C_PHASE] = 0;\
    m_sum_data_int.sum_U[A_PHASE] = 0;\
    m_sum_data_int.sum_U[B_PHASE] = 0;\
    m_sum_data_int.sum_U[C_PHASE] = 0;\
    m_sum_data_int.sum_P[A_PHASE] = 0;\
    m_sum_data_int.sum_P[B_PHASE] = 0;\
    m_sum_data_int.sum_P[C_PHASE] = 0;\
    m_sum_data_int.sum_Q[A_PHASE] = 0;\
    m_sum_data_int.sum_Q[B_PHASE] = 0;\
    m_sum_data_int.sum_Q[C_PHASE] = 0;\
}
#define  GET_SHIFT_90(offset)(((offset>255))?(offset-256):(offset+1024-256))
/*---------------------------------------------------------------*/
/*�����޹�*/
#define FQ_GET(phase)\
{\
	m_32bit_P = (int32_t)(m_sum_data_int.sum_P[phase]>>RSHIFT_BIT);\
	m_64bit_P = (int64_t)m_32bit_P*m_32bit_P;\
	m_32bit_U = (int32_t)(m_sum_data_int.sum_U[phase]>>RSHIFT_BIT);\
	m_32bit_I = (int32_t)(m_sum_data_int.sum_I[phase]>>RSHIFT_BIT);\
	m_64bit_UI= (int64_t)m_32bit_I*m_32bit_U;\
	m_64bit_Q = m_64bit_UI-m_64bit_P;\
	m_64bit_Q = GLOBAL_ABS(m_64bit_Q);\
	m_32bit_Q = measure_sqrt_64bit_data((uint64_t)m_64bit_Q);}

#define FQ_GET_1(phase){m_32bit_Q =(int32_t)(m_sum_data_int.sum_Q[phase]>>RSHIFT_BIT);}

/*����DDS*/
#define DDS_COUNT(phase)\
	{\
		m_temp_32bit = (int32_t)(m_sum_data_int.sum_P[phase]>>RSHIFT_BIT);\
		m_dds_fdatap = (float)m_temp_32bit*m_err_correct_data.k_pq[phase];\
		m_dds_fdatap = m_DDS_PQk.DDS_krpq[phase]*m_dds_fdatap;\
		m_dds_fdatap = m_dds_fdatap+m_DDS_PQk.DDS_Iline[phase];\
        FQ_GET_1(phase);\
		m_dds_fdataQ = (float)m_32bit_Q*m_err_correct_data.k_pq[phase];\
		m_dds_fdataQ = m_DDS_PQk.DDS_krpq[phase]*m_dds_fdataQ;\
		m_dds_fdataQ = m_dds_fdataQ+m_DDS_PQk.DDS_Iline[phase];\
		m_dds_sumP   = m_dds_fdatap*m_DDS_PQk.DDS_kcos[phase]+m_dds_fdataQ*m_DDS_PQk.DDS_ksin[phase];\
		m_dds_sumQ   = m_dds_fdataQ*m_DDS_PQk.DDS_kcos[phase]-m_dds_fdatap*m_DDS_PQk.DDS_ksin[phase];\
        m_phase_fp[phase] = m_dds_sumP;\
		m_phase_fq[phase] = m_dds_sumQ;\
		}
/********************************************************************
 * @������: �ҳ�
 * @����: FPGA����AD���������жϷ������304US��һ���ж�
 * @����:	
 * @���: NONE
 ���жϳ���ʵ����AD�������ݵĴ��ݣ�ʵ����6·AD���ݵĶ�ȡ��
 ���Ҷ�ȡ��ؼ�������
 ע��:��Ϊ���жϹ���Ƶ����Ϊ�����ϵͳ���ܣ����ж��в�Ҫʹ��
 ����Ƕ�ף��������CPUʹ���ʹ���
********************************************************************/
#define SUM_Q_BUF(Abuf,Bbuf,sum){sum+=(int64_t)Abuf[m_start_offset]*Bbuf[m_90_offset];}
#define SUM_P_BUF(Abuf,Bbuf,sum){sum+=(int64_t)Abuf[m_start_offset]*Bbuf[m_start_offset];}
#define SUM_UI_BUF(buf,sum){sum+=(int64_t)buf[m_start_offset]*buf[m_start_offset];}
#pragma CODE_SECTION(ISR_read_fpga_data,"ramfuncs") //���öδ���������ramfuncs��
interrupt void ISR_read_fpga_data(void)
{
	m_addr_offset  = 0; 
	m_start_offset = m_ad_buf_offset;
/*------------------------------------------------------------------------*/
/*��ȡAD_PAGE_SIZE�㲨������*/
/*------------------------------------------------------------------------*/
	for(m_int_offset=0;m_int_offset<(AD_PAGE_SIZE);m_int_offset++)
    {	
		BUS_WRITE(m_addr_offset,SET_OFFSET);			//���õ�ǰ������
        READ_FPGA_AD_DATA(IA_ADDR,m_Ia_buf);			//��ȡA�����CODE
		READ_FPGA_AD_DATA(UA_ADDR,m_Ua_buf);			//��ȡA���ѹCODE
        READ_FPGA_AD_DATA(IB_ADDR,m_Ib_buf);			//��ȡB�����CODE
		READ_FPGA_AD_DATA(UB_ADDR,m_Ub_buf);			//��ȡB���ѹCODE
        READ_FPGA_AD_DATA(IC_ADDR,m_Ic_buf);			//��ȡB�����CODE
		READ_FPGA_AD_DATA(UC_ADDR,m_Uc_buf);			//��ȡB���ѹCODE	
		m_addr_offset+=1;
        m_Ia_buf[m_ad_buf_offset]-=m_DC_Idx[A_PHASE];   //����ֱ����
        m_Ua_buf[m_ad_buf_offset]-=m_DC_Udx[A_PHASE];   //����ֱ����
        m_Ib_buf[m_ad_buf_offset]-=m_DC_Idx[B_PHASE];   //����ֱ����
        m_Ub_buf[m_ad_buf_offset]-=m_DC_Udx[B_PHASE];   //����ֱ����
        m_Ic_buf[m_ad_buf_offset]-=m_DC_Idx[C_PHASE];   //����ֱ����
        m_Uc_buf[m_ad_buf_offset]-=m_DC_Udx[C_PHASE];   //����ֱ����	
		m_ad_buf_offset++;
	}	
	for(m_int_offset=0;m_int_offset<(AD_PAGE_SIZE);m_int_offset++)
	{  
		m_90_offset=GET_SHIFT_90(m_start_offset);
		SUM_Q_BUF(m_Ia_buf,m_Ua_buf,m_sum_data_int.sum_Q[A_PHASE]);//����QA 16���ۼӺ�
		SUM_Q_BUF(m_Ib_buf,m_Ub_buf,m_sum_data_int.sum_Q[B_PHASE]);//����QB 16���ۼӺ�
		SUM_Q_BUF(m_Ic_buf,m_Uc_buf,m_sum_data_int.sum_Q[C_PHASE]);//����QC 16���ۼӺ�
		SUM_P_BUF(m_Ia_buf,m_Ua_buf,m_sum_data_int.sum_P[A_PHASE]);//����PA 16���ۼӺ�
		SUM_P_BUF(m_Ib_buf,m_Ub_buf,m_sum_data_int.sum_P[B_PHASE]);//����PB 16���ۼӺ�
		SUM_P_BUF(m_Ic_buf,m_Uc_buf,m_sum_data_int.sum_P[C_PHASE]);//����PC 16���ۼӺ�
		SUM_UI_BUF(m_Ia_buf,m_sum_data_int.sum_I[A_PHASE]);
		SUM_UI_BUF(m_Ua_buf,m_sum_data_int.sum_U[A_PHASE]);
		SUM_UI_BUF(m_Ib_buf,m_sum_data_int.sum_I[B_PHASE]);
		SUM_UI_BUF(m_Ub_buf,m_sum_data_int.sum_U[B_PHASE]);
		SUM_UI_BUF(m_Ic_buf,m_sum_data_int.sum_I[C_PHASE]);
		SUM_UI_BUF(m_Uc_buf,m_sum_data_int.sum_U[C_PHASE]);
		m_start_offset++;
	}
	if(m_ad_buf_offset>(AD_SIZE-1))    //�ж�һ���ܲ��Ƿ����
	{
		m_ad_buf_offset=0;             //����ƫ�Ƶ�
		measure_INTdata_to_Extdata();  //���ۼ����ݴ����ⲿ
		/*------------------------------------------------------------------------
		����DDS�������͵�FPGA
		Ϊ��ʹDDS����������ò��ֵĹ��ʺͲ����еĹ��ʷֱ���㣬
		��������һ���ܲ���һ��DDS���ݻ��ǰ���ܲ���һ�Ρ�
		------------------------------------------------------------------------*/
		DDS_COUNT(A_PHASE);                 
		m_dds_fsendp = m_dds_sumP;         
		m_dds_fsendq = m_dds_sumQ;
		if(m_measure_flag.phase3or4==JXMOD_3_4)
        {      
    		DDS_COUNT(B_PHASE);
    		m_dds_fsendp+= m_dds_sumP;
    		m_dds_fsendq+= m_dds_sumQ;
        }
        else
        {
           m_phase_fp[B_PHASE] = 0;
           m_phase_fq[B_PHASE] = 0;
        }
		DDS_COUNT(C_PHASE);
		m_dds_fsendp+= m_dds_sumP;
		m_dds_fsendq+= m_dds_sumQ;
		m_dds_fsendp = GLOBAL_ABS(m_dds_fsendp);
		m_dds_fsendq = GLOBAL_ABS(m_dds_fsendq);
		m_dds_fsendp = m_dds_fsendp*DDS_CONST*m_dds_constBL;
		m_dds_fsendq = m_dds_fsendq*DDS_CONST*m_dds_constBL;
		m_dds_fsendp = m_dds_fsendp*m_err_Sdata.f_path_BB[0]+0.5;
		m_dds_fsendq = m_dds_fsendq*m_err_Sdata.f_path_BB[0]+0.5;
        if(m_DDS_path_mod.P_or_Q==0)
        {
			m_DDS_path1_data.all=(uint32_t)(m_dds_fsendp);
			m_DDS_path2_data.all=(uint32_t)(m_dds_fsendp);
        }
        else if(m_DDS_path_mod.P_or_Q==1)
        {
			m_DDS_path1_data.all=(uint32_t)(m_dds_fsendq);
			m_DDS_path2_data.all=(uint32_t)(m_dds_fsendq);
        }
		else
		{
			m_DDS_path1_data.all=(uint32_t)(m_dds_fsendp);
			m_DDS_path2_data.all=(uint32_t)(m_dds_fsendq);
		}
		BUS_WRITE(m_DDS_path1_data.bytes_2.byte_0,DDS_BASE_PATH1_UNIT_SET);
		BUS_WRITE(m_DDS_path1_data.bytes_2.byte_1,DDS_BASE_PATH1_UNIT_SET+1);	
	    BUS_WRITE(m_DDS_path2_data.bytes_2.byte_0,DDS_BASE_PATH2_UNIT_SET);
		BUS_WRITE(m_DDS_path2_data.bytes_2.byte_1,DDS_BASE_PATH2_UNIT_SET+1);
		/*------------------------------------------------------------------------*/
		/*���DDS����*/
		/*------------------------------------------------------------------------*/
		measure_INTdata_clean();
		m_read_ad_finish = TURE;
	}
	PieCtrlRegs.PIEACK.all = 0xFFFF;
	return;
}
#if 0
void _PQ_count(int phase)
{
	m_temp_32bit = (int32_t)(m_sum_data_int.sum_P[phase]>>RSHIFT_BIT);
	m_dds_fdatap = (float)m_temp_32bit*m_err_correct_data.k_pq[phase];
	m_dds_fdatap = m_DDS_PQk.DDS_krpq[phase]*m_dds_fdatap;
	m_dds_fdatap = m_dds_fdatap+m_DDS_PQk.DDS_Iline[phase];
	m_32bit_Q    = (int32_t)(m_sum_data_int.sum_Q[phase]>>RSHIFT_BIT);
	m_dds_fdataQ = (float)m_32bit_Q*m_err_correct_data.k_pq[phase];
	m_dds_fdataQ = m_DDS_PQk.DDS_krpq[phase]*m_dds_fdataQ;
	m_dds_fdataQ = m_dds_fdataQ+m_DDS_PQk.DDS_Iline[phase];
	m_dds_sumP   = m_dds_fdatap*m_DDS_PQk.DDS_kcos[phase]+m_dds_fdataQ*m_DDS_PQk.DDS_ksin[phase];
	m_dds_sumQ   = m_dds_fdataQ*m_DDS_PQk.DDS_kcos[phase]-m_dds_fdatap*m_DDS_PQk.DDS_ksin[phase];
    m_phase_fp[phase] = m_dds_sumP;
	m_phase_fq[phase] = m_dds_sumQ;
	return;
}
/******************************************************************/
//TEST
/******************************************************************/
void measure_taskTEST(void*arg)
{ 
    int _90offset,pos;
	int64_t p;
	msleep(100);
	fft_creat(m_Ua_buf,1024,30000,0);
	fft_creat(m_Ia_buf,1024,15000,0);
	fft_creat(m_Ub_buf,1024,30000,120);
	fft_creat(m_Ib_buf,1024,15000,120);
	fft_creat(m_Uc_buf,1024,30000,240);
	fft_creat(m_Ic_buf,1024,15000,240);	
	fft_add(m_Ua_buf,1024,15000,1,0,3);
	fft_add(m_Ia_buf,1024,15000,2,0,3);
	fft_add(m_Ub_buf,1024,15000,3,0,3);
	fft_add(m_Ib_buf,1024,15000,4,0,3);
	fft_add(m_Uc_buf,1024,15000,5,0,3);
	fft_add(m_Ic_buf,1024,15000,6,0,3);
	GLOBAL_MEMCLR(&m_sum_data_int,sizeof(sum_data_t));
	GLOBAL_MEMCLR(&m_sum_data_ext,sizeof(sum_data_t));
    GLOBAL_MEMCLR(m_DC_Udx,3*sizeof(int32_t));
    GLOBAL_MEMCLR(m_DC_Idx,3*sizeof(int32_t));
	for(pos=0;pos<1024;pos++)
	{  
		_90offset=(((pos>255))?(pos-256):(pos+1024-256));
        m_sum_data_int.sum_I[0] += (int64_t)m_Ia_buf[pos]*m_Ia_buf[pos];
        m_sum_data_int.sum_U[0] += (int64_t)m_Ua_buf[pos]*m_Ua_buf[pos];
        m_sum_data_int.sum_P[0] += (int64_t)m_Ua_buf[pos]*m_Ia_buf[pos];
        m_sum_data_int.sum_Q[0] += (int64_t)m_Ia_buf[pos]*m_Ua_buf[_90offset];
        m_sum_data_int.sum_I[1] += (int64_t)m_Ib_buf[pos]*m_Ib_buf[pos];
        m_sum_data_int.sum_U[1] += (int64_t)m_Ub_buf[pos]*m_Ub_buf[pos];
        m_sum_data_int.sum_P[1] += (int64_t)m_Ub_buf[pos]*m_Ib_buf[pos];
        m_sum_data_int.sum_Q[1] += (int64_t)m_Ib_buf[pos]*m_Ub_buf[_90offset];
        m_sum_data_int.sum_I[2] += (int64_t)m_Ic_buf[pos]*m_Ic_buf[pos];
        m_sum_data_int.sum_U[2] += (int64_t)m_Uc_buf[pos]*m_Uc_buf[pos];
        m_sum_data_int.sum_P[2] += (int64_t)m_Uc_buf[pos]*m_Ic_buf[pos];
        m_sum_data_int.sum_Q[2] += (int64_t)m_Ic_buf[pos]*m_Uc_buf[_90offset];
	}
	m_sum_data_ext.sum_I[A_PHASE] = m_sum_data_int.sum_I[A_PHASE]>>10;
    m_sum_data_ext.sum_I[B_PHASE] = m_sum_data_int.sum_I[B_PHASE]>>10;
    m_sum_data_ext.sum_I[C_PHASE] = m_sum_data_int.sum_I[C_PHASE]>>10;
    m_sum_data_ext.sum_U[A_PHASE] = m_sum_data_int.sum_U[A_PHASE]>>10;
    m_sum_data_ext.sum_U[B_PHASE] = m_sum_data_int.sum_U[B_PHASE]>>10;
    m_sum_data_ext.sum_U[C_PHASE] = m_sum_data_int.sum_U[C_PHASE]>>10;
    m_sum_data_ext.sum_P[A_PHASE] = m_sum_data_int.sum_P[A_PHASE]>>10;
    m_sum_data_ext.sum_P[B_PHASE] = m_sum_data_int.sum_P[B_PHASE]>>10;
    m_sum_data_ext.sum_P[C_PHASE] = m_sum_data_int.sum_P[C_PHASE]>>10;
	m_sum_data_ext.sum_Q[A_PHASE] = m_sum_data_int.sum_Q[A_PHASE]>>10;
    m_sum_data_ext.sum_Q[B_PHASE] = m_sum_data_int.sum_Q[B_PHASE]>>10;
    m_sum_data_ext.sum_Q[C_PHASE] = m_sum_data_int.sum_Q[C_PHASE]>>10;	
	measure_correct_K_renew(&m_correct_level_arg);
    while(1)
	{
	    msleep(3);
		_PQ_count(A_PHASE);
		_PQ_count(B_PHASE);
		_PQ_count(C_PHASE);
		m_read_ad_finish = 1;
	}
}

void measue_GetADdata_stop(void)
{
}
void measue_GetADdata_start(void)
{
}

void measure_GetADdata_init(void)
{
	os_task_assign_stk_build(measure_taskTEST,3);
}
#else
void measure_GetADdata_ISR(void)
{
	EALLOW;
	GpioCtrlRegs.GPAMUX1.bit.GPIO1  = 0;
	GpioCtrlRegs.GPAMUX1.bit.GPIO2  = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO1   = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO2   = 0;
	GpioCtrlRegs.GPAQSEL1.bit.GPIO1 = 0;
	GpioCtrlRegs.GPAQSEL1.bit.GPIO2 = 0;
	GpioCtrlRegs.GPBCTRL.bit.QUALPRD0 = 0xFF;
	GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 2;
	GpioIntRegs.GPIOXINT2SEL.bit.GPIOSEL = 1;
	XIntruptRegs.XINT1CR.bit.POLARITY = 0;
	XIntruptRegs.XINT2CR.bit.POLARITY = 0;
	XIntruptRegs.XINT1CR.bit.ENABLE = 1;
	XIntruptRegs.XINT2CR.bit.ENABLE = 1;
	EDIS;
	EALLOW;  
   	PieVectTable.XINT1 = &ISR_read_fpga_data;
   	EDIS;   
   	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
	PieCtrlRegs.PIEIER1.bit.INTx4 = 0;          //28335PDF��51ҳ ad�ж� 28335PDF��51ҳ �ⲿ��չ�ж�
   	IER |= M_INT1;                          
    EINT; 
    ERTM;   
    GLOBAL_MEMCLR(m_DC_Udx,3*sizeof(int32_t));
    GLOBAL_MEMCLR(m_DC_Idx,3*sizeof(int32_t));
	return;
}
void measue_DDSconst_mod(uint32_t Iconst)
{
   	m_dds_constBL  = (float)Iconst/36000.0;
	return;
}
void measue_GetADdata_stop(void)
{
	PieCtrlRegs.PIEIER1.bit.INTx4 = 0;
}
void measue_GetADdata_start(void)
{
	PieCtrlRegs.PIEIER1.bit.INTx4 = 1;
}
void measure_GetADdata_init(void)
{
   measure_GetADdata_ISR();
   measue_DDSconst_mod(36000);
   return;
}
#endif

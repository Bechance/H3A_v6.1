/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:ϵͳ�������������
* ����ʱ��:2012-03-20
* ������˾:����
* �ļ�����:measure_error.c
* ������:�ҳ�
* ��������:������

˵����FPGA���ṩ2��ͨ����DDS���壬ͨ���Ǽ����ĸ����ȡ���ڣ�
�ж��͵�DDS��Ƶ���������ĸ����ʣ�ע��AD�ж��͵�DDS��Ƶ��һ��Ҫ��
ͨ������ĳ���Ȧ����Ӧ���������ܼ������������
����������ͨ��1��Ϊ���ͷ�й���ȡ�����壬��ô�����͵�ͨ��1DDS��Ƶ��Ӧ����
���й�Ϊ��׼��
***************************************************************/
#include	"measure.h"
#pragma  		DATA_SECTION (m_err_Sdata,"FAST_RAM")/*�ѱ���������FAST_RAM*/
#define         PLUS_CONST    						360000000 
/*����ȡ��ʽ  m_err_way =  
			    0    �ӹ��ͷ��ȡ���㴫ͳ��ʽ
				1    �ֶ�������� ������������
				2    �ۼ���ʽ����������
*/
#define          FILT_BUF_LEN    5     
int              m_err_way   	 = 0;
err_count_t      m_err_Sdata; 
filtering_data_t m_errfilt[3];
float            m_errfilt_buf[3][FILT_BUF_LEN];
//��ͳ�����ر���
float		     m_meter_value[3];						   /*��ʵ��ֵ*/
float		     m_PEC_value[3];							/*�����ֵ*/
float            m_ONE_err[3];
float		     m_path_count_err_value[3];
//�ۼ������ر���
float            m_meter_SUM[3];
float            m_PEC_SUM[3];
float            m_PEC_CNT[3];
float            m_SUM_err[3];
//���������ر���
BOOL             m_ZSSerrNEW = 0;
float            m_ZSSerr    = 0.0;
/**********************************************************/
//�����������
/**********************************************************/
static int _ZSSERR_handle(void)
{
    if(m_ZSSerrNEW)
	{
	    m_ZSSerrNEW = 0;
		m_err_Sdata.f_path2_err[3] = m_err_Sdata.f_path2_err[2];
		m_err_Sdata.f_path2_err[2] = m_err_Sdata.f_path2_err[1];
		m_err_Sdata.f_path2_err[1] = m_err_Sdata.f_path2_err[0];
		m_err_Sdata.f_path2_err[0] = m_ZSSerr;
		return 1;
    }
	return 0;
}

/**********************************************************/
//��ͳ������
/**********************************************************/
/********************************************************************
 * @������:   �ҳ�
 * @����:   ͨ���������
 * @����:	NONE
 * @���:	NONE
********************************************************************/
static void _count_energy(Bsize_t path)
{
	m_meter_value[path]= (36000000.0/ (float)m_err_Sdata.path_cs[path]) * ((float)m_err_Sdata.path_qs[path]);
	m_PEC_value[path]  = (float)m_err_Sdata.path_dds_cnt[path]/(10.0*m_dds_constBL);
	return;
}

/********************************************************************
 * @������: �ҳ�
 * @����: ����
 * @����:	NONE
 * @���:	NONE
********************************************************************/
static void _GetERR(Bsize_t path)
{
	float  err;
  	float  temp;
	temp = temp;
	if ( 0.00 != m_PEC_value[path])
	{
		err = ( m_meter_value[path] - m_PEC_value[path] ) / m_PEC_value[path] * 100;
		if(GLOBAL_ABS(err)>400) err = 400; 
    	m_ONE_err[path] = shift_filtering_data_obtain(&m_errfilt[path],err);
		if(m_ONE_err[path]>=0&&m_ONE_err[path]<0.001)
		{
			m_ONE_err[path]=0.001;
		}
		if(m_ONE_err[path]<0&&m_ONE_err[path]>-0.001)
		{
			m_ONE_err[path]=-0.001;
		}
	}
	return;
}
/********************************************************************
 * @������:�ҳ�
 * @����:����������ӿ�
 * @����:NONE
 * @���:0 û�����   1��ͨ��1���  2��ͨ��2��� 3��ͨ��3���
********************************************************************/
static int _ERR_handle(void)
{
    Bsize_t path;
	if ( m_err_Sdata.path_refresh[0])     //ͨ��һ
	{
	    path = 0;
		m_err_Sdata.path_refresh[path] = FAIL;
		if ( m_err_Sdata.path_cs[path] == 0 || m_err_Sdata.path_qs[path]== 0 )
		return 0;
		_count_energy(path);
		_GetERR(path);
		m_err_Sdata.f_path1_err[3] = m_err_Sdata.f_path1_err[2];
		m_err_Sdata.f_path1_err[2] = m_err_Sdata.f_path1_err[1];
		m_err_Sdata.f_path1_err[1] = m_err_Sdata.f_path1_err[0];
		m_err_Sdata.f_path1_err[0] = m_ONE_err[path];
		m_path_count_err_value[path] = m_err_Sdata.path_dds_cnt[path];
		return 1;
	}
	else if ( m_err_Sdata.path_refresh[1]) //ͨ����
	{
	    path = 1;
		m_err_Sdata.path_refresh[path] = FAIL;
		if ( m_err_Sdata.path_cs[path] == 0 || m_err_Sdata.path_qs[path]== 0 )
		return 0;
		_count_energy(path);
		_GetERR(path);
		m_err_Sdata.f_path2_err[3] = m_err_Sdata.f_path2_err[2];
		m_err_Sdata.f_path2_err[2] = m_err_Sdata.f_path2_err[1];
		m_err_Sdata.f_path2_err[1] = m_err_Sdata.f_path2_err[0];
		m_err_Sdata.f_path2_err[0] = m_ONE_err[path];
		m_path_count_err_value[path] = m_err_Sdata.path_dds_cnt[path];
		return 2;
	}
	else if ( m_err_Sdata.path_refresh[2]) //ͨ����
	{
	    path = 2;
		m_err_Sdata.path_refresh[path] = FAIL;
		if ( m_err_Sdata.path_cs[path] == 0 || m_err_Sdata.path_qs[path]== 0 )
		return 0;
		_count_energy(path);
		_GetERR(path);
		m_err_Sdata.f_path3_err[3] = m_err_Sdata.f_path3_err[2];
		m_err_Sdata.f_path3_err[2] = m_err_Sdata.f_path3_err[1];
		m_err_Sdata.f_path3_err[1] = m_err_Sdata.f_path3_err[0];
		m_err_Sdata.f_path3_err[0] = m_ONE_err[path];
		m_path_count_err_value[path] = m_err_Sdata.path_dds_cnt[path];
		return 3;
	}
	return 0;
}
/**********************************************************/
//�ۼ�������
/**********************************************************/
static void _count_SUMenergy(Bsize_t path)
{
	m_meter_SUM[path] +=((1000.0*m_dds_constBL)/(float)m_err_Sdata.path_cs[path])*((float)m_err_Sdata.path_qs[path]);
    m_PEC_CNT[path]   += m_err_Sdata.path_dds_cnt[path];
	m_PEC_SUM[path]    = 1.0/360000*m_PEC_CNT[path];
    return;
}

static void _SUMERR_lawful(Bsize_t path)
{
    float dx;
    dx = m_SUM_err[path]-m_ONE_err[path];
	dx = GLOBAL_ABS(dx);
    if(dx>0.1)
    {
       GLOBAL_MEMCLR(m_meter_SUM,sizeof(float)*3);
       GLOBAL_MEMCLR(m_PEC_CNT,sizeof(float)*3);
       GLOBAL_MEMCLR(m_PEC_SUM,sizeof(float)*3);
	   m_SUM_err[path]= m_ONE_err[path];
	}
    return;
}

static void _Get_SUMERR(Bsize_t path)
{
	float  err;
  	float  temp;
	temp = temp;
	if ( 0.00 != m_PEC_SUM[path])
	{
		err = ( m_meter_SUM[path] - m_PEC_SUM[path] ) / m_PEC_SUM[path] * 100;
		if(GLOBAL_ABS(err)>100) err =100; 
		m_SUM_err[path] = err;
		if(m_SUM_err[path]>0&&m_SUM_err[path]<0.001)
		{
			m_SUM_err[path]=0.001;
		}
		if(m_SUM_err[path]<0&&m_SUM_err[path]>-0.001)
		{
			m_SUM_err[path]=-0.001;
		}
	}
	_SUMERR_lawful(path);
	return;
}
/********************************************************************
 * @������:�ҳ�
 * @����:����������ӿ�
 * @����:NONE
 * @���:0 û�����   1��ͨ��1���  2��ͨ��2��� 3��ͨ��3���
********************************************************************/
static int _SUMERR_handle(void)
{
    Bsize_t path;
	if ( m_err_Sdata.path_refresh[0])     //ͨ��һ
	{
	    path = 0;
		m_err_Sdata.path_refresh[path] = FAIL;
		if ( m_err_Sdata.path_cs[path] == 0 || m_err_Sdata.path_qs[path]== 0 )
		return 0;
		_count_energy(path);
		_GetERR(path);
		_count_SUMenergy(path);
		_Get_SUMERR(path);
		m_err_Sdata.f_path1_err[3] = m_err_Sdata.f_path1_err[2];
		m_err_Sdata.f_path1_err[2] = m_err_Sdata.f_path1_err[1];
		m_err_Sdata.f_path1_err[1] = m_err_Sdata.f_path1_err[0];
		m_err_Sdata.f_path1_err[0] = m_SUM_err[path];
		m_path_count_err_value[path] = m_err_Sdata.path_dds_cnt[path];
		return 1;
	}
	else if ( m_err_Sdata.path_refresh[1]) //ͨ����
	{
	    path = 1;
		m_err_Sdata.path_refresh[path] = FAIL;
		if ( m_err_Sdata.path_cs[path] == 0 || m_err_Sdata.path_qs[path]== 0 )
		return 0;
		_count_energy(path);
		_GetERR(path);
		_count_SUMenergy(path);
		_Get_SUMERR(path);
		m_err_Sdata.f_path2_err[3] = m_err_Sdata.f_path2_err[2];
		m_err_Sdata.f_path2_err[2] = m_err_Sdata.f_path2_err[1];
		m_err_Sdata.f_path2_err[1] = m_err_Sdata.f_path2_err[0];
		m_err_Sdata.f_path2_err[0] = m_SUM_err[path];
		m_path_count_err_value[path] = m_err_Sdata.path_dds_cnt[path];
		return 2;
	}
	else if ( m_err_Sdata.path_refresh[2]) //ͨ����
	{
	    path = 2;
		m_err_Sdata.path_refresh[path] = FAIL;
		if ( m_err_Sdata.path_cs[path] == 0 || m_err_Sdata.path_qs[path]== 0 )
		return 0;
		_count_energy(path);
		_GetERR(path);
		_count_SUMenergy(path);
		_Get_SUMERR(path);
		m_err_Sdata.f_path3_err[3] = m_err_Sdata.f_path3_err[2];
		m_err_Sdata.f_path3_err[2] = m_err_Sdata.f_path3_err[1];
		m_err_Sdata.f_path3_err[1] = m_err_Sdata.f_path3_err[0];
		m_err_Sdata.f_path3_err[0] = m_SUM_err[path];
		m_path_count_err_value[path] = m_err_Sdata.path_dds_cnt[path];
		return 3;
	}
	return 0;
}
/*****************************************************/
//API
/*****************************************************/
void measure_error_AA(measure_param_t *pDL)
{
	int     rerr;
	if(m_err_way==0)
	{
		rerr=_ERR_handle();
	}
	else if(m_err_way==1)
	{
		rerr=_SUMERR_handle();
	}
	else if(m_err_way==2)
	{      
        rerr = _ZSSERR_handle();
	}
	if(rerr==1)
	{ 
		global_memcpy(pDL->f_path1_err,m_err_Sdata.f_path1_err,sizeof(float)*4);
		m_measure_flag.DL_path1_ERR_is_new=TURE;
	}
    else if(rerr==2)
	{ 
		global_memcpy(pDL->f_path2_err,m_err_Sdata.f_path2_err,sizeof(float)*4);
		m_measure_flag.DL_path2_ERR_is_new=TURE;
	}
    return;
}

void  measure_driver_ZSSERR_load(float err)
{
     m_ZSSerr = err;
	 m_ZSSerrNEW =1;
	 return;
}


void  measure_driver_ERR_way(int way)
{
	m_err_way = way;
	GLOBAL_MEMCLR(m_meter_SUM,sizeof(float)*3);
	GLOBAL_MEMCLR(m_PEC_CNT,sizeof(float)*3);
	GLOBAL_MEMCLR(m_PEC_SUM,sizeof(float)*3);
	return;
}

/*�жϵ���*/
void measure_error_path1_dx_counter_obtain(void)
{
	m_err_Sdata.path_dds_cnt[0] = READ32BIT_DATA(DDS_READ_PATH1_SUMFL);
	m_err_Sdata.path_refresh[0] = TURE;
	return;
}
/*�жϵ���*/
void measure_error_path2_dx_counter_obtain(void)
{
	m_err_Sdata.path_dds_cnt[1] = READ32BIT_DATA(DDS_READ_PATH2_SUMFL);
	m_err_Sdata.path_refresh[1] = TURE;
	return;
}
/*�жϵ���*/
void measure_error_path3_dx_counter_obtain(void)
{
	m_err_Sdata.path_dds_cnt[2] = READ32BIT_DATA(DDS_READ_PATH3_SUMFL);
	m_err_Sdata.path_refresh[2] = TURE;
	return;
}

void measure_filter_init(void)
{
	shift_filtering_data_algorithm_creat(m_errfilt_buf[0],FILT_BUF_LEN,0,0.05,0,&m_errfilt[0]);
	shift_filtering_data_algorithm_creat(m_errfilt_buf[1],FILT_BUF_LEN,0,0.05,0,&m_errfilt[1]);
	shift_filtering_data_algorithm_creat(m_errfilt_buf[2],FILT_BUF_LEN,0,0.05,0,&m_errfilt[2]);
	return;
}

void measure_error_count_init(void)
{
	GLOBAL_MEMCLR(&m_err_Sdata,sizeof(err_count_t));
	m_err_Sdata.f_path_BB[0] = 1;
	m_err_Sdata.f_path_BB[1] = 1;
	m_err_Sdata.f_path_BB[2] = 1;
	m_err_way                = 0;
	GLOBAL_MEMCLR(m_meter_SUM,sizeof(float)*3);
	GLOBAL_MEMCLR(m_PEC_CNT,sizeof(float)*3);
	GLOBAL_MEMCLR(m_PEC_SUM,sizeof(float)*3);
	measure_filter_init();
    return;
}

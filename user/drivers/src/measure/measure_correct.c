/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:ϵͳ�������������
* ����ʱ��:2011-12-20
* ������˾:����
* �ļ�����:measure_correct.c
* ������:�ҳ�
* ��������: ����ģ����������
***************************************************************/
#include	"measure.h"
/***************************************************************/
/******************************************************************
     AD7608�ٷ��ĵ�ADת����ϵ:CODE=(Vin/5v)X(131072)X(REF/2.5);
     ==> CODE=(Vin)X(131072)/(5)==>CODE=Vin*Kconst;
         Kconst = (131072/5);
					 	
		������           ��Ч���ϵ��Kr                     ADת����ϵ
ʵ�ʵ�ѹ----->δ������ѹ---------------------->AD������ѹ------------------>CODE
		            	(�����ѹ�����������)

��ʽ���:
        CODE=U*Kerr*Kr*Kconst
        <==>U=CODE/(Kerr*Kr*Kconst)
        <==>U=CODE*K;
        ==>K=1/(Kerr*Kr*Kconst);
******************************************************************/
#define    KUI_CONST          		((float)10.0/0x10000)   
#define    KQ_CONST           		1

#define     CTDZ_1A_RATIO           1000.0    	//1A����CT���
#define     CTDZ_10A_RATIO          1000.0    	//10A����CT���
#define		CTQB_1A_RATIO			1000.0		//1Aǯ��CT�ı��Ϊ1000
#define		CTQB_5A_RATIO			1000.0		//5Aǯ��CT�ı��Ϊ1000
#define		CTQB_20A_RATIO			4000.0		//20Aǯ��CT�ı��Ϊ4000
#define		CTQB_100A_RATIO			20000.0		//100Aǯ���CT���Ϊ1000*������20��
#define		CTQB_500A_RATIO			100000.0	//500Aǯ���CT���Ϊ2000*������50��
#define		CTQB_1000A_RATIO		200000.0	//1000Aǯ���CT���Ϊ4000*������50��
#define		CTQB_1500A_RATIO		300000.0	//1500Aǯ���CT���Ϊ6000*������50��

const      float m_U_level_Rconst[MAX_U_LEVEL_N]={0.007042,0.014084,0.028168,0.056336};/*��ѹ����λ��Ч�����б�*/
const      float m_I_level_Rconst[MAX_I_LEVEL_N]={300,3000};/*��������λ��Ч�����б�*/
const      float m_DLSR_CT_const[MAX_QB_N]=
{
	CTDZ_1A_RATIO,CTDZ_10A_RATIO,CTQB_1A_RATIO,
	CTQB_5A_RATIO,CTQB_20A_RATIO,
	CTQB_100A_RATIO,CTQB_500A_RATIO,
	CTQB_1000A_RATIO,CTQB_1500A_RATIO,
};/*��������ǯ����ֵ*/

/***************************************************************/
#pragma  	DATA_SECTION (m_err_correct_data,"FAST_RAM")/*�ѱ���������FAST_RAM*/
K_UIR_t   				m_sUI_kr;                 /*��Ч����ṹ*/
correct_data_t   		m_err_correct_data;       /*�������ݽṹ*/
correct_save_data_t     m_save_correct_data;      /*�������ݴ洢�ṹ*/
float                   m_kconst = KUI_CONST;     /*AD����*/
/***************************************************************/
/*�洢У������*/
void measure_correct_disk_init(BOOL is_save_to_disk);
BOOL fatfs_file_ISexist(const char*path);
BOOL fatfs_readARG(const char*path,void*data,int size);
BOOL fatfs_writeARG(const char*path,void*data,int size);
#define    CORRECT_NAME1         "corect1"
#define    CORRECT_NAME2         "corect2"
#define    CORRECT_NAME3         "corect3"
void measure_correct_write_to_SD(char*name,correct_save_data_t*pdata)
{ 
    int cnt = 0;
	BOOL state;
	while(++cnt<5)
	{
	    state=fatfs_writeARG(name,pdata,sizeof(correct_save_data_t));
		if(state == 1)break;
		msleep(APP_100_MS);
	}
	return;
}

BOOL measure_correct_check_SD(void)
{
    BOOL r1,r2,r3;
    r1 = fatfs_file_ISexist(CORRECT_NAME1);
    r2 = fatfs_file_ISexist(CORRECT_NAME2);
    r3 = fatfs_file_ISexist(CORRECT_NAME3);
    if(r1==FAIL&&r2==FAIL&&r3==FAIL)
    {
		measure_correct_disk_init(FAIL);
		return FAIL;
    }
	return TURE;
}
/*************************************************************/
#define    correct_save_sector1(src)  measure_correct_write_to_SD(CORRECT_NAME1,src);
#define    correct_save_sector2(src)  measure_correct_write_to_SD(CORRECT_NAME2,src);
#define    correct_save_sector3(src)  measure_correct_write_to_SD(CORRECT_NAME3,src);
#define    correct_read_sector1(src)  fatfs_readARG(CORRECT_NAME1,src,sizeof(correct_save_data_t));
#define    correct_read_sector2(src)  fatfs_readARG(CORRECT_NAME2,src,sizeof(correct_save_data_t));
#define    correct_read_sector3(src)  fatfs_readARG(CORRECT_NAME3,src,sizeof(correct_save_data_t));
/********************************************************************
 * @������: �ҳ�
 * @����: ��������У��ͼ���
 * @����:	
 * @���: NONE
********************************************************************/
static uint32_t measure_correct_crc_obtain(void*head)
{
    uint32_t crc=0;
    Bsize_t offset;
	uint16_t*pb =(uint16_t*)head;
	for(offset=0;offset<sizeof(correct_save_data_t)-sizeof(uint32_t)-1;offset++)
	{
         crc+=(*pb);
		 pb++;
	}
	return crc;
}
/********************************************************************
 * @������: �ҳ�
 * @����  : �������ݴ洢
 * @����  :	
 * @���  : NONE
********************************************************************/
void measure_correct_data_save(
		correct_save_data_t*pdata, /*ԭʼ�������ݳ�*/  
		correct_level_arg*parg,    /*��ǰ��λ�����ʶ*/
		correct_data_t*pcorrect    /*��ǰ��������*/
)
{
      uint32_t crc;
	  /*����*/
      pdata->k_Imain_A[parg->ADLSR_select][parg->Ualevel_offset]=pcorrect->k_Imain[A_PHASE];
      pdata->k_Imain_B[parg->BDLSR_select][parg->Ublevel_offset]=pcorrect->k_Imain[B_PHASE];
      pdata->k_Imain_C[parg->CDLSR_select][parg->Uclevel_offset]=pcorrect->k_Imain[C_PHASE];
	  pdata->k_Umain_A[parg->Ualevel_offset]=pcorrect->k_Umain[A_PHASE];
	  pdata->k_Umain_B[parg->Ublevel_offset]=pcorrect->k_Umain[B_PHASE];
	  pdata->k_Umain_C[parg->Uclevel_offset]=pcorrect->k_Umain[C_PHASE];
	  /*����*/
      pdata->k_Iline_A[parg->ADLSR_select][parg->Ualevel_offset]=pcorrect->k_Iline[A_PHASE];
      pdata->k_Iline_B[parg->BDLSR_select][parg->Ublevel_offset]=pcorrect->k_Iline[B_PHASE];
      pdata->k_Iline_C[parg->CDLSR_select][parg->Uclevel_offset]=pcorrect->k_Iline[C_PHASE];
	  /*�Ƕ�*/
      pdata->k_dxangle_A[parg->ADLSR_select][parg->Ualevel_offset]=pcorrect->k_dxangle[A_PHASE];
      pdata->k_dxangle_B[parg->BDLSR_select][parg->Ublevel_offset]=pcorrect->k_dxangle[B_PHASE];
      pdata->k_dxangle_C[parg->CDLSR_select][parg->Uclevel_offset]=pcorrect->k_dxangle[C_PHASE];
	  /*����*/
      pdata->k_Jline_A[parg->ADLSR_select][parg->Ualevel_offset]=pcorrect->k_Jline[A_PHASE];
      pdata->k_Jline_B[parg->BDLSR_select][parg->Ublevel_offset]=pcorrect->k_Jline[B_PHASE];
      pdata->k_Jline_C[parg->CDLSR_select][parg->Uclevel_offset]=pcorrect->k_Jline[C_PHASE];
	  /*����*/
      pdata->kC_Jline_A[parg->ADLSR_select][parg->Ualevel_offset]=pcorrect->kC_Jline[A_PHASE];
      pdata->kC_Jline_B[parg->BDLSR_select][parg->Ublevel_offset]=pcorrect->kC_Jline[B_PHASE];
      pdata->kC_Jline_C[parg->CDLSR_select][parg->Uclevel_offset]=pcorrect->kC_Jline[C_PHASE];
	  measure_driver_correct_copy();
	  crc=measure_correct_crc_obtain(pdata);
	  pdata->crc = crc;
      correct_save_sector1(pdata);
      correct_save_sector2(pdata);
      correct_save_sector3(pdata);
      return;
}


static void _CRD_copy(DLSR_CT_t Ddes,uint8_t srcUDL,uint8_t desUDL)
{
	correct_save_data_t*pdata;
	pdata = &m_save_correct_data;
	
	pdata->k_Imain_A[Ddes][desUDL]=pdata->k_Imain_A[Ddes][srcUDL];
	pdata->k_Imain_B[Ddes][desUDL]=pdata->k_Imain_B[Ddes][srcUDL];
	pdata->k_Imain_C[Ddes][desUDL]=pdata->k_Imain_C[Ddes][srcUDL];
	/*����*/
	pdata->k_Iline_A[Ddes][desUDL]=pdata->k_Iline_A[Ddes][srcUDL];
	pdata->k_Iline_B[Ddes][desUDL]=pdata->k_Iline_B[Ddes][srcUDL];
	pdata->k_Iline_C[Ddes][desUDL]=pdata->k_Iline_C[Ddes][srcUDL];
	/*�Ƕ�*/
	pdata->k_dxangle_A[Ddes][desUDL]=pdata->k_dxangle_A[Ddes][srcUDL];
	pdata->k_dxangle_B[Ddes][desUDL]=pdata->k_dxangle_B[Ddes][srcUDL];
	pdata->k_dxangle_C[Ddes][desUDL]=pdata->k_dxangle_C[Ddes][srcUDL];
	/*����*/
	pdata->k_Jline_A[Ddes][desUDL]=pdata->k_Jline_A[Ddes][srcUDL];
	pdata->k_Jline_B[Ddes][desUDL]=pdata->k_Jline_B[Ddes][srcUDL];
	pdata->k_Jline_C[Ddes][desUDL]=pdata->k_Jline_C[Ddes][srcUDL];

	/*����*/
	pdata->kC_Jline_A[Ddes][desUDL]=pdata->kC_Jline_A[Ddes][srcUDL];
	pdata->kC_Jline_B[Ddes][desUDL]=pdata->kC_Jline_B[Ddes][srcUDL];
	pdata->kC_Jline_C[Ddes][desUDL]=pdata->kC_Jline_C[Ddes][srcUDL];
    return;
}

static void _CRD_copy_10_to_1(DLSR_CT_t Ddes,DLSR_CT_t Dsrc)
{
	int offset;
	correct_save_data_t*pdata;
	pdata = &m_save_correct_data;
	for(offset = 0;offset<4;offset++){
		  /*����*/
		  pdata->k_Imain_A[Ddes][offset]=pdata->k_Imain_A[Dsrc][offset];
		  pdata->k_Imain_B[Ddes][offset]=pdata->k_Imain_B[Dsrc][offset];
		  pdata->k_Imain_C[Ddes][offset]=pdata->k_Imain_C[Dsrc][offset];
		  /*����*/
		  pdata->k_Iline_A[Ddes][offset]=pdata->k_Iline_A[Dsrc][offset];
		  pdata->k_Iline_B[Ddes][offset]=pdata->k_Iline_B[Dsrc][offset];
		  pdata->k_Iline_C[Ddes][offset]=pdata->k_Iline_C[Dsrc][offset];
		  /*�Ƕ�*/
		  pdata->k_dxangle_A[Ddes][offset]=pdata->k_dxangle_A[Dsrc][offset];
		  pdata->k_dxangle_B[Ddes][offset]=pdata->k_dxangle_B[Dsrc][offset];
		  pdata->k_dxangle_C[Ddes][offset]=pdata->k_dxangle_C[Dsrc][offset];
		  /*����*/
		  pdata->k_Jline_A[Ddes][offset]=pdata->k_Jline_A[Dsrc][offset];
		  pdata->k_Jline_B[Ddes][offset]=pdata->k_Jline_B[Dsrc][offset];
		  pdata->k_Jline_C[Ddes][offset]=pdata->k_Jline_C[Dsrc][offset];
		  /*����*/
		  pdata->kC_Jline_A[Ddes][offset]=pdata->kC_Jline_A[Dsrc][offset];
		  pdata->kC_Jline_B[Ddes][offset]=pdata->kC_Jline_B[Dsrc][offset];
		  pdata->kC_Jline_C[Ddes][offset]=pdata->kC_Jline_C[Dsrc][offset];

	}
    return;
}

void measure_driver_correct_copy(void)
{
	if(m_correct_level_arg.Ualevel_offset == U_INPUT_LEVEL_100V
		&&m_correct_level_arg.ADLSR_select>=DZ_N_10A)
	{
		_CRD_copy(m_correct_level_arg.ADLSR_select,U_INPUT_LEVEL_100V,U_INPUT_LEVEL_220V);
		_CRD_copy(m_correct_level_arg.ADLSR_select,U_INPUT_LEVEL_100V,U_INPUT_LEVEL_440V);
		_CRD_copy(m_correct_level_arg.ADLSR_select,U_INPUT_LEVEL_100V,U_INPUT_LEVEL_57V);
	}
    if(m_correct_level_arg.ADLSR_select==QB_N_5A)
    {
		_CRD_copy_10_to_1(QB_N_1A,QB_N_5A);
	}
    return;
}
/********************************************************************
 * @������: �ҳ�
 * @����  : ��һ���ϵ���������ݴ��̳���ʼ��
 * @����  : is_save_to_disk �Ƿ񱣴浽����	
 * @���  : NONE
********************************************************************/
void measure_correct_disk_init(BOOL is_save_to_disk)
{
	uint32_t crc;
	GLOBAL_MEMCLR(&m_save_correct_data,sizeof(correct_save_data_t));
	if(is_save_to_disk)
	{
		crc=measure_correct_crc_obtain(&m_save_correct_data);
		m_save_correct_data.crc = crc;
		correct_save_sector1(&m_save_correct_data);
		correct_save_sector2(&m_save_correct_data);
		correct_save_sector3(&m_save_correct_data);
	}
	return;
}
/********************************************************************
 * @������: �ҳ�
 * @����: �������ݶ�ȡ
 * @����:	
 * @���: NONE
********************************************************************/
static BOOL measure_correct_data_read(correct_save_data_t*pdata)
{
	uint32_t crc;
    if(!measure_correct_check_SD()) 
		return TURE;
	correct_read_sector1(pdata);
	crc=measure_correct_crc_obtain(pdata);
	if(crc==pdata->crc) return TURE;
	correct_read_sector2(pdata);
	crc=measure_correct_crc_obtain(pdata);
	if(crc==pdata->crc) return TURE;
	correct_read_sector3(pdata);
	crc=measure_correct_crc_obtain(pdata);
	if(crc==pdata->crc) return TURE;
    measure_correct_disk_init(FAIL);
	return FAIL;
}
/********************************************************************
 * @������: �ҳ�
 * @����: Ӱ������ϵ���仯����ȡ
 * @����:	
 * @���: NONE
********************************************************************/
static void measure_correct_Karg_obtain(correct_level_arg*parg,correct_data_t*pcorrect)
{
	correct_save_data_t*psv = &m_save_correct_data;
	pcorrect->k_Umain[A_PHASE] = psv->k_Umain_A[parg->Ualevel_offset];
	pcorrect->k_Umain[B_PHASE] = psv->k_Umain_B[parg->Ublevel_offset];
	pcorrect->k_Umain[C_PHASE] = psv->k_Umain_C[parg->Uclevel_offset];
	pcorrect->k_Imain[A_PHASE] = psv->k_Imain_A[parg->ADLSR_select][parg->Ualevel_offset];
	pcorrect->k_Imain[B_PHASE] = psv->k_Imain_B[parg->BDLSR_select][parg->Ublevel_offset];
	pcorrect->k_Imain[C_PHASE] = psv->k_Imain_C[parg->CDLSR_select][parg->Uclevel_offset];
	pcorrect->k_Iline[A_PHASE] = psv->k_Iline_A[parg->ADLSR_select][parg->Ualevel_offset];
	pcorrect->k_Iline[B_PHASE] = psv->k_Iline_B[parg->BDLSR_select][parg->Ublevel_offset];
	pcorrect->k_Iline[C_PHASE] = psv->k_Iline_C[parg->CDLSR_select][parg->Uclevel_offset];
    pcorrect->k_dxangle[A_PHASE]=(psv->k_dxangle_A[parg->ADLSR_select][parg->Ualevel_offset]);
    pcorrect->k_dxangle[B_PHASE]=(psv->k_dxangle_B[parg->BDLSR_select][parg->Ublevel_offset]);
    pcorrect->k_dxangle[C_PHASE]=(psv->k_dxangle_C[parg->CDLSR_select][parg->Uclevel_offset]);
    pcorrect->k_Jline[A_PHASE]  =(psv->k_Jline_A[parg->ADLSR_select][parg->Ualevel_offset]);
    pcorrect->k_Jline[B_PHASE]  =(psv->k_Jline_B[parg->BDLSR_select][parg->Ublevel_offset]);
    pcorrect->k_Jline[C_PHASE]  =(psv->k_Jline_C[parg->CDLSR_select][parg->Uclevel_offset]);
    pcorrect->kC_Jline[A_PHASE] =(psv->kC_Jline_A[parg->ADLSR_select][parg->Ualevel_offset]);
    pcorrect->kC_Jline[B_PHASE] =(psv->kC_Jline_B[parg->BDLSR_select][parg->Ublevel_offset]);
    pcorrect->kC_Jline[C_PHASE] =(psv->kC_Jline_C[parg->CDLSR_select][parg->Uclevel_offset]);
    return;
}
/********************************************************************
 * @������: �ҳ�
 * @����: ��ȡ��Ч�������
 * @����:	
 * @���: NONE
********************************************************************/
static void measure_correct_Kr_obtain(correct_level_arg*parg,K_UIR_t*pkr)
{
    pkr->kr_RI[A_PHASE] = m_I_level_Rconst[parg->Ilevel_offset];
    pkr->kr_RI[B_PHASE] = m_I_level_Rconst[parg->Ilevel_offset];
    pkr->kr_RI[C_PHASE] = m_I_level_Rconst[parg->Ilevel_offset];
    pkr->kr_RU[A_PHASE] = 1.0/m_U_level_Rconst[parg->Ualevel_offset];
    pkr->kr_RU[B_PHASE] = 1.0/m_U_level_Rconst[parg->Ublevel_offset];
    pkr->kr_RU[C_PHASE] = 1.0/m_U_level_Rconst[parg->Uclevel_offset];
	pkr->kr_RI[A_PHASE] = (float)m_DLSR_CT_const[parg->ADLSR_select]/pkr->kr_RI[A_PHASE];
	pkr->kr_RI[B_PHASE] = (float)m_DLSR_CT_const[parg->BDLSR_select]/pkr->kr_RI[B_PHASE];
	pkr->kr_RI[C_PHASE] = (float)m_DLSR_CT_const[parg->CDLSR_select]/pkr->kr_RI[C_PHASE];
  	return;
}
/********************************************************************
 * @������: �ҳ�
 * @����: ���²�������
 * @����:	
 * @���: NONE
********************************************************************/
void measure_correct_K_renew(correct_level_arg*parg)
{
	correct_data_t *pserr=&m_err_correct_data;
	K_UIR_t        *pkr=&m_sUI_kr;
	measure_correct_Karg_obtain(parg,pserr);/*��ȡ����ϵ��*/
	measure_correct_Kr_obtain(parg,pkr);	/*��ȡ��Ч�������*/
    pkr->kr_RI[A_PHASE] *=m_kconst;
    pkr->kr_RI[B_PHASE] *=m_kconst;
    pkr->kr_RI[C_PHASE] *=m_kconst;
    pkr->kr_RU[A_PHASE] *=m_kconst;
    pkr->kr_RU[B_PHASE] *=m_kconst;
    pkr->kr_RU[C_PHASE] *=m_kconst;
    pkr->kr_Rpq[A_PHASE] =pkr->kr_RI[A_PHASE]*pkr->kr_RU[A_PHASE];
    pkr->kr_Rpq[B_PHASE] =pkr->kr_RI[B_PHASE]*pkr->kr_RU[B_PHASE];
    pkr->kr_Rpq[C_PHASE] =pkr->kr_RI[C_PHASE]*pkr->kr_RU[C_PHASE];
    pserr->k_pq[A_PHASE] =(pserr->k_Umain[A_PHASE]+1)*(pserr->k_Imain[A_PHASE]+1);
    pserr->k_pq[B_PHASE] =(pserr->k_Umain[B_PHASE]+1)*(pserr->k_Imain[B_PHASE]+1);
    pserr->k_pq[C_PHASE] =(pserr->k_Umain[C_PHASE]+1)*(pserr->k_Imain[C_PHASE]+1);
	pserr->k_cos[A_PHASE]=cos(pserr->k_dxangle[A_PHASE]*3.1415926/180.0);
	pserr->k_sin[A_PHASE]=sin(pserr->k_dxangle[A_PHASE]*3.1415926/180.0);
	pserr->k_cos[B_PHASE]=cos(pserr->k_dxangle[B_PHASE]*3.1415926/180.0);
	pserr->k_sin[B_PHASE]=sin(pserr->k_dxangle[B_PHASE]*3.1415926/180.0);
	pserr->k_cos[C_PHASE]=cos(pserr->k_dxangle[C_PHASE]*3.1415926/180.0);
	pserr->k_sin[C_PHASE]=sin(pserr->k_dxangle[C_PHASE]*3.1415926/180.0);
	meausre_set_DDS_PQk();
    return;
}
/********************************************************************
 * @������: �ҳ�
 * @����: ������ѹ���̺���
 * @����:	pDLarg ��ǰ�������Psetp �����������
 * @���:	NONE
********************************************************************/
void  measure_driver_Ucorrect( 
		measure_param_t*pDLarg, 
		DL_correct_step_t*Psetp
)
{
#define GET_UMAIN_K(alreay,new_data)  {alreay=(alreay+new_data+alreay*new_data);}
	float  new_data;
	if ( Psetp->sub_step == 2 )
	{
	    if(pDLarg->phase_a.f_U!=0)
	    {
			new_data=Psetp->base_U/pDLarg->phase_a.f_U-1;
			GET_UMAIN_K(m_err_correct_data.k_Umain[A_PHASE],new_data);
	    }
	    if(pDLarg->phase_b.f_U!=0)
	    {
			new_data=Psetp->base_U/pDLarg->phase_b.f_U-1;
			GET_UMAIN_K(m_err_correct_data.k_Umain[B_PHASE],new_data);
	    }
	    if(pDLarg->phase_c.f_U!=0)
	    {	
			new_data=Psetp->base_U/pDLarg->phase_c.f_U-1;
			GET_UMAIN_K(m_err_correct_data.k_Umain[C_PHASE],new_data);
	    }
		measure_correct_data_save(
			&m_save_correct_data,
			&m_correct_level_arg,
			&m_err_correct_data);
		measure_correct_K_renew(&m_correct_level_arg);
		Psetp->sub_step = 3;
	}
	return;
}
/********************************************************************
 * @������: �ҳ�
 * @����: �����������̺���
 * @����:	pDLarg ��ǰ�������Psetp �����������
 * @���:	NONE
********************************************************************/
void  measure_driver_Icorrect( measure_param_t*pDLarg,DL_correct_step_t*Psetp )
{
#define GET_IMAIN_K(alreay,new_data)  {alreay=(alreay+new_data+alreay*new_data);}
	float  new_data;
	if ( Psetp->sub_step == 2 )
	{
        new_data =(pDLarg->f_path1_err[0]/100.0);
		GET_IMAIN_K(m_err_correct_data.k_Imain[Psetp->phase],new_data);
		measure_correct_data_save(
			&m_save_correct_data,
			&m_correct_level_arg,
			&m_err_correct_data);
		measure_correct_K_renew(&m_correct_level_arg);
		Psetp->sub_step = 3;
	}
	return;
}
/********************************************************************
 * @������: �ҳ�
 * @����: �����������̺���
 * @����: pDLarg ��ǰ�������Psetp �����������
 * @���: NONE
 ********************************************************************/
void  measure_driver_Ilinecorrect( measure_param_t*pDLarg, DL_correct_step_t*Psetp )
{
	float  new_data;
	if ( Psetp->sub_step == 2 )
	{
        new_data =(pDLarg->f_path1_err[0]/100.0);
		m_err_correct_data.k_Iline[Psetp->phase]+=new_data;
		measure_correct_data_save(
			&m_save_correct_data,
			&m_correct_level_arg,
			&m_err_correct_data);	
		measure_correct_K_renew(&m_correct_level_arg);
		Psetp->sub_step = 3;
	}
	return;
}
/********************************************************************
 * @������: �ҳ�
 * @����: �����������̺���
 * @����: pDLarg ��ǰ�������Psetp �����������
 * @���: NONE
********************************************************************/
void  measure_driver_Jlinecorrect( measure_param_t*pDLarg, DL_correct_step_t*Psetp )
{
	float base_J;
	if ( Psetp->sub_step == 2 )
	{	
		base_J = (pDLarg->f_path1_err[0]/0.05)/60.0;
		m_err_correct_data.k_Jline[Psetp->phase]+=base_J;
		m_err_correct_data.b_Jline_arg[Psetp->phase]= pDLarg->phase_a.f_I;
		measure_correct_data_save(
			&m_save_correct_data,
			&m_correct_level_arg,
			&m_err_correct_data);
		measure_correct_K_renew(&m_correct_level_arg);
		Psetp->sub_step = 3;
	}
   return;
}
/********************************************************************
 * @������: �ҳ�
 * @����: �����������̺���
 * @����: pDLarg ��ǰ�������Psetp �����������
 * @���: NONE
********************************************************************/
void  measure_driver_JClinecorrect( measure_param_t*pDLarg, DL_correct_step_t*Psetp )
{
	float base_J;
	if ( Psetp->sub_step == 2 )
	{	
		base_J = (pDLarg->f_path1_err[0]/0.05)/60.0;
		m_err_correct_data.kC_Jline[Psetp->phase]+=base_J;
		m_err_correct_data.bC_Jline_arg[Psetp->phase]= pDLarg->phase_a.f_I;
		measure_correct_data_save(
			&m_save_correct_data,
			&m_correct_level_arg,
			&m_err_correct_data);
		measure_correct_K_renew(&m_correct_level_arg);
		Psetp->sub_step = 3;
	}
   return;
}
/********************************************************************
 * @������: �ҳ�
 * @����: �����Ƕ����̺���
 * @����: pDLarg ��ǰ�������Psetp �����������
 * @���: NONE
********************************************************************/
void  measure_driver_Jcorrect( measure_param_t*pDLarg,DL_correct_step_t*Psetp)
{
	float base_J;
	if ( Psetp->sub_step == 2 )
	{
		base_J = (pDLarg->f_path1_err[0]/0.05)/60.0;
		m_err_correct_data.k_dxangle[Psetp->phase]+=base_J;
		m_err_correct_data.k_Jline_arg[Psetp->phase] = pDLarg->phase_a.f_I;
		m_err_correct_data.kC_Jline_arg[Psetp->phase]= pDLarg->phase_a.f_I;
		measure_correct_data_save(
			&m_save_correct_data,
			&m_correct_level_arg,
			&m_err_correct_data);
		measure_correct_K_renew(&m_correct_level_arg);
		Psetp->sub_step = 3;
	}
   return;
}
/********************************************************************
 * @������: �ҳ�
 * @����:	���������������
 * @����:	pDLarg ��ǰ�������Psetp �����������
 * @���:	NONE
********************************************************************/
void measure_filter_init(void);
void measure_driver_correct_handle( measure_param_t*pDLarg, DL_correct_step_t*Psetp )
{
	if ( Psetp->main_step>JCLINECORRECT_SETP )
		return;
	if ( Psetp->main_step == UCORRECT_SETP )
	{
		measure_driver_Ucorrect( pDLarg, Psetp );
	}
	else if ( Psetp->main_step == ICORRECT_SETP 
		||Psetp->main_step == ILINECORRECT_SETP 
		||Psetp->main_step == JCORRECT_SETP 
		||Psetp->main_step == JLINECORRECT_SETP
		||Psetp->main_step == JCLINECORRECT_SETP)
	{

        if(Psetp->sub_step != 3)
        {
			measure_filter_init();
        }
		if ( Psetp->sub_step == 1 )
		{
			if(Psetp->phase==0)meausre_DDS_out_select(1,0,0);
			else if(Psetp->phase==1)meausre_DDS_out_select(0,1,0);
			else if(Psetp->phase==2)meausre_DDS_out_select(0,0,1);
			Psetp->sub_step = 3;
		}
		if ( Psetp->main_step == ICORRECT_SETP )
		{
			measure_driver_Icorrect( pDLarg, Psetp );
		}
		else if ( Psetp->main_step == JCORRECT_SETP )
		{
			measure_driver_Jcorrect( pDLarg, Psetp );
		}
		else if ( Psetp->main_step == JLINECORRECT_SETP )
		{
			measure_driver_Jlinecorrect( pDLarg, Psetp );
		}
		else if ( Psetp->main_step == JCLINECORRECT_SETP )
		{
			measure_driver_JClinecorrect( pDLarg, Psetp );
		}
		else if ( Psetp->main_step ==ILINECORRECT_SETP )
		{
			measure_driver_Ilinecorrect( pDLarg, Psetp );
		}
	}
	return;
}
/********************************************************************
 * @������: �ҳ�
 * @����: ���ȫ������ϵ��
 * @����:	
 * @���: NONE
********************************************************************/
void measure_correct_clean_all(void)
{
	correct_save_data_t*pdata=&m_save_correct_data;
	GLOBAL_MEMCLR(pdata, sizeof(correct_save_data_t));
	pdata->crc = measure_correct_crc_obtain(pdata);
	correct_save_sector1(pdata);
	correct_save_sector2(pdata);
	correct_save_sector3(pdata); 
	measure_correct_K_renew(&m_correct_level_arg);
	return;
}

/********************************************************************
 * @������: �ҳ�
 * @����: �������ϵ��
 * @����:	
 * @���: NONE
********************************************************************/
void measure_correct_clean_data(DL_correct_step_t*Psetp)
{
	correct_save_data_t*pdata=&m_save_correct_data;
    if(Psetp->main_step==UCORRECT_SETP)
    {
		GLOBAL_MEMCLR(pdata->k_Umain_A,sizeof(float)*MAX_U_LEVEL_N);
		GLOBAL_MEMCLR(pdata->k_Umain_B,sizeof(float)*MAX_U_LEVEL_N);
		GLOBAL_MEMCLR(pdata->k_Uline_C,sizeof(float)*MAX_U_LEVEL_N);
		GLOBAL_MEMCLR(pdata->k_Uline_A,sizeof(float)*MAX_U_LEVEL_N);
		GLOBAL_MEMCLR(pdata->k_Uline_B,sizeof(float)*MAX_U_LEVEL_N);
		GLOBAL_MEMCLR(pdata->k_Uline_C,sizeof(float)*MAX_U_LEVEL_N);
		pdata->crc = measure_correct_crc_obtain(pdata);
		correct_save_sector1(pdata);
		correct_save_sector2(pdata);
		correct_save_sector3(pdata); 
		measure_correct_K_renew(&m_correct_level_arg);
		return;
	}
    else if(Psetp->main_step==ILINECORRECT_SETP)
    {
         m_err_correct_data.k_Iline[Psetp->phase]=0;
	}
    else if(Psetp->main_step==ICORRECT_SETP)
    {
        m_err_correct_data.k_Imain[Psetp->phase]=0;
	}
	else if(Psetp->main_step==JLINECORRECT_SETP)
	{
		m_err_correct_data.k_Jline[Psetp->phase]=0;
		m_err_correct_data.b_Jline_arg[Psetp->phase]=0;
	}
	else if(Psetp->main_step==JCLINECORRECT_SETP)
	{
		m_err_correct_data.kC_Jline[Psetp->phase]=0;
		m_err_correct_data.bC_Jline_arg[Psetp->phase]=0;
	}
	else if(Psetp->main_step==JCORRECT_SETP)
	{
		m_err_correct_data.k_dxangle[Psetp->phase]=0;
		m_err_correct_data.k_Jline_arg[Psetp->phase]=0;
		m_err_correct_data.kC_Jline_arg[Psetp->phase]=0;
	}
	measure_correct_data_save(&m_save_correct_data,
		&m_correct_level_arg,&m_err_correct_data);
	measure_correct_K_renew(&m_correct_level_arg);
	return;
}
/********************************************************************
 * @������: �ҳ�
 * @����: ��������ģ���ʼ��
 * @����:	
 * @���: NONE
********************************************************************/
void measure_correct_data_module_init(void)
{
	GLOBAL_MEMCLR(&m_save_correct_data,sizeof(correct_save_data_t));
	GLOBAL_MEMCLR(&m_err_correct_data,sizeof(correct_data_t));
 	measure_correct_data_read(&m_save_correct_data);
	return;
}

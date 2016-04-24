#ifndef  _PEC_2011_MEASURE_H
#define  _PEC_2011_MEASURE_H
#include "../../Osdriver.h"
/*********************************************************************
* MEASURE_H    for MEASURE_CONNECT
**********************************************************************/
enum
{
   Q_COUNT_WAY_IS_SUM = 0,
   Q_COUNT_WAY_IS_UIsubP	

};
void 	 measure_entery(void);
void 	 measure_exit(void);
void 	 measure_driver_Udown_check_start(float data);
/*********************************************************************
* MEASURE_H    for MEASURE_GetDATA
**********************************************************************/
#define     AD_SIZE  				 1024
#define  	BUS_WRITE(data,addr)     (*((uint16_t*)addr)=data)
#define  	BUS_READ(addr)   	     (*((uint16_t*)addr))
#define  	BUS_READ32bit(addr)   	 (*((uint32_t*)addr))
#define     READ32BIT_DATA(addr)  	 (BUS_READ(addr)+(BUS_READ(addr+1))*0x10000)
/*FPGAͨѶ���������������*/
#define     READ_SAMPING_POINT_ADDR  (0X41FF)
#define     IA_ADDR                  (0X4200)
#define     IB_ADDR                  (IA_ADDR+2)
#define     IC_ADDR                  (IB_ADDR+2)
#define     UA_ADDR                  (IC_ADDR+2)
#define     UB_ADDR                  (UA_ADDR+2)
#define     UC_ADDR                  (UB_ADDR+2)
#define  	SET_OFFSET				 (UC_ADDR+2)
#define  	SUM_IA_ADDR				 (SET_OFFSET+2)
#define     SUM_IB_ADDR              (SUM_IA_ADDR+4)
#define     SUM_IC_ADDR              (SUM_IB_ADDR+4)
#define     SUM_UA_ADDR              (SUM_IC_ADDR+4)
#define     SUM_UB_ADDR              (SUM_UA_ADDR+4)
#define     SUM_UC_ADDR              (SUM_UB_ADDR+4)
#define     SUM_PA_ADDR              (SUM_UC_ADDR+4)
#define     SUM_PB_ADDR              (SUM_PA_ADDR+4)
#define     SUM_PC_ADDR              (SUM_PB_ADDR+4)
#define  	SELF_U_ADDR				 (SUM_PC_ADDR+4)
#define  	SELF_I_ADDR				 (SELF_U_ADDR+2)  //0x4234  

#define     READ_FREQ                (0x42a0)		
#define     DDS_BASE_PATH1_UNIT_SET  (0x42a2)	
#define   	DDS_BASE_PATH2_UNIT_SET  (0x42a4)
#define   	DDS_BASE_PATH3_UNIT_SET  (0x42C0)	

#define   	GD_PATH1_SUB_CIRCLE	 	 (0x42a6)
#define   	GD_PATH2_SUB_CIRCLE	 	 (0x42a8)
#define   	GD_PATH3_SUB_CIRCLE	 	 (0x42C2)

#define   	DDS_PATH1_SET_CIRCLE     (0x42aa)
#define   	DDS_PATH2_SET_CIRCLE     (0x42ac)
#define   	DDS_PATH3_SET_CIRCLE     (0x42C4)

#define   	DDS_READ_PATH1_SUMFL   	 (0x42ae)
#define   	DDS_READ_PATH2_SUMFL   	 (0x42b0)
#define   	DDS_READ_PATH3_SUMFL  	 (0x42C6)

#define   	DDS_SET_BASE_OUT_PATH	 (0x42b6)

enum
{
   DDS_FOR_SINGLE_METER  = 0,
   DDS_FOR_DOUBLE_METER 
};

enum
{
   ERR_WAY_YG = 1,
   ERR_WAY_WG
};

typedef struct
{
	Bsize_t    single_or_double;  //������˫��     0 ����   1 ˫��
	Bsize_t    hand_or_auto;      //�ֶ������Զ�     0 �Զ�   1 �ֶ�
	Bsize_t    P_or_Q;            //�й����޹�       0 �й�   1 �޹�
}path_select_t;

extern path_select_t m_DDS_path_mod;

extern int16_t 	    m_Ia_buf[AD_SIZE];         
extern int16_t      m_Ib_buf[AD_SIZE];    
extern int16_t      m_Ic_buf[AD_SIZE]; 
extern int16_t      m_Ua_buf[AD_SIZE];         
extern int16_t      m_Ub_buf[AD_SIZE];    
extern int16_t      m_Uc_buf[AD_SIZE]; 

extern BOOL 	 	m_read_ad_finish;



interrupt void ISR_read_fpga_data(void);
interrupt void ISR_self_correct_read_fpga_data(void);

void measure_driver_start(void);
void measure_driver_stop(void);


typedef struct
{
 int64_t sum_U[3];
 int64_t sum_I[3];
 int64_t sum_P[3];
 int64_t sum_Q[3];
 int64_t sum_selfU;
 int64_t sum_selfI;
}sum_data_t;


typedef struct
{
	float        DDS_krpq[3];
	float        DDS_ksin[3];
	float        DDS_kcos[3];
	float        DDS_Iline[3];
}DDS_k_t;

extern sum_data_t   m_sum_data_int;
extern sum_data_t   m_sum_data_ext;
typedef struct
{
  uint16_t byte_0;
  uint16_t byte_1;
  uint16_t byte_2;
  uint16_t byte_3;
}_16bits_t;

typedef struct
{
  uint32_t byte_0;
  uint32_t byte_1;
}_32bits_t;


typedef struct
{
  uint16_t byte_0;
  uint32_t byte_1;
  uint16_t byte_3;
}_32_16bits_t;

typedef union
{
  uint64_t all;
  _32bits_t bytes_2;
  _16bits_t bytes_4;
}ad_64bit_u;




typedef struct
{
  uint16_t byte_0;
  uint16_t byte_1;
}_32div16bits_t;

typedef union
{
  uint32_t all;
_32div16bits_t bytes_2;
}ad_32bit_u;

/*********************************************************************
* MEASURE_H    for MEASURE_CORRECT
**********************************************************************/
typedef enum
{
	DZ_N_1A = 0,
	DZ_N_10A, 
	QB_N_1A, 	
	QB_N_5A, 
	QB_N_20A, 
	QB_N_100A, 
	QB_N_500A, 
	QB_N_1000A, 
	QB_N_1500A,
	DZ_LEVEL_END
}DLSR_CT_t;
/**********************************************************************/
#define  MAX_U_LEVEL_N      (4) 			 //��ʾ����ѹ��λ
#define  MAX_I_LEVEL_N      (4)
#define  MAX_QB_N    		(DZ_LEVEL_END)   //��ʾ���ǯ��λ��
/**********************************************************************/
#define          JXMOD_3_3  		1
#define          JXMOD_3_4  		0
#define   		 A_PHASE        	0
#define   		 B_PHASE      		1
#define   		 C_PHASE            2
typedef struct
{
  float  k_Umain[3];   /*�������*/     
  float  k_Uline[3];   /*�������*/   
  float  k_Imain[3];   /*�������*/ 
  float  k_Iline[3];   /*�������*/   
  float  k_pq[3];
  float  k_dxangle[3];
  float  k_Jline[3];
  float  k_sin[3];
  float  k_cos[3];
  float  k_Jline_arg[3];
  float  b_Jline_arg[3];
  float  kC_Jline[3];
  float  kC_Jline_arg[3];
  float  bC_Jline_arg[3];
}correct_data_t;


enum
{
	UCORRECT_SETP = 0,
	ICORRECT_SETP ,
	ILINECORRECT_SETP ,
	JCORRECT_SETP,
	JLINECORRECT_SETP,
	JCLINECORRECT_SETP
};
/*У�����ݽṹ��*/
typedef struct
{
   BOOL		DL_data_is_new;
   BOOL		DL_path1_ERR_is_new;
   BOOL		DL_path2_ERR_is_new;
   BOOL		DL_path3_ERR_is_new;
   BOOL     DL_XB_is_new;
   BOOL  	Ua_is_lost;  
   BOOL  	Ub_is_lost; 
   BOOL  	Uc_is_lost; 
   BOOL  	Ia_is_lost; 
   BOOL  	Ib_is_lost; 
   BOOL  	Ic_is_lost;  
   uint8_t  phase3or4; 
   uint8_t  measure_mode;
}DL_flag_t;

//���������Ӧ������ƫ��
enum
{
   I_INPUT_1 		  = 0,
   I_INPUT_10		  = 1,
   I_INPUT_100		  = 2,
   I_INPUT_1000		  = 3,
   U_INPUT_LEVEL_440V = 0,
   U_INPUT_LEVEL_220V = 1,
   U_INPUT_LEVEL_100V = 2,
   U_INPUT_LEVEL_57V  = 3
};
/*����У������ṹ��*/
typedef struct
{
	uint8_t   main_step; /*������*/
	uint8_t   sub_step;  /*�Ӳ���*/
	float     base_U;
	float     base_I;
	float     base_p;
	float     base_Q;
	uint8_t   phase;
	uint8_t   pos;
}DL_correct_step_t;
typedef  struct
{
   DLSR_CT_t ADLSR_select;    /*��������ѡ��*/
   DLSR_CT_t BDLSR_select;    /*��������ѡ��*/
   DLSR_CT_t CDLSR_select;    /*��������ѡ��*/
   uint16_t  Ualevel_offset;  /*���ѹ��λϵ��*/
   uint16_t  Ublevel_offset;  /*���ѹ��λϵ��*/
   uint16_t  Uclevel_offset;  /*���ѹ��λϵ��*/  
   uint16_t  Ilevel_offset;   /*A�������λϵ��*/
}correct_level_arg;         

typedef struct
{
/*����*/
  float  k_Umain_A[MAX_U_LEVEL_N];     
  float  k_Umain_B[MAX_U_LEVEL_N];   
  float  k_Umain_C[MAX_U_LEVEL_N];     
  float  k_Imain_A[MAX_QB_N][MAX_U_LEVEL_N];   
  float  k_Imain_B[MAX_QB_N][MAX_U_LEVEL_N];
  float  k_Imain_C[MAX_QB_N][MAX_U_LEVEL_N];
/*�Ƕ�*/
  float  k_dxangle_A[MAX_QB_N][MAX_U_LEVEL_N];
  float  k_dxangle_B[MAX_QB_N][MAX_U_LEVEL_N];
  float  k_dxangle_C[MAX_QB_N][MAX_U_LEVEL_N];
/*����*/
  float  k_Uline_A[MAX_U_LEVEL_N];     
  float  k_Uline_B[MAX_U_LEVEL_N];   
  float  k_Uline_C[MAX_U_LEVEL_N];     
  float  k_Iline_A[MAX_QB_N][MAX_U_LEVEL_N];    
  float  k_Iline_B[MAX_QB_N][MAX_U_LEVEL_N];
  float  k_Iline_C[MAX_QB_N][MAX_U_LEVEL_N];
/*����*/
  float  k_Jline_A[MAX_QB_N][MAX_U_LEVEL_N];
  float  k_Jline_B[MAX_QB_N][MAX_U_LEVEL_N];
  float  k_Jline_C[MAX_QB_N][MAX_U_LEVEL_N];
/*����ϵ��*/
  float  k_JAline_arg[MAX_QB_N][MAX_U_LEVEL_N];
  float  b_JAline_arg[MAX_QB_N][MAX_U_LEVEL_N];
  float  k_JBline_arg[MAX_QB_N][MAX_U_LEVEL_N];
  float  b_JBline_arg[MAX_QB_N][MAX_U_LEVEL_N];
  float  k_JCline_arg[MAX_QB_N][MAX_U_LEVEL_N];
  float  b_JCline_arg[MAX_QB_N][MAX_U_LEVEL_N];
//����
  float  kC_Jline_A[MAX_QB_N][MAX_U_LEVEL_N];
  float  kC_Jline_B[MAX_QB_N][MAX_U_LEVEL_N];
  float  kC_Jline_C[MAX_QB_N][MAX_U_LEVEL_N];
//����ϵ��
  float  kC_JAline_arg[MAX_QB_N][MAX_U_LEVEL_N];
  float  bC_JAline_arg[MAX_QB_N][MAX_U_LEVEL_N];
  float  kC_JBline_arg[MAX_QB_N][MAX_U_LEVEL_N];
  float  bC_JBline_arg[MAX_QB_N][MAX_U_LEVEL_N];
  float  kC_JCline_arg[MAX_QB_N][MAX_U_LEVEL_N];
  float  bC_JCline_arg[MAX_QB_N][MAX_U_LEVEL_N];
  uint32_t crc;
}correct_save_data_t;


typedef struct
{
  float  kr_RU[3];/*��ѹ��Ч����*/
  float  kr_RI[3];/*������Ч����*/
  float  kr_Rpq[3];/*���㹦�ʵ�Ч����*/
}K_UIR_t;

void  			OSSchedLock( void );

void  			OSSchedUnlock(void);

void 			measure_correct_K_renew(correct_level_arg*parg);

void 			measure_correct_clean_data(DL_correct_step_t*Psetp);

void 			measure_correct_data_module_init(void);

void 			measure_driver_Udown_ENCON(uint8_t key);

void 			measure_driver_correct_copy(void);


extern  float 	m_angle_start[6];

extern  float 	F_Harm[6][52];
/*********************************************************************
* MEASURE_H    for MEASURE_COUNT
**********************************************************************/
void 	measure_data_obtain(measure_param_t*pDL);

void 	measure_data_filtering(measure_param_t*psrc,measure_param_t*pdes);

void 	measure_driver_count_COS( measure_param_t*pDL);

void  	measure_driver_read_DLarg_init( void );
	
void	meausre_angle_data_obtain(measure_param_t*pDL);
/*********************************************************************
* MEASURE_H    for MEASURE_CTScale
**********************************************************************/
void measure_CTScale_init(void);
void measure_CTScale_select_PLL(char UI,char phase);
void measure_CTScale_select_3or4(Bsize_t JX_F);
void measure_CTScale_select_I_level(correct_level_arg*level);
void measure_CTScale_select_U_level(correct_level_arg*level);
extern Bsize_t	m_pll_pos ;
typedef  struct
{
  Bsize_t level;
  float   f_max;
  float   f_change_value;
  float   f_min;
}Ulevel_change_t;
/*********************************************************************
* MEASURE_H    for ERROR
**********************************************************************/
void  measure_error_count_init(void);

void  measure_driver_ZSSERR_load(float err);

void  measure_driver_ERR_way(int way);

void  measure_error_AA(measure_param_t *pDL);

extern int32_t     m_DC_Udx[3];
extern int32_t     m_DC_Idx[3];

void  fft_Exc(uint16_t pos );

float fft_angle_obtain(int n,int pos);

float fft_thd_obtain( int pos);

float fft_fftUage_obtain(int pos,int n);

typedef struct
{
  uint32_t path_dds_cnt[3];
  uint16_t path_qs[3];
  long	   path_cs[3];
  BOOL     path_refresh[3];
  float    f_path_BB[3];
  float    f_path1_err[4];
  float    f_path2_err[4];
  float    f_path3_err[4];
}err_count_t;
extern	float	m_path_count_err_value[3];
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
typedef struct
{
   uint32_t	  DDS_contain_A;
   uint32_t   DDS_contain_B;
   uint32_t   DDS_contain_C;
} DDS_contain_t;
void meausre_set_DDS_PQk(void);
void meausre_DDS_out_select(uint32_t a_phase,uint32_t b_phase,uint32_t c_phase);
void measure_self_DLdata_count(measure_param_t*pDL);
void measure_driver_correct_handle( measure_param_t*pDLarg, DL_correct_step_t*Psetp );
/***********************************************************/
extern	measure_param_t   		m_measure_param;
extern	a_lock_t *      		m_measure_lock;
extern	correct_level_arg       m_correct_level_arg;
extern	DL_flag_t               m_measure_flag;
extern 	err_count_t     		m_err_Sdata; 
extern 	DDS_k_t      		   	m_DDS_PQk;           
extern 	DDS_contain_t           m_DDS_contain; 
extern 	K_UIR_t   			   	m_sUI_kr;
extern 	correct_data_t   	   	m_err_correct_data;
extern 	float                   m_kconst;
extern  float       			m_dds_constBL;

void 	DA_TEST(void);
void 	measue_DDSconst_mod(uint32_t Iconst);
void 	measure_GetADdata_init(void);
void 	fft_init(void);
void 	measue_GetADdata_stop(void);
void 	measue_GetADdata_start(void);
void 	fft_creat(int*buf,int point,float A,float angle0);
void 	fft_add(int *buf,int point ,float A,float uage,float angle0,int n);
#endif

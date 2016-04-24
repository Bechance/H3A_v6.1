/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:系统电参数测量驱动
* 创建时间:2010-12-20
* 所属公司:科荟
* 文件名称:measure_GetData.c
* 创建人:揭成
* 功能描述: 测量模块数据获取
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
#pragma  	DATA_SECTION (m_Ia_buf, 		"FAST_RAM")/*把变量定义在FAST_RAM*/
#pragma  	DATA_SECTION (m_Ib_buf, 		"FAST_RAM")/*把变量定义在FAST_RAM*/
#pragma  	DATA_SECTION (m_Ic_buf, 		"FAST_RAM")/*把变量定义在FAST_RAM*/
#pragma  	DATA_SECTION (m_Ua_buf, 		"FAST_RAM")/*把变量定义在FAST_RAM*/
#pragma  	DATA_SECTION (m_Ub_buf, 		"FAST_RAM")/*把变量定义在FAST_RAM*/
#pragma  	DATA_SECTION (m_Uc_buf, 		"FAST_RAM")/*把变量定义在FAST_RAM*/
#pragma  	DATA_SECTION (m_ad_buf_offset, 	"FAST_RAM")/*把变量定义在FAST_RAM*/
#pragma  	DATA_SECTION (m_int_offset, 	"FAST_RAM")/*把变量定义在FAST_RAM*/
#pragma  	DATA_SECTION (m_addr_offset, 	"FAST_RAM")/*把变量定义在FAST_RAM*/
#pragma  	DATA_SECTION (m_sum_data_int, 	"FAST_RAM")/*把变量定义在FAST_RAM*/
#pragma  	DATA_SECTION (m_sum_data_ext, 	"FAST_RAM")/*把变量定义在FAST_RAM*/
#pragma  	DATA_SECTION (m_start_offset, 	"FAST_RAM")/*把变量定义在FAST_RAM*/
#pragma  	DATA_SECTION (m_read_ad_finish, "FAST_RAM")/*把变量定义在FAST_RAM*/
#pragma  	DATA_SECTION (m_ad_64bit, 		"FAST_RAM")/*把变量定义在FAST_RAM*/
#pragma  	DATA_SECTION (m_ad_32bit, 		"FAST_RAM")/*把变量定义在FAST_RAM*/
#pragma  	DATA_SECTION (m_90_offset,      "FAST_RAM")/*把变量定义在FAST_RAM*/
#pragma  	DATA_SECTION (m_DDS_PQk, 		"FAST_RAM")/*把变量定义在FAST_RAM*/
#pragma  	DATA_SECTION (m_DDS_path1_data, "FAST_RAM")/*把变量定义在FAST_RAM*/
#pragma  	DATA_SECTION (m_DDS_path2_data, "FAST_RAM")/*把变量定义在FAST_RAM*/
#pragma  	DATA_SECTION (m_DC_Udx,         "FAST_RAM")/*把变量定义在FAST_RAM*/
#pragma  	DATA_SECTION (m_DC_Idx,         "FAST_RAM")/*把变量定义在FAST_RAM*/
#pragma  	DATA_SECTION (m_DDS_path_mod,   "FAST_RAM")/*把变量定义在FAST_RAM*/
#pragma  	DATA_SECTION (m_phase_fp,       "FAST_RAM")/*把变量定义在FAST_RAM*/
#pragma  	DATA_SECTION (m_phase_fq,   	"FAST_RAM")/*把变量定义在FAST_RAM*/
#pragma  	DATA_SECTION (m_32bit_U,     	"FAST_RAM")/*把变量定义在FAST_RAM*/
#pragma  	DATA_SECTION (m_32bit_I,     	"FAST_RAM")/*把变量定义在FAST_RAM*/
#pragma  	DATA_SECTION (m_32bit_P,     	"FAST_RAM")/*把变量定义在FAST_RAM*/
#pragma  	DATA_SECTION (m_32bit_Q,     	"FAST_RAM")/*把变量定义在FAST_RAM*/
#pragma  	DATA_SECTION (m_64bit_P,     	"FAST_RAM")/*把变量定义在FAST_RAM*/
#pragma  	DATA_SECTION (m_64bit_Q,     	"FAST_RAM")/*把变量定义在FAST_RAM*/
#pragma  	DATA_SECTION (m_64bit_UI,     	"FAST_RAM")/*把变量定义在FAST_RAM*/
/****************************************************************/
float 		 measure_sqrt_64bit_data(uint64_t indata);
static       Bsize_t 	 	m_int_offset  	=0;   	//中断快速临时变量，为了减少中断入栈次数定义
static       Bsize_t 	 	m_start_offset	=0; 	//中断快速临时变量，为了减少中断入栈次数定义
static       Bsize_t 	 	m_addr_offset 	=0; 	//中断快速临时变量，为了减少中断入栈次数定义
static       Bsize_t     	m_ad_buf_offset	=0;		//当前缓冲位置
static       Bsize_t     	m_90_offset		=768;	//移动90度后相对m_ad_buf_offset的偏移。  	    	    
/****************************************************************/
int16_t 	 	m_Ia_buf[AD_SIZE];       			//A相电流AD数据BUF     
int16_t      	m_Ib_buf[AD_SIZE];       			//B相电流AD数据BUF     
int16_t      	m_Ic_buf[AD_SIZE];       			//C相电流AD数据BUF     
int16_t      	m_Ua_buf[AD_SIZE];       			//A相电压AD数据BUF     
int16_t      	m_Ub_buf[AD_SIZE];       			//B相电压AD数据BUF       
int16_t      	m_Uc_buf[AD_SIZE];       			//C相电压AD数据BUF    
int16_t      	m_selfU_buf[AD_SIZE];       		//自修正电压基准AD数据BUF    
int16_t      	m_selfI_buf[AD_SIZE];       		//自修正电流基准AD数据BUF    
BOOL 	 		m_read_ad_finish = FAIL; 			//表示一周波数据是否完成
sum_data_t   	m_sum_data_int;          			//中断中存放各相累加和
sum_data_t   	m_sum_data_ext;                     //传到外部的各相累加和
ad_64bit_u   	m_ad_64bit;    						//64BIT数据联合，为了方便操作而定义
uint32_t     	m_ad_32bit;                         //64BIT数据联合，为了方便操作而定义 
ad_32bit_u   	m_DDS_path1_data;                   //32BIT数据联合，为了方便操作而定义 
ad_32bit_u   	m_DDS_path2_data;                   //32BIT数据联合，为了方便操作而定义 
DDS_k_t      	m_DDS_PQk;                          //DDS数据修正系数，可以与应用中的修正系数一样
path_select_t 	m_DDS_path_mod;  					//DDS数据通道指示
extern          DL_flag_t m_measure_flag;
/*-------------------------------------------------------------------------------
DDS 计算相关变量，为了是该部分能独立，将该部分变量单独列出
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
/*计算无功引入变量*/
int32_t      m_32bit_U;  
int32_t      m_32bit_I;  
int32_t      m_32bit_P;  
int32_t      m_32bit_Q;  
int64_t      m_64bit_P;  
int64_t      m_64bit_Q;  
int64_t      m_64bit_UI;  
/*******************************************************************************
*******************************************************************************
 中断运算引入的计算宏,为了减少系统开销，没有做成函数
********************************************************************************/
#define   	READ_FPGA_AD_DATA(addr,buf)  {buf[m_ad_buf_offset]=BUS_READ(addr);}
/*---------------------------------------------------------------*/
/*将中断累积数据传到外部*/
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
/*将中断累积数据BUF清空*/
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
/*计算无功*/
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

/*计算DDS*/
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
 * @创建人: 揭成
 * @功能: FPGA控制AD采样节奏中断服务程序，304US进一次中断
 * @输入:	
 * @输出: NONE
 该中断程序实现了AD采样数据的传递，实现了6路AD数据的读取，
 并且读取相关计算结果。
 注意:因为该中断过于频繁，为了提高系统性能，该中断中不要使用
 函数嵌套，以免造成CPU使用率过高
********************************************************************/
#define SUM_Q_BUF(Abuf,Bbuf,sum){sum+=(int64_t)Abuf[m_start_offset]*Bbuf[m_90_offset];}
#define SUM_P_BUF(Abuf,Bbuf,sum){sum+=(int64_t)Abuf[m_start_offset]*Bbuf[m_start_offset];}
#define SUM_UI_BUF(buf,sum){sum+=(int64_t)buf[m_start_offset]*buf[m_start_offset];}
#pragma CODE_SECTION(ISR_read_fpga_data,"ramfuncs") //将该段代码限制在ramfuncs。
interrupt void ISR_read_fpga_data(void)
{
	m_addr_offset  = 0; 
	m_start_offset = m_ad_buf_offset;
/*------------------------------------------------------------------------*/
/*读取AD_PAGE_SIZE点波形数据*/
/*------------------------------------------------------------------------*/
	for(m_int_offset=0;m_int_offset<(AD_PAGE_SIZE);m_int_offset++)
    {	
		BUS_WRITE(m_addr_offset,SET_OFFSET);			//设置当前数据组
        READ_FPGA_AD_DATA(IA_ADDR,m_Ia_buf);			//读取A相电流CODE
		READ_FPGA_AD_DATA(UA_ADDR,m_Ua_buf);			//读取A相电压CODE
        READ_FPGA_AD_DATA(IB_ADDR,m_Ib_buf);			//读取B相电流CODE
		READ_FPGA_AD_DATA(UB_ADDR,m_Ub_buf);			//读取B相电压CODE
        READ_FPGA_AD_DATA(IC_ADDR,m_Ic_buf);			//读取B相电流CODE
		READ_FPGA_AD_DATA(UC_ADDR,m_Uc_buf);			//读取B相电压CODE	
		m_addr_offset+=1;
        m_Ia_buf[m_ad_buf_offset]-=m_DC_Idx[A_PHASE];   //消除直流点
        m_Ua_buf[m_ad_buf_offset]-=m_DC_Udx[A_PHASE];   //消除直流点
        m_Ib_buf[m_ad_buf_offset]-=m_DC_Idx[B_PHASE];   //消除直流点
        m_Ub_buf[m_ad_buf_offset]-=m_DC_Udx[B_PHASE];   //消除直流点
        m_Ic_buf[m_ad_buf_offset]-=m_DC_Idx[C_PHASE];   //消除直流点
        m_Uc_buf[m_ad_buf_offset]-=m_DC_Udx[C_PHASE];   //消除直流点	
		m_ad_buf_offset++;
	}	
	for(m_int_offset=0;m_int_offset<(AD_PAGE_SIZE);m_int_offset++)
	{  
		m_90_offset=GET_SHIFT_90(m_start_offset);
		SUM_Q_BUF(m_Ia_buf,m_Ua_buf,m_sum_data_int.sum_Q[A_PHASE]);//计算QA 16点累加和
		SUM_Q_BUF(m_Ib_buf,m_Ub_buf,m_sum_data_int.sum_Q[B_PHASE]);//计算QB 16点累加和
		SUM_Q_BUF(m_Ic_buf,m_Uc_buf,m_sum_data_int.sum_Q[C_PHASE]);//计算QC 16点累加和
		SUM_P_BUF(m_Ia_buf,m_Ua_buf,m_sum_data_int.sum_P[A_PHASE]);//计算PA 16点累加和
		SUM_P_BUF(m_Ib_buf,m_Ub_buf,m_sum_data_int.sum_P[B_PHASE]);//计算PB 16点累加和
		SUM_P_BUF(m_Ic_buf,m_Uc_buf,m_sum_data_int.sum_P[C_PHASE]);//计算PC 16点累加和
		SUM_UI_BUF(m_Ia_buf,m_sum_data_int.sum_I[A_PHASE]);
		SUM_UI_BUF(m_Ua_buf,m_sum_data_int.sum_U[A_PHASE]);
		SUM_UI_BUF(m_Ib_buf,m_sum_data_int.sum_I[B_PHASE]);
		SUM_UI_BUF(m_Ub_buf,m_sum_data_int.sum_U[B_PHASE]);
		SUM_UI_BUF(m_Ic_buf,m_sum_data_int.sum_I[C_PHASE]);
		SUM_UI_BUF(m_Uc_buf,m_sum_data_int.sum_U[C_PHASE]);
		m_start_offset++;
	}
	if(m_ad_buf_offset>(AD_SIZE-1))    //判断一个周波是否完成
	{
		m_ad_buf_offset=0;             //重置偏移点
		measure_INTdata_to_Extdata();  //将累加数据传到外部
		/*------------------------------------------------------------------------
		计算DDS常数并送到FPGA
		为了使DDS计算独立，该部分的功率和测量中的功率分别计算，
		方便了是一个周波送一次DDS数据还是半个周波送一次。
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
		/*完成DDS操作*/
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
	PieCtrlRegs.PIEIER1.bit.INTx4 = 0;          //28335PDF第51页 ad中断 28335PDF第51页 外部扩展中断
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

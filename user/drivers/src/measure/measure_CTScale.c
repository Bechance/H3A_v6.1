/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:系统电参数测量驱动
* 创建时间:2012-02-20
* 所属公司:科荟
* 文件名称:measure_CTScale.c
* 创建人:揭成
* 功能描述:电参数测量 自动换档
***************************************************************/
#include	"measure.h"
a_lock_t*   EXTIO_lock = INVALID_LOCK;
/*------------------------------------------------------*/
// IO扩展相关操作
/*------------------------------------------------------*/
extern void EXTIO_send_Ndata(void);
extern void*EXTIO_buf_obtain(void);
extern void DSP28x_usDelay( Uint32 Count );
void EXTIOLOCK(void)
{
   os_lock(EXTIO_lock);
   return;
}
void EXTIOUNLOCK(void)
{
   os_unlock(EXTIO_lock);
   return;
}
/*------------------------------------------------------*/
// 设置相应位但不操作IO
/*------------------------------------------------------*/
void EXTIO_buff_Bitrenew(uint16_t bit_name,uint8_t Clean_or_set)
{
    Bsize_t nbit;
    Bsize_t ntem;
	Bsize_t bitstate;
	uint8_t*pdata;
    pdata=EXTIO_buf_obtain();
    ntem = bit_name/8;
	nbit = bit_name%8;
	bitstate = (1<<nbit);
	if(Clean_or_set==1)
		pdata[ntem]|=bitstate;
	else
		pdata[ntem]&=(~bitstate);
	return;
}
/*------------------------------------------------------*/
// 设置相应位并且操作I/O
/*------------------------------------------------------*/
void EXTIO_Bitrenew(uint16_t offset,uint8_t Clean_or_set)
{
    EXTIOLOCK();
    EXTIO_buff_Bitrenew(offset,Clean_or_set);
	EXTIO_send_Ndata();
    EXTIOUNLOCK();
	return;
}
/*------------------------------------------------------*/
// 扩展IO初始化
/*------------------------------------------------------*/
void EXTIO_init(void)
{
    EXTIOLOCK();
	EXTIO_buff_Bitrenew(E2PROM_CS,1);   
	EXTIO_buff_Bitrenew(TEMPER_CS,1);   
	EXTIO_buff_Bitrenew(AD_RESET,1);
	EXTIO_buff_Bitrenew(AD_STBY,1);  
	EXTIO_send_Ndata();
	msleep(1);
	EXTIO_buff_Bitrenew(AD_RESET,0);
	EXTIO_send_Ndata();
	GLOBAL_MEMCLR(m_DC_Udx,3*sizeof(int32_t));
	GLOBAL_MEMCLR(m_DC_Idx,3*sizeof(int32_t));
    EXTIOUNLOCK();
	return;
}
/********************************************************************************/
Bsize_t		  m_pll_pos 	  = 0;
const uint8_t m_pll_DATA[6][3]={{0,0,0},{0,0,1},{0,1,0},{0,1,1},{1,0,0},{1,0,1}};
/********************************************************************
 * @作者: 揭成
 * @功能: 选择锁相环信号通道
 * @输入:	
 * @输出: NONE
********************************************************************/
void measure_CTScale_select_PLL(char UI,char phase)
{
	UI=DOWNCASE(UI);//转换成小写
	if(UI=='u'&&phase=='a') 	 m_pll_pos = 0;
	else if(UI=='u'&&phase=='b') m_pll_pos = 1;
	else if(UI=='u'&&phase=='c') m_pll_pos = 2;
	else if(UI=='i'&&phase=='a') m_pll_pos = 3;
	else if(UI=='i'&&phase=='b') m_pll_pos = 4;
	else if(UI=='i'&&phase=='c') m_pll_pos = 5;
    EXTIOLOCK();
	EXTIO_buff_Bitrenew(IS_4015A,m_pll_DATA[m_pll_pos][2]);  
	EXTIO_buff_Bitrenew(IS_4015B,m_pll_DATA[m_pll_pos][1]);  
	EXTIO_buff_Bitrenew(IS_4015C,m_pll_DATA[m_pll_pos][0]);
    EXTIO_send_Ndata();
    EXTIOUNLOCK();
    return;
}
/********************************************************************
 * @作者: 揭成
 * @功能: 电流档位选择
 * @输入:	
 * @输出: NONE
********************************************************************/
void measure_CTScale_select_I_level(correct_level_arg*level)
{
    EXTIOLOCK();
	if(level->ADLSR_select==QB_N_5A
		||level->ADLSR_select==QB_N_1A
		||level->ADLSR_select==QB_N_20A)
	   EXTIO_buff_Bitrenew(CTR_IA,0);   
	else 
	   EXTIO_buff_Bitrenew(CTR_IA,1);   
	
	if(level->BDLSR_select==QB_N_5A
		||level->BDLSR_select==QB_N_1A
		||level->BDLSR_select==QB_N_20A)
	   EXTIO_buff_Bitrenew(CTR_IB,0);   
	else 
	   EXTIO_buff_Bitrenew(CTR_IB,1);   
	
	if(level->CDLSR_select==QB_N_5A
		||level->CDLSR_select==QB_N_1A
		||level->CDLSR_select==QB_N_20A)
	   EXTIO_buff_Bitrenew(CTR_IC,0);   
	else 
	   EXTIO_buff_Bitrenew(CTR_IC,1);   
	EXTIO_buff_Bitrenew(WR_IA,1);  
	EXTIO_buff_Bitrenew(WR_IB,1);  
	EXTIO_buff_Bitrenew(WR_IC,1);
	if(level->Ilevel_offset==I_INPUT_1)
	{
		EXTIO_buff_Bitrenew(AD8253_A0,0);  
		EXTIO_buff_Bitrenew(AD8253_A1,0); 
	}
	else
	{
		EXTIO_buff_Bitrenew(AD8253_A0,1);
		EXTIO_buff_Bitrenew(AD8253_A1,0);  
	}
	EXTIO_send_Ndata();
	EXTIO_buff_Bitrenew(WR_IA,0);  
	EXTIO_buff_Bitrenew(WR_IB,0);  
	EXTIO_buff_Bitrenew(WR_IC,0); 
	EXTIO_send_Ndata();
    EXTIOUNLOCK();
	return;
}
/********************************************************************
 * @作者: 揭成
 * @功能: 电压档位选择
 * @输入:	
 * @输出: NONE
********************************************************************/
void measure_CTScale_select_U_level(correct_level_arg*level)
{
    EXTIOLOCK();
	EXTIO_buff_Bitrenew(WR_UA,1);  
	EXTIO_buff_Bitrenew(WR_UB,1);  
	EXTIO_buff_Bitrenew(WR_UC,1);
	if(level->Ualevel_offset==U_INPUT_LEVEL_57V)
	{
		EXTIO_buff_Bitrenew(AD8253_A0,1);  
		EXTIO_buff_Bitrenew(AD8253_A1,1);
	}
	else if(level->Ualevel_offset==U_INPUT_LEVEL_100V)
	{
		EXTIO_buff_Bitrenew(AD8253_A0,0);  
		EXTIO_buff_Bitrenew(AD8253_A1,1);
	}
	else if(level->Ualevel_offset==U_INPUT_LEVEL_220V)
	{
		EXTIO_buff_Bitrenew(AD8253_A0,1);  
		EXTIO_buff_Bitrenew(AD8253_A1,0);
	}
	else if(level->Ualevel_offset==U_INPUT_LEVEL_440V)
	{
		EXTIO_buff_Bitrenew(AD8253_A0,0);  
		EXTIO_buff_Bitrenew(AD8253_A1,0);
	}
	EXTIO_send_Ndata();
	EXTIO_buff_Bitrenew(WR_UA,0);  
	EXTIO_buff_Bitrenew(WR_UB,0);  
	EXTIO_buff_Bitrenew(WR_UC,0); 
	EXTIO_send_Ndata();
    EXTIOUNLOCK();
	return;
}
/********************************************************************
 * @作者: 揭成
 * @功能: 换挡使用到的硬件初始化
 * @输入:	
 * @输出: NONE
********************************************************************/
void measure_CTScale_init(void)
{
	EALLOW;
    GpioCtrlRegs.GPBMUX2.bit.GPIO48    = 0; //28v电源控制脚
	GpioCtrlRegs.GPBDIR.bit.GPIO48     = 1; //
	GpioDataRegs.GPBCLEAR.bit.GPIO48   = 1; //上电设置成关闭状态
	EDIS;
	os_lock_creat(&EXTIO_lock);
    EXTIO_init();
	measure_CTScale_select_PLL('I','a'); 	//选择A相电流通道琐相
    m_correct_level_arg.Ualevel_offset = U_INPUT_LEVEL_220V;
    m_correct_level_arg.Ublevel_offset = U_INPUT_LEVEL_220V;
    m_correct_level_arg.Uclevel_offset = U_INPUT_LEVEL_220V;
    m_correct_level_arg.ADLSR_select   = DZ_N_10A;
    m_correct_level_arg.BDLSR_select   = DZ_N_10A;
    m_correct_level_arg.CDLSR_select   = DZ_N_10A;
    m_correct_level_arg.Ilevel_offset  = 0;
	measure_CTScale_select_U_level(&m_correct_level_arg);
	measure_CTScale_select_I_level(&m_correct_level_arg);
	return;
}

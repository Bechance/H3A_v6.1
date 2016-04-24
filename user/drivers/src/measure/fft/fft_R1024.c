  /******************************************
* 函 数 名: fft_R1024.c
* 功能描述: 实数1024点快速傅立叶变换
* 函数说明: 使用本函数应添加
*           1.fft_R1024.C (#INCLUDE "fft.h")  
*           2.cfft32c.asm ,3.cfft32i.asm, 4.rfft32br.asm
*           5.rfft32m.asm ,6.rfft32s.asm, 7.rfft32w.asm
*           8.sel_q.asm
* 调用函数: 
* 全局变量: long ipcb[1024+2];long mag[1024/2+1]; 
*           volatile int ii; 
* 输 入:    指针long *ch （Q17格式）
* 返 回:    幅值平方 mag[1024] (Q30格式）
* 设 计 者：肖增林 日期：2005-8-1
* 修 改 者：肖增林 日期：2005-8-1
* 版 本：   V100
*************************************/
#include 	"fft.h"
#include	"../measure.h"
#include	"IQmathLib.h"


/* Create an Instance of FFT module  */
#define     TWO_N   1024 
#pragma 	DATA_SECTION(ipcb,"FFTipcb");
#pragma 	DATA_SECTION(mag,"FFTmag");
#pragma  	DATA_SECTION (fft_data, 	"FAST_RAM")/*把变量定义在FAST_RAM*/
/******************************************************************************/
RFFT32  	fft=RFFT32_1024P_DEFAULTS;  
long    	ipcb[TWO_N+2];
long   		mag[TWO_N/2+1]; 
volatile int ii; 
float 		F_Harm[6][52];//存放0－－51次谐波占总的比率，其中Harm0[k][0]=总的谐波失真度
float       F_angle[6][52];
float 		DC_Harm[6];
uint32_t	F_A[6];
int32  	 	fft_data[TWO_N];
/******************************************************************************/
static void fft_R1024(int32 *ch)
{    
	/* Initialize FFT module   */
	fft.ipcbptr=ipcb;
	fft.magptr=mag;  
	fft.init(&fft);  

	for(ii=0;ii<1024;ii++)
	{  
		ipcb[ii]=*(ch+ii)<<14;
	}
	RFFT32_brev(ipcb,ipcb,TWO_N);
	fft.calc(&fft);
	fft.split(&fft);
	fft.mag(&fft); 
	return;
} 
/******************************************************************************/
//app_for fft
//phase 表示相位选择
/******************************************************************************/
void fft_Exc(uint16_t pos )
{
	int16*	 psrc;//FFT变换的首址
	uint16_t offset;
	float 	 fsum_mag_square=0.0;
	float 	 fsum_mag=0.0;
	float 	 fbase_wave=0.0;
	if(pos>5)      return;
	if(pos==0)	 psrc=(int16*)m_Ua_buf;
	else if(pos==1)psrc=(int16*)m_Ia_buf;
	else if(pos==2)psrc=(int16*)m_Ub_buf;
	else if(pos==3)psrc=(int16*)m_Ib_buf;
	else if(pos==4)psrc=(int16*)m_Uc_buf;
	else if(pos==5)psrc=(int16*)m_Ic_buf;
	for(offset=0;offset<TWO_N;offset++)
	{
		fft_data[offset]=(int16_t)psrc[offset];
	}
	fft_R1024(fft_data);

//  计算除基波外50次谐波平方和
	for(offset=2;offset<52;offset++)
	{
		fsum_mag_square=fsum_mag_square+_IQ30toF(mag[offset]);
	}
	fsum_mag=sqrt(fsum_mag_square); // 开方后得到谐波比重

	fbase_wave=sqrt(_IQ30toF(mag[1])); //计算基波幅值
	F_A[pos] = fbase_wave;
	if(fbase_wave!=0.0)
	{
		F_Harm[pos][0]= (fsum_mag/fbase_wave)*100.0;
	}
	else
	{
		F_Harm[pos][0]=0.0;
	}
	for(offset=1;offset<52;offset++) 
	{
		if(fbase_wave!=0.0)
		{
			F_Harm[pos][offset]=(sqrt(_IQ30toF(mag[offset]))/fbase_wave)*100.0;
		}
		else
		{
			F_Harm[pos][offset]=0.0;
		}
		F_angle[pos][offset] = atan2(_IQ30toF(ipcb[offset*2]),_IQ30toF(ipcb[offset*2+1]))/3.1415926*180 ;
	}
	DC_Harm[pos]= _IQ30toF(ipcb[0])*65536; //直流分量
	if(pos==0)
		  m_DC_Udx[A_PHASE]+=(int32_t)DC_Harm[pos];
	else if(pos==1)
		  m_DC_Idx[A_PHASE]+=(int32_t)DC_Harm[pos];
	else if(pos==2)
		  m_DC_Udx[B_PHASE]+=(int32_t)DC_Harm[pos];
	else if(pos==3)
		  m_DC_Idx[B_PHASE]+=(int32_t)DC_Harm[pos];
	else if(pos==4)
		  m_DC_Udx[C_PHASE]+=(int32_t)DC_Harm[pos];
	else if(pos==5)
		  m_DC_Idx[C_PHASE]+=(int32_t)DC_Harm[pos];
	return;
}         
void fft_init(void)
{
   GLOBAL_MEMCLR(F_angle,sizeof(float)*6*52);
   GLOBAL_MEMCLR(F_Harm,sizeof(float)*6*52);
   return;
}
/*-----------------------------------------------------------------------*/
//谐波软件治理
// pos   Ua  0    Ia  1
// pos   Ub  2    Ib  3
// pos   Uc  4    Ic  5
/*-----------------------------------------------------------------------*/

float fft_angle_obtain(int n,int pos)
{
   return F_angle[pos][n];
}
float fft_thd_obtain( int pos)
{
	return F_Harm[pos][0];
}
float fft_fftUage_obtain(int pos,int n)
{
	return F_Harm[pos][n];
}
/***************************************************************/
#define  GET_BASE_J(pos,point) ((float)offset/(float)point*360)
#define  GET_WTJ(n,pos,point)  (GET_BASE_J(pos,point)*n)
void fft_creat(int*buf,int point,float A,float angle0)
{
	float angle;
	float data = 0;
	int   offset;
	for(offset=0;offset<point;offset++)
	{
		angle = GET_BASE_J(offset,point);
		angle = angle+180-angle0;
		angle = angle/360.0*2*3.1415926;
		data = sin(angle)*A;
		buf[offset] = (int)data;
	}
	return;
}

void fft_add(int *buf,int point ,float A,float uage,float angle0,int n)
{
	float angle;
	float data = 0;
	int   offset;
	for(offset=0;offset<point;offset++)
	{
		angle = GET_WTJ(n,offset,point);
		angle = angle+180-angle0;
		angle = angle/360.0*2*3.1415926;
		data = sin(angle)*A*uage/100.0;
		buf[offset] = buf[offset]+(int)data;
	}
	return;
}

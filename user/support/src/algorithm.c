/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:算法支持
* 创建时间:2010-07-21
* 所属公司 :科荟
* 文件名称:altorithm.c
* 创建人 :揭成
* 功能描述:常用算法汇总
*----------------------------------------------------------------
修改记录:

修改日期:

修改人:
*********************************************************************************************************
*/
#include "../app_support.h"

/********************************************************************
 * @创建人 :揭成
 * @功能 :二分算法
 *
 * @输入 :head_data:待查内存首个序号，
  		  array_len :待查区域长度
          data_len:待查区域中一个单元数据大小，
 		  match_data:需要查询的匹配数据
 *
 *@输出  :(void*) 输出找到数据的位置，如果为0则表示
 * 			          没有匹配到数据
********************************************************************/
void* my_dichotomy_algorithm( Bsize_t *head_data, Bsize_t array_len,
							  Bsize_t data_len, Bsize_t match_data )
{
	Bsize_t *cur_pos ;
	Bsize_t   sum_len = array_len;
	while ( 1 )
	{
		cur_pos = ( Bsize_t* )(( Bsize_t* )head_data + ( array_len / 2 ) * data_len ) ;
		if ( *cur_pos == match_data )
		{
			return ( void* )cur_pos;
		}
		if (( !array_len ) || !( array_len / 2 ) )
		{
			break;
		}
		if (( *cur_pos < match_data ) )
		{
			head_data = cur_pos;
			array_len = sum_len - array_len / 2;
			sum_len  = array_len;
		}
		else
		{
			array_len = array_len / 2;
			sum_len  = array_len;
		}
	}
	return 0;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :排序算法
 *
 * @输入 :head_data:待查内存首个序号，
 * 			array_len :待查区域长度
 *                data_len:待查区域中一个单元数据大小，
 *
 *
 *@输出  : NONE
********************************************************************/
void my_sort_algorithm( Bsize_t *head_data, Bsize_t array_len, Bsize_t data_len )
{
	return;
}

/********************************************************************
 * @创建人 :揭成
 * @功能 :滑动滤波算法创建
 *
 * @输入 : buf :存放数据缓冲池，
 *                 buf_len:  缓冲池长度
 *                 data_type,数据类型，整形 或浮点 (0:char,1,int,2,long,4,float)
 *                 err, 	 采样误差基准(在该误差范围之内认为有效)
 *                 times ，	 基准次数 当连续采样到times同样的误差范围之内数据，认为数据需要更新了
 *                 filtering_data_ctl 算法控制
 * @输出  : NONE
********************************************************************/
void shift_filtering_data_algorithm_creat( 
		void*	buf,       //存放数据缓冲池，
		Bsize_t buf_len,   //缓冲池长度
		uint8_t data_type, //数据类型，整形 或浮点 (0:char,1,int,2,long,4,float)
		float   err,       //采样误差基准(在该误差范围之内认为有效)
		Bsize_t times,     //基准次数 当连续采样到times同样的误差范围之内数据，认为数据需要更新了
		filtering_data_t*filtering_data_ctl //算法控制组
)
{
	filtering_data_ctl->buf = buf;
	filtering_data_ctl->buf_len = buf_len;
	filtering_data_ctl->data_type = data_type;
	filtering_data_ctl->err = err;
	filtering_data_ctl->times = times;
	filtering_data_ctl->data_cnt = 0;
	filtering_data_ctl->start_pos = 0;
	filtering_data_ctl->end_pos = 0;
	filtering_data_ctl->average_data = 0;
	filtering_data_ctl->times_cnt = 0;
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :滑动滤波算法平均值计算
 * @输入 :  filtering_data_ctl  算法控制
 *                  P_indata : 得到的新值
 * @输出  : NONE
********************************************************************/
float shift_filtering_data_obtain( 
		filtering_data_t*filtering_data_ctl, //算法控制组
		float f_indata                       //得到的新值
)
{
	float*pbuf = ( float* )filtering_data_ctl->buf;
	float  f_outdata;
	short offset;
	if ( filtering_data_ctl->data_cnt == filtering_data_ctl->buf_len )
	{
		f_outdata = pbuf[filtering_data_ctl->start_pos];
		if ( GLOBAL_ABS(( f_indata - filtering_data_ctl->average_data ) ) < filtering_data_ctl->err )
		{
			filtering_data_ctl->times_cnt 		= 0;
			pbuf[filtering_data_ctl->start_pos] = f_indata;
			(( filtering_data_ctl->start_pos + 1 ) >= filtering_data_ctl->buf_len )
			? ( filtering_data_ctl->start_pos = 0 )
			: ( filtering_data_ctl->start_pos++ );
			(( filtering_data_ctl->end_pos + 1 )>= filtering_data_ctl->buf_len )
			? ( filtering_data_ctl->end_pos = 0 )
			: ( filtering_data_ctl->end_pos++ );
			filtering_data_ctl->average_data
			= filtering_data_ctl->average_data + ( f_indata - f_outdata ) / filtering_data_ctl->data_cnt;
		}
		else  if ( ++( filtering_data_ctl->times_cnt ) > filtering_data_ctl->times )
		{
			filtering_data_ctl->data_cnt = 0;
			filtering_data_ctl->start_pos = 0;
			filtering_data_ctl->end_pos = 0;
			filtering_data_ctl->average_data = f_indata;
			filtering_data_ctl->times_cnt = 0;
		}
	}
	else
	{
		if ( GLOBAL_ABS(( f_indata - filtering_data_ctl->average_data ) ) < filtering_data_ctl->err )
		{
			pbuf[filtering_data_ctl->end_pos] = f_indata;
			filtering_data_ctl->data_cnt++;
			filtering_data_ctl->end_pos++;
			filtering_data_ctl->average_data = 0;
			for ( offset = 0;offset < filtering_data_ctl->data_cnt;offset++ )
			{
				filtering_data_ctl->average_data += pbuf[offset];
			}
			filtering_data_ctl->average_data
			= filtering_data_ctl->average_data / filtering_data_ctl->data_cnt;
		}
		else  if ( ++( filtering_data_ctl->times_cnt ) > filtering_data_ctl->times )
		{
			filtering_data_ctl->data_cnt = 0;
			filtering_data_ctl->start_pos = 0;
			filtering_data_ctl->end_pos = 0;
			filtering_data_ctl->average_data = f_indata;
			filtering_data_ctl->times_cnt = 0;
		}
	}
	return filtering_data_ctl->average_data;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :积分数字滤波算法创建
 * @输入 :integral_ctl : 算法控制
 *        f_indata     : 输入的新值
 * @输出 :当前平均值
********************************************************************/
void integral_average_create( integral_average_t*integral_ctl,float f_correct)
{
    integral_ctl->average_data = 0;
    integral_ctl->sum		   = 0;
    integral_ctl->f_correct	   = f_correct;
    integral_ctl->times		   = 0;
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :积分数字滤波平均值计算
 * @输入 :integral_ctl : 算法控制
 *        f_indata     : 输入的新值
 * @输出 :当前平均值
 * 说明:只适合正同向平均，就是输入是同一个方向的，要不都是正，要不都是负。
********************************************************************/
float integral_average_data_obtain( 
		integral_average_t*integral_ctl, //算法控制组
		float f_indata                   //得到的新值
)
{
#define  IABS(val) (((val)>0)?((val)):(-(val)))
    float fdx;
	float fmax = 1073741823;
	if(f_indata<0&&integral_ctl->sum>0)
	{
		integral_ctl->average_data = f_indata;
		integral_ctl->sum		   = 0;
		integral_ctl->times		   = 0;
		return f_indata;
	}
	if(f_indata>0&&integral_ctl->sum<0)
	{
		integral_ctl->average_data = f_indata;
		integral_ctl->sum		   = 0;
		integral_ctl->times		   = 0;
		return f_indata;
	}
	fdx = f_indata-integral_ctl->average_data;
    if(IABS(fdx)>integral_ctl->f_correct)
    {
		integral_ctl->average_data = f_indata;
		integral_ctl->sum		   = 0;
		integral_ctl->times		   = 0;
		return f_indata;
	}
    if(f_indata>0)
    { 
        fdx = fmax-integral_ctl->sum; 
        if(fdx<f_indata)
        {
			integral_ctl->average_data = f_indata;
			integral_ctl->sum		   = 0;
			integral_ctl->times		   = 0;
			return f_indata;
		}
	}
    else if(f_indata<0)
    {
        fdx = fmax-IABS(integral_ctl->sum); 
        if(fdx<IABS(f_indata))
        {
			integral_ctl->average_data = f_indata;
			integral_ctl->sum		   = 0;
			integral_ctl->times		   = 0;
			return f_indata;
		}
	}
    integral_ctl->sum+=f_indata;
    integral_ctl->times++;
    integral_ctl->average_data = integral_ctl->sum/(float)integral_ctl->times;
	return integral_ctl->average_data;
}

/********************************************************************
 * @创建人 :揭成
 * @功能 :均值平均值算法创建
 * @输入 :integral_ctl : 算法控制
 *        f_indata     : 输入的新值
 * @输出 :当前平均值
********************************************************************/
void period_average_create( period_average_t*period_ctl,float period_times)
{
    period_ctl->average_data	= 0;
    period_ctl->sum		  	 	= 0;
	period_ctl->cur_times       = 0;
    period_ctl->times		   	= period_times;
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :均值平均值计算
 * @输入 :period_ctl   : 算法控制
 *        f_indata     : 输入的新值
          ack          : 检测标志，输出1 表示有新的平均值，为0 表示为上一次平均值。
 * @输出 :当前平均值
 * 说明:只适合正同向平均，就是输入是同一个方向的，要不都是正，要不都是负。
********************************************************************/
float period_average_data_obtain( 
		period_average_t*period_ctl,     //算法控制组
		float f_indata,                  //得到的新值
		int*ack	                         //是否得到新的平均值
)
{
    int   ret = 0;
    period_ctl->sum+=f_indata;
	period_ctl->cur_times++;
    if(period_ctl->cur_times == period_ctl->times)
    {
        period_ctl->average_data = period_ctl->sum/period_ctl->times;
		period_ctl->cur_times = 0;
        period_ctl->sum = 0;
		ret = 1;
	}
    if(ack)
    {    
       *ack = ret;
	}
    return period_ctl->average_data;
}

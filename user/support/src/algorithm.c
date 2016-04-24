/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:�㷨֧��
* ����ʱ��:2010-07-21
* ������˾ :����
* �ļ�����:altorithm.c
* ������ :�ҳ�
* ��������:�����㷨����
*----------------------------------------------------------------
�޸ļ�¼:

�޸�����:

�޸���:
*********************************************************************************************************
*/
#include "../app_support.h"

/********************************************************************
 * @������ :�ҳ�
 * @���� :�����㷨
 *
 * @���� :head_data:�����ڴ��׸���ţ�
  		  array_len :�������򳤶�
          data_len:����������һ����Ԫ���ݴ�С��
 		  match_data:��Ҫ��ѯ��ƥ������
 *
 *@���  :(void*) ����ҵ����ݵ�λ�ã����Ϊ0���ʾ
 * 			          û��ƥ�䵽����
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
 * @������ :�ҳ�
 * @���� :�����㷨
 *
 * @���� :head_data:�����ڴ��׸���ţ�
 * 			array_len :�������򳤶�
 *                data_len:����������һ����Ԫ���ݴ�С��
 *
 *
 *@���  : NONE
********************************************************************/
void my_sort_algorithm( Bsize_t *head_data, Bsize_t array_len, Bsize_t data_len )
{
	return;
}

/********************************************************************
 * @������ :�ҳ�
 * @���� :�����˲��㷨����
 *
 * @���� : buf :������ݻ���أ�
 *                 buf_len:  ����س���
 *                 data_type,�������ͣ����� �򸡵� (0:char,1,int,2,long,4,float)
 *                 err, 	 ��������׼(�ڸ���Χ֮����Ϊ��Ч)
 *                 times ��	 ��׼���� ������������timesͬ������Χ֮�����ݣ���Ϊ������Ҫ������
 *                 filtering_data_ctl �㷨����
 * @���  : NONE
********************************************************************/
void shift_filtering_data_algorithm_creat( 
		void*	buf,       //������ݻ���أ�
		Bsize_t buf_len,   //����س���
		uint8_t data_type, //�������ͣ����� �򸡵� (0:char,1,int,2,long,4,float)
		float   err,       //��������׼(�ڸ���Χ֮����Ϊ��Ч)
		Bsize_t times,     //��׼���� ������������timesͬ������Χ֮�����ݣ���Ϊ������Ҫ������
		filtering_data_t*filtering_data_ctl //�㷨������
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
 * @������ :�ҳ�
 * @���� :�����˲��㷨ƽ��ֵ����
 * @���� :  filtering_data_ctl  �㷨����
 *                  P_indata : �õ�����ֵ
 * @���  : NONE
********************************************************************/
float shift_filtering_data_obtain( 
		filtering_data_t*filtering_data_ctl, //�㷨������
		float f_indata                       //�õ�����ֵ
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
 * @������ :�ҳ�
 * @���� :���������˲��㷨����
 * @���� :integral_ctl : �㷨����
 *        f_indata     : �������ֵ
 * @��� :��ǰƽ��ֵ
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
 * @������ :�ҳ�
 * @���� :���������˲�ƽ��ֵ����
 * @���� :integral_ctl : �㷨����
 *        f_indata     : �������ֵ
 * @��� :��ǰƽ��ֵ
 * ˵��:ֻ�ʺ���ͬ��ƽ��������������ͬһ������ģ�Ҫ����������Ҫ�����Ǹ���
********************************************************************/
float integral_average_data_obtain( 
		integral_average_t*integral_ctl, //�㷨������
		float f_indata                   //�õ�����ֵ
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
 * @������ :�ҳ�
 * @���� :��ֵƽ��ֵ�㷨����
 * @���� :integral_ctl : �㷨����
 *        f_indata     : �������ֵ
 * @��� :��ǰƽ��ֵ
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
 * @������ :�ҳ�
 * @���� :��ֵƽ��ֵ����
 * @���� :period_ctl   : �㷨����
 *        f_indata     : �������ֵ
          ack          : ����־�����1 ��ʾ���µ�ƽ��ֵ��Ϊ0 ��ʾΪ��һ��ƽ��ֵ��
 * @��� :��ǰƽ��ֵ
 * ˵��:ֻ�ʺ���ͬ��ƽ��������������ͬһ������ģ�Ҫ����������Ҫ�����Ǹ���
********************************************************************/
float period_average_data_obtain( 
		period_average_t*period_ctl,     //�㷨������
		float f_indata,                  //�õ�����ֵ
		int*ack	                         //�Ƿ�õ��µ�ƽ��ֵ
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

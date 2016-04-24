#ifndef _PEC_2010_ALGORITHM_H
#define  _PEC_2010_ALGORITHM_H
typedef  BOOL ( *match_func_t )( void*arg );

void* 	my_dichotomy_algorithm( Bsize_t *head_data, Bsize_t array_len, Bsize_t data_len, Bsize_t match_data );
void 	test_algorithm( Bsize_t data );
typedef struct
{
	void *	buf;
	Bsize_t buf_len;
	uint8_t data_type;
	Bsize_t times;
	Bsize_t times_cnt;
	Bsize_t data_cnt;
	float   err;
	Bsize_t start_pos;
	Bsize_t end_pos;
	float 	average_data;
}filtering_data_t;


typedef struct
{
    uint32_t	times;
	float       f_correct;
	float       sum;
	float 		average_data;
}integral_average_t;

typedef struct
{
    uint32_t	times;
    uint32_t	cur_times;
	float       sum;
	float 		average_data;
}period_average_t;



void 	shift_filtering_data_algorithm_creat( void*buf, Bsize_t buf_len, uint8_t data_type, float err, Bsize_t times, filtering_data_t*filtering_data_ctl );
float 	shift_filtering_data_obtain( filtering_data_t*filtering_data_ctl, float f_indata );
void 	integral_average_create( integral_average_t*integral_ctl,float f_correct);
float   integral_average_data_obtain( integral_average_t*integral_ctl,float f_indata);
void 	period_average_create( period_average_t*period_ctl,float period_times);
float 	period_average_data_obtain( period_average_t*integral_ctl,float f_indata,int*ack);
#endif

#ifndef  _PEC_VOICE_BUZ       
#define  _PEC_VOICE_BUZ
typedef struct
{
	Bsize_t ctl_times;
	Bsize_t alarm_time;
	Bsize_t stop_time;
}buz_ctl_t;

void 			app_global_buz_alarm( Bsize_t stop_jiffies, Bsize_t alarm_jiffies, Bsize_t cycle_times );
void 			app_global_buz_response( Bsize_t response_jiffies );
void 			app_global_buz_entery( void );
void 			app_global_buz_exitmod( void );
#define         buz_response(val) 	{app_global_buz_response(val);}
#endif

#ifndef _PEC_TIMER_ARRAY_LINK_LIST_2010_H
#define  _PEC_TIMER_ARRAY_LINK_LIST_2010_H
#include "../app_support.h"
#if TIMER_ARRAY_EN>0

typedef void( * tmr_callblack )( void*p_arg );
typedef struct
{
	tmr_callblack     function;
	uint32_t          expires;
	Bsize_t  		  tmr_state;
	Bsize_t           type;
	uint32_t          auto_load_expires;
	void*	          p_arg;
}tmr_member_t;

#define MAX_TMR_NUM      15
typedef struct
{  
	Bsize_t ture_size;
	tmr_member_t tmr_array[MAX_TMR_NUM];
}tmr_array_t;



#define TIMER_AUTO    	 1
#define TIMER_SETTINGS   2

enum
{
	TMR_IDLE = 0,
	TMR_HAND_UP_REQ,
	TMR_HAND_UP_STATE,
	TMR_REVERT_REQ ,
	TMR_IS_OK,
	TMR_DELETE_REQ
};
void 			timer_member_mod_expires( tmr_member_t*tmr_member, uint32_t over_time );

void 			timer_jiffies_add( void );

tmr_member_t*	timer_member_create( uint32_t need_jiffies, Bsize_t type, tmr_callblack caller, void*p_arg );

void			timer_member_state_set( tmr_member_t*tmr, Bsize_t state );

void 			timer_array_mod_init( void );
#endif
#endif

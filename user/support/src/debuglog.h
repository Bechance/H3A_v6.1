#include "../app_support.h"
#ifndef _2010_DEBUGLOG_H_
#define  _2010_DEBUGLOG_H_

#if  DEBUG_LOG_EN   >0
#define DEBUG_MSG_EN      1
#define DEBUG_ERR_EN      1


#define LOG_MSG_BOX_SIZE 	20
#define LOG_MSG_BUF_LEN     800
typedef struct
{
	char  pram_buf[4*6];
	uint8_t cur_off;
	void *parm_0;
	void *parm_1;
	void *parm_2;
	void *parm_3;
	void *parm_4;
	void *parm_5;
	void *end_pram;
}log_param;

void 			log_printf( char *fmt, log_param*param );

void 			log_debug_insmod( void );

void 			log_debug_mod_exit( void );

Bsize_t 			log_empty_read_fifo( void );

Bsize_t 			log_wait_input_msg( uint8_t*buf );


extern      		log_param     			m_log_pram;
extern      		uint8_t         			m_null_pram;
extern 			int     				m_log_line;
extern			char*         			m_log_pfile;
#define 			LOG_NP       			m_null_pram

#define   		LOGMSG(A,B,C) {	m_log_pram.parm_0 = ( void* )(&(B));\
		m_log_pram.parm_1 =  ( void* )(&(C));\
		log_printf(A,&m_log_pram);}//特别注意 ，中断当中不要使用

#define   	 	DBGMSG(A,B,C)    	LOGMSG(A,B,C)
#define 	       	USER_LOG(A)  		DBGMSG(A,LOG_NP,LOG_NP);


#define   		DEBUGLINE()   {\
		m_log_line =__LINE__;\
		m_log_pfile =__FILE__;\
		LOGMSG("<FILE :%s--LINE :%d > Describe:",m_log_pfile[0],m_log_line);}
#define   		DBGERR(A,B,C)  {DBGMSG("\n#ERR ==> ",LOG_NP,LOG_NP);DEBUGLINE();DBGMSG(A,B,C);}

#else
#define 			LOG_NP       0
#define 	       	USER_LOG(A)
#define   		DBGMSG(A,B,C)
#define   		DBGERR(A,B,C)
#endif

#endif

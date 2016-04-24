#ifndef _APP_UARTMOD_MANUALJB_UPLOAD_H_
#define _APP_UARTMOD_MANUALJB_UPLOAD_H_

#include "app_BLUETOOTHmod_inside.h"


extern uint8_t		BT_current_meter_number;
extern uint16_t		BT_total_record_counts;


#define TOTAL_MEMBERS		60
#define	MANUALJB_SAVED_FILE_NAME "errDATA"
#define UARTmod_DL_DSIZE	10
#define UARTMOD_MAX_MEMBERS 23

extern void 	app_BLUETOOTHmod_uart_send_by512(const uint8_t* psrc, const uint16_t size);
extern uint8_t 	app_BLUETOOTHmod_cal_mod256_sum(const uint8_t *buf,const uint16_t size);
extern void		app_BLUETOOTHmod_err_echo(uint8_t *m_recv_buf, const uint8_t err_code);
extern char* 	app_BLUETOOTHmod_search_string(const char *pstart,	const char *psearch,const uint16_t size);
extern void 	app_BLUETOOTHmod_window_refresh_comm_stat(uint8_t flag);
#endif

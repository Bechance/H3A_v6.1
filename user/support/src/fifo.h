#ifndef _PEC_FIFO_H_2010
#define  _PEC_FIFO_H_2010
#include "../app_support.h"
#if COMPILER_MALLOC_SUPPORT >0

typedef struct _fifo_tcb_t
{
	Bsize_t 		start_pos;
	Bsize_t 		end_pos;
	Bsize_t		buffer_size;
	unsigned char buffer_ptr[1];
} fifo_tcb_t;

fifo_tcb_t * 	fifo_buffer_init( Bsize_t buffer_size );

void 		fifo_buffer_destroy( fifo_tcb_t * * self_ptr );

void			fifo_buffer_push( fifo_tcb_t * self, unsigned char*data, Bsize_t size );

void  	 	fifo_buffer_pop( fifo_tcb_t * self, unsigned char*data, Bsize_t size );

Bsize_t 	 	fifo_buffer_get_size( fifo_tcb_t * self );

#else
typedef struct
{
	Bsize_t 		  start_pos;
	Bsize_t 		  end_pos;
	Bsize_t		  buffer_size;
	unsigned char *buffer_ptr;
} fifo_tcb_t;

void  		fifo_buffer_init( fifo_tcb_t*self, Bsize_t buffer_size, uint8_t *buf_ptr );

void		fifo_buffer_push( fifo_tcb_t * self, unsigned char*data, Bsize_t size );

void  	 	fifo_buffer_pop( fifo_tcb_t * self, unsigned char*data, Bsize_t size );

Bsize_t 	fifo_buffer_get_size( fifo_tcb_t * self );

#endif
void 		fifo_buffer_empty( fifo_tcb_t * self );
#endif


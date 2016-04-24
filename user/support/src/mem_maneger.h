#include "../app_support.h"
#ifndef  _2010_PEC_MEM_MANEGER_H_
#define _2010_PEC_MEM_MANEGER_H_
void 		app_free( void*pmem );
void *		app_malloc( uint32_t mem_size );
void 		mem_maneger_init( void );
void 		app_release_all_mem( void );
#endif

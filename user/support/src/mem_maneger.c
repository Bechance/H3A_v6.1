#include "../app_support.h"
#if MEM_MANEGER_EN>0
void*GUI_malloc(uint32_t mem_size);
void GUI_free(void*pmem);
void GUI_mem_maneger_init( void );

void*app_malloc( uint32_t mem_size )
{
     return GUI_malloc(mem_size);
}
void app_free( void*pmem )
{
     GUI_free(pmem);
	 return;
}
void mem_maneger_init(void)
{
   GUI_mem_maneger_init();
   return;
}
void app_release_all_mem( void )
{
    
}
#endif

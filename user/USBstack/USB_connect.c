#include "USB_connect.h"
#include "../includes.h"
#include "src/USBfs.c"


uint8_t  	m_USB_task_pro;
a_lock_t * 	m_fs_lock = INVALID_LOCK;
void 		USB_main_task( void*arg );



void FS_task_delete_req( void )
{
	OS_DELETE_REQ_SELF();
	return;
}
void fs_lock( void )
{
	os_lock( m_fs_lock );
	return;
}
void fs_unlock( void )
{
	os_unlock( m_fs_lock );
	return;
}
void FS_req( void )
{
	USBfs_init();
	os_lock_creat( &m_fs_lock );
	/*m_USB_task_pro
	= os_task_assign_stk_build( USB_main_task, 3 );*/
	return;
}
void FS_release( void )
{
//	OS_DELETE_REQ( m_USB_task_pro );
	return;
}

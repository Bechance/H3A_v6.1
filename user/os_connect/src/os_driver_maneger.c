/**********************************************************************************************************
* 模块名称:驱动管理模块
* 创建时间:2010-06-31
* 所属公司 :科荟
* 文件名称:drivermaneger.C
* 创建人 :揭成
* 功能描述:驱动挂载管理
*----------------------------------------------------------------
修改记录:增加驱动卸载接口

修改日期: 2010-7-26

修改人: 揭成

*********************************************************************************************************
修改记录:增加ID !=0判断

修改日期: 2010-7-29

修改人:揭成

*********************************************************************************************************
修改记录:增加驱动底层锁接口

修改日期: 2010-8-19

修改人:揭成

*********************************************************************************************************
修改记录:为驱动增加名称

修改日期: 2010-9-07

修改人:揭成

*********************************************************************************************************
*/
#include        "../os_connect.h"
os_dirver_maneger_t   m_driver_manege_tab;
a_lock_t *            m_driver_manege_lock = INVALID_LOCK;
#define               DR_LOCK()        //  {;} //os_lock( m_driver_manege_lock )
#define               DR_UNLOCK()       // {;} //os_unlock( m_driver_manege_lock )
/********************************************************************
 * @创建人:揭成
 * @功能:添加一个驱动到驱动列表
 *
 * @输入:驱动各功能函数
 *
 * @输出:添加到的驱动列表号
********************************************************************/
dev_id_t os_driver_add( os_dirver_operate_t *driver_operate, char*pname )
{
	uint8_t index;
	DR_LOCK();
	for ( index = 0;index < MAX_SURPPORT_DRIVER;index++ )
	{
		if ( !m_driver_manege_tab.os_driver_member[index].id )
		{
			m_driver_manege_tab.os_driver_member[index].os_driver_init
			= driver_operate->os_driver_init;
			m_driver_manege_tab.os_driver_member[index].os_driver_ioctl
			= driver_operate->os_driver_ioctl;
			m_driver_manege_tab.os_driver_member[index].os_driver_write
			= driver_operate->os_driver_write;
			m_driver_manege_tab.os_driver_member[index].os_driver_read
			= driver_operate->os_driver_read;
			m_driver_manege_tab.os_driver_member[index].os_driver_uninstall
			= driver_operate->os_driver_uninstall;
			m_driver_manege_tab.os_driver_member[index].id 	= index + 1;
			m_driver_manege_tab.os_driver_member[index].dev_STATE = FAIL;
#if DRIVER_NAM_SIZE >0
			os_memcpy(( uint8_t* )m_driver_manege_tab.os_driver_member[index].os_driver_name,
						  ( uint8_t* )pname,
						  ( DRIVER_NAM_SIZE - 1 ) );
#endif
			m_driver_manege_tab.real_size++;
			DR_UNLOCK();
			return ( index + 1 );
		}
	}
	DR_UNLOCK();
	return INVALID_DEV_ID;
}

/********************************************************************
 * @创建人 :揭成
 * @功能:从列表删除一个函数
 *
 * @输入:要删除驱动的列表号
 *
 *@输出:NONE
********************************************************************/
void os_driver_delete( dev_id_t dev_id )
{
	DR_LOCK();
	if ( m_driver_manege_tab.os_driver_member[dev_id-1].id != 0 )
	{
		os_memset(( uint8_t* )&m_driver_manege_tab.os_driver_member[dev_id-1],
					  0, sizeof( os_dirver_member_t ) );
	}
	m_driver_manege_tab.real_size--;
	DR_UNLOCK();
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能:卸载一个驱动
 *
 * @输入:要卸载驱动的设备号
 *
 *@输出:NONE
********************************************************************/
void os_driver_uninstall( dev_id_t id )
{
	DR_LOCK();
	if (( m_driver_manege_tab.os_driver_member[id-1].id != 0 ) && ( id != 0 ) )
	{
	    m_driver_manege_tab.os_driver_member[id-1].dev_STATE = FAIL;
		m_driver_manege_tab.os_driver_member[id-1].os_driver_uninstall();
	}
	DR_UNLOCK();
	os_driver_delete(id);
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能:初始化所有驱动
 *
 * @输入:NONE
 *
 *@输出:NONE
********************************************************************/
void os_driver_init( dev_id_t id )
{
	DR_LOCK();
	if (( m_driver_manege_tab.os_driver_member[id-1].id != 0 ) && ( id != 0 ) )
	{
	    if(!m_driver_manege_tab.os_driver_member[id-1].dev_STATE)
	    {
		    m_driver_manege_tab.os_driver_member[id-1].dev_STATE = TURE;
			m_driver_manege_tab.os_driver_member[id-1].os_driver_init();
	    }

	}
	DR_UNLOCK();
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能:读底层数据
 *
 * @输入:
 *
 *@输出:
********************************************************************/
Bsize_t os_driver_read( dev_id_t id, uint8_t*buf, Bsize_t size, void*pos )
{
    Bsize_t rize;
	DR_LOCK();
	if (( m_driver_manege_tab.os_driver_member[id-1].id != 0 ) && ( id != 0 ) )
	{
        rize = m_driver_manege_tab.os_driver_member[id-1].os_driver_read( buf, size, pos );
		DR_UNLOCK();
		return rize;
	}
	DR_UNLOCK();
	return 0;
}
/********************************************************************
 * @创建人:揭成
 * @功能:写底层数据
 *
 * @输入:
 *
 * @输出:
********************************************************************/
Bsize_t os_driver_write( dev_id_t id, uint8_t*buf, Bsize_t size, void*pos )
{
    Bsize_t rize;
	DR_LOCK();
	if (( m_driver_manege_tab.os_driver_member[id-1].id != 0 ) && ( id != 0 ) )
	{
        rize = m_driver_manege_tab.os_driver_member[id-1].os_driver_write( buf, size, pos );
		DR_UNLOCK();
		return rize;
	}
	DR_UNLOCK();
	return 0;
}
/********************************************************************
 * @创建人 :揭成
 * @功能:写底层数据
 *
 * @输入:
 *
 *@输出:
********************************************************************/
void  os_driver_ioctl( dev_id_t id, uint8_t cmd, void*arg )
{
	DR_LOCK();
	if (( m_driver_manege_tab.os_driver_member[id-1].id != 0 ) && ( id != 0 ) )
	{
		m_driver_manege_tab.os_driver_member[id-1].os_driver_ioctl( cmd, arg );
	}
	DR_UNLOCK();
	return ;
}
/********************************************************************
 * @创建人:揭成
 * @功能:打开驱动
 * @输入:
 * @输出:
********************************************************************/
dev_id_t os_driver_open(const char*name)
{
	uint8_t index;
	os_dirver_member_t*p;
	DR_LOCK();
	for ( index = 0;index < MAX_SURPPORT_DRIVER;index++ )
	{
		p = &m_driver_manege_tab.os_driver_member[index];
		if (p->id)
		{
		    if(!os_memcmp(p->os_driver_name,(char*)name,os_strlen((char*)name)))
				continue;
			DR_UNLOCK();
			os_driver_init(index + 1);
			return ( index + 1 );
		}
	}
	DR_UNLOCK();
	return 0;
}
/********************************************************************
 * @创建人:揭成
 * @功能:关闭驱动
 * @输入:
 * @输出:
********************************************************************/
void os_driver_close(dev_id_t id)
{
	DR_LOCK();
	if (( m_driver_manege_tab.os_driver_member[id-1].id != 0 ) && ( id != 0 ) )
	{
	    m_driver_manege_tab.os_driver_member[id-1].dev_STATE = FAIL;
		m_driver_manege_tab.os_driver_member[id-1].os_driver_uninstall();
	}
	DR_UNLOCK();
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能: 驱动列表初始化
 *
 * @输入:
 *
 * @输出:
********************************************************************/
void os_driver_maneger_tab_init( void )
{
	os_memset(( uint8_t* )&m_driver_manege_tab, 0, sizeof( os_dirver_maneger_t ) );
	os_lock_creat( &m_driver_manege_lock );
	return;
}

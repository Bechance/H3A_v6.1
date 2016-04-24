/**********************************************************************************************************
* ģ������:��������ģ��
* ����ʱ��:2010-06-31
* ������˾ :����
* �ļ�����:drivermaneger.C
* ������ :�ҳ�
* ��������:�������ع���
*----------------------------------------------------------------
�޸ļ�¼:��������ж�ؽӿ�

�޸�����: 2010-7-26

�޸���: �ҳ�

*********************************************************************************************************
�޸ļ�¼:����ID !=0�ж�

�޸�����: 2010-7-29

�޸���:�ҳ�

*********************************************************************************************************
�޸ļ�¼:���������ײ����ӿ�

�޸�����: 2010-8-19

�޸���:�ҳ�

*********************************************************************************************************
�޸ļ�¼:Ϊ������������

�޸�����: 2010-9-07

�޸���:�ҳ�

*********************************************************************************************************
*/
#include        "../os_connect.h"
os_dirver_maneger_t   m_driver_manege_tab;
a_lock_t *            m_driver_manege_lock = INVALID_LOCK;
#define               DR_LOCK()        //  {;} //os_lock( m_driver_manege_lock )
#define               DR_UNLOCK()       // {;} //os_unlock( m_driver_manege_lock )
/********************************************************************
 * @������:�ҳ�
 * @����:���һ�������������б�
 *
 * @����:���������ܺ���
 *
 * @���:��ӵ��������б��
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
 * @������ :�ҳ�
 * @����:���б�ɾ��һ������
 *
 * @����:Ҫɾ���������б��
 *
 *@���:NONE
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
 * @������ :�ҳ�
 * @����:ж��һ������
 *
 * @����:Ҫж���������豸��
 *
 *@���:NONE
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
 * @������ :�ҳ�
 * @����:��ʼ����������
 *
 * @����:NONE
 *
 *@���:NONE
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
 * @������ :�ҳ�
 * @����:���ײ�����
 *
 * @����:
 *
 *@���:
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
 * @������:�ҳ�
 * @����:д�ײ�����
 *
 * @����:
 *
 * @���:
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
 * @������ :�ҳ�
 * @����:д�ײ�����
 *
 * @����:
 *
 *@���:
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
 * @������:�ҳ�
 * @����:������
 * @����:
 * @���:
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
 * @������:�ҳ�
 * @����:�ر�����
 * @����:
 * @���:
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
 * @������ :�ҳ�
 * @����: �����б��ʼ��
 *
 * @����:
 *
 * @���:
********************************************************************/
void os_driver_maneger_tab_init( void )
{
	os_memset(( uint8_t* )&m_driver_manege_tab, 0, sizeof( os_dirver_maneger_t ) );
	os_lock_creat( &m_driver_manege_lock );
	return;
}

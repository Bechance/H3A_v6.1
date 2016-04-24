/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:��̬�������ģ��
* ����ʱ��:2010-07-21
* ������˾ :����
* �ļ�����:link_list_maneger.c
* ������ :�ҳ�
* ��������: �ṩ��̬����һЩAPI���ܣ�˫������
*----------------------------------------------------------------
�޸ļ�¼:

�޸�����:

�޸���:

*********************************************************************************************************
�벻Ҫ���ж��е��ø�ģ��API�ӿ�
*/
#include "../app_support.h"
#if (LINK_LIST_MANEGER_EN >0)
link_list_maneger_t  	g_link_list_maneger_tcb = {0, };
/*��̬����������*/
a_lock_t*m_link_list_lock = INVALID_SEM;

void link_list_lock( void )
{
	os_lock( m_link_list_lock );
	return;
}
void link_list_unlock( void )
{
	os_unlock( m_link_list_lock );
	return;
}
void link_list_lock_creat( void )
{
	os_lock_creat( &m_link_list_lock );
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @����:����һ����̬������ƿ�
 *
 * @����:void
 *
 *@���: ���ƿ�λ��
********************************************************************/
link_list_tcb_t*  link_list_tcb_req( void )
{
	Bsize_t index;
	link_list_tcb_t *lptr;
	link_list_lock();
	for ( index = 0;index < MAX_LINK_LIST_SIZE;index++ )
	{
		if ( g_link_list_maneger_tcb.link_list_member[index].id )
		{
			continue;
		}
		lptr = &g_link_list_maneger_tcb.link_list_member[index];
		lptr->id = BUSY;
		lptr->link_list_end = 0;
		lptr->link_list_head = 0;
		g_link_list_maneger_tcb.ture_size++;
		link_list_unlock();
		return lptr;
	}
	link_list_unlock();
	return 0;
}
/********************************************************************
 * @������ :�ҳ�
 * @����:�ͷ�������ƿ�
 *
 * @����:lptr ���ƿ�λ��
 *
 *@���: TURE �ɹ� FAIL ʧ��
********************************************************************/
BOOL link_list_tcb_delete( link_list_tcb_t*lptr )
{
	Bsize_t index;
	for ( index = 0;index < MAX_LINK_LIST_SIZE;index++ )
	{
		if ( &( g_link_list_maneger_tcb.link_list_member[index] ) != lptr )
		{
			continue;
		}
		lptr->id = IDLE;
		lptr->link_list_end = 0;
		lptr->link_list_head = 0;
		g_link_list_maneger_tcb.ture_size--;
		return TURE;
	}
	return FAIL;
}

link_list_t  m_link_member[MAX_LINK_MEMBER_SIZE];
link_list_t*m_plink_free = PNULL;
/********************************************************************
 * @������: �ҳ�
 * @����: ����һ������ؼ�
 *
 * @����:NONE

 * @���:��Աλ��
********************************************************************/
static link_list_t *link_list_member_obtain( void )
{
	link_list_t *pret;
	if ( m_plink_free->next_member )
	{
		pret = m_plink_free;
		m_plink_free = m_plink_free->next_member;
	}
	else
	{
		pret = PNULL;
	}
	return pret;
}
static void link_list_member_free( link_list_t*pmem )
{
	link_list_t *ptmp;
	ptmp = m_plink_free;
	m_plink_free = pmem;
	m_plink_free->next_member = ptmp;
	return;
}
static void link_list_member_init( void )
{
	Bsize_t offset = 0;
	link_list_t*plist = &m_link_member[0];
	link_list_t *pnext;
	m_plink_free = plist;
	for ( offset = 0;offset < MAX_LINK_MEMBER_SIZE - 1;offset++ )
	{
		pnext = plist + 1;
		plist->next_member = pnext;
		plist = pnext;
	}
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @����: Ϊ��������ռ䣬
 *
 * @����: alloc_size ������������С
 *
 *@���:  ���ƿ�λ��
********************************************************************/
static link_list_t*  link_list_alloc_space( Bsize_t alloc_size )
{
	void*lptr = 0;
	link_list_t*list_member = 0;
	lptr = ( void* )app_malloc( alloc_size );
	list_member = link_list_member_obtain();
	list_member->cur_member = lptr;
	return ( lptr && list_member ) ? list_member : 0;
}
/********************************************************************
 * @������ :�ҳ�
 * @����: �ͷ�����ռ�
 *
 * @����:  list_member :��������ĳ�Ա
 *
 *@���:  TURE OR FAIL
********************************************************************/
static BOOL link_list_free_space( link_list_t*list_member )
{
	app_free( list_member->cur_member );
	link_list_member_free( list_member );
	return TURE;
}

/********************************************************************
 * @������ :�ҳ�
 * @����: �����������һ����Ա
 *
 * @����: link_list_tcb ���� ����Ŀ��ƿ�cur_list_member ��Ҫ��ӵĳ�Ա
 *                link_direction ��ӷ���
 *
 *@���:  NONE
********************************************************************/
static void link_list_add_member( link_list_tcb_t*link_list_tcb,
								  link_list_t *cur_list_member,
								  Bsize_t  link_direction )
{
	if ( link_direction == LINK_ADD_END )
	{
		cur_list_member->next_member = 0;
		cur_list_member ->last_member = link_list_tcb->link_list_end;
		( link_list_tcb->link_list_end )->next_member =  cur_list_member;
		( link_list_tcb->link_list_end ) =  cur_list_member;
		if ( !link_list_tcb->link_list_head )
			link_list_tcb->link_list_head = cur_list_member;
	}
	else if ( link_direction == LINK_ADD_HEAD )
	{
		cur_list_member->last_member = 0;
		cur_list_member ->next_member = link_list_tcb->link_list_head;
		( link_list_tcb->link_list_head )->last_member =  cur_list_member;
		( link_list_tcb->link_list_head ) =  cur_list_member;
		if ( !link_list_tcb->link_list_end )
			link_list_tcb->link_list_end = cur_list_member;
	}
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @����: ��������ɾ��һ����Ա
 *
 * @����: link_list_tcb ���� ����Ŀ��ƿ�cur_list_member ��Ҫɾ���ĳ�Ա
 *
 *@���:  NONE
********************************************************************/
static void link_list_delete_member( link_list_tcb_t*link_list_tcb,
									 link_list_t *cur_list_member )
{
	if ( !cur_list_member->next_member )
	{
		link_list_tcb->link_list_end = cur_list_member->last_member;
	}
	if ( !cur_list_member->last_member )
	{
		link_list_tcb->link_list_head = cur_list_member->next_member;
	}
	( cur_list_member->last_member )->next_member =
		cur_list_member->next_member;
	( cur_list_member->next_member )->last_member =
		cur_list_member->last_member;
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @����: ��������ƥ��һ����Ա
 * @����: link_list_tcb ���� ����Ŀ��ƿ�cur_list_member ��Ҫƥ��ĳ�Ա
 *
 *@���:  TURE ƥ��ɹ� FAIL ƥ��ʧ��
********************************************************************/
Bsize_t link_list_offset_obtain( link_list_tcb_t*link_list_tcb,
								 Bsize_t link_direction,
								 link_list_t *cur_list_member )
{
	Bsize_t   offset = 0;
	link_list_t *lptr;
	if ( link_direction == LINK_ADD_END )
	{
		lptr = link_list_tcb->link_list_end ;
		while ( lptr )
		{
			if ( lptr == cur_list_member )
			{
				return offset;
			}
			offset++;
			lptr = lptr->last_member;
		}
		return LINK_MATCH_FAIL;
	}
	else
	{
		lptr = link_list_tcb->link_list_head;
		while ( lptr )
		{
			if ( lptr == cur_list_member )
			{
				return offset;
			}
			offset++;
			lptr = lptr->next_member;
		}
		return LINK_MATCH_FAIL;
	}
}

/********************************************************************
 * @������ :�ҳ�
 * @����: ��������ƥ��һ����Ա
 * @����: link_list_tcb ���� ����Ŀ��ƿ�pbody��Ա����
 *
 *@���:  ����ƥ������
********************************************************************/
BOOL link_list_match( link_list_tcb_t*link_list_tcb, link_list_t*plink )
{
	link_list_t *lptr;
	lptr = link_list_tcb->link_list_head;
	while ( lptr )
	{
		if ( lptr == plink )
		{
			return TURE;
		}
		lptr = lptr->next_member;
	}
	return FAIL;
}
/********************************************************************
 * @������ :�ҳ�
 * @����: ����һ�������Ա
 *
 * @����: link_list_tcb ���� ����Ŀ��ƿ�
 *                link_direction ��ӵķ���: LINK_ADD_END ����β  LINK_ADD_HEAD����ͷ
 *                body_lenth :��Ա�����С
 *@���:  NONE
********************************************************************/
link_list_t*  link_list_memeber_create( link_list_tcb_t*link_list_tcb,
										Bsize_t link_direction,
										Bsize_t body_lenth )
{
	link_list_t *cur_list_member;
	link_list_lock();
	cur_list_member = link_list_alloc_space( body_lenth );
	if ( cur_list_member )
	{
		link_list_add_member( link_list_tcb, cur_list_member, link_direction );
		link_list_unlock();
		return cur_list_member;
	}
	link_list_unlock();
	return 0;
}
/********************************************************************
 * @������ :�ҳ�
 * @����: ɾ��һ�������Ա
 *
 * @����: link_list_tcb ���� ����Ŀ��ƿ�
 *
 *                cur_list_member  Ҫ���ٵĳ�Ա
 *@���:  NONE
********************************************************************/
void link_list_memeber_destory( link_list_tcb_t*link_list_tcb, link_list_t *cur_list_member )
{
	link_list_lock();
	if ( link_list_match(
			 link_list_tcb,
			 cur_list_member ) == TURE )
	{
		link_list_delete_member( link_list_tcb, cur_list_member );
		link_list_free_space( cur_list_member );
	}
	link_list_unlock();
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @����: ��̬��������ʼ��
 *
 * @����:none
 *
 *@���:  NONE
********************************************************************/
void link_list_maneger_init( void )
{
	global_memset(
		( uint8_t* ) &g_link_list_maneger_tcb,
		0,
		sizeof( link_list_maneger_t ) );
	link_list_member_init();
	link_list_lock_creat();
	return;
}

/* for example
typedef struct
{
	Bsize_t x;
	Bsize_t y;
	Bsize_t z;
}link_test_t;

void  link_list_init_test( void*arg )
{
	link_test_t*ptr = ( link_test_t* )arg;
	ptr ->x = 100;
	ptr ->y = 200;
	ptr ->z = 300;
	return;
}
void  link_list_test( void*arg )
{
	link_list_tcb_t*link_list_tcb;
	link_list_tcb = link_list_tcb_req();
	uint32_t test_cnt = 0;
	while ( 1 )
	{
		test_cnt ++;
		if ( test_cnt > 100 )
		{
			test_cnt = 0;
			link_list_memeber_create
			(
				link_list_tcb,
				LINK_ADD_END,
				sizeof( link_test_t),
			);
		}
		msleep( TYPE_DELAY );
	}

}
*/
#endif

/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:动态链表管理模块
* 创建时间:2010-07-21
* 所属公司 :科荟
* 文件名称:link_list_maneger.c
* 创建人 :揭成
* 功能描述: 提供动态链表一些API功能，双向链表
*----------------------------------------------------------------
修改记录:

修改日期:

修改人:

*********************************************************************************************************
请不要在中断中调用该模块API接口
*/
#include "../app_support.h"
#if (LINK_LIST_MANEGER_EN >0)
link_list_maneger_t  	g_link_list_maneger_tcb = {0, };
/*动态链表锁函数*/
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
 * @创建人 :揭成
 * @功能:申请一个动态链表控制块
 *
 * @输入:void
 *
 *@输出: 控制块位置
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
 * @创建人 :揭成
 * @功能:释放链表控制块
 *
 * @输入:lptr 控制块位置
 *
 *@输出: TURE 成功 FAIL 失败
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
 * @创建人: 揭成
 * @功能: 创建一个网格控件
 *
 * @输入:NONE

 * @输出:成员位置
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
 * @创建人 :揭成
 * @功能: 为链表申请空间，
 *
 * @输入: alloc_size 构件链表包体大小
 *
 *@输出:  控制块位置
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
 * @创建人 :揭成
 * @功能: 释放链表空间
 *
 * @输入:  list_member :构件链表的成员
 *
 *@输出:  TURE OR FAIL
********************************************************************/
static BOOL link_list_free_space( link_list_t*list_member )
{
	app_free( list_member->cur_member );
	link_list_member_free( list_member );
	return TURE;
}

/********************************************************************
 * @创建人 :揭成
 * @功能: 在链表中添加一个成员
 *
 * @输入: link_list_tcb 所在 链表的控制块cur_list_member 需要添加的成员
 *                link_direction 添加方向
 *
 *@输出:  NONE
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
 * @创建人 :揭成
 * @功能: 在链表中删除一个成员
 *
 * @输入: link_list_tcb 所在 链表的控制块cur_list_member 需要删除的成员
 *
 *@输出:  NONE
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
 * @创建人 :揭成
 * @功能: 在链表中匹配一个成员
 * @输入: link_list_tcb 所在 链表的控制块cur_list_member 需要匹配的成员
 *
 *@输出:  TURE 匹配成功 FAIL 匹配失败
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
 * @创建人 :揭成
 * @功能: 在链表中匹配一个成员
 * @输入: link_list_tcb 所在 链表的控制块pbody成员特征
 *
 *@输出:  返回匹配数量
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
 * @创建人 :揭成
 * @功能: 创建一个链表成员
 *
 * @输入: link_list_tcb 所在 链表的控制块
 *                link_direction 添加的方向: LINK_ADD_END 加在尾  LINK_ADD_HEAD加在头
 *                body_lenth :成员包体大小
 *@输出:  NONE
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
 * @创建人 :揭成
 * @功能: 删除一个链表成员
 *
 * @输入: link_list_tcb 所在 链表的控制块
 *
 *                cur_list_member  要销毁的成员
 *@输出:  NONE
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
 * @创建人 :揭成
 * @功能: 动态链表管理初始化
 *
 * @输入:none
 *
 *@输出:  NONE
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

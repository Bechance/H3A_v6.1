#ifndef _PEC_LINK_LIST_2010_H
#define  _PEC_LINK_LIST_2010_H
#include "../app_support.h"
#if LINK_LIST_MANEGER_EN >0

typedef struct  _link_list_t
{
	struct  _link_list_t *last_member;
	void *			      cur_member;
	struct  _link_list_t *next_member;
}link_list_t;

typedef struct
{
	Bsize_t id;
	link_list_t * 	link_list_end;
	link_list_t * 	link_list_head;
}link_list_tcb_t;

#define  MAX_LINK_LIST_SIZE  10
#define  MAX_LINK_MEMBER_SIZE 20
typedef struct
{
	Bsize_t  ture_size;
	link_list_tcb_t link_list_member[MAX_LINK_LIST_SIZE];
}link_list_maneger_t;

#define 			LINK_ADD_END  		0
#define 			LINK_ADD_HEAD  	(~(LINK_ADD_END))
#define               LINK_MATCH_FAIL     65535
Bsize_t 			link_list_offset_obtain( link_list_tcb_t*link_list_tcb, Bsize_t link_direction, link_list_t *cur_list_member );

BOOL 			link_list_match( link_list_tcb_t*link_list_tcb, link_list_t*plink );

void 			link_list_memeber_destory( link_list_tcb_t*link_list_tcb, link_list_t *cur_list_member );

link_list_t* 		link_list_memeber_create( link_list_tcb_t*link_list_tcb, Bsize_t link_direction, Bsize_t body_lenth );

link_list_tcb_t*  	link_list_tcb_req( void );

BOOL 			link_list_tcb_delete( link_list_tcb_t*lptr );

void				link_list_maneger_init( void );

#endif
#endif


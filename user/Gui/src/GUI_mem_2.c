/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:GUI内存管理模块
* 创建时间:2012-07-22
* 所属公司 :科荟
* 文件名称: GUI_MEM.c
* 创建人 :	揭成
* 功能描述: 管理GUI内存
*----------------------------------------------------------------*/
#include	"../GUI.h"
#define    MEM_BLOCK_SIZE (25600)
static     char _mem_block[MEM_BLOCK_SIZE];
#define    URAM_N           2

typedef struct
{
   void*    pNext;
   void*    plast;
   uint32_t size;
}Alloc_t;

Alloc_t _pHe[2];
Alloc_t *_phead = 0;
Alloc_t *_pend  = 0;

#define GET_ALLOC_HEAD(pMEM)  (Alloc_t*)((long)pMEM-sizeof(Alloc_t))
#define GET_ALLOC_MEM(pMEM)   (void*)((char*)pMEM+sizeof(Alloc_t))
#define GET_ALLOC_END(pMEM)   (void*)((char*)pMEM+pMEM->size)


//在end处插入一个成员
static void _add_list(Alloc_t*p)
{
   Alloc_t*plast = _pend->plast;
   p->plast = plast;
   p->pNext = _pend;
   _pend->plast = p;
   plast->pNext = p;
   return;
}

//从链表中删除一个成员
static void _sub_list(Alloc_t*p)
{
   Alloc_t*plast = p->plast;
   Alloc_t*pNext = p->pNext;

   plast->pNext  = pNext;
   pNext->plast  = plast;
   p->plast      = 0;
   p->pNext      = 0;
   return;
}


void GUI_free(void*pmem)
{
    Alloc_t*pCUR    = GET_ALLOC_HEAD(pmem);
	Alloc_t*p       = _phead;
	Alloc_t*plast;
	GUI_LOCK();
	while(p)
	{
        if(GET_ALLOC_END(p)==pCUR)
        {
			 p->size = p->size+pCUR->size;
			 GUI_UNLOCK();
			 return;
		}
		if(GET_ALLOC_END(pCUR)==p)
		{
			 pCUR->size = pCUR->size+p->size;
            _sub_list(p);
            _add_list(pCUR);
			GUI_UNLOCK();
			return;
		}
		p  = p->pNext;
	}	
	_add_list(pCUR);
	GUI_UNLOCK();
	return;
}


static void _swpMem(void)
{
	Alloc_t*plast;
	Alloc_t*pCUR;
	Alloc_t*p = _phead->pNext;
	Alloc_t*p2;
	while(p)
	{
	    pCUR = p;
		p2   = p;
		while(p2)
		{
		    if(GET_ALLOC_END(p2)==pCUR)
	        {
				 p2->size = p2->size+pCUR->size;
				 _sub_list(pCUR);
				 p = _phead->pNext;
				 break;
			}
			else if(GET_ALLOC_END(pCUR)==p2)
			{
				 pCUR->size = pCUR->size+p2->size;
				 _sub_list(p2);
				 p = _phead->pNext;
				 break;
			}
			p2 = p2->pNext;
		}
		if(p2==0)p = p->pNext;
	}
	return;
}

void* GUI_malloc(uint32_t mem_size)
{
	Alloc_t* p = _phead;
	Alloc_t* pCUR;
	uint32_t size;
	uint8_t dMOD ; 
	uint8_t times = 0;
	GUI_LOCK();
    mem_size = mem_size+sizeof(Alloc_t);
    while(times<2)
    {
        p = _phead;
		while(p)
		{
			if(p->size>=mem_size)
			{
				pCUR = (void*)((char*)p+mem_size);
				dMOD = (long)pCUR%URAM_N;
				pCUR = (void*)((char*)pCUR+dMOD);
				mem_size+= dMOD;
	            _sub_list(p);
				size = p->size-mem_size;
				if(size>sizeof(Alloc_t))
				{
					pCUR->size = p->size-mem_size;
	            	_add_list(pCUR);
				}
				p->pNext    = 0;
				p->size     = mem_size;
  			 	GUI_UNLOCK();
				return GET_ALLOC_MEM(p);
			}
			p = p->pNext;
		}
		_swpMem();times++;
     }
   GUI_UNLOCK();
   return 0;
}


void GUI_mem_maneger_init( void )
{
    Alloc_t*p = (Alloc_t*)_mem_block;
    p->size   = MEM_BLOCK_SIZE;
	GUI_MEMCLR(_pHe, sizeof(Alloc_t)*2);
	_phead       = &_pHe[0];
	_pend        = &_pHe[1];
    _pend->plast = _phead;
	_phead->pNext= _pend;
    _add_list(p);
	return;
}

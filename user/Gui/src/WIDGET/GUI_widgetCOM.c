#include "../../Gui.h"

extern      GUI_top_t*	  m_pTop_list_head;
extern      hwin_t*	      m_hwin_list_head;
extern      hwin_t*	      m_hwin_list_end;
BOOL 		GUI_IS_Topwidget( hwin_t*p );

void GUI_WINinvalid( hwin_t*pOBJ )
{
	GUI_LOCK();
	if ( pOBJ->_WINinvalid )
		pOBJ->_WINinvalid( pOBJ );
	GUI_UNLOCK();
	return;
}


void GUI_WINinvalid_cancle( hwin_t*pOBJ )
{
	GUI_LOCK();
	if ( pOBJ->_WINinvalid_cancle )
		pOBJ->_WINinvalid_cancle( pOBJ );
	GUI_UNLOCK();
	return;
}

static BOOL GUI_ISsubord( hwin_t*p )
{
	return ( p->type == WIDGET_RADIO ) ? ( 1 ) : ( 0 );
}

void*GUI_MarkWIN_obtain( void )
{
	hwin_t*p;
	p = m_CURwidget;
	if ( !p )
	{
		return 0;
	}
	if ( GUI_ISsubord( p ) )
	{
		return m_pMember;
	}
	return p;
}



void GUI_SUBordready(int id, int state )
{
	subord_mem_t*plist;
	hwin_t*p = m_CURwidget;
	GUI_LOCK();
	if (GUI_ISsubord(p) )
	{
		plist = p->subord_head;
		while ( 1 )
		{
			if ( plist->id == id ) break;
			plist = plist->next_edit;
			if ( plist == NULL )
			{
				GUI_UNLOCK();
				return ;
			}
		}
		if ( m_pMember )
		{
			p->_WINselect( p, 0 );
			m_pMember = plist;
			p->_WINselect( p, 1 );
			GUI_UNLOCK();
			return;
		}
	}
	GUI_UNLOCK();
	return;
}


void GUI_WINready( hwin_t*pOBJ, int state )
{
	hwin_t*p;
	GUI_LOCK();
	if ( !m_CURwidget )
	{
		m_CURwidget = GET_GUIWIN;
	}
	p = m_CURwidget;
	if(GUI_ISsubord(p))
	{
		if ( m_pMember )
		{
			p->_WINselect( p, 0 );
		}
	}
	else
	{
		if ( p->_WINselect )p->_WINselect( p, 0 );
	}
	p = pOBJ;
	m_CURwidget = p;
	if ( GUI_ISsubord( p ) )
	{
		m_pMember = p->subord_head;
		p->_WINselect( p, 1 );
	}
	else
	{
		if ( p->_WINselect )p->_WINselect( p, 1 );
	}
	GUI_UNLOCK();
	return;
}



static hwin_t*GUI_nextWIN_obtain( hwin_t*pOBJ, int dir )
{
	if ( m_pTop_list_head && !GUI_IS_Topwidget( pOBJ ) )
	{
		if ( pOBJ->_WINselect )pOBJ->_WINselect( pOBJ, 0 );
	}
	while ( 1 )
	{
		if ( dir == 0 )
			pOBJ = pOBJ->next_hwin;
		else
			pOBJ = pOBJ->last_hwin;
		if ( !pOBJ )
		{
			break;
		}
		if ( m_pTop_list_head )
		{
			if ( GUI_IS_Topwidget( pOBJ ) )
			{
				if ( pOBJ->_WINselect )break;
			}
		}
		else if ( pOBJ->_WINselect )break;
	}
	return pOBJ;
}


// dir = 0 обрф  dir = 1 иорф
void GUI_WINshift( int dir )
{
	hwin_t*p;
	hwin_t*pNext;
	subord_mem_t*pNsub;
	GUI_LOCK();
	if ( !m_CURwidget )
	{
		m_CURwidget = GET_GUIWIN;
	}
	p = m_CURwidget;
	if ( GUI_ISsubord( p ) )
	{
		if ( m_pMember )
		{
			if ( dir == 0 )
				pNsub = m_pMember->next_edit;
			else
				pNsub = m_pMember->last_edit;
			if ( !pNsub )
			{
				pNext = GUI_nextWIN_obtain( p, dir );
				if ( !pNext )
				{
					GUI_UNLOCK() ;
					return;
				}
			}
		}
	}
	else
	{
		pNext = GUI_nextWIN_obtain( p, dir );
		if ( !pNext )
		{
			GUI_UNLOCK() ;
			return;
		}
	}
	if ( GUI_ISsubord( p ) )
	{
		p->_WINselect( p, 0 );
		m_pMember = pNsub;
		if ( m_pMember )
		{
			p->_WINselect( p, 1 );
			GUI_UNLOCK();
			return;
		}
	}
	else
	{
		if ( p->_WINselect )p->_WINselect( p, 0 );
	}
	m_CURwidget = pNext;
	p = pNext;
	if ( GUI_ISsubord( p ) )
	{
		m_pMember = p->subord_head;
		p->_WINselect( p, 1 );
	}
	else
	{
		if ( p->_WINselect )p->_WINselect( p, 1 );
	}
	GUI_UNLOCK();
	return;
}


void GUI_WINdo( void )
{
	if ( m_CURwidget )
	{
		GUI_LOCK();
		if ( m_CURwidget->_WINdo )
		{
			m_CURwidget->_WINdo( m_CURwidget );
		}
		GUI_UNLOCK();
	}
	return;
}

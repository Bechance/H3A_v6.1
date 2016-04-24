#if	GUI_SLIDER_EN  > 0
#define   SLID_DEU_BGCOR                 0xce79
#define   SLID_DEU_FGCOR                 0


typedef struct
{
	uint16_t   CURpos;
	uint16_t   Npos;
	uint16_t   FG;
	uint16_t   BG;
	uint16_t   WIN_BG;
}widget_slid_t;
void GUI_widget_3D_paint( uint16_t x, uint16_t y, Bsize_t wide, Bsize_t high );
BOOL GUI_slider_ontouch( hwin_t*hp, app_touch_t*data );
static uint16_t  _slid_BG = SLID_DEU_BGCOR;
static uint16_t  _slid_FG = SLID_DEU_FGCOR;

void GUI_slider_SetDeuCor(uint16_t Bg,uint16_t Fg)
{
   _slid_BG = Bg;
   _slid_FG = Fg;
   return;
}
//paint
void _slider_paint(hwin_t*hp)
{
#define   ICON_H   						 20
#define   ICON_W   						 5
	widget_slid_t*pWidget;
    int offset;
	int x0,wide,y0,high,xdx;
    pWidget = hp->pwidget;
	x0      = hp->win_x+5;
	wide    = hp->win_wide-10;
	high    = hp->win_high-ICON_H/2-5;
	y0      = hp->win_y+high;
    GUI_window_hwin_req(hp);
	GUI_conText.LCD_Read_dot(hp->win_x+3,hp->win_y+3,&pWidget->WIN_BG);
	GUI_set_Bgcolor(pWidget->WIN_BG);
    GUI_clean_window();
	GUI_set_Bgcolor(pWidget->BG);
	GUI_fill_Rect(x0,y0,wide,3);  
	GUI_widget_3D_DrawDownRect(x0,y0,wide,3); 
	xdx = wide/pWidget->Npos;
	GUI_set_Fgcolor(pWidget->FG);
    for(offset = 0;offset<pWidget->Npos;offset++)
    {
        GUI_DRAW_VLINE(x0+xdx*offset+ICON_W/2,y0-ICON_H/2-3,y0-ICON_H/2);
	}
    GUI_DRAW_VLINE(x0+wide-ICON_W/2,y0-ICON_H/2-3,y0-ICON_H/2);
	GUI_set_Bgcolor(pWidget->BG);
    if(pWidget->CURpos<(pWidget->Npos))
    {
		GUI_fill_Rect(x0+xdx*pWidget->CURpos,y0-ICON_H/2+3,ICON_W,ICON_H);  
		GUI_widget_3D_paint(x0+xdx*pWidget->CURpos,y0-ICON_H/2+3,ICON_W,ICON_H); 
    }
	else
	{
		GUI_fill_Rect(x0+wide-ICON_W,y0-ICON_H/2+3,ICON_W,ICON_H);  
		GUI_widget_3D_paint(x0+wide-ICON_W,y0-ICON_H/2+3,ICON_W,ICON_H); 
	}
	hp->WIN_Paint     = 0;
	GUI_window_hwin_release(hp);
	return;
}
//create
hwin_t* GUI_slider_create(
	int  x,
	int  y,
	int  wide,
	int  Npos,
	int  flags,
	hwin_t*  fatherhp
)
{
	hwin_t *hp;
	widget_slid_t*pWidget;
	if(fatherhp->win_high<35) return 0;
	hp = GUI_window_child_creat(x,y,wide,30,0,fatherhp );
	GUI_window_hwin_req( hp );
	hp->pwidget = GUI_malloc( sizeof( widget_slid_t ) );
	GLOBAL_MEMCLR( hp->pwidget, sizeof( widget_slid_t ) );
	pWidget 	      = ( widget_slid_t* )hp->pwidget;
	pWidget->BG       = _slid_BG;
	pWidget->FG       = _slid_FG;
	pWidget->CURpos   = 0;
	pWidget->Npos     = Npos;
	hp->type 		  = WIDGET_SCLIDER;
	hp->flag          = flags;
#if GUI_TOUCH_EN>0
	hp->_ontouch      = GUI_slider_ontouch;
#endif
	hp->_paint        = _slider_paint;
	hp->WIN_Paint     = 1;
	GUI_window_hwin_release( hp );
	return hp;
}


void GUI_slider_setN(hwin_t*hp,int N)
{
	widget_slid_t*pWidget;
	GUI_window_hwin_req( hp );
    pWidget = hp->pwidget;
	pWidget->Npos = N;
	hp->WIN_Paint = 1;
	GUI_window_hwin_release( hp );
    return;
}

void GUI_slider_setCURpos(hwin_t*hp,int CURpos)
{
	widget_slid_t*pWidget;
	GUI_window_hwin_req( hp );
    pWidget = hp->pwidget;
	pWidget->CURpos= CURpos;
	hp->WIN_Paint = 1;
	GUI_window_hwin_release( hp );
    return;
}

int GUI_slider_GetN(hwin_t*hp)
{
	widget_slid_t*pWidget= hp->pwidget;
    return pWidget->Npos;
}

int GUI_slider_GetCURpos(hwin_t*hp)
{
	widget_slid_t*pWidget= hp->pwidget;
    return pWidget->Npos;
}





#if GUI_TOUCH_EN >0
BOOL GUI_slider_ontouch( hwin_t*hp, app_touch_t*data )
{
   return 0;
}

#endif



#endif

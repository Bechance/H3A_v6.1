#include        "GUI_config.h"
#ifndef   _2010_PEC_GLOBAL_GUI_H
#define   _2010_PEC_GLOBAL_GUI_H
#include "../support\app_support.h"
/*********************************************************************
* GUI STRUCT
**********************************************************************/
typedef struct
{
	uint16_t  wide;
	uint16_t  high;
	uint16_t  CHwide;
	uint16_t  nbyte;
	char      way;
	char 	 Dbuf[256];
}GUI_front_t;

typedef struct
{
	int x;
	int y;
}GUI_POINT;


typedef void( * ASC_obtain_t )( GUI_front_t*pfront, uint16_t ASCcode, int size );

typedef struct GUI_FONT
{
	const char* start_asc;
	const char* end_asc;
	uint8_t	    nbyte;
	uint16_t  	wide;
	uint16_t  	high;
	uint16_t  	CHwide;
	char        way;    //取模方式 'H'横向取模，'V' 纵向取模
	ASC_obtain_t  ASC_obtain;
	const uint8_t*pcode;
	const struct GUI_FONT*pNext;
}GUI_FONT;


typedef struct
{
	Bsize_t  	Fcolor ;
	Bsize_t  	Bcolor ;
	GUI_FONT*	CUR_Front;
	Bsize_t     CUR_Lhigh;
	void( *LCD_draw_dot )( uint16_t x, uint16_t y, uint16_t color );
	void( *LCD_FillRect )( uint16_t x, uint16_t y, uint16_t wide, uint16_t high, uint16_t color );
	void( *LCD_Read_dot )( uint16_t x, uint16_t y, uint16_t *color );
	void( *LCD_draw_Hline )( uint16_t x1, uint16_t x2, uint16_t y, uint16_t color );
	void( *LCD_draw_Vline )( uint16_t x, uint16_t y1, uint16_t y2, uint16_t color );
	void( *LCD_Getblock_Ext )( uint16_t x, uint16_t y, uint16_t wide, uint16_t high, Bsize_t sector );
	void( *LCD_Setblock_Ext )( uint16_t x, uint16_t y, uint16_t wide, uint16_t high, Bsize_t sector );
}GUI_const_t;


typedef struct subord_mem
{
	uint16_t x;
	uint16_t y;
	uint16_t CHwide;
	uint16_t CHhigh;
	Bsize_t  eidt_lenth;
	Bsize_t  surplus_lenth;
	Bsize_t  id;
	Bsize_t  flag;
	uint16_t UNflag;
	char*    edit_shadow;
	const void*title;
	const GUI_FONT*pfront;
	struct subord_mem * next_edit;
	struct subord_mem * last_edit;
}subord_mem_t;

typedef struct _GUI_msg_t
{
	Bsize_t 		start_pos;
	Bsize_t 		end_pos;
	Bsize_t			buffer_size;
	unsigned char   buffer_ptr[1];
} GUI_MsgTCB_t;

typedef struct
{
	Bsize_t x;
	Bsize_t y;
	Bsize_t state;
}app_touch_t;

typedef struct
{
	int x1;
	int y1;
	int x2;
	int y2;
	BLKmem_t sec;
}GUI_BLK_t;


typedef enum
{
	INVALID_WIN    = 0,
	CHILD_WIN   ,
	FATHER_WIN,
	WIDGET_MSGBOX,
	WIDGET_EDIT,
	WIDGET_LISTVIEW,
	WIDGET_FORM,
	WIDGET_BUTTON,
	WIDGET_TEXTBOX,
	WIDGET_SCRB,
	WIDGET_SCLIDER,
	WIDGET_COLOR_WINDOW,
	WIDGET_PROGRESS_BAR,
	WIDGET_DROP_LIST,
	WIDGET_CHECK_BOX,
	WIDGET_RADIO,
	WIDEGE_PAD_KEY,
	WIDEGE_PAD_HAND,
	WIDGET_DIALOG
}WIN_type_t;

typedef   struct hwin
{
	WIN_type_t  	type;
	uint16_t        win_x;
	uint16_t        win_y;
	uint16_t  		win_wide;
	uint16_t  		win_high;
	uint16_t        csr_x_pos;
	uint16_t        csr_y_pos;
	uint16_t        win_Fcor;
	uint16_t        win_Bcor;
	uint16_t        flag;
	uint16_t        lcdxs; //FOR topWIN by jiecheg 2013 0304
	uint16_t        lcdys;
	uint16_t  		lcdxe;
	uint16_t  		lcdye;
	uint16_t        win_id;
	struct hwin*    father_hwin;
	struct hwin*    next_hwin;
	struct hwin*    last_hwin;
	subord_mem_t*   subord_end;
	subord_mem_t*   subord_head;
	subord_mem_t*   subord_cur;
	uint16_t        win_depth;
	GUI_MsgTCB_t*   pMSG;
	void*		    pwidget;
	uint16_t        WIN_Paint;
	//2012-12-09 by jiecheng
	BOOL ( *_ontouch )(struct hwin*hp, app_touch_t*data );
	void ( *_paint )(struct hwin*hp );
	//2013-05-09 by jiecheng
	void ( *_WINinvalid )(struct hwin*hp );
	void ( *_WINinvalid_cancle )( struct hwin*hp );
	void ( *_WINdo )(struct hwin*hp );
	void ( *_WINselect)(struct hwin*hp, int state );
}hwin_t;

typedef struct GUI_top
{
	struct GUI_top *pLast;
	struct GUI_top *pNext;
	hwin_t*pWIN;
}GUI_top_t;
extern      GUI_const_t   GUI_conText;
extern      hwin_t*	      m_CURwidget;
extern      subord_mem_t* m_pMember;
/*********************************************************************
* FONT
**********************************************************************/
extern 		uint16_t		m_GUI_CHwide;
extern 		uint16_t		m_GUI_CHhigh;
extern 		const GUI_FONT GUI_F8X13_ASCII;
extern 		const GUI_FONT GUI_F8X11_ASCII;
extern 		const GUI_FONT GUI_F8X16_ASCII;
extern 		const GUI_FONT GUI_F16X24_ASCII;
extern 		const GUI_FONT GUI_F16X32_ASCII;
extern      const GUI_FONT GUI_HZK16_EXTASCII;
extern      const GUI_FONT GUI_HZK24_EXTASCII;
extern 		const GUI_FONT GUI_F32X64_ASCII;
/*********************************************************************
* GUI-H
**********************************************************************/
void 			GUI_lib_init( void );

void 			GUI_SetFront( const GUI_FONT*pNEW );

void 			GUI_SetLineHigh( Bsize_t lineH);

const 			GUI_FONT*	GUI_GetFront( void );

void 			GUI_SET_DEFFRONT( void );

const 			GUI_FONT* GUI_GET_DEFGRONT(void);

void 			GUI_set_Fgcolor( uint16_t color );

void 			GUI_set_Bgcolor( uint16_t color );

void 			GUI_draw_line( int x0, int y0, int x1, int y1 );

void 			GUI_draw_color_line( int x0, int y0, int x1, int y1, uint16_t color );

void 			GUI_draw_Rect( uint16_t x1, uint16_t y1, Bsize_t wide, Bsize_t high );

void 			GUI_fill_Rect( uint16_t x1, uint16_t y1, Bsize_t wide, Bsize_t high );

void 			GUI_fill_Rect_At( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2 );

void 			GUI_libASC_display( int x, int y, uint16_t wide, uint16_t high, char*plib );

void 			GUI_libASC_display_At( int x, int y, uint16_t wide, uint16_t high, char*plib );

void 			GUI_lcd_resource_req( void );

void 			GUI_lcd_resource_release( void );

void 			GUI_draw_Hline( int x1, int x2, int y );

void 			GUI_draw_Vline( int x, int y1, int y2 );

uint16_t 		LCD_Color2Index_565( uint32_t Color );

/******************************************************************
*窗口属性
*******************************************************************/
#define             WIN_PAINT          	 0x100
#define             WIN_FILL             0x200
#define             WIN_3D               0x400
#define             WIN_INVALID     	 0x8000
#define             WIN_TOUCH         	 0x800
#define             WIN_MSG           	 0X2000
#define             WIN_ALL             (WIN_FILL|WIN_3D|WIN_TOUCH_EN)
#define             WIN_CLOSE_ICON_DIS  0xffff
#define             WIN_CLOSE_BUTTON    0x1000


//命名不规范，将被  WIN_TOUCH   WIN_MSG取代
#define             WIN_TOUCH_EN         0x800
#define             WIN_MSG_EN           0X2000

#define             GUI_WIN_TOUCH_EN(pWIN)  (pWIN->flag |= WIN_TOUCH)
#define             GUI_WIN_TOUCH_DIS(pWIN) (pWIN->flag &=(~WIN_TOUCH))
/******************************************************************
*GUI 核心相关头文件
*******************************************************************/


/*EXT*/
#define        	GUI_DRAW_VLINE(x,y1,y2)   	GUI_draw_line(x,y1,x,y2)
#define        	GUI_DRAW_HLINE(x1,x2,y)   	GUI_draw_line(x1,y,x2,y)
#define 		GET_XBORDER(hp)  			(hp->win_x+hp->win_wide)
#define 		GET_YBORDER(hp)  			(hp->win_y+hp->win_high)

void 		GUI_WINinvalid( hwin_t*pOBJ );

void 		GUI_WINinvalid_cancle( hwin_t*pOBJ );

void 		GUI_WINshift( int dir);

void 		GUI_WINdo( void );

void 		GUI_WINready( hwin_t*pOBJ, int state);

void 		GUI_SUBordready(int id, int state );

void*		GUI_MarkWIN_obtain(void);
/*以下函数是窗口操作相关了*/
#define         GET_GUIWIN  		GUI_hwin_obtain()
extern      hwin_t* m_cur_hwin;

void 		GUI_LOCK( void );

void 		GUI_UNLOCK( void );

void 		GUI_os_init( void );

uint32_t 	GUI_os_jiffies_obtain( void );

void 		GUI_msleep( uint32_t jiffies );

void   		GUI_TopWIN_Set( hwin_t*pOBJ );

void 	 	GUI_TopWIN_cancel( hwin_t*pOBJ );

BOOL 		GUI_foucus_dot_divide( int x, int y );

BOOL  		GUI_foucus_area_divide( int x, int y, int wide, int high );

void   		GUI_focus_cancel( void );

hwin_t*		GUI_hwin_obtain( void );

hwin_t*		GUI_CURwin_obtain();

uint16_t 	GUI_GetwinFgcor( void );

uint16_t	GUI_GetwinFgcor( void );

hwin_t* 	GUI_Getchild( Bsize_t id, hwin_t*fatherhp );

hwin_t* 	GUI_childobj_obtian( Bsize_t id, Bsize_t type, hwin_t*fatherhp );

hwin_t* 	GUI_window_child_creat( uint16_t x_coord, uint16_t y_coord, Bsize_t wide, Bsize_t high, Bsize_t flags, hwin_t*fatherhp );

hwin_t* 	GUI_window_creat( uint16_t x_coord , uint16_t y_coord, Bsize_t wide, Bsize_t high , Bsize_t flags );

void  		GUI_window_destory( hwin_t* hp );

void 		GUI_window_hwin_req( hwin_t* hp );

void 		GUI_window_hwin_release( hwin_t* hp );

void 		GUI_window_hwin_force_req( hwin_t* hp );

void 		GUI_window_hwin_force_release( hwin_t* hp );

void 		GUI_window_line_display( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2 );

void  		GUI_window_angle_line_display( uint16_t x1, uint16_t y1, float angle, uint16_t len , uint8_t flags, char*title );

void 		GUI_x_set( uint16_t x );

void 		GUI_y_set( uint16_t y );

void 		GUI_xy_set( uint16_t x, uint16_t y );

void 		GUI_x_shift( int offset );

void 		GUI_int_display( long data );

void 		GUI_int_display_At( uint16_t x, uint16_t y, long data );

void 		GUI_float_display( float data, Bsize_t bit );

void 		GUI_float_display_At( uint16_t x, uint16_t y, float data, Bsize_t bit );

void 		GUI_string_display( char*pstr );

void 		GUI_string_display_At( uint16_t x, uint16_t y, char*pstr );

void 		GUI_Frontstring_display_At( uint16_t x, uint16_t y, char*pstr, const GUI_FONT*pfront );

void 		GUI_string_window_display_At( hwin_t*pOBJ, uint16_t x, uint16_t y, char*pstr, uint16_t BGcor, uint16_t FGcor );

void 		GUI_string_wide_display_At( uint16_t x, uint16_t y, uint16_t wide, char*pstr );

void 		GUI_clean_window( void );

uint16_t	GUI_GetwinBgcor( void );

uint16_t	GUI_GetwinFgcor( void );

void 		GUI_fill_window_area( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2 );

void	    GUI_window_draw_Rect( uint16_t x1, uint16_t y1, Bsize_t wide, Bsize_t high );

void 		GUI_resource_req( void );

void 		GUI_WINrepaint( void );

void 		GUI_resource_release( void );

void  		GUI_window_draw_dot( uint16_t x, uint16_t y );

void 		GUI_widget_3D_paint( uint16_t x, uint16_t y, Bsize_t wide, Bsize_t high );

void 		GUI_widget_sink_3D_paint( uint16_t x, uint16_t y, Bsize_t wide, Bsize_t high );
/*********************************************************************
* GUI_H  GUI_2D
**********************************************************************/

void 		GUI_polygon_fill( int x, int y, const GUI_POINT*p, int size, uint16_t color );

void 		GUI_polygon_draw( int x, int y, const GUI_POINT*p, int size, uint16_t color );

void 		GUI_polygon_rotate( GUI_POINT* pDest, const GUI_POINT* pSrc, int size, float Angle );

void 		GUI_DrawEllipse( int x0, int y0, int rx, int ry, uint16_t color );

void 		GUI_FillEllipse( int x0, int y0, int rx, int ry, uint16_t color );

void 		GUI_drawcircle( uint16_t x0, uint16_t y0, uint16_t r , uint16_t color );

void 		GUI_fillcircle( uint16_t x0, uint16_t y0, uint16_t r, uint16_t color );
/*********************************************************************
* GUI_H  GUI_Memdev
**********************************************************************/
void 		GUI_Memdev_init( void );

void 		GUI_Memdev_destroy( void );

void 		GUI_Memdev_create( int x, int y, int wide, int high, void*pmem, uint32_t size );

void 		GUI_Memdev_toLCD( void );
/*********************************************************************
* GUI_H  GUI_connect
**********************************************************************/
void 		GUI_draw_dot( uint16_t X, uint16_t Y );

void 		GUI_clean_dot( uint16_t X, uint16_t Y );

void 		GUI_write_block_Extp( int x1, int y1, int x2, int y2, uint16_t sec );

uint16_t 	GUI_read_block_Extp( int x1, int y1, int x2, int y2 );

GUI_BLK_t*  GUI_read_memBlock( int x1, int y1, int x2, int y2 );

void        GUI_write_memBlock(GUI_BLK_t * p);

void 		GUI_write_block( int x1, int y1, int x2, int y2, void *BlockData, char sele );

void 		GUI_read_block( int x1, int y1, int x2, int y2, void *BlockData );

void 		GUI_write_block_Ext( int x1, int y1, int x2, int y2, uint16_t sector );

void 		GUI_print_string( int x, int y, int xs, int ys, char*str );

void 		GUI_clean_screen( void );
/*EXT*/
#define     GUI_WINDIS_VLINE(x,y1,y2)   		GUI_window_line_display(x,y1,x,y2)
#define     GUI_WINDIS_HLINE(x1,x2,y)   		GUI_window_line_display(x1,y,x2,y)
#define     MIDDLE_COORD_GET(wide,str,CHwide)   ((wide-(global_strlen((char*)str) * CHwide))/2)
#define     GUI_DISPLAY_IN_MID(y,str,CHwide)  	GUI_string_display_At(MIDDLE_COORD_GET(m_cur_hwin->win_wide,str,CHwide),y,(char*)str)
/*********************************************************************
* GUI_H  光标
**********************************************************************/
#if  GUI_CSR_EN  >0
typedef void( * GUI_tmr_callblack )( void*p_arg );
typedef struct
{
	GUI_tmr_callblack function;
	uint32_t          expires;
	uint32_t          last_jiff;
	void*	          p_arg;
}GUI_TMR_t;


typedef struct
{
	uint16_t	x;
	uint16_t  	y;
	uint16_t  	wide;
	uint16_t  	high;
	uint16_t    csrx_pos;
	uint16_t    csry_pos;
	hwin_t      *hOBJ;
	char   		occupy_flag;
	char    	flicker_flag;//=1 open;=0 shut
	void *		csr_shadow;
	GUI_TMR_t   TMR;
}GUI_csr_t;

void 			GUI_CSR_TMR_handle( void );

GUI_csr_t* 		GUI_cursor_start( hwin_t*hOBJ, int wide, int high );

void 		 	GUI_cursor_stop( GUI_csr_t * pcsr );

void 		 	GUI_cursor_shift( GUI_csr_t * pcsr , int x, int y );

void 		 	GUI_cursor_reload( GUI_csr_t * pcsr );

BOOL 		 	GUI_cursor_state_obtain( GUI_csr_t * pcsr );

void 		 	GUI_cursor_init( void );
#endif
/*********************************************************************
* GUI_H  触摸屏
**********************************************************************/
#define   TOUCH_ERR_DATA   10;
typedef struct
{
	int lcdx;  			/*物理坐标*/
	int lcdy;
	int touchx;			/*逻辑坐标*/
	int touchy;
}touch_coord_t;
typedef struct
{
	float Kx;
	float Ky;
	int lcdx;      		 /*物理坐标*/
	int lcdy;
	int touchx; 		 /*逻辑坐标*/
	int touchy;
}touch_scale_t;
typedef struct  		 /*转换后按字符类型保存，符合通用型标准*/
{
	uint8_t reset[2];
	uint8_t cKx[10];
	uint8_t cKy[10];
	uint8_t clcdx[6];    /*物理坐标*/
	uint8_t clcdy[6];
	uint8_t ctouchx[6];  /*逻辑坐标*/
	uint8_t ctouchy[6];
}touch_scale_save_t;

void 		GUI_touch_resource_req( void );

void 		GUI_touch_resource_release( void );

Bsize_t 	GUI_touch_read( void*pdata, Bsize_t size );

void 		GUI_touch_correct( void );

void 		GUI_wait_touch_release( void );

BOOL 		GUI_widget_key_cnt_wait( void );

BOOL 		GUI_task_hook( app_touch_t*pdata, Bsize_t size );

extern   	dev_id_t 	m_touch_dev_id;

extern   	a_lock_t* m_touch_read_lock;

BOOL 		GUI_GetEXTkey( uint8_t*key );

void 		GUI_SetEXTkey( uint8_t key );

enum
{
	GUI_KEY_3D_WINDOW_CLOSE = 200,
	GUI_KEY_EDIT_START,
	GUI_KEYPAD_ENTER,
	GUI_KEYPAD_EXIT,
	GUI_KEYPAD_DEL,
	GUI_KEY_EDIT_MEMEBER_SW,
	GUIE_KEY_ESC,
	GUIE_KEY_ENTER,
	GUIE_KEY_EXIT,
	GUIE_KEY_UP,
	GUIE_KEY_DOWN,
	GUIE_KEY_RIGHT,
	GUIE_KEY_LEFT,
	GUIE_KEY_0,
	GUIE_KEY_1,
	GUIE_KEY_2,
	GUIE_KEY_3,
	GUIE_KEY_4,
	GUIE_KEY_5,
	GUIE_KEY_6,
	GUIE_KEY_7,
	GUIE_KEY_8,
	GUIE_KEY_9,
	GUI_NO_KEY_START
};
/******************************************************************
*控键相关头文件
*
*******************************************************************/
#if GUI_WIDGET_FORM_EN	> 0
#define 		PLACE_MID     		(1<<0)
#define  		BLEFT             	(1<<0)
#define  		BRIGHT           	(1<<1)
#define  		BTOP            	(1<<2)
#define  		BBOTTOM	  			(1<<3)

hwin_t* 	GUI_form_creat( int xcoord, int ycoord, Bsize_t nline, Bsize_t nrow, Bsize_t grid_wide, Bsize_t grid_high, hwin_t* fatherhp );

void 		GUI_form_border_erase( Bsize_t offset, Bsize_t flags );

void 		GUI_form_select( Bsize_t offset );

void 		GUI_form_display_string( uint8_t opt, char*pst );

void 		GUI_form_display_string_At( Bsize_t offset, uint8_t opt, char*pst );

void 		GUI_form_display_float_At( Bsize_t offset, uint8_t opt, float data );

void 		GUI_form_fill( Bsize_t offset );

void 		GUI_form_fill_continue( Bsize_t offset_start, Bsize_t len );

void 		GUI_form_display_float_At( Bsize_t offset, uint8_t opt, float data );
#endif
#if GUI_WIDGET_EDIT_EN	> 0
#define 		EDIT_MEMBER_3D_EN 		    1  /*编辑成员3D支持*/
#define  		CSR_LEFT_SHIFT          	1u
#define  		CSR_RIGHT_SHIFT        		2u
#define  		EDIT_NEXT        		   	3u
#define  		EDIT_LAST       		   	4u
#define  		EDIT_ADD_END               (0x100)
#define  		MEMBER_3D            	   (0x200)
#define  		EDIT_DIS            	   (0x400)
extern          uint16_t   					m_edit_member_Fgcor;
extern          uint16_t   					m_edit_member_Bgcor;
#define   		EDIT_MEMBER_BG_COLOR        m_edit_member_Bgcor
#define   		EDIT_MEMBER_FRAME_COLOR  	m_edit_member_Fgcor

void 		GUI_edit_member_set_color( uint16_t Fgcor, uint16_t Bgcor );

hwin_t* 	GUI_edit_creat( uint16_t xcoord, uint16_t ycoord, Bsize_t wide, Bsize_t high, Bsize_t flags, hwin_t* fatherhp );

void 		GUI_edit_memeber_add( uint16_t x, uint16_t y, Bsize_t lenth, char*title, Bsize_t flags );

BOOL 		GUI_edit_memeber_destroy( Bsize_t id );

void 		GUI_edit_clean( void );

void 		GUI_edit_content_obtain(char*str,Bsize_t id,hwin_t*edit);

void 		GUI_edit_string_display( char*pstr );

void 		GUI_edit_string_display_At( char*pstr, Bsize_t id );

uint16_t 	GUI_edit_GetUNflag(hwin_t*edit,Bsize_t id);

void    	GUI_edit_SetUNflag(hwin_t*edit,Bsize_t id, uint8_t flag);

void 		GUI_edit_float_display_at( float fdata, Bsize_t id, Bsize_t ch_len );

void 		GUI_edit_int_display_At( long data, Bsize_t id );

BOOL 		GUI_edit_shift( Bsize_t flag );

BOOL  		GUI_edit_select( Bsize_t id );

Bsize_t 	GUI_edit_Boffset_obtain( void );

Bsize_t 	GUI_edit_id_obtain( void );

void 		GUI_edit_csr_shift( Bsize_t flag );

hwin_t* 	Gui_start_editOBJ_obtain( void );

BOOL		GUI_edit_ontouch( hwin_t*hp, app_touch_t*data );

uint8_t 	GUI_edit_start( hwin_t*hp, unsigned char( *callback )( hwin_t*hp ) );

void 		GUI_edit_allmember_release( hwin_t *hp );

void 		GUI_edit_init( void );

extern          hwin_t*			    	start_edit_win;
extern          uint8_t			    	edit_is_start;
extern          uint8_t              	    m_eidt_end_flag;
extern          GUI_csr_t	    	    *m_edit_csr;
#endif

#if GUI_WIDGET_MSGBOX_EN	> 0
#define 		MSGBOX_TITLE_HIGH        20
#define 		MSGBOX_BORDER       	 2

#define 		GUI_MSG_TASK_EN  		 0     /*是否启用消息退出机制*/

void   			Gui_MsgBox_init( void );

void 			GUI_MsgBOX_Setcolor( int txtcolor, int bgcolr );

void 			GUI_MsgBOX_SetFont( GUI_FONT*pFont );

void 			GUI_MsgBOX_Setlanguage(uint8_t language);

int  			GUI_MsgBox_creat( uint16_t x , uint16_t y, Bsize_t wide, Bsize_t high, char *title, Bsize_t( *callback )( hwin_t*hp ) );

int 			MsgBox( int x , int y, Bsize_t wide, Bsize_t high, char *title, char*pcontent, Bsize_t timeout );

BOOL 			MsgPasswordBox( int x , int y, char*password );

Bsize_t         key_method_input_char_handle(char*src,Bsize_t offset,Bsize_t*pos); 

#endif
#if GUI_BUTTON_EN> 0
#define         BUTTON_RESERVER       (WIN_PAINT*GUI_TOUCH_EN)

#define         BUTTON_PAINT  		    WIN_PAINT
#define         BUTTON_ICON  		    0x1000
#define         BUTTON_2STATE  		    0x4000
typedef struct
{
	char        UNpress_title[22];
	char        press_title[22];
	void*       press_icon;
	void*       unpress_icon;
	int         button_state;
	uint16_t    unpress_BG_corlor;
	uint16_t    unpress_FG_corlor;
	uint16_t    press_BG_corlor;
	uint16_t    press_FG_corlor;
	BOOL        Button_is_inv;
	BOOL        Is_2state;
	BOOL        Repress_EN;
	BOOL        Is_press;
	GUI_FONT*	pFONT;
	void( *buttoncallback )( void*arg );
}widget_button_t;

void 		_button_invalid( hwin_t*hp );

void  		_button_invalid_cancle( hwin_t*hp );

hwin_t* 	Gui_button_creat( uint16_t xcoord, uint16_t ycoord, Bsize_t wide, Bsize_t high, Bsize_t flags, hwin_t * fatherhp );

void 		Gui_button_set_color( uint16_t Fgcor, uint16_t Bgcor );

BOOL 		GUI_2state_button_press_titlecopy( hwin_t*hp, char*title );

BOOL 		GUI_2state_button_obtain( hwin_t*hp );

void 		GUI_2state_button_assign( hwin_t*hp, BOOL state );

void 		GUI_button_callback_mount( hwin_t*hp, void( *callback )( void*arg ) );

BOOL		GUI_button_ontouch( hwin_t*hp, app_touch_t*data );

BOOL 		GUI_button_title_display( hwin_t*hp, char*title );

BOOL 		GUI_button_Set_icon( hwin_t*hp, void*pDATA, int state );

void 		Gui_button_dye( hwin_t*hp, uint16_t BG, uint16_t FG );

BOOL  		GUI_button_id_mod( hwin_t*hp, Bsize_t id );

void 		Gui_button_insert_icon( hwin_t*hp, const char*picon, uint16_t Fgcor, uint16_t Bgcor );

void 		GUI_button_init( void );
#endif
#if	GUI_3D_WINDOW_EN  >0
hwin_t* 		Gui_3D_window_creat( uint16_t xcoord, uint16_t ycoord, Bsize_t wide, Bsize_t high, char*title, Bsize_t flags, hwin_t* fatherhp );
void 			GUI_3D_window_title_mod( hwin_t*hp, char*title );
BOOL 			GUI_3D_window_ontouch( hwin_t*hp, app_touch_t*data );
void  			Gui_3D_window_set_color( uint16_t Fgcor, uint16_t Bgcor, uint16_t txtcor );
void 			GUI_3D_window_init( void );
void  			GUI_3D_widow_setFornt(const GUI_FONT*pFONT );
#endif
#if	GUI_TXTBOX_EN  >0
hwin_t* 		Gui_txtbox_creat( uint16_t xcoord, uint16_t ycoord, Bsize_t wide, Bsize_t high, Bsize_t flags, hwin_t* fatherhp );

void 			GUI_txtbox_print( hwin_t*hp, char*pst );
#endif
#if GUI_PROGRESS_BAR_EN  >0
hwin_t* 		Gui_progress_bar_creat( uint16_t xcoord, uint16_t ycoord, Bsize_t wide, Bsize_t high, Bsize_t setp_num, hwin_t* fatherhp );
void 			Gui_progress_bar_advance( hwin_t*hp, uint16_t setp );
void 			Gui_progress_bar_setValue( hwin_t*hp, uint16_t nstep );
void  			Gui_progress_mod_setp( hwin_t*hp, uint16_t setp_size );
void 			Gui_progress_bar_empty( hwin_t*hp );
#endif
#if GUI_DROP_DOWN_LIST_EN  >0

Bsize_t		GUI_drop_list_unfold( hwin_t*hp );

void 		Gui_drop_list_set_color( uint16_t Fgcor, uint16_t Bgcor );

void 		Gui_drop_list_init( void );

void  		GUI_drop_list_member_assign( hwin_t*hp, Bsize_t offset );

Bsize_t  	GUI_drop_list_member_obtain( hwin_t*hp );

BOOL 		GUI_drop_list_ontouch( hwin_t*hp, app_touch_t*data );

hwin_t* 	Gui_drop_list_creat( uint16_t xcoord, uint16_t ycoord, const char *pmember[], uint8_t size, Bsize_t flag, hwin_t* fatherhp );

#define     GUI_DROP_LIST_CREATE(x,y,member,flag,fatherhp)  Gui_drop_list_creat(x,y,member,SIZE_OF_ARRAY(member),flag,fatherhp);
#endif
#if GUI_SCROLLBAR_EN>0
void		Gui_scrb_SetVkey( hwin_t*hp, uint8_t keyup, uint8_t keydown, uint8_t keyshift );

void 		Gui_scrb_shift( hwin_t*hp, int dx );

void    	Gui_scrb_Setpos( hwin_t*hp, uint16_t pos );

void    	Gui_scrb_Setsize( hwin_t*hp, uint16_t size );

hwin_t* 	Gui_scrb_create( int x, int y, int wide, int high, int flags, hwin_t*  fatherhp );

void 		Gui_scrb_Setcorlor( hwin_t*hp, uint16_t BG, uint16_t FG );

uint16_t 	Gui_scrb_Getpos( hwin_t*hp );

uint16_t 	Gui_scrb_Getsize( hwin_t*hp );

hwin_t*		Gui_scrb_attach( hwin_t*hp, int wide );
#endif

#if	GUI_SLIDER_EN  > 0
void 		GUI_slider_setCURpos(hwin_t*hp,int CURpos);

void 		GUI_slider_setN(hwin_t*hp,int N);

int  		GUI_slider_GetN(hwin_t*hp);

int  		GUI_slider_GetCURpos(hwin_t*hp);

hwin_t* 	GUI_slider_create(int x,int y,int wide,int Npos,int flags,hwin_t*fatherhp);
#endif
#if GUI_TOUCH_EN>0
typedef struct
{
	int  x  ;
	int  y  ;
	int  wide  ;
	int  high  ;
	BOOL pos_is_move;
}keypad_t;
void 		GUI_keypad_open( void );
void 		GUI_keypad_close( void );
void 		GUI_keypad_Setpos( int x, int y );
Bsize_t 	GUI_keypad_GB_obtain( char*hzbuf );
void 		GUI_keypad_read( void );
#endif

#if GUI_DIALOG_EN>0
typedef struct
{
	Bsize_t member_type;
	Bsize_t member_id;
	char*member_title;
	uint16_t x;
	uint16_t y;
	uint16_t wide;
	uint16_t high;
	Bsize_t  flag;
	void *parg;
	Bsize_t arg_size;
	void( *callblack )( hwin_t*arg );
}dialogBox_t;

typedef struct
{
	uint8_t occupy_flag;
	uint8_t member_id;
	hwin_t *pmember;
}dialogBox_ctl_t;
void GUI_dialogBox_init( void );
hwin_t*GUI_dialogBox_create( const char*title,
							 uint16_t x,
							 uint16_t y,
							 uint16_t wide,
							 uint16_t high,
							 Bsize_t flag,
							 const dialogBox_t*pbox,
							 Bsize_t  box_size
						   );
hwin_t*	 GUI_dialogBox_member_obtain( Bsize_t id );
#define  GET_DOBJ  GUI_dialogBox_member_obtain
#endif

#if GUI_CHECK_BOX_EN>0
hwin_t*	GUI_checkBox_create( const char*title, uint16_t title_cor, uint16_t xcoord, uint16_t ycoord, uint16_t flag, hwin_t*fatherhp );
BOOL 	GUI_check_box_ontouch( hwin_t*hp, app_touch_t*data );
BOOL  	GUI_checkbox_state_obtain( hwin_t*hp );
void 	GUI_checkBox_select( hwin_t*hp, BOOL state );
void 	GUI_checkbox_state_modify( hwin_t*hp );
#endif
#if GUI_RADIO_EN>0
hwin_t* GUI_radio_create( uint16_t xcoord, uint16_t ycoord, Bsize_t wide, Bsize_t high, Bsize_t flags, hwin_t* fatherhp );
BOOL 	GUI_radio_ontouch( hwin_t*hp, app_touch_t*data );
Bsize_t GUI_radio_curmember_obtain( hwin_t*hp );
void*   GUI_radio_title_obtain( hwin_t*hp );
void 	GUI_radio_select( hwin_t*hp, Bsize_t id );
void 	GUI_radio_memeber_add( uint16_t x, uint16_t y, char*title, Bsize_t id );
#endif
#if GUI_LISTVIEW_EN>0
hwin_t* GUI_listview_create( uint16_t xcoord, uint16_t ycoord, Bsize_t wide, Bsize_t high, Bsize_t flags, hwin_t* fatherhp );
void 	GUI_listview_memeber_add( char*title, Bsize_t id );
Bsize_t GUI_listview_curmember_obtain( hwin_t*hp );
void 	GUI_listview_select( hwin_t*hp, Bsize_t id );
#endif

/*GUI_COM_SUPPORT*/
void 		GUI_mem_maneger_init( void );
void 		GUI_free( void*pmem );
void*		GUI_malloc( uint32_t mem_size );
void 		GUI_memset( void*pur_add, uint8_t pad_data, uint16_t lenth );
uint16_t    GUI_strlen( char*psrc );
BOOL    	GUI_memcmp( void * pur_add, void*src_ptr, uint16_t lenth );
void 		GUI_memcpy( void * pur_add, void*src_ptr, uint16_t lenth );
void 		GUI_strcat( char*pstr1, char*pstr2 );
char* 		GUI_strcpy( char*pdest, char*psrc );
int 		GUI_ltoa( long isrc, char *pdes );
int 		GUI_ftoa( float fsrc , char*pdes, int point_bit );
char* 		GUI_string_locate( char*psrc, char*psearch );
/*********************************************************************
* GUI-H   for MSG   内部文件
**********************************************************************/
void  		GUI_WinID_post( hwin_t*hp, uint8_t id );
Bsize_t 	GUI_MsgFIFO_empty( hwin_t*hp );
Bsize_t 	GUI_MsgFIFO_write( hwin_t*hp, uint8_t*data, Bsize_t size );
Bsize_t 	GUI_MsgFIFO_read( hwin_t*hp, uint8_t*data, Bsize_t size );
BOOL  		GUI_MsgFIFO_create( hwin_t*hp, Bsize_t size );
BOOL 		GUI_MsgFIFO_del( hwin_t*hp );
Bsize_t 	GUI_key_read( hwin_t*hp, uint8_t*data, Bsize_t size );
void 		GUI_keyMSG_post( hwin_t*hp, uint8_t key);
/*********************************************************************/
/*用户扩展函数*/
/**********************************************************************/
void 				GUI_edit_start_hook( void );
void 				GUI_edit_end_hook( void );
void 				GUI_edit_run_hook( uint8_t key );
uint8_t 			GUI_edit_setting(hwin_t*hp);
void				GUI_keyExt_unLock( void );
void 				GUI_keyExt_Lock( void );
void 				GUI_SetEXTkey( uint8_t  key );
BOOL 				GUI_GetEXTkey( uint8_t* key );
void 				GUI_key_create( uint8_t key );
/*********************************************************************
*用到的外部函数
**********************************************************************/
void  				GUI_dummy_press_Ekey( uint8_t key );
void*				app_malloc( uint32_t mem_size );
void 				app_free( void*pmem );
/*********************************************************************
*本地转换宏
**********************************************************************/
#ifndef SIZE_OF_ARRAY
#define 			SIZE_OF_ARRAY( array ) (sizeof(array)/sizeof(array[0]))
#endif
#define         	GUI_key_dummy_press  		GUI_WinID_post
#define         	GUI_MsgBox_exit()  			GUI_key_dummy_press(KEY_EXIT)
#define  			GUI_DBGMSG         	 		DBGMSG
#define         	GUI_LTOA 					GUI_ltoa
#define  			GUI_FTOA         	       	GUI_ftoa
#define        	 	GUI_STRCPY                  GUI_strcpy
#define         	GUI_STRLEN                  GUI_strlen
#define         	GUI_MEMCLR(psrc,lenth)      GUI_memset((void*)(psrc),0,(lenth))
#define  			GUI_MALLOC        	 		app_malloc
#define  			GUI_FREE        	        app_free
#define     		GUI_GET_STRING_END(str)     ((char*)(str)+GUI_strlen(str)-1)
#define     		GUI_ABS(val)   	        	(((val)>0)?((val)):(-(val)))
#define 			GUI_MIN(a,b) 				((a) < (b) ? (a) : (b))
#define 			GUI_MAX(a,b) 				((a) > (b) ? (a) : (b))
#endif

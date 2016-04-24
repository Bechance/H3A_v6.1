/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:系统输入法管理
* 创建时间:2010-12-04
* 所属公司 :科荟
* 文件名称:key_input method.c
* 创建人 :揭成
* 功能描述:系统输入法相关函数
*----------------------------------------------------------------
修改记录:

修改日期:

修改人:
********************************************************************/
#include 				"key_input method.h"
#include 				"key_input_QP.c"
#include 				"key_input_123.c"
#include 				"key_input_ABC.c"
//#include 				"key_input_HZGB.c"

#define        			KEY_INPUT_FONT         ((GUI_FONT*)&GUI_HZK24_EXTASCII)
/* 输入法标识大小设置*/
#if GUI_TOUCH_EN >0
#define    	      		INPUT_ICONXS      		(LCD_WIDE-100)
#define    	      		INPUT_ICONYS      		(LCD_HIGH-60)
#define           		INPUT_ICONW     		 90
#define    	      		INPUT_ICONH      		 55
#else
#define    	      		INPUT_ICONXS      		(LCD_WIDE-70)
#define    	      		INPUT_ICONYS      		(LCD_HIGH-34)
#define           		INPUT_ICONW     		 60
#define    	      		INPUT_ICONH      		 30
#endif
/*定义变量*/
key_input_ctl_t   		m_key_input_ctl;                    /*按键输入控制*/
hwin_t*             	m_input_icon_win 		 = PNULL;   /*输入法图标窗口句柄*/
BLKmem_t               	m_con_box_buf;                     	/*保存当前输入法显存块BUF*/
uint8_t                 g_key_input_keep_secret  = FAIL;	/*输入保密，用*表示*/
Bsize_t                 m_key_input_edit_memeber = 0;
static 					char _XXchMEM[50]="";     
BOOL 	 				GUI_keypad_is_exist(void);
Bsize_t  				GUI_keypad_GB_obtain(char*hzbuf );

static void key_input_method_icon_display( void );
/*******************************************************************
 * @创建人 :揭成
 * @功能 :设置输入框位置
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void key_input_method_xy_set( uint16_t x, uint16_t y )
{
    if( m_key_input_ctl.func.Setxy)m_key_input_ctl.func.Setxy(x,y);
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能  :判断输入法是否正在运行
 * @输入  :NONE
 * @输出  :TURE OR  FAIL
********************************************************************/
BOOL key_input_method_is_run( void )
{
	return ( m_input_icon_win == PNULL ) ? ( FAIL ) : ( TURE );
}
/********************************************************************
 * @创建人:揭成
 * @功能  :输入法按键回调
 * @输入  :NONE
 *@输出   :NONE
********************************************************************/
#if GUI_TOUCH_EN >0
/********************************************************************
 * @创建人:揭成
 * @功能  :设置手写键盘位置
 * @输入  :NONE
 * @输出  :NONE
********************************************************************/
void hand_wirte_input_box_pos_set( uint16_t x, uint16_t y )
{
    GUI_keypad_Setpos(x,y);
	return;
}
void hand_write_input_entery(void)
{
	GUI_keypad_open();
    return;
}
void hand_write_input_exit(void)
{
   GUI_keypad_close();
   return;
}
void input_button_call(void*arg)
{
    hwin_t*p = (hwin_t*)arg;
    if((start_edit_win->flag&WIN_TOUCH_EN))
    {
		if(!GUI_keypad_is_exist())
		{
			GUI_button_title_display(p,"关闭键盘");
			hand_write_input_entery();
		}
		else 
		{
			GUI_button_title_display(p,"打开键盘");
			hand_write_input_exit();
		}
    }
	return;
}

void key_input_method_icon_open( void )
{
    hwin_t*p;
	m_con_box_buf=GUI_read_block_Extp(
		INPUT_ICONXS - 2,
		INPUT_ICONYS - 2,
		INPUT_ICONXS + INPUT_ICONW + 5,
		INPUT_ICONYS + INPUT_ICONH + 5);
	GUI_set_Bgcolor( 0xce79);
	GUI_set_Fgcolor( C_GUI_RED);
	m_input_icon_win= GUI_window_creat(
		  INPUT_ICONXS,
		  INPUT_ICONYS,
		  INPUT_ICONW,
		  INPUT_ICONH,WIN_3D|WIN_FILL);
	GUI_window_hwin_req(m_input_icon_win);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_set_Bgcolor(0xce79);
	GUI_set_Fgcolor(C_GUI_RED);
	p= Gui_button_creat(
		  3,
		  INPUT_ICONH/2,
		  INPUT_ICONW-8,
		  INPUT_ICONH/2-2,0xfe|BUTTON_PAINT,m_input_icon_win);
	Gui_button_dye(p,LCD_Color2Index_565(0xc0c0c0),C_GUI_RED);
	GUI_button_title_display(p,"打开键盘");
	GUI_button_callback_mount(p,input_button_call);
	key_input_method_icon_display();
	GUI_TopWIN_Set(m_input_icon_win);
	GUI_window_hwin_release(m_input_icon_win);
	return;
}
#else
void key_input_method_icon_open( void )
{
	m_con_box_buf=GUI_read_block_Extp(
		INPUT_ICONXS - 2,
		INPUT_ICONYS - 2,
		INPUT_ICONXS + INPUT_ICONW + 5,
		INPUT_ICONYS + INPUT_ICONH + 5);
	GUI_set_Bgcolor( 0xce79);
	GUI_set_Fgcolor( C_GUI_RED);
	m_input_icon_win= GUI_window_creat(
		  INPUT_ICONXS,
		  INPUT_ICONYS,
		  INPUT_ICONW,
		  INPUT_ICONH,WIN_3D|WIN_FILL);
	GUI_window_hwin_req(m_input_icon_win);
	key_input_method_icon_display();
	GUI_TopWIN_Set(m_input_icon_win);
	GUI_window_hwin_release(m_input_icon_win);
	return;
}
void hand_write_input_entery(void)
{
}
void hand_write_input_exit(void)
{
}
void hand_wirte_input_box_pos_set( uint16_t x, uint16_t y )
{
}
#endif
/********************************************************************
 * @创建人 :揭成
 * @功能 : 关闭输入法标志
 *
 * @输入 :NONE
 *
 *@输出  : NONE
********************************************************************/
void key_input_method_icon_close( void )
{
	GUI_TopWIN_cancel(m_input_icon_win);
	GUI_write_block_Extp(
		INPUT_ICONXS -2,
		INPUT_ICONYS -2,
		INPUT_ICONXS + INPUT_ICONW + 5,
		INPUT_ICONYS + INPUT_ICONH + 5, m_con_box_buf);
	GUI_window_destory( m_input_icon_win );
	m_input_icon_win = PNULL;
	hand_write_input_exit();
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能   :输入法函数入口选择
 * @输入   :NONE
 * @输出   :NONE
********************************************************************/
static void key_key_input_method_func(void)
{
	key_input_ctl_t*pkey = &m_key_input_ctl;
	if ( pkey->input_select == KEY_INPUT_PY )
	{
		//m_key_input_ctl.func.Get_code= key_method_GB;
		//m_key_input_ctl.func.Setxy   = key_input_GB_xy_set;
	}
	else if ( pkey->input_select == KEY_INPUT_QP)
	{
		m_key_input_ctl.func.Get_code = key_method_QP;
		m_key_input_ctl.func.Setxy   = key_input_QP_xy_set;
	}
	else if ( pkey->input_select == KEY_INPUT_123 )
	{
		m_key_input_ctl.func.Get_code = key_method_123;
		m_key_input_ctl.func.Setxy   = 0;
	}
	else if ( pkey->input_select == KEY_INPUT_abc )
	{
		m_key_input_ctl.func.Get_code= key_method_abc;
		m_key_input_ctl.func.Setxy   = key_input_ABC_xy_set;
	}
	else if ( pkey->input_select == KEY_INPUT_ABC )
	{
		m_key_input_ctl.func.Get_code = key_method_ABC;
		m_key_input_ctl.func.Setxy    = key_input_ABC_xy_set;
	}
    return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能   :输入法标志显示
 * @输入   :NONE
 * @输出   :NONE
********************************************************************/
static void key_input_method_icon_display( void )
{
	key_input_ctl_t*pkey = &m_key_input_ctl;
	GUI_window_hwin_req( m_input_icon_win );
	GUI_set_Bgcolor( 0xce79);
	GUI_set_Fgcolor( C_GUI_RED);
	GUI_SetFront(KEY_INPUT_FONT);
	if ( pkey->input_select == KEY_INPUT_PY )
	{
		GUI_string_display_At( 8, 1, "汉 " );
	}
	else if ( pkey->input_select == KEY_INPUT_QP)
	{
		GUI_string_display_At( 8, 1, "拼 " );
	}
	else if ( pkey->input_select == KEY_INPUT_123 )
	{
		GUI_string_display_At( 8, 1, "123" );
	}
	else if ( pkey->input_select == KEY_INPUT_abc )
	{
		GUI_string_display_At( 8, 1, "abc" );
	}
	else if ( pkey->input_select == KEY_INPUT_ABC )
	{
		GUI_string_display_At( 8, 1, "ABC" );
	}
	GUI_window_hwin_release( m_input_icon_win );
	key_key_input_method_func();
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 : 指定单条编辑输入法种类，过滤输入法
 *
 * @输入 :
 *
 *@输出  : NONE
********************************************************************/
void key_input_method_filter( uint16_t flag )
{
	key_input_ctl_t*pkey = &m_key_input_ctl;
	pkey->input_flag = flag;
	if ( !( pkey->input_flag&pkey->input_select ) )
	key_input_method_switch();
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 : 输入法切换
 *
 * @输入 :NONE
 *
 *@输出  : NONE
********************************************************************/
void key_input_method_switch( void )
{
    uint8_t temp;
	key_input_ctl_t*pkey = &m_key_input_ctl;
	if ( pkey->input_select == 0 )
	{
		pkey->input_select = KEY_FIRST_INPUT;
	}
	temp = pkey->input_select;
	temp = temp;
	while ( 1 )
	{
		if ( pkey->input_select == KEY_END_INPUT )
		{
			pkey->input_select = KEY_FIRST_INPUT;
		}
		else
		{
			pkey->input_select = pkey->input_select << 1;
		}
		if (( pkey->input_flag&pkey->input_select ) )break;
	}
	hand_write_input_exit();
	key_input_method_icon_display();
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功?: 指定输入法
 *
 * @输入 :NONE
 *
 *@输出  : NONE
********************************************************************/
void key_input_method_assign( uint16_t flag )
{
	if ( flag <= KEY_END_INPUT)
	{
		m_key_input_ctl.input_select = flag;
	}
	key_input_method_icon_display();
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 : 获取当前输入法
 *
 * @输入 :NONE
 *
 *@输出  : NONE
********************************************************************/
uint8_t key_input_method_obtain( void )
{
	return m_key_input_ctl.input_select;
}
#if GUI_TOUCH_EN>0
/********************************************************************
 * @创建人:揭成
 * @功能  :判断输入是否合法
 * @输入  :
 * @输出  :NONE
********************************************************************/
BOOL key_method_input_lawful(char*buf,Bsize_t size)
{
   if(size==0) return 0;
   if(size==1)
   {
     if(m_key_input_ctl.input_flag&KEY_INPUT_123)
     {
        if(( '0' <= buf[0])&&(buf[0]<= '9' ))return 1;
		if(buf[0]=='.' ) return 1;
     }
     if(m_key_input_ctl.input_flag&KEY_INPUT_ABC
	   ||m_key_input_ctl.input_flag&KEY_INPUT_abc)
     {
        if(( 'a' <= buf[0])&&(buf[0]<= 'z' ))return 1;
        if(( 'A' <= buf[0])&&(buf[0]<= 'Z' ))return 1;
        if(( '0' <= buf[0])&&(buf[0]<= '9' ))return 1;
		if(buf[0]=='.' ) return 1;
     }   
   }
   else
   {
     if(m_key_input_ctl.input_flag&KEY_INPUT_PY)
     {
         return 1;
     } 
   }
   return 0;
}
#endif
/********************************************************************
 * @创建人:揭成
 * @功能 :按键输入法，获取输入字符
 * @输入 :
 * @输出 :NONE
********************************************************************/
Bsize_t key_method_input( uint8_t key, char*buf )
{
	BOOL ret = 0;
	if ( edit_is_start == FAIL ) return FAIL;
#if GUI_TOUCH_EN>0
    char ctemp[3]="";
	if(GUI_keypad_is_exist())
	{
		ret = GUI_keypad_GB_obtain(ctemp);
        if(!key_method_input_lawful(ctemp,ret))
        {
              return 0;
		}
		global_memcpy(buf,ctemp,ret);
		if ( ret )
		{
            if(g_key_input_keep_secret&&ret==1)
            {
				GUI_edit_string_display( "*" );
			}
			else
			{
				buf[ret] = ASCII_NULL;
				GUI_edit_string_display( buf );
			}
		}
		else ret = 0;  
		return ret;
	}
#endif
    if(key == KEY_SPACE) 
    {
		global_strcpy(buf," ");
		return 1;
	}
    if(!m_key_input_ctl.func.Get_code)return 0;
    ret = m_key_input_ctl.func.Get_code(key,buf);
	return ret;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :  判断是字符还是汉字
 *
 * @输入 :src, offset
 *
 *@输出  : 
********************************************************************/
Bsize_t key_method_input_char_handle(char*src,Bsize_t offset,Bsize_t*pos)
{
	Bsize_t offs=0;
	while(1)
	{
		if ((src[offs]&0xff)>=0x81)
		{
			offs+=2;		
			if(offs>offset) 
			{
				(*pos) = offs-2;
				return 2;
			}
		}
		else 
		{
			offs+=1;		
			if (offs> offset) 
			{
				(*pos) = offs-1;
				return 1;
			}
		}
	}
}	
/********************************************************************
 * @创建人 :杨雄取
 * @功能   :检查将要插入的字符所处的状态,看是否覆盖了2字节汉字；
 * @输入   :
 * @输出   :
            1.如果覆盖在汉字上，返回TRUE;
            2.如果覆盖非汉字上，返回FAIL;
********************************************************************/
static uint8_t _ChkOverHZ(char *pshadow,Bsize_t HeadPos,Bsize_t *HzPos)
{    
    if(key_method_input_char_handle(pshadow,HeadPos,HzPos) == 2)
        return TRUE;
    else
    {
        HzPos = 0 ;
        return FAIL;
    }
}
/********************************************************************
 * @创建人 :揭成
 * @功能   :按键输入法，保存在缓冲pshadow
 * @输入   :
 * @输出   :NONE
********************************************************************/
Bsize_t key_method_input_shadow( uint8_t key, char*pshadow)
{
	Bsize_t offset;
	Bsize_t len = 0;
	Bsize_t EndStartPos;
	Bsize_t HeadStartPos;
	Bsize_t EndPos;
	Bsize_t CsrPos;
	Bsize_t size;
	uint8_t HeadMoveFlag;
	char nch[8]={0,};
	
	offset = GUI_edit_Boffset_obtain();
    if(!pshadow)
    {
        GLOBAL_MEMCLR(_XXchMEM,50);
		pshadow = _XXchMEM;
        GUI_edit_content_obtain(pshadow,GUI_edit_id_obtain(),m_cur_hwin);
    }
	if ( key == KEY_DEL )
	{
		if(offset==0)	return 0;
		else offset = offset-1;
		size=key_method_input_char_handle(pshadow,offset,&HeadStartPos);
		global_memcpy( &pshadow[HeadStartPos],"**",size);
		global_string_filter( pshadow, '*' );
		GUI_edit_string_display_At( pshadow, GUI_edit_id_obtain() );
		GUI_edit_select( GUI_edit_id_obtain() );
		size = global_strlen(pshadow);
		(offset<size)?(offset=offset):(offset=size);
		for ( len = 0;len < offset;len++ )
			GUI_edit_csr_shift( CSR_RIGHT_SHIFT);
		return 1;
	}
	else
	{
		len = key_method_input( key, nch );
		if(len)
		{
            if((offset+len)<=m_cur_hwin->subord_cur->eidt_lenth)
    	    {
    	        GUI_edit_string_display(nch);
                _ChkOverHZ(pshadow,offset,&HeadStartPos); //头部
                if(offset != HeadStartPos)
                {
                    EndPos = HeadStartPos+len-1;
                    CsrPos = HeadStartPos+len;
                    HeadMoveFlag = 1 ;
                }
                else
                {
                    EndPos = offset+len-1;
                    CsrPos = offset+len;
                    HeadMoveFlag = 0 ;
                }
                if(_ChkOverHZ(pshadow,EndPos,&EndStartPos) == TRUE) //尾部
                {
                    if(EndPos == EndStartPos)
                    {
                        global_memcpy( &pshadow[EndStartPos+1],"*",1);
                    }
                }
                if(HeadMoveFlag)
                    global_memcpy( &pshadow[HeadStartPos],nch,len);
                else
                    global_memcpy( &pshadow[offset],nch,len);
                global_string_filter(pshadow,'*');  
                GUI_edit_string_display_At( pshadow, GUI_edit_id_obtain() );
        		GUI_edit_select( GUI_edit_id_obtain() );
        		size = global_strlen(pshadow);
        		(CsrPos<size)?(CsrPos=CsrPos):(CsrPos=size);
        		for ( offset = 0;offset < CsrPos;offset++ )
        			GUI_edit_csr_shift( CSR_RIGHT_SHIFT);
    	    }
		}
	}
    if(_XXchMEM==pshadow)
    {
        HeadStartPos=GUI_edit_Boffset_obtain();
        GUI_edit_string_display_At(_XXchMEM,m_cur_hwin->subord_cur->id);
		GUI_edit_select(m_cur_hwin->subord_cur->id);
		while(HeadStartPos>0)
		{
			GUI_edit_csr_shift(CSR_RIGHT_SHIFT);
			HeadStartPos--;
		}
	}
	return len;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :初始化汉字输入模块
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void key_input_method_init( void )
{
	app_global_key_resource_req();
	GLOBAL_MEMCLR( &m_key_input_ctl, sizeof( key_input_ctl_t ) );
	m_key_input_ctl.input_select = KEY_INPUT_123;
	key_input_method_filter(KEY_METHOD_ALL);
	return;
}

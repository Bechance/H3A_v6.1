#include 		"key_input method.h"
#define         ABCFONT        (&GUI_HZK24_EXTASCII)

static hwin_t*  _ABC_WIN;
static char  	_ABC_arry[10][7];
static int   	_ABC_pos = 0;
static char  	_ABC_code[7]="";
static int      _ABC_win_x = 30;
static int      _ABC_win_y = 350;
static int      _ABC_mode  = 0;

#define         WIN_WIDE    	250
#define         WIN_HIGH    	105
#define         ABLK_XS      	(_ABC_win_x-2)
#define         ABLK_YS      	(_ABC_win_y-2)
#define         ABLK_XE      	(ABLK_XS+WIN_WIDE+4)
#define         ABLK_YE      	(ABLK_YS+WIN_HIGH+4)
#define         ABC_BG_COLOR    C_GUI_WHITE-1
#define         ABC_FG_COLOR    0


static  hwin_t* _ABCWINpaint(void)
{
    hwin_t*pWIN;
	GUI_window_hwin_req(GET_GUIWIN);
	pWIN=GUI_window_creat(_ABC_win_x,_ABC_win_y,WIN_WIDE,WIN_HIGH,0);
	GUI_set_Bgcolor(ABC_BG_COLOR);
	GUI_set_Fgcolor(ABC_FG_COLOR);
	GUI_window_hwin_release(GET_GUIWIN);
	return pWIN;
}

static void _ABCWINdestory(hwin_t*pObj)
{
    GUI_window_destory(pObj);
}

static  int _AGet_code0_len(char code0)
{
   if(code0 == 'w'||code0 == 'p') return 4;
   else if(code0 == 'W'||code0 == 'P') return 4;
   return 3;
}

static void _ABCcode_save(void)
{
	if(_ABC_pos<9)
	{
		global_strcpy(_ABC_arry[_ABC_pos],_ABC_code);
		_ABC_pos++;
	}
    return;
}


void _ABCcode_sys(char*str,int n)
{
    int offset;
	int code0_len;
	code0_len   = _AGet_code0_len(str[0]);
    for(offset=0;offset<code0_len;offset++)
    {
        _ABC_code[n] = str[0]+offset;
		if(str[1]!=0)
		{
        	_ABCcode_sys(&str[1],n+1);
		}
		else
		{
			_ABCcode_save();
		}
	}
	return;
}

static void _ABC_init(char*str)
{
   GLOBAL_MEMCLR(_ABC_arry,sizeof(_ABC_arry));
   GLOBAL_MEMCLR(_ABC_code,sizeof(_ABC_code));
   _ABC_pos = 0;
   _ABCcode_sys(str,0);
   return;
}

static void _ABC_string_display(int x,int y,char*str,int sel,int ofs)
{
    if(str[0] == 0) return;
	if(ofs==sel)
	{
		GUI_set_Bgcolor(ABC_FG_COLOR);
		GUI_set_Fgcolor(C_GUI_YELLOW);
		GUI_string_display_At(x,y,str);
	}
	else
	{
		GUI_set_Fgcolor(ABC_FG_COLOR);
		GUI_set_Bgcolor(ABC_BG_COLOR);
		GUI_string_display_At(x,y,str);
	}
	return;
}


static void _ABC_PY_display(char*str,int offset,int sel)
{
    int x;
	int ofs = 0;
    char buf[8]="";
	x = _ABC_WIN->win_wide/3;
	global_strcpy(buf,str);
	GUI_window_hwin_req(_ABC_WIN);
	GUI_set_Bgcolor(ABC_BG_COLOR);
	GUI_set_Fgcolor(ABC_FG_COLOR);
    GUI_SetFront(ABCFONT);
    GUI_clean_window();
    _ABC_string_display(2+x*0,3+25*1,_ABC_arry[0],sel,ofs++);;
    _ABC_string_display(2+x*1,3+25*1,_ABC_arry[1],sel,ofs++);
    _ABC_string_display(2+x*2,3+25*1,_ABC_arry[2],sel,ofs++);
    _ABC_string_display(2+x*0,3+25*2,_ABC_arry[3],sel,ofs++);
    _ABC_string_display(2+x*1,3+25*2,_ABC_arry[4],sel,ofs++);
    _ABC_string_display(2+x*2,3+25*2,_ABC_arry[5],sel,ofs++);
    _ABC_string_display(2+x*0,3+25*3,_ABC_arry[6],sel,ofs++);
    _ABC_string_display(2+x*1,3+25*3,_ABC_arry[7],sel,ofs++);
    _ABC_string_display(2+x*2,3+25*3,_ABC_arry[8],sel,ofs++);
	if(global_strlen(buf)>offset)
	{
	   buf[offset] = 0;
	}
	GUI_set_Bgcolor(ABC_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_RED);
    GUI_string_display_At(2,1,buf);
	GUI_set_Fgcolor(ABC_FG_COLOR);
	GUI_WINDIS_HLINE(0,_ABC_WIN->win_wide,26);
	GUI_window_hwin_release(_ABC_WIN);
	return;
}


static char _ABCcode_Get(uint8_t incode)
{
	char code;
	code = app_global_keytoletter(incode);
	if(_ABC_mode==1)
	{
		return code;
	}
	else
	{
	    return (code-( uint8_t )'a' +( uint8_t )'A' );
	}
}

static int _ABCcode_obtain( uint8_t code_0, char*hzbuf )
{
	uint8_t   	key;
	GUI_BLK_t*	p;
	hwin_t   *	hp;
    GUI_csr_t*	pCSR;
	char  py[8]="";
	uint16_t   offset = 0;
	uint16_t   offs   = 1;
	uint16_t   sel    = 0;
	uint16_t   ret    = 0;
	uint16_t bg,fg;
	if(app_global_decide_PYkey(code_0)==0)return 0;
	code_0 =  _ABCcode_Get(code_0);
	hp = GUI_CURwin_obtain();
	bg = GUI_conText.Bcolor;
	fg = GUI_conText.Fcolor;
	py[offset++] = code_0;
	GUI_window_hwin_release(hp);
	p  = GUI_read_memBlock(ABLK_XS,ABLK_YS,ABLK_XE,ABLK_YE);
	_ABC_WIN  = _ABCWINpaint();
	GUI_TopWIN_Set(_ABC_WIN);
	_ABC_init(py);
	_ABC_PY_display(_ABC_arry[0],offset,sel);
	pCSR=GUI_cursor_start(_ABC_WIN,2,24);		   
	while ( 1 )
	{
		msleep( TYPE_DELAY );
		if(offset!=offs)
		{
           offs = offset;
		   if(offs>0){_ABC_init(py);sel = 0;}
		   if(_ABC_pos==0)
		   		_ABC_PY_display(py,offset,sel);
		   else
		   		_ABC_PY_display(_ABC_arry[0],offset,sel);
		}
		if ( app_global_key_force_obtain( &key, 1 ) != 1 )
		{
			continue;
		}
		if ( key == KEY_DEL )  
		{ 
		   if(offset>0)
		   {
			 py[offset-1] = 0;
			 offset--;
		   }
		}
		else if(key == KEY_RIGHT&&_ABC_pos!=0&&sel<(_ABC_pos-1))
		{
		   _ABC_PY_display(_ABC_arry[0],offset,++sel);
		}
		else if(key == KEY_LEFT&&_ABC_pos!=0&&sel>0)
		{
		   _ABC_PY_display(_ABC_arry[0],offset,--sel);
		}
		else if(key == KEY_DOWN&&_ABC_pos!=0&&(sel+3)<=(_ABC_pos-1))
		{
		   	sel+=3;
		   	_ABC_PY_display(_ABC_arry[0],offset,sel);
		}
		else if(key == KEY_UP&&_ABC_pos!=0&&sel>=3)
		{
		   sel-=3;
		   _ABC_PY_display(_ABC_arry[0],offset,sel);
		}
		else if (( app_global_decide_PYkey( key ))!= FAIL)
		{
		    if(offset<6)py[offset++] = _ABCcode_Get(key);
		}
		else if ( key == KEY_ENTER)
		{
		    ret = global_strlen(_ABC_arry[sel]);
			global_strcpy(hzbuf,_ABC_arry[sel]);
			break;
		}
		else if(key == KEY_EXIT)
		{
		    ret = 0;
		    break;
		}
	}
	GUI_cursor_stop(pCSR);
	_ABCWINdestory(_ABC_WIN);
	GUI_write_memBlock(p);
	GUI_window_hwin_req(hp);
	GUI_set_Fgcolor(fg);
	GUI_set_Bgcolor(bg);
	return ret;
}

void key_input_ABC_xy_set( uint16_t x, uint16_t y )
{
    _ABC_win_x = x;
    _ABC_win_y = y;
    return;
}
int key_method_ABC( uint8_t code_0, char*hzbuf )
{
    _ABC_mode = 0;
	return _ABCcode_obtain(code_0,hzbuf);;
}

int key_method_abc( uint8_t code_0, char*hzbuf )
{
    _ABC_mode = 1;
	return _ABCcode_obtain(code_0,hzbuf);;
}

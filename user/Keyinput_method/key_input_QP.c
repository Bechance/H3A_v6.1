#include 		"key_input method.h"
#define         MAX_N          50

static hwin_t*	_pyWIN;
static hwin_t*	_hzWIN;
char  	_QP_arry[MAX_N][7];
int   	_QP_pos = 0;
static char  	_QP_code[7]="";
static int      _QP_win_x = 30;
static int      _QP_win_y = 350;
static char*    _hzbox;

#define         QPFONT        (&GUI_HZK24_EXTASCII)

#define         QPWIN_WIDE    400
#define         QPWIN_HIGH    105

#define         PYWIN_WIDE  (250)
#define         HZWIN_WIDE  (QPWIN_WIDE-PYWIN_WIDE)


#define         BLK_XS      (_QP_win_x-2)
#define         BLK_YS      (_QP_win_y-2)
#define         BLK_XE      (BLK_XS+QPWIN_WIDE+4)
#define         BLK_YE      (BLK_YS+QPWIN_HIGH+4)

#define         QP_BG_COLOR    C_GUI_WHITE-1
#define         QP_FG_COLOR    0

static  hwin_t* _WINpaint(void)
{
    hwin_t*pWIN;
	GUI_window_hwin_req(GET_GUIWIN);
	pWIN=GUI_window_creat(_QP_win_x,_QP_win_y,QPWIN_WIDE,QPWIN_HIGH,0);
	GUI_set_Bgcolor(QP_BG_COLOR);
	GUI_set_Fgcolor(QP_FG_COLOR);
    _pyWIN = GUI_window_child_creat(0,0,PYWIN_WIDE,QPWIN_HIGH,WIN_PAINT|WIN_FILL,pWIN);
    _hzWIN = GUI_window_child_creat(PYWIN_WIDE,0,HZWIN_WIDE,QPWIN_HIGH,WIN_PAINT|WIN_FILL,pWIN);
	GUI_window_hwin_release(GET_GUIWIN);
	return pWIN;
}

static void _WINdestory(hwin_t*pObj)
{
    GUI_window_destory(pObj);
}

static  int _Get_code0_len(char code0)
{
   if(code0 == 'w'||code0 == 'p') return 4;
   return 3;
}

static void _QPcode_save(void)
{
	uint8_t  code0;
	uint16_t offset,dx;
	const char**p1;
	char     buf[8]="";
	PY_num_count();
	code0   =  _QP_code[0]-'a';
	p1      =  PY_path[code0];
	if(_QP_code[1]!=0)            //‘ÀÀ„Ã·ÀŸ
	{
		for(offset=0;offset<py_Head_aToz_NUm[code0];offset++)
		{
			if(p1[offset][0]==_QP_code[1])break;
		}
		if(offset==py_Head_aToz_NUm[code0])return;
	}
	if(_QP_code[2]!=0)
	{
		for(offset=0;offset<py_Head_aToz_NUm[code0];offset++)
		{
			if(p1[offset][1]==_QP_code[2])break;
		}
		if(offset==py_Head_aToz_NUm[code0])return;
	}
	if(_QP_code[3]!=0)
	{
		for(offset=0;offset<py_Head_aToz_NUm[code0];offset++)
		{
			if(p1[offset][2]==_QP_code[3])break;
		}
		if(offset==py_Head_aToz_NUm[code0])return;
	}
	if(_QP_code[4]!=0)
	{
		for(offset=0;offset<py_Head_aToz_NUm[code0];offset++)
		{
			if(p1[offset][3]==_QP_code[4])break;
		}
		if(offset==py_Head_aToz_NUm[code0])return;
	}
	for(offset=0;offset<py_Head_aToz_NUm[code0];offset++)
	{
        buf[0] = _QP_code[0];buf[1] = 0;
	    global_strcat(buf,(char*)p1[offset]);
		global_string_filter(buf,' ');
		dx = global_strlen(buf)-global_strlen(_QP_code);
		if(global_memcmp(_QP_code,buf,global_strlen(_QP_code))&&dx<2)
		{
			if(_QP_pos<(MAX_N-1))
			{
				global_strcpy(_QP_arry[_QP_pos],buf);
				_QP_pos++;
			}
		}
	}
    return;
}


void _QPcode_sys(char*str,int n)
{
    int offset;
	int code0_len;
	code0_len   = _Get_code0_len(str[0]);
    for(offset=0;offset<code0_len;offset++)
    {
        _QP_code[n] = str[0]+offset;
		if(str[1]!=0)
		{
        	_QPcode_sys(&str[1],n+1);
		}
		else
		{
			_QPcode_save();
		}
	}
	return;
}

static void _QP_init(char*str)
{
	int xof,yof;
	char code[7];
	int  len;
	int  dx1,dx2;
	len = global_strlen(str);
	GLOBAL_MEMCLR(_QP_arry,sizeof(_QP_arry));
	GLOBAL_MEMCLR(_QP_code,sizeof(_QP_code));
	_QP_pos = 0;
	_QPcode_sys(str,0);
	for(xof=0;xof<MAX_N;xof++)
	{
		for(yof=xof+1;yof<MAX_N;yof++)
		{
		    if(_QP_arry[yof][0]==0)
				dx1 = 0xff;
			else
		    	dx1 = global_strlen(_QP_arry[yof])-len;
		    if(_QP_arry[xof][0]==0)
				dx2 = 0xff;
			else
		    	dx2 = global_strlen(_QP_arry[xof])-len;
			if(dx1<dx2)
			{
				global_memcpy(code,_QP_arry[xof],7);
				global_memcpy(_QP_arry[xof],_QP_arry[yof],7);
				global_memcpy(_QP_arry[yof],code,7);
			}
		}
	}
	return;
}


static void _QP_string_display(int x,int y,char*str,int sel,int ofs)
{
    if(str[0] == 0) return;
	if(ofs>=MAX_N)return;
	if(ofs==sel)
	{
		GUI_set_Bgcolor(QP_FG_COLOR);
		GUI_set_Fgcolor(C_GUI_YELLOW);
		GUI_string_display_At(x,y,str);
	}
	else
	{
		GUI_set_Fgcolor(QP_FG_COLOR);
		GUI_set_Bgcolor(QP_BG_COLOR);
		GUI_string_display_At(x,y,str);
	}
	return;
}

static void _QP_HZ_display(int sel,int state,int page)
{
    int x,y,offset;
	char    hz_buf[3];
	if(state==0)
	{
		_hzbox = ( char* )py_ime((uint8_t*)_QP_arry[sel]);
	}
	GUI_window_hwin_req(_hzWIN);
    GUI_SetFront(QPFONT);
	GUI_set_Bgcolor(QP_BG_COLOR);
    GUI_clean_window();
	for(offset=0;offset<9;offset++)
	{
		global_memcpy(&hz_buf[0],&_hzbox[offset*2+page*18],2);
		hz_buf[2] = ASCII_NULL;
		x = offset%3;
		y = offset/3;
		GUI_set_Fgcolor(QP_FG_COLOR);
		GUI_string_display_At(20+x*45,y*30+10,(char* )hz_buf );
		if(state==1&&hz_buf[0]!=0)
		{
			GUI_set_Fgcolor(C_GUI_RED);
		    hz_buf[0] = GLOBAL_NTOC(offset);
		    hz_buf[1] = ASCII_NULL;
			GUI_string_display_At(8+x*45,y*30+10,(char* )hz_buf );
		}
	}
	GUI_window_hwin_release(_hzWIN);
    return;
}


static void _QP_PY_display(char*str,int offset,int sel)
{
    int x;
    char buf[8]="";
	int ofs = (sel/9)*9;
	x = _pyWIN->win_wide/3;
	global_strcpy(buf,str);
	GUI_window_hwin_req(_pyWIN);
	GUI_set_Bgcolor(QP_BG_COLOR);
	GUI_set_Fgcolor(QP_FG_COLOR);
    GUI_SetFront(QPFONT);
    GUI_clean_window();
    _QP_string_display(2+x*0,1+25*1,_QP_arry[ofs],sel,ofs);ofs++;
    _QP_string_display(2+x*1,1+25*1,_QP_arry[ofs],sel,ofs);ofs++;
    _QP_string_display(2+x*2,1+25*1,_QP_arry[ofs],sel,ofs);ofs++;
    _QP_string_display(2+x*0,1+25*2,_QP_arry[ofs],sel,ofs);ofs++;
    _QP_string_display(2+x*1,1+25*2,_QP_arry[ofs],sel,ofs);ofs++;
    _QP_string_display(2+x*2,1+25*2,_QP_arry[ofs],sel,ofs);ofs++;
    _QP_string_display(2+x*0,1+25*3,_QP_arry[ofs],sel,ofs);ofs++;
    _QP_string_display(2+x*1,1+25*3,_QP_arry[ofs],sel,ofs);ofs++;
    _QP_string_display(2+x*2,1+25*3,_QP_arry[ofs],sel,ofs);ofs++;
	if(global_strlen(buf)>offset)
	{
	   buf[offset] = 0;
	}
	GUI_set_Bgcolor(QP_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_RED);
    GUI_string_display_At(2,1,buf);
	GUI_set_Fgcolor(QP_FG_COLOR);
	GUI_WINDIS_HLINE(0,_pyWIN->win_wide,26);
	GUI_window_hwin_release(_pyWIN);
    _QP_HZ_display(sel,0,0);
	return;
}


static uint8_t _QP_wait_Nkey( void )
{
	uint8_t  key;
	char     temp;
	int      page = 0;
	int      page_n = global_strlen(_hzbox)/18;
	while ( 1 )
	{
		if ( app_global_key_force_obtain( &key, 1 ) == 1 )
		{
			temp = app_global_decide_Nkey( key );
			if ( temp != FAIL )
			{
				temp = GLOBAL_CTON( temp );
				if ( temp < 9 )
					break;
			}
			else if(key == KEY_UP)
			{
			    if(page>0)
			    {
					page--;
					_QP_HZ_display(0,1,page);
			    }
			}
			else if(key == KEY_DOWN)
			{
			    if(page<page_n)
			    {
					page++;
					_QP_HZ_display(0,1,page);
			    }
			}
			else if(key == KEY_EXIT )
			{
				return 1000;
			}
		}
		msleep( 1 );
	}
	return (2*temp+page*18);
}


void key_input_QP_xy_set( uint16_t x, uint16_t y )
{
    _QP_win_x = x;
    _QP_win_y = y;
    return;
}

int key_method_QP( uint8_t code_0, char*hzbuf )
{
	uint8_t   	key;
	GUI_BLK_t*	p;
	hwin_t*   	pWIN,*hp;
    GUI_csr_t*	pCSR;
	char  		py[8]="";
	uint16_t    offset = 0;
	uint16_t   	offs   = 1;
	uint16_t   	sel    = 0;
	uint16_t   	ret    = 0;
	uint16_t 	bg,fg;
	if(app_global_decide_PYkey(code_0)==0)return 0;
	code_0 =  app_global_keytoletter(code_0);
	hp = GUI_CURwin_obtain();
	bg = GUI_conText.Bcolor;
	fg = GUI_conText.Fcolor;
	py[offset++] = code_0;
	GUI_window_hwin_release(hp);
	p     = GUI_read_memBlock(BLK_XS,BLK_YS,BLK_XE,BLK_YE);
	pWIN  = _WINpaint();
	GUI_TopWIN_Set(pWIN);
	_QP_init(py);
	_QP_PY_display(_QP_arry[0],offset,sel);
	pCSR=GUI_cursor_start(_pyWIN,2,24);		   
	while ( 1 )
	{
		msleep( TYPE_DELAY );
		if(offset!=offs)
		{
           offs = offset;
		   if(offs>0){_QP_init(py);sel = 0;}
		   if(_QP_pos==0)
		   		_QP_PY_display(py,offset,sel);
		   else
		   		_QP_PY_display(_QP_arry[0],offset,sel);
		}
		if(app_global_key_force_obtain(&key,1)!=1)
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
		else if(key == KEY_RIGHT&&_QP_pos!=0&&(sel+1)<(_QP_pos))
		{
		   _QP_PY_display(_QP_arry[0],offset,++sel);
		}
		else if(key == KEY_LEFT&&_QP_pos!=0&&sel>0)
		{
		   _QP_PY_display(_QP_arry[0],offset,--sel);
		}
		else if(key == KEY_DOWN&&_QP_pos!=0&&(sel+3)<(_QP_pos))
		{
		   	sel+=3;
		   	_QP_PY_display(_QP_arry[0],offset,sel);
		}
		else if(key == KEY_UP&&_QP_pos!=0&&sel>=3)
		{
		   sel-=3;
		   _QP_PY_display(_QP_arry[0],offset,sel);
		}
		else if (( app_global_decide_PYkey( key ))!= FAIL)
		{
		    if(offset<6)py[offset++] = app_global_keytoletter(key);
		}
		else if ( key == KEY_ENTER)
		{
            _QP_HZ_display(sel,1,0);
			ret = _QP_wait_Nkey();
			if(ret!=1000)
			{
				global_memcpy( hzbuf,&_hzbox[ret],2);
				ret = 2;
				break;
			}
            _QP_HZ_display(sel,0,0);
		}
		else if(key == KEY_EXIT)
		{
		    ret =0;
		    break;
		}
	}
	GUI_cursor_stop(pCSR);
	_WINdestory(pWIN);
	GUI_write_memBlock(p);
	GUI_window_hwin_req(hp);
	GUI_set_Fgcolor(fg);
	GUI_set_Bgcolor(bg);
	return ret;
}

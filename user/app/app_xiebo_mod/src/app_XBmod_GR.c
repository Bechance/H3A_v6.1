/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:系统设置模块主体程序
* 创建时间:2010-08-02
* 所属公司 :科荟
* 文件名称:app_XBmod_fun.c
* 创建人 :揭成
* 功能描述:实现系统设置功能
******************************************************************************************/
#include	"app_XBmod_inside.h"
#include	<stdlib.h> 
static  	a_sem_t*	_GR_sem     = INVALID_SEM;
static      int 		_GR_CURxb_pos = 0;
static      int         _GR_CURpage;
static      float 		_GR_XBdata[6][52];
static      hwin_t*     _phaseR;
/********************************************************************
 * @创建人 :揭成
 * @功能 :根据需要排列数据
 * @输入 :
 * @输出 :NONE
********************************************************************/
static void _GR_sort_data(Bsize_t flag,xb_data_t*pdata,float*fdata)
{
	Bsize_t xoffset,yoffset;
	xb_data_t bak;
	for(xoffset=0;xoffset<55;xoffset++)
	{
	  pdata[xoffset].offset = xoffset;
	  if(xoffset<52)
	  	pdata[xoffset].Usage  = fdata[xoffset];
	  else
	  	pdata[xoffset].Usage  = 0;
	  if(GLOBAL_ABS(pdata[xoffset].Usage)>100)
	  	pdata[xoffset].Usage  = 0;
	}
	if(!flag) return;
	for(xoffset=2;xoffset<52;xoffset++)
	{
		for(yoffset=xoffset+1;yoffset<52;yoffset++)
		{
			if(pdata[yoffset].Usage>pdata[xoffset].Usage)
			{
				global_memcpy(&bak,&pdata[xoffset],sizeof(xb_data_t));
				global_memcpy(&pdata[xoffset],&pdata[yoffset],sizeof(xb_data_t));
				global_memcpy(&pdata[yoffset],&bak,sizeof(xb_data_t));
			}
		}
	}
   return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :获取最大值
 * @输入 :
 * @输出 :NONE
********************************************************************/
static void _GR_GETmax(float*pmax,float*fdata)
{
	Bsize_t xoffset = 2;
	float fbak;
	fbak = fdata[xoffset];
	for(xoffset=2;xoffset<52;xoffset++)
	{
         fbak=MAX(fdata[xoffset],fbak);
	}
	*pmax = fbak;
	return;
}


static int _GR_GETlen(float data,int max,float fmax)
{
    float relen;
	if(data==0) return 0;
    relen = data*max/fmax;
	if(relen<1&&data!=0)
	{
       relen = 1;
	}
	return (int)relen;
}

static void _GRXBview(int start_n,int end_n,BOOL Is_sort)
{
	xb_data_t   Uxb_data[55];
	xb_data_t   Ixb_data[55];
	float   	fUmax,fImax;
    int         len,cnt;
	_GR_GETmax(&fUmax,_GR_XBdata[_GR_CURxb_pos]);
	_GR_GETmax(&fImax,_GR_XBdata[_GR_CURxb_pos+1]);
	app_XBmod_THDdisplay(_GR_XBdata[_GR_CURxb_pos][0],GET_DOBJ(0));
	app_XBmod_THDdisplay(_GR_XBdata[_GR_CURxb_pos+1][0],GET_DOBJ(1));
	_GR_sort_data(Is_sort,Uxb_data,_GR_XBdata[_GR_CURxb_pos]);
	_GR_sort_data(Is_sort,Ixb_data,_GR_XBdata[_GR_CURxb_pos+1]);
	for(cnt = start_n;cnt<(end_n);cnt++)
	{
		len = _GR_GETlen(Uxb_data[cnt].Usage,100,fUmax);
		app_XBmod_graph_display(len,cnt,&Uxb_data[cnt],GET_DOBJ(0));
		len = _GR_GETlen(Ixb_data[cnt].Usage,100,fImax);
		app_XBmod_graph_display(len,cnt,&Ixb_data[cnt],GET_DOBJ(1));
	}
    return;
}

/********************************************************************
 * @创建人 :揭成
 * @功能 :WAVE模块显示任务
 * @输入 :NONE
 *@输出  :NONE
********************************************************************/
static void _GRdisplay_task( void*arg )
{
	Bsize_t 	page 	= 0;
	BOOL    	Is_sort = 0;
	int     	start_n = 2,end_n = 19;
	hwin_t* 	checkbox,*phaseR;
	DL_flag_t 	dl_flag;
	GLOBAL_MEMCLR(_GR_XBdata,sizeof(float)*6*52);
	app_XBmod_window_create();
	phaseR   = GET_DOBJ(2);
	_phaseR  = phaseR;
	checkbox = GUI_childobj_obtian(GUI_KEY_SELECT_WAY,WIDGET_CHECK_BOX,GET_DOBJ(2));
	GUI_radio_select(phaseR,0);
	while(1)
	{
		if ( os_task_delete_req( SELF_PRO ) == OS_TASK_DEL_REQ )
		{
			os_task_delete( SELF_PRO );   			
		}
		if(os_sem_pend(_GR_sem,APP_100_MS*5) == OS_NO_ERR )
		{
		    page       = _GR_CURpage;
			_GR_CURxb_pos = GUI_radio_curmember_obtain(phaseR)*2;
            Is_sort    = GUI_checkbox_state_obtain(checkbox);
			start_n    = 2+17*page;
			end_n      = start_n+17; 
			app_XBmod_ruler_clean();
			os_driver_ioctl(g_DL_dev,MEASURE_CMD_XBDATA_GET,_GR_XBdata);
			_GRXBview(start_n,end_n,Is_sort);
			GUI_window_hwin_req(GET_DOBJ(2));
			GUI_set_Bgcolor(GUI_GetwinBgcor());
			GUI_set_Fgcolor(C_GUI_WHITE);
			GUI_SetFront(&GUI_HZK16_EXTASCII);
		    GUI_string_display_At(2,10,"第");
		    GUI_int_display(page+1);
		    GUI_string_display("页");
			GUI_window_hwin_release(GET_DOBJ(2));	
		}
		app_global_DLflag_obtain(&dl_flag);
		if(dl_flag.DL_XB_is_new)
		{
			os_driver_ioctl(g_DL_dev,MEASURE_CMD_XBDATA_GET,_GR_XBdata);
	        _GRXBview(start_n,end_n,Is_sort);
		}

	}
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :WAVE模块主任务
 * @输入 :NONE
 *@输出  :NONE
********************************************************************/
void app_XBmod_GRtask( void*arg )
{
	uint8_t key_buf;
	uint8_t id;
	_GR_CURpage = 0;
	os_sem_creat(&_GR_sem,1);
	id= os_task_assign_stk_build( _GRdisplay_task,10);
	msleep(APP_100_MS);
	while(1)
	{
		msleep(TYPE_DELAY);
		if ( os_task_delete_req( SELF_PRO ) == OS_TASK_DEL_REQ )
		{
			OS_DELETE_REQ( id );
		    app_XBmod_window_destory();
			os_task_delete( SELF_PRO );   			
		}
		if ( GUI_key_read(m_XBmod_win_Obj,&key_buf, 1 ) == 1 ) 
		{
			if(key_buf==KEY_RIGHT)
			{
			    _GR_CURpage = (_GR_CURpage==2)?(0):(_GR_CURpage+1);
 				os_sem_post(_GR_sem);
			}	
			else if(key_buf==KEY_LEFT)
			{
			    _GR_CURpage = (_GR_CURpage==0)?(2):(_GR_CURpage-1);
 				os_sem_post(_GR_sem);
			}	
			else if(key_buf==KEY_F1)
			{
				GUI_radio_select(_phaseR,0);
				_GR_CURpage = 0;
				os_sem_post(_GR_sem);
			}
			else if(key_buf==KEY_F2)
			{
				GUI_radio_select(_phaseR,1);
				_GR_CURpage = 0;
				os_sem_post(_GR_sem);
			}
			else if(key_buf==KEY_F3)
			{
				GUI_radio_select(_phaseR,2);
				_GR_CURpage = 0;
				os_sem_post(_GR_sem);
			}			
			else if(key_buf==KEY_F4)
			{
				GUI_checkbox_state_modify(GUI_Getchild(GUI_KEY_SELECT_WAY,GET_DOBJ(2)));
				os_sem_post(_GR_sem);
			}
		}
	}
}

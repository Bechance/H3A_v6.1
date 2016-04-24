/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:ϵͳ����ģ���������
* ����ʱ��:2010-08-02
* ������˾ :����
* �ļ�����:app_wavemod_fun.c
* ������ :�ҳ�
* ��������:ʵ��ϵͳ���ù���
******************************************************************************************/
#include	"app_wavemod_inside.h"
#include	<stdlib.h> 
#define     __YD      1.5
static  float        _YD     = __YD;
static  uint8_t 	_task_id = INVALID_PRO;
static  a_sem_t*	_sem     = INVALID_SEM;
static  m_lastxy_t 	_UIlastxy[6][1024];
static  float       _yKUI[6]={1,1,1,1,1,1};
static  float       _UImax[6],_UImaxK[6];
correct_data_t  	m_Wavemod_Dcorrect;


static void _ruler_display(void)
{
}
static void _FDL_view(float fdata,int id)
{
    if(fdata==0){GUI_edit_string_display_At("0.0000",id);return;}
	GUI_edit_float_display_at(fdata,id,6);
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :�������ʾ
 * @���� :
 * @��� :NONE
********************************************************************/
static void _DLarg_display(measure_param_t*pDL)
{
	GUI_window_hwin_req(m_WaveDL);
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_YELLOW);
	_FDL_view(pDL->phase_a.f_U,EDIT_U_A);
	_FDL_view(pDL->phase_a.f_I,EDIT_I_A);
	_FDL_view(pDL->phase_a.f_J,EDIT_J_A);
	_FDL_view(pDL->phase_b.f_U,EDIT_U_B);
	_FDL_view(pDL->phase_b.f_I,EDIT_I_B);
	_FDL_view(pDL->phase_b.f_J,EDIT_J_B);
	_FDL_view(pDL->phase_c.f_U,EDIT_U_C);
	_FDL_view(pDL->phase_c.f_I,EDIT_I_C);
	_FDL_view(pDL->phase_c.f_J,EDIT_J_C);
	_FDL_view(pDL->f_Feq, EDIT_FEQ);
	GUI_window_hwin_release(m_WaveDL);
	return;
}


static void _UItop_obtain(int pos)
{
	int16_t*pbuf[6];
	int16_t max,id;
	Bsize_t offset;
	float  fmax;
	id = id;
	os_driver_ioctl(g_DL_dev, MEASURE_CMD_GET_AD_DATA,pbuf);
    max = pbuf[pos][0];
	for(offset=1;offset<1024;offset++)
	{
	  max=MAX(max,pbuf[pos][offset]);
	}
	if(pos==0)
		fmax = g_DL_param.phase_a.f_U;
	else if(pos==1)
		fmax = g_DL_param.phase_a.f_I;
	else if(pos==2)
		fmax = g_DL_param.phase_b.f_U;
	else if(pos==3)
		fmax = g_DL_param.phase_b.f_I;
	else if(pos==4)
		fmax = g_DL_param.phase_c.f_U;
	else if(pos==5)
		fmax = g_DL_param.phase_c.f_I;	
	if(fmax==0){_YD= 10;fmax=1.0;}
	else       _YD = __YD;
	_yKUI[pos]   = fmax*1.414/max;
	_UImax[pos]  = fmax*1.414*_YD;
	_UImaxK[pos] = -1.0/_UImax[pos];
	_ruler_display();
	return;
}


static void _xy_obtain(hwin_t*hp,int16_t data,int pos,int*ry)
{
	float k,b,freal;
	freal = data*_yKUI[pos];
	b = (float)hp->win_high*0.5;
	k = (float)b*_UImaxK[pos];
	*ry= (int)(k*freal+b);
	if(*ry<1)*ry=1;
	if(*ry>hp->win_high)*ry=hp->win_high-1;
	return;
}


static void _display(int16_t*pData,hwin_t*hp,int pos,BOOL  is_first,m_lastxy_t*plast)
{
	int  last_x,last_y,x,y;
	Bsize_t   offset;
	uint16_t  color;
	float     _xstep;
	_UItop_obtain(pos);
	_xstep = (float)hp->win_wide/1024;
	app_wavemod_window_line_draw(hp);
    color =m_WaveCor[pos];
	for ( offset = 0;offset <1024;offset++)
	{
		GUI_window_hwin_req( hp );
		GUI_set_Bgcolor( C_GUI_BlACK);
        _xy_obtain(hp,pData[offset],pos,&y);
		x = (int)(_xstep*offset);
		if(offset>0)
		{
			GUI_set_Fgcolor(color);
			GUI_draw_line(
            hp->win_x+(int)last_x,
            hp->win_y+(int)last_y,
            hp->win_x+(int)x,
            hp->win_y+(int)y);
		}
		if(!is_first&&(offset<(1024-1)))
		{
			GUI_set_Fgcolor(C_GUI_BlACK);
	        GUI_draw_line(
				hp->win_x+plast[offset].x,
				hp->win_y+plast[offset].y,
				hp->win_x+plast[offset+1].x,
				hp->win_y+plast[offset+1].y);
		}
        last_x = x;
		last_y =y;
		plast[offset].x=(uint16_t)x;
		plast[offset].y=(uint16_t)y;
		GUI_window_hwin_release( hp );
	}	
    return;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :������ʾ����
 * @���� :
 * @��� :NONE
********************************************************************/
static void _display_task(void*arg)
{
	int16_t     *pbuf[6];
	int         pos,cnt=0;
	BOOL        UIEN[6];
	hwin_t*     hWave = m_WaveWIN;
	DL_flag_t   DL_flag;
	DL_flag = DL_flag;
	GLOBAL_MEMCLR(_UIlastxy,sizeof(m_lastxy_t)*1024*6);
	os_driver_ioctl(g_DL_dev, MEASURE_CMD_GET_AD_DATA,pbuf);
	GUI_checkBox_select(m_UIcheckbox[0],1);
    GUI_checkBox_select(m_UIcheckbox[1],1);
	while(1)
	{
		if ( os_task_delete_req( SELF_PRO ) == OS_TASK_DEL_REQ )
		{
			os_task_delete( SELF_PRO );   			
		}
		app_global_DLarg_obtain(&g_DL_param);
		if(os_sem_pend(_sem,TYPE_DELAY) == OS_NO_ERR )
		{
		    for(pos=0;pos<6;pos++)
		    UIEN[pos] = GUI_checkbox_state_obtain(m_UIcheckbox[pos]);
			GUI_window_hwin_req(hWave);
			GUI_set_Bgcolor( C_GUI_BlACK);
			GUI_clean_window();
			GUI_window_hwin_release(hWave);
		    for(pos=0;pos<6;pos++){
            if(UIEN[pos])_display(pbuf[pos],hWave,pos,1,_UIlastxy[pos]);}
		}
		if(++cnt%5==0)
		{
	        _DLarg_display(&g_DL_param);
		}
	    for(pos=0;pos<6;pos++){
        if(UIEN[pos])_display(pbuf[pos],hWave,pos,0,_UIlastxy[pos]);}
	}
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :WAVEģ��������
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
static void _task( void*arg )
{
	uint8_t id;
	uint8_t key_buf;
	os_sem_creat(&_sem,1);
	app_wavemod_window_creat();
	GUI_WINready(m_UIcheckbox[0],1);
	id= os_task_assign_stk_build( _display_task, 3);
	while ( 1 )
	{
		msleep(TYPE_DELAY);
		if ( os_task_delete_req( SELF_PRO ) == OS_TASK_DEL_REQ )/*�ж��Ƿ���ɾ������*/
		{
		    OS_DELETE_REQ( id );
			os_task_delete( SELF_PRO );   			
		}
		if ( app_global_key_obtain( &key_buf, 1 ) == 1 )
		{
			if ( key_buf==KEY_F1)
			{
			    GUI_checkbox_state_modify(m_UIcheckbox[0]);
				os_sem_post(_sem);
			}
			else if ( key_buf==KEY_F2)
			{
			    GUI_checkbox_state_modify(m_UIcheckbox[1]);
				os_sem_post(_sem);
			}
			else if ( key_buf==KEY_F3)
			{
			    GUI_checkbox_state_modify(m_UIcheckbox[2]);
				os_sem_post(_sem);
			}
			else if ( key_buf==KEY_F4)
			{
			    GUI_checkbox_state_modify(m_UIcheckbox[3]);
				os_sem_post(_sem);
			}
			
			else if ( key_buf==KEY_F5)
			{
			    GUI_checkbox_state_modify(m_UIcheckbox[4]);
				os_sem_post(_sem);
			}
			else if ( key_buf==KEY_0)
			{
			    GUI_checkbox_state_modify(m_UIcheckbox[5]);
				os_sem_post(_sem);
			}	
		 	else if(key_buf==KEY_LEFT)
		  	{
             	GUI_WINshift(1);
		  	}
		  	else if(key_buf==KEY_RIGHT)
		  	{
             	GUI_WINshift(0);
		  	}			 
		  	else if(key_buf==KEY_ENTER)
		  	{
             	GUI_WINdo();
				os_sem_post(_sem);
		  	}	
			else if(key_buf==KEY_EXIT)
			{
				//app_module_assignExt("START");
				app_module_switch_last_mod();
			}
		}
	}
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :��ģ�������ܳ�ʼ��
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void app_wavemod_fun_entry( void )
{
	app_global_DL_measure_entery();
	_task_id= os_task_assign_stk_build( _task, 3);
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :��ģ���������ͷ���Դ
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void app_wavemod_fun_exit( void )
{
	if ( _task_id != INVALID_PRO )
	{
		OS_DELETE_REQ( _task_id );
		_task_id = INVALID_PRO;
	}
	app_global_DL_measure_exit();
	return;
}

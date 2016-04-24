/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:ϵͳ����ģ���������
* ����ʱ��:2012-04-01
* ������˾ :����
* �ļ�����:app_ERRmod_Ucorrect.c
* ���� :�ҳ�
* ��������:��ѹ��������
******************************************************************************************/
#include				"app_ERRmod_inside.h"

/********************************************************************
 * @���� :�ҳ�
 * @���� :��ѹʵ��ֵ��ʾ
 *
 * @���� :
 *
 *@��� : NONE
********************************************************************/
void app_ERRmod_UDLarg_display(correct_option_t*Pbase,measure_param_t*pDL)
{
    float fbase;
    float fb;
    fbase = global_atof(Pbase->c_base_U);
	GUI_window_hwin_req(GET_DOBJ(UCORRECT_EDIT));
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	
	GUI_set_Fgcolor(C_GUI_DLA_COLOR);
    app_ERRmod_DL_float_display(pDL->phase_a.f_U,ERR_U_A);
	GUI_set_Fgcolor(C_GUI_DLB_COLOR);
    app_ERRmod_DL_float_display(pDL->phase_b.f_U,ERR_U_B);
	GUI_set_Fgcolor(C_GUI_DLC_COLOR);
	app_ERRmod_DL_float_display(pDL->phase_c.f_U,ERR_U_C);

	GUI_set_Fgcolor(C_GUI_YELLOW);
	fb = (fbase-pDL->phase_a.f_U)/fbase*100;
    app_ERRmod_DL_float_display(fb,ERR_CORRECT_A);
	
	fb = (fbase-pDL->phase_b.f_U)/fbase*100;
    app_ERRmod_DL_float_display(fb,ERR_CORRECT_B);
	
	fb = (fbase-pDL->phase_c.f_U)/fbase*100;
    app_ERRmod_DL_float_display(fb,ERR_CORRECT_C);
	
	GUI_window_hwin_release(GET_DOBJ(UCORRECT_EDIT));
    return;
}
/********************************************************************
 * @���� :�ҳ�
 * @���� :��ѹ������ʾ
 * @���� :
 * @��� : NONE
********************************************************************/
void app_ERRmod_Ucorrect_arg_display(correct_option_t*Pbase)
{
	Bsize_t level;
	float fbaseU = global_atof(Pbase->c_base_U);
	GUI_window_hwin_req(GET_DOBJ(UCORRECT_EDIT));
	GUI_set_Fgcolor(ERRmod_FG_COLOR);
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
    GUI_edit_string_display_At(Pbase->c_base_U,ERR_BASE_U);
    if(fbaseU<70) 
    {
	    GUI_edit_string_display_At("55V��",ERR_U_LEVEL);
		level = U_INPUT_LEVEL_57V;
    }
    else if(fbaseU<160) 
    {
	    GUI_edit_string_display_At("100V��",ERR_U_LEVEL);
		level = U_INPUT_LEVEL_100V;
    }
    else if(fbaseU<280) 
    {
	    GUI_edit_string_display_At("220V��",ERR_U_LEVEL);
		level = U_INPUT_LEVEL_220V;
    }
    else 
    {
	    GUI_edit_string_display_At("440V��",ERR_U_LEVEL);
		level = U_INPUT_LEVEL_440V;
    }
	os_driver_ioctl(g_DL_dev,MEASURE_CMD_SET_UDL_LEVEL,&level);
	GUI_window_hwin_release(GET_DOBJ(UCORRECT_EDIT));
	return;
}
/********************************************************************
 * @���� :�ҳ�
 * @���� :��ѹ������׼���ûص�
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
uint8_t app_ERRmod_Ucorrect_setting_callback(hwin_t*hp)
{
	uint8_t key;
	Bsize_t member;
	correct_option_t   		m_correct_option_shadow;
	hand_wirte_input_box_pos_set(200,250);
	key_input_method_filter(KEY_INPUT_123);;
	global_memcpy(&m_correct_option_shadow,&m_correct_option,sizeof(correct_option_t));
	while ( 1 )
	{
		msleep( TYPE_DELAY );
		if ( app_global_key_obtain( &key, 1 ) != 1 )/*�ж������Ƿ��յ�������Ӧ*/
		{
			continue;
		}
		GUI_window_hwin_req( hp );
		GUI_edit_run_hook( key );
		GUI_set_Fgcolor(ERRmod_FG_COLOR);
		GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
		if (( app_global_decide_inputkey( key ) ) != FAIL )/*�ж��Ƿ������뷨��ذ�������*/
		{
			member = GUI_edit_id_obtain();    			 /*��ȡ��ǰ�༭��ԱID*/
			if ( member == ERR_BASE_U)
			key_method_input_shadow( key,m_correct_option_shadow.c_base_U);
		}
		GUI_window_hwin_release( hp );
		if(key==KEY_EXIT||key==GUI_KEY_EXIT)
		{
			 app_ERRmod_Ucorrect_arg_display(&m_correct_option);
             break;
		}
		if(key==KEY_ENTER||key==GUI_KEY_ENTER)
		{
			global_memcpy(&m_correct_option,&m_correct_option_shadow,sizeof(correct_option_t));
			app_ERRmod_Ucorrect_arg_display(&m_correct_option);
			break;
		}
	}
    return key;
}
/********************************************************************
 * @���� :�ҳ�
 * @���� :��ѹ����
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void app_ERRmod_Ucorrect_task(void*arg)
{
	uint8_t key_buf;
	Bsize_t cnt = 0;
	measure_param_t dl;
	DL_flag_t dl_flag;
	app_ERRmod_correct_step_send( UCORRECT_SETP, 1,4,1);
	app_ERRmod_Uwindow_creat();
	app_ERRmod_Ucorrect_arg_display(&m_correct_option);
	while ( 1 )
	{
		msleep( TYPE_DELAY );
		app_global_DLflag_obtain(&dl_flag);
		if(dl_flag.DL_data_is_new&&(++cnt%30==0))
		{
			app_global_DLarg_obtain(&dl);
            app_ERRmod_UDLarg_display(&m_correct_option,&dl);
		}
		if ( app_global_key_obtain( &key_buf, 1 ) != 1 )
		{
			continue;
		}
		if(key_buf==KEY_EXIT || key_buf==GUI_KEY_EXIT)
		{
           app_ERRmod_Uwindow_destroy();
		   break;
		}
		else if(key_buf==KEY_SETTING || key_buf==GUI_KEY_SET)
		{
			GUI_edit_start(GET_DOBJ(UCORRECT_EDIT),app_ERRmod_Ucorrect_setting_callback);
		}
		else if(key_buf==KEY_F5)
		{
			if(MsgBox(250,100,300,150, "��ʾ",
			   "ȷ��ɾ����ѹ��������",APP_HALF_SEC*100)==GUIE_KEY_ENTER)
			{
				GUI_set_Fgcolor(ERRmod_FG_COLOR);
				GUI_3D_window_title_mod( m_errmod_Ucorrect_win, "����ɾ��,���Ժ�...." );
				os_driver_ioctl(g_DL_dev,MEASURE_CMD_CELAN_CUR_CORRECT_DATA,0);
				msleep( APP_HALF_SEC );
				GUI_3D_window_title_mod( m_errmod_Ucorrect_win, "�ɹ�ɾ����ѹ��������!" );
				msleep( APP_HALF_SEC );
				GUI_3D_window_title_mod( m_errmod_Ucorrect_win, "��ѹ����" );
				
			}
		}
		else if(key_buf==KEY_ENTER || key_buf==GUI_KEY_ENTER)
		{
			GUI_set_Fgcolor(ERRmod_FG_COLOR);
			GUI_3D_window_title_mod( m_errmod_Ucorrect_win, "��ʼ����....." );
			app_ERRmod_correct_step_send( UCORRECT_SETP,2,4,1);
			msleep( APP_HALF_SEC );
			GUI_3D_window_title_mod( m_errmod_Ucorrect_win, "�����������" );
			msleep( APP_HALF_SEC );
			GUI_3D_window_title_mod( m_errmod_Ucorrect_win, "��ѹ����" );
		}
	}
   return;
}

/*************************************************************************
* ģ������:UARTģ����溯������
* ����ʱ��:
* ������˾ :����
* �ļ�����:app_UATTmod_interface.c
* ������ :
* ��������:�ṩPEC-HAD��PCͨѶ��������
***************************************************************************/
#include "app_BLUETOOTHmod_inside.h"


hwin_t *			m_BLUETOOTHmod_win = PNULL;

/*������ʾ�ַ�������*/
const char *	m_BTmod_title[] = {"����ͨѶ", "[bluetooth communication]"};

const char*m_BTmod_option[][2] =
{
	//{"0.������һ��", 	"0.EXIT"}
	{"�˳�", 	"EXIT"}
};

const char *m_BTmod_refresh_option[] =
{
	"�ֶ�У���¼�¼�ϴ���:",
	"�ֶ�У���¼�ϴ���:",
	"�ȴ�����ͨѶ��......",
	"��¼�ϴ����."
};

/********************************************************************
 * @���� :������
 * @���� :����PEC-H3A��PCͨѶ��������
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_BLUETOOTHmod_window_main_creat(void)
{
	hwin_t*hp;
	
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_set_Bgcolor( BTmod_BG_CLOLR );
	GUI_clean_screen();
	GUI_set_Bgcolor( BTmod_WIN_BG_COLOR );
	GUI_set_Fgcolor( BTmod_FG_COLOR );
	
	m_BLUETOOTHmod_win = Gui_3D_window_creat(7, 10, 620, 460, ( char* )m_BTmod_title[g_language], WIN_3D | WIN_FILL, GET_GUIWIN );
	GUI_window_hwin_req( m_BLUETOOTHmod_win );

	app_BLUETOOTHmod_window_refresh_comm_stat(WAIT_FOR_CONNECTION);
	hp = Gui_button_creat(230,192 , 160,48, app_global_Ntokey( 0 ) | BUTTON_PAINT, m_BLUETOOTHmod_win );
	GUI_button_title_display( hp, ( char* )m_BTmod_option[0][g_language] );
	
	GUI_window_hwin_release( m_BLUETOOTHmod_win );
	return;
}
/********************************************************************
 * @���� :������
 * @���� :����ͨѶ��������
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_BLUETOOTHmod_window_destroy( void )
{
	GUI_window_destory( m_BLUETOOTHmod_win );
	return;
}

/********************************************************************
 * @���� :������
 * @���� :��̬ˢ����PCͨѶ״̬
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_BLUETOOTHmod_window_refresh_comm_stat(uint8_t flag)
{	
	char str_temp[60] = "";
	char *ptemp = str_temp;
	
	
	GLOBAL_MEMCLR(str_temp, sizeof(str_temp));
	global_strcat(ptemp,(char *)m_BTmod_refresh_option[flag]);
	ptemp = str_temp + global_strlen(str_temp);
	
	if(flag != WAIT_FOR_CONNECTION)
	{
		global_itoa(BT_current_JB_item_num,ptemp);
		ptemp = str_temp + global_strlen(str_temp);
		global_strcat(ptemp,"/");
		ptemp = str_temp + global_strlen(str_temp);
		global_itoa(BT_total_JB_items_uploaded, ptemp);
	}
	global_strcat(ptemp,"            ");
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_window_hwin_req( m_BLUETOOTHmod_win );
	
	GUI_string_display_At(140,140,"ͨѶ״̬:");
	GUI_string_display_At(262,140,str_temp);

	GUI_window_hwin_release( m_BLUETOOTHmod_win );
}


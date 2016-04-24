/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:�����ֿ�
* ����ʱ��:2010-11-28
* ������˾ :����
* �ļ�����:app_USBmod_hzlib_updata.c
* ������ :�ҳ�
* ��������:�������ֿ�
******************************************************************************************/
#include"app_USBmod_inside.h"
#include <stdlib.h>

/*��������*/
#define  		MAX_HZLIB_BUF_SIZE 			800
#define  		MBLIB_INFO_NAME     		"MBINFO"
#define  		MBLIB_FILE_NAME     		"MABIAOU.TXT"
#define  		HZLIB_FILE_NAME     		"HZK16"
#define  		ASC_FILE_NAME     			"ASC16"
#define  		HZK24_FILE_NAME     		"HZK24"
#define  		USBPLAN_FILE_NAME     		"PEC10PLAN"
#define  		DL_CORRECT_FILE             "DLCOR"			//�������������ļ���
#define  		PLAN_FILE_NAME     		 	"10PLAN.txt"
#define  		PLAN_INFO_NAME  		 	"PLANinfo"	
#define         XCORRD_INFO                 5
#define         YCORRD_INFO                 190
/***************************************************************************/
uint32_t 	USBFS_file_size_obtain(void);
uint32_t 	USBFS_fread(void*pbuf, uint32_t buf_size, uint8_t *flags );
uint32_t 	USBFS_fwrite(void*pbuf, uint32_t buf_size, uint8_t *flags );
uint8_t 	USBFS_fopen(char*file, char mode);
uint8_t 	USBFS_fclose(uint8_t RW);
void  		Gui_progress_mod_setp( hwin_t*hp, uint16_t setp_size );
void 		app_Extapp_updata(void);
/***************************************************************************/
/*���ļ�����*/
hwin_t*				m_hzlib_win = PNULL;
hwin_t*     		m_USB_button = PNULL;
hwin_t*				m_hzlib_progress_bar = PNULL;
hwin_t*				m_hzlib_msg_win = PNULL;
const  char*		m_hzlib_updata_runing[] = {"�������������Ժ�...", ""};
const  char*		m_hzlib_updata_finish[] = {"�������", ""};
const  char*		m_hzlib_updata_err[]    = {"����ʧ�ܣ�����U��", ""};
uint32_t     		m_write_size_already    = 0;
FIL*				m_hzlib_local_file;
/********************************************************************
 * @������ :�ܱ�Ȩ
 * @���� :�������ݱ���
 *
 * @���� :pbuf:���ݻ���;buf_size:���ݴ�С;
 *
 * @��� :�ɹ�:1;ʧ��:0
********************************************************************/
#define    W_SIZE		400
uint32_t   USB_fwirte_char(void*pbuf,uint32_t buf_size,uint8_t *flags)
{
	uint16_t temp;
	char *shadow;
	Bsize_t offset;
	Bsize_t loffset = 0;
	uint32_t r_size = 0;
	shadow=app_malloc(buf_size*2);
	for(offset=0;offset<buf_size;offset++)
	{
		temp = ((uint16_t*)pbuf)[offset];
		shadow[loffset]=(temp&0xff);
		shadow[loffset+1]=((temp&0xff00)>>8);
		loffset+=2;
	}
	r_size = USBFS_fwrite(shadow,(buf_size*2),flags);
	app_free(shadow);
	return r_size;
}
uint8_t	app_correct_data_backup(correct_save_data_t *pbuf,uint32_t b_size)
{
	uint8_t flag;
	uint8_t flags;
	uint8_t offset;
	uint8_t s_num = b_size/W_SIZE;
	
	flag = USBFS_fopen(DL_CORRECT_FILE,WRITE_PIC);
	if ( flag == FILE_OPEN_FAIL )
	{
		return FAIL;
	}
	for ( offset = 0;offset < s_num;offset++ )
	{
		USB_fwirte_char(pbuf,W_SIZE,&flags);
	}
	USB_fwirte_char(pbuf,(b_size % W_SIZE),&flags);
	USBFS_fclose(WRITE_PIC);
	return TURE;
}
/********************************************************************
 * @������ :�ܱ�Ȩ
 * @���� :��ȡ��������
 *
 * @���� :pbuf:���ݻ���;buf_size:���ݴ�С;
 *
 * @��� :�ɹ�:1;ʧ��:0
********************************************************************/
#define    R_SIZE		300
uint32_t   USB_fread_char(void*pbuf,uint32_t buf_size,uint8_t *flags)
{
	char *pst;
	char *shadow;
	Bsize_t offset;
	uint16_t temp;
	uint32_t r_size = 0;
	shadow=app_malloc(buf_size*2);
	if(!shadow) return 0;
	pst = shadow;
	r_size = USBFS_fread(pst,(buf_size*2),flags);
	r_size /= 2;
	for(offset=0;offset<r_size;offset++)
	{
		temp = (*pst)|((*(pst+1))<<8);
		pst+=2;
		((char*)pbuf)[offset] = temp;
	}
	app_free(shadow);
	return r_size;
}
uint8_t	app_correct_data_read(correct_save_data_t *pbuf,uint32_t b_size)
{
	uint8_t flag;
	uint8_t flags;
	uint16_t offset;
	uint16_t s_num = b_size/R_SIZE;
	uint32_t r_size = 0;
	flag = USBFS_fopen(DL_CORRECT_FILE,READ_PIC);
	if ( flag == FILE_OPEN_FAIL )
	{
		return FAIL;
	}
	for ( offset = 0;offset < s_num;offset++ )
	{
		r_size = USB_fread_char(pbuf,R_SIZE,&flags);
		pbuf += r_size;
	}
	USB_fread_char(pbuf,(b_size % R_SIZE),&flags);
	USBFS_fclose(READ_PIC);
	return TURE;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :�����ļ�д��ӿ�
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void  app_local_file_write( char *buf, int32_t addr, Bsize_t size )
{
   UINT  w;
   f_write(m_hzlib_local_file,buf,size,&w);
   return;
}

static BOOL _Wanging(void)
{
	if(MsgBox(200,130,250,200,"����!","ȷ������,��������ᵼ���ļ���.",0)==
		GUIE_KEY_ENTER)
		return TURE;
	return FAIL;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :�ֿ�������ʼ����
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
static BOOL app_file_updata( char*file,
							 uint32_t addr,
							 void( *wirte_data )( char *buf, int32_t addr, Bsize_t size ),
							 uint32_t max_size )
{
	uint8_t* p_file_buf;
	uint8_t  flags;
	uint32_t fread_size = 0;
	uint32_t fsize = 0;
	uint32_t flash_addr_start = addr;
	hwin_t*  hp = PNULL;
	Bsize_t  vlue  = 0;
	Bsize_t  Mvlue = 100;
	p_file_buf = ( uint8_t* )app_malloc( MAX_HZLIB_BUF_SIZE );/*�����ļ��ռ�*/
	if ( !p_file_buf )
	{
		return FAIL;
	}
	hp  = Gui_txtbox_creat( 5, 190, 300, 25, 0, m_hzlib_win );   /*����������ʾ�ı���*/
	GUI_set_Fgcolor( C_GUI_YELLOW );
	GUI_set_Bgcolor( C_GUI_HIGH_BLUE );
	flags = USBFS_fopen( file, READ_PIC);
	if ( flags == FILE_OPEN_FAIL )
	{
		GUI_txtbox_print( hp, ( char* )"����ʧ��,�����ļ��Ƿ����");
		msleep( APP_100_MS );
		GUI_window_destory( hp );  /*�����ı���*/
		app_free( p_file_buf );
		return FAIL;
	}
	fsize = USBFS_file_size_obtain();
	if(max_size!=0)
	{
		if ( fsize >max_size)
		{
			GUI_txtbox_print( hp, ( char* )"�ļ����󣬳����洢��" );
			msleep( APP_100_MS );
			USBFS_fclose(READ_PIC);
			GUI_window_destory( hp );  /*�����ı���*/
			app_free( p_file_buf );
			return FAIL;
		}
	}
	Gui_progress_mod_setp( m_hzlib_progress_bar,Mvlue);
	while ( 1 )
	{
		fread_size = USBFS_fread( p_file_buf, MAX_HZLIB_BUF_SIZE, &flags );/*����USB�����ļ�*/
		if ( flags == FILE_READ_DOING )
		{
			wirte_data(( char* )p_file_buf, flash_addr_start, fread_size ); 
			flash_addr_start += fread_size;
			m_write_size_already+=fread_size;
			vlue = Mvlue*m_write_size_already/fsize;
			Gui_progress_bar_setValue( m_hzlib_progress_bar, vlue);    
			continue;
		}
		else if ( flags == FILE_READ_FINISH )  /*�ļ���ȡ���*/
		{
			Gui_progress_bar_setValue( m_hzlib_progress_bar,Mvlue);
			wirte_data(( char* )p_file_buf, flash_addr_start, fread_size );
			m_write_size_already+=fread_size;
			break;
		}
		else if ( flags != FILE_READ_FAIL ) /*�ļ���ȡ����*/
		{
			GUI_txtbox_print(hp,(char*)m_hzlib_updata_err[g_language]);
			GUI_window_destory(hp);  	    /*�����ı���*/
			app_free( p_file_buf );
			USBFS_fclose(READ_PIC);
			return FAIL;
		}
	}
	USBFS_fclose(READ_PIC);
	GUI_window_destory( hp );   	   /*�����ı���*/
	app_free( p_file_buf ); 		   /*�ͷ��ڴ�*/
	GUI_set_Bgcolor( USBmod_WIN_BG_COLOR );
	GUI_set_Fgcolor( C_GUI_YELLOW );
	return TURE;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :���������Ϣͷ
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void app_MB_info_creat( FIL*Pfile )
{
	Bsize_t size;
	Bsize_t  len;
	uint32_t *paddr;
	Bsize_t offset;
	FRESULT ret;
	MB_member_t MB_info;
	UINT w;
    ret=f_open(Pfile,MBLIB_INFO_NAME,FA_CREATE_ALWAYS|FA_WRITE);
	if(ret!=FR_OK) return;   
	
	paddr = app_malloc( 400 * sizeof( uint32_t ) );
	size = fatfs_file_locate_icon("\r\n", MBLIB_FILE_NAME,paddr,400,(void*)0);
	for (offset=0;offset < size - 1;offset++ )
	{
		GLOBAL_MEMCLR( &MB_info,sizeof( MB_member_t ) );
		global_itoa(( paddr[offset] + 2 ),MB_info.addr );
		global_itoa( offset, MB_info.offset );
		len = paddr[offset+1] - paddr[offset] - 2;
		global_itoa( len, MB_info.size );
        f_write(Pfile,&MB_info,sizeof(MB_member_t),&w);
	}
	app_free(paddr);
	f_close(Pfile);
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :�������
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
BOOL  app_hzlib_updata_MB( void )
{
	FIL  lfile;
	FRESULT ret;
	m_hzlib_local_file = &lfile;
	GUI_window_hwin_req( m_hzlib_win );
	GUI_set_Bgcolor( USBmod_WIN_BG_COLOR );
	GUI_set_Fgcolor( C_GUI_YELLOW );
	GUI_string_display_At(XCORRD_INFO,YCORRD_INFO, "�����������..." );
	GUI_window_hwin_release( m_hzlib_win );
    ret=f_open(m_hzlib_local_file,
		MBLIB_FILE_NAME,FA_CREATE_ALWAYS|FA_WRITE);
	if(ret!=FR_OK) return FAIL;
	
	if ( app_file_updata(
			 MBLIB_FILE_NAME,
			 0,
			 app_local_file_write,
			 0x100000) )
	{
		f_close(m_hzlib_local_file);
		GUI_window_hwin_req( m_hzlib_win );
		GUI_set_Bgcolor( USBmod_WIN_BG_COLOR );
		GUI_set_Fgcolor( C_GUI_YELLOW );
		GUI_string_display_At(XCORRD_INFO, YCORRD_INFO, "���ڲ���Ŀ¼...." );
		app_MB_info_creat(&lfile);
		GUI_window_hwin_release( m_hzlib_win );
	}
	else 
	{
		return FAIL;
	}
	return TURE;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� : �����ƻ�
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void app_plan_info_display(Bsize_t num)
{
  char temp[5]="";
  GUI_set_Fgcolor( C_GUI_WHITE);
  global_itoa(num,temp);
  global_strcat(temp,"%");
  GUI_string_display_At(250,130,temp);
  return;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :����ƻ�
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void app_updata_plan( void )
{

	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :���������ļ�
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void app_updata_help( void )
{
	int size;
	FIL  lfile;
	FRESULT ret;
	size = size ;
	m_hzlib_local_file=&lfile;
	GUI_window_hwin_req( m_hzlib_win );
	GUI_set_Bgcolor( USBmod_WIN_BG_COLOR );
	GUI_set_Fgcolor( C_GUI_YELLOW );
	GUI_string_display_At(XCORRD_INFO, YCORRD_INFO, "���ڶ�ȡ�ļ�..." );
	GUI_window_hwin_release( m_hzlib_win );
    ret=f_open(m_hzlib_local_file,"HELP.TXT",FA_CREATE_ALWAYS | FA_WRITE);
	if(ret!=FR_OK) return;
	if ( app_file_updata("HELP.TXT",0,
			 app_local_file_write,
			 0x100000) )
	{
		f_close(m_hzlib_local_file);
		GUI_window_hwin_req( m_hzlib_win );
		GUI_string_display_At(XCORRD_INFO, YCORRD_INFO, "���ڸ��ļ���ҳ..." );
		app_help_file_paging();
		GUI_window_hwin_release( m_hzlib_win );
	}
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :�������ڴ���
 * @���� :NONE
 * @��� :NONE
********************************************************************/
static void app_hzlib_updata_window_create(void)
{
	GUI_set_Bgcolor( USBmod_BG_COLOR );
	GUI_clean_screen();
	GUI_set_Bgcolor( USBmod_WIN_BG_COLOR );
	GUI_set_Fgcolor( USBmod_FG_COLOR );
	m_hzlib_win =  GUI_window_child_creat( 120, 100, 400, 280, WIN_3D| WIN_FILL,GET_GUIWIN );
	m_hzlib_progress_bar = Gui_progress_bar_creat( 5, 210, 395, 30, ( 9 * 1024 ) / 1024 + 1, m_hzlib_win );
	GUI_set_Bgcolor( C_GUI_BLUE );
	GUI_set_Fgcolor( C_GUI_YELLOW );
	m_hzlib_msg_win = Gui_3D_window_creat( 90, 30, 210, 120, "������ʾ", WIN_3D | WIN_FILL, m_hzlib_win );
	GUI_window_hwin_req( m_hzlib_msg_win );
	GUI_set_Fgcolor( C_GUI_YELLOW );
	GUI_set_Bgcolor( C_GUI_BLUE );
	GUI_string_display_At( 2, 20, "�����ڶ�ȡ�ļ���Ϊ�˱��������𻵣������������벻Ҫ�ϵ�!" );
	GUI_window_hwin_release( m_hzlib_msg_win );
	return;
}

/********************************************************************
 * @������ :�ҳ�
 * @���� :����ͼ��
 * @���� :file �ļ�����PATH SD����Ҫ�洢���ļ�������
 * @��� :NONE
********************************************************************/
BOOL USBfile_copySD(const char*File,const char*PATH)
{
	FIL  lfile;
	FRESULT ret;
	DIR    dir;
    char name[20]="";
	m_hzlib_local_file = &lfile;
	GUI_window_hwin_req( m_hzlib_win );
	Gui_progress_bar_empty(m_hzlib_progress_bar);
	GUI_set_Bgcolor( USBmod_WIN_BG_COLOR );
	GUI_set_Fgcolor( C_GUI_YELLOW );
	GUI_string_display_At(XCORRD_INFO,YCORRD_INFO, "���������ļ�..." );
	GUI_window_hwin_release( m_hzlib_win );
	if(File[0]==0)return FAIL;
	if(PATH[0]!=0)
	{
	   	ret = f_opendir(&dir,PATH);
	    if(ret==FR_NO_PATH)
	    {
	        if(f_mkdir(PATH)!=FR_OK)
			   return FAIL;
		}
		global_strcpy(name,(char*)PATH);
		global_strcat(name,"/");
		global_strcat(name,(char*)File);
	}
	else
	{
		global_strcpy(name,(char*)File);
	}
    ret=f_open(m_hzlib_local_file,
		name,FA_CREATE_ALWAYS|FA_WRITE);
	if(ret!=FR_OK) return FAIL;
	GUI_window_hwin_req( m_hzlib_win );
	GUI_SetFront(&GUI_F8X16_ASCII);
	GUI_set_Bgcolor( USBmod_WIN_BG_COLOR );
	GUI_set_Fgcolor( C_GUI_YELLOW );
	GUI_SetFront(&GUI_F8X16_ASCII);
	GUI_string_display_At(XCORRD_INFO, YCORRD_INFO, "updata " );
	GUI_string_display(name);
	GUI_string_display("           ");
	GUI_window_hwin_release( m_hzlib_win );
	if ( app_file_updata(
			 (char*)File,
			 0,
			 app_local_file_write,
			 0) )
	{
		f_close(m_hzlib_local_file);
	}
	else 
	{
		return FAIL;
	}
	return TURE;
}


void app_Extfile_updata(const char* list,const char* PATH)
{
	uint32_t file_size;
	uint8_t flag;
	char *pdata,*phead,*pend,*pfinsh;
	char  name[20];
	if(USBFS_fopen((char*)list,READ_PIC)!=0)
	{
		file_size = USBFS_file_size_obtain();
		pdata = app_malloc(file_size+10);
	    phead = pdata;
		if(pdata)
		{
			while(1)
			{
				file_size=USBFS_fread(phead,512,&flag);
				if(flag==FILE_READ_FINISH)
				{
					USBFS_fclose(READ_PIC);
					break;
				}
				phead+=512;
			}
		}
	}
	phead = pdata;
	pfinsh = global_string_locate_first(pdata,":end");
	while(1)
	{
		pend  = strchr(phead,'\n');
		GLOBAL_MEMCLR(name, 20);
		global_memcpy(name,phead,(Bsize_t)(pend-phead));
		global_string_filter(name,'\r');
		global_string_filter(name,'\n');
		global_string_filter(name,' ');
		USBfile_copySD(name,PATH);
		m_write_size_already = 0;
		phead = pend+1;
		if(((uint32_t)pend+5)>(uint32_t)pfinsh)
		break;
	}
	app_free(pdata);
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :�ֿ�����
 * @���� :NONE
 * @���  :NONE
********************************************************************/
void app_hzlib_updata_entery( uint8_t key )
{
    uint8_t key_buf;
	if ( key == KEY_0 )
	{
		if(_Wanging()){
			GUI_window_hwin_req( GET_GUIWIN );
			GUI_SetFront(&GUI_HZK16_EXTASCII);
			app_hzlib_updata_window_create();
			app_hzlib_updata_MB();
		}
	}
	else if(key == KEY_F5)
	{
		GUI_window_hwin_req( GET_GUIWIN );
		GUI_SetFront(&GUI_HZK16_EXTASCII);
		app_hzlib_updata_window_create();
		m_write_size_already = 0;
	    USBfile_copySD("HZK241","");

	}
	else if ( key == KEY_1 )
	{
	    if(_Wanging()){
			GUI_window_hwin_req( GET_GUIWIN );
			GUI_SetFront(&GUI_HZK16_EXTASCII);
			app_hzlib_updata_window_create();
			m_write_size_already = 0;
		    USBfile_copySD("hzk16","");
			m_write_size_already = 0;
		    USBfile_copySD("Asc16","");
			m_write_size_already = 0;
		    USBfile_copySD("HZK24","");
			m_write_size_already = 0;
		    USBfile_copySD("HZK24T","");
	    }
	}
	else if ( key == KEY_2 )
	{
		GUI_window_hwin_req( GET_GUIWIN );
		GUI_SetFront(&GUI_HZK16_EXTASCII);
		app_hzlib_updata_window_create();
		app_updata_plan();
	}
	else if ( key == KEY_3 )
	{
		GUI_window_hwin_req( GET_GUIWIN );
		GUI_SetFront(&GUI_HZK16_EXTASCII);
		app_hzlib_updata_window_create();
		app_updata_help();	
	}
	else if ( key == KEY_5 )
	{
	   GUI_set_Bgcolor( USBmod_BG_COLOR );
	   GUI_clean_screen();
	   if ( MsgPasswordBox( 250, 250, "1234")) 
	   {
			GUI_window_hwin_req( GET_GUIWIN );
		    GUI_SetFront(&GUI_HZK16_EXTASCII);
	   		GUI_set_Fgcolor( C_GUI_YELLOW);
			GUI_SetFront(&GUI_HZK24_EXTASCII);  
			GUI_string_display_At(100, 400, "����ϵͳ��F5������������..." );
			GUI_window_hwin_release( GET_GUIWIN );	
			while(1);
	   }
	}
	else if(key == KEY_8)
	{
		GUI_window_hwin_req( GET_GUIWIN );
		GUI_SetFront(&GUI_HZK16_EXTASCII);
		app_hzlib_updata_window_create();
		app_Extfile_updata("bmplist","BMP");
	}
	else if(key == KEY_9)
	{
		GUI_window_hwin_req( GET_GUIWIN );
		GUI_SetFront(&GUI_HZK16_EXTASCII);
		app_hzlib_updata_window_create();
		app_Extapp_updata();
	}
	GUI_window_hwin_req( m_hzlib_win );
	GUI_set_Bgcolor( USBmod_WIN_BG_COLOR );
	GUI_set_Fgcolor( C_GUI_YELLOW );
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_string_display_At(XCORRD_INFO, YCORRD_INFO, "������������������˳�           " );
	GUI_window_hwin_release( m_hzlib_win );
	while ( app_global_key_obtain( &key_buf, 1 ) != 1 )
	{
		msleep( TYPE_DELAY );
	}
	GUI_window_destory( m_hzlib_win );
	GUI_window_hwin_release( GET_GUIWIN );
	return;
}

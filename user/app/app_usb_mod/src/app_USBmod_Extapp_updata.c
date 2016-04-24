/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:�����ֿ�
* ����ʱ��:2010-11-28
* ������˾ :����
* �ļ�����:app_USBmod_Extapp_updata.c
* ������ :�ҳ� and �ܱ�Ȩ
* ��������:�����ⲿӦ�ó���
******************************************************************************************/
#include"app_USBmod_inside.h"
#include <stdlib.h>
typedef struct
{
   char  Exe[20];         
   char  name[20];
   char  PressBMP[20];
   char  UNpressBMP[20];
}Extapp_name_t;

/***************************************************************************/
uint32_t 	USBFS_file_size_obtain(void);
uint32_t 	USBFS_fread(void*pbuf, uint32_t buf_size, uint8_t *flags );
uint32_t 	USBFS_fwrite(void*pbuf, uint32_t buf_size, uint8_t *flags );
uint8_t 	USBFS_fopen(char*file, char mode);
uint8_t 	USBFS_fclose(uint8_t RW);
static void USBExp_progress_view(char*str);
static void USBExp_probar_view(void);
void 		Exp_file_close(void);
void 		Exp_file_open(const char*file);
BOOL 		USBfile_copySD(const char*File,const char*PATH);
FIL*		m_fExp;
/***************************************************************************/
#define  DATA_RECORD				00		//���ݼ�¼
#define  EX_LINE_ADDRESS_RECORD		04		//��չ���Ե�ַ��¼
#define  END_OF_FILE_RECORD			01		//�ļ�������¼
#define  EX_SEG_ADDRESS_RECORD		02		//��չ�ε�ַ��¼
/*****************ʹ�ñ�������*******************/
#define  MAX_DATALINE_SIZE	    76			//һ���������������ݸ���
#define  PER_USB_SIZE    	    800			//һ�ζ�ȡUSB�ļ���С
int8_t 	 hex_data_buf[50] 		= "";		//HEX�ļ��������ݻ���
int8_t 	 g_hex_data_buf[50] 	= "";		//��ǰ��ַ�������ݻ���
uint8_t  g_USB_CUR_offset 		= 0;		//��ǰUSB���洦��ƫ����
uint8_t  g_current_updata_size 	= 0;		//��ǰ��ַ�������ݴ�С
uint32_t g_USBfile_size 		= 0;		//USB�ļ���С
int8_t   USB_file_buf[PER_USB_SIZE+MAX_DATALINE_SIZE] = "";	//һ�ζ�ȡUSB����
uint8_t  hex_error_flag 		= 0;		//ȫ��HEX���ʹ����־
uint16_t g_flash_buf[50] 		={0,};
uint8_t  g_flash_offset 		= 0;
uint8_t  g_Check_sum 			= 0;
/***********************************************/
uint32_t Current_EX_sec_address = 0;		//FLASH��ǰ��չ�ε�ַ
uint32_t Current_EX_line_address= 0;		//FLASH��ǰ��չ���Ե�ַ
uint32_t Current_load_address   = 0;		//FLASH��ǰ���ص�ַ
/********************************************************************
 * @������ : �ܱ�Ȩ
 * @����   : ��һ�ַ�ת����16���Ƶ�����
 * @����   : InputChar:��Ҫת�����ַ�
 * @���   : 16��������
********************************************************************/
char ChangeCharToHex(char InputChar)
{
	if (InputChar < '0')
	 	return(-1);
	
	if ( (InputChar >= '0') && (InputChar <= '9') )
	 	return(InputChar-'0');

	if ( (InputChar >= 'A') && (InputChar <= 'F') )
		return(InputChar-0x37);

	if ( (InputChar >= 'a') && (InputChar <= 'f') )
		return(InputChar-0x57);
	else return(-1);
}
/********************************************************************
 * @������ : �ܱ�Ȩ
 * @����   : hexͷ����
 * @����   : p_src     :Դ����ָ��ͷ��Checksum:У��ͻ���;reda_addr:���ݼ�¼��ַ����;
 *		     Byte_count:�����ֽڴ�С����;Record_type:��¼���ͻ���;
 * @���   : m_file_offset:hex�ļ�ƫ�Ƹ���;
********************************************************************/
uint8_t hex_head_handle( int8_t *p_src, uint8_t *Byte_count, uint16_t *Data_Add,
					 	uint8_t *Record_type, uint8_t *Checksum )
{
	uint8_t m_file_offset = 0;
	
	*Byte_count  = (ChangeCharToHex(p_src[m_file_offset]) << 4);
	m_file_offset += 1;
	*Byte_count |= (ChangeCharToHex(p_src[m_file_offset]) & 0x0f);
	m_file_offset += 1;
	*Data_Add 	= (uint16_t)(ChangeCharToHex(p_src[m_file_offset]) << 12);
	m_file_offset += 1;
	*Data_Add   |= (uint16_t)(ChangeCharToHex(p_src[m_file_offset]) << 8);
	m_file_offset += 1;
	*Data_Add   |= (uint16_t)(ChangeCharToHex(p_src[m_file_offset]) << 4);
	m_file_offset += 1;
	*Data_Add   |= (uint16_t)(ChangeCharToHex(p_src[m_file_offset]) & 0x0f);
	m_file_offset += 1;
	*Record_type = (ChangeCharToHex(p_src[m_file_offset]) << 4);
	m_file_offset += 1;
	*Record_type|= (ChangeCharToHex(p_src[m_file_offset]) & 0x0f);
	m_file_offset += 1;

	*Checksum = (uint8_t)(*Byte_count)+(uint8_t)(*Data_Add >> 8)+(uint8_t)*Data_Add+(uint8_t)*Record_type;
	
	return m_file_offset;
}

/********************************************************************
 * @������ : �ܱ�Ȩ
 * @���� : ���ݼ�¼����(00)
 * @���� : p_src:Դ����ָ��ͷ��p_dest:������ݻ���;bt_cnt:�����ֽڴ�С;
 *			file_offset:hex�ļ�ƫ��;Checksum:��ǰУ��ͣ�
 * @��� : �ɹ�����:file_offset:hex�ļ�ƫ�Ƹ���; ʧ��:0;
********************************************************************/
uint16_t hex_data_record_handle( int8_t *p_src, uint8_t bt_cnt, 
							int8_t * p_dest, uint8_t  Checksum )
{
	uint8_t  offset;
	int8_t * Data_buf = hex_data_buf;
	uint8_t  data_temp = 0;
	uint8_t  Checksum_temp = 0;
	uint16_t m_file_offset = 0;
	for ( offset = 0; offset < bt_cnt; offset++)
	{
		data_temp  = (ChangeCharToHex(p_src[m_file_offset]) << 4);
		m_file_offset += 1;
		data_temp |= (ChangeCharToHex(p_src[m_file_offset]) & 0x0f);
		m_file_offset += 1;
		*(Data_buf + offset) = data_temp;
		Checksum += (uint8_t)*(Data_buf + offset);
	}
	Checksum = ~Checksum;			//��Checksum����
	Checksum += 1;
	Checksum_temp  = (ChangeCharToHex(p_src[m_file_offset]) << 4);
	m_file_offset += 1;
	Checksum_temp |= (ChangeCharToHex(p_src[m_file_offset]) & 0x0f);
	m_file_offset += 1;
	if ( (Checksum & 0xff) == Checksum_temp )
	{
		g_Check_sum = Checksum_temp;
		for ( offset = 0; offset < bt_cnt; offset++)
		{
			*(p_dest + offset) = *(Data_buf + offset);
			g_current_updata_size++;
		}
		return m_file_offset;
	}
	else 
	{
		return 0;
	}

}

/********************************************************************
 * @������ : �ܱ�Ȩ
 * @���� : hex�ļ�����(:00000001FF)(01)
 * @���� : p_src:Դ����ָ��ͷ��
 *
 * @��� : �ɹ�����:file_offset:hex�ļ�ƫ�Ƹ���; ʧ��:0;
********************************************************************/
uint8_t hex_file_end( int8_t *p_src, uint8_t  Checksum )
{
	uint8_t Checksum_temp;
	Checksum_temp  = (ChangeCharToHex(p_src[0]) << 4);
	Checksum_temp |= (ChangeCharToHex(p_src[1]) & 0x0f);
	if ( Checksum_temp == 0xff )
	{
		g_Check_sum = Checksum_temp;
		return 2;
	}
	return 0;
}

/********************************************************************
 * @������ : �ܱ�Ȩ
 * @���� : ��չ�ε�ַ��¼����(02)(һ��û��)
 * @���� : p_src:Դ����ָ��ͷ��Checksum:У���;reda_addr:���ݼ�¼��ַ;
 *
 * @��� : �ɹ�����:��ǰhex�ļ�ƫ�Ƹ���; ʧ��:0;
********************************************************************/
uint8_t hex_EX_sec_handle( int8_t *p_src, uint8_t  Checksum, uint16_t reda_addr)
{
	uint8_t m_file_offset = 0;
	uint16_t m_ex_sec_address = 0;
	uint8_t Checksum_temp = 0;

	m_ex_sec_address  = (ChangeCharToHex(p_src[m_file_offset]) << 12);
	m_file_offset += 1;
	m_ex_sec_address |= (ChangeCharToHex(p_src[m_file_offset]) << 8);
	m_file_offset += 1;
	Checksum += (uint8_t)(m_ex_sec_address >> 8);
	m_ex_sec_address |= (ChangeCharToHex(p_src[m_file_offset]) << 4);
	m_file_offset += 1;
	m_ex_sec_address |= (ChangeCharToHex(p_src[m_file_offset]) & 0x0f);
	m_file_offset += 1;
	Checksum += (uint8_t)(m_ex_sec_address & 0xff);

	Checksum = ~Checksum;			//��Checksum����
	Checksum += 1;
	Checksum_temp  = (ChangeCharToHex(p_src[m_file_offset]) << 4);
	m_file_offset += 1;
	Checksum_temp |= (ChangeCharToHex(p_src[m_file_offset]) & 0x0f);
	m_file_offset += 1;

	if ( (Checksum & 0xff) == Checksum_temp )
	{
		g_Check_sum = Checksum_temp;
		Current_EX_sec_address = (uint32_t)((m_ex_sec_address << 4) + reda_addr);
		return m_file_offset;
	}

	return 0;
}

/********************************************************************
 * @������ : �ܱ�Ȩ
 * @���� : ��չ���Ե�ַ��¼����(04)
 * @���� : p_src:Դ����ָ��ͷ��Checksum:У���;reda_addr:���ݼ�¼��ַ;
 *
 * @��� : �ɹ�����:��ǰhex�ļ�ƫ�Ƹ���; ʧ��:0;
********************************************************************/
uint8_t hex_EX_line_handle( int8_t *p_src, uint8_t  Checksum, uint16_t reda_addr)
{
	uint8_t m_file_offset = 0;
	uint32_t m_ex_line_address = 0;
	uint8_t Checksum_temp = 0;

	m_ex_line_address  = (ChangeCharToHex(p_src[m_file_offset]) << 12);
	m_file_offset += 1;
	m_ex_line_address |= (ChangeCharToHex(p_src[m_file_offset]) << 8);
	m_file_offset += 1;
	Checksum += (uint8_t)(m_ex_line_address >> 8);
	m_ex_line_address |= (ChangeCharToHex(p_src[m_file_offset]) << 4);
	m_file_offset += 1;
	m_ex_line_address |= (ChangeCharToHex(p_src[m_file_offset]) & 0x0f);
	m_file_offset += 1;
	Checksum += (uint8_t)(m_ex_line_address & 0xff);

	Checksum = ~Checksum;			//��Checksum����
	Checksum += 1;
	Checksum_temp  = (ChangeCharToHex(p_src[m_file_offset]) << 4);
	m_file_offset += 1;
	Checksum_temp |= (ChangeCharToHex(p_src[m_file_offset]) & 0x0f);
	m_file_offset += 1;

	if ( (Checksum & 0xff) == Checksum_temp )
	{
		g_Check_sum = Checksum_temp;
		Current_EX_line_address = (uint32_t)((m_ex_line_address << 16) + reda_addr);
		return m_file_offset;
	}

	return 0;
}

/********************************************************************
 * @������ : �ܱ�Ȩ
 * @���� : hex�ļ���ʽ����(��ʱд�ɴ�����hex����)
 * @���� : p_src:Դ����ָ��ͷ��p_dest:������ݻ���
 *
 * @��� : �ɹ�����:����hex�ļ��ַ������� ʧ�ܷ��� 0;
********************************************************************/
uint8_t hex_format_explain( int8_t *p_src, int8_t * p_dest )
{
	uint8_t  Byte_count = 0;
	uint16_t Data_Add = 0;			//���ݼ�¼��ַ
	uint8_t  Record_type = 0;
	uint8_t  Checksum = 0;
	uint8_t  file_offset = 0;		//hex�ļ�ƫ��
	uint8_t  m_file_offset = 0;		//hex��ǰ����ƫ��

	if ( p_src[file_offset] == ':' )				//�ļ�ͷ
	{			
		file_offset += 1;
		file_offset += hex_head_handle( &p_src[file_offset],&Byte_count,&Data_Add,&Record_type,&Checksum);
		if ( Record_type == EX_LINE_ADDRESS_RECORD )		//��չ���Ե�ַ��¼
		{
			m_file_offset = hex_EX_line_handle( &p_src[file_offset], Checksum, Data_Add );
			if ( m_file_offset )
			{
				file_offset += m_file_offset;
				Current_load_address = Current_EX_line_address;
				return file_offset;
			}
			else
			{
				hex_error_flag = 1;
				return 0;
			}
		}
		else if ( Record_type == DATA_RECORD )		//���ݼ�¼
		{
			m_file_offset = hex_data_record_handle( &p_src[file_offset], Byte_count, p_dest, Checksum);
			if ( m_file_offset )
			{
				file_offset += m_file_offset;
				Current_load_address = (uint32_t)((Current_EX_line_address & 0xffff0000) | Data_Add);
				return file_offset;
			}
			else
			{
				hex_error_flag = 1;
				return 0;
			}
		}
		else if ( Record_type == END_OF_FILE_RECORD )		//�ļ�����
		{
			file_offset += hex_file_end( &p_src[file_offset], Checksum );
			g_USBfile_size = 0;
			return file_offset;
		}
	}
	
	return 0;
}

/********************************************************************
 * @������ : �ܱ�Ȩ
 * @���� : HEX�ļ������������Դ���
 * @���� : pdsrc:Դ����ָ��ͷ��i_data_size :���ݳ��ȣ�
 *
 * @��� : back_size : ��������ӵ����ݸ�����0xff:���󷵻أ�
********************************************************************/
uint8_t DataLine_integrity_handle( int8_t *pdsrc, uint16_t i_data_size )
{
	uint8_t 	back_size = 0;		//��Ҫ׷�������ݴ�С
	uint8_t 	back_offset = 1;	//Դ����ָ�븺ƫ��
	uint16_t 	temp_offset = 0;	//Դ����ָ����ƫ��
	uint8_t 	by_count = 0;		//����Ŀ¼�ֽڸ���
	uint8_t     DataLine_size = 0;	//�������ֽڸ���
	uint8_t 	state_flag = 0;		//USB����״̬��־λ
	if (pdsrc[i_data_size - back_offset] == '\r' || 
		pdsrc[i_data_size - back_offset] == '\n'  )
	{
		return 0;
	}
	else 
	{
		while (1)
		{
			if (pdsrc[i_data_size - back_offset] == ':')
				break;
			back_offset += 1;
		}
		if ( back_offset >= 3 )
		{
			temp_offset = i_data_size - back_offset;
			by_count  = (ChangeCharToHex(pdsrc[temp_offset+1]) << 4);
			by_count |= (ChangeCharToHex(pdsrc[temp_offset+2]) & 0x0f);
			DataLine_size = 8+by_count*2+2+1;
			if ( back_offset == DataLine_size )
				return 0;
			else if ( DataLine_size > back_offset )
			{	
				back_size = (DataLine_size - back_offset);
				if (USBFS_fread( (int8_t *)(pdsrc+i_data_size),back_size,&state_flag) == back_size )
				{
					return back_size;
				}
			}
			else 
			{
				return 0xff;	//���󷵻�
			}
		}
		else if ( back_offset == 1 )
		{
			if (USBFS_fread( (int8_t *)(pdsrc+i_data_size),2,&state_flag) == 2 )
			{
				by_count  = (ChangeCharToHex(*(pdsrc+i_data_size)) << 4);
				by_count |= (ChangeCharToHex(*(pdsrc+i_data_size+1)) & 0x0f);
				DataLine_size = 6+by_count*2+2+1;
				if (USBFS_fread( (int8_t *)(pdsrc+i_data_size+2),DataLine_size,&state_flag) == DataLine_size)
					return (DataLine_size+2);
			}
			return 0xff;	//���󷵻�
		}
		else if ( back_offset == 2 )
		{
			if (USBFS_fread( (int8_t *)(pdsrc+i_data_size),1,&state_flag) == 1 )
			{
				by_count  = (ChangeCharToHex(*(pdsrc+i_data_size-1)) << 4);
				by_count |= (ChangeCharToHex(*(pdsrc+i_data_size)) & 0x0f);
				DataLine_size = 6+by_count*2+2+1;
				if (USBFS_fread( (int8_t *)(pdsrc+i_data_size+2),DataLine_size,&state_flag) == DataLine_size)
					return (DataLine_size+1);

			}
			return 0xff;	//���󷵻�
		}
	}

	return 0;
}

/********************************************************************
 * @������ : �ܱ�Ȩ
 * @���� : �ڲ�FLASH�������׵�ַ0x300000
 * @���� : NONE
 *
 * @��� : �ɹ�����1�� ʧ�ܷ��� 0
********************************************************************/
uint8_t Update_Inter_Flash( const char* program )
{
	Uint16 Status;
	UINT  w;
	uint8_t offset;
	int8_t * current_data_buf = g_hex_data_buf;
	uint8_t USB_state_flag = 0;
	int8_t * cur_USB_buf = 0;
	uint16_t cur_buf_offset = 0;
	uint16_t cur_buf_offset_shadow = 0;
	uint16_t per_USB_size_offset = 0;
	uint8_t handle_size = 0;
	uint8_t flash_offset = 0;
	float   progress_sizef = 0.0;
	uint16_t progress_size = 0;
	uint16_t progress_offset = 0;
	Exp_file_open(program);
	while ( 1 )
	{
		if ( g_USBfile_size != 0 )
		{		
			if ( g_USBfile_size > PER_USB_SIZE ) 
			{
				per_USB_size_offset = PER_USB_SIZE;
			}
			else
			{
				per_USB_size_offset = g_USBfile_size;
			}
			if (USBFS_fread(USB_file_buf, per_USB_size_offset,&USB_state_flag ) == per_USB_size_offset )
			{
				cur_USB_buf = USB_file_buf;
				g_USBfile_size -= per_USB_size_offset;
				if ( (handle_size = DataLine_integrity_handle(USB_file_buf,per_USB_size_offset)) != 0xff )
				{
					g_USBfile_size -= handle_size;
					per_USB_size_offset += handle_size;
				}
				else 
				{
					USBExp_progress_view("��������ʧ��!");
					BSP_Dly_ms(1000);
					return 0;
				}
			}
			else 
			{
				USBExp_progress_view("��ȡU���ļ�����!");
				BSP_Dly_ms(1000);
				return 0;
			}
			
			while (1)
			{
				if ( (cur_buf_offset += hex_format_explain( cur_USB_buf, current_data_buf )) != cur_buf_offset_shadow )
				{
					cur_USB_buf = (USB_file_buf + cur_buf_offset);
					if ( g_current_updata_size != 0 )
					{
						g_flash_offset = g_current_updata_size / 2;
						flash_offset = 0;
						for ( offset = 0; offset < g_flash_offset;offset++)
						{
							g_flash_buf[offset]  = (uint16_t)(g_hex_data_buf[flash_offset] << 8);
							flash_offset++;
							g_flash_buf[offset] |= (uint16_t)(g_hex_data_buf[flash_offset] & 0xff);
							flash_offset++;
						}
						USBExp_probar_view();
                        f_lseek(m_fExp,(Current_load_address-0x140000)*2);
						fatfs_write_bytes(m_fExp,g_flash_buf, g_flash_offset,&w);
						g_current_updata_size = 0;
					}
				}
				else if ( hex_error_flag != 1 )
				{
					cur_buf_offset += 1;
					cur_USB_buf = (USB_file_buf + cur_buf_offset);
				}
				else 
				{
				    USBExp_progress_view("�ļ����ʹ���!");
					BSP_Dly_ms(1000);
					return 0;
				}
				if ( cur_buf_offset >= per_USB_size_offset )
				{
					cur_buf_offset = 0;
					cur_buf_offset_shadow = 0;
					break;
				}
				cur_buf_offset_shadow = cur_buf_offset;
			}
			
		}
		else if ( g_USBfile_size == 0 )	//��ȡ�ļ�����
		{
			Exp_file_close();
			USBExp_probar_view();
			EINT;
			return 1;
		}
		progress_offset++;
	}
}
/*************************************************************************/
//ZHOU end
/*************************************************************************/
uint32_t 	m_EXP_size;
#define  	MAX_PROBAR   100
extern      uint32_t     		m_write_size_already;
extern 		hwin_t*				m_hzlib_win;
extern 		hwin_t*				m_hzlib_progress_bar;	

static void USBExp_progress_view(char*str)
{
	GUI_window_hwin_req( m_hzlib_win );
	GUI_set_Bgcolor( USBmod_WIN_BG_COLOR );
	GUI_set_Fgcolor( C_GUI_YELLOW );
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_string_display_At(5, 190,str);
	GUI_string_display("             ");
	GUI_window_hwin_release( m_hzlib_win );

}

static void USBExp_probar_view(void)
{
    uint16_t vlue;
	vlue = MAX_PROBAR*(m_EXP_size-g_USBfile_size)/m_EXP_size;
	Gui_progress_bar_setValue( m_hzlib_progress_bar, vlue);   
    return;
}


void Exp_file_open(const char*file)
{
    uint8_t flag;
	char    name[20]="";
	g_USBfile_size = 0;
	flag = USBFS_fopen((char*)file,READ_PIC);
	if ( flag == FILE_OPEN_FAIL )
	{
		m_EXP_size = g_USBfile_size;
		return;
	}	
	Gui_progress_bar_empty(m_hzlib_progress_bar);
	Gui_progress_mod_setp( m_hzlib_progress_bar,MAX_PROBAR);
	g_USBfile_size = USBFS_file_size_obtain();
	m_EXP_size = g_USBfile_size;
	global_strcpy(name,"EXP/");
	global_strcat(name,(char*)file);
	m_fExp=fatfs_open(name,FA_CREATE_ALWAYS|FA_WRITE);
	USBExp_progress_view(name);
	return;
}

void Exp_file_close(void)
{
    fatfs_close(m_fExp);
	USBFS_fclose(WRITE_PIC);
    return;
}
static app_Extapp_filter(char*str)
{
	global_string_filter(str,'\r');
	global_string_filter(str,'\n');
	global_string_filter(str,' ');
	return;
}

void app_Extapp_uninstall(char*name)
{
	Extapp_name_t Ename; 
	FIL* fp;
	UINT w ;
	uint32_t lseek,lseekT;
	char sname[20]="";
    fp = fatfs_open("EXPLIST",FA_OPEN_ALWAYS|FA_READ|FA_WRITE);
    if(!fp) return;
	while(1)
	{
		f_read(fp,&Ename,sizeof(Extapp_name_t),&w);
		if(w==0)break;
		if(w==sizeof(Extapp_name_t))
		{
		    if(global_strlen(name)!=global_strlen(Ename.Exe))
				continue;
            if(!global_memcmp(Ename.Exe,name,global_strlen(name)))
				continue;
			global_strcpy(sname,"EXP/");
			global_strcat(sname,Ename.Exe);	
            f_unlink(sname);
			if(fp->fsize==sizeof(Extapp_name_t))
		    {
				fatfs_close(fp);
				f_unlink("EXPLIST");
				return;
			}
			lseekT = fp->fptr-sizeof(Extapp_name_t);
			lseek  = fp->fsize-sizeof(Extapp_name_t);
			f_lseek(fp,lseek);
			f_read(fp,&Ename,sizeof(Extapp_name_t),&w);
			f_lseek(fp,lseek);
			f_truncate(fp);
            lseek = lseekT;
		    if(lseek<fp->fsize)
			{
				f_lseek(fp,lseek);
			    f_write(fp,&Ename,sizeof(Extapp_name_t),&w);
			}
			break;
		}
	}
	fatfs_close(fp);
    return;
}



void app_Extapp_uninstall_all(void)
{
    FRESULT RET;
	Extapp_name_t Ename; 
	FIL* fp;
	UINT w ;
	char name[20]="";
	RET = RET;
    fp = fatfs_open("EXPLIST",FA_OPEN_ALWAYS|FA_READ);
    if(!fp) return;
	while(1)
	{
		f_read(fp,&Ename,sizeof(Extapp_name_t),&w);
		if(w==0)break;
		if(w==sizeof(Extapp_name_t))
		{
			GLOBAL_MEMCLR(name, 20);
			global_strcpy(name,"EXP/");
			global_strcat(name,Ename.Exe);	
            RET=f_unlink(name);
		}
	}
	fatfs_close(fp);
    RET=f_unlink("EXPLIST");
    return;
}







BOOL app_Extapp_Is_exist(char*name)
{
    BOOL  state = FAIL;
	Extapp_name_t Ename; 
	FIL* fp;
	UINT w ;
    fp = fatfs_open("EXPLIST",FA_OPEN_ALWAYS|FA_READ);
    if(!fp) return;
	while(1)
	{
		f_read(fp,&Ename,sizeof(Extapp_name_t),&w);
		if(w==0)break;
		if(w==sizeof(Extapp_name_t))
		{
		    if(global_strlen(name)!=global_strlen(Ename.Exe))
				continue;
            if(!global_memcmp(Ename.Exe,name,global_strlen(name)))
				continue;
			state = TURE;
			break;
		}
	}
	fatfs_close(fp);
	return state;
}



void app_Extapp_updata(void)
{
	uint32_t file_size;
	uint8_t  flag;
	char *	 pdata,*phead,*pend,*pfinsh;
	Extapp_name_t  Ename; 
	UINT     w;
	FIL* fp;

	if(USBFS_fopen((char*)"EXPLIST",READ_PIC)!=0)
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
	if(pfinsh)
	{
		while(1)
		{
			pend  = strchr(phead,'\n');
			if(phead[0]=='E')
			{
				GLOBAL_MEMCLR(&Ename,sizeof(Extapp_name_t));
				phead+=strlen("EXE:");
				global_memcpy(Ename.Exe,phead,(Bsize_t)(pend-phead));
	            app_Extapp_filter(Ename.Exe);
				phead = pend+1;
				pend  = strchr(phead,'\n');
				phead+=strlen("NAME:");
				global_memcpy(Ename.name,phead,(Bsize_t)(pend-phead));
	            app_Extapp_filter(Ename.name);
				phead = pend+1;
				pend  = strchr(phead,'\n');
				phead+=strlen("PRESSBMP:");
				global_memcpy(Ename.PressBMP,phead,(Bsize_t)(pend-phead));
	            app_Extapp_filter(Ename.PressBMP);
				phead = pend+1;
				pend  = strchr(phead,'\n');
				phead+=strlen("UNPRESSBMP:");
				global_memcpy(Ename.UNpressBMP,phead,(Bsize_t)(pend-phead));
	            app_Extapp_filter(Ename.UNpressBMP);
				phead = pend+1;
        		if(app_Extapp_Is_exist(Ename.Exe))
        		{
					if(MsgBox(200,200,250,120,(char*)"��ʾ",(char*)"��Ӧ���Ѿ�����,�Ƿ��滻?",APP_HALF_SEC*100)==GUIE_KEY_EXIT)
					{
						app_free(pdata);
						return ;
					}
					app_Extapp_uninstall(Ename.Exe);
				}
				Update_Inter_Flash(Ename.Exe);
				m_write_size_already = 0;
				USBfile_copySD(Ename.PressBMP,"EXP");
				m_write_size_already = 0;
				USBfile_copySD(Ename.UNpressBMP,"EXP");
				m_write_size_already = 0;	
			    fp = fatfs_open("EXPLIST",FA_OPEN_ALWAYS|FA_WRITE);
			    if(!fp) {app_free(pdata);return;}
				f_lseek(fp,fp->fsize);
	    		f_write(fp,&Ename,sizeof(Extapp_name_t),&w);
				fatfs_close(fp);
			}
			if(((uint32_t)pend+5)>(uint32_t)pfinsh)
			break;	
		}
	}
	app_free(pdata);
	return ;
}

#include "../../../includes.h"
#include "../../../Gui\Gui.h"

#ifndef _PEC_APP_VM_INSIDE_2013_H
#define _PEC_APP_VM_INSIDE_2013_H

extern const unsigned char gImage_entery[];
extern const unsigned char gImage_return[];
extern const unsigned char gImage_save[];
extern const unsigned char gImage_exit[];
void  	bsp_delay_ms( unsigned long	ms );

/*********************************************************************
* APP_WAVEMOD-H  for VMMOD fun
**********************************************************************/
void 		app_VMmod_fun_entry( void );
void 		app_VMmod_fun_exit( void );
extern		char m_VMmeter_addr[];
extern		char m_VMmeter07_addr[];
void  	bcd_convert_hex( uint8_t bcd_src, uint8_t *pur_ptr );
void  	hex_convert_bcd( uint8_t src, uint8_t *bcd_src );
void 	hand_wirte_input_box_pos_set(uint16_t x ,uint16_t y);
int 	VMCharToHex(char * str);
extern uint8_t   m_VMoffset_guiyue;
extern uint8_t	 m_VMoffset_baud;
extern uint8_t	 m_VMoffset_crc;
extern uint8_t   m_VMoffset_level;
/*enum //У�鷽ʽ  Odd  �� Event ,none
{
	NONE_CHECK = 0,
	ODD_CHECK ,
	EVEN_CHECK
};
#define   SET_SERIAL_BAUD   	0X01
#define   SET_SERIAL_CHECK 		0X02
#define   SET_EMPTY_FIFO   		0X03
#define   GET_FIFO_STATE   	 	0X04
#define   SET_SERIAL_CHECK2 	0X05*/
extern	uint8_t			g_delay_baud_flag;
/*********************************����********************************************/
#define M_SIZE    9
#define METER_INFO_SIZE  21

typedef struct
{	
	uint8_t	m_crc_flag;					//Ĭ��Ϊ0x5a
	uint8_t guiyue;					//��Լ
	uint8_t baud;					//������
	uint8_t crc;					//У��λ
	char meter_Addr[13];			//���ַ
	char zy_power_ALL[M_SIZE];  	//�����й��� 
	char zy_power_J[M_SIZE];		//��
	char zy_power_F[M_SIZE];		//��
	char zy_power_P[M_SIZE];		//ƽ
	char zy_power_G[M_SIZE];		//��
	char zw_power_ALL[M_SIZE];  	//�����޹���
	char zw_power_J[M_SIZE];		//��
	char zw_power_F[M_SIZE];		//��
	char zw_power_P[M_SIZE];		//ƽ
	char zw_power_G[M_SIZE];		//��	
	char fy_power_ALL[M_SIZE];  	//�����й���
	char fy_power_J[M_SIZE];		//��
	char fy_power_F[M_SIZE];		//��
	char fy_power_P[M_SIZE];		//ƽ
	char fy_power_G[M_SIZE];		//��	
	char fw_power_ALL[M_SIZE];  	//�����޹���
	char fw_power_J[M_SIZE];		//��
	char fw_power_F[M_SIZE];		//��
	char fw_power_P[M_SIZE];		//ƽ
	char fw_power_G[M_SIZE];		//��
	char voltage_A[M_SIZE];		//A��ѹ
	char voltage_B[M_SIZE];		//B��ѹ
	char voltage_C[M_SIZE];		//C��ѹ
	char current_A[M_SIZE];		//A����
	char current_B[M_SIZE];		//A����
	char current_C[M_SIZE];		//A����

	char meter_time_year[5];			//��ʱ��
	char meter_time_month[3];			//��ʱ��
	char meter_time_day[3];				//��ʱ��
	char meter_time_hour[3];			//��ʱ��
	char meter_time_minute[3];			//��ʱ��
	char meter_time_second[3];			//��ʱ��
}VMMeter_dl_info_t;

extern	VMMeter_dl_info_t	VMmeter_data_buf;

/*********************************************************************
* APP_WAVEMOD-H  for VMMOD INTERFACE
**********************************************************************/
#define   	VM_BG_COLOR          	    g_Bg_color	  /*�����ɫ*/
#define   	VM_FG_COLOR               	g_Fg_color       /*����ǰ��ɫ*/
#define   	VM_WIN_BG_COLOR       		g_win_Bg_color/*���ڵ�ɫ*/
#define   	BK_CORLOR    				VM_BG_COLOR

enum
{
	GUI_KEY_GUIYUE_SELECT = 100,
	GUI_KEY_BAUD_SELECT,
	GUI_KEY_CRC_SELECT,
	GUI_KEY_SAVE,
	GUI_KEY_ENTER,
	GUI_KEY_SETTING,
	GUI_KEY_CANCEL,
	GUI_KEY_EXIT
};


enum
{
	EDIT_GUIYUE=1,
	EDIT_BAUD,
	EDIT_CRC,
	EDIT_M_ADDR,  //4
	
	EDIT_Z_1,
	EDIT_Z_2,
	EDIT_Z_3,
	EDIT_Z_4,     	//8
	
	EDIT_J_1,
	EDIT_J_2,
	EDIT_J_3,
	EDIT_J_4,		//12
	
	EDIT_F_1,
	EDIT_F_2,
	EDIT_F_3,	
	EDIT_F_4,		//16
		
	EDIT_P_1,
	EDIT_P_2,
	EDIT_P_3,
	EDIT_P_4,		//20
	
	EDIT_G_1,
	EDIT_G_2,
	EDIT_G_3,
	EDIT_G_4,		//24
	
	EDIT_A_U,
	EDIT_A_I,
	EDIT_SY_A,		//27
	
	EDIT_B_U,
	EDIT_B_I,
	EDIT_SY_B,		//30
	
	EDIT_C_U,	
	EDIT_C_I,
	EDIT_SY_C,		//33
	EDIT_SY_ALL,
	EDIT_M_TIME,
	EDIT_TIP,		//36
	EDIT_YEAR,
	EDIT_MONTH,
	EDIT_DAY,
	EDIT_HOUR,
	EDIT_MINUTE,
	EDIT_SECOND
};

void app_VMmod_window_create(void);
void app_VMmod_window_destory(void);
void app_VMmod_guiyue_display(uint8_t m_offset);
void app_VMmod_baud_display(uint8_t m_offset);
void app_VMmod_CRC_display(uint8_t m_offset);
void app_VMmod_meter_addr_display(void *pdata);
void app_VMmod_set_init_display(void);
void app_VMmod_main_edit_display( VMMeter_dl_info_t * m_data_buf );
void app_VMmod_CRC_select(uint8_t m_offset);
void app_VMmod_baud_select(uint8_t m_offset);
void app_VMmod_Usart3_send(void *data,Bsize_t len);

extern hwin_t* m_VMwin_pWIN;
extern hwin_t* m_VMwin_edit;
extern hwin_t* m_VM_droplist_guiyue;
extern hwin_t* m_VM_droplist_baud;
extern hwin_t* m_VM_droplist_crc;
/*********************************************************************
* APP_WAVEMOD-H  for VMMOD Protocol_analysis
**********************************************************************/
uint8_t app_VMmod_rec_addr_judge( uint8_t *m_buf );
uint8_t app_VMmod_rec_code_flag_judge( uint8_t *m_buf );
void app_VMmod_send_VMdata( void );
void app_VMmod_unusual_send_VMdata( void );

typedef struct
{
	uint8_t head1;
	uint8_t addr[6];
	uint8_t head2;
	uint8_t cmd;
	uint8_t len;
	uint8_t crc;
	uint8_t end;
}send_VMdata_t;	//���ݽṹ


/*********************************************************************
* APP_WAVEMOD-H  for VMMOD connect
**********************************************************************/
void app_VMmod_set_data_save( VMMeter_dl_info_t *p_arg, char *m_name );
void app_VMmod_set_data_read( VMMeter_dl_info_t *p_arg, char *m_name );

#define     VM97_DATA_FILE		 ((char *)"VMD97")
#define     VM07_DATA_FILE		 ((char *)"VMD07")

#endif

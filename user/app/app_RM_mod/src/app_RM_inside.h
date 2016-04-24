#include "../../../includes.h"
#include "../../../Gui\Gui.h"
#ifndef _PEC_APP_RM_INSIDE_2013_H
#define _PEC_APP_RM_INSIDE_2013_H

void  	bsp_delay_ms( unsigned long	ms );
extern 	uint16_t 	m_GUI_3D_window_Bg_cor;
extern const unsigned char gImage_entery[];
extern const unsigned char gImage_return[];
extern const unsigned char gImage_save[];
extern const unsigned char gImage_exit[];
uint8_t app_USERinfo_Save(void);  
/*********************************************************************
* APP_RMMOD-H  for RMMOD fun
**********************************************************************/
void 		app_RMmod_fun_entry( void );
void 		app_RMmod_fun_exit( void );

extern	char m_meter_addr[13];
extern	dev_id_t g_usart3_dev_id;

void  	bcd_convert_hex( uint8_t bcd_src, uint8_t *pur_ptr );
void  	hex_convert_bcd( uint8_t src, uint8_t *bcd_src );
void 	hand_wirte_input_box_pos_set(uint16_t x ,uint16_t y);

/*enum //У�鷽ʽ  Odd  �� Event ,none
{
	NONE_CHECK = 0,
	ODD_CHECK ,
	EVEN_CHECK
};*/
/*#define   SET_SERIAL_BAUD   	0X01
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
	uint8_t guiyue;					//��Լ
	uint8_t baud;					//������
	uint8_t crc;					//У��λ
	char meter_Addr[13];			//���ַ
	char zy_power_ALL[M_SIZE];  	//�����й������ 
	char zy_power_JSALL[M_SIZE];  	//�����й������� 
	char zy_power_J[M_SIZE];		//��
	char zy_power_F[M_SIZE];		//��
	char zy_power_P[M_SIZE];		//ƽ
	char zy_power_G[M_SIZE];		//��
	char zw_power_ALL[M_SIZE];  	//�����޹������
	char zw_power_JSALL[M_SIZE];  	//�����޹�������
	char zw_power_J[M_SIZE];		//��
	char zw_power_F[M_SIZE];		//��
	char zw_power_P[M_SIZE];		//ƽ
	char zw_power_G[M_SIZE];		//��	
	char fy_power_ALL[M_SIZE];  	//�����й������
	char fy_power_JSALL[M_SIZE];  	//�����й�������
	char fy_power_J[M_SIZE];		//��
	char fy_power_F[M_SIZE];		//��
	char fy_power_P[M_SIZE];		//ƽ
	char fy_power_G[M_SIZE];		//��	
	char fw_power_ALL[M_SIZE];  	//�����޹������
	char fw_power_JSALL[M_SIZE];  	//�����޹�������
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
	
	char lost_A[M_SIZE];			//Aʧѹ
	char lost_B[M_SIZE];			//Bʧѹ
	char lost_C[M_SIZE];			//Cʧѹ
	char lost_ALL[M_SIZE];		//ȫʧѹ
	char meter_time[METER_INFO_SIZE];			//��ʱ��
	char meter_password[9];			// ����
	char m_temp[10];			    // �¶�
	char m_hum[10];					// ʪ��
}RMeter_dl_info_t;


/******************************�����ݽṹ*************************************/
typedef struct
{
	uint8_t head[3];//�ڷ���֡��Ϣ֮ǰ,����1��4���ֽ�FEH���Ի��ѽ��շ���
	uint8_t head1;
	uint8_t addr[6];
	uint8_t head2;
	uint8_t cmd;
	uint8_t len;
	uint8_t biaoshi_L;
	uint8_t biaoshi_H;
	uint8_t crc;
	uint8_t end;
}send_data_97_t;    //97�����ݽṹ

typedef struct
{
	uint8_t head[3];//�ڷ���֡��Ϣ֮ǰ���ȷ���1��4���ֽ�FEH���Ի��ѽ��շ���
	uint8_t head1;
	uint8_t addr[6];
	uint8_t head2;
	uint8_t cmd;
	uint8_t len;
	uint8_t biaoshi[4];
	uint8_t crc;
	uint8_t end;
}send_data_07_t;	//97�����ݽṹ

/******************************ʱ��**************************************/
typedef struct
{
	uint8_t head[3];//�ڷ���֡��Ϣ֮ǰ,����1��4���ֽ�FEH���Ի��ѽ��շ���
	uint8_t head1;
	uint8_t addr[6];
	uint8_t head2;
	uint8_t cmd;
	uint8_t len;
	uint8_t biaoshi_L;
	uint8_t biaoshi_H;
    uint8_t password[4];	
	uint8_t Second;
	uint8_t Minute;
	uint8_t Hour;
	uint8_t crc;
	uint8_t end;
}send_hour_97_t;


typedef struct
{
	uint8_t head[3];//�ڷ���֡��Ϣ֮ǰ���ȷ���1��4���ֽ�FEH���Ի��ѽ��շ���
	uint8_t head1;
	uint8_t addr[6];
	uint8_t head2;
	uint8_t cmd;
	uint8_t len;
	uint8_t biaoshi_L;
	uint8_t biaoshi_H;
    uint8_t password[4];	
	uint8_t week;
	uint8_t Day;
	uint8_t Month;
	uint8_t Year;
	uint8_t crc;
	uint8_t end;
}send_date_97_t;

typedef struct
{
	uint8_t head[3];//�ڷ���֡��Ϣ֮ǰ���ȷ���1��4���ֽ�FEH���Ի��ѽ��շ���
	uint8_t head1;
	uint8_t addr[6];
	uint8_t head2;
	uint8_t cmd;
	uint8_t len;
	uint8_t biaoshi_L;//������������ݱ�ʶ�����ݡ������
	uint8_t biaoshi_H;
    uint8_t password[4];
	//void*	m_data_buf;
	uint8_t crc;
	uint8_t end;
}set_data_97_t;

typedef struct
{
	uint8_t head[3];//�ڷ���֡��Ϣ֮ǰ���ȷ���1��4���ֽ�FEH���Ի��ѽ��շ���
	uint8_t head1;
	uint8_t addr[6];
	uint8_t head2;
	uint8_t cmd;
	uint8_t len;
	uint8_t biaoshi[4];
    uint8_t password[4];	
	uint8_t user_number[4];//�����ߴ���
	uint8_t Second;
	uint8_t Minute;
	uint8_t Hour;
	uint8_t crc;
	uint8_t end;
}send_hour_07_t;

typedef struct
{
	uint8_t head[3];//�ڷ���֡��Ϣ֮ǰ���ȷ���1��4���ֽ�FEH���Ի��ѽ��շ���
	uint8_t head1;
	uint8_t addr[6];
	uint8_t head2;
	uint8_t cmd;
	uint8_t len;
	uint8_t biaoshi[4];
    uint8_t password[4];	
	uint8_t user_number[4];//�����ߴ���
	uint8_t week;
	uint8_t Day;
	uint8_t Month;
	uint8_t Year;
	uint8_t crc;
	uint8_t end;
}send_date_07_t;

typedef struct
{
	uint8_t head[3];//�ڷ���֡��Ϣ֮ǰ���ȷ���1��4���ֽ�FEH���Ի��ѽ��շ���
	uint8_t head1;
	uint8_t addr[6];
	uint8_t head2;
	uint8_t cmd;
	uint8_t len;
	uint8_t biaoshi[4];
    uint8_t password[4];	
	uint8_t user_number[4];//�����ߴ���
	//void*   m_data_buf;
	uint8_t crc;
	uint8_t end;
}set_data_07_t;

typedef struct
{
	uint8_t head[3];//�ڷ���֡��Ϣ֮ǰ���ȷ���1��4���ֽ�FEH���Ի��ѽ��շ���
	uint8_t head1;
	uint8_t addr[6];
	uint8_t head2;
	uint8_t cmd;
	uint8_t len;
	uint8_t Second;
	uint8_t Minute;
	uint8_t Hour;
	uint8_t Day;
	uint8_t Month;
	uint8_t Year;
	uint8_t crc;
	uint8_t end;
}broadcast_time_t;

typedef struct
{
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint8_t week;
	uint8_t month;
	uint8_t year;

}time_data_t;
/*****************************�����ն�********************************************/
typedef struct
{
	uint8_t head[3];	//�ڷ���֡��Ϣ֮ǰ���ȷ���1��4���ֽ�FEH���Ի��ѽ��շ���
	uint8_t head1;
	uint8_t addr[4];
	uint8_t msl[2];		// ΪMSTA, FSEQ,ISEQ,��վ��ַ���������
	uint8_t head2;
	uint8_t cmd;
	uint8_t len_L;		//���ȵ�λ
	uint8_t len_H;		//���ȸ�λ
	uint8_t TNM[8];		//�������־
	uint8_t biaoshi_L;
	uint8_t biaoshi_H;
	uint8_t crc;
	uint8_t end;
}terminal_send_t;

typedef struct
{
	uint8_t head[3];	//�ڷ���֡��Ϣ֮����ȷ���1��4���ֽ�FEH���Ի��ѽ��շ���
	uint8_t head1;
	uint8_t addr[4];
	uint8_t msl[2];		// ΪMSTA, FSEQ,ISEQ,��վ��ַ���������
	uint8_t head2;
	uint8_t cmd;
	uint8_t len_L;
	uint8_t len_H;
	uint8_t TN;			//�������
	uint8_t AUT; 		//Ȩ��
    uint8_t password[3];
	uint8_t biaoshi_L;
	uint8_t biaoshi_H;
	uint8_t Second;
	uint8_t Minute;
	uint8_t Hour;
	uint8_t crc;
	uint8_t end;
}send_hour_terminal_t;


typedef struct
{
	uint8_t head[3];	//�ڷ���֡��Ϣ֮ǰ���ȷ���1��4���ֽ�FEH���Ի��ѽ��շ���
	uint8_t head1;
	uint8_t addr[4];
	uint8_t msl[2];		// ΪMSTA, FSEQ,ISEQ,��վ��ַ���������
	uint8_t head2;
	uint8_t cmd;
	uint8_t len_L;
	uint8_t len_H;
	uint8_t TN;			//�������
	uint8_t AUT; 		//Ȩ��
	uint8_t password[3];
	uint8_t biaoshi_L;
	uint8_t biaoshi_H;	
	uint8_t week;
	uint8_t Day;
	uint8_t Month;
	uint8_t Year;
	uint8_t crc;
	uint8_t end;
}send_date_terminal_t;
/**********************************���ն������ݰ�ͷ************************************************/
typedef struct
{
	uint8_t head1;
	uint8_t addr[6];
	uint8_t head2;
	uint8_t cmd;
	uint8_t len;		//����
}rev_645_head_t;


typedef struct
{
	uint8_t head1;
	uint8_t addr[4];
	uint8_t msl[2];		// ΪMSTA, FSEQ,ISEQ,��վ��ַ���������
	uint8_t head2;
	uint8_t cmd;
	uint8_t len_L;		//���ȵ�λ
	uint8_t len_H;		//���ȸ�λ
	uint8_t TNM[8];
	uint8_t biaoshi_L;
	uint8_t biaoshi_H;
}rev_terminal_head_t;

/**********************************************************************************/
typedef struct
{
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t day;
	uint8_t month;
	uint8_t year;
	uint8_t week;
}time_val_t;

enum
{
	GUIYUE_645_97=0,
	GUIYUE_645_07,	
	GUIYUE_FK_0903
};
/*********************************************************************
* APP_RMMOD-H  for RMMOD INTERFACE
**********************************************************************/
//#define   	RM_BG_COLOR          	    		C_GUI_HIGH_BLUE	  /*�����ɫ*/
#define   	RM_FG_COLOR               			C_GUI_WHITE       /*����ǰ��ɫ*/
#define   	RM_WIN_BG_COLOR       				799/*���ڵ�ɫ*/
#define   	BK_CORLOR    						RM_BG_COLOR
#define   	RM_BG_COLOR          	    		RM_WIN_BG_COLOR	  /*�����ɫ*/
//#define   	BK_CORLOR    0xce79
#define   	LBK_CORLOR   C_GUI_WHITE//LCD_Color2Index_565(0x404040)


enum
{
	GUI_KEY_METER_MOON_JIAOSHI=100,
	GUI_KEY_METER_BROADCAST_JIAOSHI,
	GUI_KEY_GUIYUE_SELECT,
	GUI_KEY_BAUD_SELECT,
	GUI_KEY_CRC_SELECT,
	GUI_KEY_ENTER,
	GUI_KEY_SETTING,
	GUI_KEY_CANCEL,
	GUI_KEY_READ_M,
	GUI_KEY_EXIT,
	GUI_KEY_XNFZ_BUTTON,
	GUI_KEY_XNFZ_START,
	GUI_KEY_XNFZ_CLOSE,
	GUI_KEY_HALT
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
	EDIT_ALL_1,
	EDIT_ALL_2,
	EDIT_ALL_3,
	EDIT_ALL_4,
	EDIT_M_PASSWORD
};

enum
{
	METER1_CS = 1,  	
	METER2_CS,
	METER3_CS,
	METER1_QS,    
	METER2_QS, 
	METER3_QS,
	TMR1_CS,
	TMR2_CS,
	TMR3_CS,
	CT_BL,  	
	EDIT_U_A,
	EDIT_U_B,
	EDIT_U_C,
	EDIT_I_A,
	EDIT_I_B,
	EDIT_I_C,
	EDIT_J_A,
	EDIT_J_B,
	EDIT_J_C,	
	EDIT_P_A,
	EDIT_P_B,
	EDIT_P_C,
	EDIT_Q_A,
	EDIT_Q_B,
	EDIT_Q_C,
	EDIT_JUAB,
	EDIT_JUAC,
	EDIT_JUBC,
	EDIT_JIAC,
	EDIT_JIAB,
	EDIT_JIBC,
	EDIT_QABC,
	EDIT_PABC,
	EDIT_COS,
	EDIT_FEQ,
	EIDT_USER_U,
	EIDT_USER_I,
	EIDT_USER_JA,
	EIDT_USER_JB,
	EIDT_USER_JC,
	EIDT_USER_JUAB,
	EIDT_USER_JUAC,
	EIDT_USER_JUBC	
};

void app_RMmod_window_create(int flag);
void app_RMmod_window_destory(void);
extern hwin_t *m_RMwin_MpWIN;
extern hwin_t *m_RMwin_DLWIN;
extern hwin_t *m_RMwin_LJTWIN;
extern hwin_t *m_RMwin_pWIN;
extern hwin_t *m_RMwin_edit;
extern hwin_t *m_meter_droplist_guiyue;
extern hwin_t *m_meter_droplist_baud;
extern hwin_t *m_meter_droplist_crc;
void app_RMmod_clean_display(void);
void app_RMmod_set_info_display(void);
void app_RMmod_rev_data_ftoa_and_display(float fdata,char *pdata,Bsize_t id);
void app_RMmod_rev_data_display(char *pdata,Bsize_t id);
void app_RMmod_rev_tip_display(void *pdata);
void app_RMmod_rev_tip2_display(uint8_t offset);
void app_RMmod_guiyue_display(uint8_t m_offset);
void app_RMmod_baud_display(uint8_t m_offset);
void app_RMmod_CRC_display(uint8_t m_offset);
void app_RMmod_meter_addr_display(void *pdata);
void app_RMmod_set_info_display(void);
void app_RMmod_logic_addr_NOTdisplay( void );
void app_RMmod_notice_window_display(void);
void app_RMmod_main_window_create(void); 
void app_RMmod_main_window_destory(void); 
void app_RM_DLarg_display(measure_param_t*pDL);
void app_RMmod_97_set_meter(uint8_t* m_DI, void* m_buf, uint8_t m_size );
void app_RMmod_07_set_meter(uint8_t* m_DI, void* m_buf, uint8_t m_size );
void app_RM_XNFZ_state_view(uint16_t color);
void app_RMmod_XNFZ_steady_handle( void );
void app_RM_DLmember_window_init(hwin_t*hp);

/*********************************************************************
* APP_RMMOD-H  for RMMOD FUN
**********************************************************************/
void app_RMmod_scia_send(void *data,Bsize_t len);
void app_RMmod_CRC_select(uint8_t m_offset,uint8_t m_flag);
void app_RMmod_Usart3_send(void *data,Bsize_t len);
extern	RMeter_dl_info_t m_meter_data;
extern	time_val_t m_meter_time_buf;
extern	uint8_t   	m_RM_rev_flag;
extern	dev_id_t	g_usart3_dev_id;
extern	dev_id_t	g_timer_dev_id;
extern 	dev_id_t	g_sht11_dev_id;
extern  uint8_t   	m_offset_guiyue;
extern  uint8_t   	m_offset_baud;
extern  uint8_t   	m_offset_crc;
extern	uint8_t		g_RM_3or4_display_flag;
/*********************************************************************
* APP_RMMOD-H  for RMMOD IEC1107
**********************************************************************/
extern	uint8_t g_guiye_DLMS_flag;
void app_RM_readout_to_meterARG( void );
void app_RM_readout_data_IEC( void );
void app_RMmod_logic_addr_display( uint8_t m_logic_addr);
void app_RMmod_protocol_tip_display( void );

/*********************************************************************
* APP_RMMOD-H  for RMMOD CheckTime
**********************************************************************/
void app_RMmod_set_time_by_command_entry(void);
void app_RMmod_broadcast_send_time_data(void);
int CharToHex(char * str);
void app_RMmod_broadcast_CheckTime_handle(void);
extern	char 	  m_meter_Password[];
/*********************************************************************
* APP_RMMOD-H  for RMMOD XNFZ
**********************************************************************/
extern uint8_t   task_RM_XNFZ_id;
extern uint8_t 	 g_RM_XNFZ_start;

enum
{
	XNFZ_IB = 1,  	
	XNFZ_FEQ,  	
	XNFZ_IMAX,  	
	XNFZ_U 
};

void app_RMmod_measure_auto_change_Ilevel( float m_IB_grade );
void app_RM_XNFZ_task(void*arg);
void app_RMmod_xnfz_window_create( void );
void app_RMmod_xnfz_window_destory( void );
BOOL app_RMmod_XNFZ_state_get( void );
uint8_t app_RM_XNFZ_3X4X_3X3X_judge(void);
void app_RMmod_XNFZ_unlawful_close( void );
void app_RM_3or4_get( void );

/*********************************************************************
* APP_RMMOD-H  for RMMOD VERIFY
**********************************************************************/
void app_RM_verify_funtion2( void );
void app_RM_verify_funtion3( void );
void app_RM_verify_funtion4( void );
extern a_q_t *  m_RMmod_keymsg_qevent;

/*********************************************************************
* APP_RMMOD-H  for RMMOD FLVERIFY
**********************************************************************/
enum
{
	EDIT_FL_CODE = 0,
	EDIT_FL_NUM,
	EDIT_FL_HOUR,
	EDIT_FL_MIN,
	EDIT_FL_END
};

enum
{
	GUI_KEY_FL_NTSELECT = 120,
	GUI_KEY_FL_SQSELECT,
	GUI_KEY_FL_FL,
	GUI_KEY_FLENTER,
	GUI_KEY_FLSETTING,
	GUI_KEY_FLSET_SET,
	GUI_KEY_FLSET_ADD,
	GUI_KEY_FL_WRITE,
	GUI_KEY_FL_READ,
	GUI_KEY_FLSET_DELETE,
	GUI_KEY_FLEXIT,
	GUI_KEY_FLHALT
};

extern hwin_t* m_FL_droplist_Ntime;
extern hwin_t* m_FL_droplist_FL;

typedef struct
{
	uint8_t m_offset;		// �ڼ�ʱ��
	uint8_t m_fl_offset;	// ����ƫ��(���ʺ�)
	char	m_fl[3];		// ����
	char    m_fl_hour[3];   // ʱ�� ʱ
	char	m_fl_min[3];    // ʱ�� ��
	uint8_t m_valid_flag;	// ��Ч��־
}FL_buf_t;

/*********************************************************************
* APP_RMMOD-H  for RMMOD LJT
**********************************************************************/
void  app_RM_LJT_init( hwin_t*hp);
void  app_RM_LJT_display( hwin_t*hp, measure_param_t*Pdl,DL_flag_t*Pdl_flag);

/*********************************************************************
* APP_RMMOD-H  for RMMOD XLVERIFY
**********************************************************************/
enum
{
	EDIT_XL_PNUM = 0,
	EDIT_XL_PTIME,
	EDIT_XL_BAT_TIME,
	EDIT_XL_BAT_STATE,
	EDIT_XL_BAT_VOL,
	EDIT_XL_ACCOUNT_DAY1,
	EDIT_XL_ACCOUNT_HOUR1,
	EDIT_XL_ACCOUNT_DAY2,
	EDIT_XL_ACCOUNT_HOUR2,
	EDIT_XL_ACCOUNT_DAY3,
	EDIT_XL_ACCOUNT_HOUR3,
	EDIT_XL_CALC_WAY,
	EDIT_XL_PERIOD,
	EDIT_XL_HC_TIME,
	EDIT_XL_PASSWORD,
	EDIT_XL_END
};

enum
{
	GUI_KEY_XL_READ = 120,
	GUI_KEY_XL_WRITE_SET,
	GUI_KEY_XL_WRITE,
	GUI_KEY_XL_WAY,
	GUI_KEY_XL_ABP_READ,
	GUI_KEY_XL_ACC_WRITE,
	GUI_KEY_XL_ACC_SET
};

typedef struct
{
	uint8_t m_battery_sate;		//�������״̬
	char 	m_P_num[8];			//��̴���
	char 	m_P_time[18];		//���һ�α��ʱ��
	char 	m_battery_RunT[10];	//��ع���ʱ��
	float 	m_battery_V;		//��ص�ѹ
}XL_other_read_t;

typedef struct
{
	uint8_t m_calc_way;			//�������������ʽ
	char 	m_XL_period[3];		//�����������
	char 	m_HC_time[3];		//����ʱ��
}XL_reg_t;

typedef struct
{
	char m_At1_day[3];
	char m_At1_hour[3];
	char m_At2_day[3];
	char m_At2_hour[3];
	char m_At3_day[3];
	char m_At3_hour[3];
}XL_account_t;

/*********************************************************************
* APP_RMMOD-H  for RMMOD SZVERIFY
**********************************************************************/
enum
{
	EDIT_SZ_TP_IB = 0,
	EDIT_SZ_TP_IMAX,
	EDIT_SZ_PERIOD,
	EDIT_SZ_HC_TIME,
	EDIT_SZ_RUN_TIME,
	EDIT_SZ_HC_NUM,
	EDIT_SZ_PASSWORD,
	EDIT_SZ_TM_MXL1,
	EDIT_SZ_TM_RXL1,
	EDIT_SZ_TM_SZE1,
	EDIT_SZ_TM_ERR1,
	EDIT_SZ_TM_RES1,
	EDIT_SZ_TM_MXL2,
	EDIT_SZ_TM_RXL2,
	EDIT_SZ_TM_SZE2,
	EDIT_SZ_TM_ERR2,
	EDIT_SZ_TM_RES2,
	EDIT_SZ_TM_MXL3,
	EDIT_SZ_TM_RXL3,
	EDIT_SZ_TM_SZE3,
	EDIT_SZ_TM_ERR3,
	EDIT_SZ_TM_RES3,
	EDIT_SZ_END
};

enum
{
	GUI_KEY_SZ_READ = 120,
	GUI_KEY_SZ_TP_SET,
	GUI_KEY_SZ_YFH_POINT,
	GUI_KEY_SZ_01IB_POINT,
	GUI_KEY_SZ_IB_POINT,
	GUI_KEY_SZ_IMAX_POINT,
	GUI_KEY_SZ_TT_SET,
	GUI_KEY_SZ_TT_READ,
	GUI_KEY_SZ_TM_XLCLEAR,
	GUI_KEY_SZ_TM_SET,
	GUI_KEY_SZ_TM_START,
	GUI_KEY_SZ_TM_STOP,
	GUI_KEY_SZ_END
};

typedef struct
{
	char m_Ib[8];
	char m_Imax[8];
}sz_TI_point_t;

typedef struct
{
	char m_XL_period[3];
	char m_HC_time[3];
	char m_HC_num[3];
	char m_run_time[3];
}sz_TT_t;

typedef struct
{
	char m_Mxl[3][9];
	char m_Rxl[3][9];
	char m_XL_SZR[3][9];
	char m_XL_ERR[3][9];
	char m_XL_RES[3][7];
}sz_result_t;

void app_XNFZ_load_for_SZ( float m_Iset );

/*********************************************************************
* APP_RMMOD-H  for RMMOD CHECKTIME
**********************************************************************/
void app_RMmod_time_ymdw_97_set(void);
void app_RMmod_time_ymdw_07_set(void);
void app_RMmod_time_hms_97_set(void);
void app_RMmod_time_hms_07_set(void);

/*********************************************************************
* APP_RMMOD-H  for search
**********************************************************************/
typedef struct
{
	RMeter_dl_info_t 	m_RM_data;
}RM_save_data_t;

extern  uint8_t flag;

extern  RM_save_data_t RM_save_data;

void 	app_RMmod_search( void *arg );

void 	app_RMmod_record_display(void*data);

void 	app_RMmod_data_all_display(RMeter_dl_info_t *m_pd,uint8_t m_flag);
#endif

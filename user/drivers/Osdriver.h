#ifndef _2010_OS_DRIVER_H_
#define _2010_OS_DRIVER_H_
#include        "../usertype.h"
typedef struct _phase_param_t
{
	float f_U; 
	float f_I;
	float f_J;
	float f_P;
	float f_Q;
}phase_param_t;

typedef struct _measure_param_t
{
	phase_param_t phase_a;
	phase_param_t phase_b;
	phase_param_t phase_c;
	float f_JIac;
	float f_JIbc;
	float f_JIab;
	float f_JUac;
	float f_JUbc;
	float f_JUab;
	float f_Pabc;
	float f_Qabc;
	float f_path1_err[4];
	float f_path2_err[4];
	float f_Feq;
	float f_Cos;
	char  cos_LC;
}measure_param_t;

#include "../os_connect\os_connect.h"
#include 	"../support\app_support.h"
/*********************************************************************
* OSDRIVER-H    for key_driver
**********************************************************************/
enum
{
	KEY_CMD_DUMMY_PRESS   =   0,
	KEY_CMD_BUF_EMPTY
};
enum
{
	KEY_F1 		= 26,
	KEY_F2 		= 27,
	KEY_F3 		= 28,
	KEY_F4 		= 29,
	KEY_F5 		= 30,
	KEY_SAVE 	= 21,
	KEY_SEARCH 	= 16,
	KEY_EXIT 	= 15,
	KEY_LEFT 	= 17,
	KEY_RIGHT 	= 20,
	KEY_UP 		= 22,
	KEY_DOWN 	= 18,
	KEY_SETTING = 25,
	KEY_SWITCH 	= 24,
	KEY_ENTER 	= 23,
	KEY_SELF 	= 10,
	KEY_USER    = 5,
	KEY_0 		= 4,
	KEY_1 		= 11,
	KEY_2 		= 12,
	KEY_3 		= 13,
	KEY_4 		= 6,
	KEY_5 		= 7,
	KEY_6 		= 8,
	KEY_7 		= 1,
	KEY_8		= 2,
	KEY_9 		= 3,
	KEY_POINT 	= 14,
	KEY_ADD_SUB = 9,
	KEY_SYS     = 14,
	KEY_DEL     = KEY_F5
};
dev_id_t key_driver_register( void );
/*********************************************************************
* OSDRIVER-H    FOR SERIAL
**********************************************************************/
dev_id_t serialC_driver_register( void );
dev_id_t serialB_driver_register( void );
dev_id_t serialD_driver_register( void );
void serialD_data_ISR(uint8_t *ch);

#define 	MAX_REV_FIFO_SIZE           256
#define 	GPS_REV_FIFO_SIZE           1024

typedef struct
{
	uint32_t set_baud;
	uint8_t  set_SCIHBAUD;
	uint8_t  set_SCILBAUD;
} baud_set_t;

enum /*校验方式  Odd  或 Event ,none*/
{
	NONE_CHECK = 0,
	ODD_CHECK ,
	EVEN_CHECK
};
#define   SET_SERIAL_BAUD   	0X01
#define   SET_SERIAL_CHECK 		0X02
#define   SET_EMPTY_FIFO   		0X03
#define   GET_FIFO_STATE   	 	0X04
#define   SET_SERIAL_CHECK2 	0X05


typedef enum
{
	AT_CMD_OPEN=10,
	AT_CMD_ENTER_CODE,
	AT_CMD_EXIT,
	AT_CMD_HANDSHAKE,
	AT_CMD_DEFAULT,
	AT_CMD_ADDR_VIEW,
	AT_CMD_VER_VIEW,
	AT_CMD_PIN_VIEW,
	AT_CMD_PIN_SET,
	AT_CMD_BAUD_VIEW,
	AT_CMD_BAUD_SET,
	AT_CMD_UARTSTOP_SET,
	AT_CMD_UARTPARITY_SET,
	AT_CMD_ROLE_VIEW,
	AT_CMD_ROLE_SET,
	AT_CMD_INQMOD_GET,
	AT_CMD_MAX_DEVICES_SET,
	AT_CMD_INQ_TIME_SET,
	AT_CMD_INQ_START,
	AT_CMD_BINDADDR_VIEW,
	AT_CMD_BINDADDR_SET,
	AT_CMD_NAME_VIEW,
	AT_CMD_NAME_SET,
	AT_CMD_MSG_VIEW,
	AT_CMD_MSG_VIEW_ONOFF,
	AT_CMD_SETTING_SET_ALL,
	AT_CMD_SETTING_INQURY_ALL,
	CMD_SET_BT_AT_DEFAULT_STATE
}AT_CMD_t;

/*********************************************************************
* OSDRIVER-H  for HT1380
**********************************************************************/
dev_id_t 	   timer_driver_register( void );
enum
{
	TMR_CMD_VALUE_SAVE = 0,
	TMR_CMD_TIME_VALUE_GET,
	TMR_CMD_TIME_STRING_SAVE,
	TMR_CMD_TIME_STRING_GET,
	TMR_CMD_TIME_DISSTRING_GET
};
typedef struct
{
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t day;
	uint8_t month;
	uint8_t year;
	uint8_t week;
}time_value_t;

typedef struct
{
	char year_buf[3];
	char month_buf[3];
	char day_buf[3];
	char hour_buf[3];
	char min_buf[3];
	char sec_buf[3];
	char week_buf[3];
}time_string_t;
/*********************************************************************
* OSDRIVER-H  FOR TOUCH
**********************************************************************/
dev_id_t touch_driver_register( void );
typedef struct
{
	Bsize_t x;
	Bsize_t y;
	Bsize_t state;
}touch_data_t;
typedef struct
{
	uint8_t  head;
	uint8_t  cmd;
	uint8_t  lenth;
}eph1200_pact_t;
enum
{
	TOUCH_CMD_SW_WRITE_MOD=0,
    TOUCH_CMD_SW_TOUCH_MOD,
    TOUCH_CMD_GET_HZ_BOX,
    TOUCH_CMD_GET_HZ_FLAG,
    TOUCH_CMD_SET_WRITE_AREA
};
/*********************************************************************
* OSDRIVER-H  for measure
**********************************************************************/
#include "src\measure\measure.h"

/*驱动命令*/
enum
{
	MEASURE_CMD_START = 0,        	/*测量数据开始*/
	MEASURE_CMD_STOP,               /*停止测量*/
	MEASURE_CMD_DL_PARAM_GET, 		/*获取测量数据*/
	MEASURE_CMD_PATH1_ERR_GET, 		/*获取通道1测量误差数据*/
	MEASURE_CMD_PATH2_ERR_GET, 		/*获取通道2测量误差数据*/
	MEASURE_CMD_PATH3_ERR_GET, 		/*获取通道3测量误差数据*/
	MEASURE_CMD_DLFLAG_GET,    		/*获取标志数据*/
	MEASURE_CMD_XBDATA_GET,    		/*获取标志数据*/
	MEASURE_CMD_PATH1_ERR_CIRCLE_GET,
	MEASURE_CMD_PATH2_ERR_CIRCLE_GET,
	MEASURE_CMD_PATH3_ERR_CIRCLE_GET,
	MEASURE_CMD_USER_QB_SELECT,
	MEASURE_CMD_USER_DL_ARG_SET,
	MEASURE_CMD_CORRECT_STEP_SET,
	MEASURE_CMD_CELAN_CUR_CORRECT_DATA,
	MEASURE_CMD_CELAN_ALL_CORRECT_DATA,
	MEASURE_CMD_GET_CORRECT_DATA,
	MEASURE_CMD_GET_DDSN,
	MEASURE_CMD_GET_CORRECT_FILE,
	MEASURE_CMD_SET_UDL_LEVEL,
	MEASURE_CMD_SET_IDL_LEVEL,
	MEASURE_CMD_GET_AD_DATA,
	MEASURE_CMD_SELECT_DOUBLE_METER_MOD,
	MEAUSRE_CMD_REAL_BUF_GET,
	MEASURE_CMD_USER_SELECT_ERRWAY,
	MEASURE_CMD_USER_LOAD_ZSSERR,
	MEASURE_CMD_CORRECT_COPY,
	MEASURE_CMD_IAUTO_LEVEL_STOP,
	MEASURE_CMD_GET_REAL_DLSR,
	MEASURE_CMD_GET_REAL_DYSR,
	MEASURE_CMD_ICONST_CHANGE
};
dev_id_t measure_driver_register( void );
/*********************************************************************
* OSDRIVER-H 	 for 		PIC32
**********************************************************************/
typedef struct
{
	char year_buf[3];
	char month_buf[3];
	char day_buf[3];
	char hour_buf[3];
	char min_buf[3];
	char sec_buf[3];
	char msec_buf[4];
}GPS_Time_Date_t;

extern	uint8_t     m_GPS_rev_fifo[];
extern	uint8_t		g_GPS_data_end;
extern	uint8_t		g_GPS_data_lost;
extern	uint8_t		g_power_state;		//全局的电源状态标志
extern	uint8_t		g_cur_power_up;		//全局的当前供电电源标志,0为外部电源,1为电池
extern	uint8_t		g_battery_charge;	//全局的电池充电标志,1为正在充电,0为无充电
extern	uint8_t		g_battery_num;		//全局的电池显示格子个数
extern	GPS_Time_Date_t	  g_GPS_buf;
void	pic32_driver_GPS_data_obtain(void);
extern	float       g_pic_wd;  	//温度
extern	float       g_pic_sd;  	//湿度
/*********************************************************************
* OSDRIVER-H 	 for 		SPI
**********************************************************************/
enum
{
	SET_SPI_BAUD =0,
	SET_SD_CS_EN,	
	SET_SD_CS_DIS,
	M25P80_SPI_CS_EN,
	M25P80_SPI_CS_DIS,
	FM25C164_SPI_CS_EN,
	FM25C164_SPI_CS_DIS
};
dev_id_t 	spi_driver_register( void );
/*********************************************************************
* OSDRIVER-H 	 for  GPIO
**********************************************************************/
enum
{
	GPIO_CMD_SET_EXT_IO_DATA =0,
	GPIO_CMD_GET_EXT_BUF,
	GPIO_CMD_SET_BUZ_ON,
	GPIO_CMD_SET_BUZ_OFF,
	GPIO_CMD_SET_485_COM,
	GPIO_CMD_WIFI_RESET
};
/*在自动供电中当同时有多个供电方式时,优先级如下:
		1:优先适配器供电,2:然后开关电源供电,3:最后电池供电*/
enum
{
	PS_NO_POWER = 0,			/*无供电*/
	PS_BATTERY,  				/*电池供电*/
	PS_EXTRAL,     				/*开关电源供电*/
	PS_EXTRAL_BATTERY,     		/*开关电源和电池供电*/
	PS_ADAPTER, 				/*适配器供电*/
	PS_ADAPTER_BATTERY, 		/*适配器和电池供电*/
	PS_ADAPTER_EXTRAL, 			/*适配器和开关电源供电*/
	PS_ADAPTER_EXTRAL_BATTERY, 	/*适配器、开关电源和电池供电*/
	PS_PA_BATTERY, 				/*适配器保护,电池供电*/
	PS_PA_EXTRAL, 				/*适配器保护,开关电源供电*/
	PS_PA_EXTRAL_BATTERY 		/*适配器保护,开关电源和电池供电*/
};

dev_id_t 	gpio_driver_register( void );

#define     EXTIO_OFFSET_GET(chipN,offset)   ((chipN-1)*8+offset)
#define     Q0            0
#define     Q1            1
#define     Q2            2
#define     Q3            3
#define     Q4            4
#define     Q5            5
#define     Q6            6
#define     Q7            7

#define     E2PROM_CS    EXTIO_OFFSET_GET(1,Q0)
#define     TEMPER_CS    EXTIO_OFFSET_GET(1,Q1)
#define     RESERVE1   	 EXTIO_OFFSET_GET(1,Q2)
#define     RESERVE2   	 EXTIO_OFFSET_GET(1,Q3)
#define     RESERVE3   	 EXTIO_OFFSET_GET(1,Q4)
#define     AD8253_A0    EXTIO_OFFSET_GET(1,Q5)
#define     AD8253_A1    EXTIO_OFFSET_GET(1,Q6)
#define     CTR_IA   	 EXTIO_OFFSET_GET(1,Q7)

#define     CTR_IB   	 EXTIO_OFFSET_GET(2,Q0)
#define     EXT_V_CTR    EXTIO_OFFSET_GET(2,Q1)
#define     CTR_IC   	 EXTIO_OFFSET_GET(2,Q2)
#define     WR_EXT_V   	 EXTIO_OFFSET_GET(2,Q3)
#define     WR_IBZ   	 EXTIO_OFFSET_GET(2,Q4)
#define     WR_UA   	 EXTIO_OFFSET_GET(2,Q5)
#define     WR_UB  		 EXTIO_OFFSET_GET(2,Q6)
#define     WR_UC  		 EXTIO_OFFSET_GET(2,Q7)

#define     WR_IA   	 EXTIO_OFFSET_GET(3,Q0)
#define     WR_IB   	 EXTIO_OFFSET_GET(3,Q1)
#define     WR_IC   	 EXTIO_OFFSET_GET(3,Q2)
#define     AD_RESET   	 EXTIO_OFFSET_GET(3,Q3)
#define     AD_STBY   	 EXTIO_OFFSET_GET(3,Q4)
#define     AD_OS0   	 EXTIO_OFFSET_GET(3,Q5)
#define     AD_OS1  	 EXTIO_OFFSET_GET(3,Q6)
#define     AD_OS2  	 EXTIO_OFFSET_GET(3,Q7)

#define     IS_4015A   	 EXTIO_OFFSET_GET(4,Q0)
#define     IS_4015B   	 EXTIO_OFFSET_GET(4,Q1)
#define     IS_4015C   	 EXTIO_OFFSET_GET(4,Q2)
#define     ISELF_EN     EXTIO_OFFSET_GET(4,Q3)
#define     IASELF_EN    EXTIO_OFFSET_GET(4,Q4)
#define     IBSELF_EN    EXTIO_OFFSET_GET(4,Q5)
#define     ICSELF_EN  	 EXTIO_OFFSET_GET(4,Q6)
#define     ISWSELF_EN   EXTIO_OFFSET_GET(4,Q7)

#define     E2PROM_WP    EXTIO_OFFSET_GET(5,Q0)
#define     RESERVE51    EXTIO_OFFSET_GET(5,Q1)
#define     RESERVE52    EXTIO_OFFSET_GET(5,Q2)
#define     RESERVE53    EXTIO_OFFSET_GET(5,Q3)
#define     RESERVE54    EXTIO_OFFSET_GET(5,Q4)
#define     RESERVE55    EXTIO_OFFSET_GET(5,Q5)
#define     RESERVE56  	 EXTIO_OFFSET_GET(5,Q6)
#define     RESERVE57    EXTIO_OFFSET_GET(5,Q7)
/*********************************************************************
* OSDRIVER-H  for 	UIOUT
**********************************************************************/
typedef struct
{
  float f_U[3];
  float f_I[3];
  float f_J[3];
  float freq;
  float f_CRTI[3];
  int   JX_F;     //0 三相四线 1,三相三线,2 单相模式，
}UIout_rise_t;

typedef struct
{
  BOOL U_EN[3];
  BOOL I_EN[3];
}UIout_ctl_t;

typedef struct 
{
	uint16_t  DL_alerting:1;  
	uint16_t  KL_alerting:1;
	uint16_t  onlineU_alerting:1;
	uint16_t  reserve:13; 
}alerting_bit_t;

typedef union
{
      alerting_bit_t bits;
	  uint16_t all;
}UIout_alerting_u;

dev_id_t UIout_driver_register( void );
//宏定义命令
enum
{
	UIOUT_OPEN    	=  0,
	UIOUT_CLOSE    		,
	UIOUT_DLARG_SET    	,
	UIOUT_CTL_SET    	,
	UIOUT_PAUSE    		,
	UIOUT_REGAIN        ,
	UIOUT_STATE_GET     ,
	UIOUT_IS_UPDES      ,
	UIOUT_CMD_1OR3_SELECT,
	UIOUT_LAWFLAG_GET	 ,
	UIOUT_LAWFLAG_CLEAN	,
	UIOUT_JXMOD_SET
};


/*********************************************************************
* OSDRIVER-H    FOR     SH11
**********************************************************************/
enum
{
	SHT11_CMD_WSD_OBTAIN    =  0,
	SHT11_CMD_END
};

void sht11_measure( float *f_humi, float *f_temp, float *f_dewpt );

/*********************************************************************
* OSDRIVER-H 	 for 		GPS
**********************************************************************/
enum
{
	GPS_START,
	GPS_ANALYSIS_START,
	GPS_TIME_GET,
	GPS_END,
	GPS_CLOSE,
	GPS_OPEN,
	GPS_SET_BAUD,
	GPS_SET_CHECK,
	GPS_EMPTY_FIFO
};

dev_id_t GPS_driver_register( void );

/*typedef struct
{
	char year_buf[3];
	char month_buf[3];
	char day_buf[3];
	char hour_buf[3];
	char min_buf[3];
	char sec_buf[3];
	char msec_buf[4];
}GPS_Time_Date_t;
*/
void UTC_Time_convert_BJ_Time( GPS_Time_Date_t *m_GPS_Time_Date_buf );

#endif


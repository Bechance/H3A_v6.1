#include "../../../includes.h"
#ifndef _PEC_APP_WIRMANEGE_INSIDE_2010_H
#define _PEC_APP_WIRMANEGE_INSIDE_2010_H
/*********************************************************************
* APP_WIRMOD-H  for WIRMOD fun
**********************************************************************/
void 		app_WIFImod_YDWIR_entry( void );
void 		app_WIFImod_YDWIR_exit( void );
void 		app_WIFImod_fun_entry( void );
void 		app_WIFImod_fun_exit( void );
/*********************************************************************
* APP_WIRMOD-H  for WIRMOD alg
**********************************************************************/
enum
{
	PHASE_INVALID = 0,
	PHASE_ADD = 1,
	PHASE_SUB
};
typedef struct
{
	uint8_t is_lost_UI;
	uint8_t Ua_lost;
	uint8_t Ub_lost;
	uint8_t Uc_lost;
	uint8_t Ia_lost;
	uint8_t Ib_lost;
	uint8_t Ic_lost;
	uint8_t phaseDir;
	char c_cos[20];
	char c_U[20];
	char c_I[20];
	uint8_t jxmod;
}WIR_ALG_t;

typedef struct
{
   uint8_t head1;
   uint8_t head2;
   uint8_t len[2];
   uint8_t Ctl;
}Wifi_data_t;



extern WIR_ALG_t*			 m_alg_presult;
extern a_sem_t *    		 m_WIFImod_sem;
extern a_sem_t *    		 m_WIFImod_Resem;
extern measure_param_t       g_DL_param;

void 				app_WIFImod_alg_entery( void );
void				app_WIFImod_alg_exit( void );


void 				app_WIFImod_LED_task(void * arg);
/*********************************************************************
* APP_WIRMOD-H  for WIRMOD INTERFACE
**********************************************************************/
enum
{
	EDIT_U_A    = 1,
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


typedef struct
{
	char    cos[10];
	char    Result_MUa[3];
	char    Result_MUb[3];
	char    Result_MUc[3];
	char    Result_MIa[4];
	char    Result_MIb[4];
	char    Result_MIc[4];
	uint8_t phaseDir;
	uint8_t verdict;
	uint8_t jxmod;
	char    GX[10];
}WIRresult_t;

extern WIRresult_t  		 m_WIRresult[4];
enum
{
 	GUI_KEY_SW = 100,
	GUI_KEY_JXMOD
};


/*********************************************************************
* APP_WIRMOD-H  for WIRMOD  WIFI
**********************************************************************/
typedef   enum
{
      ONE_METER_ERR    = 0X10,
      DOUBLE_METER_ERR = 0X11,
      READ_METER       = 0X20,
      READ_WIR         = 0X30,	  
      READ_FFT         = 0X40,
      READ_WAVE        = 0X50,
      READ_CORRECT     = 0XD0,
      READ_BB          = 0x60,
}EPAD_code_t;

void 	Wifi_rev_task(void*arg);
void 	Wifi_init(void);

extern  dev_id_t        g_gpio_dev;
extern  dev_id_t        m_WIFImod_Wifi_dev ;
extern  EPAD_code_t     m_EPAD_code;
extern  dev_id_t        g_ADC_dev;
extern  BOOL            m_IS_BB_MOD;
/*********************************************************************
* APP_WIRMOD-H  for WIRMOD redeem
**********************************************************************/
extern dev_id_t 		g_DL_dev;

void   app_WIFImod_redeem_GX_obtian(WIRresult_t*p);
#endif

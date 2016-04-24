#ifndef _PEC_APP_MOD_INSIDE_2012_H
#define _PEC_APP_MOD_INSIDE_2012_H
/*****************************************************************************/
#define   UER_DSIZE             31
typedef struct
{
	char wrok_list[UER_DSIZE];	//������
	char user_id[UER_DSIZE];	//�û���
	char user_name[UER_DSIZE];	//�û���
	char meter_id1[UER_DSIZE];	//���1
	char meter_id2[UER_DSIZE];	//���2
	char worker[UER_DSIZE];		//У��Ա
	char time_buf[20];			//ʱ��
	char TQ_id[20];
	char data_order[12];		//���ݹ�����־
	char data_ext[4];			//�Ѵ���־
	char meter_dj[6];			//�ȼ�
	char temp_buf[10];			//�¶�
	char hnm_buf[10];			//ʪ��
}DL_app_user_info_t;	

typedef struct
{
	char dlsr; 			//ǯ��ѡ��
	char bb1[9];		//����	
	char bb2[9];		//CT���
	char cs[9];
	char cs2[9];
	char TMRcs[9];
	char qs[9];
	char qs2[9];
	char qs3[9];		//Ϊ�˲�Ӱ�����ݽṹһ����,��H3A�����洢�ռ�ʱ�����
	char handOR_GD;		//��ʽ
	char PorQ;			//=0�й�У�� 	=1�޹�У��
	char meter_1OR3;	//=1����λ   	=0����λ
	char ERRtype;		//=1�и���У�� 	=2�޸���У��     =3ʱ��У��
	char meterDJ;		//�ȼ�
	char meterDJ2[10];	//�ȼ�2,ʹ��meterDJ2[0],
	char pt[10];		//PT���
}DL_app_user_set_t;

typedef struct
{
	char temp_buf[10];	//�¶�
	char hnm_buf[10];	//ʪ��
}DL_app_spread_info_t;


typedef struct
{
	DL_app_user_info_t  DL_app_user_info;
	DL_app_user_set_t	DL_app_user_set;
}DL_app_user_all_info_t;


/*********************************������********************************************/
#define   DL_DSIZE  10

typedef struct
{
	char c_U[DL_DSIZE];
	char c_I[DL_DSIZE];
	char c_J[DL_DSIZE];
	char c_P[DL_DSIZE];
	char c_Q[DL_DSIZE];
}DL_app_phase_param_str_t;

typedef struct
{
	DL_app_phase_param_str_t phase_a;
	DL_app_phase_param_str_t phase_b;
	DL_app_phase_param_str_t phase_c;
	char c_JIac[DL_DSIZE];
	char c_JIbc[DL_DSIZE];
	char c_JIab[DL_DSIZE];
	char c_JUac[DL_DSIZE];
	char c_JUbc[DL_DSIZE];
	char c_JUab[DL_DSIZE];
	char c_Pabc[DL_DSIZE];
	char c_Qabc[DL_DSIZE];
	char c_err_1[4][DL_DSIZE];	//��1���
	char c_err_2[4][DL_DSIZE];	//��2���
	char c_Feq[DL_DSIZE];
	char c_Cos [DL_DSIZE];
	char cos_LC;
}DL_app_param_str_t;

typedef struct
{
	DL_app_user_info_t  DL_app_user_info;
	DL_app_user_set_t	DL_app_user_set;
	DL_app_param_str_t  DLarg;
}Errsave_t;


typedef struct
{	
	uint8_t guiyue;				//��Լ
	uint8_t baud;				//������
	uint8_t crc;				//У��λ
	char    meter_Addr[13];		//���ַ
	float 	zy_power[5];  		//�����й��ܼ��ƽ��
	float 	fy_power[5];  		//�����й��ܼ��ƽ��
	float 	voltage_A;			//A��ѹ
	float 	current_A;			//A����
	float   cosj;				//�ܹ�������
	float 	z_power;			//˲ʱ���й�����
	char  	meter_time[21];		//��ʱ��
}Meter_dl_info_t;

//У�����������ʾ�õ��Ľṹ��
typedef struct
{
	float 	 m_creep_U;			//Ǳ����ѹ
	float 	 m_creep_I;			//Ǳ������
	uint16_t m_creep_T;			//Ǳ��ʱ��
	uint8_t	 m_creep_YorN;		//1��ʾ����Ǳ������,0Ϊ��Ч
	uint8_t  m_creep_result[3]; //��1����2�ͱ�3Ǳ���������,1Ϊ�ϸ�,0Ϊ���ϸ�
	float 	 m_start_U;			//�𶯵�ѹ
	float 	 m_start_I;			//�𶯵���
	uint16_t m_start_T;			//��ʱ��
	uint8_t	 m_start_YorN;		//1��ʾ����������,0Ϊ��Ч
	uint8_t  m_start_result[3];	//��1����2�ͱ�3�����������,1Ϊ�ϸ�,0Ϊ���ϸ�
	uint32_t m_check_CS[3];		//�������黺�棬�ֱ�Ϊ��1����2�ͱ�3�����鳣��
	uint8_t  m_meter_1or3;		//0:����1:����
}result_CS_CS_t;

#define  POINT_SIZE 	35

typedef struct
{
	char    	m_FH_I[6];
	char    	m_FH_COS[6];
	char    	m_path1_err[6];
	char    	m_path2_err[6];
	char    	m_path3_err[6];
	char    	m_err1_result[7];
	char    	m_err2_result[7];
	char    	m_err3_result[7];
	uint8_t 	m_is_empty;		//1Ϊ��ǰ������Ч��0Ϊ��Ч
}err_content_t;

//�Զ�У���������ݽṹ
typedef struct
{
	result_CS_CS_t result_CS;
	err_content_t  err_content[POINT_SIZE];
	
}Auto_DL_app_param_t;

//�洢�������սṹ
typedef struct
{
	DL_app_user_info_t   user_arg_info;
	DL_app_user_set_t    user_set_info;
	DL_app_param_str_t   param_info;
	Auto_DL_app_param_t  Auto_param_info;//�Զ�У������
	Meter_dl_info_t      meter_info;
	DL_app_spread_info_t spread_info;
	
}DL_data_t;    							//�������ݽṹ��


typedef struct
{
	char worker0[21];
	char worker1[21];
	char worker2[21];
	char worker3[21];
	char worker4[21];
	char worker5[21];
	char worker6[21];
}User_worker_t;

/*********************************END*******************************************(*/

#endif

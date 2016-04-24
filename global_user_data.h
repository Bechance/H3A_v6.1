#ifndef _PEC_APP_MOD_INSIDE_2012_H
#define _PEC_APP_MOD_INSIDE_2012_H
/*****************************************************************************/
#define   UER_DSIZE             31
typedef struct
{
	char wrok_list[UER_DSIZE];	//工单号
	char user_id[UER_DSIZE];	//用户号
	char user_name[UER_DSIZE];	//用户名
	char meter_id1[UER_DSIZE];	//表号1
	char meter_id2[UER_DSIZE];	//表号2
	char worker[UER_DSIZE];		//校验员
	char time_buf[20];			//时间
	char TQ_id[20];
	char data_order[12];		//数据关联标志
	char data_ext[4];			//已传标志
	char meter_dj[6];			//等级
	char temp_buf[10];			//温度
	char hnm_buf[10];			//湿度
}DL_app_user_info_t;	

typedef struct
{
	char dlsr; 			//钳表选择
	char bb1[9];		//倍率	
	char bb2[9];		//CT变比
	char cs[9];
	char cs2[9];
	char TMRcs[9];
	char qs[9];
	char qs2[9];
	char qs3[9];		//为了不影响数据结构一致性,新H3A用来存储日计时误差结果
	char handOR_GD;		//方式
	char PorQ;			//=0有功校验 	=1无功校验
	char meter_1OR3;	//=1三表位   	=0单表位
	char ERRtype;		//=1有负荷校验 	=2无负荷校验     =3时钟校验
	char meterDJ;		//等级
	char meterDJ2[10];	//等级2,使用meterDJ2[0],
	char pt[10];		//PT变比
}DL_app_user_set_t;

typedef struct
{
	char temp_buf[10];	//温度
	char hnm_buf[10];	//湿度
}DL_app_spread_info_t;


typedef struct
{
	DL_app_user_info_t  DL_app_user_info;
	DL_app_user_set_t	DL_app_user_set;
}DL_app_user_all_info_t;


/*********************************主测量********************************************/
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
	char c_err_1[4][DL_DSIZE];	//表1误差
	char c_err_2[4][DL_DSIZE];	//表2误差
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
	uint8_t guiyue;				//规约
	uint8_t baud;				//波特率
	uint8_t crc;				//校验位
	char    meter_Addr[13];		//表地址
	float 	zy_power[5];  		//正向有功总尖峰平谷
	float 	fy_power[5];  		//反向有功总尖峰平谷
	float 	voltage_A;			//A电压
	float 	current_A;			//A电流
	float   cosj;				//总功率因数
	float 	z_power;			//瞬时总有功功率
	char  	meter_time[21];		//表时间
}Meter_dl_info_t;

//校验结果保存和显示用到的结构体
typedef struct
{
	float 	 m_creep_U;			//潜动电压
	float 	 m_creep_I;			//潜动电流
	uint16_t m_creep_T;			//潜动时间
	uint8_t	 m_creep_YorN;		//1表示做了潜动试验,0为无效
	uint8_t  m_creep_result[3]; //表1、表2和表3潜动结果缓存,1为合格,0为不合格
	float 	 m_start_U;			//起动电压
	float 	 m_start_I;			//起动电流
	uint16_t m_start_T;			//起动时间
	uint8_t	 m_start_YorN;		//1表示做了起动试验,0为无效
	uint8_t  m_start_result[3];	//表1、表2和表3启动结果缓存,1为合格,0为不合格
	uint32_t m_check_CS[3];		//常数试验缓存，分别为表1、表2和表3的试验常数
	uint8_t  m_meter_1or3;		//0:单表；1:三表
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
	uint8_t 	m_is_empty;		//1为当前内容有效，0为无效
}err_content_t;

//自动校验最终数据结构
typedef struct
{
	result_CS_CS_t result_CS;
	err_content_t  err_content[POINT_SIZE];
	
}Auto_DL_app_param_t;

//存储数据最终结构
typedef struct
{
	DL_app_user_info_t   user_arg_info;
	DL_app_user_set_t    user_set_info;
	DL_app_param_str_t   param_info;
	Auto_DL_app_param_t  Auto_param_info;//自动校验数据
	Meter_dl_info_t      meter_info;
	DL_app_spread_info_t spread_info;
	
}DL_data_t;    							//测量数据结构体


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

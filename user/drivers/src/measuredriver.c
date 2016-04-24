/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:系统电参数测量驱动
* 创建时间:2010-08-20
* 所属公司:科荟
* 文件名称:mesuredriver.c
* 创建人:揭成
* 功能描述: 提供在线测量参数的底层支持，为了
*                        获得更好的实时性能，把该部分放在
*                        了驱动中，应用了原程序中汇编部分。
***************************************************************/
#include "../Osdriver.h"
#include "measure\measure.h"
#include "measure\measure_connect.c"
#include "measure\measure_correct.c"
#include "measure\measure_count.c"
#include "measure\measure_CTScale.c"
#include "measure\measure_error.c"
#include "measure\measure_GetData.c"
#include "measure\fft\fft_R1024.c"



measure_param_t   		    m_measure_param = {0,};
measure_param_t   		    m_measure_Real  = {0,};
DL_flag_t                   m_measure_flag  = {0,};
a_lock_t *      		    m_measure_lock  = INVALID_LOCK;
DL_correct_step_t           m_measure_correct_step = {0, 0};
const char*g_dlsr_option[2][9] = {
		{"1A端子", "5A端子","1A钳表","5A钳表", "20A钳表", "100A钳", "500A钳",    "1000A钳",  "1500A钳"},
		{"1A端子", "5A端子","1A钳表","5A钳表", "20A钳表", "100A钳", "500A钳",    "1000A钳",  "1500A钳"}		
};
extern BOOL                        m_IAUTO_level_F;
/********************************************************************
 * @创建人:揭成
 * @功能:设置误差相关参数
 * @输入:NONE
 * @输出:NONE
********************************************************************/
static void measure_driver_set_err_ARG(DL_app_user_all_info_t*pUSER)
{
    uint32_t plus_const;
    plus_const = atol(pUSER->DL_app_user_info.data_order);
	if(plus_const%36==0&&plus_const!=0)
	{
        measue_DDSconst_mod(plus_const);
	}
	else
	{
        measue_DDSconst_mod(36000);
	}
    m_err_Sdata.path_cs[0] = atol(pUSER->DL_app_user_set.cs);
    m_err_Sdata.path_qs[0] = global_atoi(pUSER->DL_app_user_set.qs);
    m_err_Sdata.path_cs[1] = atol(pUSER->DL_app_user_set.cs2);
    m_err_Sdata.path_qs[1] = global_atoi(pUSER->DL_app_user_set.qs2);
    m_err_Sdata.path_cs[2] = atol(pUSER->DL_app_user_set.TMRcs);
    m_err_Sdata.path_qs[2] = global_atoi(pUSER->DL_app_user_set.qs3);
    if(pUSER->DL_app_user_set.dlsr>QB_N_20A)
    {
        m_err_Sdata.f_path_BB[0] =1.0/global_atof(pUSER->DL_app_user_set.bb1);
	}
    else
    {
       m_err_Sdata.f_path_BB[0] =1.0;
	}
	m_DDS_path_mod.P_or_Q = pUSER->DL_app_user_set.PorQ;
	if(m_DDS_path_mod.single_or_double==0)
	{
		m_DDS_path_mod.hand_or_auto = pUSER->DL_app_user_set.handOR_GD;
		if(m_DDS_path_mod.hand_or_auto==0)
		{
			BUS_WRITE(m_err_Sdata.path_qs[0],DDS_PATH1_SET_CIRCLE);
			BUS_WRITE(m_err_Sdata.path_qs[1],DDS_PATH2_SET_CIRCLE);
			BUS_WRITE(m_err_Sdata.path_qs[2],DDS_PATH3_SET_CIRCLE);
		}
		else 
		{
			BUS_WRITE(1,DDS_PATH1_SET_CIRCLE);
			BUS_WRITE(1,DDS_PATH2_SET_CIRCLE);
		}
	}
	else
	{
	    m_DDS_path_mod.hand_or_auto = 0;
		BUS_WRITE(m_err_Sdata.path_qs[0],DDS_PATH1_SET_CIRCLE);
		BUS_WRITE(m_err_Sdata.path_qs[1],DDS_PATH2_SET_CIRCLE);
		BUS_WRITE(m_err_Sdata.path_qs[2],DDS_PATH3_SET_CIRCLE);
	}
    return;
}
/********************************************************************
 * @创建人:揭成
 * @功能:用户参数设置
 * @输入:NONE
 * @输出:NONE
********************************************************************/
void measure_driver_set_user_DL_arg(DL_app_user_all_info_t*pUSER)
{
    m_correct_level_arg.ADLSR_select=(DLSR_CT_t)pUSER->DL_app_user_set.dlsr;
    m_correct_level_arg.BDLSR_select=(DLSR_CT_t)pUSER->DL_app_user_set.dlsr;
    m_correct_level_arg.CDLSR_select=(DLSR_CT_t)pUSER->DL_app_user_set.dlsr;
	m_DDS_path_mod.single_or_double = pUSER->DL_app_user_set.meter_1OR3;
    measure_driver_set_err_ARG(pUSER);
    if(m_correct_level_arg.ADLSR_select==DZ_N_1A
		||m_correct_level_arg.ADLSR_select==QB_N_1A)
    {
         m_correct_level_arg.Ilevel_offset = I_INPUT_10;
	}
	else 
    {
         m_correct_level_arg.Ilevel_offset = I_INPUT_1;
	}	
	measure_CTScale_select_I_level(&m_correct_level_arg);/*设置电流档位*/
	measure_CTScale_select_U_level(&m_correct_level_arg);/*设置电压档位*/
	measure_correct_K_renew(&m_correct_level_arg);   	 /*更新误差*/
    return;
}
/********************************************************************
 * @创建人:揭成
 * @功能:驱动底层读入口
 * @输入:NONE
 * @输出:NONE
********************************************************************/
static Bsize_t measure_driver_read( uint8_t*buf, Bsize_t size, void*pos )
{
	return size;
}
/********************************************************************
 * @创建人:揭成
 * @功能:驱动模块控制
 * @输入:NONE
 * @输出:NONE
********************************************************************/
static void  measure_driver_ioctl( uint8_t cmd, void*arg )
{
    int16_t**pdata;
	uint8_t* updata;
	os_lock( m_measure_lock );
	switch ( cmd )
	{
	case MEASURE_CMD_START:
		measure_driver_start();
		break;
	case MEASURE_CMD_STOP:
		measure_driver_stop();
		break;
	case MEASURE_CMD_DLFLAG_GET:
		global_memcpy(arg, &m_measure_flag,sizeof(DL_flag_t));
		break;
	case MEASURE_CMD_XBDATA_GET:
		m_measure_flag.DL_XB_is_new = FAIL;
		global_memcpy(arg,F_Harm,(sizeof(float)*6*52));
		break;
	case MEASURE_CMD_USER_QB_SELECT:
		updata = (uint8_t*)arg;
		m_correct_level_arg.ADLSR_select = (DLSR_CT_t)updata[0];
		m_correct_level_arg.BDLSR_select = (DLSR_CT_t)updata[1];
		m_correct_level_arg.CDLSR_select = (DLSR_CT_t)updata[2];
		measure_CTScale_select_I_level(&m_correct_level_arg);
		measure_correct_K_renew(&m_correct_level_arg);  
		break;
	case MEASURE_CMD_DL_PARAM_GET:
		m_measure_flag.DL_data_is_new       = FAIL;
		global_memcpy(arg, &m_measure_param,sizeof(measure_param_t));
		break;
	case MEASURE_CMD_PATH1_ERR_GET:
		m_measure_flag.DL_path1_ERR_is_new  = FAIL;
		global_memcpy(arg, &m_measure_param,sizeof(measure_param_t));
		break;
	case MEASURE_CMD_PATH2_ERR_GET:
		m_measure_flag.DL_path2_ERR_is_new  = FAIL;
		global_memcpy(arg, &m_measure_param,sizeof(measure_param_t));
		break;
	case MEASURE_CMD_PATH3_ERR_GET:
		m_measure_flag.DL_path3_ERR_is_new  = FAIL;
		global_memcpy(arg, &m_measure_param,sizeof(measure_param_t));
		break;
	case MEASURE_CMD_PATH1_ERR_CIRCLE_GET:
		*((uint16_t*)arg)=BUS_READ(GD_PATH1_SUB_CIRCLE)+1;
		break;
	case MEASURE_CMD_PATH2_ERR_CIRCLE_GET:
		*((uint16_t*)arg)=BUS_READ(GD_PATH2_SUB_CIRCLE)+1;
		break;
	case MEASURE_CMD_PATH3_ERR_CIRCLE_GET:
		*((uint16_t*)arg)=BUS_READ(GD_PATH3_SUB_CIRCLE)+1;
		break;
	case MEASURE_CMD_USER_DL_ARG_SET:
		measure_driver_set_user_DL_arg((DL_app_user_all_info_t*)arg);
		break;
	case MEASURE_CMD_CORRECT_STEP_SET:
		global_memcpy(&m_measure_correct_step,
			arg,sizeof( DL_correct_step_t ) );
		break;
	case MEASURE_CMD_CELAN_ALL_CORRECT_DATA:
		measure_correct_clean_all();
		break;
	case MEASURE_CMD_CELAN_CUR_CORRECT_DATA:
		measure_correct_clean_data(&m_measure_correct_step);
	    break;
	case MEASURE_CMD_GET_CORRECT_DATA:
		*((correct_data_t**)arg)=&m_err_correct_data;
		break;
	case MEASURE_CMD_GET_DDSN:
		if(m_dds_constBL != 0)
		{
			m_path_count_err_value[0] /= m_dds_constBL;
			m_path_count_err_value[1] /= m_dds_constBL;
			m_path_count_err_value[2] /= m_dds_constBL;
		}
		global_memcpy(arg, m_path_count_err_value,sizeof(float)*3);
		break;
	case MEASURE_CMD_GET_CORRECT_FILE:
		*((correct_save_data_t**)arg)=&m_save_correct_data;
		break;
	case MEASURE_CMD_SET_UDL_LEVEL:
		m_correct_level_arg.Ualevel_offset=*((Bsize_t*)arg);
		m_correct_level_arg.Ublevel_offset=*((Bsize_t*)arg);
		m_correct_level_arg.Uclevel_offset=*((Bsize_t*)arg);
		measure_CTScale_select_U_level(&m_correct_level_arg);/*设置电压档位*/
		measure_correct_K_renew(&m_correct_level_arg);   	 /*更新误差*/
	    break;
	case MEASURE_CMD_GET_AD_DATA:
	    pdata    = (int16_t**)arg;
		pdata[0] = m_Ua_buf;pdata[2] = m_Ub_buf;pdata[4] = m_Uc_buf;
		pdata[1] = m_Ia_buf;pdata[3] = m_Ib_buf;pdata[5] = m_Ic_buf;
		break;
	case MEAUSRE_CMD_REAL_BUF_GET:
		m_measure_flag.DL_data_is_new = FAIL;
		global_memcpy(arg,&m_measure_Real,sizeof(measure_param_t));
		break;
    case MEASURE_CMD_USER_SELECT_ERRWAY:
		measure_driver_ERR_way(*((int*)arg));
		break;
    case MEASURE_CMD_USER_LOAD_ZSSERR:
		measure_driver_ZSSERR_load(*((float*)arg));
		break;
	case MEASURE_CMD_CORRECT_COPY:
		measure_driver_correct_copy();
	    break;
	case MEASURE_CMD_IAUTO_LEVEL_STOP:
		m_IAUTO_level_F = FAIL;
		break;
	case MEASURE_CMD_GET_REAL_DYSR:
		*((Bsize_t*)arg)=m_correct_level_arg.Uclevel_offset;
		break;
	case MEASURE_CMD_GET_REAL_DLSR:
		*((Bsize_t*)arg)=m_correct_level_arg.ADLSR_select;
		break;
	case MEASURE_CMD_ICONST_CHANGE:
		measue_DDSconst_mod(*((uint32_t*)arg));
		break;
	default:
		break;
	}
	os_unlock( m_measure_lock );
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 驱动底层写入口
 * @输入:	NONE
 * @输出:	NONE
********************************************************************/
static Bsize_t measure_driver_write( uint8_t*buf, Bsize_t size , void*pos )
{
	return size;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 驱动卸载
 * @输入: NONE
 * @输出: NONE
********************************************************************/
static void measure_driver_uninstall( void )
{
	measure_exit();
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 驱动初始化
 * @输入: NONE
 * @输出: NONE
********************************************************************/
static void measure_driver_init( void )
{
    GLOBAL_MEMCLR(&m_measure_correct_step,sizeof(DL_correct_step_t));
	GLOBAL_MEMCLR(&m_DDS_path_mod,sizeof(path_select_t));
	GLOBAL_MEMCLR(&m_measure_flag,sizeof(DL_flag_t));
	GLOBAL_MEMCLR(F_Harm,(sizeof(float)*6*52));
	os_lock_creat(&m_measure_lock);
	m_measure_flag.Ia_is_lost = 1;
	m_measure_flag.Ib_is_lost = 1;
	m_measure_flag.Ic_is_lost = 1;
	m_measure_flag.Ua_is_lost = 1;
	m_measure_flag.Ub_is_lost = 1;
	m_measure_flag.Uc_is_lost = 1;
	m_DDS_path_mod.single_or_double = 0;
	m_measure_correct_step.main_step=10;
    GLOBAL_MEMCLR(&m_measure_param,sizeof(measure_param_t));
	measure_entery();
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能:驱动注册
 * @输入:none
 * @输出:none
********************************************************************/
dev_id_t measure_driver_register( void )
{
	uint8_t ret;
	os_dirver_operate_t temp;
	temp.os_driver_init  = measure_driver_init;
	temp.os_driver_ioctl = measure_driver_ioctl;
	temp.os_driver_read  = measure_driver_read;
	temp.os_driver_write = measure_driver_write;
	temp.os_driver_uninstall = measure_driver_uninstall;
	ret = os_driver_add( &temp, "Measure Driver" );
	return ret;
}


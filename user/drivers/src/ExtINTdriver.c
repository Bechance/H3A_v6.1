/******************************Copyright (c)***************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:28335 pic 驱动
* 完成时间2011-11-28
* 所属公司 :科荟
* 文件名称:Pic32driver.c
* 创建人 :揭成
* 功能描述:实现pic32总线读写操作
*----------------------------------------------------------------
修改记录:

修改日期:

修改人:

*********************************************************************************************************
*/
#include "../Osdriver.h"
/*---------------------------------------------------------------*/
#define     	  ISR_REG_ADDR     0x4110
#define     	  ISR_PIC_ADDR     0x4100
/*---------------------------------------------------------------*/
typedef enum
{
   IS_KEY_DATA = 0,   		//按键数据传输   	0
   IS_WSD_DATA,      	 	//温湿度数据传输 	1
   IS_SMQ_CONNECT,    		//扫描抢连接     	2
   IS_USB_WRITE_FINISH,     //写数据是否完成 	3
   IS_SMQ_DATA,       		//扫描数据传输   	4 
   IS_USB_CONNECT,    		//USB连接   	 	5
   IS_USB_MOVE,       		//USB断开        	6
   IS_USB_DATA,       		//USB数据传输    	7
   IS_GPS_END,				//GPS数据传输结束	8
   IS_GPS_DATA,  			//GPS数据传输	 	9
   IS_PC_DATA,				//PC串口数据传输 	10
   IS_SMQ_MOVE,				//扫描抢断开	 	11
   IS_GPS_LOST,				//获取GPS信号失败	12
   IS_POWER_STATE,			//电源供电状态传输  13
   IS_BATTERY_N,			//电池电压显示个数  14
   IS_VOID					//(预留)			15
}ISR_REG_u;

#define           PIC_READ_UNLOCK()  	(PieCtrlRegs.PIEIER1.bit.INTx5= 1)
#define           PIC_READ_LOCK()  		(PieCtrlRegs.PIEIER1.bit.INTx5= 0)
float             g_pic_wd = 25.00;  	//温度
float             g_pic_sd = 50.00;  	//湿度
uint8_t			  g_GPS_data_end = 0;	//全局的GPS数据获取完成标志
uint8_t			  g_GPS_data_lost = 0;	//全局的GPS数据失败标志
uint8_t			  g_power_state = 0;	//全局的电源状态标志
uint8_t			  g_cur_power_up = 0;	//全局的当前供电模式标志,0为自动供电,1为强制电池供电
uint8_t			  g_battery_num = 0;	//全局的电池显示格子个数
uint8_t			  g_battery_charge = 0;	//全局的电池充电标志,1为正在充电,0为无充电
uint8_t           m_pic_buf[4];
BOOL              m_TMR_int[3]={0,0,0};
BOOL              m_write_is_finish;
void 			  key_read_isr( uint8_t key);
void 			  USBFS_data_ISR(uint8_t*data);
void 			  USBhid_data_ISR(uint8_t*data);
void 			  GPS_data_ISR(uint8_t*data);
void 			  USB_disk_dev_connect(void);
void 			  USB_HID_dev_connect(void);
void 			  USB_dev_move(void);
GPS_Time_Date_t	  g_GPS_buf = { 0, };
void 			  measure_error_path1_dx_counter_obtain(void);
void 			  measure_error_path2_dx_counter_obtain(void);
void 			  measure_error_path3_dx_counter_obtain(void);

void   	pic32_driver_WSD_obtain( void )
{
	g_pic_wd = m_pic_buf[0];
	g_pic_wd+=((float)m_pic_buf[1]/100.0);
	g_pic_sd = m_pic_buf[2];
	g_pic_sd+=((float)m_pic_buf[3]/100.0);
	return;
}

/********************************************************************
 * @创建人:周炳权
 * @功能:电源状态获取
 * @输入:NONE
 * @输出:NONE
********************************************************************/
void pic32_power_state_obtain( void )
{
	g_power_state = m_pic_buf[0];
	g_cur_power_up = m_pic_buf[1];
	return;
}

/********************************************************************
 * @创建人:周炳权
 * @功能:电池显示状态获取
 * @输入:NONE
 * @输出:NONE
********************************************************************/
void pic32_battery_display_obtain( void )
{
	g_battery_num = m_pic_buf[0];
	if(g_battery_num>5)g_battery_num=5;
	g_battery_charge = m_pic_buf[1];
	return;
}

/********************************************************************
 * @创建人:周炳权
 * @功能:GPS时间获取
 * @输入:NONE
 * @输出:NONE
********************************************************************/
extern	fifo_tcb_t* m_GPS_fifo_tcb;
void	pic32_driver_GPS_data_obtain( void )
{
	g_GPS_data_end = 1;
	global_memcpy(&g_GPS_buf, m_GPS_rev_fifo, sizeof(GPS_Time_Date_t));
	fifo_buffer_empty(m_GPS_fifo_tcb);
	return;
}

/********************************************************************
 * @创建人:揭成
 * @功能:外部中断扩展
 * @输入:
 * @输出:
********************************************************************/
#define  READ_PIC_BUS_DATA(){\
    m_pic_buf[0]=(BUS_READ(ISR_PIC_ADDR))&0xff;\
    m_pic_buf[1]=(BUS_READ(ISR_PIC_ADDR+1))&0xff;\
    m_pic_buf[2]=(BUS_READ(ISR_PIC_ADDR+2))&0xff;\
    m_pic_buf[3]=(BUS_READ(ISR_PIC_ADDR+3))&0xff;}

#pragma CODE_SECTION(ISR_Extint_entery,"ramfuncs")
#pragma CODE_SECTION(measure_error_path2_dx_counter_obtain,"ramfuncs")
#pragma CODE_SECTION(measure_error_path1_dx_counter_obtain,"ramfuncs")
interrupt void ISR_Extint_entery( void )
{
	ISR_REG_u reg;
	uint16_t  EXTflag_buf;
	EXTflag_buf =(BUS_READ(ISR_REG_ADDR));
	READ_PIC_BUS_DATA();
	if(EXTflag_buf&0x0400)
	{
	    reg =(ISR_REG_u)(EXTflag_buf&0x000f);//0x0007改为0x000f(zbq:3-14修改)
		if(reg==IS_KEY_DATA)		key_read_isr(m_pic_buf[0]);
		else if(reg==IS_PC_DATA)	serialD_data_ISR(&(m_pic_buf[0]));
		else if(reg==IS_SMQ_CONNECT)USB_HID_dev_connect();
		else if(reg==IS_USB_CONNECT)USB_disk_dev_connect();
		else if(reg==IS_USB_MOVE)   USB_dev_move();
		else if(reg==IS_USB_DATA)   USBFS_data_ISR(m_pic_buf);
		else if(reg==IS_SMQ_DATA)   USBhid_data_ISR(m_pic_buf);
		else if(reg==IS_USB_WRITE_FINISH)m_write_is_finish = TURE;
		else if(reg==IS_WSD_DATA)   pic32_driver_WSD_obtain();
		else if(reg==IS_GPS_DATA)   GPS_data_ISR(m_pic_buf);
		else if(reg==IS_GPS_END)    pic32_driver_GPS_data_obtain();
		else if(reg==IS_GPS_LOST)   g_GPS_data_lost = 1;
		else if(reg==IS_POWER_STATE)pic32_power_state_obtain();
		else if(reg==IS_BATTERY_N)  pic32_battery_display_obtain();
	}
	else if(EXTflag_buf&0x0100)
	{
	     m_TMR_int[0] = 1;
         measure_error_path1_dx_counter_obtain();
	}
	else if(EXTflag_buf&0x0200)
	{
		 m_TMR_int[1] = 1;
         measure_error_path2_dx_counter_obtain();
	}
	else if(EXTflag_buf&0x0800)
	{
		 m_TMR_int[2] = 1;
	     measure_error_path3_dx_counter_obtain();
	}
	PieCtrlRegs.PIEACK.all = 0xFFFF;
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能:驱动底层写入口
 * @输入:buf 写入缓存，size 要写数据的大小
 * @输出:实际写进的大小
********************************************************************/
static Bsize_t Extint_driver_write( uint8_t*buf, Bsize_t size ,void*pos )
{
	return 0;
}
/********************************************************************
 * @创建人:揭成
 * @功能:驱动底层读入口
 * @输入:buf 读入缓存      size 要读数据的大小
 * @输出:实际读到的大小
********************************************************************/
static Bsize_t ExtINT_driver_read( uint8_t*buf, Bsize_t size , void*pos )
{	
	return 0;
}
/********************************************************************
 * @创建人:揭成
 * @功能:驱动控制命令
 * @输入:cmd 控制命令  arg 控制参数
 * @输出:NONE
********************************************************************/
static void  ExtINT_driver_ioctl( uint8_t cmd, void*arg )
{
#define  GET_TMR_FLAG      0
#define  GET_TMR1_ERR      1
#define  GET_TMR2_ERR      2
#define  GET_TMR3_ERR      3
#define  TMR1_ERR_REG	  (0x42D0)
#define  TMR2_ERR_REG	  (0x42D2)
#define  TMR3_ERR_REG	  (0x42D4)
#define  READ_TIMR_ERR(ADDR) (BUS_READ(ADDR)+(BUS_READ(ADDR+1))*0x10000)
uint32_t err_reg;
	switch ( cmd )
	{
	case GET_TMR_FLAG:
		global_memcpy(arg,m_TMR_int,sizeof(BOOL)*3);
		break;
	case GET_TMR1_ERR:
		 err_reg    =  READ_TIMR_ERR(TMR1_ERR_REG);
		*((uint32_t*)arg) = err_reg;
		m_TMR_int[0] 	=  0;
		break;
	case GET_TMR2_ERR:
		 err_reg    =  READ_TIMR_ERR(TMR2_ERR_REG);
		*((uint32_t*)arg) = err_reg;
		m_TMR_int[1] 	=  0;
		break;
	case GET_TMR3_ERR:
		 err_reg    =  READ_TIMR_ERR(TMR3_ERR_REG);
		*((uint32_t*)arg) = err_reg;
		m_TMR_int[2] 	=  0;
		break;
	default:
		break;
	}
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能:驱动卸载
 * @输入:NONE
 * @输出:NONE
********************************************************************/
static void Extint_driver_uninstall(void)
{
	return;
}

void _Ext_ISR_init(void)
{
	EALLOW;
	GpioCtrlRegs.GPAMUX1.bit.GPIO1  = 0;
	GpioCtrlRegs.GPAMUX1.bit.GPIO2  = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO1   = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO2   = 0;
	GpioCtrlRegs.GPAQSEL1.bit.GPIO1 = 0;
	GpioCtrlRegs.GPAQSEL1.bit.GPIO2 = 0;
	GpioCtrlRegs.GPBCTRL.bit.QUALPRD0 = 0xFF;
	GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 2;
	GpioIntRegs.GPIOXINT2SEL.bit.GPIOSEL = 1;
	XIntruptRegs.XINT1CR.bit.POLARITY = 0;
	XIntruptRegs.XINT2CR.bit.POLARITY = 0;
	XIntruptRegs.XINT1CR.bit.ENABLE = 1;
	XIntruptRegs.XINT2CR.bit.ENABLE = 1;
	EDIS;
	EALLOW;  
   	PieVectTable.XINT2 = &ISR_Extint_entery;
   	EDIS;   
   	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          
	PieCtrlRegs.PIEIER1.bit.INTx5 = 1;         
   	IER |= M_INT1;                          
    EINT; 
    ERTM;   
	return;
}

/********************************************************************
 * @创建人:揭成
 * @功能:测量模块硬件设置
 * @输入:	
 * @输出:NONE
********************************************************************/
static void ExtINT_driver_init(void)
{
	GLOBAL_MEMCLR(&g_GPS_buf, sizeof(GPS_Time_Date_t));
    _Ext_ISR_init();
    return;
}
/********************************************************************
 * @创建人:揭成
 * @功能:驱动注册
 * @输入:none
 * @输出:NONE
********************************************************************/
dev_id_t pic_driver_register( void )
{
	uint8_t ret;
	os_dirver_operate_t temp;
	temp.os_driver_init   		= ExtINT_driver_init;
	temp.os_driver_ioctl 		= ExtINT_driver_ioctl;
	temp.os_driver_read   		= ExtINT_driver_read;
	temp.os_driver_write 		= Extint_driver_write;
	temp.os_driver_uninstall 	= Extint_driver_uninstall;
	ret = os_driver_add( &temp, "EXTINT" );
	return ret;
}

#pragma DATA_SECTION (INText_list,"DR_LIST_ADDR")
const DR_member_t INText_list[]=
{
	pic_driver_register,
	0x5a5a
};

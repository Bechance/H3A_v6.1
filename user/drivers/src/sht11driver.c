/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:SHT11传感器驱动
* 创建时间:2011-11-9
* 所属公司 :科荟
* 文件名称:sht11_driver.c
* 作者 :周炳权
* 功能描述:SHT11传感器驱动支持，提供SH11测量读取功能
*----------------------------------------------------------------
修改记录:

修改目的:

修改日期:

修改人:
***************************************************************************************************/
#include "../osdriver.h"

typedef union  
{
  uint32_t i; 
  float f; 
} sht11_value; 
  
enum 
{
	TEMP = 0,
	HUMI
}; 

#define SHT11_noACK 0 
#define SHT11_ACK   1 
                            //adr  command  r/w 
#define STATUS_REG_W 0x06   //000   0011    0 
#define STATUS_REG_R 0x07   //000   0011    1 
#define MEASURE_TEMP 0x03   //000   0001    1 
#define MEASURE_HUMI 0x05   //000   0010    1 
#define SHT_RESET    0x1e   //000   1111    0 

#define     SET_DATA_INPUT()	{EALLOW;GpioCtrlRegs.GPAMUX2.bit.GPIO26=0;GpioCtrlRegs.GPADIR.bit.GPIO26=0;EDIS;}
#define     SET_DATA_OUTPUT()	{EALLOW;GpioCtrlRegs.GPAMUX2.bit.GPIO26=0;GpioCtrlRegs.GPADIR.bit.GPIO26=1;EDIS;}
#define 	SET_SCK_OUTPUT()	{EALLOW;GpioCtrlRegs.GPAMUX2.bit.GPIO27=0;GpioCtrlRegs.GPADIR.bit.GPIO27=1;EDIS;}
#define 	SET_DATA_H()		{GpioDataRegs.GPASET.bit.GPIO26 = 1;}
#define 	SET_DATA_L()		{GpioDataRegs.GPACLEAR.bit.GPIO26 = 1;}
#define 	SET_SCK_H()			{GpioDataRegs.GPASET.bit.GPIO27 = 1;}
#define 	SET_SCK_L()			{GpioDataRegs.GPACLEAR.bit.GPIO27 = 1;}
#define		READ_DATA			GpioDataRegs.GPADAT.bit.GPIO26
#define 	DATA_OUT(s_val) 	{GpioDataRegs.GPASET.bit.GPIO26 = s_val;}

void		DSP28x_usDelay( Uint32 Count );
uint16_t m_offset_cnt = 0;
//uint16_t m_offset_x = 0;
//uint16_t m_offset_y = 0;
#pragma CODE_SECTION(us_delay,"ramfuncs") 		//将该段代码限制在ramfuncs。
#pragma CODE_SECTION(DSP28x_usDelay,"ramfuncs") //将该段代码限制在ramfuncs。
void us_delay( uint16_t x )	/* 1μs,150	MHz	*/
{
	//m_offset_cnt = (uint16_t)(x * 7.5);
	m_offset_cnt = (uint16_t)(x * 45);
	/*for ( m_offset_x = 0; m_offset_x < 282; m_offset_x++ )
	{
		for ( m_offset_y = 0; m_offset_y < m_offset_cnt; m_offset_y++ )
		{
			;//asm( "nop" );
			;//asm( "nop" );
			;//asm( "nop" );
			;//asm( "nop" );
		}
	}*/
	DSP28x_usDelay(m_offset_cnt);
	return;
}

float g_f_humi 	= 0;
float g_f_temp 	= 0;
float g_f_dewpt = 0;

/********************************************************************
 * @作者:周炳权
 * @功能: 写一字节数据和校验应答
 * @输入:	value:要发送的数据
 * @输出:应答check
********************************************************************/
char sht11_write_byte(unsigned char value) 
{  
  unsigned char i,error=0;
  SET_DATA_OUTPUT();		/*数据输出*/
  for(i=0x80;i>0;i/=2)     /*移位标识*/ 
  {  
    if(i & value)
    {
		SET_DATA_H();       /*相与i，写入SENSI总线*/ 
    }
    else 
	{
		SET_DATA_L(); 
    }
	us_delay(2);
    SET_SCK_H();            /*SENSI总线时钟*/
    us_delay(5);            /*延时大约5us*/           
    SET_SCK_L();        
  }       
  SET_DATA_L();             /*释放数据线*/             
  SET_DATA_INPUT();			/*数据输入*/
  SET_SCK_H();              /*第九个时钟为应答信号*/  
  us_delay(2); 
  error = READ_DATA;      /*检查应答，数据线会被SHT11拉低*/ 
  us_delay(2); 
  SET_SCK_L();         
  return error;             /*若error = 1，则无应答*/
}
/********************************************************************
 * @作者:周炳权
 * @功能: 读一字节数据和作出应答"ACK = 1"
 * @输入:应答位
 * @输出:读到的数据
********************************************************************/
char sht11_read_byte(unsigned char ack) 
{  
  unsigned char i,val=0;
  SET_DATA_INPUT();			/*数据输入*/ 
  for(i=0x80;i>0;i/=2)              
  { 
  	SET_SCK_H();                                  
    us_delay(2); 
    if(READ_DATA) val=(val | i); /*读位*/  
    SET_SCK_L();                                            
    us_delay(2); 
  }    
  SET_DATA_OUTPUT();	/*数据输出*/ 
  DATA_OUT(!ack);        /*"ack == 1"则拉低数据线*/ 
  SET_SCK_H();          /*第九个时钟为应答信号*/  
  us_delay(5);           
  SET_SCK_L();
  SET_DATA_INPUT();		/*数据输入*/ 
  return val; 
} 
  
/********************************************************************
 * @作者:周炳权
 * @功能:启动传输
 * @输入:NONE
 * @输出:NONE
********************************************************************/
void s_trans_start(void) 
{           
  SET_DATA_OUTPUT();	/*数据输出*/ 
  SET_DATA_H();                           
  SET_SCK_L();     		/*初始化状态*/                                                                     
  us_delay(2); 
  SET_SCK_H(); 
  us_delay(2); 
  SET_DATA_L();                           
  us_delay(2); 
  SET_SCK_L();   
  us_delay(5); 
  SET_SCK_H(); 
  us_delay(2); 
  SET_DATA_H();                           
  us_delay(2); 
  SET_SCK_L();
  us_delay(2);
  SET_DATA_INPUT();		/*数据输入*/ 
}                                        

/********************************************************************
 * @作者:周炳权
 * @功能: 通讯复位
 * @输入:NONE
 * @输出:NONE
********************************************************************/
void s_connection_reset(void) 
{   
  unsigned char i;  
  SET_DATA_OUTPUT();		/*数据输出*/ 
  SET_DATA_H();                           
  SET_SCK_L();       		/*初始化状态*/
  for(i=0;i<9;i++)          /*9个时钟周期*/ 
  { 
  	SET_SCK_H();  
    us_delay(1); 
    SET_SCK_L(); 
    us_delay(1); 
  } 
  s_trans_start();       /*传输开始*/
  SET_DATA_INPUT();		/*数据输入*/ 
} 
 
/********************************************************************
 * @作者:周炳权
 * @功能: 软件复位
 * @输入:NONE
 * @输出:应答check
********************************************************************/
char s_soft_reset(void) 
{  
  unsigned char error=0;   
  s_connection_reset();              /*复位通讯*/
  error+=sht11_write_byte(SHT_RESET);    /*发送命令给传感器*/ 
  return error;                     /*若error = 1,则传感器没有应答*/ 
} 

/********************************************************************
 * @作者:周炳权
 * @功能: 读状态寄存器和校验和
 * @输入:p_value:读到数据缓存；p_checksum:读到校验和缓存
 * @输出:应答check
********************************************************************/
char s_read_statusreg(unsigned char *p_value, unsigned char *p_checksum) 
{  
  unsigned char error=0; 
  s_trans_start();                   	 /*传输开始*/ 
  error=sht11_write_byte(STATUS_REG_R);  /*发送命令给传感器*/ 
  *p_value=sht11_read_byte(SHT11_noACK);      /*读状态寄存器(8位)*/ 
  *p_checksum=sht11_read_byte(SHT11_ACK);    /*读校验和(8位)*/   
  return error;                        /*若error = 1,则传感器没有应答*/ 
}                           

/********************************************************************
 * @作者:周炳权
 * @功能: 写状态寄存器和校验和
 * @输入:p_value:读到数据缓存；
 * @输出:应答check
********************************************************************/
char s_write_statusreg(unsigned char *p_value) 
{  
  unsigned char error=0; 
  s_trans_start();                   /*传输开始*/ 
  error+=sht11_write_byte(STATUS_REG_W);/*发送命令给传感器*/ 
  error+=sht11_write_byte(*p_value);    /*发送参数给传感器*/ 
  return error;                     /*若error >= 1,则传感器没有应答*/ 
} 

/********************************************************************
 * @作者:周炳权
 * @功能: 温度/湿度的测量和校验
 * @输入:p_value:数据缓存；p_checksum:校验和缓存；mode:测量TEMP或HUMI
 * @输出:应答check
********************************************************************/
char s_measure(uint16_t *p_value, unsigned char *p_checksum, unsigned char mode) 
{  
  unsigned char error=0;
  unsigned char value_M;
  unsigned char value_L;
  uint16_t value_a;
  uint32_t timeout = 65535;

  s_trans_start();          /*传输开始*/     
  
  switch(mode)				/*发送命令给传感器*/
  {             
    case TEMP: error+=sht11_write_byte(MEASURE_TEMP); break; 
    case HUMI: error+=sht11_write_byte(MEASURE_HUMI); break; 
    default: break;          
  }
  
  SET_DATA_INPUT();		/*数据输入*/

  while(1) 
  { 
   	if(READ_DATA == 0) 
		break; /*等待传感器完成测量*/ 
	else if( timeout == 0) 
		break; /*超时退出*/
	timeout--;
	us_delay(3);
  } 

  if(READ_DATA != 0)
  {
	error+=1;    /*或超时达到1~2秒*/
  }
                  
	value_M = sht11_read_byte(SHT11_ACK);    //read the first byte (MSB)
	value_L = sht11_read_byte(SHT11_ACK);    //read the second byte (LSB)
	value_a = ((value_M << 8) | value_L);
  *p_value = value_a;
  *p_checksum =sht11_read_byte(SHT11_noACK);  //read checksum      
  return error; 
}

/********************************************************************
 * @作者:周炳权
 * @功能: 计算温度(℃)/湿度(%RH)
 * @输入:p_humidity:12位的湿度数据；p_temperature:14位的温度数据；
 * @输出:NONE
********************************************************************/
  const float C1=-4.0;              // for 12 Bit 
  const float C2=+0.0405;           // for 12 Bit 
  const float C3=-0.0000028;        // for 12 Bit 
  const float T1=+0.01;             // for 14 Bit @ 5V 
  const float T2=+0.00008;          // for 14 Bit @ 5V         
   
void calc_sth11(float *p_humidity ,float *p_temperature) 
{ 

  float rh; 
  float t; 
  float rh_lin;             /*rh_lin:线性湿度*/ 
  float rh_true;            /*rh_true:温度补偿的湿度*/ 
  float t_C;                /*温度*/
  
  rh =*p_humidity;          /*rh:湿度12位*/ 
  t =*p_temperature;         /*t:温度14位*/ 
   
  t_C=t*0.01 - 40.1;                 /*计算温度为(摄氏度℃),VDD:5V,14位*/
  rh_lin=C3*rh*rh + C2*rh + C1;     /*计算湿度为(%RH)*/
  rh_true=(t_C-25)*(T1+T2*rh)+rh_lin;   /*计算温度补偿的湿度(%RH)*/ 
  if(rh_true>100)rh_true=100;       /*物理量最高参数*/
  if(rh_true<0.1)rh_true=0.1;       /*物理量最低参数*/

  *p_temperature=t_C;               /*返回温度(℃)*/ 
  *p_humidity=rh_true;              /*返回湿度(%RH)*/ 
}

/********************************************************************
 * @作者:周炳权
 * @功能: 计算露点
 * @输入:humidity [%RH]；temperature [度]；
 * @输出:露点值
********************************************************************/
float calc_dewpoint(float h,float t)
{ 
  float logEx,dew_point;
  logEx=0.66077+7.5*t/(237.3+t)+(log10(h)-2);
  dew_point = (logEx - 0.66077)*237.3/(0.66077+7.5-logEx);
  return dew_point;
}

/********************************************************************
 * @作者:周炳权
 * @功能: 读取温湿度以及露点(直接调用此函数)
 * @输入:b_humi:湿度缓存；b_temp:温度缓存；b_dewpt:露点缓存
 * @输出:NONE
********************************************************************/
void sht11_measure( float *f_humi, float *f_temp, float *f_dewpt )
{
  	sht11_value humi_val = {0,},temp_val = {0,};
  	float dew_point;
  	unsigned char error,checksum;
//  unsigned int i;

  	s_connection_reset();
    error=0;
	error+=s_measure((uint16_t *)&humi_val.i,&checksum,HUMI);  /*测量湿度*/
	error+=s_measure((uint16_t *)&temp_val.i,&checksum,TEMP);  /*测量温度*/
	if(error!=0) 
		s_connection_reset();                 /*error!=0则无应答，连接复位*/
	else
	{ 
		humi_val.f = (float)humi_val.i;                   /*整数转换成浮点数*/
		temp_val.f = (float)temp_val.i;                   /*整数转换成浮点数*/
		calc_sth11(&humi_val.f,&temp_val.f);            /*计算湿度和温度*/
		dew_point = calc_dewpoint(humi_val.f,temp_val.f); /*计算露点*/
		*f_humi  = humi_val.f;
		*f_temp  = temp_val.f;
		*f_dewpt = dew_point;
	}

	/*!!!为了防止传感器发热，必须保证调用周期至少为0.8S!!!*/
	return;
}

/********************************************************************
 * @创建人:
 * @功能:驱动底层读入口
 * @输入:buf 读入缓存 
 		 size要读数据的大小
 * @输出:实际读到的大小
********************************************************************/
static Bsize_t sht11_driver_read( uint8_t*buf, Bsize_t size , void*pos )
{

	return size;
}
/********************************************************************
 * @创建人:
 * @功能:驱动底层读入口
 * @输入:buf 读入缓存      
 		 size要读数据的大小
 * @输出:实际读到的大小
********************************************************************/
static Bsize_t sht11_driver_write( uint8_t*buf, Bsize_t size , void*pos )
{

	return size;
}

/********************************************************************
 * @创建人: 
 * @功能: 驱动控制命令
 * @输入: cmd 控制命令  arg 控制参数
 * @输出: NONE
********************************************************************/
static void  sht11_driver_ioctl( uint8_t cmd, void*arg )
{
	float *m_param = (float *)arg;
	switch ( cmd )
	{
	case SHT11_CMD_WSD_OBTAIN:
		sht11_measure(&m_param[0],&m_param[1],&m_param[2]);
		break;
	default:
		break;
	}
	return;
}
/********************************************************************
 * @创建人: 
 * @功能: 驱动卸载
 * @输入:	NONE
 * @输出:	NONE
********************************************************************/
static void sht11_driver_uninstall( void )
{
	return;
}

/********************************************************************
 * @创建人: 揭成
 * @功能: 驱动初始化
 * @输入:	NONE
 * @输出:	NONE
********************************************************************/
static void sht11_driver_init( void )
{
	SET_DATA_OUTPUT();
	SET_SCK_OUTPUT();
	sht11_measure(&g_f_humi,&g_f_temp,&g_f_dewpt);
	return;
}
/********************************************************************
 * @创建人:
 * @功能:驱动注册
 * @输入:none
 * @输出:NONE
********************************************************************/
dev_id_t sht11_driver_register( void )
{
	uint8_t ret;
	os_dirver_operate_t temp;
	temp.os_driver_init   = sht11_driver_init;
	temp.os_driver_ioctl  = sht11_driver_ioctl;
	temp.os_driver_read   = sht11_driver_read;
	temp.os_driver_write  = sht11_driver_write;
	temp.os_driver_uninstall = sht11_driver_uninstall;
	ret = os_driver_add( &temp, "SHT11" );
	return ret;
}
#pragma DATA_SECTION (sht11_list,"DR_LIST_ADDR")
const DR_member_t sht11_list[]=
{
	sht11_driver_register,
	0x5a5a
};

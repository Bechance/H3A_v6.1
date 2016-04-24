/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:SHT11����������
* ����ʱ��:2011-11-9
* ������˾ :����
* �ļ�����:sht11_driver.c
* ���� :�ܱ�Ȩ
* ��������:SHT11����������֧�֣��ṩSH11������ȡ����
*----------------------------------------------------------------
�޸ļ�¼:

�޸�Ŀ��:

�޸�����:

�޸���:
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
#pragma CODE_SECTION(us_delay,"ramfuncs") 		//���öδ���������ramfuncs��
#pragma CODE_SECTION(DSP28x_usDelay,"ramfuncs") //���öδ���������ramfuncs��
void us_delay( uint16_t x )	/* 1��s,150	MHz	*/
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
 * @����:�ܱ�Ȩ
 * @����: дһ�ֽ����ݺ�У��Ӧ��
 * @����:	value:Ҫ���͵�����
 * @���:Ӧ��check
********************************************************************/
char sht11_write_byte(unsigned char value) 
{  
  unsigned char i,error=0;
  SET_DATA_OUTPUT();		/*�������*/
  for(i=0x80;i>0;i/=2)     /*��λ��ʶ*/ 
  {  
    if(i & value)
    {
		SET_DATA_H();       /*����i��д��SENSI����*/ 
    }
    else 
	{
		SET_DATA_L(); 
    }
	us_delay(2);
    SET_SCK_H();            /*SENSI����ʱ��*/
    us_delay(5);            /*��ʱ��Լ5us*/           
    SET_SCK_L();        
  }       
  SET_DATA_L();             /*�ͷ�������*/             
  SET_DATA_INPUT();			/*��������*/
  SET_SCK_H();              /*�ھŸ�ʱ��ΪӦ���ź�*/  
  us_delay(2); 
  error = READ_DATA;      /*���Ӧ�������߻ᱻSHT11����*/ 
  us_delay(2); 
  SET_SCK_L();         
  return error;             /*��error = 1������Ӧ��*/
}
/********************************************************************
 * @����:�ܱ�Ȩ
 * @����: ��һ�ֽ����ݺ�����Ӧ��"ACK = 1"
 * @����:Ӧ��λ
 * @���:����������
********************************************************************/
char sht11_read_byte(unsigned char ack) 
{  
  unsigned char i,val=0;
  SET_DATA_INPUT();			/*��������*/ 
  for(i=0x80;i>0;i/=2)              
  { 
  	SET_SCK_H();                                  
    us_delay(2); 
    if(READ_DATA) val=(val | i); /*��λ*/  
    SET_SCK_L();                                            
    us_delay(2); 
  }    
  SET_DATA_OUTPUT();	/*�������*/ 
  DATA_OUT(!ack);        /*"ack == 1"������������*/ 
  SET_SCK_H();          /*�ھŸ�ʱ��ΪӦ���ź�*/  
  us_delay(5);           
  SET_SCK_L();
  SET_DATA_INPUT();		/*��������*/ 
  return val; 
} 
  
/********************************************************************
 * @����:�ܱ�Ȩ
 * @����:��������
 * @����:NONE
 * @���:NONE
********************************************************************/
void s_trans_start(void) 
{           
  SET_DATA_OUTPUT();	/*�������*/ 
  SET_DATA_H();                           
  SET_SCK_L();     		/*��ʼ��״̬*/                                                                     
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
  SET_DATA_INPUT();		/*��������*/ 
}                                        

/********************************************************************
 * @����:�ܱ�Ȩ
 * @����: ͨѶ��λ
 * @����:NONE
 * @���:NONE
********************************************************************/
void s_connection_reset(void) 
{   
  unsigned char i;  
  SET_DATA_OUTPUT();		/*�������*/ 
  SET_DATA_H();                           
  SET_SCK_L();       		/*��ʼ��״̬*/
  for(i=0;i<9;i++)          /*9��ʱ������*/ 
  { 
  	SET_SCK_H();  
    us_delay(1); 
    SET_SCK_L(); 
    us_delay(1); 
  } 
  s_trans_start();       /*���俪ʼ*/
  SET_DATA_INPUT();		/*��������*/ 
} 
 
/********************************************************************
 * @����:�ܱ�Ȩ
 * @����: �����λ
 * @����:NONE
 * @���:Ӧ��check
********************************************************************/
char s_soft_reset(void) 
{  
  unsigned char error=0;   
  s_connection_reset();              /*��λͨѶ*/
  error+=sht11_write_byte(SHT_RESET);    /*���������������*/ 
  return error;                     /*��error = 1,�򴫸���û��Ӧ��*/ 
} 

/********************************************************************
 * @����:�ܱ�Ȩ
 * @����: ��״̬�Ĵ�����У���
 * @����:p_value:�������ݻ��棻p_checksum:����У��ͻ���
 * @���:Ӧ��check
********************************************************************/
char s_read_statusreg(unsigned char *p_value, unsigned char *p_checksum) 
{  
  unsigned char error=0; 
  s_trans_start();                   	 /*���俪ʼ*/ 
  error=sht11_write_byte(STATUS_REG_R);  /*���������������*/ 
  *p_value=sht11_read_byte(SHT11_noACK);      /*��״̬�Ĵ���(8λ)*/ 
  *p_checksum=sht11_read_byte(SHT11_ACK);    /*��У���(8λ)*/   
  return error;                        /*��error = 1,�򴫸���û��Ӧ��*/ 
}                           

/********************************************************************
 * @����:�ܱ�Ȩ
 * @����: д״̬�Ĵ�����У���
 * @����:p_value:�������ݻ��棻
 * @���:Ӧ��check
********************************************************************/
char s_write_statusreg(unsigned char *p_value) 
{  
  unsigned char error=0; 
  s_trans_start();                   /*���俪ʼ*/ 
  error+=sht11_write_byte(STATUS_REG_W);/*���������������*/ 
  error+=sht11_write_byte(*p_value);    /*���Ͳ�����������*/ 
  return error;                     /*��error >= 1,�򴫸���û��Ӧ��*/ 
} 

/********************************************************************
 * @����:�ܱ�Ȩ
 * @����: �¶�/ʪ�ȵĲ�����У��
 * @����:p_value:���ݻ��棻p_checksum:У��ͻ��棻mode:����TEMP��HUMI
 * @���:Ӧ��check
********************************************************************/
char s_measure(uint16_t *p_value, unsigned char *p_checksum, unsigned char mode) 
{  
  unsigned char error=0;
  unsigned char value_M;
  unsigned char value_L;
  uint16_t value_a;
  uint32_t timeout = 65535;

  s_trans_start();          /*���俪ʼ*/     
  
  switch(mode)				/*���������������*/
  {             
    case TEMP: error+=sht11_write_byte(MEASURE_TEMP); break; 
    case HUMI: error+=sht11_write_byte(MEASURE_HUMI); break; 
    default: break;          
  }
  
  SET_DATA_INPUT();		/*��������*/

  while(1) 
  { 
   	if(READ_DATA == 0) 
		break; /*�ȴ���������ɲ���*/ 
	else if( timeout == 0) 
		break; /*��ʱ�˳�*/
	timeout--;
	us_delay(3);
  } 

  if(READ_DATA != 0)
  {
	error+=1;    /*��ʱ�ﵽ1~2��*/
  }
                  
	value_M = sht11_read_byte(SHT11_ACK);    //read the first byte (MSB)
	value_L = sht11_read_byte(SHT11_ACK);    //read the second byte (LSB)
	value_a = ((value_M << 8) | value_L);
  *p_value = value_a;
  *p_checksum =sht11_read_byte(SHT11_noACK);  //read checksum      
  return error; 
}

/********************************************************************
 * @����:�ܱ�Ȩ
 * @����: �����¶�(��)/ʪ��(%RH)
 * @����:p_humidity:12λ��ʪ�����ݣ�p_temperature:14λ���¶����ݣ�
 * @���:NONE
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
  float rh_lin;             /*rh_lin:����ʪ��*/ 
  float rh_true;            /*rh_true:�¶Ȳ�����ʪ��*/ 
  float t_C;                /*�¶�*/
  
  rh =*p_humidity;          /*rh:ʪ��12λ*/ 
  t =*p_temperature;         /*t:�¶�14λ*/ 
   
  t_C=t*0.01 - 40.1;                 /*�����¶�Ϊ(���϶ȡ�),VDD:5V,14λ*/
  rh_lin=C3*rh*rh + C2*rh + C1;     /*����ʪ��Ϊ(%RH)*/
  rh_true=(t_C-25)*(T1+T2*rh)+rh_lin;   /*�����¶Ȳ�����ʪ��(%RH)*/ 
  if(rh_true>100)rh_true=100;       /*��������߲���*/
  if(rh_true<0.1)rh_true=0.1;       /*��������Ͳ���*/

  *p_temperature=t_C;               /*�����¶�(��)*/ 
  *p_humidity=rh_true;              /*����ʪ��(%RH)*/ 
}

/********************************************************************
 * @����:�ܱ�Ȩ
 * @����: ����¶��
 * @����:humidity [%RH]��temperature [��]��
 * @���:¶��ֵ
********************************************************************/
float calc_dewpoint(float h,float t)
{ 
  float logEx,dew_point;
  logEx=0.66077+7.5*t/(237.3+t)+(log10(h)-2);
  dew_point = (logEx - 0.66077)*237.3/(0.66077+7.5-logEx);
  return dew_point;
}

/********************************************************************
 * @����:�ܱ�Ȩ
 * @����: ��ȡ��ʪ���Լ�¶��(ֱ�ӵ��ô˺���)
 * @����:b_humi:ʪ�Ȼ��棻b_temp:�¶Ȼ��棻b_dewpt:¶�㻺��
 * @���:NONE
********************************************************************/
void sht11_measure( float *f_humi, float *f_temp, float *f_dewpt )
{
  	sht11_value humi_val = {0,},temp_val = {0,};
  	float dew_point;
  	unsigned char error,checksum;
//  unsigned int i;

  	s_connection_reset();
    error=0;
	error+=s_measure((uint16_t *)&humi_val.i,&checksum,HUMI);  /*����ʪ��*/
	error+=s_measure((uint16_t *)&temp_val.i,&checksum,TEMP);  /*�����¶�*/
	if(error!=0) 
		s_connection_reset();                 /*error!=0����Ӧ�����Ӹ�λ*/
	else
	{ 
		humi_val.f = (float)humi_val.i;                   /*����ת���ɸ�����*/
		temp_val.f = (float)temp_val.i;                   /*����ת���ɸ�����*/
		calc_sth11(&humi_val.f,&temp_val.f);            /*����ʪ�Ⱥ��¶�*/
		dew_point = calc_dewpoint(humi_val.f,temp_val.f); /*����¶��*/
		*f_humi  = humi_val.f;
		*f_temp  = temp_val.f;
		*f_dewpt = dew_point;
	}

	/*!!!Ϊ�˷�ֹ���������ȣ����뱣֤������������Ϊ0.8S!!!*/
	return;
}

/********************************************************************
 * @������:
 * @����:�����ײ�����
 * @����:buf ���뻺�� 
 		 sizeҪ�����ݵĴ�С
 * @���:ʵ�ʶ����Ĵ�С
********************************************************************/
static Bsize_t sht11_driver_read( uint8_t*buf, Bsize_t size , void*pos )
{

	return size;
}
/********************************************************************
 * @������:
 * @����:�����ײ�����
 * @����:buf ���뻺��      
 		 sizeҪ�����ݵĴ�С
 * @���:ʵ�ʶ����Ĵ�С
********************************************************************/
static Bsize_t sht11_driver_write( uint8_t*buf, Bsize_t size , void*pos )
{

	return size;
}

/********************************************************************
 * @������: 
 * @����: ������������
 * @����: cmd ��������  arg ���Ʋ���
 * @���: NONE
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
 * @������: 
 * @����: ����ж��
 * @����:	NONE
 * @���:	NONE
********************************************************************/
static void sht11_driver_uninstall( void )
{
	return;
}

/********************************************************************
 * @������: �ҳ�
 * @����: ������ʼ��
 * @����:	NONE
 * @���:	NONE
********************************************************************/
static void sht11_driver_init( void )
{
	SET_DATA_OUTPUT();
	SET_SCK_OUTPUT();
	sht11_measure(&g_f_humi,&g_f_temp,&g_f_dewpt);
	return;
}
/********************************************************************
 * @������:
 * @����:����ע��
 * @����:none
 * @���:NONE
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

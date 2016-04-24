/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:
* 创建时间:2013-06-7
* 所属公司:科荟
* 文件名称:app_WIFImod_Wifi_send.c
* 创建人    :揭成
* 功能描述:WIFI发送
*-------------------------------------------------------------------*/
#include	"app_WIFImod_inside.h"
#define      DLEN_SIZE    1000
/*-------------------------------------------------------------------*/
dev_id_t    m_WIFImod_Wifi_dev     = INVALID_DEV_ID;
uint8_t	    m_mem_send[DLEN_SIZE];
char        m_mem_data[DLEN_SIZE];
float       m_WIFIfft[60];
float 		m_WIFI_XBdata[6][52];

const char* m_DLkey_tab[]=
{
   "Ua=",   "Ia=",   "Ja=","Pa=","Qa=",
   "Ub=",   "Ib=",   "Jb=","Pb=","Qb=",
   "Uc=",   "Ic=",   "Jc=","Pc=","Qc=",
   "JIac=", "JIbc=", "JIab=",
   "JUac=", "JUbc=", "JUab=",
   "Pabc=", "Qabc=", "Err1=",
   "Err2=", "Err3=", "Err4=",
   "ErrQ1=","ErrQ2=","ErrQ3=",
   "ErrQ4=","Feq=",  "Cos=",
   "CosLc=","JXMOD="
};


const char* m_WIRkey_tab[]=
{
   "Cos=", "Ua=", "Ub=",   "Uc=","Ia=",
   "Ib=",  "Ic=", "JXMOD=","PHASE_ORDER=","RESULT=",
   "G="
};
/********************************************************************
 * @创建人 :揭成
 * @功能 :WIFI原子级发送
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
static void Wifi_send_data(uint8_t*pD,Bsize_t len)
{
  os_driver_write(m_WIFImod_Wifi_dev,pD,len,0);
  return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :电参数数据发送前创建
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
static void app_WIFImod_DLdata_create(char*pbuf)
{
	int pos;
	DL_flag_t dl_flag;
	char ch[10]="";
	float*pf = (float*)&g_DL_param;
	app_global_DLflag_obtain(&dl_flag);
	for(pos=0;pos<SIZE_OF_ARRAY(m_DLkey_tab)-2;pos++)
	{
		GLOBAL_MEMCLR(ch, 10);
		global_strcat(pbuf,m_DLkey_tab[pos]);
		global_ftoa(pf[pos],ch,2);
		global_strcat(pbuf,ch);
		global_strcat(pbuf,",");
	}
	GLOBAL_MEMCLR(ch, 10);
	global_strcat(pbuf,m_DLkey_tab[pos++]);
	if(g_DL_param.cos_LC==0)ch[0]=' ';
    else ch[0]=g_DL_param.cos_LC;
	global_strcat(pbuf,ch);
	global_strcat(pbuf,",");
	GLOBAL_MEMCLR(ch, 10);
	global_strcat(pbuf,m_DLkey_tab[pos]);
	if(dl_flag.phase3or4==JXMOD_3_3)global_strcat(pbuf,"T");
	else global_strcat(pbuf,"F");
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能   :查线数据发送前创建
 * @输入   :NONE
 * @输出   :NONE
********************************************************************/
static void app_WIFImod_WIRdata_create(char*pbuf)
{
	int pos,Tabpos;
    WIRresult_t*pResult;
	for(pos=0;pos<4;pos++)
	{
	    Tabpos = 0;
	    pResult = &m_WIRresult[pos];
		global_strcat(pbuf,m_WIRkey_tab[Tabpos++]);
		global_strcat(pbuf,pResult->cos);
		global_strcat(pbuf,",");
		global_strcat(pbuf,m_WIRkey_tab[Tabpos++]);
		global_strcat(pbuf,pResult->Result_MUa);
		global_strcat(pbuf,",");
		global_strcat(pbuf,m_WIRkey_tab[Tabpos++]);
		global_strcat(pbuf,pResult->Result_MUb);
		global_strcat(pbuf,",");
		global_strcat(pbuf,m_WIRkey_tab[Tabpos++]);
		global_strcat(pbuf,pResult->Result_MUc);
		global_strcat(pbuf,",");
		global_strcat(pbuf,m_WIRkey_tab[Tabpos++]);
		global_strcat(pbuf,pResult->Result_MIa);
		global_strcat(pbuf,",");
		global_strcat(pbuf,m_WIRkey_tab[Tabpos++]);
		global_strcat(pbuf,pResult->Result_MIb);
		global_strcat(pbuf,",");
		global_strcat(pbuf,m_WIRkey_tab[Tabpos++]);
		global_strcat(pbuf,pResult->Result_MIc);
		global_strcat(pbuf,",");	
		global_strcat(pbuf,m_WIRkey_tab[Tabpos++]);
		if(pResult->jxmod==JXMOD_3_3)global_strcat(pbuf,"T");
		else global_strcat(pbuf,"F");
		global_strcat(pbuf,",");	
		global_strcat(pbuf,m_WIRkey_tab[Tabpos++]);
		if(pResult->phaseDir==PHASE_ADD)global_strcat(pbuf,"S");
		else global_strcat(pbuf,"R");
		global_strcat(pbuf,",");	
		global_strcat(pbuf,m_WIRkey_tab[Tabpos++]);
		if(pResult->verdict==TURE)global_strcat(pbuf,"T");
		else global_strcat(pbuf,"F");
		global_strcat(pbuf,",");	
		global_strcat(pbuf,m_WIRkey_tab[Tabpos++]);
		global_strcat(pbuf,pResult->GX);
		if(pos<4)global_strcat(pbuf,";");
	}
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :FFT数据标题发送前创建
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
static void app_WIFImod_FFTdata_title_create(int xpos,char*pbuf)
{
    GLOBAL_MEMCLR(m_WIFIfft,sizeof(float)*52);
    if(xpos==0)
    {
        global_memcpy(m_WIFIfft,m_WIFI_XBdata[0],52);
		global_strcat(pbuf,"PHASE=Ua,");
    }
    else if(xpos==1)
    {
        global_memcpy(m_WIFIfft,m_WIFI_XBdata[1],52);
		global_strcat(pbuf,"PHASE=Ia,");
    }
    else if(xpos==2)
    {
        global_memcpy(m_WIFIfft,m_WIFI_XBdata[2],52);
		global_strcat(pbuf,"PHASE=Ub,");
    }
    else if(xpos==3)
    {
        global_memcpy(m_WIFIfft,m_WIFI_XBdata[3],52);
		global_strcat(pbuf,"PHASE=Ib,");
    }
    else if(xpos==4)
    {
        global_memcpy(m_WIFIfft,m_WIFI_XBdata[4],52);
		global_strcat(pbuf,"PHASE=Uc,");
    }
    else if(xpos==5)
    {
        global_memcpy(m_WIFIfft,m_WIFI_XBdata[5],52);
		global_strcat(pbuf,"PHASE=Ic,");
    }
    return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :FFT数据发送前创建
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
static void app_WIFImod_FFTdata_create(char*pbuf)
{
	int xpos,pos;
	char ch[10]="";
	float*pf = m_WIFIfft;
	os_driver_ioctl(g_DL_dev,MEASURE_CMD_XBDATA_GET,m_WIFI_XBdata);
	for(xpos=0;xpos<6;xpos++)
	{
		app_WIFImod_FFTdata_title_create(xpos,pbuf);
		global_strcat(pbuf,"THD=");
		GLOBAL_MEMCLR(ch, 10);
		global_ftoa(pf[0],ch,2);
		global_strcat(pbuf,ch);
		global_strcat(pbuf,"%,DATA=");
		for(pos=2;pos<52;pos++)
		{
			GLOBAL_MEMCLR(ch, 10);
			global_ftoa(pf[pos],ch,2);
			global_strcat(pbuf,ch);
			if(pos==51)
			global_strcat(pbuf,";");
			else
			global_strcat(pbuf,"~");
		}
	}
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :变比数据发送前创建
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
static void app_WIFImod_BBdata_create(char*pbuf)
{
	char ch[10]="";
	GLOBAL_MEMCLR(ch, 10);
	global_strcat(pbuf,"M=");
	global_ftoa(g_DL_param.phase_c.f_I,ch,2);
	global_strcat(pbuf,ch);
	global_strcat(pbuf,",");
	GLOBAL_MEMCLR(ch, 10);
	global_strcat(pbuf,"S=");
	global_ftoa(g_DL_param.phase_a.f_I,ch,2);
	global_strcat(pbuf,ch);
	global_strcat(pbuf,",");
	GLOBAL_MEMCLR(ch, 10);
	global_strcat(pbuf,"PD=");
	global_ftoa(g_DL_param.f_JIac,ch,2);
	global_strcat(pbuf,ch);		
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :FFT数据发送
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
static void app_WIFImod_fftdata_send(void)
{
	Wifi_data_t*phead;
	char*pD;
	uint8_t*pCUR;
	uint16_t len,Dlen;
	phead = (Wifi_data_t*)m_mem_send;
	pD    = (uint8_t*)m_mem_data;
	GLOBAL_MEMCLR(pD, DLEN_SIZE);
	app_WIFImod_FFTdata_create(pD);
	len = global_strlen(pD);
	Dlen = len + 1;
	phead->head1 = 0x68;
	phead->head2 = 0x86;
	phead->len[0]= (Dlen&0xff);
	phead->len[1]= (Dlen>>8);
	phead->Ctl   = 0x42;
	pCUR = (uint8_t*)&phead->Ctl+1;
	global_memcpy(pCUR,pD,len);
	pCUR+=len;
	*pCUR++ = 0;
	*pCUR++ = 0x16;
	Wifi_send_data((uint8_t*)phead,sizeof(Wifi_data_t)+len+2);
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :变笔数据
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
static void app_WIFImod_BBdata_send(void)
{
	Wifi_data_t*phead;
	char*pD;
	uint8_t*pCUR;
	uint16_t len,Dlen;
	phead = (Wifi_data_t*)m_mem_send;
	pD    = (uint8_t*)m_mem_data;
	GLOBAL_MEMCLR(pD, DLEN_SIZE);
	app_WIFImod_BBdata_create(pD);
	len = global_strlen(pD);
	Dlen = len + 1;
	phead->head1 = 0x68;
	phead->head2 = 0x86;
	phead->len[0]= (Dlen&0xff);
	phead->len[1]= (Dlen>>8);
	phead->Ctl   = 0x62;
	pCUR = (uint8_t*)&phead->Ctl+1;
	global_memcpy(pCUR,pD,len);
	pCUR+=len;
	*pCUR++ = 0;
	*pCUR++ = 0x16;
	Wifi_send_data((uint8_t*)phead,sizeof(Wifi_data_t)+len+2);
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :电参数发送
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
static void app_WIFImod_DLdata_send(uint8_t ctl)
{
	Wifi_data_t*phead;
	char*pD;
	uint8_t*pCUR;
	uint16_t len,Dlen;
	phead = (Wifi_data_t*)m_mem_send;
	pD    = (uint8_t*)m_mem_data;
	GLOBAL_MEMCLR(pD, DLEN_SIZE);
	app_WIFImod_DLdata_create(pD);
	len = global_strlen(pD);
	Dlen = len + 1;
	phead->head1 = 0x68;
	phead->head2 = 0x86;
	phead->len[0]= (Dlen&0xff);
	phead->len[1]= (Dlen>>8);
	phead->Ctl   = ctl;
	pCUR = (uint8_t*)&phead->Ctl+1;
	global_memcpy(pCUR,pD,len);
	pCUR+=len;
	*pCUR++ = 0;
	*pCUR++ = 0x16;
	Wifi_send_data((uint8_t*)phead,sizeof(Wifi_data_t)+len+2);
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :查线数据发送
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
static void app_WIFImod_WIRdata_send(void)
{
	Wifi_data_t*phead;
	char*pD;
	uint8_t*pCUR;
	uint16_t len,Dlen;
	phead = (Wifi_data_t*)m_mem_send;
	pD    = (uint8_t*)m_mem_data;
	GLOBAL_MEMCLR(pD, DLEN_SIZE);
	app_WIFImod_WIRdata_create(pD);
	len  = global_strlen(pD);
	Dlen = len + 1;
	phead->head1 = 0x68;
	phead->head2 = 0x86;
	phead->len[0]= (Dlen&0xff);
	phead->len[1]= (Dlen>>8);
	phead->Ctl   = 0x33;
	pCUR = (uint8_t*)&phead->Ctl+1;
	global_memcpy(pCUR,pD,len);
	pCUR+=len;
	*pCUR++ = 0;
	*pCUR++ = 0x16;
	Wifi_send_data((uint8_t*)phead,sizeof(Wifi_data_t)+len+2);
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :Wifi数据发送任务
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
static void app_WIFImod_Wifi_task(void*arg)
{
	uint8_t err;
	int cnt=0;
	while ( 1 )
	{
		msleep(TYPE_DELAY);
		if(++cnt%100!=0)continue;
		if(m_EPAD_code==READ_WIR)
		{
			app_WIFImod_WIRdata_send();
			app_WIFImod_DLdata_send(0x32);  
		}
		else if(m_EPAD_code==ONE_METER_ERR)
		{
			app_WIFImod_DLdata_send(0x12);  
		}
		else if(m_EPAD_code==READ_FFT)
		{
			app_WIFImod_fftdata_send();
		}
		else if(m_EPAD_code==READ_BB)
		{
			app_WIFImod_BBdata_send();
		}
	}
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :Wifi初始化
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void Wifi_init(void)
{
	uint32_t baud = 115200;
	m_WIFImod_Wifi_dev =  serialB_driver_register();
	if ( m_WIFImod_Wifi_dev != INVALID_DEV_ID )
	{
		os_driver_init( m_WIFImod_Wifi_dev );					 //初始化串口驱动;
	}
	os_driver_ioctl( m_WIFImod_Wifi_dev, SET_SERIAL_BAUD, &baud );			 //波特率设置;	
	os_driver_ioctl( g_gpio_dev, GPIO_CMD_WIFI_RESET, 0);		 
	os_task_assign_stk_build( app_WIFImod_Wifi_task, 3);
	os_task_assign_stk_build(Wifi_rev_task,3);
    return;
}

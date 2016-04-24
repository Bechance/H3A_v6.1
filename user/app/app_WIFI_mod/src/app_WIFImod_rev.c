/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:
* 创建时间:2013-06-7
* 所属公司:科荟
* 文件名称:app_WIFImod_Wifi_rev.c
* 创建人    :揭成
* 功能描述:WIFI接收
*-------------------------------------------------------------------*/
#include		    "app_WIFImod_inside.h"
EPAD_code_t         m_EPAD_code = 0;
char       			m_mem_rev[200];
DL_app_user_set_t   m_WIRuser_setting;
/********************************************************************
 * @创建人:揭成
 * @功能  :epad 控制命令
 * @输入  :NONE
 * @输出  :NONE
********************************************************************/
static void  Wifi_epad_code_obtain(Wifi_data_t*phead)
{	
	uint8_t*pCUR;
	uint16_t Dlen;
	Dlen = phead->len[1];
	Dlen = Dlen*0x100+phead->len[0];		
	pCUR = (uint8_t*)&phead->Ctl+1;
	os_driver_read( m_WIFImod_Wifi_dev,pCUR,Dlen,0);
	m_EPAD_code  =  (EPAD_code_t)(*pCUR);
	if(m_EPAD_code==READ_BB)
	{
		m_IS_BB_MOD = TURE;
	}
	else
	{
		m_IS_BB_MOD = FAIL;
	}
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :解析PAD选项
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
static char* _epad_code_encode(char* pCUR,char*pdes,char*title)
{
	char *ps,*pe;
    ps=global_string_locate_first(pCUR,title)+strlen(title);
    pe=global_string_locate_first(pCUR,",");
    global_memcpy(pdes,ps,((long)pe-(long)ps));
    return (pe+1);
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :解析PAD选项
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
static void  Wifi_epad_setting(Wifi_data_t*phead)
{	
	uint8_t*pCUR;
	uint16_t Dlen;
	char ch[10];
	Dlen = phead->len[1];
	Dlen = Dlen*0x10+phead->len[0];		
	pCUR = (uint8_t*)&phead->Ctl+1;
	os_driver_read( m_WIFImod_Wifi_dev,pCUR,Dlen,0);
	pCUR[Dlen-1] =',';
    GLOBAL_MEMCLR(ch,10);
    pCUR=_epad_code_encode(pCUR,ch,"CONST=");
    GLOBAL_MEMCLR(m_WIRuser_setting.cs,9);
	global_strcpy(m_WIRuser_setting.cs,ch);
    GLOBAL_MEMCLR(ch,10);
    pCUR=_epad_code_encode(pCUR,ch,"ROUND_NUM=");
    GLOBAL_MEMCLR(m_WIRuser_setting.qs,9);
	global_strcpy(m_WIRuser_setting.qs,ch);
    GLOBAL_MEMCLR(ch,10);
    pCUR=_epad_code_encode(pCUR,ch,"WAY=");
	m_WIRuser_setting.PorQ     = ch[0];
	m_WIRuser_setting.handOR_GD= ch[1];
    GLOBAL_MEMCLR(ch,10);
    pCUR=_epad_code_encode(pCUR,ch,"INPUT=");
	os_driver_ioctl(g_DL_dev,MEASURE_CMD_USER_DL_ARG_SET,&m_WIRuser_setting);
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能   :修正ADE7880
 * @输入   :NONE
 * @输出   :NONE
********************************************************************/
static void  send_CMD_correct_7780(char cmd,char UorI,char phase,char opt)
{ 
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能  :接收修正命令
 * @输入  :NONE
 * @输出  :NONE
********************************************************************/
static void  Wifi_epad_correct(Wifi_data_t*phead)
{

    return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能   :WIFI接收任务
 * @输入   :NONE
 * @输出   :NONE
********************************************************************/
void Wifi_rev_task(void*arg)
{
	Bsize_t size;
	Wifi_data_t*phead ;
	phead=(Wifi_data_t*)m_mem_rev;
	GLOBAL_MEMCLR((char*)phead,sizeof(char)*200);
	global_strcpy(m_WIRuser_setting.cs,"36000");
	global_strcpy(m_WIRuser_setting.qs,"10");
	while ( 1 )
	{
		msleep( TYPE_DELAY );
		size = os_driver_read( m_WIFImod_Wifi_dev, ( uint8_t* )&phead->head1, 1, 0 );
		if ( size != 1 || ( phead->head1!= 0x68))
		{
			continue;
		}
		size = os_driver_read( m_WIFImod_Wifi_dev, ( uint8_t* )&phead->head2, 1, 0 );
		if ( size != 1 || ( phead->head2!= 0x86))
		{
			continue;
		}
		size = os_driver_read( m_WIFImod_Wifi_dev,(uint8_t*)phead->len,sizeof(Wifi_data_t )-2,0);
		if ( size != sizeof( Wifi_data_t )-2)	 
		{
			continue;
		}
		if(phead->Ctl==0x01)
		{
            Wifi_epad_code_obtain(phead);
		}
		else if(phead->Ctl==0x11)
		{
            Wifi_epad_setting(phead);
		}
		else if(phead->Ctl==0xD0)
		{
            Wifi_epad_correct(phead);
		}
	}
}

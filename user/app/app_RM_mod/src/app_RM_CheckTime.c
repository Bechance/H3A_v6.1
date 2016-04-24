/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:����Уʱ����
* ����ʱ��:2013-07-02
* ������˾ :����
* �ļ�����:app_RM_CheckTime.c
* ���� :�ܱ�Ȩ
* ��������:ʵ�ֶ���Уʱ����
******************************************************************************************/
#include	"app_RM_inside.h"

char 	  m_meter_Password[9] = "";
time_val_t 	  m_time_buf={0,};

/*****************************************************************************/
uint8_t MsgPasswordBox_edit_callback_input( hwin_t*hp )
{
	uint8_t key;
	char  password[9] = "";
	GLOBAL_MEMCLR(m_meter_Password, sizeof(m_meter_Password));
	key_input_method_assign( KEY_INPUT_123 );
	GUI_window_hwin_req( hp );
	app_global_key_control_power_req();
	GUI_set_Fgcolor( C_GUI_YELLOW );
	GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
	while ( 1 )
	{
		msleep( 1 );
		if ( app_global_key_force_obtain( &key, 1 ) != 1 )
		{
			continue;
		}
		if (( app_global_decide_inputkey( key ) ) != FAIL )
		{
			key_method_input_shadow( key, password );
		}
		else if ( key == KEY_ENTER )
		{
			global_memcpy(m_meter_Password,password, sizeof(password));
			break;
		}
		else if ( key == KEY_EXIT )
		{
			break;
		}
		
	}
	app_global_key_control_power_release();
	GUI_window_hwin_release( hp );
	return key;
}
static Bsize_t MsgPasswordBox_callback_input( hwin_t*pwin )
{
	Bsize_t 	num;
	hwin_t *	edit;
	edit = GUI_edit_creat( 0, 20,
						   pwin->win_wide - 2,
						   pwin->win_high - 20,WIN_TOUCH_EN, pwin );
	GUI_set_Fgcolor( C_GUI_BlACK );
	GUI_set_Bgcolor( MSGBOX_BG_COLOR );
	GUI_window_hwin_req( edit );
	GUI_SetFront(GUI_GET_DEFGRONT());	
	GUI_edit_memeber_add( 5, 5, 8, "����������:", MEMBER_3D | 1 );
	GUI_window_hwin_release( edit );
	g_key_input_keep_secret = FAIL; /*���óɲ���������ģʽ*/
	num = GUI_edit_start( edit, MsgPasswordBox_edit_callback_input );
	g_key_input_keep_secret = FAIL;
	return num;
}

/********************************************************************
 * @���� :
 * @���� :������ʾ�Ի���
 * @���� :(x,y)����
 *@���  :NONE
********************************************************************/
BOOL MsgPasswordBox_input(int x,int y,Bsize_t wide,Bsize_t high,char*title)

{
	uint8_t key;
	while (1)
	{	
		key = GUI_MsgBox_creat(
				  x, y, wide,high, title,
				  MsgPasswordBox_callback_input );
		if ( key == KEY_ENTER ) return TURE;
		else if ( key == KEY_EXIT ) return FAIL;

	}
}

/********************************************************************
 *@���� :���ı�
 *@���� :645�㲥Уʱ()
 *@ʵ�ַ�ʽ:�ȶ�ȡ���ʱ�䣬��У����ʱ��Ƚ�,�����4.8������,ֱ����У����ʱ����Ϊ�㲥Уʱ.
 		 ��������4.8����,�򰴵��ʱ��,�������ʱ�俿��4.8���Ӷ�ʱ,
 		 ��:����ʱ��Ϊ16:25:00ʱ,У���ǵ�ʱ��Ϊ16:45:34ʱ,��У��ʱ��Ϊ16:29:48ʱ.
 *@���� :NONE
 *@��� :NONE   
********************************************************************/

/********************************************************************
 *@���� :���ı�
 *@���� :
 *@���� :NONE
 *@��� :*flag=0;�����ݵ�����������
 		 *flag=1;�����ݴ�����������
 		 *flag=2;������С����������
********************************************************************/
uint8_t app_RMmod_time_compare_fun(uint8_t meter_time,uint8_t eq_time,uint8_t *flag)
{
	if(eq_time == meter_time)
	{
		*flag=0;
		return 0;
	}
	else 
	{
		if(meter_time>eq_time)
			*flag=1;
		else
			*flag=2;
		
		return 1;
	}
}
/********************************************************************
 *@���� :���ı�
 *@���� :
 *@���� :NONE
 *@��� :1 ��2��ʾ�����ڵ���4.8���ӣ�0��ʾС��4.8����,
 
 		 *flag=0;�����ݵ�����������
 		 *flag=1;�����ݴ�����������
 		 *flag=2;������С����������
********************************************************************/
uint8_t app_RMmod_broadcast_time_handle(uint8_t *flag)
{
	uint8_t meter_time_minute=0;
	uint8_t meter_time_second=0;
	uint8_t eq_time_minute=0;
	uint8_t eq_time_second=0;
	int32_t meter_time=0;
	int32_t eq_time=0;
	if(app_RMmod_time_compare_fun(m_meter_time_buf.year,m_time_buf.year,flag))
		return 1;
	if(app_RMmod_time_compare_fun(m_meter_time_buf.month,m_time_buf.month,flag))
		return 1;
	if(app_RMmod_time_compare_fun(m_meter_time_buf.day,m_time_buf.day,flag))
		return 1;
	if(app_RMmod_time_compare_fun(m_meter_time_buf.hour,m_time_buf.hour,flag))
		return 1;

	bcd_convert_hex(m_meter_time_buf.min,&meter_time_minute);
	bcd_convert_hex(m_meter_time_buf.sec,&meter_time_second);
	bcd_convert_hex(m_time_buf.min,&eq_time_minute);
	bcd_convert_hex(m_time_buf.sec,&eq_time_second);
	meter_time=meter_time_minute*60+meter_time_second; //���Ӽ�����
	eq_time=eq_time_minute*60+eq_time_second;
	if(GLOBAL_ABS(meter_time-eq_time)>(4*60+48))  //(4*60+48)Ϊ4��48��
	{
		if(meter_time>eq_time)
		{
			*flag =1;
			return 1;
		}
		else if(meter_time<eq_time)
		{
			*flag =2;
			return 1;
		}
	}
	else
	{
		return 0;
	}
	
	return 0;
}

uint8_t app_RMmod_broadcast_time_subb(uint8_t temp,uint8_t *data,uint8_t subb,uint8_t jishu)
{
	if(temp<subb)
	{
		hex_convert_bcd(temp+jishu-subb,data);
		return 1;
	}
	else
	{
		hex_convert_bcd(temp-subb,data);
		return 0;
	}
}

uint8_t app_metermod_broadcast_time_add(uint8_t temp,uint8_t *data,uint8_t add,uint8_t jishu)
{
	if((temp+add)>=60)
	{
		hex_convert_bcd((uint8_t)temp+add-jishu,data);
		return 1;
	}
	else
	{
		hex_convert_bcd((uint8_t)temp+add,data);
		return 0;
	}
}


/********************************************************************
 * @���� :���ı�
 * @���� :��ȡʱ��BUF
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_RMmod_time_obtain( time_val_t*ptime )
{
	os_driver_ioctl(g_timer_dev_id,TMR_CMD_TIME_VALUE_GET,ptime );
	return;
}
/********************************************************************
 * @���� :���ı�
 * @���� :��������
 * @���� :*data����  len����
 * @��� :NONE
********************************************************************/
void app_RMmod_scia_send(void *data,Bsize_t len)
{
	os_driver_write( g_usart3_dev_id, data,len, 0 );
	return;
}

/********************************************************************
 *@���� :���ı�
 *@���� :�㲥645(97 ,07)��Լ�ĶԶ๦�ܵ����ж�ʱ
 *@���� :NONE
 *@��� :NONE 
 *@ʵ�ַ�ʽ:�ȶ�ȡ���ʱ�䣬��У����ʱ��Ƚ�,�����4.8������,ֱ����У����ʱ����Ϊ�㲥Уʱ.
 		 ��������4.8����,�򰴵��ʱ��,�������ʱ�俿��4.8���Ӷ�ʱ,
 		 ��:����ʱ��Ϊ16:25:00ʱ,У���ǵ�ʱ��Ϊ16:45:34ʱ,��У��ʱ��Ϊ16:29:48ʱ.

********************************************************************/
void app_RMmod_broadcast_send_time_data(void)
{
	uint8_t flag=0;
	uint8_t back_data=0;
	uint8_t temp[6]={0,};
	int8_t broadcast_Addr[20] = {'9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', 0};
	broadcast_time_t  broadcast_set_time={0,};
	if(m_meter_data.guiyue!= 0||m_meter_data.guiyue!= 1)  //97��07Э��
		return;
	app_RMmod_time_obtain(&m_time_buf);
	if(app_RMmod_broadcast_time_handle(&flag)==1)   //�����ڵ���4.8����
	{
		bcd_convert_hex(m_meter_time_buf.sec,&temp[0]);
		bcd_convert_hex(m_meter_time_buf.min,&temp[1]);
		bcd_convert_hex(m_meter_time_buf.hour,&temp[2]);
		bcd_convert_hex(m_meter_time_buf.day,&temp[3]);
		bcd_convert_hex(m_meter_time_buf.month,&temp[4]);
		bcd_convert_hex(m_meter_time_buf.year,&temp[5]);
		if(flag==1)   //�����ݼ�ȥ4.8����
		{
			back_data=app_RMmod_broadcast_time_subb(temp[0],&broadcast_set_time.Second,48-20,60);
			back_data=app_RMmod_broadcast_time_subb(temp[1],&broadcast_set_time.Minute,4+back_data,60);
			back_data=app_RMmod_broadcast_time_subb(temp[2],&broadcast_set_time.Hour,back_data,24);
			back_data=app_RMmod_broadcast_time_subb(temp[3],&broadcast_set_time.Day,back_data,31);
			back_data=app_RMmod_broadcast_time_subb(temp[4],&broadcast_set_time.Month,back_data,12);
			back_data=app_RMmod_broadcast_time_subb(temp[5],&broadcast_set_time.Year,back_data,100);
		}
		else if(flag==2)//�����ݼ���4.8����
		{
			back_data=app_metermod_broadcast_time_add(temp[0],&broadcast_set_time.Second,48,60);
			back_data=app_metermod_broadcast_time_add(temp[1],&broadcast_set_time.Minute,4+back_data,60);
			back_data=app_metermod_broadcast_time_add(temp[2],&broadcast_set_time.Hour,back_data,24);
			back_data=app_metermod_broadcast_time_add(temp[3],&broadcast_set_time.Day,back_data,32);
			back_data=app_metermod_broadcast_time_add(temp[4],&broadcast_set_time.Month,back_data,13);
			back_data=app_metermod_broadcast_time_add(temp[5],&broadcast_set_time.Year,back_data,100);
		}
	}
	else							//���С��4.8����
	{
		broadcast_set_time.Second= m_time_buf.sec;
		broadcast_set_time.Minute= m_time_buf.min;
		broadcast_set_time.Hour  = m_time_buf.hour;
		broadcast_set_time.Day   = m_time_buf.day;
		broadcast_set_time.Month = m_time_buf.month;
		broadcast_set_time.Year  = m_time_buf.year;
	}
	broadcast_set_time.head[0]=0xFE;
	broadcast_set_time.head[1]=0xFE;
	broadcast_set_time.head[2]=0xFE;
	broadcast_set_time.head1=0x68;	
	broadcast_set_time.addr[0] = (( CharToHex(&broadcast_Addr[10]) & 0x0F ) << 4 ) + ( CharToHex(&broadcast_Addr[11]) & 0x0F );//0x99;
	broadcast_set_time.addr[1] = (( CharToHex(&broadcast_Addr[8]) & 0x0F ) << 4 ) + (CharToHex(&broadcast_Addr[9]) & 0x0F );//0x99;
	broadcast_set_time.addr[2] = (( CharToHex(&broadcast_Addr[6]) & 0x0F ) << 4 ) + ( CharToHex(&broadcast_Addr[7]) & 0x0F );//0x99;
	broadcast_set_time.addr[3] = (( CharToHex(&broadcast_Addr[4]) & 0x0F ) << 4 ) + ( CharToHex(&broadcast_Addr[5]) & 0x0F );//0x99;
	broadcast_set_time.addr[4] = (( CharToHex(&broadcast_Addr[2]) & 0x0F ) << 4 ) + ( CharToHex(&broadcast_Addr[3]) & 0x0F );//0x99;
	broadcast_set_time.addr[5] = (( CharToHex(&broadcast_Addr[0]) & 0x0F ) << 4 ) + ( CharToHex(&broadcast_Addr[1]) & 0x0F );//0x99;
	broadcast_set_time.head2=0x68;
	broadcast_set_time.cmd=0x08;
	broadcast_set_time.len=0x06;
	broadcast_set_time.Second= broadcast_set_time.Second+0x33;
	broadcast_set_time.Minute= broadcast_set_time.Minute+0x33;
	broadcast_set_time.Hour  = broadcast_set_time.Hour+0x33;
	broadcast_set_time.Day   = broadcast_set_time.Day+0x33;
	broadcast_set_time.Month = broadcast_set_time.Month+0x33;
	broadcast_set_time.Year  = broadcast_set_time.Year+0x33;
	broadcast_set_time.crc=global_sum_array((char*)&broadcast_set_time.head1,sizeof(broadcast_time_t)-5,IS_CHAR);
	broadcast_set_time.end=0x16;
	app_RMmod_scia_send(&broadcast_set_time, sizeof(broadcast_time_t));
	msleep(20);
	app_RMmod_rev_tip_display("�㲥��ʱ���!");
	return;
}

/********************************************************************
 *@���� :���ı�
 *@���� :�ǹ㲥97��Լ�ĶԶ๦�ܵ����ж�ʱ(ʱ����,��У����ʱ��Ϊ׼)
 *@���� :NONE
 *@��� :NONE
 
 //Ȩ�޵�һλ,����������(˵��:��������Ϊ12 34 56 78  ,����ʱ��:12 78 56 34)

********************************************************************/
void app_RMmod_time_hms_97_set(void)
{
	send_hour_97_t  set_time={0,};
	app_RMmod_time_obtain(&m_time_buf);
	set_time.head[0]=0xFE;
	set_time.head[1]=0xFE;
	set_time.head[2]=0xFE;
	set_time.head1=0x68;	
	set_time.addr[0] = (( CharToHex(&m_meter_addr[10]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[11]) & 0x0F );//0x99;
	set_time.addr[1] = (( CharToHex(&m_meter_addr[8]) & 0x0F ) << 4 ) + (CharToHex(& m_meter_addr[9]) & 0x0F );//0x99;
	set_time.addr[2] = (( CharToHex(&m_meter_addr[6]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[7]) & 0x0F );//0x99;
	set_time.addr[3] = (( CharToHex(&m_meter_addr[4]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[5]) & 0x0F );//0x99;
	set_time.addr[4] = (( CharToHex(&m_meter_addr[2]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[3]) & 0x0F );//0x99;
	set_time.addr[5] = (( CharToHex(&m_meter_addr[0]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[1]) & 0x0F );//0x99;
	set_time.head2=0x68;
	set_time.cmd=0x04;
	set_time.len=0x09;	
	set_time.biaoshi_L=0x11+0x33;
	set_time.biaoshi_H=0xc0+0x33;
	//Ȩ�޵�һλ,����������(˵��:��������Ϊ12 34 56 78  ,����ʱ��:12 78 56 34)
	set_time.password[0] =0x33+ (( CharToHex(&m_meter_Password[0]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_Password[1]) & 0x0F );
	set_time.password[1] =0x33+ (( CharToHex(&m_meter_Password[6]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_Password[7]) & 0x0F );
	set_time.password[2] =0x33+ (( CharToHex(&m_meter_Password[4]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_Password[5]) & 0x0F );
	set_time.password[3] =0x33+ (( CharToHex(&m_meter_Password[2]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_Password[3]) & 0x0F );
	
	set_time.Second=m_time_buf.sec+0x33;
	set_time.Minute=m_time_buf.min+0x33;
	set_time.Hour=m_time_buf.hour+0x33;
	set_time.crc=global_sum_array((char*)&set_time.head1,sizeof(send_hour_97_t)-5,IS_CHAR);
	set_time.end=0x16;
	app_RMmod_scia_send((char*)&set_time,sizeof(send_hour_97_t));
	return;
}
/********************************************************************
 *@���� :���ı�
 *@���� :�ǹ㲥97��Լ�ĶԶ๦�ܵ����ж�ʱ(��������,��У����ʱ��Ϊ׼)
 *@���� :NONE
 *@��� :NONE
********************************************************************/
void app_RMmod_time_ymdw_97_set(void)
{
	send_date_97_t  set_date_time={0,};
	app_RMmod_time_obtain(&m_time_buf);
	set_date_time.head[0]=0xFE;
	set_date_time.head[1]=0xFE;
	set_date_time.head[2]=0xFE;
	set_date_time.head1=0x68;	
	set_date_time.addr[0] = (( CharToHex(&m_meter_addr[10]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[11]) & 0x0F );//0x99;
	set_date_time.addr[1] = (( CharToHex(&m_meter_addr[8]) & 0x0F ) << 4 ) + (CharToHex(& m_meter_addr[9]) & 0x0F );//0x99;
	set_date_time.addr[2] = (( CharToHex(&m_meter_addr[6]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[7]) & 0x0F );//0x99;
	set_date_time.addr[3] = (( CharToHex(&m_meter_addr[4]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[5]) & 0x0F );//0x99;
	set_date_time.addr[4] = (( CharToHex(&m_meter_addr[2]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[3]) & 0x0F );//0x99;
	set_date_time.addr[5] = (( CharToHex(&m_meter_addr[0]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[1]) & 0x0F );//0x99;
	set_date_time.head2=0x68;
	set_date_time.cmd=0x04;
	set_date_time.len=0x0A;	
	set_date_time.biaoshi_L=0x10+0x33;
	set_date_time.biaoshi_H=0xc0+0x33;
	//Ȩ�޵�һλ,����������(˵��:��������Ϊ12 34 56 78  ,����ʱ��:12 78 56 34)
	set_date_time.password[0] =0x33+ (( CharToHex(&m_meter_Password[0]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_Password[1]) & 0x0F );
	set_date_time.password[1] =0x33+ (( CharToHex(&m_meter_Password[6]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_Password[7]) & 0x0F );
	set_date_time.password[2] =0x33+ (( CharToHex(&m_meter_Password[4]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_Password[5]) & 0x0F );
	set_date_time.password[3] =0x33+ (( CharToHex(&m_meter_Password[2]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_Password[3]) & 0x0F );
	set_date_time.week=m_time_buf.week+0x33;
	set_date_time.Day=m_time_buf.day+0x33;
	set_date_time.Month=m_time_buf.month+0x33;
	set_date_time.Year=m_time_buf.year+0x33;
	set_date_time.crc=global_sum_array((char*)&set_date_time.head1,sizeof(send_date_97_t)-5,IS_CHAR);
	set_date_time.end=0x16;
	app_RMmod_scia_send((char*)&set_date_time,sizeof(send_date_97_t));
	return;
}
/********************************************************************
 *@���� :���ı�
 *@���� :�ǹ㲥07��Լ�ĶԶ๦�ܵ����ж�ʱ(ʱ����,��У����ʱ��Ϊ׼)
 *@���� :NONE
 *@��� :NONE
********************************************************************/
void app_RMmod_time_hms_07_set(void)
{
	send_hour_07_t  set_time={0,};
	app_RMmod_time_obtain(&m_time_buf);  //�������ʱ��
	set_time.head[0]=0xFE;
	set_time.head[1]=0xFE;
	set_time.head[2]=0xFE;
	set_time.head1=0x68;	
	set_time.addr[0] = (( CharToHex(&m_meter07_addr[10]) & 0x0F ) << 4 )+ ( CharToHex(&m_meter07_addr[11]) & 0x0F );
	set_time.addr[1] = (( CharToHex(&m_meter07_addr[8]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[9]) & 0x0F );
	set_time.addr[2] = (( CharToHex(&m_meter07_addr[6]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[7]) & 0x0F );
	set_time.addr[3] = (( CharToHex(&m_meter07_addr[4]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[5]) & 0x0F );
	set_time.addr[4] = (( CharToHex(&m_meter07_addr[2]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[3]) & 0x0F );
	set_time.addr[5] = (( CharToHex(&m_meter07_addr[0]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[1]) & 0x0F );
	set_time.head2=0x68;
	set_time.cmd=0x14;
	set_time.len=15;
	set_time.biaoshi[0]=0x02+0x33;
	set_time.biaoshi[1]=0x01+0x33;
	set_time.biaoshi[2]=0x00+0x33;
	set_time.biaoshi[3]=0x04+0x33;
	//Ȩ�޵�һλ,����������(˵��:��������Ϊ12 34 56 78  ,����ʱ��:12 78 56 34)
	set_time.password[0] =0x33+ (( CharToHex(&m_meter_Password[0]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_Password[1]) & 0x0F );
	set_time.password[1] =0x33+ (( CharToHex(&m_meter_Password[6]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_Password[7]) & 0x0F );
	set_time.password[2] =0x33+ (( CharToHex(&m_meter_Password[4]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_Password[5]) & 0x0F );
	set_time.password[3] =0x33+ (( CharToHex(&m_meter_Password[2]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_Password[3]) & 0x0F );
	set_time.user_number[0]=0x00+0x33;
	set_time.user_number[1]=0x00+0x33;
	set_time.user_number[2]=0x00+0x33;
	set_time.user_number[3]=0x00+0x33;
	set_time.Second=m_time_buf.sec+0x33;
	set_time.Minute=m_time_buf.min+0x33;
	set_time.Hour=m_time_buf.hour+0x33;
	set_time.crc=global_sum_array((char*)&set_time.head1,sizeof(send_hour_07_t)-5,IS_CHAR);
	set_time.end=0x16;
	app_RMmod_scia_send((char*)&set_time,sizeof(send_hour_07_t));
	return;
}
/********************************************************************
 *@���� :���ı�
 *@���� :�ǹ㲥07��Լ�ĶԵ����ж�ʱ(��������,��У����ʱ��Ϊ׼)
 *@���� :NONE
 *@��� :NONE
********************************************************************/
void app_RMmod_time_ymdw_07_set(void)
{
	send_date_07_t  set_date_time={0,};
	app_RMmod_time_obtain(&m_time_buf);
	set_date_time.head[0]=0xFE;
	set_date_time.head[1]=0xFE;
	set_date_time.head[2]=0xFE;
	set_date_time.head1=0x68;	
	set_date_time.addr[0] = (( CharToHex(&m_meter07_addr[10]) & 0x0F ) << 4 )+ ( CharToHex(&m_meter07_addr[11]) & 0x0F );
	set_date_time.addr[1] = (( CharToHex(&m_meter07_addr[8]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[9]) & 0x0F );
	set_date_time.addr[2] = (( CharToHex(&m_meter07_addr[6]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[7]) & 0x0F );
	set_date_time.addr[3] = (( CharToHex(&m_meter07_addr[4]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[5]) & 0x0F );
	set_date_time.addr[4] = (( CharToHex(&m_meter07_addr[2]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[3]) & 0x0F );
	set_date_time.addr[5] = (( CharToHex(&m_meter07_addr[0]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[1]) & 0x0F );
	set_date_time.head2=0x68;
	set_date_time.cmd=0x14;
	set_date_time.len=16;
	set_date_time.biaoshi[0]=0x01+0x33;
	set_date_time.biaoshi[1]=0x01+0x33;
	set_date_time.biaoshi[2]=0x00+0x33;
	set_date_time.biaoshi[3]=0x04+0x33;
	//Ȩ�޵�һλ,����������(˵��:��������Ϊ12 34 56 78  ,����ʱ��:12 78 56 34)
	set_date_time.password[0] =0x33+ (( CharToHex(&m_meter_Password[0]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_Password[1]) & 0x0F );
	set_date_time.password[1] =0x33+ (( CharToHex(&m_meter_Password[6]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_Password[7]) & 0x0F );
	set_date_time.password[2] =0x33+ (( CharToHex(&m_meter_Password[4]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_Password[5]) & 0x0F );
	set_date_time.password[3] =0x33+ (( CharToHex(&m_meter_Password[2]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_Password[3]) & 0x0F );
	set_date_time.user_number[0]=0x00+0x33;
	set_date_time.user_number[1]=0x00+0x33;
	set_date_time.user_number[2]=0x00+0x33;
	set_date_time.user_number[3]=0x00+0x33;
	set_date_time.week=m_time_buf.week+0x33;
	set_date_time.Day=m_time_buf.day+0x33;
	set_date_time.Month=m_time_buf.month+0x33;
	set_date_time.Year=m_time_buf.year+0x33;
	set_date_time.crc=global_sum_array((char*)&set_date_time.head1,sizeof(send_date_07_t)-5,IS_CHAR);
	set_date_time.end=0x16;
	app_RMmod_scia_send((char*)&set_date_time,sizeof(send_date_07_t));
	return;
}
/********************************************************************
* @���� :���ı�
* @���� :����Уʱ���
* @���� :NONE
* @��� :NONE
********************************************************************/
/*void app_RMmod_set_time_by_command_entry(void)
{
	hand_wirte_input_box_pos_set(20,35);
	if(MsgPasswordBox_input(250,180, 300, 200,"����Уʱ")==TURE)
	{
		if(m_meter_data.guiyue==0)//97
		{
			app_RMmod_time_ymdw_97_set();
			msleep(80);
			app_RMmod_time_hms_97_set();
			msleep(50);
			app_RMmod_rev_tip_display("ʱ���������!");
			m_RM_rev_flag=FAIL;

		}
		else if(m_meter_data.guiyue==1)// 07
		{	
			app_RMmod_time_ymdw_07_set();
			msleep(80);
			app_RMmod_time_hms_07_set();
			msleep(50);
			app_RMmod_rev_tip_display("ʱ���������!");
			m_RM_rev_flag=FAIL;
		}
	}
	return;
}*/

/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :�㲥Уʱʱ���ж�
* @���� :NONE
* @��� :������������1,���򷵻�0
********************************************************************/
void app_BCDconvertHEX( uint8_t *m_des,uint8_t *m_src)
{
	uint8_t m_mod = 0;
	uint8_t m_sec = 0;
	m_mod = *m_src / 16;
	m_sec = *m_src % 16;
	*m_des = m_mod * 10 + m_sec;
	return;
}

uint8_t app_RMmod_broadcast_time_lag_judge( void )
{
	uint8_t m_ret = 0;
	uint8_t m_hour = 0;
	uint8_t m_min = 0;
	uint8_t m_sec = 0;
	float   m_instrument_second = 0;
	float   m_meter_second = 0;
	float   m_D_value_second = 0;
	if ( (m_meter_time_buf.year  == m_time_buf.year) && 
		 (m_meter_time_buf.month == m_time_buf.month) &&
		 (m_meter_time_buf.day   == m_time_buf.day) )
	{
		app_BCDconvertHEX(&m_hour,&m_meter_time_buf.hour);
		app_BCDconvertHEX(&m_min,&m_meter_time_buf.min);
		app_BCDconvertHEX(&m_sec,&m_meter_time_buf.sec);
		m_meter_second = (m_hour * 3600.0) + (m_min * 60.0) + m_sec;
		app_BCDconvertHEX(&m_hour,&m_time_buf.hour);
		app_BCDconvertHEX(&m_min,&m_time_buf.min);
		app_BCDconvertHEX(&m_sec,&m_time_buf.sec);
		m_instrument_second = (m_hour * 3600.0) + (m_min * 60.0) + m_sec;
		if ( m_instrument_second >= m_meter_second )
			 m_D_value_second = m_instrument_second - m_meter_second;
		else m_D_value_second = m_meter_second - m_instrument_second;
		if ( m_D_value_second < 300 )	m_ret = 1;
	}
	
	return m_ret;
}


/********************************************************************
* @���� :�ܱ�Ȩ
* @���� :�㲥Уʱ
* @���� :NONE
* @��� :NONE
********************************************************************/
void app_RMmod_broadcast_CheckTime_handle(void)
{
	int8_t broadcast_Addr[20] = {'9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', 0};
	broadcast_time_t  broadcast_set_time={0,};
	if((m_meter_data.guiyue!= 0) && (m_meter_data.guiyue!= 1))  //97��07Э��
		return;
	app_RMmod_time_obtain(&m_time_buf);
	
	broadcast_set_time.Second= m_time_buf.sec;
	broadcast_set_time.Minute= m_time_buf.min;
	broadcast_set_time.Hour  = m_time_buf.hour;
	broadcast_set_time.Day   = m_time_buf.day;
	broadcast_set_time.Month = m_time_buf.month;
	broadcast_set_time.Year  = m_time_buf.year;
	
	broadcast_set_time.head[0]=0xFE;
	broadcast_set_time.head[1]=0xFE;
	broadcast_set_time.head[2]=0xFE;
	broadcast_set_time.head1=0x68;	
	broadcast_set_time.addr[0] = (( CharToHex(&broadcast_Addr[10]) & 0x0F ) << 4 ) + ( CharToHex(&broadcast_Addr[11]) & 0x0F );//0x99;
	broadcast_set_time.addr[1] = (( CharToHex(&broadcast_Addr[8]) & 0x0F ) << 4 ) + (CharToHex(&broadcast_Addr[9]) & 0x0F );//0x99;
	broadcast_set_time.addr[2] = (( CharToHex(&broadcast_Addr[6]) & 0x0F ) << 4 ) + ( CharToHex(&broadcast_Addr[7]) & 0x0F );//0x99;
	broadcast_set_time.addr[3] = (( CharToHex(&broadcast_Addr[4]) & 0x0F ) << 4 ) + ( CharToHex(&broadcast_Addr[5]) & 0x0F );//0x99;
	broadcast_set_time.addr[4] = (( CharToHex(&broadcast_Addr[2]) & 0x0F ) << 4 ) + ( CharToHex(&broadcast_Addr[3]) & 0x0F );//0x99;
	broadcast_set_time.addr[5] = (( CharToHex(&broadcast_Addr[0]) & 0x0F ) << 4 ) + ( CharToHex(&broadcast_Addr[1]) & 0x0F );//0x99;
	broadcast_set_time.head2=0x68;
	broadcast_set_time.cmd=0x08;
	broadcast_set_time.len=0x06;
	broadcast_set_time.Second= broadcast_set_time.Second+0x33;
	broadcast_set_time.Minute= broadcast_set_time.Minute+0x33;
	broadcast_set_time.Hour  = broadcast_set_time.Hour+0x33;
	broadcast_set_time.Day   = broadcast_set_time.Day+0x33;
	broadcast_set_time.Month = broadcast_set_time.Month+0x33;
	broadcast_set_time.Year  = broadcast_set_time.Year+0x33;
	broadcast_set_time.crc=global_sum_array((char*)&broadcast_set_time.head1,sizeof(broadcast_time_t)-5,IS_CHAR);
	broadcast_set_time.end=0x16;
	app_RMmod_scia_send(&broadcast_set_time, sizeof(broadcast_time_t));
	msleep(20);
	if ( app_RMmod_broadcast_time_lag_judge() )
	{
			app_RMmod_rev_tip_display("�㲥Уʱ���!");
			GUI_3D_window_title_mod(m_RMwin_pWIN,"�ɹ��Ե��ܱ���й㲥Уʱ!");
	}
	else
	{
		app_RMmod_rev_tip_display("�㲥Уʱʧ��!");
		GUI_3D_window_title_mod(m_RMwin_pWIN,"������������ܱ�ʱ���������5����,�㲥Уʱʧ��!");
	}
	return;
}


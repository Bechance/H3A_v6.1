/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:读表校时程序
* 创建时间:2013-07-02
* 所属公司 :科荟
* 文件名称:app_RM_CheckTime.c
* 作者 :周炳权
* 功能描述:实现读表校时功能
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
	GUI_edit_memeber_add( 5, 5, 8, "请输入密码:", MEMBER_3D | 1 );
	GUI_window_hwin_release( edit );
	g_key_input_keep_secret = FAIL; /*设置成不保密输入模式*/
	num = GUI_edit_start( edit, MsgPasswordBox_edit_callback_input );
	g_key_input_keep_secret = FAIL;
	return num;
}

/********************************************************************
 * @作者 :
 * @功能 :密码提示对话框
 * @输入 :(x,y)坐标
 *@输出  :NONE
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
 *@作者 :范文斌
 *@功能 :645广播校时()
 *@实现方式:先读取电表时间，雨校验仪时间比较,若相差4.8分钟内,直接以校验仪时间作为广播校时.
 		 若相差大于4.8分钟,则按电表时间,向检验仪时间靠近4.8分钟对时,
 		 如:电表的时间为16:25:00时,校验仪的时间为16:45:34时,则校对时间为16:29:48时.
 *@输入 :NONE
 *@输出 :NONE   
********************************************************************/

/********************************************************************
 *@作者 :范文斌
 *@功能 :
 *@输入 :NONE
 *@输出 :*flag=0;表数据等于仪器数据
 		 *flag=1;表数据大于仪器数据
 		 *flag=2;表数据小于仪器数据
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
 *@作者 :范文斌
 *@功能 :
 *@输入 :NONE
 *@输出 :1 或2表示相差大于等于4.8分钟，0表示小于4.8分钟,
 
 		 *flag=0;表数据等于仪器数据
 		 *flag=1;表数据大于仪器数据
 		 *flag=2;表数据小于仪器数据
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
	meter_time=meter_time_minute*60+meter_time_second; //分钟加秒钟
	eq_time=eq_time_minute*60+eq_time_second;
	if(GLOBAL_ABS(meter_time-eq_time)>(4*60+48))  //(4*60+48)为4分48秒
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
 * @作者 :范文斌
 * @功能 :获取时间BUF
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_RMmod_time_obtain( time_val_t*ptime )
{
	os_driver_ioctl(g_timer_dev_id,TMR_CMD_TIME_VALUE_GET,ptime );
	return;
}
/********************************************************************
 * @作者 :范文斌
 * @功能 :发送数据
 * @输入 :*data数据  len长度
 * @输出 :NONE
********************************************************************/
void app_RMmod_scia_send(void *data,Bsize_t len)
{
	os_driver_write( g_usart3_dev_id, data,len, 0 );
	return;
}

/********************************************************************
 *@作者 :范文斌
 *@功能 :广播645(97 ,07)规约的对多功能电表进行对时
 *@输入 :NONE
 *@输出 :NONE 
 *@实现方式:先读取电表时间，雨校验仪时间比较,若相差4.8分钟内,直接以校验仪时间作为广播校时.
 		 若相差大于4.8分钟,则按电表时间,向检验仪时间靠近4.8分钟对时,
 		 如:电表的时间为16:25:00时,校验仪的时间为16:45:34时,则校对时间为16:29:48时.

********************************************************************/
void app_RMmod_broadcast_send_time_data(void)
{
	uint8_t flag=0;
	uint8_t back_data=0;
	uint8_t temp[6]={0,};
	int8_t broadcast_Addr[20] = {'9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', 0};
	broadcast_time_t  broadcast_set_time={0,};
	if(m_meter_data.guiyue!= 0||m_meter_data.guiyue!= 1)  //97或07协议
		return;
	app_RMmod_time_obtain(&m_time_buf);
	if(app_RMmod_broadcast_time_handle(&flag)==1)   //相差大于等于4.8分钟
	{
		bcd_convert_hex(m_meter_time_buf.sec,&temp[0]);
		bcd_convert_hex(m_meter_time_buf.min,&temp[1]);
		bcd_convert_hex(m_meter_time_buf.hour,&temp[2]);
		bcd_convert_hex(m_meter_time_buf.day,&temp[3]);
		bcd_convert_hex(m_meter_time_buf.month,&temp[4]);
		bcd_convert_hex(m_meter_time_buf.year,&temp[5]);
		if(flag==1)   //表数据减去4.8分钟
		{
			back_data=app_RMmod_broadcast_time_subb(temp[0],&broadcast_set_time.Second,48-20,60);
			back_data=app_RMmod_broadcast_time_subb(temp[1],&broadcast_set_time.Minute,4+back_data,60);
			back_data=app_RMmod_broadcast_time_subb(temp[2],&broadcast_set_time.Hour,back_data,24);
			back_data=app_RMmod_broadcast_time_subb(temp[3],&broadcast_set_time.Day,back_data,31);
			back_data=app_RMmod_broadcast_time_subb(temp[4],&broadcast_set_time.Month,back_data,12);
			back_data=app_RMmod_broadcast_time_subb(temp[5],&broadcast_set_time.Year,back_data,100);
		}
		else if(flag==2)//表数据加上4.8分钟
		{
			back_data=app_metermod_broadcast_time_add(temp[0],&broadcast_set_time.Second,48,60);
			back_data=app_metermod_broadcast_time_add(temp[1],&broadcast_set_time.Minute,4+back_data,60);
			back_data=app_metermod_broadcast_time_add(temp[2],&broadcast_set_time.Hour,back_data,24);
			back_data=app_metermod_broadcast_time_add(temp[3],&broadcast_set_time.Day,back_data,32);
			back_data=app_metermod_broadcast_time_add(temp[4],&broadcast_set_time.Month,back_data,13);
			back_data=app_metermod_broadcast_time_add(temp[5],&broadcast_set_time.Year,back_data,100);
		}
	}
	else							//相差小于4.8分钟
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
	app_RMmod_rev_tip_display("广播对时完成!");
	return;
}

/********************************************************************
 *@作者 :范文斌
 *@功能 :非广播97规约的对多功能电表进行对时(时分秒,以校验仪时间为准)
 *@输入 :NONE
 *@输出 :NONE
 
 //权限第一位,接着是密码(说明:比如输入为12 34 56 78  ,则发送时是:12 78 56 34)

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
	//权限第一位,接着是密码(说明:比如输入为12 34 56 78  ,则发送时是:12 78 56 34)
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
 *@作者 :范文斌
 *@功能 :非广播97规约的对多功能电表进行对时(年月日周,以校验仪时间为准)
 *@输入 :NONE
 *@输出 :NONE
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
	//权限第一位,接着是密码(说明:比如输入为12 34 56 78  ,则发送时是:12 78 56 34)
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
 *@作者 :范文斌
 *@功能 :非广播07规约的对多功能电表进行对时(时分秒,以校验仪时间为准)
 *@输入 :NONE
 *@输出 :NONE
********************************************************************/
void app_RMmod_time_hms_07_set(void)
{
	send_hour_07_t  set_time={0,};
	app_RMmod_time_obtain(&m_time_buf);  //获得仪器时间
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
	//权限第一位,接着是密码(说明:比如输入为12 34 56 78  ,则发送时是:12 78 56 34)
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
 *@作者 :范文斌
 *@功能 :非广播07规约的对电表进行对时(年月日周,以校验仪时间为准)
 *@输入 :NONE
 *@输出 :NONE
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
	//权限第一位,接着是密码(说明:比如输入为12 34 56 78  ,则发送时是:12 78 56 34)
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
* @作者 :范文斌
* @功能 :命令校时入口
* @输入 :NONE
* @输出 :NONE
********************************************************************/
/*void app_RMmod_set_time_by_command_entry(void)
{
	hand_wirte_input_box_pos_set(20,35);
	if(MsgPasswordBox_input(250,180, 300, 200,"命令校时")==TURE)
	{
		if(m_meter_data.guiyue==0)//97
		{
			app_RMmod_time_ymdw_97_set();
			msleep(80);
			app_RMmod_time_hms_97_set();
			msleep(50);
			app_RMmod_rev_tip_display("时间设置完成!");
			m_RM_rev_flag=FAIL;

		}
		else if(m_meter_data.guiyue==1)// 07
		{	
			app_RMmod_time_ymdw_07_set();
			msleep(80);
			app_RMmod_time_hms_07_set();
			msleep(50);
			app_RMmod_rev_tip_display("时间设置完成!");
			m_RM_rev_flag=FAIL;
		}
	}
	return;
}*/

/********************************************************************
* @作者 :周炳权
* @功能 :广播校时时差判断
* @输入 :NONE
* @输出 :符合条件返回1,否则返回0
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
* @作者 :周炳权
* @功能 :广播校时
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void app_RMmod_broadcast_CheckTime_handle(void)
{
	int8_t broadcast_Addr[20] = {'9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', 0};
	broadcast_time_t  broadcast_set_time={0,};
	if((m_meter_data.guiyue!= 0) && (m_meter_data.guiyue!= 1))  //97或07协议
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
			app_RMmod_rev_tip_display("广播校时完成!");
			GUI_3D_window_title_mod(m_RMwin_pWIN,"成功对电能表进行广播校时!");
	}
	else
	{
		app_RMmod_rev_tip_display("广播校时失败!");
		GUI_3D_window_title_mod(m_RMwin_pWIN,"由于仪器与电能表时差大于正负5分钟,广播校时失败!");
	}
	return;
}


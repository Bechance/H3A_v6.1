#include 	"../USB_connect.h"
#include 	"../../includes.h"
#define 				GET_FILE_CTL()  			  	(&m_fp_ctl);
#define  		 		USB_WATI_CLK()					{while (GpioDataRegs.GPBDAT.bit.GPIO61==1);while(GpioDataRegs.GPBDAT.bit.GPIO61==0);}

#define                 SEND_WRITE_INFO_TO_PIC()        (GpioDataRegs.GPBSET.bit.GPIO60=1)
#define                 CLOSE_WRITE_INFO_TO_PIC()       (GpioDataRegs.GPBCLEAR.bit.GPIO60=1)

#define                 PIC_BUS_ADDR        		  	 0x4100
#define 				MAX_USBREV_FIFO_SIZE           	 2048

USB_file_ctl_t 			m_fp_ctl;
USB_dev_state_u		    _USBstate ;
USB_dev_state_u*		pUSBstate = (USB_dev_state_u*)0x4112;
uint8_t      			m_USB_rev_fifo[MAX_USBREV_FIFO_SIZE] = {0, };			//USBFIFO
fifo_tcb_t   			m_USB_fifo_tcb_body = {0, };  			//usb控制TCB
fifo_tcb_t*   			m_USB_fifo_tcb = &m_USB_fifo_tcb_body;  //usb控制TCB
uint8_t      			m_HID_rev_fifo[50] = {0, };				//HIDFIFO
fifo_tcb_t   			m_HID_fifo_tcb_body = {0, };  			//HID控制TCB
fifo_tcb_t*   			m_HID_fifo_tcb = &m_HID_fifo_tcb_body;  //HID控制TCB
uint8_t      			m_GPS_rev_fifo[50] = {0, };				//GPSFIFO
fifo_tcb_t   			m_GPS_fifo_tcb_body = {0, };  			//GPS控制TCB
fifo_tcb_t*   			m_GPS_fifo_tcb = &m_GPS_fifo_tcb_body;  //GPS控制TCB
extern                  BOOL              m_write_is_finish;
	
void USB_disk_dev_connect(void)
{
 //  *pUSBstate = IS_USB_DISK_DEV_CONNECT;
   return;
}
void USB_HID_dev_connect(void)
{
//   *pUSBstate = IS_USB_HID_DEV_CONNECT;
   return;
}
void USB_dev_move(void)
{
 //  *pUSBstate = USB_DEV_IS_IDLE;
   return;
}
/********************************************************************
 * @创建人:揭成
 * @功能  :USB数据压栈
 * @输入  :NONE
 * @输出  :NONE
********************************************************************/
void USBFS_data_ISR(uint8_t*data)
{
	if(*pUSBstate != IS_USB_DISK_DEV_CONNECT) return ;
    fifo_buffer_push(m_USB_fifo_tcb,data,4);/*将接收到的数据装进FIFO*/
    return;
}
/********************************************************************
 * @创建人:揭成
 * @功能  :HID数据压栈
 * @输入  :NONE
 * @输出  :NONE
********************************************************************/
void USBhid_data_ISR(uint8_t*data)
{
	if(*pUSBstate != IS_USB_HID_DEV_CONNECT) return ;
    fifo_buffer_push(m_HID_fifo_tcb,data,4);/*将接收到的数据装进FIFO*/
    return;
}
/********************************************************************
 * @创建人:周炳权
 * @功能  :HID数据压栈
 * @输入  :NONE
 * @输出  :NONE
********************************************************************/
void GPS_data_ISR(uint8_t*data)
{
    fifo_buffer_push(m_GPS_fifo_tcb,data,4);/*将接收到的数据装进FIFO*/
    return;
}
/********************************************************************
 * @创建人:揭成
 * @功能  :读取FIFO数据
 * @输入  :NONE
 * @输出  :NONE
********************************************************************/
static Bsize_t USB_read_FIFO(fifo_tcb_t*pfifo,uint8_t*buf,Bsize_t size)
{
	Bsize_t real_size = 0;
	Bsize_t times;
	real_size = fifo_buffer_get_size( pfifo );
	if ( real_size < size )
	{
	    times = (size-real_size+299)/300;
		msleep(times);
		real_size	= fifo_buffer_get_size( pfifo );
		if ( real_size == 0 )
		{
			return 0; 
		}
	}
	if ( real_size > size )
	{
		real_size = size;
	}
	fifo_buffer_pop( pfifo, buf, real_size );
	return real_size;
}
/********************************************************************
 * @创建人:揭成
 * @功能  :往总线发送USB数据
 * @输入  :NONE
 * @输出  :NONE
********************************************************************/
static void USBFS_data_send(void*head,uint32_t lenth)
{
	uint16_t offset = 0;
	uint8_t* pdata=(uint8_t*)head; 
	DINT;
	SEND_WRITE_INFO_TO_PIC();
	for ( offset = 0;offset < lenth;offset+=4)
	{
		*( uint16_t* )(PIC_BUS_ADDR+0) = pdata[offset];
		*( uint16_t* )(PIC_BUS_ADDR+1) = pdata[offset+1];
		*( uint16_t* )(PIC_BUS_ADDR+2) = pdata[offset+2];
		*( uint16_t* )(PIC_BUS_ADDR+3) = pdata[offset+3];
		USB_WATI_CLK();
	}
	CLOSE_WRITE_INFO_TO_PIC();
	EINT;
    return;
}
/********************************************************************
 * @创建人:揭成
 * @功能  :往总线发送USB数据
 * @输入  :NONE
 * @输出  :NONE
********************************************************************/
static BOOL USBFS_data_read(void*pdata,uint16_t data_len)
{
   Bsize_t size;
   uint8_t end[2];
   fifo_tcb_t*ptcp = m_USB_fifo_tcb;
   size=USB_read_FIFO(ptcp,pdata,data_len);
   if(size!=data_len)return FAIL;
   size=USB_read_FIFO(ptcp,end,2);
   if(size!=2)return FAIL;
    return TURE;
}	
/********************************************************************
 * @创建人:揭成
 * @功能  :等待数据
 * @输入  :NONE
 * @输出  :NONE
********************************************************************/
static BOOL USBFS_wait_data(USB_pact_head_t*phead,uint16_t *datalen)
{
    int timeout=0;
//	Bsize_t size;
    uint16_t data_len;
	fifo_tcb_t*ptcp = m_USB_fifo_tcb;
    while(++timeout<500)
    {
   		if(!USB_read_FIFO(ptcp,(uint8_t*)phead,1))continue;
        if(phead->head_1!=0x68)continue;
   		if(!USB_read_FIFO(ptcp,(uint8_t*)&phead->head_2,1))continue;
        if(phead->head_2!=0x86)continue;
        USB_read_FIFO(ptcp,(uint8_t*)&phead->cmd,sizeof(USB_pact_head_t)-2);
		data_len = phead->data_len_h*0x100+phead->data_len_l;
		*datalen = data_len;
		return TURE;
	}
    return FAIL;
}
/********************************************************************
 * @创建人:揭成
 * @功能  :打开文件
 * @输入  :NONE
 * @输出  :读到的数据长度
********************************************************************/
uint8_t USBFS_fopen(char*file, char mode)
{
    USB_pact_head_t*phead;
	uint8_t*pdata;
	uint8_t crc =0;
	uint8_t data[4];
	uint16_t data_len;
	uint32_t fsize =0;
	if(*pUSBstate != IS_USB_DISK_DEV_CONNECT) return 0;
	if(m_fp_ctl.state) return 0;
	phead 				= app_malloc(1024);
	data_len            = global_strlen(file);
	phead->head_1 		= 0x68;
	phead->head_2 		= 0x86;
	phead->cmd    		= PIC_CMD_OPEN_FILE;
	phead->write_or_read = mode;
	phead->data_len_l 	= data_len&0xff;
	phead->data_len_h 	= ((data_len&0xff00)>>8);
	pdata				= (uint8_t*)&phead->data_len_l+1;
	global_memcpy(pdata,file,data_len);
	pdata               = pdata+data_len;
	pdata[0]			= crc;
	pdata[1]			= 0x16;
	USBFS_data_send(phead,sizeof(USB_pact_head_t)+data_len+2);
    if(USBFS_wait_data(phead,&data_len))
    {
	    if(USBFS_data_read(data,data_len))
	    {
			fsize =((uint32_t)data[0]<<24)+((uint32_t)data[1]<<16)+((uint32_t)data[2]<<8)+data[3];
			global_memcpy(m_fp_ctl.file_name,(char*)file,global_strlen(file));
			m_fp_ctl.fsize = fsize;
			m_fp_ctl.fptr  = 0;
			m_fp_ctl.state = TURE;
	    }
    }
	fifo_buffer_empty(m_USB_fifo_tcb);
	app_free(phead);
	return 1;
}


uint32_t USBFS_file_size_obtain(void)
{
   return  m_fp_ctl.fsize;
}

/********************************************************************
 * @创建人:揭成
 * @功能  :关闭文件
 * @输入  :RW
 * @输出  :NONE
********************************************************************/
uint8_t USBFS_fclose(uint8_t RW)
{
    USB_pact_head_t*phead;
	uint8_t*pdata;
	uint8_t crc =0;
//	uint16_t data_len = 2;
	if(*pUSBstate != IS_USB_DISK_DEV_CONNECT) return 0;
	phead 				 = app_malloc(1024);
	phead->head_1 		 = 0x68;
	phead->head_2 		 = 0x86;
	phead->cmd    		 = PIC_CMD_RW_FILE_CLOSE;
	phead->write_or_read = RW;
	phead->data_len_l 	 = 0;
	phead->data_len_h 	 = 0;
	pdata				 = (uint8_t*)&phead->data_len_l+1;
	pdata[0]             = crc;
	pdata[1]             = 0x16;
	m_write_is_finish = FAIL;
	USBFS_data_send(phead,sizeof(USB_pact_head_t)+2);
	while(!m_write_is_finish);
    GLOBAL_MEMCLR(&m_fp_ctl, sizeof(USB_file_ctl_t));
	return  0;
}
/********************************************************************
 * @创建人 :揭成
 * @功能   :读取USB 文件内容
 * @输入   :NONE
 * @输出   :读到的数据长度
********************************************************************/
uint32_t USBFS_fread(void*pbuf,uint32_t buf_size,uint8_t *flags )
{
    USB_pact_head_t*phead;
	uint8_t*		pdata;
	uint8_t 		crc =0;
	uint16_t 		data_len = 2;
	uint16_t 		read_len;
	phead 				= app_malloc(1024);
	phead->head_1 		= 0x68;
	phead->head_2 		= 0x86;
	phead->cmd    		= PIC_CMD_RW_FILE_READY;
	phead->write_or_read = READ_PIC;
	phead->data_len_l 	= data_len&0xff;
	phead->data_len_h 	=((data_len&0xff00)>>8);
	pdata				=(uint8_t*)&phead->data_len_l+1;
	pdata[0]            =buf_size&0xff;
	pdata[1]            =((buf_size&0xff00)>>8);
	pdata[2]            =crc;
	pdata[3]            =0x16;
	USBFS_data_send(phead,sizeof(USB_pact_head_t)+data_len+2);
    if(USBFS_wait_data(phead,&data_len))
    {
        if(phead->cmd==PIC_CMD_RW_FILE_DO)
        {
		    if(USBFS_data_read(pbuf,data_len))
		    {
		        *flags = FILE_READ_DOING;
				read_len = phead->data_len_h*0x100+phead->data_len_l;
				if(read_len<buf_size) *flags = FILE_READ_FINISH;
		    }
        }
    }
	fifo_buffer_empty(m_USB_fifo_tcb);
	app_free(phead);
	return  read_len;
}
/********************************************************************
 * @创建人:揭成
 * @功能  :写USB文件
 * @输入  :NONE
 * @输出  :一共写了多少字节
********************************************************************/
uint32_t USBFS_fwrite(void*pbuf, uint32_t buf_size, uint8_t *flags )
{
    USB_pact_head_t*phead;
	uint8_t*		pdata;
	uint8_t 		crc =0;
	uint16_t 		data_len = buf_size;
  //uint8_t  		is_write[2];
	uint16_t 		write_len =0 ;
	phead 				= app_malloc(1024*5);
	phead->head_1 		= 0x68;
	phead->head_2 		= 0x86;
	phead->cmd    		= PIC_CMD_RW_FILE_DO;
	phead->write_or_read = WRITE_PIC;
	phead->data_len_l 	= data_len&0xff;
	phead->data_len_h 	= ((data_len&0xff00)>>8);
	pdata				= (uint8_t*)&phead->data_len_l+1;
    global_memcpy(pdata,pbuf,data_len);
	pdata+=data_len;
	pdata[0]            = crc;
	pdata[1]            = 0x16;
	
	m_write_is_finish = FAIL;
	USBFS_data_send(phead,sizeof(USB_pact_head_t)+data_len+2);
	while(!m_write_is_finish);
	
	app_free(phead);
	return  write_len;
}
/********************************************************************
 * @创建人:揭成
 * @功能  :获取USB HID设备数据
 * @输入  :pdata   用户数据区
 * @输出  :NONE
********************************************************************/
Bsize_t USB_HID_data_obtain(void*pdata)
{
    USB_pact_head_t 	head;
	fifo_tcb_t*			ptcp = m_HID_fifo_tcb;
	uint16_t  			data_len;
	uint8_t             data[2];
    USB_pact_head_t*	phead=&head;
	while(1)
	{
   		if(!USB_read_FIFO(ptcp,(uint8_t*)phead,1))return 0;
        if(phead->head_1!=0x68)continue;
   		if(!USB_read_FIFO(ptcp,(uint8_t*)&phead->head_2,1))continue;
        if(phead->head_2!=0x86)continue;
        USB_read_FIFO(ptcp,(uint8_t*)&phead->cmd,sizeof(USB_pact_head_t)-2);
		data_len = phead->data_len_h*0x100+phead->data_len_l;
		msleep(1);
        USB_read_FIFO(ptcp,(uint8_t*)pdata,data_len);
        USB_read_FIFO(ptcp,(uint8_t*)data,2);
		fifo_buffer_empty(ptcp);
		break;
	}
   return data_len;
}
/********************************************************************
 * @创建人:揭成
 * @功能  :USB功能初始化
 * @输入  :NONE
 * @输出  :NONE
********************************************************************/
void USBfs_init(void)
{
	EALLOW;
    GpioCtrlRegs.GPBMUX2.bit.GPIO60= 0; 
	GpioCtrlRegs.GPBDIR.bit.GPIO60 = 1;
    GpioCtrlRegs.GPBMUX2.bit.GPIO61= 0; 
	GpioCtrlRegs.GPBDIR.bit.GPIO61 = 0;
	EDIS;
	GpioDataRegs.GPBCLEAR.bit.GPIO60 = 1;
	//pUSBstate=&_USBstate;
	pUSBstate = (USB_dev_state_u*)0x4112;

    GLOBAL_MEMCLR(&m_fp_ctl, sizeof(USB_file_ctl_t));
	GLOBAL_MEMCLR(m_USB_rev_fifo, 1024);
	GLOBAL_MEMCLR(m_HID_rev_fifo, 50);
	fifo_buffer_init( m_USB_fifo_tcb, MAX_USBREV_FIFO_SIZE, m_USB_rev_fifo );
	fifo_buffer_init( m_HID_fifo_tcb, 50, m_HID_rev_fifo );
	fifo_buffer_init( m_GPS_fifo_tcb, 50, m_GPS_rev_fifo );
	return;
}

/********************************************************************
 * @创建人 :周炳权
 * @功能   :请求获取GPS时间
 * @输入   :NONE
 * @输出   :NONE
********************************************************************/
//#define PC_DATA_COM 0x07;#define GPS_DATA_COM	0x08
void app_GPS_date_and_time_req( void )
{
	uint16_t offset = 0;
	uint8_t  pdata[8]={0x68,0x86,0x08,0x01,0,0,0,0}; 
	DINT;
	SEND_WRITE_INFO_TO_PIC();
	for ( offset = 0;offset < 8;offset+=4)
	{
		*( uint16_t* )(PIC_BUS_ADDR+0) = pdata[offset];
		*( uint16_t* )(PIC_BUS_ADDR+1) = pdata[offset+1];
		*( uint16_t* )(PIC_BUS_ADDR+2) = pdata[offset+2];
		*( uint16_t* )(PIC_BUS_ADDR+3) = pdata[offset+3];
		USB_WATI_CLK();
	}
	CLOSE_WRITE_INFO_TO_PIC();
	EINT;
	return;
}
/********************************************************************
 * @创建人 :周炳权
 * @功能   :请求切换电源
 * @输入   :NONE
 * @输出   :NONE
********************************************************************/
//#define POWER_SWITCH_COM	0x09
void app_power_switch_req( void )
{
	uint16_t offset = 0;
	uint8_t  pdata[8]={0x68,0x86,0x09,0x01,0,0,0,0}; 
	DINT;
	SEND_WRITE_INFO_TO_PIC();
	for ( offset = 0;offset < 8;offset+=4)
	{
		*( uint16_t* )(PIC_BUS_ADDR+0) = pdata[offset];
		*( uint16_t* )(PIC_BUS_ADDR+1) = pdata[offset+1];
		*( uint16_t* )(PIC_BUS_ADDR+2) = pdata[offset+2];
		*( uint16_t* )(PIC_BUS_ADDR+3) = pdata[offset+3];
		USB_WATI_CLK();
	}
	CLOSE_WRITE_INFO_TO_PIC();
	EINT;
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能   :USB扫描主任务
 * @输入   :arg
 * @输出   :NONE
********************************************************************/
/*
char m_test_buf[1204];
char aaa[1204];
void  USB_main_task( void*arg )
{
	char hid[50]="";
	Bsize_t size;
	Bsize_t ioffset;
	Bsize_t offset;
	uint8_t flag;
    while(1)
    {
		msleep(10);
		if(pUSBstate == IS_USB_HID_DEV_CONNECT)
		{
			USB_HID_data_obtain(hid);
		}

	}
  
	char   name[20]="";
	global_memset(m_test_buf,'1',1024);
	m_test_buf[1023] ='\n';
	m_test_buf[1024] =0;
	while ( 1 )
	{
		msleep( APP_100_MS );

   	    if(*pUSBstate == IS_USB_DISK_DEV_CONNECT)
		{

		   for(ioffset=0;ioffset<1;ioffset++)
		  	{
	  		    GLOBAL_MEMCLR(name, 20);
                global_itoa(ioffset,name);
			    global_strcat(name,".txt");
				if(USBFS_fopen(name,WRITE_PIC)!=0)
				{
				    for(offset=0;offset<100;offset++)
				    {
						size=USBFS_fwrite(m_test_buf,512,&flag);
				    }
					USBFS_fclose(WRITE_PIC);
				}
		  	}

			if(USBFS_fopen("0.txt",READ_PIC)!=0)
			{
			   while(1)
			   {
				size=USBFS_fread(aaa,801,&flag);
				if(flag==FILE_READ_FINISH)
				{
					USBFS_fclose(READ_PIC);
					break;
				}
				}
			}

		 
			while(1);
		}
		OS_DELETE_REQ_SELF();
	}
}*/

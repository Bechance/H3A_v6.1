#ifndef     	FS_H_PEC_2011
#define      	FS_H_PEC_2011
#include 		"../usertype.h"

#define  SYNS_1   		0X68
#define  SYNS_2   		0X86
#define  READ_PIC 		1
#define  WRITE_PIC 		2
enum
{
   PIC_CMD_OPEN_FILE = 0X01,
   PIC_CMD_RW_FILE_READY,
   PIC_CMD_RW_FILE_DO,
   PIC_CMD_RW_FILE_FSEEK,
   PIC_CMD_RW_FILE_CLOSE
};


typedef struct
{
     uint8_t head_1;
     uint8_t head_2;
	 uint8_t cmd;
     uint8_t write_or_read;  //1表示读，2表示写
	 uint8_t data_len_h;
	 uint8_t data_len_l;
}USB_pact_head_t;


typedef struct
{
	char  	 file_name[20];
	uint8_t  rwmod;
	BOOL     state;
	uint32_t fsize;
	uint32_t fptr;
}USB_file_ctl_t;

typedef enum
{
   IS_USB_HID_DEV_CONNECT =0,
   IS_USB_DISK_DEV_CONNECT,
   USB_DEV_IS_IDLE
}USB_dev_state_u;


enum
{
	FILE_OPEN_FAIL = FAIL,
	FILE_OPEN_TURE = TURE,
	FILE_CLOSE_FAIL = FAIL,
	FILE_CLOSE_TURE = TURE,
	FILE_READ_DOING  =  3,
	FILE_READ_FINISH,
	FILE_READ_FAIL,
	FILE_READ_START
};
void 		FS_req( void );

void 		FS_release( void );

uint32_t 	USBFS_fread(void*pbuf, uint32_t buf_size, uint8_t *flags );

uint32_t 	USBFS_fwrite(void*pbuf, uint32_t buf_size, uint8_t *flags );

uint8_t 	USBFS_fopen(char*file, char mode);

uint8_t 	USBFS_fclose(uint8_t RW);

Bsize_t 	USB_HID_data_obtain(void*pdata);

int 		FS_fleek(int pos);

int  		FS_file_seek_obtain( void );

void 		USBfs_init(void);

void  		BSP_Dly( unsigned long	us );

extern      USB_dev_state_u*		pUSBstate;

#endif


#ifndef _SYSATA_H
#define _SYSATA_H
/****************************************************************************************/
/**	与用户硬件相关部分，用户需要改动													*/
/****************************************************************************************/

#define		MASK_DATA					(~ATA_DATA)
#define 	U16_REG_BASE_ADDR			0x4A00											/**< Register base address */

#define    	ATA_REG_DATA          	(U16_REG_BASE_ADDR + 0x00 )		/*数据寄存器*/
#define    	ATA_REG_ERR           	(U16_REG_BASE_ADDR + 0x01 )		/*读错误寄存器*/
#define    	ATA_REG_FEATURE       	(U16_REG_BASE_ADDR + 0x01 )		/*写功能寄存器*/
#define    	ATA_REG_SECCNT        	(U16_REG_BASE_ADDR + 0x02 )		/*扇区计数器*/
#define    	ATA_REG_SECTOR        	(U16_REG_BASE_ADDR + 0x03 )		/*扇区号*/
#define    	ATA_REG_CYLINDER_LOW  	(U16_REG_BASE_ADDR + 0x04 )		/*柱面低8位*/
#define    	ATA_REG_CYLINDER_HIGH 	(U16_REG_BASE_ADDR + 0x05 )		/*柱面高8位*/
#define    	ATA_REG_DEVICE_HEAD   	(U16_REG_BASE_ADDR + 0x06 )		/*选择主从,模式,磁头*/
#define    	ATA_REG_COMMAND       	(U16_REG_BASE_ADDR + 0x07 )		/*写命令寄存器*/
#define    	ATA_REG_STATUS        	(U16_REG_BASE_ADDR + 0x07 )		/*读状态寄存器*/
#define    	ATA_REG_CONTROL       	(U16_REG_BASE_ADDR + 0x0E )		/*写控制寄存器*/
#define    	ATA_REG_ASTATUS       	(U16_REG_BASE_ADDR + 0x0E ) 	/*读辅助状态寄存器*/

/*************************************************************************************************/
/*驱动与硬件对ATA设备寄存器操作接口*/
#define		GetData()			SYS_PortIn(ATA_REG_DATA)				/*读数据寄存器,与驱动接口*/
#define		SetData(x)			SYS_PortOut(ATA_REG_DATA,x)				/*写数据寄存器,与驱动接口*/

#define		GetERR()			SYS_PortIn(ATA_REG_ERR)					/*读错误寄存器,与驱动接口*/
#define		SetFeature(x)		SYS_PortOut(ATA_REG_FEATURE,x)			/*写特征寄存器,与驱动接口*/

#define		GetSECCNT()			SYS_PortIn(ATA_REG_SECCNT)				/*读扇区计数寄存器,与驱动接口*/
#define		SetSECCNT(x)		SYS_PortOut(ATA_REG_SECCNT,x)			/*写扇区计数寄存器,与驱动接口*/

#define		GetSector()			SYS_PortIn(ATA_REG_SECTOR)				/*读扇区寄存器,与驱动接口*/
#define		SetSector(x)		SYS_PortOut(ATA_REG_SECTOR,x)			/*写扇区寄存器,与驱动接口*/

#define		GetCylinderLow()	SYS_PortIn(ATA_REG_CYLINDER_LOW)		/*读柱面低8位寄存器,与驱动接口*/
#define		SetCylinderLow(x)	SYS_PortOut(ATA_REG_CYLINDER_LOW,x)		/*写柱面低8位寄存器,与驱动接口*/

#define		GetCylinderHigh()	SYS_PortIn(ATA_REG_CYLINDER_HIGH)		/*读柱面高8位寄存器,与驱动接口*/
#define		SetCylinderHigh(x)	SYS_PortOut(ATA_REG_CYLINDER_HIGH,x)	/*写柱面高8位寄存器,与驱动接口*/

#define		GetDeviceHead()		SYS_PortIn(ATA_REG_DEVICE_HEAD)			/*读设备磁头寄存器,与驱动接口*/
#define		SetDeviceHead(x)	SYS_PortOut(ATA_REG_DEVICE_HEAD,x)		/*写设备磁头寄存器,与驱动接口*/

#define		GetStatus()			SYS_PortIn(ATA_REG_STATUS)				/*读状态寄存器,与驱动接口*/
#define		SetCommand(x)		SYS_PortOut(ATA_REG_COMMAND,x)			/*写命令寄存器,与驱动接口*/

#define		GetAStatus()		SYS_PortIn(ATA_REG_ASTATUS)				/*读辅助状态寄存器,与驱动接口*/
#define		SetControl(x)		SYS_PortOut(ATA_REG_CONTROL,x)			/*写控制寄存器,与驱动接口*/


/*硬件相关函数*/
Uint16 	SYS_PortIn( Uint16 reg );						/*从ATA设备指定的寄存器读出一个字（2字节）*/
void 	SYS_PortOut( Uint16 reg, Uint16 data );		/*从ATA设备指定的寄存器写入一个字*/
void 	SYS_IdeHardReset( void );						/*硬件复位*/
Uint8 	SYS_HaveCard( void );							/*检测CF卡是否存在*/



#endif

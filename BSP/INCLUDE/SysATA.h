
#ifndef _SYSATA_H
#define _SYSATA_H
/****************************************************************************************/
/**	���û�Ӳ����ز��֣��û���Ҫ�Ķ�													*/
/****************************************************************************************/

#define		MASK_DATA					(~ATA_DATA)
#define 	U16_REG_BASE_ADDR			0x4A00											/**< Register base address */

#define    	ATA_REG_DATA          	(U16_REG_BASE_ADDR + 0x00 )		/*���ݼĴ���*/
#define    	ATA_REG_ERR           	(U16_REG_BASE_ADDR + 0x01 )		/*������Ĵ���*/
#define    	ATA_REG_FEATURE       	(U16_REG_BASE_ADDR + 0x01 )		/*д���ܼĴ���*/
#define    	ATA_REG_SECCNT        	(U16_REG_BASE_ADDR + 0x02 )		/*����������*/
#define    	ATA_REG_SECTOR        	(U16_REG_BASE_ADDR + 0x03 )		/*������*/
#define    	ATA_REG_CYLINDER_LOW  	(U16_REG_BASE_ADDR + 0x04 )		/*�����8λ*/
#define    	ATA_REG_CYLINDER_HIGH 	(U16_REG_BASE_ADDR + 0x05 )		/*�����8λ*/
#define    	ATA_REG_DEVICE_HEAD   	(U16_REG_BASE_ADDR + 0x06 )		/*ѡ������,ģʽ,��ͷ*/
#define    	ATA_REG_COMMAND       	(U16_REG_BASE_ADDR + 0x07 )		/*д����Ĵ���*/
#define    	ATA_REG_STATUS        	(U16_REG_BASE_ADDR + 0x07 )		/*��״̬�Ĵ���*/
#define    	ATA_REG_CONTROL       	(U16_REG_BASE_ADDR + 0x0E )		/*д���ƼĴ���*/
#define    	ATA_REG_ASTATUS       	(U16_REG_BASE_ADDR + 0x0E ) 	/*������״̬�Ĵ���*/

/*************************************************************************************************/
/*������Ӳ����ATA�豸�Ĵ��������ӿ�*/
#define		GetData()			SYS_PortIn(ATA_REG_DATA)				/*�����ݼĴ���,�������ӿ�*/
#define		SetData(x)			SYS_PortOut(ATA_REG_DATA,x)				/*д���ݼĴ���,�������ӿ�*/

#define		GetERR()			SYS_PortIn(ATA_REG_ERR)					/*������Ĵ���,�������ӿ�*/
#define		SetFeature(x)		SYS_PortOut(ATA_REG_FEATURE,x)			/*д�����Ĵ���,�������ӿ�*/

#define		GetSECCNT()			SYS_PortIn(ATA_REG_SECCNT)				/*�����������Ĵ���,�������ӿ�*/
#define		SetSECCNT(x)		SYS_PortOut(ATA_REG_SECCNT,x)			/*д���������Ĵ���,�������ӿ�*/

#define		GetSector()			SYS_PortIn(ATA_REG_SECTOR)				/*�������Ĵ���,�������ӿ�*/
#define		SetSector(x)		SYS_PortOut(ATA_REG_SECTOR,x)			/*д�����Ĵ���,�������ӿ�*/

#define		GetCylinderLow()	SYS_PortIn(ATA_REG_CYLINDER_LOW)		/*�������8λ�Ĵ���,�������ӿ�*/
#define		SetCylinderLow(x)	SYS_PortOut(ATA_REG_CYLINDER_LOW,x)		/*д�����8λ�Ĵ���,�������ӿ�*/

#define		GetCylinderHigh()	SYS_PortIn(ATA_REG_CYLINDER_HIGH)		/*�������8λ�Ĵ���,�������ӿ�*/
#define		SetCylinderHigh(x)	SYS_PortOut(ATA_REG_CYLINDER_HIGH,x)	/*д�����8λ�Ĵ���,�������ӿ�*/

#define		GetDeviceHead()		SYS_PortIn(ATA_REG_DEVICE_HEAD)			/*���豸��ͷ�Ĵ���,�������ӿ�*/
#define		SetDeviceHead(x)	SYS_PortOut(ATA_REG_DEVICE_HEAD,x)		/*д�豸��ͷ�Ĵ���,�������ӿ�*/

#define		GetStatus()			SYS_PortIn(ATA_REG_STATUS)				/*��״̬�Ĵ���,�������ӿ�*/
#define		SetCommand(x)		SYS_PortOut(ATA_REG_COMMAND,x)			/*д����Ĵ���,�������ӿ�*/

#define		GetAStatus()		SYS_PortIn(ATA_REG_ASTATUS)				/*������״̬�Ĵ���,�������ӿ�*/
#define		SetControl(x)		SYS_PortOut(ATA_REG_CONTROL,x)			/*д���ƼĴ���,�������ӿ�*/


/*Ӳ����غ���*/
Uint16 	SYS_PortIn( Uint16 reg );						/*��ATA�豸ָ���ļĴ�������һ���֣�2�ֽڣ�*/
void 	SYS_PortOut( Uint16 reg, Uint16 data );		/*��ATA�豸ָ���ļĴ���д��һ����*/
void 	SYS_IdeHardReset( void );						/*Ӳ����λ*/
Uint8 	SYS_HaveCard( void );							/*���CF���Ƿ����*/



#endif

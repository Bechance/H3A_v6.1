#ifndef __IDE_H
#define __IDE_H

#define NULL 0

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/***************************************************************************************/
/** ������������																	   */
/***************************************************************************************/
#define MAX_DEVICE 	2					/*�����豸��,����ֻ�����ʹ��豸,�������ֵΪ2*/
#define TIMEOUTSET		0x0005ffff		/*��ʱ��Ӧ�豸(��CPU���ٶ��й�,û��׼ȷ��ʱ��)*/


/****************************************************************************************/
/**	����Ϊ���������岿�ݣ��û�����Ķ�													*/
/****************************************************************************************/
/*ATA ָ�����*/
#define 	IDE_CMD_ATAPI_RESET      		0x08
#define 	IDE_CMD_RECALIBRATE      		0x10
#define 	IDE_CMD_READ             		0x20	// ����������
#define 	IDE_CMD_WRITE            		0x30	// д��������
#define 	IDE_CMD_VERIFY           		0x40
#define 	IDE_CMD_SEEK             		0x70
#define		IDE_CMD_EXEC_DIAGNOSE    		0x90
#define		IDE_CMD_SET_DRIVE_PARAMETERS 	0x91
#define 	IDE_CMD_READ_MULTIPLE    		0xC4	// ��������
#define 	IDE_CMD_WRITE_MULTIPLE   		0xC5	// ������д
#define 	IDE_CMD_SET_MULTIPLE    		0xC6
#define 	IDE_CMD_READ_DMA         		0xC8
#define 	IDE_CMD_WRITE_DMA             	0xCA
#define 	IDE_CMD_GET_MEDIA_STATUS      	0xDA
#define    	IDE_CMD_STANDBY_IMMEDIATE		0xE0	// Standby Immediate
#define    	IDE_CMD_IDLE_IMMEDIATE 			0xE1	// Idle Immediate
#define 	IDE_CMD_SETFEATURES				0xEF
#define 	IDE_CMD_IDENTIFY              	0xEC
#define 	IDE_CMD_MEDIA_EJECT           	0xED


/*״̬�Ĵ������λ*/
#define		ATA_STAT_REG_ERR 				0x01	/*�豸�����˴���*/
#define		ATA_STAT_REG_IDX  				0x02  	/**/
#define		ATA_STAT_REG_CORR  				0x04  	/**/
#define		ATA_STAT_REG_DRQ  				0x08 	/*�����ݴ�������*/
#define		ATA_STAT_REG_DSC  				0x10 	/**/
#define		ATA_STAT_REG_DF   				0x20 	/**/
#define		ATA_STAT_REG_DRDY  				0x40 	/*�豸׼����*/
#define		ATA_STAT_REG_BSY  				0x80 	/*�豸æ*/
#define		ATA_STAT_BSY_DRDY		(ATA_STAT_REG_BSY+ATA_STAT_REG_DRDY)
#define		ATA_STAT_BSY_DRQ		(ATA_STAT_REG_BSY+ATA_STAT_REG_DRQ)

/*�豸��ͷ�Ĵ���*/
#define		ATA_DevReg_DEV0  				0x00	/*�豸Ϊ����*/
#define		ATA_DevReg_DEV1  				0x10	/*�豸Ϊ����*/
#define		ATA_DevReg_b5   				0x20	/*����λ��Ϊ1*/
#define		ATA_DevReg_LBA  				0x40	/*ģʽѡ��λ*/
#define		ATA_DevReg_b7   				0x80	/*����λ��Ϊ1*/

/*���ƼĴ���*/
#define 	CF_SOFT_RESET					0x04	/*�����λ��Ч*/
#define 	CF_DISABLE_INT					0x00	/*�����ж�*/
#define 	CF_ENABLE_INT					0x02	/*ʹ���ж�*/


/*���������16λ�����������,��8λ��ATA����Ĵ���һ��,��8λ�Զ���*/
#define ATA_EER_REG_AMNF					0x01	/*һ�����*/
#define ATA_EER_REG_ABRT 					0x04	/*ָ����Ч����*/
#define ATA_EER_REG_IDNF 					0x10	/*Ѱ������ַ����*/
#define ATA_EER_REG_UNC 					0x40	/*�����˲��ɾ����Ĵ���*/
#define ATA_EER_REG_BBK						0x80	/*���ִ����*/
/*�Զ���*/
#define CARD_IS_FALSE						0x00ff	/*CF�������ڻ���Ч*/
#define CARD_IS_TRUE						0x0000	/*CF����Ч*/
#define CARD_TIMEOUT						0x0100	/*��CF��������ʱ����*/


#define THIS_DEVICE			((ATA_DevReg&0x10)>>4)	/*��ǰ�������豸��ATA_SelectDevice����ѡ����������豸��ͨ���ò�������ȷ���ղ�ѡ�����豸*/

/*�豸ʶ����Ϣ*/
/*ȫ���豸��Ϣ*/
typedef struct _IDENTIFY_DATA
{
	Uint16 GeneralConfiguration;            // 00  	 ��ͨ����-CF����־0x848A
	Uint16 NumberOfCylinders;               // 1   	 Ĭ��������
	Uint16 Reserved1;                       // 2
	Uint16 NumberOfHeads;                   // 3   	 Ĭ�ϴ�ͷ��
	Uint16 UnformattedBytesPerTrack;      	// 4   	 ��2.0�淶�Ѳ�ʹ��
	Uint16 UnformattedBytesPerSector;     	// 5   	 ��2.0�淶�Ѳ�ʹ��
	Uint16 SectorsPerTrack;                 // 6   	 ÿ�ŵ�Ĭ��������
	Uint32 SectorsPerCard;                  // 7-8 	 ÿ��������(Word 7=MSW,Word 8=LSW)
	Uint16 VendorUnique;					// 9 	 ��2.0�淶�Ѳ�ʹ��
	Uint16 SerialNumber[10];                // 10-19 ASCII���кţ�������д��
	Uint16 BufferType;                      // 20 	 ��2.0�淶�Ѳ�ʹ��
	Uint16 BufferSectorSize;                // 21    ��2.0�淶�Ѳ�ʹ��
	Uint16 NumberOfEccBytes;                // 22    ��/д��ָ��ʱ��ECC�ֽ���
	Uint16 FirmwareRevision[4];             // 23-26 ASCII�̼��汾����˸�ʽ
	Uint16 ModelNumber[20];                 // 27-46 ASCIIģʽ�ţ�������д������˸�ʽ
	Uint16 MaximumBlockTransfer;            //*47    ���� ��/дָ����������š�
	Uint16 Reserved2;                    	// 48
	Uint16 Capabilities;                    // 49
	Uint16 Reserved3;                       // 50

	Uint16 PioCycleTimingMode;             	//*51    PIO���ݴ�������ʱ��ģʽ

	Uint16 DmaCycleTimingMode;             	// 52    ��2.0�淶�Ѳ�ʹ��
	Uint16 TranslationFieldsValid;          // 53    0λΪ1,Word54 to 58 ��Ч;1λΪ1,Word64 to 70 ��Ч

	Uint16 NumberOfCurrentCylinders;      	// 54    ��ǰ������
	Uint16 NumberOfCurrentHeads;            // 55    ��ǰ��ͷ��
	Uint16 CurrentSectorsPerTrack;          // 56    ��ǰÿ�ŵ�������
	Uint32 CurrentSectorCapacity;           // 57-58 ��ǰ����������LBAs��(Word 58=MSW,Word 57=LSW)

	Uint16 CurrentMultiSectorSetting;       //*59    λ8Ϊ1,��λ0-7,ָ���ɶ�������д��������
	Uint32 UserAddressableSectors;          // 60-61 LABģʽ�¿ɱ�Ѱַ��������
	Uint16 Reserved4[2] ;           		// 62-63
	Uint16 AdvancedPIOModes ;               // 64    ��2.0�淶ʹ��
	Uint16 Reserved6[2] ;         			// 65-66
	Uint16 MinimumPIOCycleTime;             // 67    ������������СPIO��������ʱ��
	Uint16 MinimumPIOCycleTimeIORDY; 		// 68    IORDY������С��СPIO��������ʱ��
	Uint16 Reserved7[10];                   // 69-79
	Uint32 AtaVersion;           			// 80-81 CF������0
	Uint16 FeaturesCommandSupported[3];		// 82-84 �������������
	Uint16 FeaturesCommandEnabled[3];    	// 85-87 ���������ʹ��
	Uint16 Reserved8;       				// 88
	Uint16 MajorRevision;                   // 89    ��ȫ��λ��ȫ������ʱ��Ҫ��
	Uint16 MinorRevision;                   // 90    ��ǿ�Ͱ�ȫ��λ��ȫ������ʱ��Ҫ��
	Uint16 PowerManagementValue;			// 91	 ��ǰ�߼���Դ����ֵ									//     91
	Uint16 Reserved9[36];                   // 92-127
	Uint16 SecurityStatus;					// 128   ��ȫ״̬
	Uint16 VendorUniqueBytes[32];			// 129-159
	Uint16 PowerRequirementDescription; 	// 160   ��ԴҪ������
	Uint16 Reserved10;						// 161
	Uint16 KeyManagementSupported;        	// 162   ���������
	Uint16 Reserved11[83];                  // 163-255

} CF_IDENTIFY_DATA, *PCF_IDENTIFY_DATA;

/*�豸������Ϣ*/
typedef struct _ATA_IDENTIFY_DATA
{
	Uint16 NumberOfCylinders;               // 1   	 Ĭ��������
	Uint16 NumberOfHeads;                   // 3   	 Ĭ�ϴ�ͷ��
	Uint16 SectorsPerTrack;                 // 6   	 ÿ�ŵ�Ĭ��������
	Uint32 CurrentSectorCapacity;           // 57-58 ��ǰ����������LBAs��(Word 58=MSW,Word 57=LSW)

} ATA_IDENTIFY_DATA, *PATA_IDENTIFY_DATA;


/*�豸��Ϣ�ṹ*/
typedef struct
{
	Uint8   			ATAIsOK;			/*�豸����Ϊ1����Ч���д�Ϊ0*/
	Uint16 				ERRCODE;			/*�豸�������*/
	Uint32 				ERRSECTOR;			/*��д�����ǳ����������*/
	ATA_IDENTIFY_DATA	INFODATA;			/*�豸�Ļ�����Ϣ*/
}ATADEVICE, *PATADEVICE;

extern ATADEVICE 	ATA[MAX_DEVICE];		/*�豸ʶ����Ϣʶ��ṹ*/



/******************************************************************************************/
/*��غ���ԭ��*/
/******************************************************************************************/

//****************************************************************************************
/*�û�ʹ�ú���*/
void 	USB_detecte( void );
void 	ATA_INI( void );																/*�����������ʼ����ATA�豸��ʼ��*/
void  	ATA_FindIDEDevice( void );													/*Ӳ����λ,����IDE�豸������ʼ��*/

Uint8 	ATA_IdleImmediate( Uint8 Device );				    						/*�����������״̬(Ӳ��ת��)*/
Uint8 	ATA_StandbyImmediate( Uint8 Device );											/*�����������״̬(Ӳ��ͣת)*/
Uint8 	ATA_ReadSector( Uint8 Device, Uint16 *Buffer, Uint32 Sector, Uint8 count );		/*�����������ݺ���*/
Uint8 	ATA_WriteSector( Uint8 Device, Uint16 * Buffer, Uint32 Sector , Uint8 count );	/*д���豸������ݺ���*/
Uint8 	ATA_IdentifyDrive( Uint8 Device, void *PTR );									/*��ȡ�豸��Ϣ*/

//****************************************************************************************
/*�����ڲ�ʹ�ú���*/
Uint8 	ATA_IssueIDEIdentify( Uint8 Device );				/*�豸ʶ��,��ȡ�豸������Ϣ*/

Uint8 	ATA_SetFeature( Uint8 Device );					/*�豸������������*/
void  	ATA_WriteSetting( Uint32 Sectors , Uint8 Count );	/*дIDE������������*/
void  	ATA_ReadSetting( Uint32 Sectors , Uint8 Count );	/*дIDE������������*/
void 	ATA_WriteData512B( Uint16 * Buffer );				/*����д512�ֽ�����*/
void 	ATA_ReadData512B( Uint16 * Buffer );				/*������512�ֽ�����*/

#endif

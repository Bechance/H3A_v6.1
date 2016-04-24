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
/** Çı¶¯²ÎÊıÅäÖÃ																	   */
/***************************************************************************************/
#define MAX_DEVICE 	2					/*¶¨ÒåÉè±¸Êı,ÓÉÓÚÖ»ÓĞÖ÷ºÍ´ÓÉè±¸,ËùÒÔ×î´óÖµÎª2*/
#define TIMEOUTSET		0x0005ffff		/*³¬Ê±ÏìÓ¦Éè±¸(ÓëCPUµÄËÙ¶ÈÓĞ¹Ø,Ã»ÓĞ×¼È·µÄÊ±¼ä)*/


/****************************************************************************************/
/**	ÒÔÏÂÎª±¾Çı¶¯¶¨Òå²¿·İ£¬ÓÃ»§²»Ğè¸Ä¶¯													*/
/****************************************************************************************/
/*ATA Ö¸Áî´úÂë*/
#define 	IDE_CMD_ATAPI_RESET      		0x08
#define 	IDE_CMD_RECALIBRATE      		0x10
#define 	IDE_CMD_READ             		0x20	// ¶ÁÉÈÇøÃüÁî
#define 	IDE_CMD_WRITE            		0x30	// Ğ´ÉÈÇøÃüÁî
#define 	IDE_CMD_VERIFY           		0x40
#define 	IDE_CMD_SEEK             		0x70
#define		IDE_CMD_EXEC_DIAGNOSE    		0x90
#define		IDE_CMD_SET_DRIVE_PARAMETERS 	0x91
#define 	IDE_CMD_READ_MULTIPLE    		0xC4	// ¶àÉÈÇø¶Á
#define 	IDE_CMD_WRITE_MULTIPLE   		0xC5	// ¶àÉÈÇøĞ´
#define 	IDE_CMD_SET_MULTIPLE    		0xC6
#define 	IDE_CMD_READ_DMA         		0xC8
#define 	IDE_CMD_WRITE_DMA             	0xCA
#define 	IDE_CMD_GET_MEDIA_STATUS      	0xDA
#define    	IDE_CMD_STANDBY_IMMEDIATE		0xE0	// Standby Immediate
#define    	IDE_CMD_IDLE_IMMEDIATE 			0xE1	// Idle Immediate
#define 	IDE_CMD_SETFEATURES				0xEF
#define 	IDE_CMD_IDENTIFY              	0xEC
#define 	IDE_CMD_MEDIA_EJECT           	0xED


/*×´Ì¬¼Ä´æÆ÷Ïà¹ØÎ»*/
#define		ATA_STAT_REG_ERR 				0x01	/*Éè±¸·¢ÉúÁË´íÎó*/
#define		ATA_STAT_REG_IDX  				0x02  	/**/
#define		ATA_STAT_REG_CORR  				0x04  	/**/
#define		ATA_STAT_REG_DRQ  				0x08 	/*ÓĞÊı¾İ´«ÊäÇëÇó*/
#define		ATA_STAT_REG_DSC  				0x10 	/**/
#define		ATA_STAT_REG_DF   				0x20 	/**/
#define		ATA_STAT_REG_DRDY  				0x40 	/*Éè±¸×¼±¸ºÃ*/
#define		ATA_STAT_REG_BSY  				0x80 	/*Éè±¸Ã¦*/
#define		ATA_STAT_BSY_DRDY		(ATA_STAT_REG_BSY+ATA_STAT_REG_DRDY)
#define		ATA_STAT_BSY_DRQ		(ATA_STAT_REG_BSY+ATA_STAT_REG_DRQ)

/*Éè±¸´ÅÍ·¼Ä´æÆ÷*/
#define		ATA_DevReg_DEV0  				0x00	/*Éè±¸ÎªÖ÷ÅÌ*/
#define		ATA_DevReg_DEV1  				0x10	/*Éè±¸Îª´ÓÅÌ*/
#define		ATA_DevReg_b5   				0x20	/*±£ÁôÎ»£¬Îª1*/
#define		ATA_DevReg_LBA  				0x40	/*Ä£Ê½Ñ¡ÔñÎ»*/
#define		ATA_DevReg_b7   				0x80	/*±£ÁôÎ»£¬Îª1*/

/*¿ØÖÆ¼Ä´æÆ÷*/
#define 	CF_SOFT_RESET					0x04	/*Èí¼ş¸´Î»ÓĞĞ§*/
#define 	CF_DISABLE_INT					0x00	/*½ûÄÜÖĞ¶Ï*/
#define 	CF_ENABLE_INT					0x02	/*Ê¹ÄÜÖĞ¶Ï*/


/*´íÎó´úÂëÓÉ16Î»¶ş½øÖÆÊı×é³É,µÍ8Î»ÓëATA´íÎó¼Ä´æÆ÷Ò»ÖÂ,¸ß8Î»×Ô¶¨Òå*/
#define ATA_EER_REG_AMNF					0x01	/*Ò»°ã´íÎó*/
#define ATA_EER_REG_ABRT 					0x04	/*Ö¸ÁîÎŞĞ§³ö´í*/
#define ATA_EER_REG_IDNF 					0x10	/*Ñ°ÉÈÇøµØÖ·³ö´í*/
#define ATA_EER_REG_UNC 					0x40	/*·¢ÉúÁË²»¿É¾ÀÕıµÄ´íÎó*/
#define ATA_EER_REG_BBK						0x80	/*·¢ÏÖ´íÎó¿é*/
/*×Ô¶¨Òå*/
#define CARD_IS_FALSE						0x00ff	/*CF¿¨²»´æÔÚ»òÎŞĞ§*/
#define CARD_IS_TRUE						0x0000	/*CF¿¨ÓĞĞ§*/
#define CARD_TIMEOUT						0x0100	/*¶ÔCF¿¨²Ù×÷³¬Ê±³ö´í*/


#define THIS_DEVICE			((ATA_DevReg&0x10)>>4)	/*µ±Ç°²Ù×÷µÄÉè±¸£¬ATA_SelectDeviceº¯ÊıÑ¡ÔñÁËÖ÷»ò´ÓÉè±¸£¬Í¨¹ı¸Ã²Ù×÷¿ÉÒÔÈ·¶¨¸Õ²ÅÑ¡¶¨µÄÉè±¸*/

/*Éè±¸Ê¶±ğĞÅÏ¢*/
/*È«²¿Éè±¸ĞÅÏ¢*/
typedef struct _IDENTIFY_DATA
{
	Uint16 GeneralConfiguration;            // 00  	 ÆÕÍ¨ÉèÖÃ-CF¿¨±êÖ¾0x848A
	Uint16 NumberOfCylinders;               // 1   	 Ä¬ÈÏÖùÃæÊı
	Uint16 Reserved1;                       // 2
	Uint16 NumberOfHeads;                   // 3   	 Ä¬ÈÏ´ÅÍ·Êı
	Uint16 UnformattedBytesPerTrack;      	// 4   	 ÔÚ2.0¹æ·¶ÒÑ²»Ê¹ÓÃ
	Uint16 UnformattedBytesPerSector;     	// 5   	 ÔÚ2.0¹æ·¶ÒÑ²»Ê¹ÓÃ
	Uint16 SectorsPerTrack;                 // 6   	 Ã¿´ÅµÀÄ¬ÈÏÉÈÇøÊı
	Uint32 SectorsPerCard;                  // 7-8 	 Ã¿¿¨ÉÈÇøÊı(Word 7=MSW,Word 8=LSW)
	Uint16 VendorUnique;					// 9 	 ÔÚ2.0¹æ·¶ÒÑ²»Ê¹ÓÃ
	Uint16 SerialNumber[10];                // 10-19 ASCIIĞòÁĞºÅ£¨¿¿ÓÒÌîĞ´£©
	Uint16 BufferType;                      // 20 	 ÔÚ2.0¹æ·¶ÒÑ²»Ê¹ÓÃ
	Uint16 BufferSectorSize;                // 21    ÔÚ2.0¹æ·¶ÒÑ²»Ê¹ÓÃ
	Uint16 NumberOfEccBytes;                // 22    ¶Á/Ğ´³¤Ö¸ÁîÊ±µÄECC×Ö½ÚÊı
	Uint16 FirmwareRevision[4];             // 23-26 ASCII¹Ì¼ş°æ±¾¡£´ó¶Ë¸ñÊ½
	Uint16 ModelNumber[20];                 // 27-46 ASCIIÄ£Ê½ºÅ£¨¿¿×óÌîĞ´£©£¬´ó¶Ë¸ñÊ½
	Uint16 MaximumBlockTransfer;            //*47    ¶àÖØ ¶Á/Ğ´Ö¸Áî×î´óÉÈÇøºÅ¡£
	Uint16 Reserved2;                    	// 48
	Uint16 Capabilities;                    // 49
	Uint16 Reserved3;                       // 50

	Uint16 PioCycleTimingMode;             	//*51    PIOÊı¾İ´«ÊäÖÜÆÚÊ±ĞòÄ£Ê½

	Uint16 DmaCycleTimingMode;             	// 52    ÔÚ2.0¹æ·¶ÒÑ²»Ê¹ÓÃ
	Uint16 TranslationFieldsValid;          // 53    0Î»Îª1,Word54 to 58 ÓĞĞ§;1Î»Îª1,Word64 to 70 ÓĞĞ§

	Uint16 NumberOfCurrentCylinders;      	// 54    µ±Ç°ÖùÃæÊı
	Uint16 NumberOfCurrentHeads;            // 55    µ±Ç°´ÅÍ·Êı
	Uint16 CurrentSectorsPerTrack;          // 56    µ±Ç°Ã¿´ÅµÀÉÈÇøÊı
	Uint32 CurrentSectorCapacity;           // 57-58 µ±Ç°ÉÈÇøÈİÁ¿£¨LBAs£©(Word 58=MSW,Word 57=LSW)

	Uint16 CurrentMultiSectorSetting;       //*59    Î»8Îª1,ÔòÎ»0-7,Ö¸³ö¿É¶àÉÈÇø¶ÁĞ´µÄÉÈÇøÊı
	Uint32 UserAddressableSectors;          // 60-61 LABÄ£Ê½ÏÂ¿É±»Ñ°Ö·µÄÉÈÇøÊı
	Uint16 Reserved4[2] ;           		// 62-63
	Uint16 AdvancedPIOModes ;               // 64    ÔÚ2.0¹æ·¶Ê¹ÓÃ
	Uint16 Reserved6[2] ;         			// 65-66
	Uint16 MinimumPIOCycleTime;             // 67    ·ÇÁ÷¿ØÖÆÏÂ×îĞ¡PIO´«ÊäÖÜÆÚÊ±¼ä
	Uint16 MinimumPIOCycleTimeIORDY; 		// 68    IORDYÁ÷¿ØÖÆĞ¡×îĞ¡PIO´«ÊäÖÜÆÚÊ±¼ä
	Uint16 Reserved7[10];                   // 69-79
	Uint32 AtaVersion;           			// 80-81 CF¿¨·µ»Ø0
	Uint16 FeaturesCommandSupported[3];		// 82-84 ÌØÕ÷£¯ÃüÁî¼¯¼¼³Ö
	Uint16 FeaturesCommandEnabled[3];    	// 85-87 ÌØÕ÷£¯ÃüÁî¼¯Ê¹ÄÜ
	Uint16 Reserved8;       				// 88
	Uint16 MajorRevision;                   // 89    °²È«µ¥Î»ÍêÈ«²Á³ıµÄÊ±¼äÒªÇó
	Uint16 MinorRevision;                   // 90    ÔöÇ¿ĞÍ°²È«µ¥Î»ÍêÈ«²Á³ıµÄÊ±¼äÒªÇó
	Uint16 PowerManagementValue;			// 91	 µ±Ç°¸ß¼¶µçÔ´¹ÜÀíÖµ									//     91
	Uint16 Reserved9[36];                   // 92-127
	Uint16 SecurityStatus;					// 128   °²È«×´Ì¬
	Uint16 VendorUniqueBytes[32];			// 129-159
	Uint16 PowerRequirementDescription; 	// 160   µçÔ´ÒªÇóÃèÊö
	Uint16 Reserved10;						// 161
	Uint16 KeyManagementSupported;        	// 162   °´¼ü¹ÜÀí±í
	Uint16 Reserved11[83];                  // 163-255

} CF_IDENTIFY_DATA, *PCF_IDENTIFY_DATA;

/*Éè±¸»ù±¾ĞÅÏ¢*/
typedef struct _ATA_IDENTIFY_DATA
{
	Uint16 NumberOfCylinders;               // 1   	 Ä¬ÈÏÖùÃæÊı
	Uint16 NumberOfHeads;                   // 3   	 Ä¬ÈÏ´ÅÍ·Êı
	Uint16 SectorsPerTrack;                 // 6   	 Ã¿´ÅµÀÄ¬ÈÏÉÈÇøÊı
	Uint32 CurrentSectorCapacity;           // 57-58 µ±Ç°ÉÈÇøÈİÁ¿£¨LBAs£©(Word 58=MSW,Word 57=LSW)

} ATA_IDENTIFY_DATA, *PATA_IDENTIFY_DATA;


/*Éè±¸ĞÅÏ¢½á¹¹*/
typedef struct
{
	Uint8   			ATAIsOK;			/*Éè±¸Õı³£Îª1£¬ÎŞĞ§»òÓĞ´íÎª0*/
	Uint16 				ERRCODE;			/*Éè±¸³ö´í´úÂë*/
	Uint32 				ERRSECTOR;			/*¶ÁĞ´ÉÈÇøÊÇ³ö´íµÄÉÈÇøºÅ*/
	ATA_IDENTIFY_DATA	INFODATA;			/*Éè±¸µÄ»ù±¾ĞÅÏ¢*/
}ATADEVICE, *PATADEVICE;

extern ATADEVICE 	ATA[MAX_DEVICE];		/*Éè±¸Ê¶±ğĞÅÏ¢Ê¶±ğ½á¹¹*/



/******************************************************************************************/
/*Ïà¹Øº¯ÊıÔ­ĞÍ*/
/******************************************************************************************/

//****************************************************************************************
/*ÓÃ»§Ê¹ÓÃº¯Êı*/
void 	USB_detecte( void );
void 	ATA_INI( void );																/*×ÜÏßÉèÖÃÓë³õÊ¼»¯£¬ATAÉè±¸³õÊ¼»¯*/
void  	ATA_FindIDEDevice( void );													/*Ó²¼ş¸´Î»,²éÕÒIDEÉè±¸£¬²¢³õÊ¼»¯*/

Uint8 	ATA_IdleImmediate( Uint8 Device );				    						/*Á¢¼´½øÈë¿ÕÏĞ×´Ì¬(Ó²ÅÌ×ª¶¯)*/
Uint8 	ATA_StandbyImmediate( Uint8 Device );											/*Á¢¼´½øÈë´ı»ú×´Ì¬(Ó²ÅÌÍ£×ª)*/
Uint8 	ATA_ReadSector( Uint8 Device, Uint16 *Buffer, Uint32 Sector, Uint8 count );		/*¶Á³öÉÈÇøÊı¾İº¯Êı*/
Uint8 	ATA_WriteSector( Uint8 Device, Uint16 * Buffer, Uint32 Sector , Uint8 count );	/*Ğ´ÈëÉè±¸ÉÈÇøı¾İº¯Êı*/
Uint8 	ATA_IdentifyDrive( Uint8 Device, void *PTR );									/*»ñÈ¡Éè±¸ĞÅÏ¢*/

//****************************************************************************************
/*Çı¶¯ÄÚ²¿Ê¹ÓÃº¯Êı*/
Uint8 	ATA_IssueIDEIdentify( Uint8 Device );				/*Éè±¸Ê¶±ğ,»ñÈ¡Éè±¸»ù±¾ĞÅÏ¢*/

Uint8 	ATA_SetFeature( Uint8 Device );					/*Éè±¸×ÜÏßÌØĞÔÉèÖÃ*/
void  	ATA_WriteSetting( Uint32 Sectors , Uint8 Count );	/*Ğ´IDE¶àÊıÉÈÇøÊı¾İ*/
void  	ATA_ReadSetting( Uint32 Sectors , Uint8 Count );	/*Ğ´IDE¶àÊıÉÈÇøÊı¾İ*/
void 	ATA_WriteData512B( Uint16 * Buffer );				/*Á¬ĞøĞ´512×Ö½ÚÊı¾İ*/
void 	ATA_ReadData512B( Uint16 * Buffer );				/*Á¬Ğø¶Á512×Ö½ÚÊı¾İ*/

#endif

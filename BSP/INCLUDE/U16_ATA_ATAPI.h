/**
 * @file		U16_ATA_ATAPI.h
 * @brief		U16 ATA/ATAPI APIs Definition(global)
 * @date		2007/06/27
 *
 * Copyright(C) SEIKO EPSON CORPORATION 2007. All rights reserved.
 */

#ifndef U16_ATA_ATAPI_H
#define U16_ATA_ATAPI_H

#ifdef __cplusplus
extern "C"
{
#endif /* cplusplus */

	/*****************************************
	 * definition of "define"
	 *****************************************/
	/** @name U16_STATUS
	 *	 The status returned to other module is defined.
	 */
	/*@{*/
#define U16_STATUS_SUCCESS					(0)			/**< It succeeded. */
#define U16_STATUS_UNSUCCESSFUL				(-1)		/**< It failed. */
#define U16_STATUS_INVALID_PARAMETER		(-10)		/**< Invalid parameter. */
	/*@}*/

	/** @name U16_INFO
	 *	 Information on the device notified other module is defined.
	 */
	/*@{*/
#define U16_INFO_NO_DEVICE					(0x00)		/**< There is not a device. */
#define U16_INFO_ATA_DEVICE					(0x01)		/**< ATA device exists. */
#define U16_INFO_ATAPI_DEVICE				(0x02)		/**< ATAPI device exists. */
	/*@}*/

	/** @name U16_DEVICE
	 *	 The command execution result of the notification to other module is defined.
	 */
	/*@{*/
#define U16_DEVICE_ERR						(0x01)		/**< Device error */
	/*@}*/

	/** @name U16_API_PARAMETER
	 *	 The parameter that other module sets is defined.
	 */
	/*@{*/
#define U16_MASTER_DEVICE					(0x00)		/**< Selecting of the master device.  */
#define U16_SLAVE_DEVICE					(0x10)		/**< Selecting of the slave device.  */
#define U16_NORMAL_ATA_CMD					(0x00)		/**< Normal ATA command. */
#define U16_EX_ATA_CMD						(0x01)		/**< Expansion ATA command. */
#define U16_DATA_IN							(0x00)		/**< Receiving of data. */
#define U16_DATA_OUT						(0x01)		/**< Sending of data. */
#define U16_PIO_TRAN						(0x00)		/**< Data transfer with PIO. */
#define U16_DMA_TRAN						(0x01)		/**< Data transfer with DMA. */
#define U16_HARD_RESET						(0x01)		/**< Hard reset is issued. */
#define U16_SOFT_RESET						(0x00)		/**< Soft reset is issued. */
	/*@}*/

#define	U16_DRIVENUM						(0x00)		/** driver number */
	/*****************************************
	 * definition of "struct"
	 *****************************************/
	/** @struct tagU16_DEVICE_INFO
	 * @brief The structure of device information is defined.
	 */
	typedef struct tagU16_DEVICE_INFO
	{
		Uint8 master;								/**< (out)Device information of the master */
		Uint8 slave;								/**< (out)Device information of the slave */
	}U16_DEVICE_INFO;


	/** @struct tagU16_ATA_CMD_BLK
	 * @brief  The structure of the ATA command block is defined.
	 */
	typedef struct tagU16_ATA_CMD_BLK
	{
		Uint8 features;								/**< Features register */
		Uint8 featuresPrevious;						/**< Features register ( For expansion command ) */
		Uint8 sectorCount;							/**< Sector count register */
		Uint8 sectorCountPrevious;					/**< Sector count register ( For expansion command ) */
		Uint8 lbaLow;								/**< LBA low register */
		Uint8 lbaLowPrevious;						/**< LBA low register ( For expansion command ) */
		Uint8 lbaMid;								/**< LBA middle register */
		Uint8 lbaMidPrevious;						/**< LBA middle register ( For expansion command ) */
		Uint8 lbaHigh;								/**< LBA high register */
		Uint8 lbaHighPrevious;						/**< LBA high register ( For expansion command ) */
		Uint8 device;								/**< Device register */
		Uint8 command;								/**< Command register */
	}U16_ATA_CMD_BLK;


	/** @struct tagU16_ATA_REG_BLK
	 * @brief The structure of the ATA task file register block is defined.
	 */
	typedef struct tagU16_ATA_REG_BLK
	{
		Uint8 error;								/**< Error register */
		Uint8 sectorCount;							/**< Sector count register */
		Uint8 sectorCountPrevious;                  /**< Sector count register ( For expansion command ) */
		Uint8 lbaLow;                               /**< LBA low register */
		Uint8 lbaLowPrevious;                       /**< LBA low register  ( For expansion command ) */
		Uint8 lbaMid;                               /**< LBA middle register */
		Uint8 lbaMidPrevious;                       /**< LBA middle register ( For expansion command ) */
		Uint8 lbaHigh;                              /**< LBA high register */
		Uint8 lbaHighPrevious;                      /**< LBA high register ( For expansion command ) */
		Uint8 device;                               /**< Device register */
		Uint8 status;                               /**< Status register */
		Uint8 reserved;
	}U16_ATA_REG_BLK;

	/** @struct tagU16_ATA_STS_BLK
	 * @brief The structure of the ATA status block is defined.
	 */
	typedef struct tagU16_ATA_STS_BLK
	{
		Uint8 error;								/**< Error register */
		Uint8 status;								/**< Status register */
	}U16_ATA_STS_BLK;


	/** @struct tagU16_ATAPI_STS_BLK
	 * @brief The structure of the ATAPI status block is defined.
	 */
	typedef struct tagU16_ATAPI_STS_BLK
	{
		Uint8 error;								/**< Error register */
		Uint8 status;                               /**< Status register */
	}U16_ATAPI_STS_BLK;


	/** @struct tagU16_ATA_CMD_PARA
	 * @brief The structure of the ATA command parameter is defined.
	 */
	typedef struct tagU16_ATA_CMD_PARA
	{
		Uint8 cmdType;						/**< (in)  ATA command type */
		U16_ATA_CMD_BLK cmdBlk;						/**< (in)  ATA command block is set */
		Uint32 dataSize;						/**< (io)  (in)  Byte count of transfer data is set.
												 *		   (out) I return the byte count that transferred it actually.
												 */
		Uint16 *pTranBuf;								/**< (in)  Buffer pointer for data transfer is set.
	    										 *		   In the case that dataSize is 0 it is invalid.
	    										 */
		Uint8 direction;					/**< (in)  Data transfer direction is set.
												 *			- U16_DATA_IN
												 *				- Receiving of data.
												 *			- U16_DATA_OUT
												 *				- Receiving of data.
												 */
		Uint8 tranType;						/**< (in)  Type of data transfer is set
												 *			- U16_DMA_TRAN
												 *				- Data transfer with DMA
												 *			- U16_PIO_TRAN
												 *				- Data transfer with PIO.
												 */
		Uint8 pioSize; 					/**< (in)  Transfer byte count of 1PIO is set.
												 *         To set 512 multiples
												 */
		U16_ATA_STS_BLK stsBlk;  					/**< (out) Area that returns the status is set */
	}U16_ATA_CMD_PARA;


	/** @struct tagU16_ATAPI_CMD_PARA
	 * @brief The structure of the ATAPI command parameter is defined.
	 */
	typedef struct tagU16_ATAPI_CMD_PARA
	{
		Uint8 cmdBlk[12];					/**< (in)  ATA command block is set */
		Uint32 dataSize;						/**< (io)  (in)  Byte count of transfer data is set.
												 *		   (out) I return the byte count that transferred it actually.
												 */
		Uint16 *pTranBuf;								/**< (in)  Buffer pointer for data transfer is set.
	    										 *		   In the case that dataSize is 0 it is invalid.
	    										 */
		Uint8 direction;					/**< (in)  Data transfer direction is set.
												 *			- U16_DATA_IN
												 *				- Receiving of data.
												 *			- U16_DATA_OUT
												 *				- Receiving of data.
												 */
		Uint8 tranType;						/**< (in)  Mode of data transfer is set
												 *			- U16_DMA_TRAN
												 *				- Data transfer with DMA
												 *			- U16_PIO_TRAN
												 *				- Data transfer with PIO.
												 */
		U16_ATAPI_STS_BLK stsBlk;					/**< (out) Area that returns the status is set */
	}U16_ATAPI_CMD_PARA;


	/*****************************************
	 * Declaration of function prototype
	 *****************************************/
	extern short RegRead( Uint16 Reg );
	extern void  RegWrite( Uint16 Reg, Uint16 val );
	extern short U16_Initialize( U16_DEVICE_INFO *pDeviceInfo );
	extern short U16_ExecATACommand( Uint8 selectDevice, U16_ATA_CMD_PARA *pATACmdPara );
	extern short U16_ExecATAPICommand( Uint8 selectDevice, U16_ATAPI_CMD_PARA *pATAPICmdPara );
	extern short U16_ResetDevice( Uint8 resetType );
	extern short U16_GetTaskFileReg( Uint8 selectDevice, U16_ATA_REG_BLK *pATARegBlk );
	extern short U16_GetStatusReg( Uint8 selectDevice,	Uint8 *pATAStatus );

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* U16_ATA_ATAPI_H */

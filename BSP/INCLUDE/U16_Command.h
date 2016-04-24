/**
 * @file		U16_Command.h
 * @brief		U16 command APIs Definition(global)
 * @date		2007/06/27
 *
 * Copyright(C) SEIKO EPSON CORPORATION 2007. All rights reserved.
 */

#ifndef U16_CMD_H
#define U16_CMD_H

#ifdef __cplusplus
extern "C"
{
#endif /* cplusplus */

#include <U16_ATA_ATAPI.h>

	/** @name U16_CMD_STATUSU
	 *	 The status returned to another module is defined.
	 */
	/*@{*/
#define U16_CMD_STATUS_SUCCESS				(0)						/**< It succeeded. */
#define U16_CMD_STATUS_UNSUCCESSFUL			(-1)					/**< It failed. */
#define U16_CMD_STATUS_INVALID_PARAMETER	(-10)					/**< Invalid parameter. */
	/*@}*/


	/** @name U16_CMD
	 *	 Connected device type is defined.
	 */
	/*@{*/
#define U16_CMD_DIRECT_DEVICE				(0x00)					/**< Direct Access (HDD) device */
#define U16_CMD_CDROM_DEVICE				(0x05)					/**< CD-ROM or DVD device */
#define U16_CMD_MO_DEVICE					(0x07)					/**< MO device */
#define U16_CMD_NO_DEVICE					(0xFF)					/**< No device or unsupport device */
	/*@}*/

	/** @struct tagU16_CMD_DEVICE_INFO
	 * @brief The structure of device information is defined.
	 */
	typedef struct tagU16_CMD_DEVICE_INFO
	{
		Uint8 driveType;					/**< (out) Drive type */
		Uint16 sectorSize;					/**< (out) Sector size */
		Uint32 maxLba;						/**< (out) Maximum LBA */
	}U16_CMD_DEVICE_INFO;


	/*****************************************
	 * Declaration of function prototype
	 *****************************************/
	extern short U16_CmdInitialize( void );
	extern short U16_CmdSetDeviceNumber( Uint8 deviceNum );
	extern short U16_CmdDeviceInitialize( Uint8 deviceNum );
	extern short U16_CmdGetDeviceInformation( Uint8 deviceNum, U16_CMD_DEVICE_INFO *pDeviceInfo );
	extern short U16_CmdReadSector( Uint8 deviceNum, Uint32 sectorNum,  Uint32 sectorCount, Uint16 sectorSize, Uint16 *pDataBuf );
	extern short U16_CmdWriteSector( Uint8 deviceNum, Uint32 sectorNum,  Uint32 sectorCount, Uint16 sectorSize, Uint16 *pDataBuf );
	extern short U16_CmdDeviceReset( Uint8 deviceNum );

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* U16_CMD_H */

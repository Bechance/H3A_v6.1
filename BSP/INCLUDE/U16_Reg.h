/**
 * @file		U16_Reg.h
 * @brief		S1R72U16 ATA task file register definition(global)
 * @date		2007/06/27
 *
 * Copyright(C) SEIKO EPSON CORPORATION 2007. All rights reserved.
 */

#ifndef U16_REG_H
#define U16_REG_H

#ifdef __cplusplus
extern "C"
{
#endif /* cplusplus */



	/*****************************************************************************
	 * Macro for register access
	 *****************************************************************************/
	/*
	#define RegRead(reg)					(*(reg))
	#define RegWrite(reg, value)			(*(reg)  =  (value))
	*/

	/********************************************************************************/
	/* Correct the value of U16_REG_BASE_ADDR according to a using system.			*/
	/********************************************************************************/

	/** @name U16_REG
	 *	 The register of S1R72U16 is defined.
	 */
	/*@{*/
#define U16_REG_BASE_ADDR			0x4A00											/**< Register base address */
#define U16_REG16_DATA				(U16_REG_BASE_ADDR + 0x00 )	/**< Data register */
#define U16_REG08_FEATURES			(U16_REG_BASE_ADDR + 0x01 )	/**< Features register */
#define U16_REG08_ERROR				(U16_REG_BASE_ADDR + 0x01 )	/**< Error register */
#define U16_REG08_SECTOR_COUNT		(U16_REG_BASE_ADDR + 0x02 )	/**< Sector count register */
#define U16_REG08_LBA_LOW			(U16_REG_BASE_ADDR + 0x03 )	/**< LBA Low register */
#define U16_REG08_LBA_MID			(U16_REG_BASE_ADDR + 0x04 )	/**< LBA Mid register */
#define U16_REG08_LBA_HIGH			(U16_REG_BASE_ADDR + 0x05 )	/**< LBA High register */
#define U16_REG08_DEVICE			(U16_REG_BASE_ADDR + 0x06 )	/**< Device register */
#define U16_REG08_COMMAND			(U16_REG_BASE_ADDR + 0x07 )	/**< Command register */
#define U16_REG08_STATUS			(U16_REG_BASE_ADDR + 0x07 )	/**< Status register */
#define U16_REG08_DEV_CONTROL		(U16_REG_BASE_ADDR + 0x0E )	/**< Device control register */
#define U16_REG08_ALT_STATUS		(U16_REG_BASE_ADDR + 0x0E )	/**< Alternate status register */
#define U16_REG08_INTERRUPT_REASON	 U16_REG08_SECTOR_COUNT									/**< ATAPI interrupt reason register */
#define U16_REG08_BYTE_COUNT_LOW	 U16_REG08_LBA_MID										/**< ATAPI byte count register */
#define U16_REG08_BYTE_COUNT_HIGH	 U16_REG08_LBA_HIGH										/**< ATAPI byte count register */
	/*@}*/


#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* U16_REG_H */

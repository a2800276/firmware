/*
 * @brief LPC18xx/43xx SD/MMC card driver
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#ifndef __SDMMC_18XX_43XX_H_
#define __SDMMC_18XX_43XX_H_

#include "everykey/everykey.h"
#include "sdmmc.h"
#include "sdmmc_standard.h"

#ifdef __cplusplus
extern "C" {
#endif

//taken from sdif_18xx_43xx.h

/** @brief Setup options for the SDIO driver
 */
#define US_TIMEOUT            1000000		/*!< give 1 atleast 1 sec for the card to respond */
#define MS_ACQUIRE_DELAY      (10)			/*!< inter-command acquire oper condition delay in msec*/
#define INIT_OP_RETRIES       50			/*!< initial OP_COND retries */
#define SET_OP_RETRIES        1000			/*!< set OP_COND retries */
#define SDIO_BUS_WIDTH        4				/*!< Max bus width supported */
#define SD_MMC_ENUM_CLOCK       400000		/*!< Typical enumeration clock rate */
#define MMC_MAX_CLOCK           20000000	/*!< Max MMC clock rate */
#define MMC_LOW_BUS_MAX_CLOCK   26000000	/*!< Type 0 MMC card max clock rate */
#define MMC_HIGH_BUS_MAX_CLOCK  52000000	/*!< Type 1 MMC card max clock rate */
#define SD_MAX_CLOCK            25000000	/*!< Max SD clock rate */

/** @brief SDIO card-type register defines
 */
#define MCI_CTYPE_8BIT          (1 << 16)		/*!< Enable 4-bit mode */
#define MCI_CTYPE_4BIT          (1 << 0)		/*!< Enable 8-bit mode */

/** @brief  SDIO DMA descriptor size (des1) register defines
 */
#define MCI_DMADES1_BS1(x)      (x)				/*!< Size of buffer 1 */
#define MCI_DMADES1_BS2(x)      ((x) << 13)		/*!< Size of buffer 2 */
#define MCI_DMADES1_MAXTR       4096			/*!< Max transfer size per buffer */

/** @brief SDIO Interrupt status & mask register defines
 */
#define MCI_INT_SDIO            (1 << 16)		/*!< SDIO interrupt */
#define MCI_INT_EBE             (1 << 15)		/*!< End-bit error */
#define MCI_INT_ACD             (1 << 14)		/*!< Auto command done */
#define MCI_INT_SBE             (1 << 13)		/*!< Start bit error */
#define MCI_INT_HLE             (1 << 12)		/*!< Hardware locked error */
#define MCI_INT_FRUN            (1 << 11)		/*!< FIFO overrun/underrun error */
#define MCI_INT_HTO             (1 << 10)		/*!< Host data starvation error */
#define MCI_INT_DTO             (1 << 9)		/*!< Data timeout error */
#define MCI_INT_RTO             (1 << 8)		/*!< Response timeout error */
#define MCI_INT_DCRC            (1 << 7)		/*!< Data CRC error */
#define MCI_INT_RCRC            (1 << 6)		/*!< Response CRC error */
#define MCI_INT_RXDR            (1 << 5)		/*!< RX data ready */
#define MCI_INT_TXDR            (1 << 4)		/*!< TX data needed */
#define MCI_INT_DATA_OVER       (1 << 3)		/*!< Data transfer over */
#define MCI_INT_CMD_DONE        (1 << 2)		/*!< Command done */
#define MCI_INT_RESP_ERR        (1 << 1)		/*!< Command response error */
#define MCI_INT_CD              (1 << 0)		/*!< Card detect */

/** @brief SDIO Command register defines
 */
#define MCI_CMD_START           (1UL << 31)		/*!< Start command */
#define MCI_CMD_VOLT_SWITCH     (1 << 28)		/*!< Voltage switch bit */
#define MCI_CMD_BOOT_MODE       (1 << 27)		/*!< Boot mode */
#define MCI_CMD_DISABLE_BOOT    (1 << 26)		/*!< Disable boot */
#define MCI_CMD_EXPECT_BOOT_ACK (1 << 25)		/*!< Expect boot ack */
#define MCI_CMD_ENABLE_BOOT     (1 << 24)		/*!< Enable boot */
#define MCI_CMD_CCS_EXP         (1 << 23)		/*!< CCS expected */
#define MCI_CMD_CEATA_RD        (1 << 22)		/*!< CE-ATA read in progress */
#define MCI_CMD_UPD_CLK         (1 << 21)		/*!< Update clock register only */
#define MCI_CMD_INIT            (1 << 15)		/*!< Send init sequence */
#define MCI_CMD_STOP            (1 << 14)		/*!< Stop/abort command */
#define MCI_CMD_PRV_DAT_WAIT    (1 << 13)		/*!< Wait before send */
#define MCI_CMD_SEND_STOP       (1 << 12)		/*!< Send auto-stop */
#define MCI_CMD_STRM_MODE       (1 << 11)		/*!< Stream transfer mode */
#define MCI_CMD_DAT_WR          (1 << 10)		/*!< Read(0)/Write(1) selection */
#define MCI_CMD_DAT_EXP         (1 << 9)		/*!< Data expected */
#define MCI_CMD_RESP_CRC        (1 << 8)		/*!< Check response CRC */
#define MCI_CMD_RESP_LONG       (1 << 7)		/*!< Response length */
#define MCI_CMD_RESP_EXP        (1 << 6)		/*!< Response expected */
#define MCI_CMD_INDX(n)         ((n) & 0x1F)

/** @brief  SDIO chained DMA descriptor
 */
typedef struct {
	volatile uint32_t des0;						/*!< Control and status */
	volatile uint32_t des1;						/*!< Buffer size(s) */
	volatile uint32_t des2;						/*!< Buffer address pointer 1 */
	volatile uint32_t des3;						/*!< Buffer address pointer 2 */
} pSDMMC_DMA_T;

/** @brief  SDIO device type
 */
typedef struct _sdif_device {
	/* MCI_IRQ_CB_FUNC_T irq_cb; */
	pSDMMC_DMA_T mci_dma_dd[1 + (0x10000 / MCI_DMADES1_MAXTR)];
	/* uint32_t sdio_clk_rate; */
	/* uint32_t sdif_slot_clk_rate; */
	/* int32_t clock_enabled; */
} sdif_device;

//end c&p from sdif_18xx_43xx.h

/** @defgroup SDMMC_18XX_43XX CHIP: LPC18xx/43xx SD/MMC driver
 * @ingroup CHIP_18XX_43XX_Drivers
 * @{
 */

#define CMD_MASK_RESP       (0x3UL << 28)
#define CMD_RESP(r)         (((r) & 0x3) << 28)
#define CMD_RESP_R0         (0 << 28)
#define CMD_RESP_R1         (1 << 28)
#define CMD_RESP_R2         (2 << 28)
#define CMD_RESP_R3         (3 << 28)
#define CMD_BIT_AUTO_STOP   (1 << 24)
#define CMD_BIT_APP         (1 << 23)
#define CMD_BIT_INIT        (1 << 22)
#define CMD_BIT_BUSY        (1 << 21)
#define CMD_BIT_LS          (1 << 20)	/* Low speed, used during acquire */
#define CMD_BIT_DATA        (1 << 19)
#define CMD_BIT_WRITE       (1 << 18)
#define CMD_BIT_STREAM      (1 << 17)
#define CMD_MASK_CMD        (0xff)
#define CMD_SHIFT_CMD       (0)

#define CMD(c, r)        ( ((c) &  CMD_MASK_CMD) | CMD_RESP((r)) )

#define CMD_IDLE            CMD(MMC_GO_IDLE_STATE, 0) | CMD_BIT_LS    | CMD_BIT_INIT
#define CMD_SD_OP_COND      CMD(SD_APP_OP_COND, 1)      | CMD_BIT_LS | CMD_BIT_APP
#define CMD_SD_SEND_IF_COND CMD(SD_CMD8, 1)      | CMD_BIT_LS
#define CMD_MMC_OP_COND     CMD(MMC_SEND_OP_COND, 3)    | CMD_BIT_LS | CMD_BIT_INIT
#define CMD_ALL_SEND_CID    CMD(MMC_ALL_SEND_CID, 2)    | CMD_BIT_LS
#define CMD_MMC_SET_RCA     CMD(MMC_SET_RELATIVE_ADDR, 1) | CMD_BIT_LS
#define CMD_SD_SEND_RCA     CMD(SD_SEND_RELATIVE_ADDR, 1) | CMD_BIT_LS
#define CMD_SEND_CSD        CMD(MMC_SEND_CSD, 2) | CMD_BIT_LS
#define CMD_SEND_EXT_CSD    CMD(MMC_SEND_EXT_CSD, 1) | CMD_BIT_LS | CMD_BIT_DATA
#define CMD_DESELECT_CARD   CMD(MMC_SELECT_CARD, 0)
#define CMD_SELECT_CARD     CMD(MMC_SELECT_CARD, 1)
#define CMD_SET_BLOCKLEN    CMD(MMC_SET_BLOCKLEN, 1)
#define CMD_SEND_STATUS     CMD(MMC_SEND_STATUS, 1)
#define CMD_READ_SINGLE     CMD(MMC_READ_SINGLE_BLOCK, 1) | CMD_BIT_DATA
#define CMD_READ_MULTIPLE   CMD(MMC_READ_MULTIPLE_BLOCK, 1) | CMD_BIT_DATA | CMD_BIT_AUTO_STOP
#define CMD_SD_SET_WIDTH    CMD(SD_APP_SET_BUS_WIDTH, 1) | CMD_BIT_APP
#define CMD_STOP            CMD(MMC_STOP_TRANSMISSION, 1) | CMD_BIT_BUSY
#define CMD_WRITE_SINGLE    CMD(MMC_WRITE_BLOCK, 1) | CMD_BIT_DATA | CMD_BIT_WRITE
#define CMD_WRITE_MULTIPLE  CMD(MMC_WRITE_MULTIPLE_BLOCK, 1) | CMD_BIT_DATA | CMD_BIT_WRITE | CMD_BIT_AUTO_STOP

/* Card specific setup data */
typedef struct _mci_card_struct {
	sdif_device sdif_dev;
	SDMMC_CARD_T card_info;
} mci_card_struct;


void sd_init();
void sd_shutdown();

/** gets the response fields.
@param response buffer to hold the response. Must be at least 4*uint32_t. */
void sd_get_response(uint32_t* response);


/** sends a command to the CIU (card interface unit)
@param cmd command to send
@param arg command argument */
int32_t sd_send_command(uint32_t cmd, uint32_t arg);

/**
 * @brief	Get card's current state (idle, transfer, program, etc.)
 * @return	Current SD card transfer state
 */
int32_t Chip_SDMMC_GetState();

/**
 * @brief	Function to enumerate the SD/MMC/SDHC/MMC+ cards
 * @param	pcardinfo	: Pointer to pre-allocated card info structure
 * @return	1 if a card is acquired, otherwise 0
 */
uint32_t Chip_SDMMC_Acquire(mci_card_struct *pcardinfo);

/**
 * @brief	Get the device size of SD/MMC card (after enumeration)
 * @return	Card size in number of bytes (capacity)
 */
int32_t Chip_SDMMC_GetDeviceSize();

/**
 * @brief	Get the number of device blocks of SD/MMC card (after enumeration)
 * Since Chip_SDMMC_GetDeviceSize is limited to 32 bits cards with greater than
 * 2 GBytes of data will not be correct, in such cases users can use this function
 * to get the size of the card in blocks.
 * @return	Number of 512 bytes blocks in the card
 */
int32_t Chip_SDMMC_GetDeviceBlocks();

/**
 * @brief	Performs the read of data from the SD/MMC card
 * @param	buffer		: Pointer to data buffer to copy to
 * @param	start_block	: Start block number
 * @param	num_blocks	: Number of block to read
 * @return	Bytes read, or 0 on error
 */
int32_t Chip_SDMMC_ReadBlocks(void *buffer, int32_t start_block, int32_t num_blocks);

/**
 * @brief	Performs write of data to the SD/MMC card
 * @param	buffer		: Pointer to data buffer to copy to
 * @param	start_block	: Start block number
 * @param	num_blocks	: Number of block to write
 * @return	Number of bytes actually written, or 0 on error
 */
int32_t Chip_SDMMC_WriteBlocks(void *buffer, int32_t start_block, int32_t num_blocks);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __SDMMC_18XX_43XX_H_ */

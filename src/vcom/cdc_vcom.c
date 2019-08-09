/*
 * @brief Virtual Comm port call back routines
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2013
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
#include "../Drivers/chip.h"
#include <string.h>
#include "app_usbd_cfg.h"
#include "cdc_vcom.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/**
 * Global variable to hold Virtual COM port control data.
 */
VCOM_DATA_T g_vCOM;

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* VCOM bulk EP_IN endpoint handler */
static ErrorCode_t VCOM_bulk_in_hdlr(USBD_HANDLE_T hUsb, void *data, uint32_t event)
{
	VCOM_DATA_T *pVcom = (VCOM_DATA_T *) data;

	switch(event) {
	case USB_EVT_IN:
	//	pVcom->tx_flags &= ~VCOM_TX_BUSY;
		if(pVcom->tx_count > 0)
		{
			pVcom->tx_count -= USBD_API->hw->WriteEP(pVcom->hUsb, USB_CDC_IN_EP, pVcom->tx_buff, pVcom->tx_count);
		}
		else
		{
			pVcom->tx_flags &= ~VCOM_TX_BUSY;
		}
		break;
	default:
		while(1);
		break;
	}

	return LPC_OK;
}

/* VCOM bulk EP_OUT endpoint handler */
static ErrorCode_t VCOM_bulk_out_hdlr(USBD_HANDLE_T hUsb, void *data, uint32_t event)
{
	VCOM_DATA_T *pVcom = (VCOM_DATA_T *) data;

	switch (event) {
	case USB_EVT_OUT:
		if(pVcom->rx_count > (VCOM_RX_BUF_SZ - 64)) {
		//	pVcom->rx_count = 0;  // Avoid buffer overflow (by discarding data)
			pVcom->rx_flags |= VCOM_RX_BUF_FULL;
		}
		else {
			pVcom->rx_count += USBD_API->hw->ReadEP(hUsb, USB_CDC_OUT_EP, &pVcom->rx_buff[pVcom->rx_count]);
		}
		if (pVcom->rx_flags & VCOM_RX_BUF_QUEUED) {
			pVcom->rx_flags &= ~VCOM_RX_BUF_QUEUED;
			if (pVcom->rx_count != 0) {
				pVcom->rx_flags |= VCOM_RX_BUF_FULL;
			}

		}
		else if (pVcom->rx_flags & VCOM_RX_DB_QUEUED) {
			pVcom->rx_flags &= ~VCOM_RX_DB_QUEUED;
			pVcom->rx_flags |= VCOM_RX_DONE;
		}
		break;

	case USB_EVT_OUT_NAK:
		/* queue free buffer for RX */
		if ((pVcom->rx_flags & (VCOM_RX_BUF_FULL | VCOM_RX_BUF_QUEUED)) == 0) {
			USBD_API->hw->ReadReqEP(hUsb, USB_CDC_OUT_EP, pVcom->rx_buff, VCOM_RX_BUF_SZ);
			pVcom->rx_flags |= VCOM_RX_BUF_QUEUED;
		}
		break;

	default:
		while(1);
		break;
	}

	return LPC_OK;
}

/* Set line coding call back routine */
static ErrorCode_t VCOM_SetLineCode(USBD_HANDLE_T hCDC, CDC_LINE_CODING *line_coding)
{
	VCOM_DATA_T *pVcom = &g_vCOM;

	/* Called when baud rate is changed/set. Using it to know host connection state */
	pVcom->tx_flags = VCOM_TX_CONNECTED;	/* reset other flags */

	return LPC_OK;
}

// hang on each of this callbacks to notice it's being used
static ErrorCode_t CDC_BulkIN_Hdlr(USBD_HANDLE_T a, void* b, uint32_t c)
{
	while(1);
	return LPC_OK;
}
static ErrorCode_t CDC_BulkOUT_Hdlr(USBD_HANDLE_T a, void* b, uint32_t c)
{
	while(1);
	return LPC_OK;
}
static ErrorCode_t CDC_InterruptEP_Hdlr(USBD_HANDLE_T a, void* b, uint32_t c) // optional
{
	while(1);
	return LPC_OK;
}
//static ErrorCode_t CDC_Ep0_Hdlr(USBD_HANDLE_T a, void* b, uint32_t c) // optional overrideable
//{
//	return LPC_OK; // recibí USB_EVT_RESET; USB_EVT_SETUP
//}
static ErrorCode_t SendEncpsCmd(USBD_HANDLE_T a, uint8_t* b, uint16_t c)
{
	while(1);
	return LPC_OK;
}
static ErrorCode_t GetEncpsResp(USBD_HANDLE_T a, uint8_t** b, uint16_t* c)
{
	while(1);
	return LPC_OK;
}
static ErrorCode_t SetCommFeature(USBD_HANDLE_T a, uint16_t b, uint8_t* c, uint16_t d)
{
	while(1);
	return LPC_OK;
}
static ErrorCode_t GetCommFeature(USBD_HANDLE_T a, uint16_t b, uint8_t** c, uint16_t* d)
{
	while(1);
	return LPC_OK;
}
static ErrorCode_t ClrCommFeature(USBD_HANDLE_T a, uint16_t b)
{
	while(1);
	return LPC_OK;
}
static ErrorCode_t SetCtrlLineState(USBD_HANDLE_T a, uint16_t b)
{
	// llamada al conectar el cable usb, no parece ser importante
	return ERR_USBD_UNHANDLED;
}
static ErrorCode_t SendBreak(USBD_HANDLE_T a, uint16_t b)
{
	while(1);
	return LPC_OK;
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Virtual com port init routine */
ErrorCode_t vcom_init(USBD_HANDLE_T hUsb, USB_CORE_DESCS_T *pDesc, USBD_API_INIT_PARAM_T *pUsbParam)
{
	USBD_CDC_INIT_PARAM_T cdc_param;
	ErrorCode_t ret = LPC_OK;
	uint32_t ep_indx;

	g_vCOM.hUsb = hUsb;
//	memset((void *) &cdc_param, 0, sizeof(USBD_CDC_INIT_PARAM_T));
	cdc_param.mem_base = pUsbParam->mem_base;
	cdc_param.mem_size = pUsbParam->mem_size;
	cdc_param.cif_intf_desc = (uint8_t *) find_IntfDesc(pDesc->high_speed_desc, CDC_COMMUNICATION_INTERFACE_CLASS);
	cdc_param.dif_intf_desc = (uint8_t *) find_IntfDesc(pDesc->high_speed_desc, CDC_DATA_INTERFACE_CLASS);
	cdc_param.CIC_GetRequest = NULL;
	cdc_param.CIC_SetRequest = NULL;
	cdc_param.CDC_BulkIN_Hdlr = CDC_BulkIN_Hdlr;
	cdc_param.CDC_BulkOUT_Hdlr = CDC_BulkOUT_Hdlr;
	cdc_param.SendEncpsCmd = SendEncpsCmd;
	cdc_param.GetEncpsResp = GetEncpsResp;
	cdc_param.SetCommFeature = SetCommFeature;
	cdc_param.GetCommFeature = GetCommFeature;
	cdc_param.ClrCommFeature = ClrCommFeature;
	cdc_param.SetCtrlLineState = SetCtrlLineState;
	cdc_param.SendBreak = SendBreak;
	cdc_param.SetLineCode = VCOM_SetLineCode;
	cdc_param.CDC_InterruptEP_Hdlr = CDC_InterruptEP_Hdlr;
	cdc_param.CDC_Ep0_Hdlr = NULL;//CDC_Ep0_Hdlr;

	ret = USBD_API->cdc->init(hUsb, &cdc_param, &g_vCOM.hCdc);

	if (ret == LPC_OK) {
		/* allocate transfer buffers */
		g_vCOM.rx_buff = (uint8_t *) cdc_param.mem_base;
		cdc_param.mem_base += VCOM_RX_BUF_SZ;
		cdc_param.mem_size -= VCOM_RX_BUF_SZ;

		g_vCOM.tx_buff = (uint8_t *) cdc_param.mem_base;
		cdc_param.mem_base += VCOM_TX_BUF_SZ;
		cdc_param.mem_size -= VCOM_TX_BUF_SZ;

		/* register endpoint interrupt handler */
		ep_indx = (((USB_CDC_IN_EP & 0x0F) << 1) + 1);
		ret = USBD_API->core->RegisterEpHandler(hUsb, ep_indx, VCOM_bulk_in_hdlr, &g_vCOM);
		if (ret == LPC_OK) {
			/* register endpoint interrupt handler */
			ep_indx = ((USB_CDC_OUT_EP & 0x0F) << 1);
			ret = USBD_API->core->RegisterEpHandler(hUsb, ep_indx, VCOM_bulk_out_hdlr, &g_vCOM);
		}
		/* update mem_base and size variables for cascading calls. */
		pUsbParam->mem_base = cdc_param.mem_base;
		pUsbParam->mem_size = cdc_param.mem_size;
	}

	return ret;
}

/* Virtual com port buffered read routine */
uint32_t vcom_bread(uint8_t *pBuf, uint32_t buf_len)
{
	VCOM_DATA_T *pVcom = &g_vCOM;
	uint16_t cnt = 0;
	/* read from the default buffer if any data present */
	if (pVcom->rx_count) {
		/* enter critical section */
		NVIC_DisableIRQ(USB_IRQn);

		cnt = (pVcom->rx_count < buf_len) ? pVcom->rx_count : buf_len;
		memcpy(pBuf, pVcom->rx_buff, cnt);
		pVcom->rx_rd_count += cnt;

		if((pVcom->rx_flags & VCOM_RX_BUF_FULL) &&
				(pVcom->rx_count - cnt < 64)) {
			pVcom->rx_flags &= ~VCOM_RX_BUF_FULL;
			pVcom->rx_count += USBD_API->hw->ReadEP(pVcom->hUsb, USB_CDC_OUT_EP,
										&pVcom->rx_buff[pVcom->rx_count - cnt]);
		}
		if (pVcom->rx_rd_count >= pVcom->rx_count) {
			pVcom->rx_flags &= ~VCOM_RX_BUF_FULL;
			pVcom->rx_rd_count = pVcom->rx_count = 0;
		}
		/* exit critical section */
		NVIC_EnableIRQ(USB_IRQn);
	}
	return cnt;

}

/* Virtual com port read routine */
ErrorCode_t vcom_read_req(uint8_t *pBuf, uint32_t len)
{
	VCOM_DATA_T *pVcom = &g_vCOM;

	/* check if we queued Rx buffer */
	if (pVcom->rx_flags & (VCOM_RX_BUF_QUEUED | VCOM_RX_DB_QUEUED)) {
		return ERR_BUSY;
	}
	/* enter critical section */
	NVIC_DisableIRQ(USB_IRQn);
	/* if not queue the request and return 0 bytes */
	USBD_API->hw->ReadReqEP(pVcom->hUsb, USB_CDC_OUT_EP, pBuf, len);
	/* exit critical section */
	NVIC_EnableIRQ(USB_IRQn);
	pVcom->rx_flags |= VCOM_RX_DB_QUEUED;

	return LPC_OK;
}

/* Gets current read count. */
uint32_t vcom_read_cnt(void)
{
	VCOM_DATA_T *pVcom = &g_vCOM;
	uint32_t ret = 0;

	if (pVcom->rx_flags & VCOM_RX_DONE) {
		ret = pVcom->rx_count;
		pVcom->rx_count = 0;
	}

	return ret;
}

/* Virtual com port write routine*/
uint32_t vcom_write(uint8_t *pBuf, uint32_t len)
{
	VCOM_DATA_T *pVcom = &g_vCOM;
	uint32_t ret = 0;

	if ( pVcom->tx_flags & VCOM_TX_CONNECTED ) {

		if( (pVcom->tx_flags & VCOM_TX_BUSY) == 0 ) {

			pVcom->tx_flags |= VCOM_TX_BUSY;

			/* enter critical section */
			NVIC_DisableIRQ(USB_IRQn);
			ret = USBD_API->hw->WriteEP(pVcom->hUsb, USB_CDC_IN_EP, pBuf, len);
			/* exit critical section */
			NVIC_EnableIRQ(USB_IRQn);
		}
		else
		{
			if (len + (pVcom->tx_count) > VCOM_TX_BUF_SZ) {
				len = VCOM_TX_BUF_SZ - pVcom->tx_count;
			}
			if(len)
			{
				/* enter critical section */
				NVIC_DisableIRQ(USB_IRQn);
				memcpy(&pVcom->tx_buff[pVcom->tx_count], pBuf, len);
				pVcom->tx_count += len;
				ret = len;//pVcom->tx_count;
				/* exit critical section */
				NVIC_EnableIRQ(USB_IRQn);
			}
		}
	}

	return ret;
}

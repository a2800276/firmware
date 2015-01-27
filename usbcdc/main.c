#include "everykey/everykey.h"

//USB descriptors

uint8_t usb_dev_desc[] = {/*TODO*/};
uint8_t usb_fs_conf_desc[] = {/*TODO*/};
uint8_t* usb_string_desc[] =  {/*TODO*/};
uint8_t cdc_control_intf_desc[] = {/*TODO*/};
uint8_t cdc_data_intf_desc[] = {/*TODO*/};

// USB driver memory

#define USB_MEM_BASE ((uint8_t*)(0x20000000))
#define USB_MEM_SIZE 0x2000

#define CDC_MEM_BASE ((uint8_t*)(0x20002000))
#define CDC_MEM_SIZE 0x2000

//param structs for USB driver

USB_CORE_DESCS coreDescs = {
	usb_dev_desc,
	usb_string_desc,
	usb_fs_conf_desc,
	usb_fs_conf_desc,
	NULL
};


USB_API_INIT_PARAM usbInit = {
	(uint32)(&(USB_HW[1])),   //usb_reg_base
	(uint32)(USB_MEM_BASE),   //mem_base
  4,                        //max_num_ep
  {0,0,0},                  //pad0[3]
  NULL,                     //USB_Reset_Event
  NULL,                     //USB_Suspend_Event
  NULL,                     //USB_Resume_Event
  NULL,                     //reserved_sbz
  NULL,                     //USB_SOF_Event
  NULL,                     //USB_WakeUpCfg
  NULL,                     //USB_Power_Event
  NULL,                     //USB_Error_Event
  NULL,                     //USB_Configure_Event
  NULL,                     //USB_Interface_Event
  NULL,                     //USB_Feature_Event
  NULL,                     //virt_to_phy
  NULL                      //cache_flush
};


USB_CDC_INIT_PARAM cdcInit = {
	CDC_MEM_BASE,     //mem_base
	CDC_MEM_SIZE,     //mem_size
	cdc_control_desc, //cif_intf_desc
	cdc_data_desc,    //dif_intf_desc
	NULL,             //CIC_GetRequest
  NULL,             //CIC_SetRequest
  NULL,             //CDC_BulkIN_Hdlr
  NULL,             //CDC_BulkOUT_Hdlr
  NULL,             //SendEncpsCmd
  NULL,             //GetEncpsResp
  NULL,             //SetCommFeature
  NULL,             //GetCommFeature
  NULL,             //ClrCommFeature
  NULL,             //SetCtrlLineState
  NULL,             //SendBreak
  NULL,             //SetLineCode
  NULL,             //CDC_InterruptEP_Hdlr
  NULL,             //CDC_Ep0_Hdlr
	/* TODO: Which ones need to be set? */
};

USB_HANDLE usbHandle;
USB_HANDLE cdcHandle;

void fail(uint8_t reason) {
	write_pin(LED3_PIN, true);
	while (1) {}
}

void usb0_handler(void) {
    USB_ROM_API->hw->ISR(usbHandle);
}

void main(void) {
	sparrow_init();


	ROM_ERR ret = USB_ROM_API->hw->Init(&usbHandle, &coreDescs, &usbInit);
	if (ret) fail(1);

	#
	#
	uint32_t usbLen = USB_ROM_API->hw->GetMemSize(&usbInit);

	uint32_t cdclen = USB_ROM_API->cdc->GetMemSize(&cdcInit);
	USB_ROM_API->cdc->Init(usbHandle, cdcInit, &cdcHandle);
}

void systick() {
	static uint32_t counter;
	if (read_pin(BUTTON_PIN)) counter++;
	else counter--;

	write_pin(LED1_PIN, (counter / 50) & 1);
	write_pin(LED2_PIN, ((counter + 10) / 50) & 1);
	write_pin(LED3_PIN, ((counter + 20) / 50) & 1);
}

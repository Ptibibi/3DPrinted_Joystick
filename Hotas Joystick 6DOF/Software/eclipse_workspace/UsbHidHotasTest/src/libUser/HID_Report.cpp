 /*******************************************************************************
 HID_Report.h
 V 0.01

 Description:

 ********************************************************************************
 ********************************************************************************
 V0.01 --> 13/04/15	Edition originale by BRB
 *******************************************************************************/

//#include "usb_device.h"
#include "HID_Report.h"


CHidReport::CHidReport() {
	/* Init Device Library,Add Supported Class and Start the library*/
	USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS);

	USBD_RegisterClass(&hUsbDeviceFS, &USBD_CUSTOM_HID);

	USBD_CUSTOM_HID_RegisterInterface(&hUsbDeviceFS, &USBD_CustomHID_fops_FS);

	USBD_Start(&hUsbDeviceFS);
}

CHidReport::~CHidReport() {
}

void CHidReport::initialize() {
}

void CHidReport::sendReport(GPIO_PinState* pButton, int16_t rx, int16_t ry, int16_t rz, int16_t x, int16_t y, int16_t throttleValue) {
  int i;
  for (i = 0; i < 64; i++)
  {
    if (*(pButton + i) == true)
    	HotasReport.buttons |= (uint64_t)1 << i;
    else
    	HotasReport.buttons &= ~((uint64_t)1 << i);
  }

  HotasReport.rxAxis = rx;
  HotasReport.ryAxis = ry;
  HotasReport.rzAxis = rz;
  HotasReport.xAxis = x;
  HotasReport.yAxis = y;

  HotasReport.throttle = throttleValue;

  HotasReport.dPad1 = 0;
  HotasReport.dPad2 = 0;

  USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, (uint8_t*)&HotasReport, 87);
}

 /*******************************************************************************
 HID_Report.h
 V 0.01

 Description:

 ********************************************************************************
 ********************************************************************************
 V0.01 --> 13/04/15	Edition originale by BRB
 *******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _HID_REPORT_
#define _HID_REPORT_
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "../src/usb/usbd_def.h"
#include "../src/usb/usbd_core.h"
#include "../src/usb/usbd_desc.h"
#include "../src/usb/usbd_customhid.h"
#include "../src/usb/usbd_custom_hid_if.h"

extern USBD_HandleTypeDef hUsbDeviceFS;

typedef union {
	uint64_t buttons;

	struct{
		uint8_t button1 : 1;
		uint8_t button2 : 1;
		uint8_t button3 : 1;
		uint8_t button4 : 1;
		uint8_t button5 : 1;
		uint8_t button6 : 1;
		uint8_t button7 : 1;
		uint8_t button8 : 1;

		uint8_t button9 : 1;
		uint8_t button10 : 1;
		uint8_t button11 : 1;
		uint8_t button12 : 1;
		uint8_t button13 : 1;
		uint8_t button14 : 1;
		uint8_t button15 : 1;
		uint8_t button16 : 1;

		uint8_t button17 : 1;
		uint8_t button18 : 1;
		uint8_t button19 : 1;
		uint8_t button20 : 1;
		uint8_t button21 : 1;
		uint8_t button22 : 1;
		uint8_t button23 : 1;
		uint8_t button24 : 1;

		uint8_t button25 : 1;
		uint8_t button26 : 1;
		uint8_t button27 : 1;
		uint8_t button28 : 1;
		uint8_t button29 : 1;
		uint8_t button30 : 1;
		uint8_t button31 : 1;
		uint8_t button32 : 1;

		uint8_t button33 : 1;
		uint8_t button34 : 1;
		uint8_t button35 : 1;
		uint8_t button36 : 1;
		uint8_t button37 : 1;
		uint8_t button38 : 1;
		uint8_t button39 : 1;
		uint8_t button40 : 1;

		uint8_t button41 : 1;
		uint8_t button42 : 1;
		uint8_t button43 : 1;
		uint8_t button44 : 1;
		uint8_t button45 : 1;
		uint8_t button46 : 1;
		uint8_t button47 : 1;
		uint8_t button48 : 1;

		uint8_t button49 : 1;
		uint8_t button50 : 1;
		uint8_t button51 : 1;
		uint8_t button52 : 1;
		uint8_t button53 : 1;
		uint8_t button54 : 1;
		uint8_t button55 : 1;
		uint8_t button56 : 1;

		uint8_t button57 : 1;
		uint8_t button58 : 1;
		uint8_t button59 : 1;
		uint8_t button60 : 1;
		uint8_t button61 : 1;
		uint8_t button62 : 1;
		uint8_t button63 : 1;
		uint8_t button64 : 1;

		int16_t	rxAxis;
		int16_t	ryAxis;

		int16_t	rzAxis;
		int16_t	xAxis;

		int16_t	yAxis;
		int16_t	zAxis;
		int16_t	throttle1;
		int16_t	throttle2;

		uint8_t	dPad1 : 4;
		uint8_t	dPad2 : 4;
	};
} HID_HotasReport_Data_t;

class CHidReport {
  //Method
  public:
    CHidReport();
    ~CHidReport();
    void initialize();
    void sendReport(GPIO_PinState* pButton, int16_t rx, int16_t ry, int16_t rz, int16_t x, int16_t y, int16_t z, int16_t throttle1, int16_t throttle2, int16_t dpad1, int16_t dpad2);

  //Variables
  private:
    HID_HotasReport_Data_t HotasReport;

};
#ifdef __cplusplus
}
#endif
#endif /*_HID_REPORT_ */


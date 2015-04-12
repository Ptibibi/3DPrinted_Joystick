/**
  ******************************************************************************
  * @file           : USB_DEVICE
  * @date           : 11/04/2015 11:32:28  
  * @version        : v1.0_Cube
  * @brief          : This file implements the USB Device 
  ******************************************************************************
  *
  * COPYRIGHT(c) 2015 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  * 1. Redistributions of source code must retain the above copyright notice,
  * this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  * this list of conditions and the following disclaimer in the documentation
  * and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of its contributors
  * may be used to endorse or promote products derived from this software
  * without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/

#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_customhid.h"
#include "usbd_custom_hid_if.h"

/* USB Device Core handle declaration */
USBD_HandleTypeDef hUsbDeviceFS;
typedef struct{
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
    int16_t	throttle;

    uint8_t	dPad1 : 4;
    uint8_t	dPad2 : 4;
} HID_HotasReport_Data_t;
static HID_HotasReport_Data_t HotasReport;

/* init function */				        
void MX_USB_DEVICE_Init(void)
{
  /* Init Device Library,Add Supported Class and Start the library*/
  USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS);

  USBD_RegisterClass(&hUsbDeviceFS, &USBD_CUSTOM_HID);

  USBD_CUSTOM_HID_RegisterInterface(&hUsbDeviceFS, &USBD_CustomHID_fops_FS);

  USBD_Start(&hUsbDeviceFS);
}
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

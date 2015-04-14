/**
  ******************************************************************************
  * @file           : usbd_custom_hid_if.c
  * @author         : MCD Application Team
  * @version        : V2.2.0
  * @date           : 13-June-2014
  * @brief          : USB Device Custom HID interface file.
  ******************************************************************************
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
#include "usbd_custom_hid_if.h"


#include "usbd_conf.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

//__ALIGN_BEGIN static uint8_t CUSTOM_HID_ReportDesc_FS[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END =
static uint8_t CUSTOM_HID_ReportDesc_FS[USBD_CUSTOM_HID_REPORT_DESC_SIZE] =
//static uint8_t CUSTOM_HID_ReportDesc_FS[] =
{ //83
  /* USER CODE BEGIN 0 */ 
  //0x00,

  // 6DOF Hotas 8
  0x05, 0x01,							// USAGE_PAGE (Generic Desktop)
  0x09, 0x04,							// USAGE (Joystick)
  0xa1, 0x01,							// COLLECTION (Application)
  0x85, 0x01,							//   REPORT_ID (1)
  // 64 Buttons 16
  0x05, 0x09,							//   USAGE_PAGE (Button)
  0x19, 0x01,							//   USAGE_MINIMUM (Button 1)
  0x29, 0x40,							//   USAGE_MAXIMUM (Button 64)
  0x15, 0x00,							//   LOGICAL_MINIMUM (0)
  0x25, 0x01,							//   LOGICAL_MAXIMUM (1)
  0x75, 0x01,							//   REPORT_SIZE (1)
  0x95, 0x40,							//   REPORT_COUNT (64)
  0x81, 0x02,							//   INPUT (Data,Var,Abs)
  // 6 16bit Axis 29
  0x05, 0x01,							//   USAGE_PAGE (Generic Desktop)
  0xa1, 0x00,							//   COLLECTION (Physical)
  0x09, 0x33,							//     USAGE (Rx)
  0x09, 0x34,							//     USAGE (Ry)
  0x09, 0x35,							//     USAGE (Rz)
  0x09, 0x30,							//     USAGE (X)
  0x09, 0x31,							//     USAGE (Y)
  0x09, 0x32,							//     USAGE (X)
  0x16, 0x00, 0x80,						//     LOGICAL_MINIMUM (-32768)
  0x26, 0xFF, 0x7F,						//     LOGICAL_MAXIMUM (32767)
  0x75, 0x10,							//     REPORT_SIZE (16)
  0x95, 0x06,							//     REPORT_COUNT (6)
  0x81, 0x02,							//     INPUT (Data,Var,Abs)
  0xc0,								//   END_COLLECTION
  // 1 16bit Throttle 16
  0x05, 0x02,							//   USAGE_PAGE (Simulation Controls)
  0x09, 0xBB,							//   USAGE (Throttle)
  0x16, 0x00, 0x80,						//   LOGICAL_MINIMUM (-32768)
  0x26, 0xFF, 0x7F,						//   LOGICAL_MAXIMUM (32767)
  0x75, 0x10,							//   REPORT_SIZE (16)
  0x95, 0x01,							//   REPORT_COUNT (1)
  0x81, 0x02,							//   INPUT (Data,Var,Abs)
  // 2 Hat Switches 16
  0x05, 0x01,							//   USAGE_PAGE (Generic Desktop)
  0x09, 0x39,							//   USAGE (Hat switch)
  0x09, 0x39,							//   USAGE (Hat switch)
  0x15, 0x01,							//   LOGICAL_MINIMUM (1)
  0x25, 0x08,							//   LOGICAL_MAXIMUM (8)
  0x95, 0x02,							//   REPORT_COUNT (2)
  0x75, 0x04,							//   REPORT_SIZE (4)
  0x81, 0x02,							//   INPUT (Data,Var,Abs)

  /* USER CODE END 0 */ 
  0xC0    /*     END_COLLECTION	             */
   
}; 
/* USB handler declaration */
/* Handle for USB Full Speed IP */
USBD_HandleTypeDef  *hUsbDevice_0;

extern USBD_HandleTypeDef hUsbDeviceFS;

/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static int8_t CUSTOM_HID_Init_FS     (void);
static int8_t CUSTOM_HID_DeInit_FS   (void);
static int8_t CUSTOM_HID_OutEvent_FS (uint8_t event_idx, uint8_t state);
/* USER CODE BEGIN 2 */ 

/* USER CODE END 2 */ 

USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops_FS = 
{
  CUSTOM_HID_ReportDesc_FS,
  CUSTOM_HID_Init_FS,
  CUSTOM_HID_DeInit_FS,
  CUSTOM_HID_OutEvent_FS,
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  CUSTOM_HID_Init_FS
  *         Initializes the CUSTOM HID media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_Init_FS(void)
{
  hUsbDevice_0 = &hUsbDeviceFS;
  /* USER CODE BEGIN 4 */ 

  return (0);
  /* USER CODE END 4 */ 
}

/**
  * @brief  CUSTOM_HID_DeInit_FS
  *         DeInitializes the CUSTOM HID media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_DeInit_FS(void)
{
  /* USER CODE BEGIN 5 */ 
  return (0);
  /* USER CODE END 5 */ 
}

/**
  * @brief  CUSTOM_HID_OutEvent_FS
  *         Manage the CUSTOM HID class events       
  * @param  event_idx: event index
  * @param  state: event state
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_OutEvent_FS  (uint8_t event_idx, uint8_t state)
{ 
  /* USER CODE BEGIN 6 */
  return (0);
  /* USER CODE END 6 */ 
}

/* USER CODE BEGIN 7 */ 
/**
  * @brief  USBD_CUSTOM_HID_SendReport_FS
  *         Send the report to the Host       
  * @param  report: the report to be sent
  * @param  len: the report length
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
/*static int8_t USBD_CUSTOM_HID_SendReport_FS ( uint8_t *report,uint16_t len)
{

  return USBD_CUSTOM_HID_SendReport(hUsbDevice_0, report, len);
}*/

/* USER CODE END 7 */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

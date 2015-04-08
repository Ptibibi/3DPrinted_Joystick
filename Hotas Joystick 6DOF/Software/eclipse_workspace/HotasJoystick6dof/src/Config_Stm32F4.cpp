#include "Config_Stm32F4.h"

void setConfigGPIO(GPIO_TypeDef* GPIO_Def, GPIO_InitTypeDef GPIO_InitDefInput) {
	// Enable GPIO Peripheral clock
	uint32_t portNumber = (uint32_t)GPIO_Def - (uint32_t)GPIOA_BASE;
	RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN << (portNumber));

	GPIO_InitTypeDef GPIO_InitDef;
	GPIO_InitDef.Pin = GPIO_InitDefInput.Pin;
	GPIO_InitDef.Mode = GPIO_InitDefInput.Mode;
	GPIO_InitDef.Pull = GPIO_InitDefInput.Pull;
	GPIO_InitDef.Speed = GPIO_InitDefInput.Speed;
	GPIO_InitDef.Alternate = GPIO_InitDefInput.Alternate;
	HAL_GPIO_Init(GPIO_Def, &GPIO_InitDef);
}

ADC_HandleTypeDef setConfigADC(GPIO_TypeDef* GPIO_Def, GPIO_InitTypeDef GPIO_InitDefInput, ADC_HandleTypeDef ADC_HandleDefInput, ADC_ChannelConfTypeDef ADC_ChannelConfDefInput) {
	setConfigGPIO(GPIO_Def, GPIO_InitDefInput);

	ADC_HandleTypeDef ADC_HandleDef;
	HAL_ADC_MspInit(&ADC_HandleDef);
	if (ADC_HandleDefInput.Instance)
		ADC_HandleDef.Instance = ADC_HandleDefInput.Instance; //ADC1;
	if (ADC_HandleDefInput.Init.ClockPrescaler)
		ADC_HandleDef.Init.ClockPrescaler = ADC_HandleDefInput.Init.ClockPrescaler; //ADC_CLOCKPRESCALER_PCLK_DIV8;
	if (ADC_HandleDefInput.Init.Resolution)
		ADC_HandleDef.Init.Resolution = ADC_HandleDefInput.Init.Resolution; //ADC_RESOLUTION12b;
	if (ADC_HandleDefInput.Init.DataAlign)
		ADC_HandleDef.Init.DataAlign = ADC_HandleDefInput.Init.DataAlign; //ADC_DATAALIGN_RIGHT;
	if (ADC_HandleDefInput.Init.ScanConvMode)
		ADC_HandleDef.Init.ScanConvMode = ADC_HandleDefInput.Init.ScanConvMode; //ENABLE;
	if (ADC_HandleDefInput.Init.ScanConvMode)
		ADC_HandleDef.Init.EOCSelection = ADC_HandleDefInput.Init.ScanConvMode; //EOC_SEQ_CONV;
	if (ADC_HandleDefInput.Init.ContinuousConvMode)
		ADC_HandleDef.Init.ContinuousConvMode = ADC_HandleDefInput.Init.ContinuousConvMode; //ENABLE;
	if (ADC_HandleDefInput.Init.DMAContinuousRequests)
		ADC_HandleDef.Init.DMAContinuousRequests = ADC_HandleDefInput.Init.DMAContinuousRequests; //ENABLE;
	if (ADC_HandleDefInput.Init.NbrOfDiscConversion)
		ADC_HandleDef.Init.NbrOfDiscConversion = ADC_HandleDefInput.Init.NbrOfDiscConversion; //1;
	if (ADC_HandleDefInput.Init.DiscontinuousConvMode)
		ADC_HandleDef.Init.DiscontinuousConvMode = ADC_HandleDefInput.Init.DiscontinuousConvMode; //DISABLE;
	if (ADC_HandleDefInput.Init.NbrOfConversion)
		ADC_HandleDef.Init.NbrOfConversion = ADC_HandleDefInput.Init.NbrOfConversion; //8;
	if (ADC_HandleDefInput.Init.ExternalTrigConvEdge)
		ADC_HandleDef.Init.ExternalTrigConvEdge = ADC_HandleDefInput.Init.ExternalTrigConvEdge; //ADC_EXTERNALTRIGCONVEDGE_NONE;
	if (ADC_HandleDefInput.Init.ExternalTrigConv)
		ADC_HandleDef.Init.ExternalTrigConv = ADC_HandleDefInput.Init.ExternalTrigConv; //ADC_EXTERNALTRIGCONV_T1_CC1;
	HAL_ADC_Init(&ADC_HandleDef);

	ADC_ChannelConfTypeDef ADC_ChannelConfDef;
	if (ADC_ChannelConfDefInput.Channel)
		ADC_ChannelConfDef.Channel = ADC_ChannelConfDefInput.Channel; //ADC_CHANNEL_0;
	if (ADC_ChannelConfDefInput.Rank)
		ADC_ChannelConfDef.Rank = ADC_ChannelConfDefInput.Rank; //1;
	if (ADC_ChannelConfDefInput.SamplingTime)
		ADC_ChannelConfDef.SamplingTime = ADC_ChannelConfDefInput.SamplingTime; //ADC_SAMPLETIME_3CYCLES;
	if (ADC_ChannelConfDefInput.Offset)
		ADC_ChannelConfDef.Offset = ADC_ChannelConfDefInput.Offset; //0;
	HAL_ADC_ConfigChannel(&ADC_HandleDef, &ADC_ChannelConfDef);

	HAL_ADC_Start(&ADC_HandleDef);

	return ADC_HandleDef;
}

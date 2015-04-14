#include "Analog_Measure.h"

CAnalogMeasure::CAnalogMeasure(_gpioxConfig gpioConfig, ADC_HandleTypeDef analogConfig, ADC_ChannelConfTypeDef ADC_ChannelConfDef, int16_t outputMin, int16_t outputMax, uint32_t storageAddr, const char* name) {
	gpioConf = gpioConfig;
	adcHandle = analogConfig;
	adcChannelConf = ADC_ChannelConfDef;
	inputRatioMin = 0;
	inputRatioMax = 0;
	inputRatioNeutral = 0;
	outputRatioMin = outputMin;
	outputRatioMax = outputMax;
	storageAddrMeasure = storageAddr;
	nameMeasure = name;
	coeffAPart1 = 0;
	coeffBPart1 = 0;
	coeffAPart2 = 0;
	coeffBPart2 = 0;

	timer.start();

	//core->SetPin(gpioConf);
	//adcHandle = core->SetAnalog(gpioConf, adcHandle, adcChannelConf);
	adcHandle = setConfigADC(gpioConf.gpiox, gpioConf.initDef, adcHandle, adcChannelConf);
}

CAnalogMeasure::~CAnalogMeasure() {
}

void CAnalogMeasure::initialize() {
  initCoeff();
  updateCoeff();
}

void CAnalogMeasure::initCoeff() {
	/*if (EEPROM.read(storageAddrMeasure) == STORAGE_CODE_OK) {
		inputRatioMin = EEPROM.read(storageAddrMeasure + 1);
		inputRatioMin += EEPROM.read(storageAddrMeasure + 2) << 8;

		inputRatioMax = EEPROM.read(storageAddrMeasure + 3);
		inputRatioMax += EEPROM.read(storageAddrMeasure + 4) << 8;

		inputRatioNeutral = EEPROM.read(storageAddrMeasure + 5);
		inputRatioNeutral += EEPROM.read(storageAddrMeasure + 6) << 8;
	}
	else {*/
		inputRatioMin = 0;
		inputRatioMax = 1024;
		inputRatioNeutral = 0;
	//}
}

void CAnalogMeasure::saveCoeff() {
	/*EEPROM.write(storageAddrMeasure, STORAGE_CODE_OK);
	EEPROM.write(storageAddrMeasure + 1, (inputRatioMin) & 0x00FF);
	EEPROM.write(storageAddrMeasure + 2, (inputRatioMin >> 8) & 0x00FF);

	EEPROM.write(storageAddrMeasure + 3, (inputRatioMax) & 0x00FF);
	EEPROM.write(storageAddrMeasure + 4, (inputRatioMax >> 8) & 0x00FF);

	EEPROM.write(storageAddrMeasure + 5, (inputRatioNeutral) & 0x00FF);
	EEPROM.write(storageAddrMeasure + 6, (inputRatioNeutral >> 8) & 0x00FF);*/
}

void CAnalogMeasure::updateCoeff() {
	float inMin = (float)inputRatioMin;
	float inMax = (float)inputRatioMax;
	float inMiddle = (inMax - inMin) / 2 + inMin;
	float inMiddlePart1 = inMiddle + inputRatioNeutral;
	float inMiddlePart2 = inMiddle - inputRatioNeutral;
	float outMin = (float)outputRatioMin;
	float outMax = (float)outputRatioMax;
	float outMiddle = (outMax - outMin) / 2 + outMin;

	//coeffA = (outMax - outMin) / (inMax - inMin);
	//coeffB = ((inMax * outMin) - (inMin * outMax)) / (inMax - inMin);
	coeffAPart1 = (outMax - outMiddle) / (inMax - inMiddlePart1);
	coeffBPart1 = ((inMax * outMiddle) - (inMiddlePart1 * outMax)) / (inMax - inMiddlePart1);

	coeffAPart2 = (outMiddle - outMin) / (inMiddlePart2 - inMin);
	coeffBPart2 = ((inMiddlePart2 * outMin) - (inMin * outMiddle)) / (inMiddlePart2 - inMin);
}

int32_t CAnalogMeasure::getAnalogValue() {
  return HAL_ADC_GetValue(&adcHandle);
}

int16_t CAnalogMeasure::applicCoeff(int32_t value) {
	float inMin = (float)inputRatioMin;
	float inMax = (float)inputRatioMax;
	float inMiddle = (inMax - inMin) / 2 + inMin;
	float inMiddlePart1 = inMiddle + inputRatioNeutral;
	float inMiddlePart2 = inMiddle - inputRatioNeutral;
	float outMin = (float)outputRatioMin;
	float outMax = (float)outputRatioMax;
	float outMiddle = (outMax - outMin) / 2 + outMin;

	float val = value;
	float pos = 0;

	if (val > inMiddlePart1) {
		pos = coeffAPart1 * val + coeffBPart1;
		if (pos > outMax)
			pos = outMax;
	}
	else if (inMiddlePart2 < val) {
		pos = coeffAPart2 * val + coeffBPart2;
		if (outMin > pos)
			pos = outMin;
	}
	else
		pos = outMiddle;

	return (int16_t)pos;
}

int16_t CAnalogMeasure::getMeasure() {
  return applicCoeff(getAnalogValue());
}

void CAnalogMeasure::setCalibration() {
	trace_printf("CALIBRATION %s Start!\n", nameMeasure);
	timer.sleep(5000);

	trace_printf("CALIBRATION %s Set MAX!\n", nameMeasure);
	timer.sleep(5000);
	int32_t maxValue = getAnalogValue();

	trace_printf("CALIBRATION %s Release!\n", nameMeasure);
	timer.sleep(5000);
	int32_t maxReleaseValue = getAnalogValue();

	trace_printf("CALIBRATION %s Set MIN!\n", nameMeasure);
	timer.sleep(5000);
	int32_t minValue = getAnalogValue();

	trace_printf("CALIBRATION %s Release!\n", nameMeasure);
	timer.sleep(5000);
	int32_t minReleaseValue = getAnalogValue();

	trace_printf("CALIBRATION %s Finished!\n", nameMeasure);
	int32_t inMiddle = ((maxValue - minValue) / 2) + minValue;
	if (abs(maxReleaseValue - inMiddle) > abs(minReleaseValue - inMiddle))
		inputRatioNeutral = (uint16_t)abs(maxReleaseValue - inMiddle) + ANALOG_INPUT_NEUTRAL_OFFSET;
	else
		inputRatioNeutral = (uint16_t)abs(minReleaseValue - inMiddle) + ANALOG_INPUT_NEUTRAL_OFFSET;
	inputRatioMax = (uint16_t)maxValue;
	inputRatioMin = (uint16_t)minValue;

	saveCoeff();
	updateCoeff();
	trace_printf("%d %d %d %d %d\n", inputRatioMin, inputRatioNeutral, inputRatioMax, outputRatioMin, outputRatioMax);
	timer.sleep(5000);
}

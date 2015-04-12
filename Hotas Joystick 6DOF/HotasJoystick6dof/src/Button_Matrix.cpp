#include "Button_Matrix.h"

CButtonMatrix::CButtonMatrix(_gpioxConfig* pRowMap, _gpioxConfig* pColMap) {
	memcpy(rowMap, pRowMap, sizeof(_gpioxConfig) * BUTTON_MATRIX_NB_ROWS);
	memcpy(colMap, pColMap, sizeof(_gpioxConfig) * BUTTON_MATRIX_NB_COLS);
	stateMatrix = 0;
	uint16_t i;
	for (i = 0; i < BUTTON_MATRIX_NB; i++)
		buttonStatus[i] = GPIO_PIN_RESET;

	timer.start();
}

CButtonMatrix::~CButtonMatrix() {
}

void CButtonMatrix::initialize() {
	//Set ROWS
	uint16_t i;
	for (i=0; i<BUTTON_MATRIX_NB_ROWS; i++) {
		setConfigGPIO(rowMap[i].gpiox, rowMap[i].initDef);
		HAL_GPIO_WritePin(rowMap[i].gpiox, rowMap[i].initDef.Pin, GPIO_PIN_RESET);
	}

	//Set COLS
	for (i=0; i<BUTTON_MATRIX_NB_COLS; i++) {
		setConfigGPIO(colMap[i].gpiox, colMap[i].initDef);
	}
	timer.sleep(100);

	stateMatrix = 0;
	setMatrix(stateMatrix);
}

void CButtonMatrix::updateButtonStatus() {
	uint16_t i;
	uint16_t indexButton;
	for (i=0; i<BUTTON_MATRIX_NB_COLS; i++) {
		indexButton = i + (stateMatrix * BUTTON_MATRIX_NB_COLS);
		buttonStatus[indexButton] = HAL_GPIO_ReadPin(rowMap[i].gpiox, rowMap[i].initDef.Pin);
	}
}

void CButtonMatrix::updateMatrixStatus() {
	if (stateMatrix >= BUTTON_MATRIX_NB_ROWS - 1)
		stateMatrix = 0;
	else
		stateMatrix++;

	setMatrix(stateMatrix);
}

void CButtonMatrix::setMatrix(uint16_t stateMatrix) {
	uint16_t i;
	for (i=0; i < BUTTON_MATRIX_NB_ROWS; i++) {
	    HAL_GPIO_WritePin(rowMap[i].gpiox, rowMap[i].initDef.Pin, GPIO_PIN_RESET);
	}
    HAL_GPIO_WritePin(rowMap[stateMatrix].gpiox, rowMap[stateMatrix].initDef.Pin, GPIO_PIN_SET);
}

void CButtonMatrix::getButtonStatus(GPIO_PinState* pButtonStatus) {
	updateButtonStatus();
	updateMatrixStatus();

	uint16_t i;
	for (i = 0; i < BUTTON_MATRIX_NB; i++)
		pButtonStatus[i] = buttonStatus[i];
}

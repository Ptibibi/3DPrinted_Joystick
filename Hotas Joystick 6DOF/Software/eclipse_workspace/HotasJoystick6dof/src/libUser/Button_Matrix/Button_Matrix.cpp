#include "Button_Matrix.h"

CButtonMatrix::CButtonMatrix(_gpioxConfig* pRowMap, _gpioxConfig* pColMap, GPIO_PinState* pButtonStatus) {
	buttonStatus = pButtonStatus;
	stateMatrix = 0;

	//Set ROWS
	uint16_t i;
	for (i=0; i<BUTTON_MATRIX_NB_ROWS; i++)
		rowPin[i] = new CPin(pRowMap[i]);

	//Set COLS
	for (i=0; i<BUTTON_MATRIX_NB_COLS; i++) {
		colPin[i] = new CPin(pColMap[i]);
		colPin[i]->Write(GPIO_PIN_RESET);
	}
	timer.start();
}

CButtonMatrix::~CButtonMatrix() {
}

void CButtonMatrix::initialize() {
	//Set status
	uint16_t i;
	for (i = 0; i < BUTTON_MATRIX_NB; i++)
		buttonStatus[i] = GPIO_PIN_RESET;

	timer.sleep(100);
	setMatrix(stateMatrix);
}

void CButtonMatrix::updateButtonStatus() {
	uint16_t i;
	uint16_t offset = stateMatrix * BUTTON_MATRIX_NB_ROWS;
	for (i=0; i<BUTTON_MATRIX_NB_COLS; i++) {
		buttonStatus[i + offset] = rowPin[i]->Read();
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
	for (i=0; i < BUTTON_MATRIX_NB_COLS; i++) {
		colPin[i]->Write(GPIO_PIN_RESET);
	}
	colPin[stateMatrix]->Write(GPIO_PIN_SET);
}

void CButtonMatrix::getButtonStatus() {
	updateButtonStatus();
	updateMatrixStatus();
}
